static void MyIdleTask(void* pvParameters)
{
  //The Idle Task. With the highest priority value, this task calls when no other task is running.
  //bool task3WasHigh = false;
  while(1)
  { 
    //digitalWrite(TASK_1,LOW);
    digitalWrite(TASK_2,LOW); 
    digitalWrite(TASK_3,LOW);
    digitalWrite(TASK_IDLE,HIGH);
    //Serial.println(F("Idle"));
    delay(50);
  }
}
