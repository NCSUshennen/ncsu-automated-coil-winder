/**
 * Task1
 * 
 * Dan Hayduk
 * April 28, 2020
 * 
 * This task contains an algorithm to read G-Code, line by line, sent by the Raspberry Pi, and send the appropriate signals
 * to the X, Y and Z motors to accomplish movement.
 * 
 * The G-Code is expected to send numerical movement values corresponding to movement distance in millimeters. The Arduino can preemptively predict 
 * if a G-Code command is about to send the winding head out of bounds and stop the winding algorithm before it runs the movement. 
 * However, these bounds need to be updated once exact measurements are taken.
 * 
 * The task also reads zeroing switches and over-position switches has been implemented in order to prevent the winding head from moving out of bounds.
 * If an over-position switch is hit, the user will have to manually turn the threaded rods to move the head back to a position away
 * from the switch. If any switch is hit unexpectedly, the Arduino will not have the correct position stored in the currentPosition
 * variable, so the winding head must be zeroed before another winding algorithm can be executed.
 * 
 * There is also functionality to read the alarm signals given by the motor drivers, and send an error to the Pi if any of the drivers indicate a problem. 
 * The algorithm also polls the IR presence sensor during movement and sends an error to the Pi if no wire is detected in the feeding system.
 * 
 * The out-of-bounds detection, overposition switches, zeroing switches, alarms, and presence detection can all be disabled via the #defines in RTOS_Main.
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
 *  (StartEncode): Enable the encoder interrupt to take measurements of wire moving around stator teeth
 *  (StopEncode): Disable the encoder interrupt to make it stop taking measurements, as wire is no longer being wound around stator teeth
 *  
 * Multiple commands can be entered in the same line, but unexpected behavior may occur if commands in the same line contradict one another
 * (Ex. "G27" and "G90" in the same line are fine since their functions are unrelated, but "X10" and "X5" will just read the first "X10" and ignore the "X5")
 */

#define PULSES_PER_MM 160
#define X_AXIS_LIMIT 1000.0
#define Y_AXIS_LIMIT 600.0
#define Z_AXIS_LIMIT 200.0

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

        enableRotaryEncoder = false;
        counter = 0;
        Distance = 0;
        
        readingGCode = true;
        char receivedChar;
        while (true)
        {
          task = 1;
          digitalWrite(TASK_1,HIGH);
          digitalWrite(TASK_2,LOW); 
          digitalWrite(TASK_3,LOW);
          digitalWrite(TASK_IDLE,LOW);
          
          Serial.print("ready\n");
          
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
          bool startEncode = (gCodeString.indexOf("Start") >= 0);
          bool stopEncode = (gCodeString.indexOf("Stop") >= 0);
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

          if (startEncode)
          {
            enableRotaryEncoder = true;
          }

          if (stopEncode)
          {
            enableRotaryEncoder = false;  
          }
          
          // Read X input if there is any
          if (xIndex >= 0)
          {
            
            int lineXIndex = gCodeString.indexOf("\n", xIndex);
            int spaceXIndex = gCodeString.indexOf(" ", xIndex);

            int endXIndex = minIndexOf2(lineXIndex, spaceXIndex);
            if (endXIndex < 0)
            {
              Serial.print(BAD_COMMAND_ERROR);
              break;
            }
            
            // endXIndex is now the index of the nearest space, linefeed, or null character
            magnitudeString = "";
            int i;
            for (i=xIndex+1; i<endXIndex; i++)
            {
              magnitudeString += gCodeString.charAt(i);
            }

            float prospectiveDestinationX = 0.0;
            
            if (isFloat(magnitudeString))
            {
              prospectiveDestinationX = magnitudeString.toFloat();
            }
            else
            {
              Serial.print(BAD_COMMAND_ERROR);
              break;
            }

#if ENABLE_OUTOFBOUNDS_DETECTION
            // Check to see if the destination would send the head out of bounds, and prevent that from happening
            if (absPositioning)
            {
              // Check if destination is in bounds
              if (prospectiveDestinationX >= 0 && prospectiveDestinationX <= X_AXIS_LIMIT)
              {
                destination[X] = prospectiveDestinationX;
              }
              else
              {
                Serial.print(DESTINATION_OUT_OF_BOUNDS_ERROR);
                break;
              }
            }
            else
            {
              // Check if destination plus current position is in bounds
              if (prospectiveDestinationX + currentPosition[X] >= 0 && prospectiveDestinationX + currentPosition[X] <= X_AXIS_LIMIT)
              {
                destination[X] = prospectiveDestinationX;
              }
              else
              {
                Serial.print(DESTINATION_OUT_OF_BOUNDS_ERROR);
                break;
              }
            } 
#else
            destination[X] = prospectiveDestinationX;
#endif
       
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
              Serial.print(BAD_COMMAND_ERROR);
              break;
            }
            
            // endYIndex is now the index of the nearest space, linefeed, or end of the string
            
            magnitudeString = "";
            int i;
            for (i=yIndex+1; i<endYIndex; i++)
            {
              magnitudeString += gCodeString.charAt(i);
            }

            float prospectiveDestinationY = 0.0;
            
            if (isFloat(magnitudeString))
            {
              prospectiveDestinationY = magnitudeString.toFloat();
            }
            else
            {
              Serial.print(BAD_COMMAND_ERROR);
              break;
            }

#if ENABLE_OUTOFBOUNDS_DETECTION
            // Check to see if the destination would send the head out of bounds, and prevent that from happening
            if (absPositioning)
            {
              // Check if destination is in bounds
              if (prospectiveDestinationY >= 0 && prospectiveDestinationY <= Y_AXIS_LIMIT)
              {
                destination[Y] = prospectiveDestinationY;
              }
              else
              {
                Serial.print(DESTINATION_OUT_OF_BOUNDS_ERROR);
                break;
              }
            }
            else
            {
              // Check if destination plus current position is in bounds
              if (prospectiveDestinationY + currentPosition[Y] >= 0 && prospectiveDestinationY + currentPosition[Y] <= Y_AXIS_LIMIT)
              {
                destination[Y] = prospectiveDestinationY;
              }
              else
              {
                Serial.print(DESTINATION_OUT_OF_BOUNDS_ERROR);
                break;
              }
            }
#else
              destination[Y] = prospectiveDestinationY;
#endif
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
              Serial.print(BAD_COMMAND_ERROR);
              break;
            }
            
            // endZIndex is now the index of the nearest space, linefeed, or end of the string
            
            magnitudeString = "";
            int i;
            for (i=zIndex+1; i<endZIndex; i++)
            {
              magnitudeString += gCodeString.charAt(i);
            }

            float prospectiveDestinationZ = 0.0;
            
            if (isFloat(magnitudeString))
            {
              prospectiveDestinationZ = magnitudeString.toFloat();
            }
            else
            {
              Serial.print(BAD_COMMAND_ERROR);
              break;
            }

