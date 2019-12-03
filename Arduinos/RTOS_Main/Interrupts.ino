/**
 * Interrupts
 * 
 * Dan Hayduk
 * November 2, 2019
 * 
 * This code handles all interrupt and interrupt-like functionality. In the end,
 * this should only entail timer interrupts, a reaction to serial input, 
 * and possibly some abort functionality for safety.
 */

ISR(TIMER1_COMPA_vect)
{
  /**
   * This interrupt goes off every half second
   */
   xSemaphoreGiveFromISR(xSemaphoreTimer, NULL);
}

ISR(TIMER1_COMPB_vect)
{
  /**
   * This interrupt goes off every quarter second (currently not used)
   */
}

void serialEvent() 
{
  /**
  * This function runs whenever there is new Serial data to read. It then reads it and stores it in inputString.
  * When a new line is detected, indicating the end of a command, this function checks to see if the string in inputString
  * corresponds to one of the commands. If so, it carries out the corresponding action. After reading the full command,
  * inputString is cleared so that new commands can be processed.
  * 
  * Returns: nothing
  */

  while (Serial.available()) 
  {
    
    if (readingGCode)
    {
      // Every character gets written to the G-Code message buffer
      inChar = (char)Serial.read();
      xMessageBufferSend(xMessageBufferGCode, &inChar, sizeof(inChar), 0);
    }
    else
    {
      //Store input value in the string inputString.
      //get the new byte:
      inChar = (char)Serial.read();
      // add it to the inputString:
      if (inChar == '\n')
      {
        //End of the command, check to see if it corresponds to one of our chosen commands      
        if (inputString.equals(SERIAL_TEST))
        {
          // Pretend to be General Grievous and print "General Kenobi!"
          // This is used as a sanity check to ensure serial data is read properly
          Serial.write("General Kenobi!\n");
        }
        else if (inputString.equals(READY_ASK))
        {
          // Used for handshaking between the Arduino and the Pi
          askedForReady = true;
        }
        else if(inputString.equals(TASK_1_COMMAND_SIMPLEX))
        {
          manualTurnDirection = 0;
          // Send a message to Task Manual Turn F
          xMessageBufferSend(xMessageBufferManualTurnDirection, &manualTurnDirection, sizeof(manualTurnDirection), 0);
        }
        else if(inputString.equals(TASK_1_COMMAND_SIMPLEY))
        {
          manualTurnDirection = 1;
          // Send a message to Task Manual Turn F
          xMessageBufferSend(xMessageBufferManualTurnDirection, &manualTurnDirection, sizeof(manualTurnDirection), 0);
        }
        else if(inputString.equals(TASK_1_COMMAND_SIMPLEZ))
        {
          manualTurnDirection = 2;
          // Send a message to Task Manual Turn F
          xMessageBufferSend(xMessageBufferManualTurnDirection, &manualTurnDirection, sizeof(manualTurnDirection), 0);
        }
        else if(inputString.equals(TASK_1_COMMAND_SIMPLEXR))
        {
          manualTurnDirection = 3;
          // Send a message to Task Manual Turn F
          xMessageBufferSend(xMessageBufferManualTurnDirection, &manualTurnDirection, sizeof(manualTurnDirection), 0);
        }
        else if(inputString.equals(TASK_1_COMMAND_SIMPLEYR))
        {
          manualTurnDirection = 4;
          // Send a message to Task Manual Turn F
          xMessageBufferSend(xMessageBufferManualTurnDirection, &manualTurnDirection, sizeof(manualTurnDirection), 0);
        }
        else if(inputString.equals(TASK_1_COMMAND_SIMPLEZR))
        {
          manualTurnDirection = 5;
          // Send a message to Task Manual Turn F
          xMessageBufferSend(xMessageBufferManualTurnDirection, &manualTurnDirection, sizeof(manualTurnDirection), 0);
        }
        else if(inputString.equals(TASK_1_COMMAND))
        {
          // Give a semaphore to Task 1
          xSemaphoreGive(xSemaphore1);
        }
        else if(inputString.equals(TASK_1_BEGIN_G_CODE))
        {
          // Give a semaphore to begin G Code
          xSemaphoreGive(xSemaphorePercent);
        }
        else if (inputString.equals(TASK_2_COMMAND_ALL))
        {
          sensorNum = 0;
          //Give a message to Task 2
          xMessageBufferSend(xMessageBuffer2, &sensorNum, sizeof(sensorNum), 0);
          //Don't worry for now if the message buffer is full, just ignore this command
        }
        else if (inputString.equals(TASK_2_COMMAND_SENSOR1))
        {
          sensorNum = 1;
          //Give a message to Task 2
          xMessageBufferSend(xMessageBuffer2, &sensorNum, sizeof(sensorNum), 0);
          //Don't worry for now if the message buffer is full, just ignore this command
        }
        else if (inputString.equals(TASK_2_COMMAND_SENSOR2))
        {
          sensorNum = 2;
          //Give a message to Task 2
          xMessageBufferSend(xMessageBuffer2, &sensorNum, sizeof(sensorNum), 0);
          //Don't worry for now if the message buffer is full, just ignore this command
        }
        else if (inputString.equals(TASK_3_COMMAND))
        {
          // Give a semaphore to Task 3
          xSemaphoreGive(xSemaphore3);
        }
        inputString = "";
      }
      else
      {
        inputString += inChar;
      }
    }
  }
}

void xMotorISR()
{
  /**
   * Pulse received on the X Motor, send a message to the Motor Simulator Task
   */

  if (digitalRead(MOTOR_X1_DIR))
  {
    motorMessage = X_REVERSE;
  }
  else
  {
    motorMessage = X_FORWARD;
  }

  xMessageBufferSendFromISR(xMessageBufferM, &motorMessage, sizeof(motorMessage), NULL);
}

void yMotorISR()
{
  /**
   * Pulse received on the Y Motor, send a message to the Motor Simulator Task
   */

  if (digitalRead(MOTOR_Y_DIR))
  {
    motorMessage = Y_REVERSE;
  }
  else
  {
    motorMessage = Y_FORWARD;
  }

  xMessageBufferSendFromISR(xMessageBufferM, &motorMessage, sizeof(motorMessage), NULL);
}

void zMotorISR()
{
  /**
   * Pulse received on the Z Motor, send a message to the Motor Simulator Task
   */

  if (digitalRead(MOTOR_Z_DIR))
  {
    motorMessage = Z_REVERSE;
  }
  else
  {
    motorMessage = Z_FORWARD;
  }

  xMessageBufferSendFromISR(xMessageBufferM, &motorMessage, sizeof(motorMessage), NULL);
}
