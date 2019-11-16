/**
 * Task1
 * 
 * Dan Hayduk
 * November 15, 2019
 * 
 * Task 1 currently sends all the correct control signals to the motor driver to make the motor spin one revolution.
 * It will soon be updated to interpret simple G-Code sent from the Pi to send signals to multiple motors.
 */

static void MyTask1(void* pvParameters)
{
  //Task 1. We display the task label on Serial monitor.
  while(1)
  { 
    if (xSemaphoreTake(xSemaphore1, portMAX_DELAY) == pdTRUE)
    {
      task = 1;
      digitalWrite(TASK_1,HIGH);
      digitalWrite(TASK_2,LOW); 
      digitalWrite(TASK_3,LOW);
      digitalWrite(TASK_IDLE,LOW);

      // Set up motor
      vTaskDelay(1/portTICK_PERIOD_MS);
      //digitalWrite(MOTOR_1_DIR, HIGH);
      vTaskDelay(1/portTICK_PERIOD_MS);
      
      // Spin 51200 steps
      unsigned int i; // 65535 MAX
      for (i=0; i<51200; i++)
      {
        digitalWrite(MOTOR_1_PLS, HIGH);
        //vTaskDelay(1/portTICK_PERIOD_MS);
        digitalWrite(MOTOR_1_PLS, LOW);
        //vTaskDelay(1/portTICK_PERIOD_MS);
        //Serial.println(i);
      }
      
      // Disable motor
      digitalWrite(MOTOR_1_ENA, LOW);
      vTaskDelay(1/portTICK_PERIOD_MS);
      digitalWrite(MOTOR_1_DIR, LOW);
      vTaskDelay(1/portTICK_PERIOD_MS);
    }
  }
}
