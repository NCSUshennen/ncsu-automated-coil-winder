/**
 * MotorSimulator
 * 
 * Dan Hayduk
 * December 2, 2019
 * 
 * This task is used to help debug the algorithm to read G-Code. Rather than attach the Arduino to the actual
 * motor drivers, this code allows for simulating the position of the head on a 2x2 grid of LEDs, as well as
 * printing the position that would result from running a certain winding algorithm. This not only makes prototyping
 * simpler, but avoids the risk of damaging the motors or their drivers while testing the G-Code algorithm.
 * 
 * Note: To use the simulator, make sure to connect LEDs to pins 4-7 as specified in their macros and use jumper
 * wires to connect ports 37 and 18, 31 and 19, and 25 and 20.
 */

#define TURN_RATE 800
#define MM_RATE 160
#define LED_CHANGE_RATE 8

static void MotorSimulator(void* pvParameters)
{
  int simulatorPosition[3] = {0, 0, 0};
  int simulatorPositionMM[3] = {0, 0, 0};
  unsigned int pulsesToNext[3] = {LED_CHANGE_RATE, LED_CHANGE_RATE, LED_CHANGE_RATE}; // Used to keep track of when to "move" the head on the LEDs
  unsigned int pulsesToPrev[3] = {LED_CHANGE_RATE, LED_CHANGE_RATE, LED_CHANGE_RATE};
  boolean positionWasUpdated = false;


  digitalWrite(X0_Y0, HIGH);
  digitalWrite(X1_Y0, LOW);
  digitalWrite(X0_Y1, LOW);
  digitalWrite(X1_Y1, LOW);

  while (1)
  {
    enum {X_FORWARD, X_REVERSE, Y_FORWARD, Y_REVERSE, Z_FORWARD, Z_REVERSE} receivedMotorMessage;
    if (xMessageBufferReceive(xMessageBufferM, &receivedMotorMessage, sizeof(receivedMotorMessage), portMAX_DELAY) > 0)
    {
      task = 5;
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
          if (pulsesToPrev[X] <= 0 && simulatorPosition[X] > 0)
          {
             positionWasUpdated = true;
             simulatorPosition[X]--;
             pulsesToNext[X] = LED_CHANGE_RATE;
             pulsesToPrev[X] = LED_CHANGE_RATE;
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
          if (pulsesToPrev[Y] <= 0 && simulatorPosition[Y] > 0)
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
          if (pulsesToPrev[Z] <= 0 && simulatorPosition[Z] > 0)
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

      if (positionWasUpdated)
      {
        // Calculate the simulator's position in millimeters and print it

        Serial.print("Position:  X: ");
        Serial.print(simulatorPosition[X]/20.0);
        Serial.print(", Y: ");
        Serial.print(simulatorPosition[Y]/20.0);
        Serial.print(", Z: ");
        Serial.println(simulatorPosition[Z]/20.0);
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