#if ENABLE_OUTOFBOUNDS_DETECTION
            // Check to see if the destination would send the head out of bounds, and prevent that from happening
            if (absPositioning)
            {
              // Check if destination is in bounds
              if (prospectiveDestinationZ >= 0 && prospectiveDestinationZ <= Z_AXIS_LIMIT)
              {
                destination[Z] = prospectiveDestinationZ;
              }
              else
              {
                Serial.print(DESTINATION_OUT_OF_BOUNDS_ERROR);
                break;
              }
            }
            else
            {
              // Check if destination plus current position is in bounds
              if (prospectiveDestinationZ + currentPosition[Z] >= 0 && prospectiveDestinationZ + currentPosition[Z] <= Z_AXIS_LIMIT)
              {
                destination[Z] = prospectiveDestinationZ;
              }
              else
              {
                Serial.print(DESTINATION_OUT_OF_BOUNDS_ERROR);
                break;
              }
            }
#else
            destination[Z] = prospectiveDestinationZ;
#endif          
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
            bool x1AlarmWentOff = false;
            bool x2AlarmWentOff = false;
            bool wireIsGone = false;
            bool xZeroingSwitchOpen = true;
            bool overPositionSwitchOpen = true;
            bool x1AlarmDidntGoOff = true;
            bool x2AlarmDidntGoOff = true;
            bool wireIsStillThere = true;
            for (i=0; i<totalPulses[X]; i++)
            { 
              
              // Poll the overposition and zeroing switches
#if ENABLE_OVERPOSITION
              overPositionSwitchOpen = (digitalRead(OVER_POSITION1) == HIGH);
              if (!overPositionSwitchOpen)
              {
                 // Always stop movement if the over-position switch has been hit
                 hitOverPositionSwitch = true;
                 overPositionSwitchOpen = true;
                 break;
              }
              overPositionSwitchOpen = (digitalRead(OVER_POSITION2) == HIGH);
              if (!overPositionSwitchOpen)
              {
                 // Always stop movement if the over-position switch has been hit
                 hitOverPositionSwitch = true;
                 overPositionSwitchOpen = true;
                 break;
              }
#endif
#if ENABLE_ZEROING
              xZeroingSwitchOpen = (digitalRead(ZEROING_X) == HIGH);
              if (!xZeroingSwitchOpen && xMovementNegative)
              {
                 // Only stop moving if the zeroing switch is hit if we are trying to go in the negative X-Direction
                 hitXZeroingSwitch = true;
                 xZeroingSwitchOpen = true;
                 break;
              }
#endif

              // Poll the X1 and X2 Alarms
#if ENABLE_ALARMS
              x1AlarmDidntGoOff = (digitalRead(MOTOR_X1_ALM) == HIGH);
              if (!x1AlarmDidntGoOff)
              {
                 // Always stop movement if the alarm goes off
                 x1AlarmWentOff = true;
                 x1AlarmDidntGoOff = true;
                 break;
              }
              x2AlarmDidntGoOff = (digitalRead(MOTOR_X2_ALM) == HIGH);
              if (!x2AlarmDidntGoOff)
              {
                 // Always stop movement if the alarm goes off
                 x2AlarmWentOff = true;
                 x2AlarmDidntGoOff = true;
                 break;
              }
#endif

              // Check the IR sensor for wire presence
#if ENABLE_PRESENCE_DETECTION
              wireIsStillThere = (digitalRead(WIRE_PRESENCE) == HIGH);
              if (!wireIsStillThere)
              {
                 // Always stop movement if wire is no longer detected
                 wireIsGone = true;
                 wireIsStillThere = true;
                 break;
              }
#endif 
              // If no issues, send the pulse
              digitalWrite(MOTOR_X1_PLS, HIGH);
              digitalWrite(MOTOR_X2_PLS, HIGH);
              vTaskDelay(motionDelay/portTICK_PERIOD_MS/4);
              digitalWrite(MOTOR_X1_PLS, LOW);
              digitalWrite(MOTOR_X2_PLS, LOW);
              vTaskDelay(motionDelay/portTICK_PERIOD_MS/4);
            }

            // Throw an error if one of the enabled switches has been hit, an alarm has gone off, or the wire is missing
