#include "serial.h"
#include "robotGeometry.h"
#include "math.h"
#include <Arduino.h>

float X_Position = 0;
float Y_Position = 0;
float Position_Num = 0;

void PointRecive(void)
{
  int StrLength = 0;
  int num1 = 0;      //X坐标位数                             
  int num2 = 0;      //Y坐标位数 

  if (Serial.available())// 当串口有接收到信息后
  {                      
    Serial.println("Serial Data Available..."); // 通过串口监视器通知用户

    String positionData = Serial.readString();    // 将接收到的信息使用readString()存储于positionData变量
    Serial.print("Received Serial Data: ");     // 然后通过串口监视器输出serialData变量内容
    Serial.println(positionData);                 // 以便查看serialData变量的信息

   StrLength = positionData.length();            //获取字符串长度
   //Serial.print(StrLength);

    for(int i = 1; i < StrLength; i++)          //求得坐标是几位数
    {
      if(positionData[i] == ',')
      {
        num1 = i - 1;
      }
      if(positionData[i] == ']')
      {
        num2 = i - num1 - 3;//2
      }
    }
    
    //求得X坐标
    if(num1 == 1)
    {
      X_Position = int(positionData[num1]) - 48;
    }
    if(num1 == 2)      
    {
      X_Position = (int(positionData[num1]) - 48) + (int(positionData[num1 - 1]) - 48) * 10;
    }
    if(num1 == 3)
    {
      X_Position = (int(positionData[num1]) - 48) + (int(positionData[num1 - 1]) - 48) * 10 + (int(positionData[num1 - 2]) - 48) * 100;
    }

    //求得Y坐标
    if(num2 == 1)
    {
      Y_Position = int(positionData[StrLength - 2]) - 48;
    }
    if(num2 == 2)      
    {
      Y_Position = (int(positionData[StrLength - 2]) - 48) + (int(positionData[StrLength - 3]) - 48) * 10;
    }
    if(num2 == 3)
    {
      Y_Position = (int(positionData[StrLength - 2]) - 48) + (int(positionData[StrLength - 3]) - 48) * 10 + (int(positionData[StrLength - 4]) - 48) * 100;
    }

    Serial.print("(");
    Serial.print(X_Position);
    Serial.print(",");
    Serial.print(Y_Position);
    Serial.print(")");

    //接收多少个坐标数据
    if(Position_Num <= 6)
    {
      Position_Num++;
    }else
    {
      Position_Num = 0;
    }

    //清理串口缓存
    clear_buffer();
  }
} 

//返回X坐标
float getX_Position(void)
{
  //求解末端关节运动X方向坐标
  float sin_theta;
  float xPosition;
  sin_theta = X_Position / sqrt(X_Position*X_Position + Y_Position*Y_Position);
  xPosition = X_Position - 58 * sin_theta;//55为关末端关节到气泵嘴的中心距离
  return xPosition;
}

//返回Y坐标
float getY_Position(void)
{
  //求解末端关节运动Y方向坐标
  float cos_theta;
  float yPosition;
  cos_theta = Y_Position / sqrt(X_Position*X_Position + Y_Position*Y_Position);
  yPosition = Y_Position - 58 * cos_theta;//55为关末端关节到气泵嘴的中心距离
  return yPosition;
}

float getPosition_Num(void)
{
  return Position_Num;
}

void SendCmd()
{
    Serial.write("DONE");
}

void clear_buffer(void)
{
  while(Serial.read() >= 0){};
}
