static void MyTask1(void* pvParameters)
{
  //Task 1. We display the task label on Serial monitor.
  while(1)
  { 
    if (xSemaphoreTake(xSemaphore1, portMAX_DELAY) == pdTRUE)
    {
      digitalWrite(TASK_1,HIGH);
      digitalWrite(TASK_2,LOW); 
      digitalWrite(TASK_3,LOW);
      digitalWrite(TASK_IDLE,LOW); 
      Serial.println(F("Task1"));
      //vTaskDelay(100/portTICK_PERIOD_MS);
    }
  }
}
