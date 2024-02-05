# 基础知识与常见指标
参考[[📘ClassNotes/⚡Circuit/电工电子技术/第十章 模拟-数字信号转换器|第十章 模拟-数字信号转换器]]，
对于单片机采集外部的模拟信号， 需要通过AD转换转换成数字量进行表示。数字位数越多，对应的精度越高

ADC常见的三个技术指标有: 
- **分辨率**: 对于<mark style="background: transparent; color: red">允许范围内的模拟信号</mark>，能够输出离散数字信号值的个数。 常常使用bit 表示，且为$2^{x}$, 
  说明: 首先需要知道A/D转换芯片的分辨率, 最终计算出的模拟量的值为: 
$$量程最小值 + \frac{度数值}{分辨率} \times  量程 $$
- 转换误差: 通常是以输出误差的最大形式给出, 表示AD转换器实际输出的电压值相对理论产生的电压值的差。 
- **转换速率**: 是单位时间重复进行数据转换的次数, 即1s内转换的次数。 是转换时间的倒数 

> [!NOTE] 不同类型的AD转换器的转换速率比较
> 并行比较型的转换速率最高, (一般50ns之内)
> 逐次比较型的速率次之(10-50us之内) 
> 间接A/D转换器速度最慢。例如双积分型在几十到几百毫秒之间。 

AD转换其包括采样，保持, 量化， 编码四个步骤。

采样: 将连续变化的模拟量转换为时间上离散的模拟量
保持: 将模拟信号保存起来直到下一时刻
- 采样器和保持电路统称采样-保持电路
量化: 将采样电平规划为与之接近的离散数字电平
编码: 将量化后的结果按照一定的数字形式表示 

# ADC转换简介
A/D转换包括直接比较型和间接比较型。 

其中逐次比较型的ADC应用比较多。间接比较型是指输入的模拟量不是直接与参考电压比较， 而是将某种中间物理量与参考电压比较。常见的有双积分型和积分型ADC 

比较型ADC的转换示意图如下: 
![[attachments/Pasted image 20240108103900.png|400]]
对于ADC, 基本原理是从高位到低位试探比较。 最终将其中的值输出到输出缓冲器进行输出。 


双积分形ADC的转换原理图如下: 
1. 使用电子开关，积分器, 比较器等
![[attachments/Pasted image 20240108103622.png|600]]
将输入电压变换为与其平均值成正比的时间间隔, 并将时间间隔转换为数据量。积分器将其转换为相应的与积分， $V_i$越大, 积分输出的电压也越大，积分时间也越长, 是一个间接转换的过程。 

需要说明的是， 往往使用如下的两种电平划分方式, 而<mark style="background: transparent; color: red">第二种由于代表的模拟电压值为区间中点, 因此量化误差较小</mark>  
![[attachments/picture-filter-1704695952386.jpg|600]]
# A/D 转换基本电路内部结构图
## (1) 采样-保持(S-H)电路
补充: 采样-保持基本电路如图所示,  其中Q1为采样开关 
![[attachments/Pasted image 20240108111134.png|500]]

<mark style="background: transparent; color: red">集成式集成-采样电路LE198电路</mark>的原理图如下: 其中MOS管T1是控制开关,
当VL输入为1时, 两个电压均为跟随状态,即VO = VI , 而当VL = 0时 由于电容的放电作用, VO处的电压有一定的保持时间。

其中电容选用10-20uF保证放电时间充足。 
![[attachments/Pasted image 20240108124036.png|600]]
![[attachments/Pasted image 20240108124412.png|600]]

