/**
 * RTOS_Main
 * 
 * Dan Hayduk
 * November 2, 2019
 * 
 * This code establishes the RTOS System with working semaphores. Tasks MyTask1, MyTask2, and MyTask3 
 * are set up to run the algorithms we originally assigned to Arduino 1, 2, and 3, respectively.
 */

// Includes the RTOS and semaphores
#include <Arduino_FreeRTOS.h>
#include "semphr.h"

// Needed to for semaphore configuration
#define INCLUDE_vTaskSuspend 1
#define configSUPPORT_DYNAMIC_ALLOCATION 1

// LEDs to indicate which task is currently running
#define DEBUG_LED 7
#define TASK_1 8
#define TASK_2 9
#define TASK_3 10
#define TASK_IDLE 11

// Pins used in Task 3
#define TEST_SIGNAL 52
#define OUTPUT_SIGNAL A0

//Global semaphore variables
SemaphoreHandle_t xSemaphore1;
SemaphoreHandle_t xSemaphore2;
SemaphoreHandle_t xSemaphore3;

//Global variables to store output signal data in Task 3
int voltages[1000];

String inputString = "";


void setup()
{
  //Initialize the Serial Monitor with 9600 baud rate
  Serial.begin(9600);
  inputString.reserve(100);
  
  // Tell the Pi that I'm ready
  Serial.write("ready\n");
  
  Init_Pins();
  Init_Semaphores();
  Init_Tasks();
}

void loop()
{
  //There is no instruction in loop section of the code.
}

void Init_Pins()
{
  /**  
   * Initializes all pins which will be used for the system.
   * 
   * Returns: nothing
   */

  pinMode(DEBUG_LED, OUTPUT);
  pinMode(TASK_1,OUTPUT);
  pinMode(TASK_2,OUTPUT);
  pinMode(TASK_3,OUTPUT);
  pinMode(TASK_IDLE,OUTPUT);

  pinMode(TEST_SIGNAL, OUTPUT);
  pinMode(OUTPUT_SIGNAL, INPUT); // That is, this is the output of the system, which the Arduino is measuring
}

void Init_Semaphores()
{
  /**
   * Create semaphores for each task.
   * 
   * Returns: nothing
   */
  xSemaphore1 = xSemaphoreCreateCounting(10, 0);
  xSemaphore2 = xSemaphoreCreateCounting(10, 0);
  xSemaphore3 = xSemaphoreCreateCounting(10, 0);
}

void Init_Tasks()
{
  /**
   * Create three tasks with labels Task1, Task2 and Task3 and assign the priority as 1, 2 and 3 
   * respectively. We also create fourth task labeled as IdleTask when the there is no task in 
   * operation and it has the highest priority.
   * 
   * Returns: nothing
   */
  
  xTaskCreate(MyTask1, "Task1", 100, NULL, 1, NULL);
  xTaskCreate(MyTask2, "Task2", 100, NULL, 2, NULL);
  xTaskCreate(MyTask3, "Task3", 100, NULL, 3, NULL);
  xTaskCreate(MyIdleTask, "IdleTask", 100, NULL, 0, NULL);
  //We can change the priority of task according to our desire by changing the numerics
  //between NULL texts.
}
