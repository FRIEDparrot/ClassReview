# 基本结构
DS1302芯片是一个SPI时序的芯片, 需要使用3根线来模拟SPI时序来实现通信,  

DS1302芯片的连接电路如图所示: 
是一个涓流充电时钟芯片, 由于充电完成之后, 由于内部放电, 可以以微小的电流达到充电的效果。功率< 1mW
内部具有时钟， 日历以及31字节的RAM， 通过三线制的SPI接口进行通信, 通过提供时， 分，秒，周，年月等, 其中可以通过AM/PM设置每个月天数和闰年的天数，并通过AM/PM设置, 使用24小时制的时间格式, 

其中简易三线 SPI的通信线包括 
1. 复位数据线(P36), 
2. IO使能线 (P34) -> 传输数据, 即将MISO, MOSI合成一条。  
3. 时钟线 (P35) 

> [!NOTE] 标准四线制接口
> 四线制接口包括: 
> 1. MISO
> 2. MOSI 
> 3. 复位线/使能线 
> 4. 时钟线(SCLK)

![[attachments/Pasted image 20240104100647.png|500]]

DS1302是DS1202的改进版本, 具有双电源管脚, VCC2是主电源, VCC1是备用电源。
其中往往电脑中是有自带的电池, 保证电脑关机之后，时钟芯片依然工作。

- 具有计算2100年之前的时分秒日历等功能。 具有31位的RAM。是串行IO的通信方式。
- 工作电压支持 2.0-5.5V 电压工作。
- 当其工作在2.0V时, 电流<300nA, 功耗极低
- 在读写时钟和RAM时, 有单字节和多字节字符组传输的方式
- 具有两种封装, 包括DIP和SOIC贴片封装
- 兼容TTL 电平 
- 工作温度范围是$-40C\degree - 85C\degree$(工业级) 

<mark style="background: transparent; color: red">晶振是32.768K</mark>的晶振电路。5是复位, 6为IO, 7为时钟引脚


> [!NOTE] 常见的时钟芯片分类
> 一般而言, 时钟芯片主要分为两类, 其中
> 第一类是体积小, 引脚较少的, 但是需要外接的晶振和备用电源实现供电。例如 DS1302, DS1337, DS1338, DS1390 以及并行的DS1558等等。
> 第二类是体积较大的直插式芯片, 内部集成了可充电电池以及标准晶振的部分， 使用较为方便。 常用的有DS12887, DS12887A, DS12B887, DS12C887等等。 在51单片机教程中给出了DS12C887的使用方法以及操作

# DS1302寄存器使用简介
## (1) 寄存器的结构和位控制
DS1302是占用单片机==将其时间数据读取出来并进行显示==。 

DS1302的寄存器格式如下: 
有1个控制寄存器和12个日历， 时钟寄存器。 31位的RAM存储空间。 


RST回到高电平时, 开始工作。 
控制寄存器的格式如下图 , 用于对DS1302的读写过程进行控制
![[attachments/Pasted image 20240104110006.png|700]]
第7位时1, 第六位是RAM/CK, 当6位为1时, 是操作RAM的操作状态, 而$\overline{CK}$低电平有效(在为0时,是操作寄存器模式)。  

而5-1 (A4- A0), 当RAM功能时, 是RAM的地址, 而寄存器功能时, 是寄存器地址
RD/ WR用于控制读写功能。 在0时为写操作, 在1时为读操作。 

常用的操作格式为:

| D7 | D6 | D5 | D4 | D3 | D2 | D1 | D0 |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| 1 | 0 |  |  |  |  |  | R/W |
|  |  |  |  |  |  |  |  |
各个<mark style="background: transparent; color: red">寄存器的地址可以通过如下进行访问</mark> : 
![[attachments/Pasted image 20240104110735.png|500]]
![[attachments/Pasted image 20240104110744.png|500]]
对应地，需要写入， 则R/W为0, 需要读取, 则R/W为1

## (2) 日历-时钟寄存器格式
日历/时钟寄存器以BCD码形式存入的。 
秒和分寄存器中<mark style="background: transparent; color: red">秒的个位为低4位, 而秒的十位为前面的 3位(由于不可能大于5)</mark> 第7位CH为时钟工作的标志位, 当CH = 0时, 处于工作状态。 如果<mark style="background: transparent; color: red">CH = 1时, DS1302停止工作</mark>, 而分的最高位固定为0。 

