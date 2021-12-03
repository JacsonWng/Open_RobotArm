#include "serial.h"
#include <Arduino.h>

double X_Position = 0;
double Y_Position = 0;
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
      if(positionData[i] == ')')
      {
        num2 = i - num1 - 2;
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
      Y_Position = int(positionData[StrLength - 3]) - 48;
    }
    if(num2 == 2)      
    {
      Y_Position = (int(positionData[StrLength - 3]) - 48) + (int(positionData[StrLength - 4]) - 48) * 10;
    }
    if(num2 == 3)
    {
      Y_Position = (int(positionData[StrLength - 3]) - 48) + (int(positionData[StrLength - 4]) - 48) * 10 + (int(positionData[StrLength - 5]) - 48) * 100;
    }

    Serial.print("(");
    Serial.print(X_Position);
    Serial.print(",");
    Serial.print(Y_Position);
    Serial.print(")");

    //接收多少个坐标数据
    if(Position_Num <= 5)
    {
      Position_Num++;
    }else
    {
      Position_Num = 0;
    }
  }

  //清理串口缓存
  clear_buffer();
} 

//返回X坐标
float getX_Position(void)
{
  return X_Position;
}

//返回Y坐标
float getY_Position(void)
{
  return Y_Position;
}

float getPosition_Num(void)
{
  return Position_Num;
}

void SendCmd(char cmd,int myFlag)
{
  if(myFlag == 2)
  {
    Serial.write(cmd);
  }
}

void clear_buffer(void)
{
  while(Serial.read() >= 0){};
}
