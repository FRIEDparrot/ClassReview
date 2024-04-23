## 一、基本定时器操控编程
参考[[💻softwares/⚡Circuit/🎮STM32教程/二、中断, 定时器与AD转换/二、STM32定时器中断的使用|二、STM32定时器中断的使用]]中的PWM输出部分;
`TIM_CtrlPWMOutputs(TIM2, FunctionalState::ENABLE)`; only for TIM 1 and TIM8

对于每一个General TIMER, 都具有4个Channel, 每一个Channel对应一个比较的值, 以Channel2 为例, 则需要设置CCR2的值为比较的值，而调用下面一组的部分为比较值的设定和重设: 

```cpp
TIM_OC2Init();
TIM_SetCompare2();
```
需要注意的是下面这个补全，一定不能选带 N 的部分， 否则直接无法初始化
![[attachments/Pasted image 20240415000346.png]]

注意是不带N的补全而不是带N的补全!!!!! 

## 二、使用DMA进行LED插值数据的转运

> [!caution] STM32DMA外设
> 根据数据手册描述, stm32F103c8t6中容量只有DMA1外设可用, 参考p275
> 如果试图操作DMA2, 则会出现HardError, 对应的处理函数会在 stm32f10x_it.c中跳入死循环。

参考[[💻softwares/⚡Circuit/🎮STM32教程/二、中断, 定时器与AD转换/四、ADC与DMA存储器转运#四、DMA 直接存储器读取|直接存储器读取]], 由于在一个亮->灭周期内, 先计算出所有的插值值, 存储在
![[Excalidraw/RGB LED 的PWM操控 2024-04-15 09.42.45]]
DAM分为DMA1和DMA2, 共有12个通道

对于TIM寄存器地址的查询， 先在数据手册中找到TIM1起始地址为0x4001 2C00, TIM1_CCR3的偏移地址为 0x3C, 则寻址地址为0x40012C3C, 对于TIM2, 地址为 0x4000 0000

我们使用三个SRC_Buffer1, SRC_Buffer2, SRC_Buffer3 来保存一定时刻的 R, G, B 的占空比数据, 并分别调用三个DMA进行转运, 转运过程中， 每一次在指定的DMA更新次数后，更改占空比的大小。 并且不产生中断。而每一次在转运完毕之后。

```cpp
官方库中例程的描述如下:
/*
TIM1CLK = SystemCoreClock, Prescaler = 0, TIM1 counter clock = SystemCoreClock
  SystemCoreClock is set to 72 MHz for Low-density, Medium-density, High-density
  and Connectivity line devices and to 24 MHz for Low-Density Value line and
  Medium-Density Value line devices.
  
  The objective is to configure TIM1 channel 3 to generate complementary PWM
  signal with a frequency equal to 17.57 KHz:
     - TIM1_Period = (SystemCoreClock / 17570) - 1
  and a variable duty cycle that is changed by the DMA after a specific number of
  Update DMA request.
  
  The number of this repetitive requests is defined by the TIM1 Repetition counter,
  each 3 Update Requests, the TIM1 Channel 3 Duty Cycle changes to the next new 
  value defined by the SRC_Buffer . 
*/
这个程序就是通过DMA来自动重置占空比的程序。这样我们就能实现仅在颜色更换完之后进入一次中断计算两次颜色更新之间的颜色插值即可， 而颜色更换过程中的部分交给DMA处理即可。
```

### (1) 更新事件及其概念
每一次计数器溢出时， 自动重装并产生更新事件,  
在 TIM_Prescaler_Config 中, 每一个TIM_Prescaler对应有两个ReloadMode, 其中 Update 表示在设置完毕后, 等待下一个 Update 事件之后设置(建议用这个), immediate 表示设置时立刻设置。一般选用 Update 即可。
```cpp
@arg TIM_PSCReloadMode_Update: The Prescaler is loaded at the update event.
@arg TIM_PSCReloadMode_Immediate: The Prescaler is loaded immediately.
```

注意: 通过 TIM_RepetitionCounter = 5; 是设定重复计数次数, 对应寄存器是 TIMx_RCR, 
1. Repetition Count 只有 TIM1 和 TIM8 才能使用, 因此 TIM2 没有 Repetition Counter 选项。之后可以使用 TIM8 来操控闪烁灯来变换灯光整体闪烁的快慢。
RepetitionCounter可以设置更新事件的触发方式, 等到看TIM1再研究

### (2) DMA 触发源选择
如图, <mark style="background: transparent; color: red">TIM_DMACmd 函数是选取 DMA 触发源的函数, 也是TIM的DMA设置中最为重要的函数</mark>，当设置为Update之后, 则每一次定时器更新之后, 就会触发 DMA 转运一次。另外也可以在其他时机触发转运。<mark style="background: transparent; color: red">但是, 也可以配置其他的转运触发源</mark>, 

```cpp title:DMA触发源选择函数
/* ENABLE the DMA request by using TIM_DMACmd */
TIM_DMACmd(TIM2, TIM_DMA_Update, FunctionalState::ENABLE);  // use TIM update as interrupt source for DMA reload
```
另外, UpdateDMA request  enable 是指访问寄存器时, Update时, 允许访问一次。实际上是设置 TIMx -> DIER参数, 因此可以同时配置多个中断源, 即可以用 `|` 连接第二个参数

> [!caution] 注意
> 为了让DMA能够转运, 必须将DMA_Priority设置为高等级High, 这样才能实现转运, 否则DMA不能正常工作;

第二, <b><mark style="background: transparent; color: blue">DMA, 各类外设和定时器的关系是一个定时器的各个通道和DMA有严格的对应关系,</mark></b> 在p281有DMA1的请求对应的每个外设的情况， 具体重点如下表: 
![[attachments/Pasted image 20240415213407.png]]

其中, TIM1-UP, TIM2-UP, .... TIM4-UP 是定时器 Update 产生的事件, 而TIM_CHx是TIM_CHx < 或者 > 产生的事件, 我们一般用到的是 TIMx_UP 事件, 例如<b><mark style="background: transparent; color: blue">必须使用DMA-Channel2端口</mark></b>
获取 TIM2的触发事件。

==但是, 我们也可以利用其他的事件==, 我们考虑采用如下方法: 
1. 使用 TIM4_CH1事件为标志转运 TIM4_CCR1 
2. 使用 TIM4_CH2事件为标志转运 TIM4_CCR2 
3. 使用 TIM4_CH3事件为标志转运 TIM4_CCR3 
这样我们实现多通道DMA转运和RGB呼吸灯，会出现BUG, 这是由于每一次转运时机不是 Update 造成的, 如果不是Update 时才转运, 则导致修改占空比之后， 若CH增加会导致继续触发, 导致某些通道实际的装填迅速完成, 产生装填不同步BUG 

> [!hint] 说明 
> 由于难以转运多个通道, 如果是单色的呼吸灯, 可以使用 DMA 进行数据转运, 但是多通道的RGB操控, 还是考虑使用中断来进行;

如果采用上述方式， 在DMACmd中， 设置中断允许位如下:
```cpp title:设置中断允许位
/* ENABLE the DMA request by using TIM_DMACmd */
TIM_DMACmd(TIM2, TIM_DMA_CC1 | TIM_DMA_CC2 | TIM_DMA_CC3, FunctionalState::ENABLE);  //**use TIM update as interrupt source for DMA reload
```

下面的代码演示了将数组中的部分利用 DMA 转运到 TIM4_CCR1
```cpp fold
#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"

#define MAX_PWM_PERIOD 100     // Timer2 reload value 

#define TIM4_CCR1_Address    0x40000834
#define TIM4_CCR2_Address    0x40000838
#define TIM4_CCR3_Address    0x4000083C

uint16_t LIGHT_INTENSE_B[3] = {100, 100, 100};  // Blue  Duty

void LED_GPIO_Config(void){
    /*======================== GPIO Output Init =====================*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, FunctionalState::ENABLE);
    // Use PA1 as AFPP mode
    GPIO_InitTypeDef* GPIO_InitStruct = new GPIO_InitTypeDef();
    GPIO_InitStruct->GPIO_Mode = GPIOMode_TypeDef::GPIO_Mode_AF_PP;
    GPIO_InitStruct->GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
    GPIO_InitStruct->GPIO_Speed = GPIOSpeed_TypeDef::GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, GPIO_InitStruct);
}

// DMA -> (DMA1_C1 for TIM4_CH1, DMA1_C4 for TIM4_CH2, DMA1_C5 for TIM4_CH3)
void LED_DMA_Config(void){
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, FunctionalState::ENABLE);
    // DMA is used for tranform the memory(RGB array) to 
    DMA_InitTypeDef* DMA_InitStruct1 = new DMA_InitTypeDef();
    DMA_StructInit(DMA_InitStruct1);
    DMA_InitStruct1->DMA_Mode     = DMA_Mode_Circular;
    DMA_InitStruct1->DMA_Priority = DMA_Priority_High;      // ** caution : this must be at least "high"
    DMA_InitStruct1->DMA_M2M      = DMA_M2M_Disable;
    DMA_InitStruct1->DMA_BufferSize= 3;
    DMA_InitStruct1->DMA_DIR      = DMA_DIR_PeripheralDST;  // from Memory to Peripheral 

    DMA_InitStruct1->DMA_MemoryBaseAddr = (uint32_t)LIGHT_INTENSE_B;  // from R 
    DMA_InitStruct1->DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStruct1->DMA_MemoryInc      = DMA_MemoryInc_Enable;

    DMA_InitStruct1->DMA_PeripheralBaseAddr = (uint32_t)TIM4_CCR1_Address;     
    DMA_InitStruct1->DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
    DMA_InitStruct1->DMA_PeripheralInc      = DMA_PeripheralInc_Disable;        

    // use DMA1 Channel 1 for CH
    DMA_Init(DMA1_Channel1, DMA_InitStruct1);
    DMA_Cmd(DMA1_Channel1, FunctionalState::ENABLE);  /* Enable the DMA1-5 for transforming */
}

/* Use Timer4 as the Timer for RGB LED -> Output Compare Mode in 1,2,3 channels for R, G, B */
void LED_Timer_Config(void){
    /*======================= Timer Base Init ======================*/
    // Init TIM4 TimeBase Unit -> USE  CH2, CH3, CH4 all 3 ports, The Period is 50ms for smooth transform 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, FunctionalState::ENABLE);

    TIM_DeInit(TIM4);
    TIM_InternalClockConfig(TIM4);                                  // use Internal clock as clock source;
    TIM_TimeBaseInitTypeDef* TIM4_TimeBaseInitStruct = new TIM_TimeBaseInitTypeDef();
    TIM_TimeBaseStructInit(TIM4_TimeBaseInitStruct);                   // counter: up
    TIM4_TimeBaseInitStruct->TIM_CounterMode   = TIM_CounterMode_Up;
    TIM4_TimeBaseInitStruct->TIM_Prescaler     = 7200 - 1;             // 0.1ms per tick
    TIM4_TimeBaseInitStruct->TIM_Period        = MAX_PWM_PERIOD - 1;   // 0.01s per cycle
    TIM_TimeBaseInit(TIM4, TIM4_TimeBaseInitStruct);

    /*===========  Init Different Channels ===============*/
    /*===========  Init OC3 for Green ====================*/
    /*==========   Init OC4 for BLUE =====================*/
    /* Init OC2 for RED */   /*@note: use the Channel 2 as port, use TIM_OC2Init and TIM_SetCompare2*/
    TIM_OCInitTypeDef* TIM4_OC1InitStruct = new TIM_OCInitTypeDef();  // Output Compare Mode -> used for generate PWM wave
    TIM_OCStructInit(TIM4_OC1InitStruct);
    TIM4_OC1InitStruct -> TIM_OCMode     = TIM_OCMode_PWM1;          // TIMx_CNT < TIMx_CCRx high, otherwise low;
    TIM4_OC1InitStruct -> TIM_OCPolarity = TIM_OCPolarity_High;      // CCER -> CC2P 
    TIM4_OC1InitStruct -> TIM_OutputState= TIM_OutputState_Enable;   // TIM2 Output in CH2
    TIM4_OC1InitStruct -> TIM_Pulse  = 0;                            // pulse value in Capture Compare Register.(CCR2)
    TIM_OC1Init(TIM4, TIM4_OC1InitStruct);                           // output Channel2 (TIM2_CH2, PA1);
    
    TIM_OCInitTypeDef* TIM4_OC2InitStruct = new TIM_OCInitTypeDef();
    TIM_OCStructInit(TIM4_OC2InitStruct);
    TIM4_OC2InitStruct -> TIM_OCMode      = TIM_OCMode_PWM1;
    TIM4_OC2InitStruct -> TIM_OCPolarity  = TIM_OCPolarity_High;
    TIM4_OC2InitStruct -> TIM_OutputState = TIM_OutputState_Enable;
    TIM4_OC2InitStruct -> TIM_Pulse       = 0;
    TIM_OC2Init(TIM4, TIM4_OC2InitStruct);    // output Channel3 (TIM2_CH3, PA2);

    TIM_OCInitTypeDef* TIM4_OC3InitStruct = new TIM_OCInitTypeDef();
    TIM_OCStructInit(TIM4_OC3InitStruct);
    TIM4_OC3InitStruct -> TIM_OCMode      = TIM_OCMode_PWM1;
    TIM4_OC3InitStruct -> TIM_OCPolarity  = TIM_OCPolarity_High;
    TIM4_OC3InitStruct -> TIM_OutputState = TIM_OutputState_Enable;
    TIM4_OC3InitStruct -> TIM_Pulse       = 0;
    TIM_OC3Init(TIM4, TIM4_OC3InitStruct);    // output Channel4 (TIM2_CH4, PA3); -> all use one struct for init

    /* ENABLE the DMA request by using TIM_DMACmd -> enable all CC1, CC2, CC3 event */
    TIM_DMACmd(TIM4, TIM_DMA_CC1 | TIM_DMA_CC2 | TIM_DMA_CC3, FunctionalState::ENABLE);  //**use TIM update as interrupt source for DMA reload
    
    TIM_Cmd(TIM4, FunctionalState::ENABLE);
}

void LED_Init(){
    LED_GPIO_Config();
    LED_DMA_Config();
    LED_Timer_Config();
}

#pragma endregion // PWM_Control

// use usart3 (PB10, PB11 as the uart)
int main(){
    OLED_Init();
    LED_Init();
	while(1){
    }
}
```

另外我们还可以精简代码，定时器三个端口显然可以通过一个数组初始化。


### (3) 使用定时器中断实现呼吸变色灯功能



```cpp title:中断呼吸RGB_LED灯的代码
#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"
#include "stdlib.h" // srand function

#define MAX_PWM_PERIOD 100   // Timer2 reload value
#define TIM4_CCR1_Address    0x40000834
#define TIM4_CCR2_Address    0x40000838
#define TIM4_CCR3_Address    0x4000083C

/* for adjust the mix proportion of each light */
float RED_INTERNSE_PRESCALER   = 0.5;
float GREEN_INTENSE_PERSCALER  = 0.5;
float BLUE_INTENSE_PRESCALER   = 0.5;

/*  MODE0: ONLY USE ONE COLOR LIGHT
    MODE1: ONE COLOR LIGHT AND DIM
    MODE2: MULTI COLOR LIGHT AND DIM
    MODE3(BLEND): MULTI COLOR LIGHT TRANSFORM */
uint8_t RGBLED_MODE = 2;

//******   Color  Definition     ****** 
struct RGBColor{
    uint8_t R = 0;
    uint8_t G = 0;
    uint8_t B = 0;
    RGBColor(uint8_t R, uint8_t G, uint8_t B){
        this->R = R;
        this->G = G;
        this->B = B;
    }
};

RGBColor RGB_RED       = RGBColor(255, 0  , 0  );
RGBColor RGB_YELLOW    = RGBColor(255, 255, 0  );
RGBColor RGB_GREEN     = RGBColor(0  , 128, 0  );
RGBColor RGB_BLUE      = RGBColor(0  , 0  , 128);
RGBColor RGB_LIGHTBLUE = RGBColor(51 , 102, 255);
RGBColor RGB_LIGHTGEEEN= RGBColor(204, 255, 204);
RGBColor RGB_WHITE     = RGBColor(255, 255, 255);

/* @note: the method is compare with Compare register1 */
RGBColor RGBColorList[] = {RGB_RED, RGB_BLUE, RGB_GREEN, RGB_YELLOW, RGB_LIGHTBLUE, RGB_LIGHTGEEEN, RGB_WHITE};
uint8_t  RGBColorList_Size = 7;

// ****** used for light intense interpolation ***********
static float LIGHT_INTENSE_INTERP[100] = {0.00,0.01,0.03,0.04,0.05,0.06,0.07,0.08,0.09,0.10,0.10,0.12,0.13,0.14,0.15,0.17,0.18,0.20,0.22,0.24,0.26,0.29,0.33,0.37,0.42,0.47,0.52,0.57,0.61,0.66,0.70,0.73,0.77,0.81,0.84,0.87,0.90,0.92,0.94,0.96,0.97,0.98,0.98,0.99,0.99,0.99,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,0.99,0.99,0.98,0.98,0.97,0.96,0.94,0.93,0.91,0.89,0.86,0.84,0.81,0.77,0.73,0.69,0.65,0.60,0.55,0.50,0.44,0.39,0.34,0.29,0.23,0.21,0.19,0.17,0.16,0.14,0.13,0.12,0.11,0.10,0.09,0.08,0.07,0.06,0.06,0.05,0.04,0.04,0.03,0.02,0.01};
uint16_t LIGHT_INTENSE_R_BLEND[100];  // Red   Duty Array for blend 
uint16_t LIGHT_INTENSE_G_BLEND[100];  // Green Duty Array for blend 
uint16_t LIGHT_INTENSE_B_BLEND[100];  // Blue  Duty Array for blend

RGBColor curr_color = RGBColor(0,0,0); 
RGBColor next_color = RGBColor(0,0,0); // initialize by init function

uint16_t color_index1 = 0, color_index2 = 0;  // change light speed -> use color index2
uint8_t color_update_interval = 1;   // when color_index2 reach color_update_interval, color_index1++

// called in the interruption to calculate the interpolation before the next state
void RGBColor_Blend_Update(void){
    for (int i = 0; i < 100; i++) {
        float coef = MAX_PWM_PERIOD/256.0;  uint16_t j = 100 - i;
        LIGHT_INTENSE_R_BLEND[i] = (int)( RED_INTERNSE_PRESCALER  * coef * (j * curr_color.R  + i * next_color.R)/100);
        LIGHT_INTENSE_G_BLEND[i] = (int)( GREEN_INTENSE_PERSCALER * coef * (j * curr_color.G  + i * next_color.G)/100);
        LIGHT_INTENSE_B_BLEND[i] = (int)( BLUE_INTENSE_PRESCALER  * coef * (j * curr_color.B  + i * next_color.B)/100);
    }
}



/* RGB LED Configuration Functions */
void LED_GPIO_Config(void){
    /*======================== GPIO Output Init =====================*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, FunctionalState::ENABLE);
    // Use PA1 as AFPP mode
    GPIO_InitTypeDef* GPIO_InitStruct = new GPIO_InitTypeDef();
    GPIO_InitStruct->GPIO_Mode = GPIOMode_TypeDef::GPIO_Mode_AF_PP;
    GPIO_InitStruct->GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
    GPIO_InitStruct->GPIO_Speed = GPIOSpeed_TypeDef::GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, GPIO_InitStruct);
}

/* Use Timer4 as the Timer for RGB LED -> Output Compare Mode in 1,2,3 channels for R, G, B */
void LED_Timer_Config(void){
    /*======================= Timer Base Init ======================*/
    // Init TIM4 TimeBase Unit -> USE  CH2, CH3, CH4 all 3 ports, The Period is 50ms for smooth transform 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, FunctionalState::ENABLE);

    TIM_DeInit(TIM4);
    TIM_InternalClockConfig(TIM4);                                  // use Internal clock as clock source;
    TIM_TimeBaseInitTypeDef* TIM4_TimeBaseInitStruct = new TIM_TimeBaseInitTypeDef();
    TIM_TimeBaseStructInit(TIM4_TimeBaseInitStruct);                   // counter: up
    TIM4_TimeBaseInitStruct->TIM_CounterMode   = TIM_CounterMode_Up;   
    TIM4_TimeBaseInitStruct->TIM_Prescaler     = 7200 - 1;             // 0.1ms per tick
    TIM4_TimeBaseInitStruct->TIM_Period        = MAX_PWM_PERIOD - 1;   // 0.01s per cycle
    TIM_TimeBaseInit(TIM4, TIM4_TimeBaseInitStruct);
    /* Init OC2 for RED */   /*@note: use the Channel 2 as port, use TIM_OC2Init and TIM_SetCompare2*/
    TIM_OCInitTypeDef* TIM4_OCInitStruct = new TIM_OCInitTypeDef();  // Output Compare Mode -> used for generate PWM wave
    TIM_OCStructInit(TIM4_OCInitStruct);
    TIM4_OCInitStruct -> TIM_OCMode     = TIM_OCMode_PWM1;          // TIMx_CNT < TIMx_CCRx high, otherwise low;
    TIM4_OCInitStruct -> TIM_OCPolarity = TIM_OCPolarity_High;      // CCER -> CC2P 
    TIM4_OCInitStruct -> TIM_OutputState= TIM_OutputState_Enable;   // TIM2 Output in CH2
    TIM4_OCInitStruct -> TIM_Pulse      = 0;                        // pulse value in Capture Compare Register.(CCR2)
    TIM_OC1Init(TIM4, TIM4_OCInitStruct);                           // output Channel2 (TIM2_CH2, PA1);
    TIM_OC2Init(TIM4, TIM4_OCInitStruct);    // output Channel3 (TIM2_CH3, PA2);
    TIM_OC3Init(TIM4, TIM4_OCInitStruct);    // output Channel4 (TIM2_CH4, PA3); -> all use one struct for init
    
    // Use Timer as Interrupt source for the array update
    if (RGBLED_MODE == 0){
        TIM_ITConfig(TIM4,  TIM_IT_Update, FunctionalState::DISABLE);
        // for mode0, initialize the init Duty
        float coef = MAX_PWM_PERIOD /256.0;
        TIM_SetCompare1(TIM4, (int)(RED_INTERNSE_PRESCALER * curr_color.R * coef));
        TIM_SetCompare2(TIM4, (int)(GREEN_INTENSE_PERSCALER * curr_color.G * coef));
        TIM_SetCompare3(TIM4, (int)(BLUE_INTENSE_PRESCALER * curr_color.B * coef));
    }
    else{
        TIM_ITConfig(TIM4,  TIM_IT_Update, FunctionalState::ENABLE);
        /* NVIC Configurations */
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        NVIC_InitTypeDef* NVIC_InitStruct = new NVIC_InitTypeDef();
        NVIC_InitStruct->NVIC_IRQChannel = TIM4_IRQn;
        NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority = 2;
        NVIC_InitStruct->NVIC_IRQChannelSubPriority = 1;
        NVIC_InitStruct->NVIC_IRQChannelCmd = FunctionalState::ENABLE;

        NVIC_Init(NVIC_InitStruct);
    }
    TIM_Cmd(TIM4, FunctionalState::ENABLE);
}

// for mode 1, 2, 3
extern "C"{
    // note: use SetCurrentDataCounter in Interruption for sync all DMAs
    void TIM4_IRQHandler(void){
        if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET){
            // calculate color index
            if (color_index2 < color_update_interval){
                color_index2++;
            }else{
                color_index2 = 0;
                color_index1++;
                if (color_index1 >= 100){
                    color_index1 = 0;
                    // for mode 2 and 3, generate a new color
                    if (RGBLED_MODE == 2 || RGBLED_MODE == 3){
                        curr_color = next_color;
                        float r = (float)rand()/RAND_MAX * RGBColorList_Size;
                        uint16_t index = (int)r;
                        next_color = RGBColorList[index];
                    }
                    if (RGBLED_MODE == 3){
                        RGBColor_Blend_Update();
                    }
                }
            }
            // set the current color 
            switch (RGBLED_MODE)
            {
            case 1: // follow case 2 
            case 2: {
                float coef = LIGHT_INTENSE_INTERP[color_index1] * MAX_PWM_PERIOD/256.0;
                TIM_SetCompare1(TIM4, (int)(coef * curr_color.R * RED_INTERNSE_PRESCALER));
                TIM_SetCompare2(TIM4, (int)(coef * curr_color.G * GREEN_INTENSE_PERSCALER));
                TIM_SetCompare3(TIM4, (int)(coef * curr_color.B * BLUE_INTENSE_PRESCALER));
                break;
                }
            case 3:{
                TIM_SetCompare1(TIM4, LIGHT_INTENSE_R_BLEND[color_index1]);
                TIM_SetCompare2(TIM4, LIGHT_INTENSE_G_BLEND[color_index1]);
                TIM_SetCompare3(TIM4, LIGHT_INTENSE_B_BLEND[color_index1]);
                break;
                }
            default: break;
            }
        }
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    }
}

// Use this function for initialize or restart as another mode 
void LED_Init(RGBColor color){
    LED_GPIO_Config();
    TIM_Cmd(TIM4, FunctionalState::DISABLE);
    color_index1 = 0; color_index2 = 0;
    if (RGBLED_MODE == 0 || RGBLED_MODE == 1){
        curr_color = color;
    }else{
        curr_color = RGBColor(0,0,0);
    }
    next_color = color;
    LED_Timer_Config();
}

// use usart3 (PB10, PB11 as the uart)
int main(){
    OLED_Init();
    LED_Init(RGB_BLUE);
	while(1){
        
    }
}
```

此时我们即可实现相应的功能;
