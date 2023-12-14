只需要让阳极输出高电平， 阴极输出低电平即可。

74HC595的级联方法: 将上一级的串行输出设置下一个的串行输入，即SER2(QH'接入SER2)
![[attachments/Pasted image 20231212223249.png|900]]
对于A5/A6, 可以通过3个IO口以及两个74Hc595直接控制8行8列

A6和A7的部分是四片595网络的级联
实际上只要3个IO口即可实现全部控制

为了实现类似于动态数码管的动态扫描, 实际上即为循环扫描, 有则点亮对应的灯。


在字模提取软件中, 可以对对应的进行描点。
![[attachments/Pasted image 20231213090705.png|500]]
描点完毕之后进行纵向取模即可。

扫描的函数是COL_PORT_SCAN函数。下面程序显示了数字6 
```c
#include "public.h"
#include <STDLIB.H>  // read files
#define LED_COL_PORT P0;

sbit SH_CLK = P3^6;
sbit ST_ClK = P3^5;
sbit HC_SER_INPUT = P3^4;

void HC595_write(u8 value){
    int i;
    for (i = 0; i < 8; i++){
        HC_SER_INPUT = value >> 7;
        value <<= 1;
        SH_CLK = 0;
        Delay5us();
        SH_CLK = 1;
        Delay5us();
    }
    ST_ClK = 0;
    Delay5us();
    ST_ClK = 1;
}

void HC595_ERASE(){
    HC595_write(0x00);
}

// scan each line while delay 10us
u8 COLPORT_SCAN_ARR[8] = {0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0xf}; // 0111 1111 1101
void HC595_COLPORT_SCAN(u8 Data[8]){
    int i;
    for (i = 0; i< 8; i++){
        HC595_write(Data[i]);
        P0 = COLPORT_SCAN_ARR[i];
        Delay5us();
        P0 = 0xff; // reset the column port
        HC595_ERASE();
        Delay5us();
    }
}


void main(){
    u8 digit[8] = {0x00,0x00,0x3C,0x52,0x52,0x4C,0x00,0x00};  // using the abs software
    while(1){
        P0 = 0xdf;  // 1101 1111;
        HC595_COLPORT_SCAN(digit);
        Delay10us(1);
    }
}

```