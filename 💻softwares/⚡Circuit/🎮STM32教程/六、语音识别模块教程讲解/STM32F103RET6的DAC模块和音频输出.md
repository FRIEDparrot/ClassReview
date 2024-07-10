对于STM32F103RET6, 有2个12位的DAC转换, 并且可以<mark style="background: transparent; color: red">将两路的数字信号转换成模拟信号输出</mark>。
具有8个DAC Trigger 输入, DAC通道也可以通过DMA使用Timer 进行输出。

对于STM32， SPI1_NSS 和 SPI1_SCK (PA4, PA5)分别可以作为 DAC_OUT1 和 DAC_OUT2 
为了在使用DAC的同时能够保证SPI1有用, 可以使用 SPI1_SCK 和SPI1_MISO 的复用功能。

首先, 我们可以将 SPI1 复用至 SPI3 , 包括(PB3, PB4, PB5, NSS复用至PA10);

DAC 具有为 $V_{REF}$ 为2.4~3.3V 大小的参考电压。并可选用 8 位和 12 位DAC模式, 可选用 1-2个 Channel (各自具有独立的转换装置)。 同时, 每个通道均可使用DMA驱动。
- **8 位右对齐模式** 
- **12位右对齐模式** 
- **12位左对齐模式** 
并具有同步更新功能 (synchronized update capability), 可以同时进行双通道的转换。

除了软件触发外, DAC转换可以使用外部触发(External triggers), 其中包括SWTrigx, TIM2-TIM8,  TIM15 的触发方式, 即一般都是通过定时器来触发 DAC 转换的。<mark style="background: transparent; color: red">也支持EXTI line9 进行触发</mark> 
在使用 Init 函数时,可以
```cpp 
DAC_Init(DAC_Channel_1, DAC_InitStruct);
```

需要说明 , 一旦使用 DAC_Cmd 开启了某个通道的转换, 则自动PA4或者PA5会连接到其上。 此时, <mark style="background: transparent; color: red">为避免寄生损耗，必须将对应的PA4或者PA5设置为 AIN 模式</mark> (AIN 是Analog). 除此之外，还有设置 Output Buffer, 可以减小输出阻抗, 同时, 

DAC 根据 DAC_DHRyyyD 寄存器的内容, 进行数据的移位。
DAC 的底层是先将数据存储在 DHR1, DHR2 中, 再拿出存储的数据进行DA 转换的, 转换时会将数据先写入 DAC_DORx 寄存器中。(参考下面的基本框图) 
![[attachments/Pasted image 20240623005126.png]]
通过硬件的Trigger 或者 APB1 控制 12位 DHR 转运到 DOR 中, 当有硬件的Trigger设置时, 则会在Trigger 的三个时钟之后, 写入数据。 

对于 DAC 的电压计算, 使用DOR寄存器进行计算: 
$$DAC_{output} = V_{REF} \times \frac{DOR}{4096}$$
如果使用DMA进行转运, 也可以采用一个DMA通道来转运两个通道的 DAC 数据。具体取决于 DMAENx 位的设置。需要注意的是DMA和DAC的协调一致性必须尽可能进行保证。

### (3) DAC 噪声生成
对于进行伪噪声的生成，初始时, 12位寄存器值为0xAAA (101010101010), 然后进行逐步移位获取。触发是使用 WAVE[1:0] 为01
![[attachments/Pasted image 20240623013845.png]]
需要说明的是, 在噪声生成的情况下, 必须采用触发, 一般必须使用定时器。
当WAVE[1:0] 为"10"时, 则生成三角波。 
另外， **需要注意的是, MAMP[3:0]位必须在使能DAC之前进行设置**。这个决定了产生的三角波以及噪声波的幅值。这个会在结构体中进行配置如下:
![[attachments/Pasted image 20240623015900.png]]
当对于三角和噪声波使用时, 我们一般驱动音乐时,禁止即可。
![[attachments/Pasted image 20240623020100.png]]

