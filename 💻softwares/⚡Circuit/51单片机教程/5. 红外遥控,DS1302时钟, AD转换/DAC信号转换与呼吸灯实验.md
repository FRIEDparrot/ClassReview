# DAC概述以及主要参数
为了让51单片机输出模拟信号, 我们通常需要DA转换器来输出模拟信号

DAC的专用芯片成本很高， 常常使用PWM技术来获取模拟的DAC输出。对于PWM模块电路， 只要输出PWM的波来产生DAC功能。

DAC即数字/模拟信号转换器。DAC将输出电压模拟信号， 用于驱动某些器件, 例如音频信号的采集和还原等等。

首先采集模拟信号, 经过处理之后输出模拟信号 。 

常用的技术指标有: 
1. 分辨率: 输入数字量最低有效位时， 输出模拟量的变化。反映了最小的变化。可以通过输出的数字量数据乘对应的值即为模拟电压值。 精度越高，模拟量的变化越连续。
2. 线性度: 实际转换特性曲线和理想直线状态值的差别。 
3. 绝对精度和相对精度 : 绝对精度是指在整个刻度范围内，**任一输入数码所对应的模拟量实际输出值与理论值之间的最大误差**;  相对精度是最大误差相对于满刻度的百分比表示。这个误差应当低于$\frac{1}{2}LSB$ 

例如: 一个8位D/A转换器, 最大数字量的输出值为
$$\frac{255}{256}V_{REF} \rightarrow   LSB = \frac{1}{256}V_{REF}$$
则$\frac{1}{2}LSB = \frac{1}{512}V_{REF}$
4. 建立时间$t_{set}$: 指输入数字量变化时, 输出电压变化到相应的稳定电压值所需要的时间。(分为超高速， 高速， 中速，低速等几档)
5. 转换速率SR : 大信号工作状态下模拟电压的变化率。 
6. 温度系数: 在输入不变的状态下, 输出模拟电压随温度变化产生的变化量。 一般以温度每升高1$C\degree$, 对应的输出电压变化百分数作为温度系数。

