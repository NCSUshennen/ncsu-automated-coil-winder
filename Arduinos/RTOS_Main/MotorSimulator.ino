/**
 * MotorSimulator
 * 
 * Dan Hayduk
 * April 28, 2020
 * 
 * This task is used to help debug the algorithm to read G-Code. Rather than attach the Arduino to the actual
 * motor drivers, this code allows for simulating the position of the head on a 2x2 grid of LEDs, as well as
 * printing the position that would result from running a certain winding algorithm. This not only makes prototyping
 * simpler, but avoids the risk of damaging the motors or their drivers while testing the G-Code algorithm.
 * 
 * The rotary encoder can also be simulated by entering in certain dimension specifications ELONGATION_PERCENT, WIND_WIDTH,
 * WIND_LENGTH, and WIRE_DIAMETER.
 * 
 * Note: To use the simulator, make sure to connect LEDs to pins 4-7 as specified in their macros and use jumper
 * wires to connect pins 37 and 18, 31 and 19, and 25 and 20. To simulate the rotary encoder, connect pin 22 to pin
 * 2 and pin 3 to GND.
 */

#define TURN_RATE 800
#define MM_RATE 160
#define LED_CHANGE_RATE 8

// Used for simulating a given elongation
#define ELONGATION_PERCENT 4.67
#define WIND_WIDTH 10
#define WIND_LENGTH 130
#define WIRE_DIAMETER 1

#define ENCODER_MM_RATE MM_RATE/(1+ELONGATION_PERCENT/100.0)

int simulatorPosition[3] = {0, 0, 0};
int simulatorPositionMM[3] = {0, 0, 0};
unsigned int pulsesToNext[3] = {LED_CHANGE_RATE, LED_CHANGE_RATE, LED_CHANGE_RATE}; // Used to keep track of when to "move" the head on the LEDs
unsigned int pulsesToPrev[3] = {LED_CHANGE_RATE, LED_CHANGE_RATE, LED_CHANGE_RATE};
boolean positionWasUpdated = false;

unsigned long int totalSimulatorPulses = 0;
unsigned long int totalPulsesSentToEncoder = 0;
unsigned long int pulsesIShouldHaveSent = 0;

unsigned long int xPulsesThisMovement = 0;
unsigned long int yPulsesThisMovement = 0;

