/**
 * RTOS_Main
 * 
 * Dan Hayduk
 * December 2, 2019
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
#define TASK_1 8
#define TASK_2 9
#define TASK_3 10
#define TASK_IDLE 11

//Pins used in Task 1
#define MOTOR_X1_PLS 23
#define MOTOR_X1_DIR 25
#define MOTOR_X1_ENA 27

#define MOTOR_Y_PLS 31
#define MOTOR_Y_DIR 33
#define MOTOR_Y_ENA 35

#define MOTOR_Z_PLS 39
#define MOTOR_Z_DIR 41
#define MOTOR_Z_ENA 43

#define MOTOR_X2_PLS 47
#define MOTOR_X2_DIR 49
#define MOTOR_X2_ENA 51

// Pins for pulse interrupts for the Motor Simulator
// Turn these off when not using the Motor Simulator
#define USE_MOTOR_SIMULATOR 0

#define X_INTERRUPT 18
#define Y_INTERRUPT 19
#define Z_INTERRUPT 20

// Pins used in Task 2
#define SENSOR_1 A1
#define SENSOR_2 A2

// Pins used in Task 3
#define TEST_SIGNAL_RANDC 52
#define TEST_SIGNAL_L 53
#define OUTPUT_SIGNAL A0

// Pins used in Task MotorSimulator
#define X0_Y0 7
#define X1_Y0 6
#define X0_Y1 5
#define X1_Y1 4

// Used for easier array access when determining coordinates
#define X 0
#define Y 1
#define Z 2

// Commands
#define SERIAL_TEST "Hello There!"
#define READY_ASK "isArduinoReady"
#define TASK_1_COMMAND_SIMPLEX "spinMotorOnceXUp" // that is, increase the X-Value
#define TASK_1_COMMAND_SIMPLEY "spinMotorOnceYUp"
#define TASK_1_COMMAND_SIMPLEZ "spinMotorOnceZDown" // that is, decrease the Z-Value, which means the motor spins forward
#define TASK_1_COMMAND_SIMPLEXR "spinMotorOnceXDown"
#define TASK_1_COMMAND_SIMPLEYR "spinMotorOnceYDown"
#define TASK_1_COMMAND_SIMPLEZR "spinMotorOnceZUp"
#define TASK_1_COMMAND "beginWindingPath"
#define TASK_1_BEGIN_G_CODE "%"
#define TASK_2_COMMAND_ALL "getAllSensorValue"
#define TASK_2_COMMAND_SENSOR1 "getSensor1SensorValue"
#define TASK_2_COMMAND_SENSOR2 "getSensor2SensorValue"
#define TASK_3_COMMAND "doPostWindingTest"

// Macros to set timing for timer interrupts


uint8_t task = 0; // 0 indicates Idle
bool askedForReady = false;
bool readingGCode = false;

//Global semaphore variables
SemaphoreHandle_t xSemaphore1;
SemaphoreHandle_t xSemaphorePercent;
SemaphoreHandle_t xSemaphore3;
SemaphoreHandle_t xSemaphoreTimer;

//Global message variables
MessageBufferHandle_t xMessageBuffer2;
uint8_t sensorNum; // Indicator for the sensor to read the next time Task 2 runs. If this number is 0, read all the sensors
MessageBufferHandle_t xMessageBufferM; // Message buffer for the motor simulator, to be updated whenver a pulse is given
enum {X_FORWARD, X_REVERSE, Y_FORWARD, Y_REVERSE, Z_FORWARD, Z_REVERSE} motorMessage;
MessageBufferHandle_t xMessageBufferGCode;
char inChar;
MessageBufferHandle_t xMessageBufferManualTurnDirection;
uint8_t manualTurnDirection;

//Global variables to store output signal data in Task 3
int voltages[1000];

String inputString = ""; // Used for reading commands
String gCodeString = ""; // Used for reading G-Code during a winding algorithm
String magnitudeString = ""; // Used while reading G-Code to determine the magnitude of each movement

void setup()
{
  //Initialize the Serial Monitor with 9600 baud rate
  Serial.begin(9600);
  inputString.reserve(100);
  gCodeString.reserve(100);
  magnitudeString.reserve(20);

  //Serial.write("ready\n");
  
  Init_Pins();
  Init_SemaphoresAndMessageBuffers();
  Init_Timers();
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

  pinMode(TASK_1, OUTPUT);
  pinMode(TASK_2, OUTPUT);
  pinMode(TASK_3, OUTPUT);
  pinMode(TASK_IDLE, OUTPUT);

  pinMode(MOTOR_X1_PLS, OUTPUT);
  pinMode(MOTOR_X1_DIR, OUTPUT);
  pinMode(MOTOR_X1_ENA, OUTPUT);

  pinMode(MOTOR_Y_PLS, OUTPUT);
  pinMode(MOTOR_Y_DIR, OUTPUT);
  pinMode(MOTOR_Y_ENA, OUTPUT);

  pinMode(MOTOR_Z_PLS, OUTPUT);
  pinMode(MOTOR_Z_DIR, OUTPUT);
  pinMode(MOTOR_Z_ENA, OUTPUT);

  pinMode(MOTOR_X2_PLS, OUTPUT);
  pinMode(MOTOR_X2_DIR, OUTPUT);
  pinMode(MOTOR_X2_ENA, OUTPUT);

#if USE_MOTOR_SIMULATOR
  pinMode(X_INTERRUPT, INPUT);
  attachInterrupt(digitalPinToInterrupt(X_INTERRUPT), xMotorISR, RISING);
  pinMode(Y_INTERRUPT, INPUT);
  attachInterrupt(digitalPinToInterrupt(Y_INTERRUPT), yMotorISR, RISING);
  pinMode(Z_INTERRUPT, INPUT);
  attachInterrupt(digitalPinToInterrupt(Z_INTERRUPT), zMotorISR, RISING);
#endif

  pinMode(X0_Y0, OUTPUT);
  pinMode(X1_Y0, OUTPUT);
  pinMode(X0_Y1, OUTPUT);
  pinMode(X1_Y1, OUTPUT);

  pinMode(SENSOR_1, INPUT);
  pinMode(SENSOR_2, INPUT);

  pinMode(TEST_SIGNAL_RANDC, OUTPUT);
  pinMode(TEST_SIGNAL_L, OUTPUT);
  pinMode(OUTPUT_SIGNAL, INPUT); // That is, this is the output of the system, which the Arduino is measuring

  digitalWrite(TASK_1, LOW);
  digitalWrite(TASK_2, LOW);
  digitalWrite(TASK_3, LOW);
  digitalWrite(TASK_IDLE, LOW);

  digitalWrite(X0_Y0, LOW);
  digitalWrite(X1_Y0, LOW);
  digitalWrite(X0_Y1, LOW);
  digitalWrite(X1_Y1, LOW);

  digitalWrite(MOTOR_X1_PLS, LOW);
  digitalWrite(MOTOR_X1_DIR, LOW);
  digitalWrite(MOTOR_X1_ENA, LOW);

  digitalWrite(MOTOR_Y_PLS, LOW);
  digitalWrite(MOTOR_Y_DIR, LOW);
  digitalWrite(MOTOR_Y_ENA, LOW);

  digitalWrite(MOTOR_Z_PLS, LOW);
  digitalWrite(MOTOR_Z_DIR, LOW);
  digitalWrite(MOTOR_Z_ENA, LOW);

  digitalWrite(MOTOR_X2_PLS, LOW);
  digitalWrite(MOTOR_X2_DIR, LOW);
  digitalWrite(MOTOR_X2_ENA, LOW);
}

void Init_SemaphoresAndMessageBuffers()
{
  /**
   * Create semaphores and message buffers for each task.
   * 
   * Returns: nothing
   */
   
  xSemaphore1 = xSemaphoreCreateCounting(10, 0);
  xSemaphorePercent = xSemaphoreCreateCounting(10, 0);
  xSemaphore3 = xSemaphoreCreateCounting(10, 0);
  xSemaphoreTimer = xSemaphoreCreateCounting(10, 0);
  
  xMessageBuffer2 = xMessageBufferCreate(100*sizeof(sensorNum));
  xMessageBufferM = xMessageBufferCreate(100*sizeof(motorMessage));
  xMessageBufferGCode = xMessageBufferCreate(100*sizeof(inChar));
  xMessageBufferManualTurnDirection = xMessageBufferCreate(100*sizeof(manualTurnDirection));
}