# DAC工作原理
### (1) DAC 工作原理
参考[[📘ClassNotes/⚡Circuit/电工电子技术/第十章 模拟-数字信号转换器#一、D/A转换器|第十章 模拟-数字信号转换器]]， DAC由数字寄存器(D0-D7)和加法(求和)运算放大器得到。
![[Excalidraw/第十章 模拟-数字信号转换器 2023-12-27 12.17.06|500]]
有输出电压计算公式
$$V_0 = V_{ref} * \frac{z}{256}$$
输出的电压值需要$V_{REF}$为参考值。 位数不同，对应的的 8位中的电阻不一定是平均分配的。

### (2) PWM 概念模拟DAC的方法
大多数使用PWM(Pulse Width Modulation)即脉宽调制， 通过高分辨率计数器的使用, 
![[Excalidraw/DAC数字-模拟信号转换 2024-01-10 11.37.01]]
PWM广泛应用于功率控制, 伺服控制, 调光等等的电力控制中。 实际上是对具体的模拟信号进行编码， PWM信号仍然是数字的, 不是模拟量。

对应地, 数字信号b对应的模拟信号a如上图所示, 其中每个周期中有高电平持续时间和低电平持续时间， 通过**高电平持续时间(黑色)占据整个周期的时间比例(即占空比)代表对应的不同电压输出**， 实现模拟信号的PWM输出。 实质上是<mark style="background: transparent; color: red">输出占空比可调的信号</mark>。

只要带宽足够，任何模拟值均可以通过PWM进行模拟。
![[attachments/Pasted image 20240110113154.png|400]]
周期可以通过定时器产生。 划分可以通过0.01ms 进行采样。 通过占空比调节即可改变DAC的电压。 
常见DAC0832, PCF8591等等均可以作为DAC转换芯片。 只要使用PWM加上外部的RC电路就可以实现模拟量的输出功能。 

# DAC转换的PWM硬件部分
说明: 在原理图上的电容表示法中, 单位: (uF > nF > pF)
104表示100000pF即$10^{5} pF$,或者0.1uF 其中**10为整数部分, 4为后面要补的0个数**, 例如100表示10pF

开发板上的原理图如图所示, 其中使用LM358作为运算放大器(这个可以在proteus中找到)。
![[attachments/Pasted image 20240110120155.png|550]]
上图实际上是一个同相比例运算电路,  由于输入是5V超过量程, 实际上变为运放跟随电路
PWM是什么输入, DAC就是什么输出， 并通过发光二极管的亮度变化获取电压的改变量。
![[attachments/Pasted image 20240110134101.png|500]]
即可以通过DAC实现呼吸灯由灭逐渐变亮再变灭的过程。 同时可以通过DAC1调节其他的电压输出, 另外也可以连到AIN3上, 实现AIN3测量采集对应ADC上的模拟信号和显示的功能。 

对于PWM波经过RC电路, 实际上模拟的是8位的DAC功能,  由于系统电压是5V, 则计算关系使用256计算。 

# 呼吸灯代码
 
程序要求连接AIN3和DAC1实现数码管上显示输入的得到的DAC的PWM模拟电压, 同时指示灯DA1呈呼吸状亮灭。

我们设总周期为1ms, 并划分为0.01ms(10us)的段。如果改变了, 则高电平时间也相应地改变。


为了让PWM输出的更加流畅， 需要定义 `tim_h, tim_l` 两个参数来确定最小周期(例如0.01ms)。 
需要定义time_scale参数, time_scale 设置放大倍数, 即使用
$$\text{time\_scale} \space \times  最小周期T = \text{PWM周期}$$
而在每个周期内, 通过判断个数调节占空比， 则映射到正弦曲线上会获取到不同的电压。 
![[Excalidraw/DAC信号转换与呼吸灯实验 2024-01-10 16.08.21|800]]
我们此处取150为周期的倍频, 对于PWM, <mark style="background: transparent; color: red">产生的是固定周期的方波</mark>，

另外， 为了每一次给定时器在中断中赋值时， 仍然赋值初始值, 可以使用一个全局变量直接先存储对应的初始值。 
```c
u8 G_tim_h = 0x00, G_tim_l = 0x00; 
void PWM_INIT(u8 tim_h, u8 tim_l, u16 tim_scale, u8 duty){
    // Timer0 Start
    G_tim_h = tim_h; G_tim_l = tim_l; // also storged in the global variables.
} 
// 
void TIMER0_FUNC () interrupt 1{
    // for Timer1, interruption is 1
    TH0 = G_tim_h, TL0 = G_tim_l;
}
```

在中断函数中，定时代表0.01ms到达, 每一次加到某个值, 即设定P2_1的电平从高电平变为低电平。 通过静态变量保存。

我们取占空比(duty, PWM ratio) 设置一个对应部分, 当计数次数达到占空比时， 将端口设置为0, 即输出

```c
void TIMER0_FUNC () interrupt 1{
    u16 time = 0;
    // for Timer1, interruption is 1
    TH0 = G_tim_h, TL0 = G_tim_l;
    time ++;
    if (time == G_duty) DAC_PORT = 0;
    if (time == G_tim_scale) {
        time = 0; // when
        DAC_PORT = 1;
    }
}
```

我们只需要在主程序中更改占空比即可。 
可以单独设置一个占空比值的函数，修改时只要 修改全局变量的值即可。 

但是，需要注意， 如果是在time增加过程中减小局部变量的值，则往往会由于time已经超过duty无法判断，因此我们定义一个信号全局变量duty_new  (注意是duty_new不是scale_new)

需要注意的是, PWM 周期不能太大, 最好在1ms左右, 否则可能出现闪烁

另外占空比一般不要直接设置为100, 可以设置为70左右， 让实际观察确定对应值。


```c
// 错误代码
void PWM_SET(u16 duty_new){
    G_duty = duty_new; //
}
```


下面这个虽然能够运行并且输出呼吸灯， 但是往往由于中断太多次数导致 LED扫描时不断闪烁，因此我们可以考虑让其不闪烁的方案。 
```c
#include <REGX52.H>
#include "public.h"
#include "LED_controller.h"
#include "PCF8591_func.h"
#include "EEPROM_OP.h"
#define PI 3.14

sbit DAC_PORT = P2^1;

u8 G_tim_h = 0x00, G_tim_l = 0x00;
u16 G_tim_scale = 0x00 , G_duty = 0x00, G_duty_new= 0x00;  // use global variables 

// Calculate voltage 
u8 LEDNum[3] = {0, 0, 0}; //

/// @brief INIT Timer 0 in way1, and set time in tim_h and tim_l for period
/// @param tim_h timer0 high bit , 0xfc for 1ms, 0xff for 10us
/// @param tim_l timer0 low bit  , 0x66 for 1ms, 0xf6 for 10us
/// @param tim_scale period doubling (best for >=100)
/// @param duty Duty Cycle, or PWM ratio. (must less than tim_scale)
void PWM_INIT(u8 tim_h, u8 tim_l, u16 tim_scale, u16 duty){
    // Timer0 Start 
    G_tim_h = tim_h;
    G_tim_l = tim_l;
    G_tim_scale = tim_scale;
    G_duty = duty; G_duty_new = duty;

    DAC_PORT = 1; // set DAC port as 1 to init.
    // also storged in the global variables.  
    TMOD &= ~T0_GATE_;        // set T0 gate as 0;
    TMOD &= ~T0_CT_;          // set T0 CT as 0; (工作在定时方式)
    TMOD |= T0_M0_;           // set the MO to 1;
    TMOD &= ~T0_M1_;          // set the M1 to 0; 
    EA = 1; ET0 = 1;          // set interrupt gate as open 
    TH0 = tim_h; TL0 = tim_l; // set the initial value (for count)
    TR0 = 1;                  // start the timer 0 (note if timer 1 isTR1) 
}


void TIMER0_FUNC () interrupt 1{
    static u16 time = 0; // be attention for static
    // for Timer1, interruption is 1
    TH0 = G_tim_h, TL0 = G_tim_l;  // reset timer 
    time ++;
    if (time >= G_duty) {
        DAC_PORT = 0;
    }
    else DAC_PORT = 1;
    if (time == G_tim_scale) { 
        time = 0; // when
        G_duty = G_duty_new; // set to new variable.    
    }
}

void PWM_SET(u16 duty_new){
    G_duty_new = duty_new; 
}

void Show_LED(){
    float Voltage_res = 5 * (float)G_duty_new/G_tim_scale;
    LEDNum[0] = ((int)Voltage_res)%10;
    LEDNum[1] = ((int)(Voltage_res * 10))%10;
}

void main(){
    u16 res = 0, mode = 0;
    u16 time_scale = 100;
    XPT2046_Init();
    PWM_INIT(0xff, 0xed, time_scale, 0);
    while (1)
    {
        LED_ShowNum(0, LEDNum[0], 1); // note -> need 20ms
        LED_ShowNum(1, LEDNum[1], 0); // note -> need 20ms
        Delay(10); // 0.6 us
        if (mode == 0){
            res ++;
            if (res == 60){
                mode = 1;
            }
        }
        else{
            res--;
            if (res == 0){
                mode = 0;
            }
        }
        Show_LED(); // recalculate;

        PWM_SET(res);
    }
    
}
```

另外， 由于输出的是脉宽调制波, 所以其实并不是积分的波形，而灯泡是按照平均功率亮起的。 
![[attachments/Pasted image 20240110201836.png|500]]

为了保证在呼吸灯运行过程中, LED能够正常显示, 我们思考如何减少中断的数量 : 
由于PWM的周期是1ms, 而实际上中断却执行了100次,  这个是非常耗费资源和时间的, 由于中断延时器完全可以延时达到1ms, 我们只需设置两次中断,  即计算好两次定时器中断的时间即可。 

我们在主循环里面放置一个三角状变化的变量t, 表示执行次数, 则。

首先我们设10us为中断周期T , 此时延时t个10us, 对应的定时器初值是: 
INI = (int)(65536 - 9.246 * t)
只需设置高位为: 
TH = INI >> 8;
TL = INI & 0x0f;
延时一个中断，再设置初值为:
INI2 = (int) (65536 - 9.246 * (100 -t))  = (int) (64611 + 9.246 * t);
另外减小t的变化范围, 减小为5-65


因此每一次自增的时间约为两个LED 进行Show的时间(约为4-5ms), 设置总的周期数为200即可。 每一次变量自增时，重新计算和赋值两个中断次数, 这样就实现了有效减小中断的次数。 

需要说明的是， 不能使用PCF8591和XPT2046解读脉宽调制的模拟量。

修改完毕之后， 代码入下: 
```c 
#include <REGX52.H>
#include "public.h"
#include "LED_controller.h"
#include "EEPROM_OP.h"
#define PI 3.14

sbit DAC_PORT = P2^1;

u8 G_TIMH_INT = 0x00, G_TIML_INT = 0x00; // interruption time initial value
u8 G_TIMH_WAT = 0x00, G_TIML_WAT = 0x00; // wait time value 


u8 LEDNum[2] = {0, 0}; 

///@brief Init_dutyt is 5-10 (not zero), give LED initial light, tim_scale = 100 (as default)
void PWM_INIT(u8 Init_duty){
    u16 TIMINI ;
    u16 TIMWAT ;
    TIMINI = (int)(65536.0 - 9.246 * Init_duty);
    TIMWAT =  (int)(64611.4 + 9.246 * Init_duty);
    G_TIMH_INT = TIMINI / 256;
    G_TIML_INT = TIMINI % 256;
    G_TIMH_WAT = TIMWAT / 256;
    G_TIML_WAT = TIMWAT % 256;

    DAC_PORT = 1; // set DAC port as 1 to init.
    // also storged in the global variables.  
    TMOD &= ~T0_GATE_;        // set T0 gate as 0;
    TMOD &= ~T0_CT_;          // set T0 CT as 0; (工作在定时方式)
    TMOD |= T0_M0_;           // set the MO to 1;
    TMOD &= ~T0_M1_;          // set the M1 to 0; 
    EA = 1; ET0 = 1;          // set interrupt gate as open 
    TH0 = G_TIMH_INT; TL0 = G_TIML_INT; // set the initial value (for count)
    TR0 = 1;                  // start the timer 0 (note if timer 1 isTR1) 
}

u8 timer_mode = 1; // timer_mode record the current state of PORT should be;

// 1ms as total function
void TIMER0_FUNC () interrupt 1{
    if (timer_mode){// enter wait mode 
        timer_mode = 0; // set timer mode to another
        DAC_PORT = 0;
        TH0 = G_TIMH_WAT;
        TL0 = G_TIML_WAT;
    }else{
        timer_mode = 1; // set timer mode to another
        DAC_PORT = 1;
        TH0 = G_TIMH_INT;
        TL0 = G_TIML_INT;
    }
}

void PWM_SET(u16 duty_new){
    u16 TIMINI, TIMWAT;
    TIMINI = (int)(65536.0 - 9.246 * duty_new);
    TIMWAT =  (int)(64611.4 + 9.246 * duty_new);
    G_TIMH_INT = TIMINI / 256;
    G_TIML_INT = TIMINI % 256;
    G_TIMH_WAT = TIMWAT / 256;
    G_TIML_WAT = TIMWAT % 256;
}

void Show_LED(u16 res){
    // 0000 0011
    float Voltage_res = 5.0 * (float)res / 100;  // get voltage of pin AIN3
    LEDNum[0] = ((int)Voltage_res)%10;
    LEDNum[1] = ((int)(Voltage_res * 10))%10;
}

void main(){
    u16 res = 5, mode = 0;
    PWM_INIT(5); // 0xff, 0xe9, time_scale, 0
    while (1)
    {
        LED_ShowNum(0, LEDNum[0], 1); // -> need about 2ms
        LED_ShowNum(1, LEDNum[1], 0); // -> 4ms 
		Delay(10);
        // 0.6 us
        if (mode == 0){
            res ++;
            if (res == 95){
                mode = 1;
            }
        }
        else{
            res--;
            if (res == 5){
                mode = 0;
            }
        }
        PWM_SET(res);
        Show_LED(res); // recalculate;
    } 
}
```

使用这个代码可以清晰不闪烁地看到数字随着灯的亮灭变化， 并可以获取到相应的仿真。

![[attachments/Pasted image 20240110214954.png|700]]


