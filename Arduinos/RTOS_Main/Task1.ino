/**
 * Task1
 * 
 * Dan Hayduk
 * January 30, 2020
 * 
 * This task contains an algorithm to read G-Code, line by line, sent by the Raspberry Pi, and send the appropriate signals
 * to the X, Y and Z motors to accomplish movement.
 * 
 * The G-Code is expected to send numerical movement values corresponding to movement distance in millimeters. The ability to read
 * zeroing switches and over-position switches has been implemented in order to prevent the winding head from moving out of bounds.
 * If an over-position switch is hit, the user will have to manually turn the threaded rods to move the head back to a position away
 * from the switch. If any switch is hit unexpectedly, the Arduino will not have the correct position stored in the currentPosition
 * variable, so the winding head must be zeroed before another winding algorithm can be executed.
 * 
 * There are currently plans to implement a feature to preemptively predict if a G-Code command is about to send the winding head out of
 * bounds and stop the winding algorithm before it runs the movement. However, this has not yet been implemented.
 * 
 * The following G-Code commands can be entered:
 *  G0 or G00: Set rapid-motion mode off (off by default)
 *  G1 or G01: Set rapid-motion mode on
 *  G27: Indicates that the current position is position (0, 0, 0)
 *  G28: Tells the head to move directly to the position indicated by the last G27 command, or where the head started at the beginning of the winding
 *       path if there was no G27 command
 *  G90: Set to absolute positioning mode (default)
 *  G91: Set to relative/incremental positioning mode
 *  X#: Move in the X-direction to this location (if absolute positioning) or this distance in the X-direction (if relative/incremental positioning)
 *  Y#: Move in the Y-direction to this location (if absolute positioning) or this distance in the Y-direction (if relative/incremental positioning)
 *  Z#: Move in the Z-direction to this location (if absolute positioning) or this distance in the Z-direction (if relative/incremental positioning)
 *  
 * Multiple commands can be entered in the same line, but unexpected behavior may occur if commands in the same line contradict one another
 * (Ex. "G27" and "G90" in the same line are fine since their functions are unrelated, but "X10" and "X5" will just read the first "X10" and ignore the "X5")
 */

#define PULSES_PER_MM 160

float currentPosition[3] = {0, 0, 0};