> [!caution] 说明
> 电容选用在uF量级
> 对于电压跟随器，有一个最低跟随电压值, 并且跟随负电压更为稳定
> 另外如果是交流电压， 需要注意交流电压频率， 防止C6导通
> 
> 需要说明的是, 在T1从高阻态到导通的时间内, 如果VI有变化, 则可能导致前面一个电路的电压变化较大, 从而需要增加$D_{1}, D_{2}$构成保护电路,
> <mark style="background: transparent; color: red">保护原理是在U4保持的电压比U3大或者小一个二极管导通电压</mark>(0.7V)时,  二极管导通, 从而将左侧电压($V_{O}'$, 即第一个电压表测量的电压)限制在$V_I + V_D$以内, 起到保护电路的作用
## (2) A/D转换器 

其中A/D转换并行比较型以及反馈比较型两类。 其中三位并行比较型A/D转换器的原理电路如下图所示: 
![[attachments/picture-filter-1704695442118.jpg|600]]
上图的三位并行比较A/D转换电路由电压比较器, 寄存器和代码转换器三个模块组成。 其中电平的划分使用上述的第二种方式, 

由于<mark style="background: transparent; color: red">实际电压是从V1输入的</mark>, 因此是直接并行比较的, 转换时间主要受到比较器, 触发器和优先编码器限制， 转换速度快。 
但是缺点是分辨率提高时， 需要的元件以几何级数增加
而且在过程中这种电路<mark style="background: transparent; color: red">不需要采样-保持电路
</mark>, 直接利用比较和寄存器的保持功能即可。
![[attachments/picture-filter-1704696519636.jpg|800]]

另外一种是反馈比较型, 可以参考[[📘ClassNotes/⚡Circuit/电工电子技术/第十章 模拟-数字信号转换器#二、A/D 转换器|反馈式AD转换器]] 
![[Excalidraw/第十章 模拟-数字信号转换器 2024-01-02 15.08.37|900]]
优点是在精度增加时, 只需少量的电阻可以达到较大的精度， 但是需要进行采样-保持电路。 

需要说明的是， 还有一种比较原理是计数器比较方法， 但是思路简单且转换时间长。 

> [!cite] 芯片知识
> 常见的并行AD转换器有AD0912, AD9002, AD9020等,  而集成逐次比较型A/D转换器有ADC0804/0808/0809系列, AD575, AD574A 等

## (3) 间接A/D
一般的A/D转换器属于V-T(时间)变换型,  V-F(频率)变换型等， 然后进行定时(实际上是固定频率计数)或者计数。 
常见的AD转换器的逻辑电路参考课本p102 : 
![[attachments/picture-filter-1704697391122.jpg|500]]

# XPT2046芯片 的A/D转换使用方法
## (1) XPT2046芯片引脚简介
课本上给出的是ADC0804芯片的电路。 而XPT2046是51单片机的芯片。 
XPT2046 是一款 4 线制电阻式触摸屏控制器，<mark style="background: transparent; color: red">内含 12 位分辨率 125KHz 转换速率逐步逼近型 A/D 转换器</mark>。常常使用于触摸屏中，用于触摸屏数据获取。 

有如下两种封装方式 (小体积QFN16以及 TSSOP): 
![[attachments/Pasted image 20240108151556.png]]

主要特性
①工作电压范围为 1.5V～5.25V
②支持 1.5V～5.25V 的数字 I/O 口  
③内**含 2.5V 参考电压源** (参考电压为2.5V, 从而<mark style="background: transparent; color: red">可以采集模拟电压为1.5-2.5V</mark>), 事实上，只需将5V的参考电压源接入, 即可改变其量程变为测量5V的部分。 
④电源电压测量（ 0V~6V）-> 6V是测量0-5V的电源电压。
⑤内建结温测量功能(有内部的温度传感器, 由于本身会发热, 一般可以测量工作过程中点的温度)。
⑥触摸压力测量
⑦采用 **3 线制 SPI 通信接口** (DSP1302, 74HC595等) 
⑧具有自动省电功能 (降低功耗等)

对于XPT2046, 在开发板上的操作管脚是P34-P37

参考[[💻softwares/⚡Circuit/💻51单片机教程/5. 红外遥控,DS1302时钟, AD转换/DS1302高精度时钟与SPI通信|DSP1302通信原理]] , 获取SPI 通信方法
对于XPT2046, 其各个管脚功能如下:  
1.  BUSY : 忙信号线，当CS为高电平时为高阻态， 
2.  DIN: 串行数据输入端, 当CS为低电平时, 是串行通信使用DCLK （DCLK为下降沿时, ）
3. CS: 片选信号即使能信号, 由于低电平有效, 在使用过程中, 需要将此线拉低。 
4. DCLK 和SPI通信中的外部信号输入相同。 
5. 电源
6. XP ：X+ 位置输入端
7. YP : Y+ 输入端
8. XN : X- 输入端
9. YN : Y- 输入端
需要说明， 触摸屏上往往有四根线分别为X+, X-, Y+, Y-用于采集X, Y的位置。
在采集外部信号时，可以使XPT2046工作在单端模式， 使上述每一个端口作为信号采集输入端。
10. GND 
11. V_BAT : 电池监视输入端, 采集电池的电压信号  
12. AUX: ADC辅助输入通道
13. VREF : <mark style="background: transparent; color: red">参考电源的输入管脚</mark>, 只要将其称1/2^12, 再乘AD转换值, 即可得到最终的值。 
14.  IOVDD : 数字电源输入引脚, 往往是和VCC连接在一起的。 
15. $\overline{PENIRQ}$: 中断引脚。 通过检测此信号， 可以检测是否有按下。 
16. DOUT：串行输出引脚。 

其中DOUT, DIN可以并联也可以分开成标准四线制的SPI接口。 

## (2) XPT2046 
XPT2046具有差分模式和单端模式

通过XP, YP, XN,YN连接四个

当工作于触摸屏检测触摸位置时， 需要工作在差分模式, 此时XP, YP, XN, YN 连接

XPT2046设置模式的方法列表如下:  即单端输入, 需要设定A0-A2的值。 
![[attachments/Pasted image 20240108161623.png|900]]

其中, XPT2046工作时序信号控制方式如下: 

![[attachments/Pasted image 20240108162008.png|900]]
1. <mark style="background: transparent; color: red">S为开始位</mark>, 当为1则为一个新的数据到来，如果为0, 则忽略引脚的数据。 
2. A2-A0选择通道,  单端模式下， 例如设置001时, 则可以采集XP所采集的模拟量。其中V_BAT , AUX_IN 和 TEMP管脚均具有测量功能。 只需选择对应的模式(即模拟信号通道采集控制)即可。
3. MODE 用于设置SPT2046的分辨率, 当为1时，则选择8位分辨率, 当为0时， 选择12位的分辨率。一般默认选择12位的分辨率
4. $SER/\overline{DFR}$是设<mark style="background: transparent; color: red">置单端输入或者差分输入方式</mark>, 对于采用模拟信号采集功能, 则使用单端输入(设置1), 设置为0是差分输入, 用于电子触摸屏等等。 
5. PD1, PD0 设置芯片的功耗, 当都为0时, 处于掉电低功耗模式并且有比中断管脚输出, 均为1时， 处于供电状态并且禁止比中断管脚的输出功能。 (可以默认为00) 

![[attachments/Pasted image 20240108161803.png|800]]
从时序图中可以看出, 信号发送是从高位到低位的。 从高位向低位进行写入；而读取时也是从高位向低位的。 

首先将CS设置为低电平进行使能,  DCLK进行时钟输入, DIN 是单片机写入
首先发送数据之后, 等待空闲时间(周期),  然后读取12位数据(0-11), 也是从11-0进行读取的。 
然后再发送4个时钟, 实际上这四位是省去的。 

得到的12位数据是AD值，我们需要将其乘VREF$\times \frac{1}{2^{12}}$, 在单片机中VREF为VCC, 可以认为是5V。

# XPT2046 编程与控制方法

在开发板上的电路图如图所示: 其中左侧的管脚功能已经说明, 而右侧的部分是将AIN0连接到滑动变阻器, 通过AIN0的移动实现不同的电压模拟量的获取。
![[attachments/Pasted image 20240108213309.png|600]]
此时， 对于AIN0上电压的采集即可实现对应的模拟量采集。 
在开发板上, NTC 连接的NTC1 是热敏电阻。
AIN2是连接到了GR1, 即光敏电阻上， 可以采集光敏电阻的电压值。 

开发板上的DAC模块中 还有 J52, 对应的AIN3方便于外部的模拟信号连接到单片机进行采集。其中AIN1, AIN2, AIN3 分别连接到这些模块上， 可以预留使用。 
![[attachments/Pasted image 20240108214416.png|150]]
例如烟雾报警器， 接法是首先将一段共地(结GND), 然后将另一端接到AIN3上。 

通过使用AIN0,AIN1, AIN2和AIN3分别可以实现变阻器, 热敏电阻, 光敏电阻和外部信号的采集。 只需要设置相应的A0A1A2即可。

> [!caution] 注意
> 由于P3_7管脚也是采集温度的管脚
> ADC模块不能和DS18B20进行同时使用。 

编写程序实现数码管上显示的AD模块采集电位器的电压值。 

首先, 由于使用类似于SPI接口的P3^4通信方式, 因此可以看出:  
每一次的r2为00000或者111111, 这显然是不对的。
![[attachments/Pasted image 20240109105412.png]]
我们注意到由于是下降沿读取的， 因此将第二个时序更改为下降沿之后经过一定的延时读取， 则得到如下的正确的12位数据 : 
![[attachments/Pasted image 20240109111948.png|300]]

正确的读取SPI时序通信代码如下 (需要注意由于是先高位， 使用左移):
```c
void XPT2046_Read_Data(u8 msg){
    u8 i;
    DIN = 0;
    _nop_(); 
    DCLK = 0;
    _nop_();
    // start send byte 
    CS = 0;  // enable  CS
    _nop_();
    for (i = 0; i < 8; i++){
        DIN = msg >> 7 ;
        _nop_();
        DCLK = 1;
        msg <<= 1; // send byte;
        _nop_(); 
        DCLK = 0;
        _nop_();
    }
    DIN = 0;
    _nop_();
    _nop_();
    DCLK = 1;
    _nop_();
    for (i = 0; i < 8; i++){
        data_res[0] <<=1;
        DCLK = 0; // corresponding to SPI time sequence, read at  
        _nop_();
        if (DOUT) data_res[0]++;
        DCLK = 1; // create rising edge 
        _nop_();
    }
    for (i = 0; i<8; i++){
        data_res[1] <<= 1;
        DCLK = 0;
        _nop_();
        if (DOUT) data_res[1]++;
        DCLK = 1;
        _nop_();
    }
    DCLK = 0;
    _nop_();
    CS = 1;   // disable
    _nop_();
}
```

另外需要注意, 由于使用int 型会导致除法整除4096时变成0，需要进行float 强制类型转换。


可以运行的代码如下: 
```c
#include <REGX52.H>
#include "public.h"
#include "LED_controller.h"
#include <INTRINS.H> 

// A/D Changer to view the different voltage level using I2C and SPI time sequence 
sbit DIN  = P3^4;  // IN port -> also act as SDA for PCF8591;
sbit DCLK = P3^6;  // clock input -> also act as SCL for PCF8591;
sbit CS   = P3^5;  // enable port ;
sbit DOUT = P3^7;  // OUT port 


float result = 0; // storge the tested voltage. 
float Voltage_res = 0;
u8 data_res[2] = {0x00, 0x00};

void XPT2046_Init(void){
    CS = 1;
    DCLK = 1;
    DIN = 1;  // note P3^4 have no pulling resistance and the default should be high.
    _nop_();
}

// use message for data 
void XPT2046_Read_Data(u8 msg){
    u8 i;
    DIN = 0;
    _nop_(); 
    DCLK = 0;
    _nop_();
    // start send byte 
    CS = 0;  // enable  CS
    _nop_();
    for (i = 0; i < 8; i++){
        DIN = msg >> 7 ;
        _nop_();
        DCLK = 1;
        msg <<= 1; // send byte;
        _nop_(); 
        DCLK = 0;
        _nop_();
    }
    DIN = 0;
    _nop_();
    _nop_();
    DCLK = 1;
    _nop_();
    for (i = 0; i < 8; i++){
        data_res[0] <<=1;
        DCLK = 0; // corresponding to SPI time sequence, read at  
        _nop_();
        if (DOUT) data_res[0]++;
        DCLK = 1; // create rising edge 
        _nop_();
    }
    for (i = 0; i<8; i++){
        data_res[1] <<= 1;
        DCLK = 0;
        _nop_();
        if (DOUT) data_res[1]++;
        DCLK = 1;
        _nop_();
    }
    DCLK = 0;
    _nop_();
    CS = 1;   // disable
    _nop_();
}

// Calculate voltage 
u8 LEDNum[3] = {0, 0, 0}; // 
void Voltage_Calculation(void){
    int res = 0;
    res = (int)data_res[0] * 16 + (int)data_res[1] /16;
    Voltage_res = (float)res / 4096 * 5; // reset the voltage level

    LEDNum[0] = ((int)Voltage_res)%10;
    LEDNum[1] = ((int)(Voltage_res * 10))%10;
    LEDNum[2] = ((int)(Voltage_res * 100))%10;
}

void main(){
    XPT2046_Init();
    while (1)
    {
        XPT2046_Read_Data(0x94);  // use XP as AIN0, send 1001 0100
        Voltage_Calculation();    // calculate the voltage
        LED_ShowNum(0, LEDNum[0], 1);
        LED_ShowNum(1, LEDNum[1], 0);
        LED_ShowNum(2, LEDNum[2], 0);
    }
}
```


# Proteus 实现ADC转换仿真 

可以使用PCF 8591进行仿真XPT2046,  但PCF8591是使用I2C 通信方法的8位的ADC芯片，同时兼有DAC转换功能。因此我们可以将AIN0上的电压转换为数字量进行显示。 

对于XPT2046芯片, EXT在进行A/D转换时需要外接晶振， 

![[attachments/Pasted image 20240108163510.png]]


一个可以使用的PCF8591的电路图如下, 但为了在Proteus中完成仿真, 我们只要关注SCL, SDA，而OSC可以一般放空引脚, EXT在AD转换时， 可以接地处理。A0-A2是地址值, 
![[attachments/Pasted image 20240109125009.png]]
我们使用如下的电路图在Proteus 中进行仿真: 
![[attachments/Pasted image 20240109125422.png]]


Each PCF8591 device in an I2C-bus system is activated by sending a valid address to the device. The address consists of a fixed part and a programmable part. The programmable part must be set according to the address pins A0, A1 and A2. The address always has to be sent as the first byte after the start condition in the I2C-bus protocol. The last bit of the address byte is the read/write-bit which sets the direction of the following data transfer 

从上面的部分， 我们可以总结出, 首先应当给出地址值,  第一个字节用于标明写或者读取。 
![[attachments/Pasted image 20240109141751.png|400]]


![[attachments/Pasted image 20240109143614.png|900]]

在编程中的注意事项：
1. 运算符的优先级
```c
if ((msg & 0x80 )> 0) // 在使用&运算符时需要注意优先级问题，必须加括号
```
调试成功后的代码如下: 
![[attachments/Pasted image 20240109144807.png|350]]
2. 获得ack的方法是在给出上升沿delay之后再获取稳定的ack值， 且不需要再次重置DIN
另外第一次是0x90(写), 不是0x91(读),不要搞混。
![[attachments/Pasted image 20240109153926.png|400]]
3. start 和stop 的前两句避免bug(必须制造上升或者下降沿)
```c
DIN = 0; //
```
4. 在send_ack  和send_nack中, 一定要先重置DCLK为0
```c
DCLK = 0;
```

The A/D converter makes use of the successive approximation conversion technique. The on-chip D/A converter and a high-gain comparator are used temporarily during an A/D conversion cycle. 

The first byte transmitted in a read cycle contains the conversion result code of the previous read cycle

从入下图可以看出， 如果给AutoIncrement 设置为1时, 则依次转换1,2,3,4四个口。 此时需要内部的晶振工作。 同时,需要设置第6位(左2)为out 状态
![[attachments/Pasted image 20240109154852.png]]

首先发送一个地址和控制信号， 然后重新开始发送read信号。 
时序图如下: 
![[attachments/Pasted image 20240109155831.png|800]]

通过第一次使用NACK进行获取对应的电压和调节电阻, 可以实现对应的电压数字量获取， 但是如果使用三个bit的数据, 则可能出现问题。
![[attachments/Pasted image 20240109162857.png|800]]

给出可以用于仿真电压模拟的代码如下: 
```c
#include <REGX52.H>
#include "public.h"
#include "LED_controller.h"
#include "EEPROM_OP.h"
#include <INTRINS.H> 

// A/D Changer to view the different voltage level using I2C and SPI time sequence 
sbit DIN  = P3^4;  // IN port -> also act as SDA for PCF8591;
sbit DCLK = P3^6;  // clock input -> also act as SCL for PCF8591;
sbit CS   = P3^5;  // enable port ;
sbit DOUT = P3^7;  // OUT port 

float result = 0; // storge the tested voltage. 
float Voltage_res = 0;
u8 data_res[2] = {0x00, 0x00};


void PCF8591_Init(void){ // set all as high voltage level 
    // 10010001
    DIN = 1;
    _nop_(); 
    DCLK = 1;
    _nop_();
}

void I2C_Start(){
    DIN = 1; // add it first 
    _nop_();
    DCLK = 1;
    _nop_();
    DIN = 0;
    _nop_();
    DCLK = 0;
    _nop_();

}

void I2C_Stop(){
    DIN = 0; // 
    _nop_();
    DCLK = 1;
    _nop_();
    DIN = 1; // stop the message 
    _nop_();
}

// if get answer it will return 0;
void I2C_SendByte(u8 msg){
    u8 i, ack = 0;
    DIN = 0; // start send data 
    _nop_();
    UART_Debug("Sending msg: ");
    for (i = 0; i < 8;  i++){
        if ((msg & 0x80 )> 0) { // **CAUTION 
            DIN = 1; UART_Debug("1");
        }
        else {
            DIN = 0; UART_Debug("0");
        }
        msg <<= 1;
        _nop_();
        DCLK = 1;
        _nop_();
        DCLK = 0; // reset to 0 
        _nop_();
    }
}

void I2C_SendAck(){
    DCLK = 0; // reset DCLK 
    _nop_();

    DIN = 0;
    _nop_(); 
    DCLK = 1;
    _nop_();
    DCLK = 0;
    _nop_();
    DIN = 1; // re-pull-up the bus 
    _nop_();
}

void I2C_SendNack(){
    DCLK = 0; // Reset DCLK 
    _nop_();

    DIN = 1;
    _nop_();
    DCLK = 1;
    _nop_();
    DCLK = 0;
    _nop_();
}

u8 I2C_Wait_Answer(void){
    u8 time_temp = 0;
    DIN = 1;
    _nop_();
    while (DIN)
    {
        time_temp++;
        if (time_temp >100){
            I2C_Stop();
            return 1;
        }
    }
    // clock pulse for acknowledgement 
    DCLK = 1;
    _nop_();
    DCLK = 0;
    _nop_();
    return 0;
}

u8 I2C_ReadByte(u8 ack){
    u8 dat, i = 0; // at first DIN = 1;
    DIN = 1;
    _nop_();
    for (i = 0; i < 8; i++){
        dat <<= 1;
        if (DIN){
            dat++; 
            UART_Debug("1");
        }else{
            UART_Debug("0");
        }
        DCLK = 1;
        _nop_();
        DCLK = 0;
        _nop_();
    }
    if (ack){
        I2C_SendAck();
    }else{
        I2C_SendNack();
    }
    _nop_();
    return dat;
}

void PCF8591_Read_Data(){
    u8 ans = 0, res = 0;
    I2C_Start();
    I2C_SendByte(0x90);  // address byte 1001 0000  
    ans = I2C_Wait_Answer();
    // 0100 0100 as I2C bus protocol
    I2C_SendByte(0x04); // Send Control Byte -> 8 digits
    ans = I2C_Wait_Answer();
    I2C_Start();
    I2C_SendByte(0x91);  // Begin read byte;
    ans = I2C_Wait_Answer();
    if (!ans) {
        UART_Debug("Receive data: ");
    }
    data_res[0] = I2C_ReadByte(0);
    UART_Debug("\n");
    I2C_Stop();
}

void XPT2046_Init(void){
    CS = 1;
    DCLK = 1;
    DIN = 1;  // note P3^4 have no pulling resistance and the default should be high.
    _nop_();
}

// use message for data 
void XPT2046_Read_Data(u8 msg){
    u8 i;
    DIN = 0;
    _nop_(); 
    DCLK = 0;
    _nop_();
    // start send byte 
    CS = 0;  // enable  CS
    _nop_();
    for (i = 0; i < 8; i++){
        DIN = msg >> 7 ;
        _nop_();
        DCLK = 1;
        msg <<= 1; // send byte;
        _nop_(); 
        DCLK = 0;
        _nop_();
    }
    DIN = 0;
    _nop_();
    _nop_();
    DCLK = 1;
    _nop_();
    for (i = 0; i < 8; i++){
        data_res[0] <<=1;
        DCLK = 0; // corresponding to SPI time sequence, read at  
        _nop_();
        if (DOUT) data_res[0]++;
        DCLK = 1; // create rising edge 
        _nop_();
    }
    for (i = 0; i<8; i++){
        data_res[1] <<= 1;
        DCLK = 0;
        _nop_();
        if (DOUT) data_res[1]++;
        DCLK = 1;
        _nop_();
    }
    DCLK = 0;
    _nop_();
    CS = 1;   // disable
    _nop_();
}

// Calculate voltage 
u8 LEDNum[3] = {0, 0, 0}; // 
void Voltage_Calculation_XPT2046(void){
    int res = 0;
    res = (int)data_res[0] * 16 + (int)data_res[1] /16;
    Voltage_res = (float)res / 4096 * 5; // reset the voltage level
    LEDNum[0] = ((int)Voltage_res)%10;
    LEDNum[1] = ((int)(Voltage_res * 10))%10;
    LEDNum[2] = ((int)(Voltage_res * 100))%10;
}

void Voltage_Calculation_PCF8591(void){
    int res = 0; 
    res = (int) data_res[0];
    Voltage_res = (flaot)res / 256 * 5;
    LEDNum[0] = ((int)Voltage_res)%10;
    LEDNum[1] = ((int)(Voltage_res * 10))%10;
    LEDNum[2] = ((int)(Voltage_res * 100))%10;
}

void main(){
    
    UART_INIT();
    UART_Debug("UART_Init: \n");
    PCF8591_Init();
    while (1)
    {
        PCF8591_Read_Data();  // use X0 (send 0100, 0100);
        Voltage_Calculation_PCF8591();
        LED_ShowNum(0, LEDNum[0], 1);
        LED_ShowNum(1, LEDNum[1], 0);
        LED_ShowNum(2, LEDNum[2], 0);
    }
}
```

获得的仿真图部分如下(可以调节滑动变阻器实现电压测量): 
![[attachments/Pasted image 20240109164016.png]]