小时寄存器: 如果当D7部分为1时为12小时制, 为0时24小时制。在12小时方式下，位 5 是 AM/PM 位，此位为逻辑高电平表示 PM。在 24 小时方式下，位5 是第2 个10 小时位（20-23时）。

当设置12小时制时, 前四位为个位, 而第五位为十位 (D4,D5分别决定) 
![[attachments/Pasted image 20240104113524.png|500]]

![[attachments/Pasted image 20240104163410.png|700]]
另外还有写保护寄存器功能：最高位WP为写保护寄存器功能。当WP=1时, 只读模式,  而WP= 0时, 关闭写保护功能。
慢充电寄存器: 在DS1302掉电时,  会调用备用电源保护数据。此配置寄存器用于调整充电模式, 当TCS为1010时为慢充电模式, 低4位选项与DS1302内部电路有关。 
![[attachments/Pasted image 20240104113529.png|500]]
对于日，月，年，星期等完全类似

### (3) BCD码存储方法
BCD码是使用四位二进制码数据表示0-9之间的表示方法。 
对于BCD码格式存放转为十进制存放的方法， 只需要先对16取余， 而高位只需要除以16，即有:
```c
A = 0xff;
A_L = A%16; // 个位上的数字 
A_H = A/16;  // 十位上的数字
```

# DS1302读写时序与SPI总线通信方法
对DS1302读写时序的控制, 只需使用如下的读写时序图: 
在每一次字节的发送时, 都是<mark style="background: transparent; color: red">从低位开始写到高位</mark>,  

读时是在控制命令发送的下一个下降沿进行读取， 而写入时， 是在上升沿进行写入的。 

在发送和接受时，使能端CE**必须设置为1**,  读写完成之后， 需要设置为低电平，等待下一次操作。
读的操作时， 设置R/W 是1, 而写的操作时, R/C仍然设置为0, 在发送完成之后, 下一个下降沿开始读。 
![[attachments/Pasted image 20240104144914.png|600]]

首先需要说明， 要将写保护功能进行关闭。 

写入时， 必须传入BCD码, 使用四位的二进制来表示BCD码。 读取时， 也需要将BCD码转换成十进制进行显示。

基本过程如下: 
1. 解除写保护, 如果让秒运行起来， 则设置秒的最高位为1
2. 一个关键的点时时序图的读取， 在数据写完毕之后， 应当保持为高电平, 
![[attachments/Pasted image 20240104184754.png|600]]
首先, 在SPI总线时序中, 初始时为低电平，<mark style="background: transparent; color: red">写入时 , 先设置IO为要写入的， 然后再</mark>变为高电平。 
读取时, 初始时为高电平, 

`````ad-caution 
title: 时序中的注意事项
collapse: open
1. 由于写函数结束后总线是高电平，在两次写函数结束时， **应当在两次写的函数完毕之后, 给一个回零的低电平** 
2. 由于读函数是给8次下降沿和7此上升沿， 需要一个if语句来判断最后一个不加上升沿(加了一般由于RST没有影响)
`````

3. 其二是每两次Reading 函数中会出现一次全为1的部分。 例如我们仅仅对秒进行读取， 则会得到如下的结果， 其中1000000为正确的时间, 而1111111显然是错的。
![[attachments/Pasted image 20240104190741.png]]

首先， 调试的方案是首先暂停时钟(设置秒为0x80)， 暂停时钟之后， 读数仍然有111111的情况, 因此问题应当位于读取的函数。
![[attachments/Pasted image 20240104193139.png]]
其解决办法是<mark style="background: transparent; color: red">在读取函数中加入下面的关键代码</mark>:  
在整个的RST读完之后需要将RST拉低， 首先读时需要设置方向是读取方向, 首先应当设置IO口的方向为读取(即输入状态)， 由于单片机的IO口是准双向的IO口,  读之后方向会改变, 读完之后要进行输出。

