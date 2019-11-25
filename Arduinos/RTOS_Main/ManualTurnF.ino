/**
 * TaskManualTurnF
 * 
 * Dan Hayduk
 * November 22, 2019
 * 
 * Used for manually turning a given motor a given amount of turns. For prototyping, both with the motor simulator and the actual frame.
 */

static void MyTaskManualTurnF(void* pvParameters)
{
  while(1)
  { 
    if (xSemaphoreTake(xSemaphoreManualTurnF, portMAX_DELAY) == pdTRUE)
    {
      task = 5;
      
      vTaskDelay(2/portTICK_PERIOD_MS);
      
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