static void MotorSimulator(void* pvParameters)
{
  digitalWrite(X0_Y0, HIGH);
  digitalWrite(X1_Y0, LOW);
  digitalWrite(X0_Y1, LOW);
  digitalWrite(X1_Y1, LOW);

  while (1)
  {
    static enum {S0, S1, S2, S3, S4} nextState = S0;
    enum {X_FORWARD, X_REVERSE, Y_FORWARD, Y_REVERSE, Z_FORWARD, Z_REVERSE} receivedMotorMessage;
    if (xMessageBufferReceive(xMessageBufferM, &receivedMotorMessage, sizeof(receivedMotorMessage), portMAX_DELAY) > 0)
    {
      task = 4;
      switch(receivedMotorMessage)
      {
        case X_FORWARD:
          pulsesToNext[X]--;
          pulsesToPrev[X]++;
          if (pulsesToNext[X] <= 0)
          {
             positionWasUpdated = true;
             simulatorPosition[X]++;
             pulsesToNext[X] = LED_CHANGE_RATE;
             pulsesToPrev[X] = LED_CHANGE_RATE;
          }
          break;
        case X_REVERSE:
          pulsesToNext[X]++;
          pulsesToPrev[X]--;
          if (pulsesToPrev[X] <= 0)
          {
             //Serial.println("I have been activated!");
             positionWasUpdated = true;
             simulatorPosition[X]--;
             pulsesToNext[X] = LED_CHANGE_RATE;
             pulsesToPrev[X] = LED_CHANGE_RATE;
             //Serial.println(simulatorPosition[X]);
             //Serial.println(simulatorPosition[Y]);
             //Serial.println(simulatorPosition[Z]);
          }
          break;
        case Y_FORWARD:
          pulsesToNext[Y]--;
          pulsesToPrev[Y]++;
          if (pulsesToNext[Y] <= 0)
          {
             positionWasUpdated = true;
             simulatorPosition[Y]++;
             pulsesToNext[Y] = LED_CHANGE_RATE;
             pulsesToPrev[Y] = LED_CHANGE_RATE;
          }
          break;
        case Y_REVERSE:
          pulsesToNext[Y]++;
          pulsesToPrev[Y]--;
          if (pulsesToPrev[Y] <= 0)
          {
             positionWasUpdated = true;
             simulatorPosition[Y]--;
             pulsesToNext[Y] = LED_CHANGE_RATE;
             pulsesToPrev[Y] = LED_CHANGE_RATE;
          }
          break;
        case Z_FORWARD:
          pulsesToNext[Z]++;
          pulsesToPrev[Z]--;
          if (pulsesToPrev[Z] <= 0)
          {
             positionWasUpdated = true;
             simulatorPosition[Z]--;
             pulsesToNext[Z] = LED_CHANGE_RATE;
             pulsesToPrev[Z] = LED_CHANGE_RATE;
          }
          break;
        case Z_REVERSE:
          pulsesToNext[Z]--;
          pulsesToPrev[Z]++;
          if (pulsesToNext[Z] <= 0)
          {
             positionWasUpdated = true;
             simulatorPosition[Z]++;
             pulsesToNext[Z] = LED_CHANGE_RATE;
             pulsesToPrev[Z] = LED_CHANGE_RATE;
          }
          break;
        default:
          break;
      }

      // Determine whether totalSimulatorPulses should be incremented for purposes of the rotary encoder
      switch (nextState)
      {
        case S0:
          // Rotary encoder simulator is off (not encoding)
          if (enableRotaryEncoder && (receivedMotorMessage == X_FORWARD || receivedMotorMessage == X_REVERSE))
          {
            nextState = S1;  
          }
          else if (enableRotaryEncoder && (receivedMotorMessage == Y_FORWARD || receivedMotorMessage == Y_REVERSE))
          {
            nextState = S3;
          }
          else
          {
            nextState = S0;  
          }
          break;
        case S1:
          // Start encoded X-movement
          xPulsesThisMovement = 1;
          totalSimulatorPulses++;
          
          if (enableRotaryEncoder && (receivedMotorMessage == X_FORWARD || receivedMotorMessage == X_REVERSE))
          {
            nextState = S2;  
          }
          else if (enableRotaryEncoder && (receivedMotorMessage == Y_FORWARD || receivedMotorMessage == Y_REVERSE))
          {
            nextState = S3;  
          }
          else
          {
            nextState = S0;  
          }
          break;
        case S2:
          // Continue encoded X-Movement
          xPulsesThisMovement++;
          if (xPulsesThisMovement <= MM_RATE*WIND_WIDTH)
          {
            totalSimulatorPulses++;
          }

          if (enableRotaryEncoder && (receivedMotorMessage == X_FORWARD || receivedMotorMessage == X_REVERSE))
          {
            nextState = S2;  
          }
          else if (enableRotaryEncoder && (receivedMotorMessage == Y_FORWARD || receivedMotorMessage == Y_REVERSE))
          {
            nextState = S3;  
          }
          else
          {
            nextState = S0;
          }
          break;
        case S3:
          // Start encoded Y-movement
          yPulsesThisMovement = 1;
          totalSimulatorPulses++;
          
          if (enableRotaryEncoder && (receivedMotorMessage == Y_FORWARD || receivedMotorMessage == Y_REVERSE))
          {
            nextState = S4;  
          }
          else if (enableRotaryEncoder && (receivedMotorMessage == X_FORWARD || receivedMotorMessage == X_REVERSE))
          {
            nextState = S1;  
          }
          else
          {
            nextState = S0;  
          }
          break;
        case S4:
          // Continue encoded Y-Movement
          yPulsesThisMovement++;
          if (yPulsesThisMovement <= MM_RATE*WIND_LENGTH + MM_RATE*WIRE_DIAMETER)
          {
            totalSimulatorPulses++;
          }

          if (enableRotaryEncoder && (receivedMotorMessage == Y_FORWARD || receivedMotorMessage == Y_REVERSE))
          {
            nextState = S4;  
          }
          else if (enableRotaryEncoder && (receivedMotorMessage == X_FORWARD || receivedMotorMessage == X_REVERSE))
          {
            nextState = S1;  
          }
          else
          {
            nextState = S0;
          }
          break;
        default:
          if (enableRotaryEncoder && (receivedMotorMessage == X_FORWARD || receivedMotorMessage == X_REVERSE))
          {
            nextState = S1;  
          }
          else if (enableRotaryEncoder && (receivedMotorMessage == Y_FORWARD || receivedMotorMessage == Y_REVERSE))
          {
            nextState = S3;
          }
          else
          {
            nextState = S0;  
          }
          break;  
      }

      
      pulsesIShouldHaveSent = totalSimulatorPulses/(ENCODER_MM_RATE/3.0); 
      // Based on the total number of motor driver pulses so far, how many pulses should have been sent to pin 2 to simulate the encoder ticking?
      
      if (pulsesIShouldHaveSent > totalPulsesSentToEncoder)
      {
        // Based on the total number of motor driver pulses so far, one more needs to be sent to pin 2 to accurately simulate the rotary encoder
        digitalWrite(SIMULATOR_TO_ENCODER, HIGH);
        vTaskDelay(1/portTICK_PERIOD_MS/4);
        digitalWrite(SIMULATOR_TO_ENCODER, LOW);
        totalPulsesSentToEncoder++;
      }
      
      if (positionWasUpdated)
      {
        // Calculate the simulator's position in millimeters and print it
#if MOTOR_SIMULATOR_PRINTING
        Serial.print("Position:  X: ");
        Serial.print(simulatorPosition[X]/20.0);
        Serial.print(", Y: ");
        Serial.print(simulatorPosition[Y]/20.0);
        Serial.print(", Z: ");
        Serial.println(simulatorPosition[Z]/20.0);
#endif
        positionWasUpdated = false;
      }
      
      if (simulatorPosition[X] == 0 && simulatorPosition[Y] == 0 /*&& simulatorPosition[Z] == 0*/)
      {
        // Light the 0, 0 LED
        digitalWrite(X0_Y0, HIGH);
        digitalWrite(X1_Y0, LOW);
        digitalWrite(X0_Y1, LOW);
        digitalWrite(X1_Y1, LOW); 
      }
      else if (simulatorPosition[X] == 1 && simulatorPosition[Y] == 0 /*&& simulatorPosition[Z] == 0*/)
      {
        // Light the 1, 0 LED
        digitalWrite(X0_Y0, LOW);
        digitalWrite(X1_Y0, HIGH);
        digitalWrite(X0_Y1, LOW);
        digitalWrite(X1_Y1, LOW); 
      }
      else if (simulatorPosition[X] == 0 && simulatorPosition[Y] == 1 /*&& simulatorPosition[Z] == 0*/)
      {
        // Light the 0, 1 LED
        digitalWrite(X0_Y0, LOW);
        digitalWrite(X1_Y0, LOW);
        digitalWrite(X0_Y1, HIGH);
        digitalWrite(X1_Y1, LOW); 
      }
      else if (simulatorPosition[X] == 1 && simulatorPosition[Y] == 1 /*&& simulatorPosition[Z] == 0*/)
      {
        // Light the 1, 1 LED
        digitalWrite(X0_Y0, LOW);
        digitalWrite(X1_Y0, LOW);
        digitalWrite(X0_Y1, LOW);
        digitalWrite(X1_Y1, HIGH); 
      }
      else
      {
        // Turn off all LEDs
        digitalWrite(X0_Y0, LOW);
        digitalWrite(X1_Y0, LOW);
        digitalWrite(X0_Y1, LOW);
        digitalWrite(X1_Y1, LOW); 
      }
    }
  }
}
