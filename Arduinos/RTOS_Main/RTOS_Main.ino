/**
 * RTOS_Main
 * 
 * Dan Hayduk
 * October 26, 2019
 * 
 * This code establishes the RTOS System with working semaphores. Tasks MyTask1, MyTask2, and MyTask3 
 * are set up to run the algorithms we originally assigned to Arduino 1, 2, and 3, respectively.
 * 
 * Some functionality for MyTask3 has also been established. The command to initiate the task is currently
 * driven by rising-edge input to pin 23, which is polled by the Idle Task. I would have liked for this trigger
 * to have been initiated by an interrupt, but interrupts in Arduino do not seem to play nicely with semaphores.
 * 
 * MyTask3 currently successfully raises a 5-Volt input test signal and records an array of values when measuring
 * a signal for one second. However, since it currently uses the delay() function, the data captured does not accurately
 * capture a one-second period. This will be problematic when trying to use an output signal to determine a transfer
 * function, so current future work involves investigating the use of a timer peripheral to take more precisely-timed
 * measurements.
 */

// Includes the RTOS and semaphores
#include <Arduino_FreeRTOS.h>
#include "semphr.h"

// Needed to for semaphore configuration
#define INCLUDE_vTaskSuspend 1
#define configSUPPORT_DYNAMIC_ALLOCATION 1

// LEDs to indicate which task is currently running
#define TASK_1 8
#define TASK_2 9
#define TASK_3 10
#define TASK_IDLE 11

// The pin to signal a request to run Task 3
#define TASK_3_COMMAND 23

// Pins used in Task 3
#define TEST_SIGNAL 52
#define OUTPUT_SIGNAL A0

//Global semaphore variables
SemaphoreHandle_t xSemaphore1;
SemaphoreHandle_t xSemaphore2;
SemaphoreHandle_t xSemaphore3;

//Global variables to store output signal data in Task 3
float voltages[1000];

void setup()
{
  //Initialize the Serial Monitor with 9600 baud rate
  Serial.begin(9600);
  //Serial.println(F("In Setup function"));
  
  //Set the digital pins
  pinMode(TASK_1,OUTPUT);
  pinMode(TASK_2,OUTPUT);
  pinMode(TASK_3,OUTPUT);
  pinMode(TASK_IDLE,OUTPUT);

  pinMode(TASK_3_COMMAND, INPUT);

  pinMode(TEST_SIGNAL, OUTPUT);
  pinMode(OUTPUT_SIGNAL, INPUT); // That is, this is the output of the system, which the Arduino is measuring

  //Create semaphores for each task, besides the Idle task
  xSemaphore1 = xSemaphoreCreateCounting(10, 0);
  xSemaphore2 = xSemaphoreCreateCounting(10, 0);
  xSemaphore3 = xSemaphoreCreateCounting(10, 0);

  //Create three tasks with labels Task1, Task2 and Task3 and assign the priority as 1, 2 and 3 
  //respectively. We also create fourth task labeled as IdleTask when the there is no task in 
  //operation and it has the highest priority.
  xTaskCreate(MyTask1, "Task1", 100, NULL, 1, NULL);
  xTaskCreate(MyTask2, "Task2", 100, NULL, 2, NULL);
  xTaskCreate(MyTask3, "Task3", 100, NULL, 3, NULL);
  xTaskCreate(MyIdleTask, "IdleTask", 100, NULL, 0, NULL);
  
  //We can change the priority of task according to our desire by changing the numerics
  //between NULL texts.
}

void loop()
{
  //There is no instruction in loop section of the code.
}

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

static void MyTask3(void* pvParameters)
{ 
  /* Task 3: 

     When called by the semaphore, this task raises the test signal,
     then captures the value of the output signal every microsecond for one second (?)
  
  */
  while(1)
  { 
    if (xSemaphoreTake(xSemaphore3, portMAX_DELAY) == pdTRUE)
    {
      digitalWrite(TASK_1,LOW);
      digitalWrite(TASK_2,LOW); 
      digitalWrite(TASK_3,HIGH);
      digitalWrite(TASK_IDLE,LOW);

      digitalWrite(TEST_SIGNAL, HIGH);
      int i;
      int voltagesSize = 1000;
      for (i=0; i<voltagesSize; i++)
      {
        float voltage = analogRead(OUTPUT_SIGNAL)/204.6;
        voltages[i] = voltage;
        delay(1);
      }
      
      for (i=0; i<voltagesSize; i++)
      {
        Serial.println(voltages[i]);  
      }
      digitalWrite(TEST_SIGNAL, LOW);
    }
  }
}

static void MyIdleTask(void* pvParameters)
{
  //The Idle Task. With the highest priority value, this task calls when no other task is running.
  bool task3WasHigh = false;
  while(1)
  { 
    digitalWrite(TASK_1,LOW);
    digitalWrite(TASK_2,LOW); 
    digitalWrite(TASK_3,LOW);
    digitalWrite(TASK_IDLE,HIGH);
    //Serial.println(F("Idle"));
    delay(50);

    //Check to see if TASK_3_COMMAND is high, then give a semaphore
    if (digitalRead(TASK_3_COMMAND) == HIGH && !task3WasHigh)
    {
      xSemaphoreGive(xSemaphore3);
      task3WasHigh = true;
      //We shouldn't need to care if this fails due to a full semaphore
    }
    else if (digitalRead(TASK_3_COMMAND) == LOW)
    {
      task3WasHigh = false;  
    }
  }
}
