/**
 * RTOS_Main
 * 
 * Dan Hayduk
 * March 5, 2020
 * 
 * This code establishes the RTOS System with working semaphores and message buffers. Tasks MyTask1, MyTask2, and MyTask3 
 * are set up to run the algorithms we originally assigned to Arduino 1, 2, and 3, respectively. An additional task has
 * been added for zeroing, titled MyTask4.
 */

// Includes the RTOS, semaphores and message buffers
#include <Arduino_FreeRTOS.h>
#include "semphr.h"
#include "message_buffer.h"

// Needed for semaphore configuration
#define INCLUDE_vTaskSuspend 1
#define configSUPPORT_DYNAMIC_ALLOCATION 1

// LEDs to indicate which task is currently running
#define TASK_1 8
#define TASK_2 9
#define TASK_3 10
#define TASK_IDLE 11

//Pins used in Task 1
#define MOTOR_Z_DIR 23
#define MOTOR_Z_PLS 25
#define MOTOR_Z_ALM 27

#define MOTOR_Y_DIR 29
#define MOTOR_Y_PLS 31
#define MOTOR_Y_ALM 33

#define MOTOR_X2_DIR 35
#define MOTOR_X2_PLS 37
#define MOTOR_X2_ALM 39

#define MOTOR_X1_DIR 41
#define MOTOR_X1_PLS 43
#define MOTOR_X1_ALM 45

#define ZEROING_Z 49
#define ZEROING_Y 51
#define ZEROING_X 53

#define WIRE_LENGTH_A 2
#define WIRE_LENGTH_B 3
#define WIRE_LENGTH_SW 5
#define WIRE_PRESENCE 52

#define OVER_POSITION1 40
#define OVER_POSITION2 A11
#define OVER_POSITION3 A12
#define OVER_POSITION4 A13
#define OVER_POSITION5 A14
#define OVER_POSITION6 A15

// Used to enable or disable the zeroing switches, over-position switches, alarm detection, and presence detection in the winding algorithm
// These should all be set to 1 in the final version, but for debugging, they can be set to 0 to avoid having to jump wires when testing other components
#define ENABLE_OUTOFBOUNDS_DETECTION 0
#define ENABLE_OVERPOSITION 0
#define ENABLE_ZEROING 0
#define ENABLE_ALARMS 0
#define ENABLE_PRESENCE_DETECTION 0

// Pins for pulse interrupts for the Motor Simulator
// Turn these off when not using the Motor Simulator
#define USE_MOTOR_SIMULATOR 1

#define X_INTERRUPT 18
#define Y_INTERRUPT 19
#define Z_INTERRUPT 20
#define SIMULATOR_TO_ENCODER 22

// Pins used in Task 2
#define SENSOR_1 A1
#define SENSOR_2 A2

// Pins used in Task 3
#define TEST_SIGNAL_RANDC1 28
#define TEST_SIGNAL_RANDC2 30
#define TEST_SIGNAL_RANDC3 32
#define TEST_SIGNAL_RANDC4 34
#define TEST_SIGNAL_RANDC5 36
#define TEST_SIGNAL_L 38
#define OUTPUT_SIGNAL A0
#define INPUT_VOLTAGE A7

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
#define TASK_4_COMMAND "beginZeroing"

// Error Messages
#define BAD_COMMAND_ERROR "ErrorBadCommand\n"
#define OVER_CURRENT_ERROR "ErrorOverCurrent\n"
#define DESTINATION_OUT_OF_BOUNDS_ERROR "ErrorDestinationOutOfBounds\n"
#define OVER_POSITION_ERROR "ErrorHitOverPositionSwitch\n"
#define ZEROING_Z_ERROR "ErrorHitZZeroingSwitch\n"
#define ZEROING_Y_ERROR "ErrorHitYZeroingSwitch\n"
#define ZEROING_X_ERROR "ErrorHitXZeroingSwitch\n"
#define ALARM_X1_ERROR "ErrorAlarmX1\n"
#define ALARM_X2_ERROR "ErrorAlarmX2\n"
#define ALARM_Y_ERROR "ErrorAlarmY\n"
#define ALARM_Z_ERROR "ErrorAlarmZ\n"
#define FAILED_ZEROING_Z_ERROR "ErrorFailedToHitZZeroingSwitch\n"
#define FAILED_ZEROING_Y_ERROR "ErrorFailedToHitYZeroingSwitch\n"
#define FAILED_ZEROING_X_ERROR "ErrorFailedToHitXZeroingSwitch\n"
#define WIRE_PRESENCE_ERROR "ErrorNoWireDetected\n"

// Rotary encoder globals
float pi = 3.14159; 
float Distance = 0;
float temp = 0;
float wheelCircum = 200; //in mm 
float PPR = 600;
volatile unsigned int temp2, counter = 0; // These variables will increase or decrease depending on the rotation of encoder
bool enableRotaryEncoder = false;
bool rotaryEncoderMoved = false;