<mark style="background: transparent; color: red">由于该单片机的IO口没有上拉电阻</mark>，所有首先要在RST = 0时, 释放时钟总线, 将IO口作为输出, 这个是由于让没有上拉的板也能IO口作为输出的附加语句， 在时序中没有体现。 
```c
    // ** Key Code are : **
    SCLK =  1;
    _nop_();
    
    IO = 0 ; // 释放时钟总线, 将IO口作为输出 
    _nop_();
    IO = 1 ;
    _nop_();
// 这样做的原因是读取过程中是输入, 而后面设置IO= 0, IO = 1是将IO作为输出进行使用
```

调试成功的截图如下: 
![[attachments/Pasted image 20240104214311.png]]


> [!caution] 读字节函数的说明!!!!
> 对于读字节函数, 在时序图中， 制造8次下降沿和7次上升沿,  因此不能简单给一个for循环, 否则被认为是解读下一次数据

常用的低位读取办法
```c
value=(temp<<7)|(value>>1);//先将value右移1位，然后temp左移7位，最后或运算
```

2. 对于u8类型的值，不能使用整形的取余运算， 因此可以先转换为整形, 再相加得到整形数据
```c
sec = (sec_t >> 4) * 10  + ((int) sec_t % 16); // 其中sec 为 int , sec_t 为 u8 
```

`````ad-note
title: 补充 
collapse: collapse
```c
// in the register operation, 6 bit(CK) are all 0, 
#define Reg_sec_w 0x80
#define Reg_sec_r 0x81 // 1000 0000
#define Reg_min_w 0x82
#define Reg_min_r 0x83
#define Reg_hour_w 0x84
#define Reg_hour_r 0x85
#define Reg_day_w 0x86
#define Reg_day_r 0x87
#define Reg_mon_w 0x88
#define Reg_mon_r 0x89
#define Reg_wek_w 0x8a
#define Reg_wek_r 0x8b
#define Reg_year_w 0x8c
#define Reg_year_r 0x8d
#define Reg_write_protection_w 0x8e
#define Reg_write_protection_r 0x8f
```
`````

```c
#include <REGX52.H>
#include <INTRINS.H>
#include "public.h"
#include "EEPROM_OP.h"
#include "LED_controller.h"

sbit SCLK = P3^6; // CLK port
sbit IO = P3^4 ;   // I/O port 
sbit EN = P3^5 ;   // Enable port


// send must started at low voltage and end as SCLK = 1 at end 
void Send_byte(u8 msg){ 
    u8 i, temp;
    for (i=0; i<8;i++){
        // send byte from low digit to high digit
        temp = (msg & 0x01); msg >>= 1;

        SCLK = 0; 
        _nop_();
        IO = temp; // operate IO first and then make rising edge.
        _nop_();
        SCLK = 1;       // make a rising edge to send 
        _nop_();
    }
    _nop_();
}

// read: high voltage at first and SCLK = 0 at end 
u8 Read_byte(void){ 
    u8 i, res = 0;
    SCLK = 1;
    _nop_();
    UART_Debug ("Reading: ");
    for (i = 0; i < 8; i++){
        // make plummet, read data and then rise
        SCLK = 0;
        _nop_();
        res >>= 1;
        // don't read before plummet
        if (IO == 1) {
            res |= 0x80;
            UART_Debug("1");
        }else{
            UART_Debug("0");
        }
        _nop_();
        if (i!=7){ // at last time it would not create rise.            
            SCLK = 1;                 // wait for another bit 
            _nop_();
        }
    }

    UART_Debug("\n");
    return res;
}

// must init before use 
void DS1302_Init(void){
    EN = 0;       // at first is zero 
    _nop_();
    SCLK = 0;     // 
    _nop_(); // wait for voltage level build 
}

// operation defined above (end with w)
void DS1302_Write(u8 operation, u8 msg){
    EN = 1;
    _nop_();
    Send_byte(operation);
    Send_byte(msg); // after it send twice, 
    SCLK = 0; // set SCLK as zero (since after send it is high)
    _nop_();
    EN = 0;
    _nop_();
}

// operation defined above (end with r)
u8 DS1302_Read(u8 operation){
    u8 res;
    EN = 1;
    _nop_();
    Send_byte(operation);
    res = Read_byte();
    EN = 0;
    _nop_(); 
    // ** Key Code **
    SCLK =  1;
    _nop_();
    IO = 0 ;
    _nop_();
    IO = 1 ;
    _nop_();
    return res;
}

int sec = 0, min=0, hour=0, day =0, week =0, month = 0 , year = 0; 
u8 h_mode = 0, AM_PM = 0; // 24 h: 0  / 12h: 1

void DS1302_Get_Time(){
    u8 sec_t, min_t, hour_t, day_t, week_t, month_t, year_t;
    sec_t = (DS1302_Read(Reg_sec_r) & 0x7f);  // not consider D7
    min_t = (DS1302_Read(Reg_min_r) & 0x7f);  
    hour_t = DS1302_Read(Reg_hour_r);  // not consider D6,D7;
    day_t  =(DS1302_Read(Reg_day_r) & 0x3f);
    week_t =(DS1302_Read(Reg_wek_r) & 0x0f);
    month_t=(DS1302_Read(Reg_mon_r) & 0x1f);
    year_t = DS1302_Read(Reg_year_r);

    sec = (sec_t >> 4) * 10  + ((int) sec_t % 16);
    min = (min_t >> 4) * 10  + ((int) min_t % 16);
    if ((hour_t & 0x80) ==  0) {
        h_mode = 0; hour_t = (hour_t & 0x3f); // 24 hrs
    }else{
        h_mode = 1; hour_t = (hour_t & 0x1f); // 12 hrs 
        AM_PM = (hour_t & 0x20) == 0 ? 0 : 1; // 0: AM  1: PM 
    }
    hour = (hour_t >> 4)*10 + ((int)hour_t % 16);

    ES = 0; // disable the interruption when sending data.
    TI = 1; // first, we enable the TI before 
    printf("sec: %d\n", sec);
    printf("min: %d\n", min);
    printf("hour: %d\n",hour);
    printf("day: %d\n", day);
    printf("week: %d\n", week);
    printf("month: %d\n",month);
    printf("year: %d\n",year);
    while (!TI); // wait for sending;
    TI = 0;
    ES = 1;
}
```

