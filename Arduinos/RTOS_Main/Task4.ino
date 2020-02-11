/**
 * Task4
 * 
 * Dan Hayduk
 * February 6, 2020
 * 
 * Task 4 is the zeroing thread; it sends signals to the motor drivers in order to bring the winding head to the point (0, 0, 0).
 * This is defined by the positions of the zeroing switches, which will be used to indicate when the winding head has successfully
 * reached the zero point.
 */

#define X_AXIS_LIMIT 160000
#define Y_AXIS_LIMIT 96000
#define Z_AXIS_LIMIT 32000

static void MyTask4(void* pvParameters)
{  
  while(1)
  { 
    if (xSemaphoreTake(xSemaphore4, portMAX_DELAY) == pdTRUE)
    {
      // Set all motors in the negative direction
      digitalWrite(MOTOR_X1_DIR, HIGH);
      digitalWrite(MOTOR_X2_DIR, HIGH);
      digitalWrite(MOTOR_Y_DIR, HIGH);
      digitalWrite(MOTOR_Z_DIR, LOW);

      // Move x-axis until we hit the zeroing switch
      unsigned long int i;
      bool hitXZeroingSwitch = false;
      for (i=0; i<X_AXIS_LIMIT; i++)
      {
        if (digitalRead(ZEROING_X) != HIGH)
        {
          // Stop moving if the zeroing switch is hit
          hitXZeroingSwitch = true;
          break;
        }

        if (digitalRead(OVER_POSITION) != HIGH)
        {
          break;
        }
        
        digitalWrite(MOTOR_X1_PLS, HIGH);
        digitalWrite(MOTOR_X2_PLS, HIGH);
        vTaskDelay(1/portTICK_PERIOD_MS/4);
        digitalWrite(MOTOR_X1_PLS, LOW);
        digitalWrite(MOTOR_X2_PLS, LOW);
        vTaskDelay(1/portTICK_PERIOD_MS/4);
      }

      // Break if we move more than the max distance or hit the over-position switch without hitting the zeroing switch
      if (!hitXZeroingSwitch)
      {
        Serial.println("ErrorFailedToHitXZeroingSwitch");
        continue; 
      }

      // Move y-axis until we hit the zeroing switch
      bool hitYZeroingSwitch = false;
      for (i=0; i<Y_AXIS_LIMIT; i++)
      {
        if (digitalRead(ZEROING_Y) != HIGH)
        {
          // Stop moving if the zeroing switch is hit
          hitYZeroingSwitch = true;
          break;
        }

        if (digitalRead(OVER_POSITION) != HIGH)
        {
          break;
        }
        
        digitalWrite(MOTOR_Y_PLS, HIGH);
        vTaskDelay(1/portTICK_PERIOD_MS/4);
        digitalWrite(MOTOR_Y_PLS, LOW);
        vTaskDelay(1/portTICK_PERIOD_MS/4);
      }

      // Break if we move more than the max distance or hit the over-position switch without hitting the zeroing switch
      if (!hitYZeroingSwitch)
      {
        Serial.println("ErrorFailedToHitYZeroingSwitch");
        continue; 
      }

      bool hitZZeroingSwitch = false;
      for (i=0; i<Z_AXIS_LIMIT; i++)
      {
        if (digitalRead(ZEROING_Z) != HIGH)
        {
          // Stop moving if the zeroing switch is hit
          hitZZeroingSwitch = true;
          break;
        }

        if (digitalRead(OVER_POSITION) != HIGH)
        {
          break;
        }
        
        digitalWrite(MOTOR_Z_PLS, HIGH);
        vTaskDelay(1/portTICK_PERIOD_MS/4);
        digitalWrite(MOTOR_Z_PLS, LOW);
        vTaskDelay(1/portTICK_PERIOD_MS/4);
      }

      // Break if we move more than the max distance or hit the over-position switch without hitting the zeroing switch
      if (!hitZZeroingSwitch)
      {
        Serial.println("ErrorFailedToHitZZeroingSwitch");
        continue; 
      }
      
      Serial.println("Ta-da!");
      
      // Reset this position as (0, 0, 0)
      currentPosition[X] = 0;
      currentPosition[Y] = 0;
      currentPosition[Z] = 0;

      digitalWrite(MOTOR_X1_DIR, LOW);
      digitalWrite(MOTOR_X2_DIR, LOW);
      digitalWrite(MOTOR_Y_DIR, LOW);
      digitalWrite(MOTOR_Z_DIR, LOW);
    }
  }
}
