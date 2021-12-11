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
int myFlag3 = 0;
int myFlag4 = 0;
int myFlag5 = 0;
int myFlag6 = 0;

//函数声明
void SetToPosition_1(void);
void SetToPosition_2(void);
void SetToPosition_3(void);
void SetToPosition_4(void);
void SetToPosition_5(void);
void SetToPosition_6(void);

void setup()
{
  //打开串口
  Serial.begin(9600);

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
  //delay(1000);
  Serial.write("START");
}

void loop()
{
  PointRecive();        //获取物块位置坐标
  if(myFlag1 != -1)     //坐标执行
  {
    SetToPosition_1();
  }else if(myFlag2 != -1)
  {
    SetToPosition_2();
  }else if(myFlag3 != -1)
  {
    SetToPosition_3();
  }

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


/*************执行openmv给定的坐标*******************/
void SetToPosition_1(void)      //第一次抓取
{
  if((getPosition_Num() == 1) && (myFlag1 == 0))
  {
    //获取坐标，移动到目标点
    blockPoint1.xmm = getX_Position();
    blockPoint1.ymm = getY_Position();
    interpolator.setInterpolation(0,120,120,0,  blockPoint1.xmm,blockPoint1.ymm,-52,0,  60);    //进行插值
    myFlag1 = 1;
    controlPump(1);   //气泵打开，电磁阀关闭
  }
  //检测上次插值完成，机械臂到达目标点之后向上移动一段
  if((interpolator.isFinished() != 0) && (myFlag1 == 1))
  {
    delay(500);     //延时吸起物体
    controlPump(2); //气泵和电磁阀都关闭
    interpolator.setInterpolation(blockPoint1.xmm,blockPoint1.ymm,-52,0,  blockPoint1.xmm,blockPoint1.ymm,20,0,  60);
    myFlag1 = 2;
  }
  //检测上次插值完成，机械臂运动到放置点
  if((interpolator.isFinished() != 0) && (myFlag1 == 2))
  {
    interpolator.setInterpolation(blockPoint1.xmm,blockPoint1.ymm,20,0,  0,200,-42,0,  50);
    myFlag1 = 3;
  }
  //到达目标点后电磁阀打开
  if((interpolator.isFinished() != 0) && (myFlag1 == 3))
  {
    delay(500);
    controlPump(3);   //电磁阀打开，物体放下
    delay(500);
    interpolator.setInterpolation(0,200,-42,0,  0,200,80,0,  22);
    SendCmd();   //第一次坐标抓取完成
    myFlag1 = -1;     //结束标志位
  }
}

//第二次抓取
void SetToPosition_2(void)    
{
  if(interpolator.isFinished() != 0)//判断上次插值是否完成
  {
    if((getPosition_Num() == 2) && (myFlag2 == 0))
    {
      blockPoint2.xmm = getX_Position();
      blockPoint2.ymm = getY_Position();
      interpolator.setInterpolation(0,200,80,0,  blockPoint2.xmm,blockPoint2.ymm,-52,0,  50);
      myFlag2 = 1;
      controlPump(1);
    }
  }
  //检测上次插值完成，机械臂到达目标点之后向上移动一段
  if((interpolator.isFinished() != 0) && (myFlag2 == 1))
  {
    delay(500);     //延时吸起物体
    controlPump(2); //气泵和电磁阀都关闭
    interpolator.setInterpolation(blockPoint2.xmm,blockPoint2.ymm,-52,0,  blockPoint2.xmm,blockPoint2.ymm,20,0,  60);
    myFlag2 = 2;
  }
  //检测上次插值完成，机械臂运动到放置点
  if((interpolator.isFinished() != 0) && (myFlag2 == 2))
  {
    interpolator.setInterpolation(blockPoint2.xmm,blockPoint2.ymm,20,0,  0,200,-17,0,  50);
    myFlag2 = 3;
  }
  //到达目标点后电磁阀打开
  if((interpolator.isFinished() != 0) && (myFlag2 == 3))
  {
    delay(500);
    controlPump(3);
    delay(500);
    interpolator.setInterpolation(0,200,-17,0,  0,200,80,0,  22);
    SendCmd();   //第二次坐标抓取完成
    myFlag2 = -1;     //结束标志
  }
}

//第三次抓取
void SetToPosition_3(void)
{
  if(interpolator.isFinished() != 0)
  {
    if((getPosition_Num() == 3) && (myFlag3 == 0))
    {
      blockPoint3.xmm = getX_Position();
      blockPoint3.ymm = getY_Position();
      interpolator.setInterpolation(0,200,80,0,  blockPoint3.xmm,blockPoint3.ymm,-52,0,  50);
      myFlag3 = 1;
      controlPump(1);
    }
  }
  //检测上次插值完成，机械臂到达目标点之后向上移动一段
  if((interpolator.isFinished() != 0) && (myFlag3 == 1))
  {
    delay(500);     //延时吸起物体
    controlPump(2); //气泵和电磁阀都关闭
    interpolator.setInterpolation(blockPoint3.xmm,blockPoint3.ymm,-52,0,  blockPoint3.xmm,blockPoint3.ymm,20,0,  60);
    myFlag3 = 2;
  }
  //检测上次插值完成，机械臂运动到放置点
  if((interpolator.isFinished() != 0) && (myFlag3 == 2))
  {
    interpolator.setInterpolation(blockPoint3.xmm,blockPoint3.ymm,20,0,  0,200,8,0,  50);
    myFlag3 = 3;
  }
  //到达目标点后电磁阀打开
  if((interpolator.isFinished() != 0) && (myFlag3 == 3))
  {
    delay(500);
    controlPump(3);
    delay(500);
    interpolator.setInterpolation(0,200,8,0,  0,50,120,0,  22);
    SendCmd();   //第一次坐标抓取完成
    myFlag3 = -1;     //结束标志
  }
}






