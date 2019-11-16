/**
 * Task3
 * 
 * Dan Hayduk
 * November 2, 2019
 * 
 * Some functionality for MyTask3 has been established. The command to initiate the task is currently
 * driven by rising-edge input to pin 21. An interrupt (currently subject to debounce issues) is used 
 * to tell the idle task to give MyTask3 a semaphore, then the idle task gives the semaphore whenever it can. 
 * Using the idle task for this allows the ISR to terminate quickly and avoid interfering with the rest of the program. 
 * This will soon be upgraded to be driven by a serial command.
 * 
 * MyTask3 currently successfully raises a 5-Volt input test signal and records an array of values when measuring
 * a signal for one second. However, since it currently uses the delay() function, the data captured does not accurately
 * capture a one-second period. This will be problematic when trying to use an output signal to determine a transfer
 * function, so current future work involves investigating the use of a timer peripheral to take more precisely-timed
 * measurements.
 */

static void MyTask3(void* pvParameters)
{ 
  while(1)
  { 
    if (xSemaphoreTake(xSemaphore3, portMAX_DELAY) == pdTRUE)
    {
      task = 3;
      digitalWrite(TASK_1,LOW);
      digitalWrite(TASK_2,LOW); 
      digitalWrite(TASK_3,HIGH);
      digitalWrite(TASK_IDLE,LOW);

      digitalWrite(TEST_SIGNAL, HIGH);
      int i;
      int voltagesSize = 1000;
      for (i=0; i<voltagesSize; i++)
      {
        int voltage = analogRead(OUTPUT_SIGNAL);
        voltages[i] = voltage;
        delay(1);
      }
      
      for (i=0; i<voltagesSize; i++)
      {
        Serial.println(ADCToVoltage(voltages[i]));  
      }
      digitalWrite(TEST_SIGNAL, LOW);
      //Serial.write("ready\n");
    }
  }
}
