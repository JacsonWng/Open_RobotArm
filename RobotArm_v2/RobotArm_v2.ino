#include <Arduino.h>
#include "pinout.h"
#include "mystepper.h"
#include "setup.h"
#include "robotGeometry.h"
#include "interPolation.h"
#include "serial.h"
#include "pump.h"

MyStepper stepperRotate(stepPin1, dirPin1);
MyStepper stepperLower(stepPin2, dirPin2);
MyStepper stepperHigher(stepPin3, dirPin3);

RobotGeometry geometry;     //正运动学
Interpolation interpolator; //插值算法

PostionPoint blockPoint1; //目标物块的平面二维坐标
PostionPoint blockPoint2;
PostionPoint blockPoint3;
PostionPoint blockPoint4;
PostionPoint blockPoint5;

int myFlag1 = 0;
int myFlag2 = 0;

void setup()
{
  //打开串口
  Serial.begin(115200);

  //初始化气泵
  pumpSet();

  //为机械臂设置零位，步进电机驱动模式，16细分
  ArmToZero();

  //reduction of steppers..
  stepperHigher.setReductionRatio(32.0 / 9.0, 200 * 16); //big gear: 32, small gear: 9, steps per rev: 200, microsteps: 16
  stepperLower.setReductionRatio(32.0 / 9.0, 200 * 16);
  stepperRotate.setReductionRatio(32.0 / 9.0, 200 * 16);

  //初始化
  interpolator.setInterpolation(0,120,120,0, 0,120,120,0,  60);//54
  //Serial.print("Start!\n");
}

/*************执行openmv给定的坐标*******************/
void SetToPosition_1(void)      //第一次抓取
{
 if((getPosition_Num() == 1) && (myFlag1 == 0))
  {
    //获取坐标
    blockPoint1.xmm = getX_Position();
    blockPoint1.ymm = getY_Position();
    interpolator.setInterpolation(0,120,120,0,  blockPoint1.xmm,blockPoint1.ymm,-60,0,  60);    //进行插值
    myFlag1 = 1;
    controlPump(1);   //气泵打开，电磁阀关闭
  }
  //检测上次插值完成
  if((interpolator.isFinished() != 0) && (myFlag1 == 1))
  {
    controlPump(2); //气泵和电磁阀都关闭
    interpolator.setInterpolation(blockPoint1.xmm,blockPoint1.ymm,-60,0,  0,210,60,0,  60);
    myFlag1 = 2;
  }
  if((interpolator.isFinished() != 0) && (myFlag1 == 2))
  {
    controlPump(3);   //电磁阀打开，物体放下
    SendCmd('y',myFlag1);   //第一次坐标抓取完成
    myFlag1 = -1;     //结束标志位
  }
}

void SetToPosition_2(void)    //第二次抓取
{
  if((getPosition_Num() == 2) && (myFlag2 == 0))
  {
    blockPoint2.xmm = getX_Position();
    blockPoint2.ymm = getY_Position();
    interpolator.setInterpolation(0,210,60,0,  blockPoint2.xmm,blockPoint2.ymm,-60,0,  60);
    myFlag2 = 1;
    controlPump(1);
  }
  //检测上次插值完成
  if((interpolator.isFinished() != 0) && (myFlag2 == 1))
  {
    controlPump(2); 
    interpolator.setInterpolation(blockPoint2.xmm,blockPoint2.ymm,-60,0,  0,210,60,0,  60);
    myFlag2 = 2;
  }
  if((interpolator.isFinished() != 0) && (myFlag2 == 2))
  {
    controlPump(3);
    SendCmd('y',myFlag2);   //第一次坐标抓取完成
    myFlag2 = -1;     //结束标志
  }
}


void loop()
{
  PointRecive();    //获取物块位置坐标
  SetToPosition_1();
  SetToPosition_2();
  //SetToPosition_3();






  //update and Calculate all Positions, Geometry and Drive all Motors...
  interpolator.updateActualPosition();                                                        //实时更新获取机械臂末端位置坐标
  geometry.set(interpolator.getXPosmm(), interpolator.getYPosmm(), interpolator.getZPosmm()); //根据位置坐标进行关节角度的逆解
  
  //进行角度弧度值对应脉冲数的计算
  stepperRotate.stepToPositionRad(geometry.getRotRad());
  stepperLower.stepToPositionRad(geometry.getLowRad());
  stepperHigher.stepToPositionRad(geometry.getHighRad());
  
  //更新关节角度
  stepperRotate.update(1);
  stepperLower.update(2);
  stepperHigher.update(3);
}







/* void interpolatorTest(void)
{
  //检测上次插值是否完成
  if(interpolator.isFinished() != 0)
  {
    myFlag++;
    if(myFlag == 1)
    {
      interpolator.setInterpolation(0,210,0,0, 120,19.5,120,0,  10);//10
    }
    if(myFlag == 2)
    {
      interpolator.setInterpolation(120,19.5,120,0,  0,210,0,0,  10);
    }
    if(myFlag == 3)
    {
      interpolator.setInterpolation(0,210,0,0,  0,60,0,0,  10);
    }
    if(myFlag == 4)
    {
      interpolator.setInterpolation(0,60,0,0,  0,120,120,0,  10);
    }
    if(myFlag == 5)
    {
      interpolator.setInterpolation(0,120,120,0,  0,210,0,0,  10);
    }
    if(myFlag == 6)
    {
       interpolator.setInterpolation(0,254,0,0,  0,120,120,0,  54);
    }
    if(myFlag == 7)
    {
      interpolator.setInterpolation(0,120,120,0,  0,54,120,0,  54);
    }
    if(myFlag == 8)
    {
      interpolator.setInterpolation(0,54,120,0,  60,120,-80,0,  54);
    }
    if(myFlag == 9)
    {
      interpolator.setInterpolation(60,120,-80,0,  0,120,120,0,  54);
    }
  }

} */