#if ENABLE_OVERPOSITION
            if (hitOverPositionSwitch)
            {
              Serial.print(OVER_POSITION_ERROR);
              hitOverPositionSwitch = false;
              break;
            }
#endif
#if ENABLE_ZEROING
            if (hitXZeroingSwitch)
            {
              Serial.print(ZEROING_X_ERROR);
              hitXZeroingSwitch = false;
              break;  
            }
#endif
#if ENABLE_ALARMS
            if (x1AlarmWentOff)
            {
              Serial.print(ALARM_X1_ERROR);
              x1AlarmWentOff = false;
              break;
            }
            if (x2AlarmWentOff)
            {
              Serial.print(ALARM_X2_ERROR);
              x2AlarmWentOff = false;
              break;
            }
#endif
#if ENABLE_PRESENCE_DETECTION
            if (wireIsGone)
            {
              Serial.print(WIRE_PRESENCE_ERROR);
              wireIsGone = false;
              break;
            }
#endif
          
            bool hitYZeroingSwitch = false;
            bool yAlarmWentOff = false;
            bool yZeroingSwitchOpen = true;
            bool yAlarmDidntGoOff = true;
            overPositionSwitchOpen = true;
            for (i=0; i<totalPulses[Y]; i++)
            {
              // Poll the overposition and zeroing switches
#if ENABLE_OVERPOSITION
              overPositionSwitchOpen = (digitalRead(OVER_POSITION3) == HIGH);
              if (!overPositionSwitchOpen)
              {
                 // Always stop movement if the over-position switch has been hit
                 hitOverPositionSwitch = true;
                 overPositionSwitchOpen = true;
                 break;
              }
              overPositionSwitchOpen = (digitalRead(OVER_POSITION4) == HIGH);
              if (!overPositionSwitchOpen)
              {
                 // Always stop movement if the over-position switch has been hit
                 hitOverPositionSwitch = true;
                 overPositionSwitchOpen = true;
                 break;
              }
#endif
#if ENABLE_ZEROING
              yZeroingSwitchOpen = (digitalRead(ZEROING_Y) == HIGH);
              if (!yZeroingSwitchOpen && yMovementNegative)
              {
                 // Only stop moving if the zeroing switch is hit if we are trying to go in the negative Y-Direction
                 hitYZeroingSwitch = true;
                 yZeroingSwitchOpen = true;
                 break;
              }
#endif
              // Poll the Y Alarm
#if ENABLE_ALARMS
              yAlarmDidntGoOff = (digitalRead(MOTOR_Y_ALM) == HIGH);
              if (!yAlarmDidntGoOff)
              {
                 // Always stop movement if the alarm goes off
                 yAlarmWentOff = true;
                 yAlarmDidntGoOff = true;
                 break;
              }
#endif
              // Check the IR sensor for wire presence
#if ENABLE_PRESENCE_DETECTION
              wireIsStillThere = (digitalRead(WIRE_PRESENCE) == HIGH);
              if (!wireIsStillThere)
              {
                 // Always stop movement if wire is no longer detected
                 wireIsGone = true;
                 wireIsStillThere = true;
                 break;
              }
#endif

              // If no issues, send the pulse
              digitalWrite(MOTOR_Y_PLS, HIGH);
              vTaskDelay(motionDelay/portTICK_PERIOD_MS/4);
              digitalWrite(MOTOR_Y_PLS, LOW);
              vTaskDelay(motionDelay/portTICK_PERIOD_MS/4); 
            }

            
            // Throw an error if one of the enabled switches has been hit, an alarm has gone off, or the wire is missing
