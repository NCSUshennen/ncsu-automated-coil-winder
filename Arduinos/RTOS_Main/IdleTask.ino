static void MyIdleTask(void* pvParameters)
{
  //The Idle Task. With the highest priority value, this task calls when no other task is running.
  while(1)
  { 
    task = 0;
    digitalWrite(TASK_1,LOW);
    digitalWrite(TASK_2,LOW); 
    //digitalWrite(TASK_3,LOW);
    digitalWrite(TASK_IDLE,HIGH);
    if (askedForReady)
    {
      Serial.write("ready\n");
      askedForReady = false;  
    }
    delay(50);
  }
}
