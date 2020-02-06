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

#define X_AXIS_LIMIT 1000
#define Y_AXIS_LIMIT 600
#define Z_AXIS_LIMIT 200

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
      int i;
      bool hitXZeroingSwitch = false;
      for (i=0; i<X_AXIS_LIMIT; i++)
      {
        digitalWrite(MOTOR_X1_PLS, HIGH);
        digitalWrite(MOTOR_X2_PLS, HIGH);
        vTaskDelay(1/portTICK_PERIOD_MS/4);
        digitalWrite(MOTOR_X1_PLS, LOW);
        digitalWrite(MOTOR_X2_PLS, LOW);
        vTaskDelay(1/portTICK_PERIOD_MS/4);

        if (digitalRead(ZEROING_X) != HIGH)
        {
          // Only stop moving if the zeroing switch is hit if we are trying to go in the negative X-Direction
          hitXZeroingSwitch = true;
          break;
        }
      }

      // Break if move more than the max distance without hitting the switch
      if (!hitXZeroingSwitch || digitalRead(OVER_POSITION) == HIGH)
      {
        Serial.println("ErrorFailedToHitXZeroingSwitch");
        continue; 
      }

      // Move y-axis until we hit the zeroing switch
      bool hitYZeroingSwitch = false;
      for (i=0; i<Y_AXIS_LIMIT; i++)
      {
        digitalWrite(MOTOR_Y_PLS, HIGH);
        vTaskDelay(1/portTICK_PERIOD_MS/4);
        digitalWrite(MOTOR_Y_PLS, LOW);
        vTaskDelay(1/portTICK_PERIOD_MS/4);

        if (digitalRead(ZEROING_Y) != HIGH)
        {
          // Only stop moving if the zeroing switch is hit if we are trying to go in the negative X-Direction
          hitYZeroingSwitch = true;
          break;
        }
      }

      if (!hitYZeroingSwitch || digitalRead(OVER_POSITION) == HIGH)
      {
        Serial.println("ErrorFailedToHitYZeroingSwitch");
        continue; 
      }

      bool hitZZeroingSwitch = false;
      for (i=0; i<Z_AXIS_LIMIT; i++)
      {
        digitalWrite(MOTOR_Z_PLS, HIGH);
        vTaskDelay(1/portTICK_PERIOD_MS/4);
        digitalWrite(MOTOR_Z_PLS, LOW);
        vTaskDelay(1/portTICK_PERIOD_MS/4);

        if (digitalRead(ZEROING_Z) != HIGH)
        {
          // Only stop moving if the zeroing switch is hit if we are trying to go in the negative X-Direction
          hitZZeroingSwitch = true;
          break;
        }
      }

      if (!hitZZeroingSwitch || digitalRead(OVER_POSITION) == HIGH)
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
