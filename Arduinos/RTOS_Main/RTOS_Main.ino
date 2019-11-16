/**
 * RTOS_Main
 * 
 * Dan Hayduk
 * November 2, 2019
 * 
 * This code establishes the RTOS System with working semaphores and message buffers. Tasks MyTask1, MyTask2, and MyTask3 
 * are set up to run the algorithms we originally assigned to Arduino 1, 2, and 3, respectively.
 */

// Includes the RTOS, semaphores and message buffers
#include <Arduino_FreeRTOS.h>
#include "semphr.h"
#include "message_buffer.h"

// Needed to for semaphore configuration
#define INCLUDE_vTaskSuspend 1
#define configSUPPORT_DYNAMIC_ALLOCATION 1

// LEDs to indicate which task is currently running
#define DEBUG_LED 7
#define TASK_1 8
#define TASK_2 9
#define TASK_3 10
#define TASK_IDLE 11

//Pins used in Task 1
#define MOTOR_1_PLS 23
#define MOTOR_1_DIR 25
#define MOTOR_1_ENA 27

// Pins used in Task 2
#define SENSOR_1 A1
#define SENSOR_2 A2

// Pins used in Task 3
#define TEST_SIGNAL 52
#define OUTPUT_SIGNAL A0

// Commands
#define SERIAL_TEST "Hello There!"
#define READY_ASK "isArduinoReady"
#define TASK_1_COMMAND_SIMPLE "spinMotorOnce"
#define TASK_2_COMMAND_ALL "getAllSensorValue"
#define TASK_2_COMMAND_SENSOR1 "getSensor1SensorValue"
#define TASK_2_COMMAND_SENSOR2 "getSensor2SensorValue"
#define TASK_3_COMMAND "doPostWindingTest"

//What task am I in?
uint8_t task = 0; // 0 indicates Idle
bool askedForReady = false;


//Global semaphore variables
SemaphoreHandle_t xSemaphore1;
SemaphoreHandle_t xSemaphore3;

//Global message variables
MessageBufferHandle_t xMessageBuffer2;
uint8_t sensorNum; //Indicator for the sensor to read the next time Task 2 runs. If this number is 0, read all the sensors


//Global variables to store output signal data in Task 3
int voltages[1000];

String inputString = "";


void setup()
{
  //Initialize the Serial Monitor with 9600 baud rate
  Serial.begin(9600);
  inputString.reserve(100);

  Serial.println("asodihgobiadsbg");
  
  Init_Pins();
  Init_SemaphoresAndMessageBuffers();
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
  pinMode(TASK_1, OUTPUT);
  pinMode(TASK_2, OUTPUT);
  pinMode(TASK_3, OUTPUT);
  pinMode(TASK_IDLE, OUTPUT);

  pinMode(MOTOR_1_PLS, OUTPUT);
  pinMode(MOTOR_1_DIR, OUTPUT);
  pinMode(MOTOR_1_ENA, OUTPUT);

  pinMode(SENSOR_1, INPUT);
  pinMode(SENSOR_2, INPUT);

  pinMode(TEST_SIGNAL, OUTPUT);
  pinMode(OUTPUT_SIGNAL, INPUT); // That is, this is the output of the system, which the Arduino is measuring

  digitalWrite(DEBUG_LED, LOW);
  digitalWrite(TASK_1, LOW);
  digitalWrite(TASK_2, LOW);
  digitalWrite(TASK_3, LOW);
  digitalWrite(TASK_IDLE, LOW);

  digitalWrite(MOTOR_1_PLS, LOW);
  digitalWrite(MOTOR_1_DIR, LOW);
  digitalWrite(MOTOR_1_ENA, LOW);
}

void Init_SemaphoresAndMessageBuffers()
{
  /**
   * Create semaphores and message buffers for each task.
   * 
   * Returns: nothing
   */
  xSemaphore1 = xSemaphoreCreateCounting(10, 0);
  xSemaphore3 = xSemaphoreCreateCounting(10, 0);

  xMessageBuffer2 = xMessageBufferCreate(100*sizeof(sensorNum));
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

float ADCToVoltage(int adcValue)
{
  /**
   * Takes the 10-bit integer value as read by the ADC and returns a floating-point decimal
   * corresponding to the voltage.
   * 
   * Returns: the corresponding voltage level read by the ADC
   */

   return adcValue/204.6;
}
