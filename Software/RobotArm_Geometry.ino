//date:2021.10.23
//author:wang xiao
//MCU:arduino nano 
#include "math.h"

//底座电机引脚
#define dirPin1 2  //方向引脚
#define stepPin1 3  //步进引脚
//步进步数
#define MS1_1 4       //选择电机驱动模式
#define MS2_1 5
#define MS3_1 6

//大臂电机引脚
#define dirPin2 7  //方向引脚
#define stepPin2 8  //步进引脚
#define MS1_2 9       //选择电机驱动模式
#define MS2_2 10
#define MS3_2 11

//小臂电机引脚
#define dirPin3 12  //方向引脚
#define stepPin3 13  //步进引脚

//传动比9:32
double gearRate = 32.0 / 9.0;

//标志位
unsigned int flag = 1;

/*机械臂正运动学部分*/
double rside;           //rside = sqrt(xmm*xmm + ymm*ymm)为坐标(xmm, ymm)的直角边的平方

//各个关节旋转角度
//double angle1;
double angle2;
double angle3;

//几何三角形角度
double low;
double high;


void setup() {
  // put your setup code here, to run once:
  //打开串口
  Serial.begin(115200);

  //buttom motor
  pinMode(dirPin1, OUTPUT);
  pinMode(stepPin1, OUTPUT);
  //驱动模式选择
  pinMode(MS1_1, OUTPUT);
  pinMode(MS2_1, OUTPUT);
  pinMode(MS3_1, OUTPUT);
  //sixteenth step 十六分之一步进
  digitalWrite(MS1_1, HIGH);
  digitalWrite(MS2_1, HIGH);
  digitalWrite(MS3_1, HIGH);

  //big arm motor
  pinMode(dirPin2, OUTPUT);
  pinMode(stepPin2, OUTPUT);
  //驱动模式选择
  pinMode(MS1_2, OUTPUT);
  pinMode(MS2_2, OUTPUT);
  pinMode(MS3_2, OUTPUT);
  //sixteenth step 十六分之一步进
  digitalWrite(MS1_2, HIGH);
  digitalWrite(MS2_2, HIGH);
  digitalWrite(MS3_2, HIGH);

  //small arm motor
  pinMode(dirPin3, OUTPUT);
  pinMode(stepPin3, OUTPUT);
  //驱动模式选择
  //  pinMode(MS1_3, OUTPUT);
  //  pinMode(MS2_3, OUTPUT);
  //  pinMode(MS3_3, OUTPUT);
  //  //sixteenth step 十六分之一步进
  //  digitalWrite(MS1_2, HIGH);
  //  digitalWrite(MS2_2, HIGH);
  //  digitalWrite(MS3_2, HIGH);
}

//底座电机控制程序
void ButtomMotor_con(bool dir, float angle1, unsigned int Ht, unsigned int Lt)
{
  digitalWrite(dirPin1, dir); //设置逆时针转动

  //电机慢速转动
  for (int x = 0; x < (angle1 / 360.0)*gearRate * 3200; x++)
  {
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(Ht);
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(Lt);
  }
}

//大臂电机控制程序
void BigArmMotor_con(bool dir, float angle2, unsigned int Ht, unsigned int Lt)
{
  digitalWrite(dirPin2, dir); //设置逆时针转动

  //电机慢速转动
  for (int x = 0; x < (angle2 / 360.0)*gearRate * 3200; x++)
  {
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(Ht);
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(Lt);
  }
}

//小臂电机控制程序
void SmallArmMotor_con(bool dir, float angle3, unsigned int Ht, unsigned int Lt)
{
  digitalWrite(dirPin3, dir); //设置逆时针转动

  //电机慢速转动
  for (int x = 0; x < (angle3 / 360.0)*gearRate * 3200; x++)
  {
    digitalWrite(stepPin3, HIGH);
    delayMicroseconds(Ht);
    digitalWrite(stepPin3, LOW);
    delayMicroseconds(Lt);
  }
}

