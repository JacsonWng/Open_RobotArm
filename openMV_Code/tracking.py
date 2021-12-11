# Blob Detection Example
#
# This example shows off how to use the find_blobs function to find color
# blobs in the image. This example in particular looks for dark green objects.

import sensor, image, time
from pyb import UART

# For color tracking to work really well you should ideally be in a very, very,
# very, controlled enviroment where the lighting is constant...
red_threshold = (24, 100, 40, 127, -128, 127)
gree_threshold = (59, 100, -44, -23, 20, 127)
blue_threshold = (24, 84, -48, 44, -73, -35)
yellow_threshold = (58, 100, -38, 127, 43, 127)
orange_thershold = (49, 100, 26, 127, 7, 127)
purple_threshole = (27, 43, 6, 26, -32, -13))

thresholds = [red_threshold,gree_threshold,blue_threshold,yellow_threshold,orange_thershold,purple_threshole]
#设置不同颜色的阈值，括号里面的数值分别是L A B 的最大值和最小值（minL, maxL, minA,
# maxA, minB, maxB），LAB的值在图像左侧三个坐标图中选取。如果是灰度图，则只需
#设置（min, max）两个数字即可。

#图像初始化
sensor.reset() # Initialize the camera sensor.
sensor.set_pixformat(sensor.RGB565) # use RGB565.
sensor.set_framesize(sensor.QVGA) # use VGA for speed.
sensor.skip_frames(10) # Let new settings take affect.
sensor.set_auto_whitebal(False)#关闭白平衡。白平衡是默认开启的，在颜色识别中，需要关闭白平衡

#串口初始化
uart = UART(3, 9600)#打开串口
uart.init(9600, bits=8, parity=None, stop=1)  #8位数据位，无校验位，1位停止位

clock = time.clock() # Tracks FPS.
myFlag = 0
posNum = 0      #发送的坐标个数

#颜色物料块的坐标二维列表
cubePosition = [[0 for x in range(2)] for y in range(6)]
#print(cubePosition)
#time.sleep_ms(3000)#延时等待机械臂准备就绪

while(True):
    clock.tick() # Track elapsed milliseconds between snapshots().
    img = sensor.snapshot()#.lens_corr(strength = 1.8, zoom = 1.0) # Take a picture and return the image.
    img.lens_corr(1.8)  #画面矫正
    img.draw_rectangle(58,60,178,132)  #框选一个规定区域

    #确认机械臂发送命令
    if uart.any():#串口有数据发过来
        cmd = uart.readline().decode().strip()#把接收的数据变为字符串
        if cmd == "START":
            print(cmd)
            myFlag = 1
            #print(myFlag)

        if cmd == "DONE":#上一个坐标物块移动完毕
            if posNum == 2:#第二个坐标
                p2 = str(cubePosition[1])
                uart.write(p2)
            if posNum == 3:#第三个坐标
                p3 = str(cubePosition[2])
                uart.write(p3)
            if posNum == 4:#第四个坐标
                p4 = str(cubePosition[3])
                uart.write(p4)
            if posNum == 5:#第五个坐标
                p5 = str(cubePosition[4])
                uart.write(p5)
            if posNum == 6:#第六个坐标
                p6 = str(cubePosition[5])
                uart.write(p6)
            posNum = posNum + 1 #下一个坐标


    #寻找六个色块
    if myFlag == 1:
        for i in range(0,6):
            #  pixels_threshold=100, area_threshold=100
            blobs = img.find_blobs([thresholds[i]], pixels_threshold=100, area_threshold=100)#200

            if blobs:
            #如果找到了目标颜色
            #print(blobs)
                for b in blobs:
                #迭代找到的目标颜色区域
                     # Draw a rect around the blob.
                    img.draw_rectangle(b.rect()) # rect,b[0:4]
                    #用矩形标记出目标颜色区域
                    img.draw_cross(b.cx(),b.cy()) # cx, cy
                    #在目标颜色区域的中心画十字形标记
                    cy = b.cx() - 58       #跟我机械臂坐标方向相同
                    cx = b.cy() - 60
                    #实际世界坐标系地址
                    bx = (cx/132.0) * 300
                    by = (cy/178.0) * 400
                #映射到机械臂坐标（300x400mm的板子）
                cubePosition[i][0] = int(bx - 58)
                cubePosition[i][1] = int(by - 53)
                print("(%d,%d)"%(cx,cy))
                print('\n')
            time.sleep_ms(200)#延时0.2秒
        myFlag = 0
        posNum = 1
        print(str(cubePosition),'\n')
        for k in range(6):
            print(str(cubePosition[k]))

    #发送第一个坐标
    if posNum == 1:
        p1 = str(cubePosition[0])   #首先发送第一个坐标
        #img_data1 = bytearray([0x2C,0x12,130,120])   #必须传输一个字节数组
        #uart.write(img_data1)
        uart.write(p1)
        posNum = posNum + 1







    #print(clock.fps()) # Note: Your OpenMV Cam runs about half as fast while
    # connected to your computer. The FPS should increase once disconnected.