void Init_Timers()
{
  /**  
   * Set up the timer peripheral and all timers that will be used in the three algorithms.
   * 
   * Note: clock frequency is 16 MHz
   * See Datasheet page 154 and the Youtube tutorial by GreatScott! for details on timer setup
   * 
   * Returns: nothing
   */

   TCCR1A = 0;
   TCCR1B = 0;
   OCR1A = 62499; //one-second interrupt
   OCR1B = 15624; //quarter-second interrupt (not used)
   TCCR1B |= (1<<CS12) | (1<<WGM12); //prescalar 256, CTC mode; each tick is 16 us
   TIMSK1 |= (1<<OCIE1A) | (1<<OCIE1B); //enable 2 timer compare interrupts
   TIMSK1 &= ~(1<<OCIE1A); //disable interrupt A
}

void Init_Tasks()
{
  /**
   * Create three tasks with labels Task1, Task2, Task3 and MotorSimulator and assign priorities. 
   * We also create a fourth task labeled IdleTask to run when no task is in operation. 
   * The IdleTask has the lowest priority.
   * 
   * Returns: nothing
   */
  
  xTaskCreate(MyTask1, "Task1", 100, NULL, 1, NULL);
  xTaskCreate(MyTask2, "Task2", 100, NULL, 2, NULL);
  xTaskCreate(MyTask3, "Task3", 100, NULL, 3, NULL);
  xTaskCreate(MotorSimulator, "MotorSimulator", 100, NULL, 4, NULL);
  xTaskCreate(MyIdleTask, "IdleTask", 100, NULL, 0, NULL);

  xTaskCreate(MyTaskManualTurn, "TaskManualTurn", 100, NULL, 5, NULL);
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

void toggleLED(int ledPort)
{
   /**
    * Used to set an LED high if it is low, and low if it is high.
    * Parameter ledPort must correspond to a port specified as an output.
    * 
    * Returns: nothing
    */

    if (digitalRead(ledPort) == HIGH)
    {
      digitalWrite(ledPort, LOW);  
    }
    else
    {
      digitalWrite(ledPort, HIGH);  
    }
}

bool isInt(String wouldBeInt)
{
  /**
    * Reads a String to determine whether it can be converted into a valid integer variable
    * 
    * Returns: true if it can, false if it can't
    */  

    // The first character can be a '-'
    if (wouldBeInt.length() > 0 && wouldBeInt.charAt(0) != '0' && wouldBeInt.charAt(0) != '1' && wouldBeInt.charAt(0) != '2' && wouldBeInt.charAt(0) != '3' && wouldBeInt.charAt(0) != '4' &&
    wouldBeInt.charAt(0) != '5' && wouldBeInt.charAt(0) != '6' && wouldBeInt.charAt(0) != '7' && wouldBeInt.charAt(0) != '8' && wouldBeInt.charAt(0) != '9' && wouldBeInt.charAt(0) != '-')
    {
      return false;
    }

    int i;
    for (i=1; i<wouldBeInt.length(); i++)
    {
      if (wouldBeInt.charAt(i) != '0' && wouldBeInt.charAt(i) != '1' && wouldBeInt.charAt(i) != '2' && wouldBeInt.charAt(i) != '3' && wouldBeInt.charAt(i) != '4' &&
      wouldBeInt.charAt(i) != '5' && wouldBeInt.charAt(i) != '6' && wouldBeInt.charAt(i) != '7' && wouldBeInt.charAt(i) != '8' && wouldBeInt.charAt(i) != '9')
      {
        return false;  
      }  
    }
    return true;
}

int minIndexOf2(int index1, int index2)
{
  /**
    * In a list of 2 potential index values, return the minimum value that isn't -1, if there is one
    * This is used for reading G-Code
    * 
    * Note: This shouldn't be a problem, but this should not be used for reading G-Code Strings longer
    * than 999 characters
    * 
    * Returns: lowest non-negative integer in the array, or -1 if there is no non-negative integer
    */

    if ((index1 < 0) && (index2 < 0))
    {
      return -1;
    }
    else
    {
      int minIndex = 999;
      if ((index1 >= 0) && (index1 < minIndex))
      {
        minIndex = index1;
      }
      if ((index2 >= 0) && (index2 < minIndex))
      {
        minIndex = index2;
      }
      return minIndex;
    }
}
