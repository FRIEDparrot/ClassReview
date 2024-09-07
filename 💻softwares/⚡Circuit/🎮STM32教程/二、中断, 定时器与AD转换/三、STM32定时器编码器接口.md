---
~
---
# 一、定时器编码器接口的概念
通过定时编码器接口， 可以读取旋转编码器的速度， 同时实现全自动的读取获得。 
定时器的编码器接口(Encoder Interface) , 可以<b><mark style="background: transparent; color: blue">接受增量正交编码器的信号</mark></b>,  可以获取正交编码器旋转产生的正交脉冲实现自动控制CNT进行自增或者自减，来指示编码器的旋转方向与速度。 

对于旋转编码器， **实际上也是每隔一段时间取一次计次值**, 获取编码器的旋转速度。 
可以实现全自动的速度测量， 例如用于电机的 PID 控制， 而由于**电机的旋转速度比较高， 往往使用无接触式的霍尔传感器或者光栅进行测速**, 

编码器接口相当于是**带有方向控制的外部时钟**， 可以同时控制 CNT 的计数时钟和计数方向。
速度可以每一次取值并清零进行获取。 
编码器接口可以**实现进行自增计次和自减计次**。

每个高级和通用定时器均有1个编码器接口， 如果编码器较多, 可使用F407(有14个定时器) 
对于编码器的两个输入引脚是**借用输入捕获通道1和输入捕获通道2, 即对应CH1和CH2引脚**。 

通过在定时器中测量正交编码器的频率， 即可获取旋转速度。 

正交信号中, 首先**计次精度更高**(AB相均可计次, 频率提高一倍)
第二是正交信号可以抗噪声(一相不变而另一项交替时认为是噪声)

正交信号的计次原理: **要求产生下图的完整序列**: 左图为正转, 右图为反转
![[attachments/Pasted image 20240211154124.png|600]]
例如: **正转**时包括**A上升B低, B上升A高, A下降B高和B下降A低四个序列**, 而反转对应的高低电平状态与此刚好相反。 

计数器是增加或者减少由另一相的状态进行确定。 如果出现在上面的表中， 则计次自增; 否则计次自减。因而实际上一般旋转得到的是4的倍数。 

针对编码器接口， 实际接入的是TI1FP1和TI2FP2, 借用CH1和CH2的配置， **并使用了CH1和CH2的边沿检测和编码器接口**。但**不使用计数时钟和计数方向**。交叉，预分频器和CCR与编码器无关。 
![[attachments/Pasted image 20240211162307.png|700]]
输出时，编码器接口控制CNT计数时钟的自增和自减操作。 

需要注意的是, 我们在编码模式状态下, 往往设置ARR为65535最大, 从而**利用补码得到复数, 将16位无符号数转换成为16位的有符号整数**, 例如65535对应-1等等。

对于编码信号，在下图中编码模式的计数方法给出了TI1FP1和TI2FP2分别在上升和下降沿情况下的TI1, TI2计数情况。 
![[attachments/Pasted image 20240211163917.png|700]]
图中看出: 
1. TI2FP2 = High, 则TI1FP1 = Falling `->` 向上计数。(TI1FP1超前90度时, 上升)
2. TI1FP2 = LOW 则TI1FP1 = Rising `->` 向上计数。

编码器分为三种工作模式， 包括仅在TI1计数, 仅在TI2计数和TI1和TI2都计数。
如果是第1,2种工作状态, 分别是仅在A 相的上升沿或者下降沿自增或者自减； 仅在B相的上升下降沿自增自减，以及全都自增或者自减, 其中**采用全部的精度较高**。 

计时期间的时序图参考下图(p393) 
![[attachments/Pasted image 20240211165144.png]]
另外也可以在GPIO中配置极性选择可以选择输入反相的操作。 

如果选择上升沿参数，则信号直通且高低电平不反转； 而选择下降沿的参数则加非门反转极性。 
The timer, when configured in Encoder Interface mode provides information on the sensor’s current position. The user can obtain dynamic information (speed, acceleration, deceleration) by measuring the period between two encoder events using a second timer configured in capture mode.

