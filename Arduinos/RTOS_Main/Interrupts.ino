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

  digitalWrite(DEBUG_LED, HIGH);
  while (Serial.available()) 
  {
    //Store input value in the string inputString.
    
    //get the new byte:
    char inChar = (char)Serial.read();
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
        askedForReady = true;
      }
      else if(inputString.equals(TASK_1_COMMAND_SIMPLE))
      {
        // Give a semaphore to Task 1
        xSemaphoreGive(xSemaphore1);
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
      else if (inputString.equals("0"))
      {
        digitalWrite(MOTOR_1_PLS, LOW);  
      }
      else if (inputString.equals("1"))
      {
        digitalWrite(MOTOR_1_PLS, HIGH);  
      }
      inputString = "";
    }
    else
    {
      inputString += inChar;
    }
  }
  digitalWrite(DEBUG_LED, LOW);
}