static void MyTask1(void* pvParameters)
{
  //int currentPosition[3] = {0, 0, 0};
  while(1)
  { 
    if (xSemaphoreTake(xSemaphore1, portMAX_DELAY) == pdTRUE)
    {
      task = 1;
      digitalWrite(TASK_1,HIGH);
      digitalWrite(TASK_2,LOW); 
      digitalWrite(TASK_3,LOW);
      digitalWrite(TASK_IDLE,LOW);

      int badCommands = 0;
      bool rapidMotion = false;
      int motionDelay = 5;
      float destination[3] = {0, 0, 0};
      bool absPositioning = true;
      bool firstTimeG28 = true;

      Serial.print("ready\n");
      
      // Await 1st %
      if (xSemaphoreTake(xSemaphorePercent, portMAX_DELAY) == pdTRUE)
      {
        task = 1;
        digitalWrite(TASK_1,HIGH);
        digitalWrite(TASK_2,LOW); 
        digitalWrite(TASK_3,LOW);
        digitalWrite(TASK_IDLE,LOW);

        readingGCode = true;
        char receivedChar;
        while (true)
        {
          Serial.print("ready\n");
          
          // Keep reading G-Code lines until we get % or too many bad commands
          if (badCommands >= 3)
          {
            Serial.print("ErrorTooManyBadCommands\n");
            break; 
          }
          
          gCodeString = "";
          do
          {
            // Read characters and put them in gCodeString until we get '\n' or '\0'
            if (xMessageBufferReceive(xMessageBufferGCode, &receivedChar, sizeof(receivedChar), portMAX_DELAY) > 0)
            {
              gCodeString += receivedChar;
            }
          } while (receivedChar != '\n' && receivedChar != '\0');
          
          // Check for % or commands
          if (gCodeString.equals("%\n"))
          {
            break;
          }

          // Note: for some reason, I can't do logical operations directly involving the indexOf function,
          // but storing them in boolean variables then performing the logical operations seems to work better

          bool g0 = (gCodeString.indexOf("G0") >= 0);
          bool notG01 = (gCodeString.indexOf("G01") < 0);
          bool g1 = (gCodeString.indexOf("G1") >= 0);
          bool g01 = (gCodeString.indexOf("G01") >= 0);
          bool g27 = (gCodeString.indexOf("G27") >= 0);
          bool g28 = (gCodeString.indexOf("G28") >= 0);
          bool g90 = (gCodeString.indexOf("G90") >= 0);
          bool g91 = (gCodeString.indexOf("G91") >= 0);
          int xIndex = gCodeString.indexOf("X");
          int yIndex = gCodeString.indexOf("Y");
          int zIndex = gCodeString.indexOf("Z");
          
          if (g0 && notG01)
          {
            rapidMotion = true;
            motionDelay = 1;
          }
          else if (g1 || g01)
          {
            rapidMotion = false;
            motionDelay = 5;
          }

          if (g27)
          {
            currentPosition[X] = 0;
            currentPosition[Y] = 0;
            currentPosition[Z] = 0;
          }
          
          if (g28)
          {
            if (absPositioning)
            {
              destination[X] = 0;
              destination[Y] = 0;
              destination[Z] = 0;
            }
            else
            {
              destination[X] = -currentPosition[X];
              destination[Y] = -currentPosition[Y];
              destination[Z] = -currentPosition[Z];
            }
          }

          if (g90)
          {
            absPositioning = true;
          }
          else if (g91)
          {
            absPositioning = false;
          }

          // Read X input if there is any
          //unsigned int additionalPulses[3] = {0, 0, 0};
          if (xIndex >= 0)
          {
            
            int lineXIndex = gCodeString.indexOf("\n", xIndex);
            int spaceXIndex = gCodeString.indexOf(" ", xIndex);

            int endXIndex = minIndexOf2(lineXIndex, spaceXIndex);
            if (endXIndex < 0)
            {
              badCommands++;
              continue;
            }
            
            // endXIndex is now the index of the nearest space, linefeed, or null character
            magnitudeString = "";
            int i;
            for (i=xIndex+1; i<endXIndex; i++)
            {
              magnitudeString += gCodeString.charAt(i);
            }
            
            if (isFloat(magnitudeString))
            {
              destination[X] = magnitudeString.toFloat();
            }
            else
            {
              badCommands++;
              continue;
            }
          }
          else if (!g28)
          {
            // Do not move in the X direction
            if (absPositioning)
            {
              destination[X] = currentPosition[X];  
            }
            else
            {
              destination[X] = 0;
            }  
          }

          // Read Y input if there is any
          if (yIndex >= 0)
          {
            
            int lineYIndex = gCodeString.indexOf("\n", yIndex);
            int spaceYIndex = gCodeString.indexOf(" ", yIndex);

            int endYIndex = minIndexOf2(lineYIndex, spaceYIndex);

            if (endYIndex < 0)
            {
              badCommands++;
              continue;
            }
            
            // endYIndex is now the index of the nearest space, linefeed, or end of the string
            
            magnitudeString = "";
            int i;
            for (i=yIndex+1; i<endYIndex; i++)
            {
              magnitudeString += gCodeString.charAt(i);
            }
            
            if (isFloat(magnitudeString))
            {
              destination[Y] = magnitudeString.toFloat();
            }
            else
            {
              badCommands++;
              continue;
            }
          }
          else if (!g28)
          {
            // Do not move in the Y direction
            if (absPositioning)
            {
              destination[Y] = currentPosition[Y];  
            }
            else
            {
              destination[Y] = 0;
            }  
          }

          // Read Z input if there is any
          if (zIndex >= 0)
          {
            
            int lineZIndex = gCodeString.indexOf("\n", zIndex);
            int spaceZIndex = gCodeString.indexOf(" ", zIndex);

            int endZIndex = minIndexOf2(lineZIndex, spaceZIndex);

            if (endZIndex < 0)
            {
              badCommands++;
              continue;
            }
            
            // endZIndex is now the index of the nearest space, linefeed, or end of the string
            
            magnitudeString = "";
            int i;
            for (i=zIndex+1; i<endZIndex; i++)
            {
              magnitudeString += gCodeString.charAt(i);
            }
            
            if (isFloat(magnitudeString))
            {
              destination[Z] = magnitudeString.toInt();
            }
            else
            {
              badCommands++;
              continue;
            }
          }
          else if (!g28)
          {
            // Do not move in the Z direction
            if (absPositioning)
            {
              destination[Z] = currentPosition[Z];  
            }
            else
            {
              destination[Z] = 0;
            }  
          }

          // Movement
          if ((absPositioning && destination[X]==currentPosition[X] && destination[Y]==currentPosition[Y] && destination[Z]==currentPosition[Z])
          || (!absPositioning && destination[X]==0 && destination[Y]==0 && destination[Z]==0))
          {
            //Don't move
            badCommands = 0;
          }
          else
          {
            // Movement in at least one direction needs to occur
            unsigned long int totalPulses[3] = {0, 0, 0};
            bool xMovementNegative = false;
            bool yMovementNegative = false;
            bool zMovementNegative = false;
            
            // Set direction and calculate totalPulses
            if (absPositioning)
            {
              // Set direction
              if (destination[X] < currentPosition[X])
              {
                digitalWrite(MOTOR_X1_DIR, HIGH);
                digitalWrite(MOTOR_X2_DIR, HIGH);
                xMovementNegative = true;
              }
              else
              {
                digitalWrite(MOTOR_X1_DIR, LOW);
                digitalWrite(MOTOR_X2_DIR, LOW);  
              }
              if (destination[Y] < currentPosition[Y])
              {
                digitalWrite(MOTOR_Y_DIR, HIGH);
                yMovementNegative = true;
              }
              else
              {
                digitalWrite(MOTOR_Y_DIR, LOW);  
              }
              if (destination[Z] > currentPosition[Z])
              {
                digitalWrite(MOTOR_Z_DIR, HIGH);
              }
              else
              {
                digitalWrite(MOTOR_Z_DIR, LOW);
                zMovementNegative = true;
              }

              // Calculate totalPulses
              float distanceToCover = abs(destination[X]-currentPosition[X]);
              totalPulses[X] = distanceToCover * PULSES_PER_MM;
              distanceToCover = abs(destination[Y]-currentPosition[Y]);
              totalPulses[Y] = distanceToCover * PULSES_PER_MM;
              distanceToCover = abs(destination[Z]-currentPosition[Z]);
              totalPulses[Z] = distanceToCover * PULSES_PER_MM;
            }
            else
            {
              // Set direction
              if (destination[X] < 0)
              {
                digitalWrite(MOTOR_X1_DIR, HIGH);
                digitalWrite(MOTOR_X2_DIR, HIGH);
                xMovementNegative = true;
              }
              else
              {
                digitalWrite(MOTOR_X1_DIR, LOW);
                digitalWrite(MOTOR_X2_DIR, LOW);  
              }
              if (destination[Y] < 0)
              {
                digitalWrite(MOTOR_Y_DIR, HIGH);
                yMovementNegative = true;
              }
              else
              {
                digitalWrite(MOTOR_Y_DIR, LOW);  
              }
              if (destination[Z] > 0)
              {
                digitalWrite(MOTOR_Z_DIR, HIGH);
              }
              else
              {
                digitalWrite(MOTOR_Z_DIR, LOW);
                zMovementNegative = true;
              }

              // Calculate totalPulses
              totalPulses[X] = abs(destination[X])*PULSES_PER_MM;
              totalPulses[Y] = abs(destination[Y])*PULSES_PER_MM;
              totalPulses[Z] = abs(destination[Z])*PULSES_PER_MM;
            }

            // Send those pulses
            unsigned long int i;
            bool hitXZeroingSwitch = false;
            bool hitOverPositionSwitch = false;
            for (i=0; i<totalPulses[X]; i++)
            {
               digitalWrite(MOTOR_X1_PLS, HIGH);
               digitalWrite(MOTOR_X2_PLS, HIGH);
               vTaskDelay(motionDelay/portTICK_PERIOD_MS/4);
               digitalWrite(MOTOR_X1_PLS, LOW);
               digitalWrite(MOTOR_X2_PLS, LOW);
               vTaskDelay(motionDelay/portTICK_PERIOD_MS/4);
//#if USE_MOTOR_SIMULATOR
//#else
               // Check to see if a contact switch has been hit and react accordingly
               if (digitalRead(ZEROING_X) != HIGH && xMovementNegative)
               {
                  // Only stop moving if the zeroing switch is hit if we are trying to go in the negative X-Direction
                  hitXZeroingSwitch = true;
                  break;
               }
               else if (digitalRead(OVER_POSITION) != HIGH)
               {
                  // Always stop movement if the over-position switch has been hit
                  hitOverPositionSwitch = true;
                  break;
               }
//#endif
            }

//#if USE_MOTOR_SIMULATOR
//#else
            if (hitXZeroingSwitch)
            {
              Serial.print(ZEROING_X_ERROR);
              break;  
            }
            else if (hitOverPositionSwitch)
            {
              Serial.print(OVER_POSITION_ERROR);
              break;
            }
//#endif
            bool hitYZeroingSwitch = false;
            for (i=0; i<totalPulses[Y]; i++)
            {
               digitalWrite(MOTOR_Y_PLS, HIGH);
               vTaskDelay(motionDelay/portTICK_PERIOD_MS/4);
               digitalWrite(MOTOR_Y_PLS, LOW);
               vTaskDelay(motionDelay/portTICK_PERIOD_MS/4);

//#if USE_MOTOR_SIMULATOR
//#else
               // Check to see if a contact switch has been hit and react accordingly
               if (digitalRead(ZEROING_Y) != HIGH && yMovementNegative)
               {
                  // Only stop moving if the zeroing switch is hit if we are trying to go in the negative Y-Direction
                  hitYZeroingSwitch = true;
                  break;
               }
               else if (digitalRead(OVER_POSITION) != HIGH)
               {
                  // Always stop movement if the over-position switch has been hit
                  hitOverPositionSwitch = true;
                  break;
               }
//#endif
            }

//#if USE_MOTOR_SIMULATOR
//#else
            if (hitYZeroingSwitch)
            {
              Serial.print(ZEROING_Y_ERROR);
              break;  
            }
            else if (hitOverPositionSwitch)
            {
              Serial.print(OVER_POSITION_ERROR);
              break;
            }
//#endif
            
            bool hitZZeroingSwitch = false;
            for (i=0; i<totalPulses[Z]; i++)
            {
               digitalWrite(MOTOR_Z_PLS, HIGH);
               vTaskDelay(motionDelay/portTICK_PERIOD_MS/4);
               digitalWrite(MOTOR_Z_PLS, LOW);
               vTaskDelay(motionDelay/portTICK_PERIOD_MS/4);

//#if USE_MOTOR_SIMULATOR
//#else
               // Check to see if a contact switch has been hit and react accordingly
               if (digitalRead(ZEROING_Z) != HIGH && zMovementNegative)
               {
                  // Only stop moving if the zeroing switch is hit if we are trying to go in the negative Z-Direction
                  hitZZeroingSwitch = true;
                  break;
               }
               else if (digitalRead(OVER_POSITION) != HIGH)
               {
                  // Always stop movement if the over-position switch has been hit
                  hitOverPositionSwitch = true;
                  break;
               }
//#endif
            }
            
//#if USE_MOTOR_SIMULATOR
//#else
            if (hitZZeroingSwitch)
            {
              Serial.print(ZEROING_Z_ERROR);
              break;  
            }
            else if (hitOverPositionSwitch)
            {
              Serial.print(OVER_POSITION_ERROR);
              break;
            }
//#endif

            // Set all direction values low
            digitalWrite(MOTOR_X1_DIR, LOW);
            digitalWrite(MOTOR_X2_DIR, LOW);
            digitalWrite(MOTOR_Y_DIR, LOW);
            digitalWrite(MOTOR_Z_DIR, LOW);

            // Update currentPosition
            if (absPositioning)
            {
              currentPosition[X] = destination[X];
              currentPosition[Y] = destination[Y];
              currentPosition[Z] = destination[Z];
            }
            else
            {
              currentPosition[X] += destination[X];
              currentPosition[Y] += destination[Y];
              currentPosition[Z] += destination[Z];
            }

            /*// Clear additionalPulses
            additionalPulses[X] = 0;
            additionalPulses[Y] = 0;
            additionalPulses[Z] = 0;*/
            
            badCommands = 0;
          }
        }
      }
    }
    readingGCode = false;
  }
}
