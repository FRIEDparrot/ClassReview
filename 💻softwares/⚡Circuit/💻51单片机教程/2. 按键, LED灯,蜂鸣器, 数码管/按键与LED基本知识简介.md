# 独立按键基本原理
在独立按键模块中, 接到了**P30(RXD), P31(TXD), P32, P33**上。

参考[[💻softwares/⚡Circuit/⚛️Proteus/仿真教程/1.proteus界面简介和基本操作#二、接线原理|接线原理]], 输入口和输出口默认是高电平的, **将按键按下相当于将口接地置于低电平**。

寄存器可以实现读取高低电平的原理。
![[attachments/Pasted image 20231203115224.png|700]]

下面的代码定义的是**位寄存器** 
```c
/*------------------------------------------------
P2 Bit Registers
------------------------------------------------*/
sbit P2_0 = 0xA0;
sbit P2_1 = 0xA1;
sbit P2_2 = 0xA2;
sbit P2_3 = 0xA3;
sbit P2_4 = 0xA4;
sbit P2_5 = 0xA5;
sbit P2_6 = 0xA6;
sbit P2_7 = 0xA7;
```
**操作上述的数据**(8位数)只给0,1数据, 即可以**实现操作端口的高低电平状态**,
当整体传入一个P2时, 需要给定0xFF等等, 而仅操作一端口只需0,1, 例如`P2_0  = 0`操作P2为低电平。

LED的亮度和电流有关,<mark style="background: transparent; color: red"> 一般的电流为3mA-20MA</mark>, 200$\Omega$ 
对于直插式二极管, **长脚为阳极, 短阴极(负极)** 
或者内部大的一块是阴极, 小的是阳极， 彩色标记通常阴极
![[Excalidraw/按键的基本原理 2023-12-07 21.12.02]]

电阻大小的选择: 由于发光二极管为1.7V, 则V_LED = 3.3 V, 此时视需要的电流选取电阻。
![[attachments/Pasted image 20231203121756.png|600]]

因此只需要添加检测代码
```c
	while(1){
		if (P3_1 == 0){
			P2_6 = 1;
			P2_7 = 0;
		}
		else{
			P2_6 = 0;
			P2_7 = 1;
		}
	}
即通过控制按键1控制6,7的亮灭
```

需要注意按键顺序并不相同。

最简单的LED灯左右移动代码如下:
```c
void ShiftLEDleft(){
	bool pre = P2_0;
	P2_0 = P2_1;
	P2_1 = P2_2;
	P2_2 = P2_3;
	P2_3 = P2_4;
	P2_4 = P2_5;
	P2_5 = P2_6;
	P2_6 = P2_7;
	P2_7 = pre;
}

void ShiftLEDright(){
    bool pre = P2_7;
    P2_7 = P2_6;
	P2_6 = P2_5;
	P2_5 = P2_4;
	P2_4 = P2_3;
	P2_3 = P2_2;
	P2_2 = P2_1; 
	P2_1 = P2_0;
	P2_0 = pre;
}
```

# 按钮移位操作 
51单片机中使用类型sbit (single bit )来进行按钮的设置
一般点一次操作一次的方法如下
注意需要先Delay(10)来防抖
```c
sbit btn2 = P3^0;
if (!btn2) { // add the number when the button is pressed
			Delay(10);
			if (!btn2){
				num = 0; // return the num to zero 
				while (!btn2); // then do nothing until next press 
			}
		}
```

如果需要进行计时, 则可以使用如下的代码<mark style="background: transparent; color: red">检测按键的长按</mark>
```c
sbit btn1 = P3^1;
if (!btn1) {
			Delay(10);
			if(!btn1){
				unsigned int xms = 0;
					while(!btn1){  // count in the range 5000 
						Delay(1); 
						if (xms< 5000) xms ++;
					}
					if (xms < 600) {
						if (num < 255) num++;
					}
					else{
						if(num > 0) num--;
					}
					while(!btn1); // then do nothing until next press 
			}
		}
```


另外写一个用于循环移位的函数, 即将目前的数字算出移位之后的十进制数, 注意enum的用法: 
```c
enum Dir{
	left,
	right
};

int ShiftNum(int num, enum Dir d){
	int n = num;
	int n0, n1, n2, n3, n4, n5, n6, n7;
	int res = 0;

	n0 = n % 2;  n = n / 2;
	n1 = n % 2;  n = n / 2;
	n2 = n % 2;  n = n / 2;
	n3 = n % 2;  n = n / 2;
	n4 = n % 2;  n = n / 2;
	n5 = n % 2;  n = n / 2;
	n6 = n % 2;  n = n / 2;
	n7 = n % 2;  n = n / 2;

	if (d == left){
		res = n7 + n0 * 2 + n1 * 4 + n2 * 8 + n3 * 16 + n4 * 32 + n5 * 64 + n6 * 128;
	}
	else if (d == right){
		res = n1 + n2 * 2 + n3 * 4 + n4 * 8 + n5 * 16 + n6 * 32 + n7 * 64 + n0 * 128;
	}
	return res;
}

```


另外也可以使用功能LEDNum和循环移位的功能
也可以使用基本的量 0x01
```c
unsigned char LEDNum; 

... LEDNum++;  

0000 0001 = 0x01 << 0
0000 0010 = 0x01 << 1
0000 0100 = 0x01 << 2
// 相应地, 可以使用 0xFE点亮一个灯并让其左右移动
// 也可以
P2 = ~(0x01 << LEDNum)
```

# 防抖动的原因
由于当按钮按下时会有抖动的情况， 抖动过程之后才能得到稳定的低电平状态, 因此<mark style="background: transparent; color: red">为了防止抖动, </mark>可能会由于抖动导致多次触发, 且往往抖动是5-10ms的范围。


![[attachments/Pasted image 20231206013728.png|500]]


在独立按键中, P32 和 P33也可以作为**外部的中断按键** 
![[attachments/Pasted image 20231206014356.png]]


# 按键检测封装函数及其优化

```c
#include <regx52.h>

typedef unsigned char u8;
typedef unsigned int u16;
// 定义控制管脚 
sbit Key1 = P3^1 ; // 位定义
sbit Key2 = P3^0; 
sbit Key3 = P3^2; 
sbit Key4 = P3^3; 

// 定义按键按下的值
#define KEY1_PRESS 1
#define KEY2_PRESS 2
#define KEY3_PRESS 3
#define KEY4_PRESS  4
#define KEY_UNPRESS 5

// 检测按键的按一次状态时, 使用static关键字可以在局部函数退出之后，不销毁该变量的值
static u8 key = 1;
 
// 返回确定按下的按键返回值 
u8 key_scan(){
	if (Key1 == 0|| Key2 == 0 || Key 3 == 0 || K4 == 0 ){	
		// 消抖处理 ** 在消抖处理处可以进行优化, 
		delay(10);
		if (Key1 == 0) return KEY1_PRESS; 
		else if (Key2 == 0) return KEY2_PRESS;
		else if (Key3 == 0) return KEY3_PRESS;
		else if (Key 4 == 0) return KEY4_PRESS;
		else if (Key1 && Key2 && Key3 && Key4) return KEY_UNPRESS;
	}
// 如何进行优化: 四个按键同时判断消抖处理 
// 注意: 这个仅检测单次按键 
}

```

# 实现LED左右移动的自带库函数
首先给出10us, 11.0592MHz的延时函数。
```c 
void Delay10us(void)	//@11.0592MHz
{
	unsigned char data i;
	i = 2;
	while (--i); // 先自减
}

// 也可以使用如下的函数
typedef unsigned int u16;
void Delay10us(u16 x10us){
	while (x10us--);
}
```


在intrins. h中, 其代码如下: 
第一个参数是端口, 第二个是位数
```c
/*--------------------------------------------------------------------------
INTRINS.H

Intrinsic functions for C51.
Copyright (c) 1988-2010 Keil Elektronik GmbH and ARM Germany GmbH
All rights reserved.
--------------------------------------------------------------------------*/

#ifndef __INTRINS_H__
#define __INTRINS_H__

#pragma SAVE

#if defined (__CX2__)
#pragma FUNCTIONS(STATIC)
/* intrinsic functions are reentrant, but need static attribute */
#endif

extern void          _nop_     (void);
extern bit           _testbit_ (bit);
extern unsigned char _cror_    (unsigned char, unsigned char); // 实现LED右移 
extern unsigned int  _iror_    (unsigned int,  unsigned char);      
extern unsigned long _lror_    (unsigned long, unsigned char);  // 实现LED左移
extern unsigned char _crol_    (unsigned char, unsigned char);  
extern unsigned int  _irol_    (unsigned int,  unsigned char);
extern unsigned long _lrol_    (unsigned long, unsigned char);
extern unsigned char _chkfloat_(float);
#if defined (__CX2__)
extern int           abs       (int);
extern void          _illop_   (void);
#endif
#if !defined (__CX2__)
extern void          _push_    (unsigned char _sfr);
extern void          _pop_     (unsigned char _sfr);
#endif

#pragma RESTORE

#endif
```

```c 
LED_PORT = _cror_(LED_PORT, 1);
```

库函数中的左移和右移, 

```c
void Delay(unsigned int xms) // @11.0592MHz 
{
	while (xms > 0){
		unsigned char data i, j;
		_nop_();
		for (i = 2; i>0;i--){
			for (j = 199; j> 0; j--);
		}
		xms--;
	}
}
```

# 控制端口的二进制码 

| 数字 | 二进制 |
| ---- | ------ |
| 0    | 0000   |
| 1    | 0001   |
| 2    | 0010   |
| 3    | 0011   |
| 4    | 0100   |
| 5    | 0101   |
| 6    | 0110   |
| 7    | 0111   |
| 8    | 1000   |
| 9    | 1001   |
| A    | 1010   |
| B    | 1011   |
| C    | 1100   |
| D    | 1101   |
| E    | 1110   |
| F    | 1111   |


