/**
 * Task3
 * 
 * Dan Hayduk
 * November 16, 2019
 * 
 * MyTask3 is currently a demonstration of the timer peripheral. It uses timer interrupts
 * to make an LED flash three times at a rate of one flash per second.
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

      float r = 0;
      bool rFailed = false;
      float l = 0;
      bool lFailed = false;
      
      digitalWrite(TEST_SIGNAL_RANDC, LOW);
      digitalWrite(TEST_SIGNAL_L, LOW);

      // Resistance
      
      // Wait 1 second
      TCNT1 = 0;
      TIMSK1 |= (1<<OCIE1A); //enable timer interrupt A
      if (xSemaphoreTake(xSemaphoreTimer, 2000/portTICK_PERIOD_MS) == pdTRUE)
      {
        TIMSK1 |= (1<<OCIE1A); //disable timer interrupt A
        digitalWrite(TEST_SIGNAL_RANDC, HIGH);
        // Wait 1 second
        TCNT1 = 0;
        TIMSK1 |= (1<<OCIE1A); //enable timer interrupt A
        if (xSemaphoreTake(xSemaphoreTimer, 2000/portTICK_PERIOD_MS) == pdTRUE)
        {
          TIMSK1 |= (1<<OCIE1A); //disable timer interrupt A
        }
        else
        {
          Serial.write("Resistance Measurement Failed\n");
          rFailed = true;
        }

        // Read voltage measurement and convert to resistance
        r = ADCToVoltage(analogRead(OUTPUT_SIGNAL))*RIN/(3.3-ADCToVoltage(analogRead(OUTPUT_SIGNAL)));
      }
      else
      {
        Serial.write("Resistance Measurement Failed\n");
        rFailed = true;
      }

      if (!rFailed)
      {
        Serial.write("Resistance: ");
        Serial.print(r);
        Serial.write("\n");  
      }
      
      //Goal: Use timer interrupts to flash the test signal LED three times, toggling every half-second
      /*unsigned int i;
      TCNT1 = 0; //set the starting count to 0
      TIMSK1 |= (1<<OCIE1A); //enable timer interrupt A
      for (i=0; i<6; i++)
      {
        if (xSemaphoreTake(xSemaphoreTimer, 600/portTICK_PERIOD_MS) == pdTRUE) //Used to figure out of the timer reached the desired count
        {
          toggleLED(TEST_SIGNAL_RANDC); 
        }
        else
        {
          Serial.print("When i=");
          Serial.print(i);
          Serial.print(", the timer was not accessed successfully. The timer reads ");
          Serial.println(TCNT1);  
        }
      }
      TIMSK1 &= ~(1<<OCIE1A); //disable timer interrupt A*/

      
      /*digitalWrite(TEST_SIGNAL, HIGH);
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
      digitalWrite(TEST_SIGNAL, LOW);*/
    }
  }
}