主要的函数定义如下:  
```cpp
void DAC_DeInit(void);
void DAC_Init(uint32_t DAC_Channel, DAC_InitTypeDef* DAC_InitStruct);
void DAC_StructInit(DAC_InitTypeDef* DAC_InitStruct);
void DAC_Cmd(uint32_t DAC_Channel, FunctionalState NewState);
void DAC_DMACmd(uint32_t DAC_Channel, FunctionalState NewState);
void DAC_SoftwareTriggerCmd(uint32_t DAC_Channel, FunctionalState NewState);
void DAC_DualSoftwareTriggerCmd(FunctionalState NewState);
void DAC_WaveGenerationCmd(uint32_t DAC_Channel, uint32_t DAC_Wave, FunctionalState NewState);
void DAC_SetChannel1Data(uint32_t DAC_Align, uint16_t Data);
void DAC_SetChannel2Data(uint32_t DAC_Align, uint16_t Data);
void DAC_SetDualChannelData(uint32_t DAC_Align, uint16_t Data2, uint16_t Data1);
uint16_t DAC_GetDataOutputValue(uint32_t DAC_Channel);
```


对于DAC使用定时器作为触发时, 必须使用一个 TimeBaseInitStruct 来进行定时器的初始化(参考[[💻softwares/⚡Circuit/🎮STM32教程/二、中断, 定时器与AD转换/三、STM32定时器编码器接口|三、STM32定时器编码器接口]]以及[[💻softwares/⚡Circuit/🎮STM32教程/二、中断, 定时器与AD转换/二、STM32定时器中断的使用|STM32定时器中断的使用]]) 
```cpp
TIM_TimeBaseInitTypeDef TIM_InitStruct;
```

例如, 我们使用一个32位的正弦波, 且对应的周期为3s, 则首先定义正弦波基准为4095/2 = 2047, 然后进行插值即可:
```c
static const uint16_t Sine12bit[32] = {
                      2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056,
                      3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909,
                      599, 344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647};
```
对应的触发转运周期可以计算为:
3s / 32 = 93.75ms, 对应地,  可以计算出总的 prescaler 
$$\frac{3}{32}  /  \frac{1}{72MHz}  =  6750000$$
可以设置预分频为10000 - 1, 重装定时器 675 - 1 


需要注意的是, <mark style="background: transparent; color: red">如果需要输出双路的波形, 则可以使用 Dual 数组的模式, 其中高16位是一个DAC通道, 低16位为另一个DAC通道</mark>, 因此可以使用类似的代码:
```c
for (Idx = 0; Idx < 32; Idx++){
DualSine12bit[Idx] = (Sine12bit[Idx] << 16) + (Sine12bit[Idx]);
}
```

如果使用DMA, 则转运目标应当为 DHRx 寄存器, 其中DHR1寄存器的地址为:
DAC_channel1 12-bit right-aligned data  (DAC_DHR12R1), 即12位右对齐的部分。

DAC的地址为0x40007400, 对应偏移量为0x08, 则地址为 0x40007408 为DAC1的Channel ,注意: 当使用Dual模式时, 则转运到 0x40007420 处。

我们需要的效果是, 一次转运 16 个字节(右对齐), 且要求转运端数组处, 地址进行自增， 目标端处, 地址不自增。因此使用如下代码:
```cpp
DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
DMA_InitStruct.DMA_BufferSize = 32;  // 共转运 32 个数据， 然后返回起点。
```
![[attachments/Pasted image 20240213151600.png|800]]
方法即从 SRAM 转运到 Flash, 因此使用 const 放在 SRAM 中。
使用 TIM7 进行硬件触发 DMA 转运, 生成相应的正弦波。

