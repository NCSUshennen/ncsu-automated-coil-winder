/**
 * Task3
 * 
 * Dan Hayduk
 * November 16, 2019
 * 
 * MyTask3 is currently a demonstration of the timer peripheral. It uses timer interrupts
 * to make an LED flash three times at a rate of one flash per second.
 */

#define RIN1 5130000.0
#define RIN2 98290.0
#define RIN3 11470.0
#define RIN4 464.6
#define RIN5 150.0
#define CIN 0.00022 
#define VIN 5.0

#define ADC_LOW_THRESHOLD 3

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

      digitalWrite(TEST_SIGNAL_RANDC1, HIGH);
      digitalWrite(TEST_SIGNAL_L, LOW);

      // Resistance
      
      // Wait 1 second
      vTaskDelay(1000/portTICK_PERIOD_MS);
      double Vout = 0.0;
      float Rin = 0.0;
      
      // Read voltage measurements using different input resistances until we find one resulting in an output voltage near 2.5V
      float Vout1 = ADCToVoltage(analogRead(OUTPUT_SIGNAL));

      if (Vout1 <= 2.5)
      {
        //Lower the first test signal and raise the second one
        digitalWrite(TEST_SIGNAL_RANDC1, LOW);
        vTaskDelay(1000/portTICK_PERIOD_MS);
        digitalWrite(TEST_SIGNAL_RANDC2, HIGH);
        vTaskDelay(1000/portTICK_PERIOD_MS);

        float Vout2 = ADCToVoltage(analogRead(OUTPUT_SIGNAL));
        if (Vout2 <= 2.5)
        {
          //Lower the second test signal and raise the third one
          digitalWrite(TEST_SIGNAL_RANDC2, LOW);
          vTaskDelay(1000/portTICK_PERIOD_MS);
          digitalWrite(TEST_SIGNAL_RANDC3, HIGH);
          vTaskDelay(1000/portTICK_PERIOD_MS);

          float Vout3 = ADCToVoltage(analogRead(OUTPUT_SIGNAL));
          if (Vout3 <= 2.5)
          {
            //Lower the third test signal and raise the fourth one
            digitalWrite(TEST_SIGNAL_RANDC3, LOW);
            vTaskDelay(1000/portTICK_PERIOD_MS);
            digitalWrite(TEST_SIGNAL_RANDC4, HIGH);
            vTaskDelay(1000/portTICK_PERIOD_MS);

            float Vout4 = ADCToVoltage(analogRead(OUTPUT_SIGNAL));
            if (Vout4 <= 2.5)
            {
              //Lower the fourth test signal and raise the fifth one
              digitalWrite(TEST_SIGNAL_RANDC4, LOW);
              vTaskDelay(1000/portTICK_PERIOD_MS);
              digitalWrite(TEST_SIGNAL_RANDC5, HIGH);
              vTaskDelay(1000/portTICK_PERIOD_MS);

              float Vout5 = ADCToVoltage(analogRead(OUTPUT_SIGNAL));
              if (Vout5 <= 0.1)
              {
                //Use 1.1V instead of 5V as a reference
                analogReference(INTERNAL1V1);
                settleADC();

                int digitalVout = analogRead(OUTPUT_SIGNAL);
                double Vout6 = ADCToVoltage1V1(digitalVout);

                //Reset ADC reference to 5V
                analogReference(DEFAULT);
                settleADC();

                //Use Vout6 and RIN5
                Vout = Vout6;
                Rin = RIN5;
              }
              else
              {
                //Use either Vout5 and RIN5 or Vout4 and RIN4, depending on which Voutx is closer to 2.5 V
                if (Vout5 - 2.5 < 2.5 - Vout4)
                {
                  //Use Vout5 and RIN5
                  Vout = Vout5;
                  Rin = RIN5;
                }
                else
                {
                  //Use Vout4 and RIN4
                  Vout = Vout4;
                  Rin = RIN4;
                }
              }
            }
            else
            {
              //Use either Vout4 and RIN4 or Vout3 and RIN3, depending on which Voutx is closer to 2.5 V
              if (Vout4 - 2.5 < 2.5 - Vout3)
              {
                //Use Vout4 and RIN4
                Vout = Vout4;
                Rin = RIN4;
              }
              else
              {
                //Use Vout3 and RIN3
                Vout = Vout3;
                Rin = RIN3;
              }
            }
          }
          else
          {
            //Use either Vout3 and RIN3 or Vout2 and RIN2, depending on which Voutx is closer to 2.5 V
            if (Vout3 - 2.5 < 2.5 - Vout2)
            {
              //Use Vout3 and RIN3
              Vout = Vout3;
              Rin = RIN3;
            }
            else
            {
              //Use Vout2 and RIN2
              Vout = Vout2;
              Rin = RIN2;
            }
          }
        }
        else
        {
          //Use either Vout2 and RIN2 or Vout1 and RIN1, depending on which Voutx is closer to 2.5 V
          if (Vout2 - 2.5 < 2.5 - Vout1)
          {
            //Use Vout2 and RIN2
            Vout = Vout2;
            Rin = RIN2;
          }
          else
          {
            //Use Vout1 and RIN1
            Vout = Vout1;
            Rin = RIN1;
          }
        }
      }
      else
      {
        //Use Vout1 and RIN1
        Vout = Vout1;
        Rin = RIN1;
      }

      if (VIN - Vout > 0)
      {
        //Calculate resistance based on Vout and Rin
        r = Vout*Rin/(VIN-Vout);
      }
      else
      {
        //Prevent division by 0
        rFailed = true;
      }
      
      // Lower all test signals
      digitalWrite(TEST_SIGNAL_RANDC1, LOW);
      digitalWrite(TEST_SIGNAL_RANDC2, LOW);
      digitalWrite(TEST_SIGNAL_RANDC3, LOW);
      digitalWrite(TEST_SIGNAL_RANDC4, LOW);
      digitalWrite(TEST_SIGNAL_RANDC5, LOW);
      
      // Wait 1 second
      vTaskDelay(1000/portTICK_PERIOD_MS);
      
      // Inductance
      
      // Wait 1 second
      vTaskDelay(1000/portTICK_PERIOD_MS);
      
      digitalWrite(TEST_SIGNAL_L, HIGH);
      
      unsigned int t1 = 0;
      unsigned int t2 = 0;
      
      int sig = analogRead(OUTPUT_SIGNAL);

      //Serial.println(prevSignal);
      //Serial.println(sig);

      TCNT1 = 0;
      
      if (sig > ADC_LOW_THRESHOLD)
      {
        int numTimes = 0;
        while (sig > ADC_LOW_THRESHOLD)
        {
          sig = analogRead(OUTPUT_SIGNAL);
          if (TCNT1 > OCR1A)
          {
            //If one second has passed, abort and give failure signal
            lFailed = true;
            break;  
          }
          numTimes++;
        }
        t1 = TCNT1;
        //Serial.print("The first while loop ran ");
        //Serial.print(numTimes);
        //Serial.println(" times.");
        //Serial.print("t1 = ");
        //Serial.println(t1);
        numTimes = 0;
        while (sig <= ADC_LOW_THRESHOLD)
        {
          sig = analogRead(OUTPUT_SIGNAL);
          if (TCNT1 > OCR1A)
          {
            //If one second has passed, abort and give failure signal
            lFailed = true;
            break;  
          }
          numTimes++;
        }
        
        t2 = TCNT1;
        Serial.print("The second while loop ran ");
        Serial.print(numTimes);
        Serial.println(" times.");
        Serial.print("t2 = ");
        Serial.println(t2);
      }
      else
      {
        int numTimes = 0;
        while (sig <= ADC_LOW_THRESHOLD)
        {
          sig = analogRead(OUTPUT_SIGNAL);
          if (TCNT1 > OCR1A)
          {
            //If one second has passed, abort and give failure signal
            lFailed = true;
            break;  
          }
          numTimes++;
        }
        t1 = TCNT1;
        //Serial.print("The first while loop ran ");
        //Serial.print(numTimes);
        //Serial.println(" times.");
        //Serial.print("t1 = ");
        //Serial.println(t1);
        numTimes = 0;
        while (sig > ADC_LOW_THRESHOLD)
        {
          sig = analogRead(OUTPUT_SIGNAL);
          if (TCNT1 > OCR1A)
          {
            //If one second has passed, abort and give failure signal
            lFailed = true;
            break;  
          }
          numTimes++;
        }
        t2 = TCNT1;
        Serial.print("The second while loop ran ");
        Serial.print(numTimes);
        Serial.println(" times.");
        Serial.print("t2 = ");
        Serial.println(t2);
      }
        
      unsigned int timeBetweenPulses = t2-t1;
      //if (timeBetweenPulses <= 0)
      //{ 
        //timeBetweenPulses += (65535);
      //}
        
      double period = 2.0*timeBetweenPulses/62500.0;
      Serial.println(1.0/period);
      l = 1.0/((2.0*PI*(1.0/period))*(2.0*PI*(1.0/period))*CIN);

      
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