这个说明**编码模式可以获取旋转编码器的当前位置**。而当获取编码器的动态参数时, **可以使用另一个定时器，将其设置为输入捕获模式并测量两次编码器事件的间隔**(将输入来源设置为编码器事件对应的定时器)从而获取速度，加速度等等参数。

# 二、软件编程实现位置测量
要求: 显示屏当前旋钮的位置和速度。其中以初始位置为0位置。
在实际的编程中， 使用TIM3接入编码器,  其中PA6和PA7用于输入编码模式的捕获

在捕获之后，使用TIM2测量相应的间隔， 并计算速度的大小。 

基本流程:
1. RCC开启时钟(开启GPIO和定时器时钟)
2. 配置GPIO (PA6, PA7配置为输入模式) 
3. 配置时基单元， 一般配置为编码器模式， 自动重装ARR一般给定65535, 而PSC一般给1即可, 只需使用一个硬件自带的CNT进行计数即可。
4. 配置输入捕获单元 `->` 包括滤波器和极性参数选择 
5. 配置编码器接口模式 
6. 启动定时器  

```cpp 
void TIM_EncoderInterfaceConfig(TIM_TypeDef* TIMx, uint16_t TIM_EncoderMode,
                                uint16_t TIM_IC1Polarity, uint16_t TIM_IC2Polarity); // 进行定时器编码器的接口配置, 参数1为定时器, 2为模式, 3和4选择两个通道的电平极性

uint16_t TIM_GetCounter(TIM_TypeDef* TIMx); // 获取编码计数器的值
```

编码器的配置首先需要进行端口IC 配置, 方法是利用上升沿和下降沿配置每一个端口是否反相， <mark style="background: transparent; color: red">如果指定触发上升沿则不反相， 下降沿则反相。 </mark> 
```cpp
TIM_ICInitTypeDef* TIM3_ICInitStruct = new TIM_ICInitTypeDef();
// not use ICInit to initialize 1 by 1 here 
TIM_ICStructInit(TIM3_ICInitStruct); 
TIM3_ICInitStruct ->TIM_Channel = TIM_Channel_1 ;
TIM3_ICInitStruct ->TIM_ICFilter = 0xF;
// set Rising edge here means not reverse high and low voltage level
// if use rising edge, the corresponding port will be reversed 
TIM_ICInit(TIM3, TIM3_ICInitStruct); 
TIM3_ICInitStruct -> TIM_Channel = TIM_Channel_2;
TIM_ICInit (TIM3,  TIM3_ICInitStruct); // 其中极性配置可以在Encoder Config函数中进行

TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
```


```cpp title:旋转编码器精确显示旋转次数(或者测量位置)的代码
#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"

using namespace std;

void Encoder_Init(){ // Init the encoder timer 3
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, FunctionalState::ENABLE);
    GPIO_InitTypeDef* GPIO_InitStruct = new GPIO_InitTypeDef();
    GPIO_InitStruct->GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct->GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStruct->GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, GPIO_InitStruct);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, FunctionalState::ENABLE);
    TIM_TimeBaseInitTypeDef* TIM3_TBInitStruct = new TIM_TimeBaseInitTypeDef();
    TIM3_TBInitStruct -> TIM_ClockDivision = TIM_CKD_DIV1;
    TIM3_TBInitStruct -> TIM_CounterMode = TIM_CounterMode_Up;
    TIM3_TBInitStruct -> TIM_Period = 0xffff;  // 65535 
    TIM3_TBInitStruct -> TIM_Prescaler = 1 - 1; // 0 
    TIM3_TBInitStruct -> TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, TIM3_TBInitStruct);

    TIM_ICInitTypeDef* TIM3_ICInitStruct = new TIM_ICInitTypeDef();
    // not use ICInit to initialize 1 by 1 here 
    TIM_ICStructInit(TIM3_ICInitStruct);
    TIM3_ICInitStruct ->TIM_Channel = TIM_Channel_1;
    TIM3_ICInitStruct ->TIM_ICFilter = 0xF;
    // set Rising edge here means not reverse high and low voltage level
    // if use rising edge, the corresponding port will be reversed 
    TIM_ICInit(TIM3, TIM3_ICInitStruct);  
    TIM3_ICInitStruct -> TIM_Channel = TIM_Channel_2;
    TIM_ICInit (TIM3,  TIM3_ICInitStruct);

    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
}

void Encoder_Start(){
    TIM_Cmd(TIM3, FunctionalState::ENABLE);

}

// get the location of the rotate encoder.
int16_t GetCNT(){ // signed integar
    return TIM_GetCounter(TIM3);
}

int main(){
    OLED_Init();
    Encoder_Init();
    Encoder_Start();
    int cnt;
    while (1)
    {
        OLED_ShowSignedNum(2,1,((int16_t)GetCNT())/4,5);
    }
}
```