参考[[💻softwares/⚡Circuit/🎮STM32教程/五、扩展模块/RGBLED呼吸灯的PWM+中断操控(有DMA讲解)|呼吸灯DMA讲解]], 我们如果使用
![[attachments/Pasted image 20240415213407.png]]
以及 p282 页的对应关系表, 可知, <mark style="background: transparent; color: red">DAC_Channel1 仅仅会触发 DMA2_Channel3</mark> 进行转运, 但是我们是使用 TIM7 进行转运的, 对应的 TIM7_UP 在  DMA2_Channel_4中, 因此我们只需初始化 DMA2_Channel4;
![[attachments/Pasted image 20240623113544.png]]
为了让 TIM7 能够触发中断, 需要加上:
```cpp 
TIM_DMACmd(TIM7, TIM_DMA_Update , ENABLE);
```


> [!caution] 在TIM中进行的 Trigger Output 设置
> 由于 DAC 是以TIM7作为Trigger的, 即需要 `DAC_InitStruct.DAC_Trigger = DAC_Trigger_T7_TRGO;`
> 对应地，在TIM设置部分，也必须要开启 TRGO, 方法是 
> `TIM_SelectOutputTrigger(TIM7, TIM_TRGOSource_Update);  // Use TRGO Mode and enable the DAC Trigger`


### (4) 软件编程
使用PA4输出正弦波的代码如下: 
下面的程序采用 TIM7 作为触发将数组中的部分转运到 DAC 寄存器处使引脚上输出正弦波。
```cpp 
#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"

#define  DAC_Channel1Address 0x40007408  // DAC_DHR12R1 register

#define  DAC_ChannelNumber 1
#define  DAC1_TIMER        TIM7

// used for storge the sin wave.
const uint16_t Sine12bit[32] = {
                      2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056,
                      3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909, 
                      599, 344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647
                      };

void MyDAC_rccInit(){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
}

// use TIM7 and DAC for output the analog wave  
void MyDAC_dacInit(){
    DAC_InitTypeDef DAC_InitStruct;
    DAC_InitStruct.DAC_Trigger = DAC_Trigger_T7_TRGO;  // Use Timer 7 for trigger DAC .
    DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;
    DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
    DAC_InitStruct.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bits11_0;
    DAC_Init(DAC_Channel_1, &DAC_InitStruct);
    DAC_Cmd(DAC_Channel_1, ENABLE);
}

void MyDAC_ioInit(){
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN; // analog mode
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_4;    // DAC_OUT1
    GPIO_InitStruct.GPIO_Speed= GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void MyDAC_timInit(){
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 10000 - 1;
    TIM_TimeBaseInitStruct.TIM_Period    = 1350 - 1;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;   // only valid for TIM1 and TIM8
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStruct);

    TIM_SelectOutputTrigger(TIM7, TIM_TRGOSource_Update);  // Use TRGO Mode and enable the DAC Trigger 

    TIM_DMACmd(TIM7, TIM_DMA_Update , ENABLE);
    TIM_Cmd(TIM7, ENABLE);
}

void MyDAC_dmaInit(){
    DMA_InitTypeDef DMA_InitStruct;
    DMA_StructInit(&DMA_InitStruct);
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	
    DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)(&Sine12bit);
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  // uint16_t, half word
    
    DMA_InitStruct.DMA_PeripheralBaseAddr = DAC_Channel1Address;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_BufferSize = 32;   // caution : this gives the whole data size 

    DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;  // circular transfer mode. 
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;

    DMA_Init(DMA2_Channel4, &DMA_InitStruct);
    DMA_Cmd(DMA2_Channel4, ENABLE);  // in the source, increase the memory 
}

// output the traingle wave at gpio pin4, also use the DMA for transfer data. 
int main(){
    OLED_Init();
    MyDAC_rccInit();
    MyDAC_ioInit();
    MyDAC_dacInit();
    MyDAC_dmaInit();
    MyDAC_timInit();
    while (1)
    {
    }
}

```

