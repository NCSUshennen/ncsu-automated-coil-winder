/**
 * Task3
 * 
 * Dan Hayduk
 * April 28, 2020
 * 
 * MyTask3 runs the post-winding test. It first runs tests on four reference resistors (RIN1-RIN4) and performs voltage division
 * to determine the resistance of the coil. It then creates two LC circuits with two separate parallel capacitors and compares their
 * oscillation frequencies to determine a rough estimate of their inductance and capacitance. Using relays for this as specified in
 * the hardware specs presents some challenges, but the estimate works for relative comparisons of coils and inductors.
 */

#define RIN1 98290.0
#define RIN2 11470.0
#define RIN3 464.6
#define RIN4 150.0
#define CIN1 0.0000001
#define CIN2 0.00000333
#define VIN 5.0

#define ADC_LOW_THRESHOLD 10
#define ADC_READINGS_SIZE 500

unsigned int adcReadings[ADC_READINGS_SIZE] = {0};
unsigned int adcTimes[ADC_READINGS_SIZE] = {0};

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
      float c = 0;

      digitalWrite(TEST_SIGNAL_R1, HIGH);
      digitalWrite(TEST_SIGNAL_COIL, HIGH);

      //
      // Resistance
      //
      
      // Wait 1 second
      vTaskDelay(1000/portTICK_PERIOD_MS);
      double Vout = 0.0;
      float Rin = 0.0;
      
      // Read voltage measurements using different input resistances until we find one resulting in an output voltage near 2.5V
      float Vout1 = ADCToVoltage(analogRead(OUTPUT_SIGNAL));

      if (Vout1 <= 2.5)
      {
        //Lower the first test signal and raise the second one
        digitalWrite(TEST_SIGNAL_R1, LOW);
        vTaskDelay(1000/portTICK_PERIOD_MS);
        digitalWrite(TEST_SIGNAL_R2, HIGH);
        vTaskDelay(1000/portTICK_PERIOD_MS);

        float Vout2 = ADCToVoltage(analogRead(OUTPUT_SIGNAL));
        if (Vout2 <= 2.5)
        {
          //Lower the second test signal and raise the third one
          digitalWrite(TEST_SIGNAL_R2, LOW);
          vTaskDelay(1000/portTICK_PERIOD_MS);
          digitalWrite(TEST_SIGNAL_R3, HIGH);
          vTaskDelay(1000/portTICK_PERIOD_MS);

          float Vout3 = ADCToVoltage(analogRead(OUTPUT_SIGNAL));
          if (Vout3 <= 2.5)
          {
            //Lower the third test signal and raise the fourth one
            digitalWrite(TEST_SIGNAL_R3, LOW);
            vTaskDelay(1000/portTICK_PERIOD_MS);
            digitalWrite(TEST_SIGNAL_R4, HIGH);
            vTaskDelay(1000/portTICK_PERIOD_MS);

            float Vout4 = ADCToVoltage(analogRead(OUTPUT_SIGNAL));
            if (Vout4 <= 0.1)
            {
              //Use 1.1V instead of 5V as a reference
              analogReference(INTERNAL1V1);
              settleADC();

              int digitalVout = analogRead(OUTPUT_SIGNAL);
              double Vout5 = ADCToVoltage1V1(digitalVout);

              //Reset ADC reference to 5V
              analogReference(DEFAULT);
              settleADC();

              //Use Vout5 and RIN4
              Vout = Vout5;
              Rin = RIN4;
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
      digitalWrite(TEST_SIGNAL_R1, LOW);
      digitalWrite(TEST_SIGNAL_R2, LOW);
      digitalWrite(TEST_SIGNAL_R3, LOW);
      digitalWrite(TEST_SIGNAL_R4, LOW);
      digitalWrite(TEST_SIGNAL_COIL, LOW);
      
      // Wait 1 second
      vTaskDelay(1000/portTICK_PERIOD_MS);

      //
      // Inductance and Capacitance
      //

      // Generate a waveform as an array of ints
      analogReference(INTERNAL1V1);
      settleADC();

      unsigned int i = 0;
      digitalWrite(TEST_SIGNAL_R4, HIGH);
      digitalWrite(TEST_SIGNAL_COIL, HIGH);
      digitalWrite(TEST_SIGNAL_C1, HIGH);
      vTaskDelay(1000/portTICK_PERIOD_MS);
      digitalWrite(TEST_SIGNAL_R4, LOW);

      // Wait for a value below the threshold, indicating the relay has had time to react
      unsigned int currentValue = 0;
      for (i=0; i<ADC_READINGS_SIZE; i++)
      {
        currentValue = analogRead(OUTPUT_SIGNAL);
        if (currentValue <= ADC_LOW_THRESHOLD)
        {
          break;  
        }
      }

      // THEN start recording data
      TCNT1 = 0;
      for (i=0; i<ADC_READINGS_SIZE; i++)
      {
        adcReadings[i] = analogRead(OUTPUT_SIGNAL);
        adcTimes[i] = TCNT1;  
      }

      analogReference(DEFAULT);
      settleADC();
      
      // Look through that array of ints to determine the frequency
      unsigned int t1 = 0;
      lFailed = true;

      // Look for the next point that's above the threshold and mark it as t1
      for(i = 0; i<ADC_READINGS_SIZE; i++)
      {
        if (adcReadings[i] > ADC_LOW_THRESHOLD)
        {
          t1 = adcTimes[i];
          lFailed = false;
          break;  
        }  
      }

      double period1 = 2*0.0000005*t1;
      double f1 = 1.0/period1;

      // Lower all signals and wait one second
      digitalWrite(TEST_SIGNAL_R1, LOW);
      digitalWrite(TEST_SIGNAL_R2, LOW);
      digitalWrite(TEST_SIGNAL_R3, LOW);
      digitalWrite(TEST_SIGNAL_R4, LOW);
      digitalWrite(TEST_SIGNAL_COIL, LOW);
      digitalWrite(TEST_SIGNAL_C1, LOW);
      digitalWrite(TEST_SIGNAL_C2, LOW);
      vTaskDelay(1000/portTICK_PERIOD_MS);
      
      // Now do it again, calculating f2 using the second parallel capacitance C2
      analogReference(INTERNAL1V1);
      settleADC();

      i = 0;
      digitalWrite(TEST_SIGNAL_R4, HIGH);
      digitalWrite(TEST_SIGNAL_COIL, HIGH);
      digitalWrite(TEST_SIGNAL_C2, HIGH);
      vTaskDelay(1000/portTICK_PERIOD_MS);
      digitalWrite(TEST_SIGNAL_R4, LOW);

      // Wait for a value below the threshold, indicating the relay has had time to react
      for (i=0; i<ADC_READINGS_SIZE; i++)
      {
        currentValue = analogRead(OUTPUT_SIGNAL);
        if (currentValue <= ADC_LOW_THRESHOLD)
        {
          break;  
        }
      }

      // THEN start recording data
      TCNT1 = 0;
      for (i=0; i<ADC_READINGS_SIZE; i++)
      {
        adcReadings[i] = analogRead(OUTPUT_SIGNAL);
        adcTimes[i] = TCNT1;  
      }

      analogReference(DEFAULT);
      settleADC();

      // Look through that array of ints to determine the frequency
      unsigned int t2 = 0;
      lFailed = true;

      // Look for the next point that's above the threshold and mark it as t2
      for(i=0; i<ADC_READINGS_SIZE; i++)
      {
        if (adcReadings[i] > ADC_LOW_THRESHOLD)
        {
          t2 = adcTimes[i];
          lFailed = false;
          break;
        }  
      }
      
      // Do math
      double period2 = 2*0.0000005*t2;
      double f2 = 1.0/period2;

      // Now use the frequencies to determine l and c
      l = (1.0-(f2*f2)/(f1*f1))/(4.0*PI*PI*f2*f2*(CIN2-CIN1));
      c = 1.0/(4.0*PI*PI*f1*f1*l)-CIN1;

      
      // Print out resistance, inductance and capacitance values
      if (!rFailed)
      {
        Serial.print(r);
        Serial.print(" Ohms");
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
        Serial.print(lMilliHenrys);
        Serial.print(" mH\n");

        float cMicroFarads = c*1000000.0;
        Serial.print(cMicroFarads);
        Serial.print(" uF\n");
      }
      else
      {
        Serial.print("Inductance Measurement Failed\n");
        Serial.print("Capacitance Measurement Failed\n"); 
      }

      digitalWrite(TEST_SIGNAL_R1, LOW);
      digitalWrite(TEST_SIGNAL_R2, LOW);
      digitalWrite(TEST_SIGNAL_R3, LOW);
      digitalWrite(TEST_SIGNAL_R4, LOW);
      digitalWrite(TEST_SIGNAL_COIL, LOW);
      digitalWrite(TEST_SIGNAL_C1, LOW);
      digitalWrite(TEST_SIGNAL_C2, LOW);
    }
  }
}