如果希望清零cnt, 则可以在调用之后使用: 
```cpp
TIM_SetCounter(TIM3,  0);  // 清零计数器
```


# 三、利用定时器实现精确少延时测量旋钮的速度

我们使用定时器2来测量旋钮的速度, 首先, 基本的过程是通过TIM3的触发TIM2定时器，  每一次触发时, 都会比较与上一次触发的时间, 从而计算出旋钮的旋转速度。 

首先一圈是20次旋钮, 因此旋钮一次旋转角度为 18度, 此时TIM3每一次的触发, 都会被TIM2所记录。 即获取TIM2两次触发的间隔来获取角速度。
测量旋钮速度的基本方法是测频法，即使用输入捕获功能,  基本原理与测量PWM波的频率类似。

测速方法是使用从模式并且以Encoder为输入源， 

方法一是<mark style="background: transparent; color: red">使用 TIM2的定时器定时中断, 每隔1s进行中断并读取相应的速度</mark>， 但是这样往往会导致频繁请求中断，同时刷新显示的频率也较慢。
```cpp title:定时器中断方式
extern "C"{
	void TIM2_IRQHandler(void){
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
		{
			Speed = Encoder_Get();
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		}
	}
}
```

参考[[💻softwares/⚡Circuit/🎮STM32教程/二、中断, 定时器与AD转换/二、STM32定时器中断的使用#(2) 定时器的内部结构|定时器的内部结构]], 基本的方法是让TRGI作为内部时钟, 
我们参考`TIM_InternalTriggerConnection`(p408), 对于TIM2, 其触发源ITR2为TIM3, 因此可以将InputTrigger设置为ITR2来实现 TIM3 触发 TIM2
`TIM_SelectInputTrigger(TIM2, TIM_TS_ITR2);` 
每一次TIM3触发时,  TIM2自动回零
```cpp
TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);
```

而基本的思路是每次GetCapture(TIM3), 且在获得满时, 显示为0, 在不满时, 计算对应的角速度, 频率越高, 速度越快。
我们仅考虑一种模式, 在这种模式下, 主循环会快速准确读取旋钮的位置和速度。
由于编码器作为编码之后, 就失去了触发的能力, 因此可以考虑使用软件进行计数和触发。

```cpp title:在主程序中,测量角速度以及位置
#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"
#include <algorithm>

using namespace std;

void Encoder_Init(){ // Init the encoder timer 3
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, FunctionalState::ENABLE);
    GPIO_InitTypeDef* GPIO_InitStruct = new GPIO_InitTypeDef();
    GPIO_InitStruct->GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct->GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStruct->GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, GPIO_InitStruct);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, FunctionalState::ENABLE);
    TIM_TimeBaseInitTypeDef* TIM3_TBInitStruct = new TIM_TimeBaseInitTypeDef();
    TIM3_TBInitStruct -> TIM_ClockDivision = TIM_CKD_DIV1;
    TIM3_TBInitStruct -> TIM_CounterMode = TIM_CounterMode_Up;
    TIM3_TBInitStruct -> TIM_Period = 0xffff;  // 65535 
    TIM3_TBInitStruct -> TIM_Prescaler = 1 - 1; // 0 
    TIM3_TBInitStruct -> TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, TIM3_TBInitStruct);

    TIM_ICInitTypeDef* TIM3_ICInitStruct = new TIM_ICInitTypeDef();
    // not use ICInit to initialize 1 by 1 here 
    TIM_ICStructInit(TIM3_ICInitStruct); 
    TIM3_ICInitStruct ->TIM_Channel = TIM_Channel_1 ;
    TIM3_ICInitStruct ->TIM_ICFilter = 0xF;
    // set Rising edge here means not reverse high and low voltage level
    // if use rising edge, the corresponding port will be reversed 
    TIM_ICInit(TIM3, TIM3_ICInitStruct);  
    TIM3_ICInitStruct -> TIM_Channel = TIM_Channel_2;
    TIM_ICInit (TIM3,  TIM3_ICInitStruct);

    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
}

void Encoder_Start(){
    TIM_Cmd(TIM3, FunctionalState::ENABLE);
}

// use TIM2 for as IC mode for measuring the speed of rotator
void TIM2_Init(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, FunctionalState::ENABLE);
    GPIO_InitTypeDef* GPIO_InitStruct = new GPIO_InitTypeDef();
    GPIO_InitStruct->GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct->GPIO_Pin = GPIO_Pin_0;   // PA0 as the 
    GPIO_InitStruct->GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, GPIO_InitStruct);
    // Init the Timer2
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, FunctionalState::ENABLE);
    TIM_InternalClockConfig(TIM2);  // according to the internal clock and count automatically 

    TIM_TimeBaseInitTypeDef* TIM2_TBInitStruct = new TIM_TimeBaseInitTypeDef();
    TIM2_TBInitStruct->TIM_ClockDivision = TIM_CKD_DIV1;
    TIM2_TBInitStruct->TIM_CounterMode = TIM_CounterMode_Up;
    TIM2_TBInitStruct->TIM_RepetitionCounter = 0;
    TIM2_TBInitStruct->TIM_Prescaler = 720 - 1; // 10^-5 per count 
    TIM2_TBInitStruct->TIM_Period = 30000 - 1;  // if theres no instance in 0.3 sec, will reset to 0
    TIM_TimeBaseInit(TIM2, TIM2_TBInitStruct);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);

    TIM_Cmd(TIM2, ENABLE);
}

int16_t loc = 0;
float velocity = 0;

// get the location of the rotate encoder. 
void GetLocVelocity(){ // signed integar
    static int16_t pre_loc = 0;
    static int16_t t_pre = 0;
    static int16_t t_sum = 0;

    loc = (int16_t)TIM_GetCounter(TIM3)/4;
    int t_cur = TIM_GetCounter(TIM2);

    // 30000 correspond to ARR settings 
    int16_t interval= t_cur - t_pre >= 0? t_cur - t_pre: t_cur-t_pre + 30000; // caution! for signed int -> maximum 32767
    t_pre = t_cur;

    if (loc!=pre_loc){
        velocity = (loc - pre_loc) * 18 * 10000/(float)t_sum; 
        pre_loc = loc; t_sum = 0; // reset t_sum to start count;
    }

    // prevent t_sum overflow 
    if (t_sum < 30000) t_sum = t_sum + interval < 0? 30000: t_sum + interval; 
    else velocity = 0;  // reset velocity count
}

int main(){
    OLED_Init();
    Encoder_Init();
    Encoder_Start();
    TIM2_Init();
    // int cnt;
    while (1)
    {
        GetLocVelocity();
        OLED_ShowSignedNum(1,1, loc, 5);
        float vec; 
        if (velocity < 0){
            vec = -velocity;
            OLED_ShowChar(2,1,'-');
            OLED_ShowNum(2,2, vec, 4);
        }else{
            vec = velocity;
            OLED_ShowChar(2,1,'+');
            OLED_ShowNum(2,2, vec, 4);
        }
        OLED_ShowChar(2,6, '.');
        OLED_ShowNum(2,7,(vec-(int)vec * 100), 2);
        OLED_ShowString(2,9,"deg/s");
    }
    
}
```

另外需要说明的是, 由于软件进入计时函数的时间有限, 则速度并不是准确值。

而较为准确的可以采用中断获取。 