uint8_t task = 0; // 0 indicates Idle
bool askedForReady = false;
bool readingGCode = false;

//Global semaphore variables
SemaphoreHandle_t xSemaphore1;
SemaphoreHandle_t xSemaphorePercent;
SemaphoreHandle_t xSemaphore3;
SemaphoreHandle_t xSemaphore4;
SemaphoreHandle_t xSemaphoreTimerA;
SemaphoreHandle_t xSemaphoreTimerB;

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
  
  Init_Pins();
  Init_SemaphoresAndMessageBuffers();
  Init_Timers();
  Init_Tasks();
}

void loop()
{
  //There is no instruction in the loop section of the code.
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
  pinMode(MOTOR_X1_ALM, INPUT);

  pinMode(MOTOR_Y_PLS, OUTPUT);
  pinMode(MOTOR_Y_DIR, OUTPUT);
  pinMode(MOTOR_Y_ALM, INPUT);

  pinMode(MOTOR_Z_PLS, OUTPUT);
  pinMode(MOTOR_Z_DIR, OUTPUT);
  pinMode(MOTOR_Z_ALM, INPUT);

  pinMode(MOTOR_X2_PLS, OUTPUT);
  pinMode(MOTOR_X2_DIR, OUTPUT);
  pinMode(MOTOR_X2_ALM, INPUT);

  pinMode(ZEROING_X, INPUT);
  pinMode(ZEROING_Y, INPUT);
  pinMode(ZEROING_Z, INPUT);
  
  pinMode(OVER_POSITION1, INPUT);
  pinMode(OVER_POSITION2, INPUT);
  pinMode(OVER_POSITION3, INPUT);
  pinMode(OVER_POSITION4, INPUT);
  pinMode(OVER_POSITION5, INPUT);
  pinMode(OVER_POSITION6, INPUT);

#if USE_MOTOR_SIMULATOR
  pinMode(X_INTERRUPT, INPUT);
  attachInterrupt(digitalPinToInterrupt(X_INTERRUPT), xMotorISR, RISING);
  pinMode(Y_INTERRUPT, INPUT);
  attachInterrupt(digitalPinToInterrupt(Y_INTERRUPT), yMotorISR, RISING);
  pinMode(Z_INTERRUPT, INPUT);
  attachInterrupt(digitalPinToInterrupt(Z_INTERRUPT), zMotorISR, RISING);
  pinMode(SIMULATOR_TO_ENCODER, OUTPUT);
  digitalWrite(SIMULATOR_TO_ENCODER, LOW);
#endif

  pinMode(MOTOR_X1_ALM, INPUT);
  pinMode(MOTOR_X2_ALM, INPUT);
  pinMode(MOTOR_Y_ALM, INPUT);
  pinMode(MOTOR_Z_ALM, INPUT);

  pinMode(X0_Y0, OUTPUT);
  pinMode(X1_Y0, OUTPUT);
  pinMode(X0_Y1, OUTPUT);
  pinMode(X1_Y1, OUTPUT);

  pinMode(SENSOR_1, INPUT);
  pinMode(SENSOR_2, INPUT);

  // Rotary encoder setup
  pinMode(WIRE_LENGTH_A, INPUT_PULLUP); // internal pullup input pin 2 
  pinMode(WIRE_LENGTH_B, INPUT_PULLUP); // internal pullup input pin 3
  pinMode(WIRE_LENGTH_SW, INPUT); //reset switch 
  
  //Setting up interrupt
  //A rising pulse from encoder activated ai0()
  attachInterrupt(digitalPinToInterrupt(WIRE_LENGTH_A), ai0, RISING);

  pinMode(WIRE_PRESENCE, INPUT);

  pinMode(TEST_SIGNAL_RANDC1, OUTPUT);
  pinMode(TEST_SIGNAL_RANDC2, OUTPUT);
  pinMode(TEST_SIGNAL_RANDC3, OUTPUT);
  pinMode(TEST_SIGNAL_RANDC4, OUTPUT);
  pinMode(TEST_SIGNAL_RANDC5, OUTPUT);
  pinMode(TEST_SIGNAL_L, OUTPUT);
  pinMode(OUTPUT_SIGNAL, INPUT); // That is, this is the output of the system, which the Arduino is measuring
  pinMode(INPUT_VOLTAGE, INPUT);

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

  digitalWrite(MOTOR_Y_PLS, LOW);
  digitalWrite(MOTOR_Y_DIR, LOW);

  digitalWrite(MOTOR_Z_PLS, LOW);
  digitalWrite(MOTOR_Z_DIR, LOW);

  digitalWrite(MOTOR_X2_PLS, LOW);
  digitalWrite(MOTOR_X2_DIR, LOW);

  digitalWrite(TEST_SIGNAL_RANDC1, LOW);
  digitalWrite(TEST_SIGNAL_RANDC2, LOW);
  digitalWrite(TEST_SIGNAL_RANDC3, LOW);
  digitalWrite(TEST_SIGNAL_RANDC4, LOW);
  digitalWrite(TEST_SIGNAL_RANDC5, LOW);
  digitalWrite(TEST_SIGNAL_L, LOW);
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
  xSemaphore4 = xSemaphoreCreateCounting(10, 0);
  xSemaphoreTimerA = xSemaphoreCreateCounting(10, 0);
  xSemaphoreTimerB = xSemaphoreCreateCounting(10, 0);
  
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
   OCR1B = 1; //one-tick interrupt
   TCCR1B |= (1<<CS12) | (1<<WGM12); //prescalar 256, CTC mode; each tick is 16 us
   TIMSK1 |= (1<<OCIE1A) | (1<<OCIE1B); //enable 2 timer compare interrupts
   TIMSK1 &= ~(1<<OCIE1A); //disable interrupt A
   TIMSK1 &= ~(1<<OCIE1B); //disable interrupt B
}