`````ad-note
title: 可运行备份
collapse: close 

```c
#include <REGX52.H>
#include <INTRINS.H>
#include "public.h"
#include "EEPROM_OP.h"
#include "LED_controller.h"

// in the register operation, 6 bit(CK) are all 0, 
#define Reg_sec_w 0x80
#define Reg_sec_r 0x81 // 1000 0000
#define Reg_min_w 0x82
#define Reg_min_r 0x83
#define Reg_hour_w 0x84
#define Reg_hour_r 0x85
#define Reg_day_w 0x86
#define Reg_day_r 0x87
#define Reg_mon_w 0x88
#define Reg_mon_r 0x89
#define Reg_wek_w 0x8a
#define Reg_wek_r 0x8b
#define Reg_year_w 0x8c
#define Reg_year_r 0x8d
#define Reg_write_protection_w 0x8e
#define Reg_write_protection_r 0x8f

sbit SCLK = P3^6; // CLK port
sbit IO = P3^4 ;   // I/O port 
sbit EN = P3^5 ;   // Enable port


// send must started at low voltage and end as SCLK = 1 at end 
void Send_byte(u8 msg){ 
    u8 i, temp;
    for (i=0; i<8;i++){
        // send byte from low digit to high digit
        temp = (msg & 0x01); msg >>= 1;

        SCLK = 0; 
        _nop_();
        IO = temp; // operate IO first and then make rising edge.
        _nop_();
        SCLK = 1;       // make a rising edge to send 
        _nop_();
    }
    _nop_();
}

// read: high voltage at first and SCLK = 0 at end 
u8 Read_byte(void){ 
    u8 i, res = 0;
    SCLK = 1;
    _nop_();
    UART_Debug ("Reading: ");
    for (i = 0; i < 8; i++){
        // make plummet, read data and then rise
        SCLK = 0;
        _nop_();
        res >>= 1;
        // don't read before plummet
        if (IO == 1) {
            res |= 0x80;
            UART_Debug("1");
        }else{
            UART_Debug("0");
        }
        _nop_();
        if (i!=7){ // at last time it would not create rise.            
            SCLK = 1;                 // wait for another bit 
            _nop_();
        }
    }

    UART_Debug("\n");
    return res;
}

