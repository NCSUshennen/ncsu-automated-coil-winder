/**
 * TaskManualTurn
 * 
 * Dan Hayduk
 * December 2, 2019
 * 
 * Used for manually turning a given motor a given amount of turns in a given direction. For prototyping, both with the motor simulator and the actual frame.
 */

#define TURN_RATE 800

static void MyTaskManualTurn(void* pvParameters)
{
  while(1)
  {
    uint8_t receivedManualTurnDirection;
    unsigned long int i; // 4294967295 MAX
    if (xMessageBufferReceive(xMessageBufferManualTurnDirection, &receivedManualTurnDirection, sizeof(receivedManualTurnDirection), portMAX_DELAY) > 0)
    {
      task = 5;
      switch (receivedManualTurnDirection)
      {
        case 0:
          vTaskDelay(2/portTICK_PERIOD_MS);
      
          // Spin 800 steps in the X direction
          for (i=0; i<TURN_RATE; i++)
          {
            digitalWrite(MOTOR_X1_PLS, HIGH);
            digitalWrite(MOTOR_X2_PLS, HIGH);
            vTaskDelay(1/portTICK_PERIOD_MS/4);
            digitalWrite(MOTOR_X1_PLS, LOW);
            digitalWrite(MOTOR_X2_PLS, LOW);
            vTaskDelay(1/portTICK_PERIOD_MS/4);
          }
      
          // Disable motors
          vTaskDelay(1/portTICK_PERIOD_MS);
          digitalWrite(MOTOR_X1_DIR, LOW);
          digitalWrite(MOTOR_X2_DIR, LOW);
          vTaskDelay(1/portTICK_PERIOD_MS);
          break;
        case 1:
          vTaskDelay(2/portTICK_PERIOD_MS);
      
          // Spin 800 steps in the Y direction
          for (i=0; i<TURN_RATE; i++)
          {
            digitalWrite(MOTOR_Y_PLS, HIGH);
            vTaskDelay(1/portTICK_PERIOD_MS/4);
            digitalWrite(MOTOR_Y_PLS, LOW);
            vTaskDelay(1/portTICK_PERIOD_MS/4);
          }
      
          // Disable motors
          vTaskDelay(1/portTICK_PERIOD_MS);
          digitalWrite(MOTOR_Y_DIR, LOW);
          vTaskDelay(1/portTICK_PERIOD_MS);
          break;
        case 2:
          vTaskDelay(2/portTICK_PERIOD_MS);
      
          // Spin 800 steps in the Z direction
          for (i=0; i<TURN_RATE; i++)
          {
            digitalWrite(MOTOR_Z_PLS, HIGH);
            vTaskDelay(1/portTICK_PERIOD_MS/4);
            digitalWrite(MOTOR_Z_PLS, LOW);
            vTaskDelay(1/portTICK_PERIOD_MS/4);
          }
      
          // Disable motors
          vTaskDelay(1/portTICK_PERIOD_MS);
          digitalWrite(MOTOR_Z_DIR, LOW);
          vTaskDelay(1/portTICK_PERIOD_MS);
          break;
        case 3:
          vTaskDelay(1/portTICK_PERIOD_MS);
          digitalWrite(MOTOR_X1_DIR, HIGH);
          digitalWrite(MOTOR_X2_DIR, HIGH);
          vTaskDelay(1/portTICK_PERIOD_MS);
          
          // Spin 800 steps in the reverse X direction
          for (i=0; i<TURN_RATE; i++)
          {
            digitalWrite(MOTOR_X1_PLS, HIGH);
            digitalWrite(MOTOR_X2_PLS, HIGH);
            vTaskDelay(1/portTICK_PERIOD_MS/4);
            digitalWrite(MOTOR_X1_PLS, LOW);
            digitalWrite(MOTOR_X2_PLS, LOW);
            vTaskDelay(1/portTICK_PERIOD_MS/4);
          }
      
          // Disable motors
          vTaskDelay(1/portTICK_PERIOD_MS);
          digitalWrite(MOTOR_X1_DIR, LOW);
          digitalWrite(MOTOR_X2_DIR, LOW);
          vTaskDelay(1/portTICK_PERIOD_MS);
          break;
        case 4:
          vTaskDelay(1/portTICK_PERIOD_MS);
          digitalWrite(MOTOR_Y_DIR, HIGH);
          vTaskDelay(1/portTICK_PERIOD_MS);
          
          // Spin 800 steps in the reverse Y direction
          for (i=0; i<TURN_RATE; i++)
          {
            digitalWrite(MOTOR_Y_PLS, HIGH);
            vTaskDelay(1/portTICK_PERIOD_MS/4);
            digitalWrite(MOTOR_Y_PLS, LOW);
            vTaskDelay(1/portTICK_PERIOD_MS/4);
          }
      
          // Disable motors
          vTaskDelay(1/portTICK_PERIOD_MS);
          digitalWrite(MOTOR_Y_DIR, LOW);
          vTaskDelay(1/portTICK_PERIOD_MS);
          break;
        case 5:
          vTaskDelay(1/portTICK_PERIOD_MS);
          digitalWrite(MOTOR_Z_DIR, HIGH);
          vTaskDelay(1/portTICK_PERIOD_MS);
          
          // Spin 800 steps in the reverse Z direction
          for (i=0; i<TURN_RATE; i++)
          {
            digitalWrite(MOTOR_Z_PLS, HIGH);
            vTaskDelay(1/portTICK_PERIOD_MS/4);
            digitalWrite(MOTOR_Z_PLS, LOW);
            vTaskDelay(1/portTICK_PERIOD_MS/4);
          }
      
          // Disable motors
          vTaskDelay(1/portTICK_PERIOD_MS);
          digitalWrite(MOTOR_Z_DIR, LOW);
          vTaskDelay(1/portTICK_PERIOD_MS);
          break;
        default:
          break;  
      }
    }
    
    /*if (xSemaphoreTake(xSemaphoreManualTurnF, portMAX_DELAY) == pdTRUE)
    {
      task = 5;
      
      vTaskDelay(2/portTICK_PERIOD_MS);
      
      // Spin 800 steps in the X and Y directions
      unsigned long int i; // 4294967295 MAX
      for (i=0; i<800; i++)
      {
        digitalWrite(MOTOR_X1_PLS, HIGH);
        digitalWrite(MOTOR_X2_PLS, HIGH);
        vTaskDelay(1/portTICK_PERIOD_MS/4);
        digitalWrite(MOTOR_X1_PLS, LOW);
        digitalWrite(MOTOR_X2_PLS, LOW);
        vTaskDelay(1/portTICK_PERIOD_MS/4);
        //Serial.println(i);
      }
      
      // Disable motors
      vTaskDelay(1/portTICK_PERIOD_MS);
      digitalWrite(MOTOR_X1_DIR, LOW);
      digitalWrite(MOTOR_X2_DIR, LOW);
      vTaskDelay(1/portTICK_PERIOD_MS);
    }*/
  }
}
