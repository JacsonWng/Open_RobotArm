#ifndef SERIAL_H_
#define SERIAL_H_


//目标物体坐标
struct PostionPoint
{
  float xmm;
  float ymm;
};

extern double X_Position;
extern double Y_Position;
extern float Position_Num;

void PointRecive(void);//接收openmv发送坐标
float getX_Position(void);
float getY_Position(void); 
float getPosition_Num(void);
void SendCmd(char cmd,int myFlag);
void clear_buffer(void);

#endif