需要注意的是, 当使用Normal 作为传输模式时, 则需要
- 在 Normal 模式下，DMA 传输完成后，计数器寄存器会保持为 0，不会自动重载。因此，如果你希望启动第二次 DMA 传输，需要执行以下步骤：
    1. 关闭 DMA 通道。
    2. 重新设置 DMA 传输数据个数（CNDTR 寄存器）。
    3. 再次开启 DMA 通道。


需要注意的是, **如果需要驱动喇叭等更加大功率的设备,则我们必须采用 DAC_OutputBuffer_Enable 模式, 但是这样相应的，连续性会变差。如果使用DAC_OutputBuffer_Disable, 则连续性更好但是产生的电压会更小**。

具体可从数据手册中p111看到DAC的特性表, 如下图: 
![[attachments/Pasted image 20240623145817.png]] 
其中最大当 buffer ON时输出约为3.1V ，而buffer off 时, 输出为 V_REF - 1LSB, 其中LSB 是12位DAC的最小电压分辨率值。


### (5) 以DAC为触发的DMA转运   

DMA 转运有一个DAC触发源
![[attachments/Pasted image 20240623113544.png]] 
<mark style="background: transparent; color: red">Channel1的触发源仅限于DMA2_Channel3, 而Channel2的触发源仅限于DMA2_Channel4</mark>, 注意和定时器不同。

使用 DAC 的触发代码如下: 注意仅限于 DMA2_Channel3 才能用 
```c
#include "stm32f10x.h"
// #include "OLED.h"
// #include "Delay.h"

#define  DAC_Channel1Address 0x40007408  // DAC_DHR12R1 register

// #define  DAC_ChannelNumber 1
// #define  DAC1_TIMER        TIM7

const uint16_t Sine12bit[32] = {
                      2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056,
                      3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909, 
                      599, 344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647
                      };

void MyDAC_rccInit(){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
}

// use TIM7 and DAC for output the analog wave  
void MyDAC_dacInit(){
    DAC_InitTypeDef DAC_InitStruct;
    DAC_InitStruct.DAC_Trigger = DAC_Trigger_T7_TRGO;  // Use Timer 7 for trigger DAC .
    DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;
    DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
    DAC_InitStruct.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bits11_0;
    DAC_Init(DAC_Channel_1, &DAC_InitStruct);
}

void MyDAC_ioInit(){
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN; // analog mode
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_4;    // DAC_OUT1
    GPIO_InitStruct.GPIO_Speed= GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void MyDAC_timInit(){
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 10000 - 1;
    TIM_TimeBaseInitStruct.TIM_Period    = 1350 - 1;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;   // only valid for TIM1 and TIM8
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStruct);
    
    TIM_SelectOutputTrigger(TIM7, TIM_TRGOSource_Update);  // Use TRGO Mode and enable the DAC Trigger 
    // TIM_DMACmd(TIM7, TIM_DMA_Update , ENABLE);
}

void MyDAC_dmaInit(){
    DMA_DeInit(DMA2_Channel3);  // only DMA2_Channel3 is available !!! 
    DMA_InitTypeDef DMA_InitStruct;
    DMA_StructInit(&DMA_InitStruct);
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;

    DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)(&Sine12bit);
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  // uint16_t, half word
    
    DMA_InitStruct.DMA_PeripheralBaseAddr = DAC_Channel1Address;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_BufferSize = 32;   // caution : this gives the whole data size 

    DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;  // circular transfer mode. 
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;
    DMA_Init(DMA2_Channel3, &DMA_InitStruct);
}

int main(void)
{
    MyDAC_rccInit();
    MyDAC_ioInit();
    MyDAC_dacInit();
    MyDAC_dmaInit();
    MyDAC_timInit();
    TIM_Cmd(TIM7, ENABLE);
    DMA_Cmd(DMA2_Channel3, ENABLE);  // in the source, increase the memory
    DAC_Cmd(DAC_Channel_1, ENABLE);
    DAC_DMACmd(DAC_Channel_1,ENABLE);

  while (1)
  {
  }
}

```

> [!caution] 总结
> 换DMA触发源时, 一定要换通道!!!!!! 

