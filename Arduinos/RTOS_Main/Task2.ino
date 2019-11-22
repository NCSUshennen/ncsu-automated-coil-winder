/**
 * Task2
 * 
 * Dan Hayduk
 * November 6, 2019
 * 
 * Task 2 can be used to read the data from a sensor and send it back as serial data to the Pi.
 * For now, two sensors are used as test cases, creatively named Sensor 1 and Sensor 2. A message
 * buffer is used to determine which sensor should be read, or whether both (all) should be read.
 */

static void MyTask2(void* pvParameters)
{  
  //Task 2. We display the task label on Serial monitor.
  while(1)
  { 
    uint8_t receivedSensorNum;
    if (xMessageBufferReceive(xMessageBuffer2, &receivedSensorNum, sizeof(receivedSensorNum), portMAX_DELAY) > 0)
    {
      task = 2;
      digitalWrite(TASK_1,LOW);
      digitalWrite(TASK_2,HIGH); 
      digitalWrite(TASK_3,LOW);
      digitalWrite(TASK_IDLE,LOW);

      float voltage;
      switch(receivedSensorNum)
      {
        case 0:
          //Process all sensors
          voltage = ADCToVoltage(analogRead(SENSOR_1));
          Serial.println(voltage);
          voltage = ADCToVoltage(analogRead(SENSOR_2));
          Serial.println(voltage);
          break;
        case 1:
          //Sensor 1 process
          voltage = ADCToVoltage(analogRead(SENSOR_1));
          Serial.println(voltage);
          break;
        case 2:
          //Sensor 2 process
          voltage = ADCToVoltage(analogRead(SENSOR_2));
          Serial.println(voltage);
          break;
        default:
          Serial.println("Error: invalid sensor chosen\n");
          break; 
      }
      //Serial.write("ready\n");
    }
  }
}
