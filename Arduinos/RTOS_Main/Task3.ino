/**
 * Task3
 * 
 * Dan Hayduk
 * November 16, 2019
 * 
 * MyTask3 is currently a demonstration of the timer peripheral. It uses timer interrupts
 * to make an LED flash three times at a rate of one flash per second.
 */

#define RIN 1000.0
#define CIN 0.00022 
#define VIN 5.0

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

      digitalWrite(TEST_SIGNAL_RANDC, HIGH);
      digitalWrite(TEST_SIGNAL_L, LOW);

      // Resistance
      
      // Wait 1 second
      vTaskDelay(1000/portTICK_PERIOD_MS);
      
      // Read voltage measurement and convert to resistance
      float Vout = ADCToVoltage(analogRead(OUTPUT_SIGNAL));
 
      Serial.println(Vout);
      
      if (VIN - Vout > 0)
      {
        r = Vout*RIN/(VIN-Vout);
      }
      else
      {
        rFailed = true;
      }

      digitalWrite(TEST_SIGNAL_RANDC, LOW);

      // Wait 1 second
      vTaskDelay(1000/portTICK_PERIOD_MS);

      /*// Resistance
      
      // Wait 1 second
      TCNT1 = 0;
      TIMSK1 |= (1<<OCIE1A); //enable timer interrupt A
      if (xSemaphoreTake(xSemaphoreTimerA, 2000/portTICK_PERIOD_MS) == pdTRUE)
      {
        TIMSK1 &= ~(1<<OCIE1A); //disable timer interrupt A

        // Read voltage measurement and convert to resistance
        float Vout = ADCToVoltage(analogRead(OUTPUT_SIGNAL));
        //Serial.println(Vout);

        if (VIN - Vout > 0)
        {
          r = Vout*RIN/(VIN-Vout);
        }
        else
        {
          rFailed = true;
        }

        digitalWrite(TEST_SIGNAL_RANDC, LOW);

        // Wait 1 second
        TCNT1 = 0;
        TIMSK1 |= (1<<OCIE1A); //enable timer interrupt A
        if (xSemaphoreTake(xSemaphoreTimerA, 2000/portTICK_PERIOD_MS) == pdTRUE)
        {
          TIMSK1 &= ~(1<<OCIE1A); //disable timer interrupt A
        }
        else
        {
          rFailed = true;
        }
      }
      else
      {
        rFailed = true;
      }*/

      /*// Inductance

      // Wait 1 second
      TCNT1 = 0;
      TIMSK1 |= (1<<OCIE1A); //enable timer interrupt A
      if (xSemaphoreTake(xSemaphoreTimerA, 2000/portTICK_PERIOD_MS) == pdTRUE)
      {
        TIMSK1 &= ~(1<<OCIE1A); //disable timer interrupt A
        digitalWrite(TEST_SIGNAL, HIGH);

        int t1 = 0;
        int t2 = 0;

        int prevSignal = analogRead(OUTPUT_SIGNAL);
        int sig = analogRead(OUTPUT_SIGNAL);
        
        if (sig-prevSignal > 0)
        {
          while (sig-prevSignal >= 0)
          {
            prevSignal = sig;
            sig = analogRead(OUTPUT_SIGNAL);
            if (TCNT1 > OCR1A)
            {
              //If one second has passed, abort and give failure signal
              lFailed = true;
              break;  
            }
          }
          t1 = TCNT1;
          Serial.print("t1 = ");
          Serial.println(t1);
          while (sig-prevSignal <= 0)
          {
            prevSignal = sig;
            sig = analogRead(OUTPUT_SIGNAL);
            if (TCNT1 > OCR1A)
            {
              //If one second has passed, abort and give failure signal
              lFailed = true;
              break;  
            }
          }
          t2 = TCNT1;
          Serial.print("t2 = ");
          Serial.println(t2);
        }
        else
        {
          while (sig-prevSignal <= 0)
          {
            prevSignal = sig;
            sig = analogRead(OUTPUT_SIGNAL);
            if (TCNT1 > OCR1A)
            {
              //If one second has passed, abort and give failure signal
              lFailed = true;
              break;  
            }
          }
          t1 = TCNT1;
          Serial.print("t1 = ");
          Serial.println(t1);
          while (sig-prevSignal >= 0)
          {
            prevSignal = sig;
            sig = analogRead(OUTPUT_SIGNAL);
            if (TCNT1 > OCR1A)
            {
              //If one second has passed, abort and give failure signal
              lFailed = true;
              break;  
            }
          }
          t2 = TCNT1;
          Serial.print("t2 = ");
          Serial.println(t2);
        }
        
        int timeBetweenPulses = t2-t1;
        if (timeBetweenPulses <= 0)
        {
          timeBetweenPulses += (65535);
        }

        Serial.println(timeBetweenPulses);

        float period = 2.0*timeBetweenPulses/62500.0;
        l = 1.0/((2.0*PI*(1.0/period))*(2.0*PI*(1.0/period))*CIN);
      }
      else
      {
        lFailed = true;
      }*/

      if (!rFailed)
      {
        Serial.print("Resistance: ");
        Serial.print(r);
        Serial.print("\n");  
      }
      else
      {
        Serial.print("Resistance Measurement Failed");
        Serial.print("\n"); 
      }

      if (!lFailed)
      {
        float lMilliHenrys = l*1000.0;
        Serial.print("Inductance: ");
        Serial.print(lMilliHenrys);
        Serial.print(" mH\n");  
      }
      else
      {
        Serial.print("Inductance Measurement Failed");
        Serial.print("\n"); 
      }

      digitalWrite(TEST_SIGNAL_RANDC, LOW);
      digitalWrite(TEST_SIGNAL_L, LOW);

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
