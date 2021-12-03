#include "setup.h"
#include "pinout.h"
#include <Arduino.h>

/*为机械臂设置零位*/
void ArmToZero(void)
{
  double num1 = 0, num2 = 0;
  double gearRatio = 35 / 9.0;
  double stepNum1 = (47 / 360.0) * gearRatio * 200 * 16; //47为大臂起始位置到垂直位置的角度
  double stepNum2 = (21 / 360.0) * gearRatio * 200 * 16; //21为小臂起始位置到水平直位置的角度

  //大臂归零
  while (num1 < stepNum1)
  {
    digitalWrite(dirPin2, LOW);
    delayMicroseconds(5);
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(200);
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(200);
    num1++;
  }
  
  //小臂归零
  while (num2 < stepNum2)
  {
    digitalWrite(dirPin3, HIGH);
    delayMicroseconds(5);
    digitalWrite(stepPin3, HIGH);
    delayMicroseconds(200);
    digitalWrite(stepPin3, LOW);
    delayMicroseconds(200);
    num2++;
  }

}