// must init before use 
void DS1302_Init(void){
    EN = 0;       // at first is zero 
    _nop_();
    SCLK = 0;     // 
    _nop_(); // wait for voltage level build 
}

// operation defined above (end with w)
void DS1302_Write(u8 operation, u8 msg){
    EN = 1;
    _nop_();
    Send_byte(operation);
    Send_byte(msg); // after it send twice, 
    SCLK = 0; // set SCLK as zero (since after send it is high)
    _nop_();
    EN = 0;
    _nop_();
}

// operation defined above (end with r)
u8 DS1302_Read(u8 operation){
    u8 res;
    EN = 1;
    _nop_();
    Send_byte(operation);
    res = Read_byte();
    EN = 0;
    _nop_(); 
    // ** Key Code **
    SCLK =  1;
    _nop_();
    IO = 0 ;
    _nop_();
    IO = 1 ;
    _nop_();
    return res;
}

int sec = 0, min=0, hour=0, day =0, week =0, month = 0 , year = 0; 
u8 h_mode = 0, AM_PM = 0; // 24 h: 0  / 12h: 1

void DS1302_Get_Time(){
    u8 sec_t, min_t, hour_t, day_t, week_t, month_t, year_t;
    sec_t = (DS1302_Read(Reg_sec_r) & 0x7f);  // not consider D7
    min_t = (DS1302_Read(Reg_min_r) & 0x7f);  
    hour_t = DS1302_Read(Reg_hour_r);  // not consider D6,D7;
    day_t  =(DS1302_Read(Reg_day_r) & 0x3f);
    week_t =(DS1302_Read(Reg_wek_r) & 0x0f);
    month_t=(DS1302_Read(Reg_mon_r) & 0x1f);
    year_t = DS1302_Read(Reg_year_r); 

    sec = (sec_t >> 4) * 10  + ((int) sec_t % 16);
    min = (min_t >> 4) * 10  + ((int) min_t % 16);
    if ((hour_t & 0x80) ==  0) {
        h_mode = 0; hour_t = (hour_t & 0x3f); // 24 hrs
    }else{
        h_mode = 1; hour_t = (hour_t & 0x1f); // 12 hrs 
        AM_PM = (hour_t & 0x20) == 0 ? 0 : 1; // 0: AM  1: PM 
    }
    hour = (hour_t >> 4)*10 + ((int)hour_t % 16);
    day = (day_t >> 4) * 10 + ((int)hour_t % 16);
    week = ((int) week_t);
    month_t = (month_t >> 4)*10 + ((int)month_t % 16);
    year_t  = (year_t >> 4)*10 + ((int)year_t % 16);
    
    ES = 0; // disable the interruption when sending data.
    TI = 1; // first, we enable the TI before 
    printf("sec: %d\n", sec);
    printf("min: %d\n", min);
    printf("hour: %d\n",hour);
    printf("day: %d\n", day);
    printf("week: %d\n", week);
    printf("month: %d\n",month);
    printf("year: %d\n",year);
    while (!TI); // wait for sending;
    TI = 0;
    ES = 1;
}

// 
void DS1302_Set_Time_Hex(u8 sec_w, u8 min_w, u8 hour_w, u8 day_w, u8 week_w, u8 mon_w, u8 year_w){
    DS1302_Write(Reg_write_protection_w,0x00);
    DS1302_Write(Reg_sec_w, sec_w);
    DS1302_Write(Reg_min_w, min_w);
    DS1302_Write(Reg_hour_w, hour_w);
    DS1302_Write(Reg_day_w, day_w);
    DS1302_Write(Reg_wek_w, week_w);
    DS1302_Write(Reg_mon_w, mon_w);
    DS1302_Write(Reg_year_w, year_w);
    DS1302_Write(Reg_write_protection_w,0x80);
}

void main(){
    int i;
    UART_INIT();
    DS1302_Init(); // must init before use 
    DS1302_Set_Time_Hex(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    while (1){
        DS1302_Get_Time();
        for( i = 0; i < 40;i ++){
            LED_ShowDateTime((u8)hour,(u8)min,(u8)sec, (u8)day, 0);
            Delay(1);
        }
    }
}

```
`````