//底座电机和大臂电机一起运动
void MoveTogether(bool dir, unsigned angle_all, unsigned int Ht, unsigned int Lt)
{
  for (int x = 0; x < (angle_all / 360.0)*gearRate * 3200; x++)
  {
    //底座电机
    digitalWrite(dirPin1, dir);
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(Ht);
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(Lt);

    //大臂电机
    digitalWrite(dirPin2, dir);
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(Ht);
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(Lt);
  }
}

/*机械臂正运动学____机械臂初始状态为：底座随意角度，大臂处于竖直位置，小臂处于水平位置*/
void Arm_Geometry(double angle1, double xmm, double ymm) //输入末端坐标，计算旋转角度
{
  //计算几何三角形的边长
  rside = sqrt(xmm * xmm + ymm * ymm);
  Serial.print("rside=");
  Serial.print(rside);
  Serial.print("\n");
  Serial.print("angle1=");
  Serial.print(angle1);
  Serial.print("\n");

  high = (asin((0.5 * rside) / 120.0)) * 2.0;           //等边三角形的底对角弧度值

  //计算大臂旋转角度
  if (ymm > 0)
  {
    low = (PI / 2.0) - acos(xmm / rside) - ((PI - high) / 2.0); //计算大臂转动的弧度值,low可正可负
    angle2 = (low / PI) * 180;                              //弧度和角度的转换，可正可负
    Serial.print("angle2 =");
    Serial.print(angle2);
    Serial.print("\n");
  }
  else
  {
    low = (PI / 2.0) + acos(xmm / rside) - ((PI - high) / 2.0);
    angle2 = (low / PI) * 180;
    Serial.print("angle2 =");
    Serial.print(angle2);
    Serial.print("\n");

  }
  //小臂角度
  angle3 = ((PI / 2.0 + low - high) / PI) * 180.0;
  Serial.print("angle3 =");
  Serial.print(angle3);
  Serial.print("\n");
  Serial.print("\n");

  //执行角度
  if (flag == 1)
  {
    if (angle2 >= 0){            //当大臂的旋转角度为正时
      ButtomMotor_con(LOW, angle1, 100, 100);
      SmallArmMotor_con(HIGH, angle3, 200, 200);
      BigArmMotor_con(LOW, angle2, 200, 200);
    } else{                      //当大臂旋转角度为负时
      ButtomMotor_con(LOW, angle1, 100, 100);
      BigArmMotor_con(HIGH, -angle2, 200, 200);
      SmallArmMotor_con(HIGH, angle3, 200, 200);
    }
    
  } else
  {
    if (angle2 >= 0){
      BigArmMotor_con(HIGH, angle2, 200, 200);
      SmallArmMotor_con(LOW, angle3, 200, 200);
      ButtomMotor_con(HIGH, angle1, 100, 100);
    } else{
      SmallArmMotor_con(LOW, angle3, 200, 200);
      BigArmMotor_con(LOW, -angle2, 200, 200);
      ButtomMotor_con(HIGH, angle1, 200, 200);
    }
 
  }
}

void loop() {
  //机械臂正运动学
  flag = 1;
  if (flag == 1)
  {
    Arm_Geometry(180, 180, -140);            //底座旋转xx度，运动到(xmm, ymm)坐标处
    delay(2000);
    flag = 0;
    Arm_Geometry(180, 180, -140);
    delay(2000);
  }


/*以下为测试代码*/
  //  MoveTogether(LOW, 90, 100, 200);                        //设置转动方向，旋转角度，脉冲高电平时间，脉冲低电平时间
  //  SmallArmMotor_con(HIGH, 40, 200, 200);
  //  delay(2000);
  //  SmallArmMotor_con(LOW, 40, 200, 200);
  //  MoveTogether(HIGH, 90, 100, 200);
  //  delay(2000);



  //  if(flag == 1)
  //  {
  //    SmallArmMotor_con(LOW, 90, 200, 200);
  //    flag = 2;
  //    delay(3000);
  //    }
  //  else if(flag == 2)
  //  {
  //    SmallArmMotor_con(HIGH, 90, 200, 200);
  //    flag = 1;
  //    delay(3000);
  //    }
}
