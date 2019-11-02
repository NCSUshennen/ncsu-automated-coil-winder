static void MyTask2(void* pvParameters)
{  
  //Task 2. We display the task label on Serial monitor.
  while(1)
  { 
    if (xSemaphoreTake(xSemaphore2, portMAX_DELAY) == pdTRUE)
    {
      digitalWrite(TASK_1,LOW);
      digitalWrite(TASK_2,HIGH); 
      digitalWrite(TASK_3,LOW);
      digitalWrite(TASK_IDLE,LOW);   
      Serial.println(F("Task2"));
      //vTaskDelay(100/portTICK_PERIOD_MS);
    }
  }
}
