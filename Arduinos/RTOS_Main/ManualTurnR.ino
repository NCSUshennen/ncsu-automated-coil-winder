/**
 * TaskManualTurnR
 * 
 * Dan Hayduk
 * November 22, 2019
 * 
 * Task ManualTurnR is the same as Task ManualTurnF, but in reverse. This is to be used for testing the movement in both directions of a motor on the frame
 * (or on the motor simulator).
 */

static void MyTaskManualTurnR(void* pvParameters)
{
  while(1)
  { 
    if (xSemaphoreTake(xSemaphoreManualTurnR, portMAX_DELAY) == pdTRUE)
    {
      task = 6;
      
      vTaskDelay(1/portTICK_PERIOD_MS);
      digitalWrite(MOTOR_X_DIR, HIGH);
      vTaskDelay(1/portTICK_PERIOD_MS);
      
      // Spin 800 steps in the X and Y directions
      unsigned long int i; // 4294967295 MAX
      for (i=0; i<800; i++)
      {
        digitalWrite(MOTOR_X_PLS, HIGH);
        vTaskDelay(1/portTICK_PERIOD_MS/4);
        digitalWrite(MOTOR_X_PLS, LOW);
        vTaskDelay(1/portTICK_PERIOD_MS/4);
        digitalWrite(MOTOR_Y_PLS, HIGH);
        vTaskDelay(1/portTICK_PERIOD_MS/4);
        digitalWrite(MOTOR_Y_PLS, LOW);
        vTaskDelay(1/portTICK_PERIOD_MS/4);
        //Serial.println(i);
      }
      
      // Disable motors
      vTaskDelay(1/portTICK_PERIOD_MS);
      digitalWrite(MOTOR_X_DIR, LOW);
      digitalWrite(MOTOR_Y_DIR, LOW);
      vTaskDelay(1/portTICK_PERIOD_MS);
    }
  }
}
