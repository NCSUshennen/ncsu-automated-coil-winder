/**
 * MotorSimulator
 * 
 * Dan Hayduk
 * November 18, 2019
 * 
 * This task is used to help debug the algorithm to read G-Code. Rather than attach the Arduino to the actual
 * motor drivers, this code allows for simulating the position of the head on a 2x2 grid of LEDs. This not
 * only makes prototyping simpler, but avoids the risk of damaging the motors or their drivers while testing
 * the G-Code algorithm.
 * 
 * Note: To use the simulator, make sure to connect LEDs to pins 4-7 as specified in their macros and use jumper
 * wires to connect ports 23 and 18, 31 and 19, and 39 and 20.
 */

static void MotorSimulator(void* pvParameters)
{
  int simulatorPosition[3] = {0, 0, 0};
  unsigned int pulsesToNext[3] = {51200, 51200, 51200}; // Used to keep track of when to "move" the head on the LEDs
  unsigned int pulsesToPrev[3] = {51200, 51200, 51200};

  digitalWrite(X0_Y0, HIGH);
  digitalWrite(X1_Y0, LOW);
  digitalWrite(X0_Y1, LOW);
  digitalWrite(X1_Y1, LOW);

  while (1)
  {
    enum {X_FORWARD, X_REVERSE, Y_FORWARD, Y_REVERSE, Z_FORWARD, Z_REVERSE} receivedMotorMessage;
    if (xMessageBufferReceive(xMessageBufferM, &receivedMotorMessage, sizeof(receivedMotorMessage), portMAX_DELAY) > 0)
    {
      switch(receivedMotorMessage)
      {
        case X_FORWARD:
          pulsesToNext[X]--;
          pulsesToPrev[X]++;
          if (pulsesToNext[X] <= 0)
          {
             simulatorPosition[X]++;
             pulsesToNext[X] = 51200;
             pulsesToPrev[X] = 51200;
          }
          break;
        case X_REVERSE:
          pulsesToNext[X]++;
          pulsesToPrev[X]--;
          if (pulsesToPrev[X] <= 0 && simulatorPosition[X] > 0)
          {
             simulatorPosition[X]--;
             pulsesToNext[X] = 51200;
             pulsesToPrev[X] = 51200;
          }
          break;
        case Y_FORWARD:
          pulsesToNext[Y]--;
          pulsesToPrev[Y]++;
          if (pulsesToNext[Y] <= 0)
          {
             simulatorPosition[Y]++;
             pulsesToNext[Y] = 51200;
             pulsesToPrev[Y] = 51200;
          }
          break;
        case Y_REVERSE:
          pulsesToNext[Y]++;
          pulsesToPrev[Y]--;
          if (pulsesToPrev[Y] <= 0 && simulatorPosition[Y] > 0)
          {
             simulatorPosition[Y]--;
             pulsesToNext[Y] = 51200;
             pulsesToPrev[Y] = 51200;
          }
          break;
        case Z_FORWARD:
          pulsesToNext[Z]--;
          pulsesToPrev[Z]++;
          if (pulsesToNext[Z] <= 0)
          {
             simulatorPosition[Z]++;
             pulsesToNext[Z] = 51200;
             pulsesToPrev[Z] = 51200;
          }
          break;
        case Z_REVERSE:
          pulsesToNext[Z]++;
          pulsesToPrev[Z]--;
          if (pulsesToPrev[Z] <= 0 && simulatorPosition[Z] > 0)
          {
             simulatorPosition[Z]--;
             pulsesToNext[Z] = 51200;
             pulsesToPrev[Z] = 51200;
          }
          break;
        default:
          break;
      }

      if (simulatorPosition[X] == 0 && simulatorPosition[Y] == 0 && simulatorPosition[Z] == 0)
      {
        // Light the 0, 0 LED
        digitalWrite(X0_Y0, HIGH);
        digitalWrite(X1_Y0, LOW);
        digitalWrite(X0_Y1, LOW);
        digitalWrite(X1_Y1, LOW); 
      }
      else if (simulatorPosition[X] == 1 && simulatorPosition[Y] == 0 && simulatorPosition[Z] == 0)
      {
        // Light the 1, 0 LED
        digitalWrite(X0_Y0, LOW);
        digitalWrite(X1_Y0, HIGH);
        digitalWrite(X0_Y1, LOW);
        digitalWrite(X1_Y1, LOW); 
      }
      else if (simulatorPosition[X] == 0 && simulatorPosition[Y] == 1 && simulatorPosition[Z] == 0)
      {
        // Light the 0, 1 LED
        digitalWrite(X0_Y0, LOW);
        digitalWrite(X1_Y0, LOW);
        digitalWrite(X0_Y1, HIGH);
        digitalWrite(X1_Y1, LOW); 
      }
      else if (simulatorPosition[X] == 1 && simulatorPosition[Y] == 1 && simulatorPosition[Z] == 0)
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