#if ENABLE_OVERPOSITION
            if (hitOverPositionSwitch)
            {
              Serial.print(OVER_POSITION_ERROR);
              hitOverPositionSwitch = false;
              break;
            }
#endif
#if ENABLE_ZEROING
            if (hitYZeroingSwitch)
            {
              Serial.print(ZEROING_Y_ERROR);
              hitYZeroingSwitch = false;
              break;  
            }
#endif
#if ENABLE_ALARMS
            if (yAlarmWentOff)
            {
              Serial.print(ALARM_Y_ERROR);
              yAlarmWentOff = false;
              break;
            }
#endif
#if ENABLE_PRESENCE_DETECTION
            if (wireIsGone)
            {
              Serial.print(WIRE_PRESENCE_ERROR);
              wireIsGone = false;
              break;
            }
#endif
      
            bool hitZZeroingSwitch = false;
            bool zAlarmWentOff = false;
            bool zZeroingSwitchOpen = true;
            bool zAlarmDidntGoOff = true;
            overPositionSwitchOpen = true;
            for (i=0; i<totalPulses[Z]; i++)
            {
               // Poll the overposition and zeroing switches
#if ENABLE_OVERPOSITION
              overPositionSwitchOpen = (digitalRead(OVER_POSITION5) == HIGH);
              if (!overPositionSwitchOpen)
              {
                 // Always stop movement if the over-position switch has been hit
                 hitOverPositionSwitch = true;
                 overPositionSwitchOpen = true;
                 break;
              }
              overPositionSwitchOpen = (digitalRead(OVER_POSITION6) == HIGH);
              if (!overPositionSwitchOpen)
              {
                 // Always stop movement if the over-position switch has been hit
                 hitOverPositionSwitch = true;
                 overPositionSwitchOpen = true;
                 break;
              }
#endif
#if ENABLE_ZEROING
              zZeroingSwitchOpen = (digitalRead(ZEROING_Z) == HIGH);
              if (!zZeroingSwitchOpen && zMovementNegative)
              {
                 // Only stop moving if the zeroing switch is hit if we are trying to go in the negative Z-Direction
                 hitZZeroingSwitch = true;
                 zZeroingSwitchOpen = true;
                 break;
              }
#endif
              // Poll the Z Alarm
#if ENABLE_ALARMS
              zAlarmDidntGoOff = (digitalRead(MOTOR_Z_ALM) == HIGH);
              if (!zAlarmDidntGoOff)
              {
                 // Always stop movement if the alarm goes off
                 zAlarmWentOff = true;
                 zAlarmDidntGoOff = true;
                 break;
              }
#endif
              // Check the IR sensor for wire presence
#if ENABLE_PRESENCE_DETECTION
              wireIsStillThere = (digitalRead(WIRE_PRESENCE) == HIGH);
              if (!wireIsStillThere)
              {
                 // Always stop movement if wire is no longer detected
                 wireIsGone = true;
                 wireIsStillThere = true;
                 break;
              }
#endif

              // If no issues, send the pulse
              digitalWrite(MOTOR_Z_PLS, HIGH);
              vTaskDelay(motionDelay/portTICK_PERIOD_MS/4);
              digitalWrite(MOTOR_Z_PLS, LOW);
              vTaskDelay(motionDelay/portTICK_PERIOD_MS/4);
            }

            // Throw an error if one of the enabled switches has been hit, an alarm has gone off, or the wire is missing
#if ENABLE_OVERPOSITION
            if (hitOverPositionSwitch)
            {
              Serial.print(OVER_POSITION_ERROR);
              hitOverPositionSwitch = false;
              break;
            }
#endif
#if ENABLE_ZEROING
            if (hitZZeroingSwitch)
            {
              Serial.print(ZEROING_Z_ERROR);
              hitZZeroingSwitch = false;
              break;  
            }
#endif
#if ENABLE_ALARMS
            if (zAlarmWentOff)
            {
              Serial.print(ALARM_Z_ERROR);
              zAlarmWentOff = false;
              break;
            }
#endif
#if ENABLE_PRESENCE_DETECTION
            if (wireIsGone)
            {
              Serial.print(WIRE_PRESENCE_ERROR);
              wireIsGone = false;
              break;
            }
#endif
          
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
          }
        }
      }
    }
    readingGCode = false;
    Serial.print(Distance);
    Serial.print("\n");
  }
}
