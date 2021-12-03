#include "pump.h"
#include <Arduino.h>
#include <Servo.h> 

//创建气泵和电磁阀控制对象
//（Servo myPump）为气泵 （ Servo myValve）为电磁阀  
// 该变量用气泵电磁阀开关（0为关闭，180为开启）
Servo myPump;
Servo myValve;

void pumpSet(void)
{
  // 代表气泵插6引脚，电磁阀插9引脚 arduino控制  
  myPump.attach(6);
  myValve.attach(9);

  myPump.write(0);  
  myValve.write(0);  
}

void controlPump(int flag)
{
  if(flag == 1)
  {
    myPump.write(180);         //打开气泵
    myValve.write(0);          //电磁阀关闭
  }
  else if(flag == 2)
  {
    myPump.write(0);           //关闭气泵
    myValve.write(0);          //电磁阀关闭
  }
  else if(flag == 3)
  {
    myPump.write(0);    
    myValve.write(180);        //电磁阀打开
  }
}