void Init_Tasks()
{
  /**
   * Create tasks with labels Task1, Task2, Task3, Task4, MotorSimulator and TaskManualTurn and assign priorities. 
   * There is another task labeled IdleTask, which has the lowest priority and runs when no task is in operation. 
   * 
   * Returns: nothing
   */
  
  xTaskCreate(MyTask1, "Task1", 100, NULL, 1, NULL);
  xTaskCreate(MyTask2, "Task2", 100, NULL, 2, NULL);
  xTaskCreate(MyTask3, "Task3", 200, NULL, 3, NULL);
  xTaskCreate(MyTask4, "Task4", 100, NULL, 4, NULL);
  xTaskCreate(MotorSimulator, "MotorSimulator", 200, NULL, 5, NULL);
  xTaskCreate(MyTaskManualTurn, "TaskManualTurn", 100, NULL, 6, NULL);
  xTaskCreate(MyIdleTask, "IdleTask", 100, NULL, 0, NULL);

  

}

float ADCToVoltage(int adcValue)
{
  /**
   * Takes the 10-bit integer value as read by the ADC and returns a floating-point decimal
   * corresponding to the voltage, assuming a 5V reference.
   * 
   * Returns: the corresponding voltage level read by the ADC
   */

   return adcValue/204.6;
}

double ADCToVoltage1V1(int adcValue)
{
  /**
   * Takes the 10-bit integer value as read by the ADC and returns a floating-point decimal
   * corresponding to the voltage, assuming a 1.1V reference.
   * 
   * Returns: the corresponding voltage level read by the ADC
   */

   return adcValue/930.0;
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

bool isFloat(String wouldBeFloat)
{
   /**
    * Reads a String to determine whether it can be converted into a valid floating-point variable
    * 
    * Returns: true if it can, false if it can't
    */  

    // The first character can be a '-'
    if (wouldBeFloat.length() > 0 && wouldBeFloat.charAt(0) != '0' && wouldBeFloat.charAt(0) != '1' && wouldBeFloat.charAt(0) != '2' && wouldBeFloat.charAt(0) != '3' && wouldBeFloat.charAt(0) != '4' &&
    wouldBeFloat.charAt(0) != '5' && wouldBeFloat.charAt(0) != '6' && wouldBeFloat.charAt(0) != '7' && wouldBeFloat.charAt(0) != '8' && wouldBeFloat.charAt(0) != '9' && wouldBeFloat.charAt(0) != '.' && 
    wouldBeFloat.charAt(0) != '-')
    {
      return false;
    }

    int i;
    for (i=1; i<wouldBeFloat.length(); i++)
    {
      if (wouldBeFloat.length() > 0 && wouldBeFloat.charAt(i) != '0' && wouldBeFloat.charAt(i) != '1' && wouldBeFloat.charAt(i) != '2' && wouldBeFloat.charAt(i) != '3' && wouldBeFloat.charAt(i) != '4' &&
      wouldBeFloat.charAt(i) != '5' && wouldBeFloat.charAt(i) != '6' && wouldBeFloat.charAt(i) != '7' && wouldBeFloat.charAt(i) != '8' && wouldBeFloat.charAt(i) != '9' && wouldBeFloat.charAt(i) != '.')
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

void settleADC()
{
    /**
    * This function is to be called after the ADC reference voltage has been changed in order to allow it
    * to settle. It does this by performing 50 dummy conversions
    * 
    * Returns: nothing
    */

    int i;
    int dummyConversionVariable;
    for (i = 0; i < 50; i++)
    {
      dummyConversionVariable = analogRead(OUTPUT_SIGNAL);
    }
}
