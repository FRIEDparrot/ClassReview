针对定时器和DMA的联合使用以及DMA外设，参考[[💻softwares/⚡Circuit/🎮STM32教程/五、扩展模块/RGBLED呼吸灯的PWM+中断操控(有DMA讲解)|RGBLED呼吸灯的PWM+中断操控(有DMA讲解)]] 
# ADC数模转换概念
### (1) 相关外设
首先介绍电位器:12 一端连接VCC , 一段连接GND即可, 没有方向， 相当于滑动变阻器
![[Excalidraw/四、ADC模数转换简介 2024-02-12 16.05.57]]

STM32有12位的ADC, 结果最大为4095, 可以读取

对于反射式红外传感器, 也可以由ADC获取对应的颜色变化等等。 
ADC可以将引脚上连续变化的模拟电压转换成内存中存储的数字变量, 来实现从模拟电路到数字的转换。(DAC也可以使用PWM进行考虑实现)

音频解码和信号发生器一般都需要使用DAC且不好使用PWM代替

对于STM32F103C8T6, 没有DAC的外设。

对于阻值不确定的电阻(例如测量传感器的阻值情况)， 可以**串联一个电阻**
![[attachments/Pasted image 20240212193642.png]]
### (2) ADC 型号简介 
STM32的ADC是12位的逐次逼近的ADC, 主要参数参考[[💻softwares/⚡Circuit/💻51单片机教程/5. 红外遥控,DS1302时钟, AD转换/ADC模数转换与信号处理|ADC模数转换与信号处理]] 
转换时间1us
分辨率 $2^{12}$
转换误差: 
输入电压0-3.3V, 转换0-4095

共有18个输入通道, 包括测量2个内部信号源和16个外部信号源 (内部参考电压是1.2V左右的基准电压), 则当供电不为 3.3V时, 使用此校准。 

有规则组和注入组(突发事件)两个转换单元 >  可以在一个组中进行一次性转换多个值

有模拟看门狗自动监测输入电压范围, 当电压超过阈值时进行申请中断来执行命令。 

其中ADC资源包括ADC1、ADC2，10个外部输入通道


### (3) ADC基本结构 
STM32单片机需要使用外设进行ADC转换，常常使用ADC0809芯片(仅有8个输入通道)(往往会内部集成ADC外设)
![[attachments/Pasted image 20240212164955.png|500]]
上图中 IN0- IN7是通道选择开关, ADDA是选择某个通道的控制，其中我们可以**仅使用一个AD转换器加上多路开关即可实现对上述多行的AD转换**。
选中某一路通道时, 只需要将通道号放在ADDA-ADDC三个引脚上， 然后从ALE输入一个锁存信号，即可实现对应通路的AD转换。
STM32有18路开关。

首先， 在ADC0809中是带有一个DAC的, 通过一个**外部输入的未知编码的输入模拟量电压**和**内部DAC输出的已知编码的电压**。经过**逐次比较使得DAC输出和编码输入基本相等**即可。(DAC由VREF+和VREF-供电),  转换完成之后, 从D0-D7输出数字量

其中START开启转换, EOC是End of  coonversion `->` 即转换完成的输出。 
# 二、STM32 的 ADC 内部原理 
### (1) STM32 的ADC结构
对于下图的STM32 , 使用MUX 输入多路的模拟信号(其中一次可以输入多条路),  **根据图中, Regular Channels可以一次性选择16通道(转换完毕一次性输出), 而Injected Channels可以一次选择4个通道**。

对于规则组转换， 如果配合Regular data register, 一次只能存一个AD转换结果(如果不想之前结果被覆盖, **则使用时最好配合DMA转运数据实现**)；但是Injected data register 可以一次寄存4个结果。 
![[Excalidraw/四、ADC模数转换简介 2024-02-12 17.09.14|800]]
ADC的触发包括软件触发和硬件触发， 其中硬件触发可以采用转换触发部分作为触发源。(分开有规则和注入组的触发源)

可以使用功能 TRGO 定时器主模式输出用于触发转换操作`->` 可以利用定时器来间隔申请中断来间隔实现ADC转换。 如果使用定时器1ms申请一次中断， 则**频繁中断会直接影响程序的进行** 

> [!caution] 频繁进行中断但是中断中仅完成简单工作的情况处理
> 往往可以使用TRGO进行输出， 并使用功能具有相应支持的硬件， 
> 方法例如将TIM3**定时为1ms并选择TRGO输出**; 
> 最后在ADC系统**中选择开始触发信号为TIM3的TRGO即可完成TIM3进行定时中断**。

另外也可以选择外部中断引脚触发转换。 
$VDDA, VSSA$ 和$V_{REF} +$与$V_{REF}-$ 其中VDDA和VSSA为ADC的供电引脚， 而上面的是参考电压(决定输出范围)。 
同时, VDDA, VSSA也是内部模拟部分的电源; 例如ADC, 锁相环和RC振荡器等等。 

另外**DMA请求部分对应触发DMA和进行数据转运**, 在**多路复用AD转换时往往使用**。 

**ADCCCLK**对应原理图中的CLOCK, **ADC预分频器来源于RCC部分**, 参考**时钟树右侧的ADC预分频器部分**, 可以选2,4,6,8分频, <b><mark style="background: transparent; color: blue">由于标准时钟是72MHz的, 而ADDCLK最大14Mhz, 因此只能选择6分频(12M)和8分频(9MHz)</mark></b> 
![[Excalidraw/二、STM32定时器中断的使用 2024-02-04 17.31.39|1000]]
另外， 在**定时器的Flags标志位**(即状态寄存器标志位)中,  **只需读取标志位即可获取转换是否结束**。<mark style="background: transparent; color: red">EOC 表示转换结束， JEOC为注入转换结束标志位, AWD为模拟看门狗事件标志位</mark>。 
interrupt enable bits 用于进行使能中断。  

### (2) ADC外部通道对应的GPIO口 
ADC以及对应的转换通道如下（没有ADC3）: 

| 通道 | ADC1 | ADC2 | ADC3 |
| ---- | ---- | ---- | ---- |
| 通道0 | PA0 | PA0 | PA0 |
| 通道1 | PA1 | PA1 | PA1 |
| 通道2 | PA2 | PA2 | PA2 |
| 通道3 | PA3 | PA3 | PA3 |
| 通道4 | PA4 | PA4 | PF6 |
| 通道5 | PA5 | PA5 | PF7 |
| 通道6 | PA6 | PA6 | PF8 |
| 通道7 | PA7 | PA7 | PF9 |
| 通道8 | PB0 | PB0 | PF10 |
| 通道9 | PB1 | PB1 |  |
| 通道10 | PC0 | PC0 | PC0 |
| 通道11 | PC1 | PC1 | PC1 |
| 通道12 | PC2 | PC2 | PC2 |
| 通道13 | PC3 | PC3 | PC3 |
| 通道14 | PC4 | PC4 |  |
| 通道15 | PC5 | PC5 |  |
| 通道16 | 温度传感器 |  |  |
| 通道17 | 内部参考电压 |  |  |
注意: 对STM32F103C8T6, 芯片只有10个外部输入通道, 即PA0-PA7,  PB0-PB1
![[attachments/Pasted image 20240212185223.png]]
两个ADC的端口是完全相同的。 而**使用两个ADC的原因是使用双ADC模式的ADC高级功能**
即ADC1和ADC2一起工作, 组成同步模式和交叉模式等等; **例如交叉模式下, ADC1和ADC2交叉对一个通道进行交叉采样, 可以进一步提高采样率**; ADC1和ADC2的引脚是完全相同的, 而ADC3中会有变化。
### (3) 规则组的4种AD转换模式 
4种转换模式包括**单次转换连续转换**； **非扫描和扫描模式**；
都可以在ADC的初始化结构体中进行选择进行。 

非扫描模式下. **仅有序列1有效**, 但是**可以在这个序列下选择任意的转换通道**(10个)。 

对于连续转换非扫描模式，当一次转换之后， 会立刻开始下一轮的转换; 并进行持续。 **因此只需要触发一次， 就可以进行一直转换**。 不用开始和判断结束, 且变更通道时； 

对于**单次转换扫描模式**下: 
1.  **每触发一次转换就会停下, 下一次只有再次触发才能开始**。
2.  需要在初始化结构体中设置**转换的通道数目**(指定前x个位置) 
3.  为了防止数据被覆盖， 需要使用DMA即时挪走数据
4. **全部的通道转换完成之后， 才产生EOC信号**， 转换结束。

连续转换扫表一次转换完成之后立刻完成下一次转换。 

在扫描模式下， 还有间断模式。  在转换过程中每几个转换完成就暂停， 需要触发才能继续。 

### (4) 触发控制和数据对齐
![[attachments/Pasted image 20240212192310.png]]
数据右对齐(从右开始)和左对齐方式见下表: 
![[attachments/Pasted image 20240212192441.png]]
**一般使用右对齐方式进行读取**; 
而左对齐读的值比实际大16倍`->` 如果不需要太高分辨率然后舍弃后4位

•STM32 ADC的总转换时间为: 
  TCONV = **采样时间** + 12.5个ADC周期;
例如: 当ADCCLK=14MHz，采样时间为1.5个ADC周期时
  TCONV = 1.5 + 12.5 = 14个ADC周期 = 1μs
其中采样时间可以在程序中配置, 较大可以**避免毛刺信号干扰**。 

### (5) ADC自校准模式的使用 
对于ADC的自校准模式, 校准可大幅减小因内部电容器组的变化而造成的准精度误差。校准期间，在每个电容器上都会计算出一个误差修正码(数字值)，这个码用于消除在随后的转换中每个电容器上产生的误差。

• 比较好的方法是**在每次上电后执行一次校准**

另外, 启动校准前， ADC必须处于关电状态超过至少两个ADC时钟周期

# 三、软件编程
### (1) 相关函数
我们整理过程: 
![[attachments/Pasted image 20240212184317.png|800]]
1. 首先使能相应的时钟， 配置预分频器为6分频; 配置GPIO (AIN模式)
2. 配置TIM3使用TRGO输出信号控制ADC转换的启动和停止 
3. 配置标志位， 使能位(ITConfig) , NVIC (配置优先级) 和中断
4. 启动ADC (ADC_cmd) 可以在此时进行校准 
5. 启动TIM3, 打开触发控制 

相关库函数如下: 
```cpp title:RCC时钟控制在RCC.h中
void RCC_ITConfig(uint8_t RCC_IT, FunctionalState NewState); // 中断使能位
void RCC_ADCCLKConfig(uint32_t RCC_PCLK2); // 用于配置ADCCLK分频器
```
可以选择6分频: 
```cpp
void ADC_DeInit(ADC_TypeDef* ADCx);
void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct);
void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct);
void ADC_Cmd(ADC_TypeDef* ADCx, FunctionalState NewState); // enable ADC 
void ADC_DMACmd(ADC_TypeDef* ADCx, FunctionalState NewState);  // 如果使用DMA转运数据， 则需要对这个进行初始化。 
void ADC_ITConfig(ADC_TypeDef* ADCx, uint16_t ADC_IT, FunctionalState NewState); // 中断输出控制

// 控制校准的函数
void ADC_ResetCalibration(ADC_TypeDef* ADCx);   // 复位校准
FlagStatus ADC_GetResetCalibrationStatus(ADC_TypeDef* ADCx);  // 获取复位校准状态
void ADC_StartCalibration(ADC_TypeDef* ADCx);    // 开始校准
FlagStatus ADC_GetCalibrationStatus(ADC_TypeDef* ADCx);    // 获取开始校准状态

void ADC_SoftwareStartConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);  // 用于软件触发开始控制 
FlagStatus ADC_GetSoftwareStartConvStatus(ADC_TypeDef* ADCx); // 获取软件开始转换状态 -> 注意不能判断是否转换结束 -> 源码是获取CR2的SWSTART位, -> 转换开始之后即被清除。(一般不使用, 用GetFlagStatus)

// 间断模式 -> 第一个配置隔 Number 个进行自动间断, 第二个配置是否启用 
void ADC_DiscModeChannelCountConfig(ADC_TypeDef* ADCx, uint8_t Number);
void ADC_DiscModeCmd(ADC_TypeDef* ADCx, FunctionalState NewState);

// ************* 规则组通道配置************
void ADC_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime);  // 用于对序列的位置写入指定的通道, 一次写入一个, Rank 表示写入通道在序列中的位置, 同时配置采样时间。 
void ADC_ExternalTrigConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);  // ADC 外部触发转换控制
uint16_t ADC_GetConversionValue(ADC_TypeDef* ADCx);  // 获取AD转换数据寄存器 
```

```cpp title:注入组配置函数
void ADC_AutoInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_InjectedDiscModeCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_ExternalTrigInjectedConvConfig(ADC_TypeDef* ADCx, uint32_t ADC_ExternalTrigInjecConv);
void ADC_ExternalTrigInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_SoftwareStartInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
FlagStatus ADC_GetSoftwareStartInjectedConvCmdStatus(ADC_TypeDef* ADCx);
void ADC_InjectedChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime);
void ADC_InjectedSequencerLengthConfig(ADC_TypeDef* ADCx, uint8_t Length);
void ADC_SetInjectedOffset(ADC_TypeDef* ADCx, uint8_t ADC_InjectedChannel, uint16_t Offset);
uint16_t ADC_GetInjectedConversionValue(ADC_TypeDef* ADCx, uint8_t ADC_InjectedChannel);
```

```cpp title:模拟看门狗配置函数
void ADC_AnalogWatchdogCmd(ADC_TypeDef* ADCx, uint32_t ADC_AnalogWatchdog); // 是否启动
void ADC_AnalogWatchdogThresholdsConfig(ADC_TypeDef* ADCx, uint16_t HighThreshold, uint16_t LowThreshold);   // 配置高低阈值 
void ADC_AnalogWatchdogSingleChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel); // 配置看门通道
```

```cpp title:ADC温度传感器,内部参考电压控制
void ADC_TempSensorVrefintCmd(FunctionalState NewState);  // 如果使用ADC温度传感和内部参考电压, 可以使用这个进行开启。
```

```cpp title:中断函数
FlagStatus ADC_GetFlagStatus(ADC_TypeDef* ADCx, uint8_t ADC_FLAG);  // 获取中断标志位, 判断是否转换完成
void ADC_ClearFlag(ADC_TypeDef* ADCx, uint8_t ADC_FLAG); // 清除中断标志位
ITStatus ADC_GetITStatus(ADC_TypeDef* ADCx, uint16_t ADC_IT);   // 获取中断状态
void ADC_ClearITPendingBit(ADC_TypeDef* ADCx, uint16_t ADC_IT);  // 清除中断挂起位
```

### (2) 基本步骤
在RegularChannelConfig中有相应的ADC通道设置 其中有1-17共有17个ADC口配置
```cpp title:
void ADC_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime); 
```

其中在ADCInitSreuct中, 有如下的参数部分
```cpp 
    ADC_InitTypeDef* ADC1_InitStruct = new ADC_InitTypeDef();
    ADC1_InitStruct -> ADC_Mode = ADC_Mode_Independent; // two ADC work independently
    ADC1_InitStruct -> ADC_DataAlign = ADC_DataAlign_Right;  // 设置数据对其
    ADC1_InitStruct -> ADC_ContinuousConvMode = FunctionalState::DISABLE;     // 不使用连续模式
    ADC1_InitStruct -> ADC_ScanConvMode = FunctionalState::DISABLE;  // 扫描模式选择
    ADC1_InitStruct -> ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;  // 选择触发控制的触发源 -> 即定义启动ADC转换的外部触发源 -> 我们使用定时器时, 一般都是用TRGO进行触发。 另外也可以使用None (不使用外部触发，使用内部软件触发) -> 如果要求低可以软件
    ADC1_InitStruct -> ADC_NbrOfChannel = 1; // only use 1 channel 
```

其中Mode选项包括: 1是独立模式， 其他的都是双ADC模式
```cpp title:ADCMode的取值
#define ADC_Mode_Independent                       ((uint32_t)0x00000000)
#define ADC_Mode_RegInjecSimult                    ((uint32_t)0x00010000)
#define ADC_Mode_RegSimult_AlterTrig               ((uint32_t)0x00020000)
#define ADC_Mode_InjecSimult_FastInterl            ((uint32_t)0x00030000)
#define ADC_Mode_InjecSimult_SlowInterl            ((uint32_t)0x00040000)
#define ADC_Mode_InjecSimult                       ((uint32_t)0x00050000)
#define ADC_Mode_RegSimult                         ((uint32_t)0x00060000)
#define ADC_Mode_FastInterl                        ((uint32_t)0x00070000)
#define ADC_Mode_SlowInterl                        ((uint32_t)0x00080000)
#define ADC_Mode_AlterTrig                         ((uint32_t)0x00090000)
```

在初始化之后, 需要进行校准， 而校准的部分可以使用如下代码完成: 
其中对于GetResetCalibrationStatus, 根据源码, 获取的是 ADCx->CR2 的值并和`CR2_RSTCAL_Set` 比较, 参考数据手册, 实际上参考下图, 则 <mark style="background: transparent; color: red">当数据为0时, 校准寄存器已经完成校准</mark> , 因此复位过程中为SET。
![[attachments/Pasted image 20240213100433.png|700]] 
而GetCalibrationStatus是 CR2_CAL_Set 的值, 0 表示Calibration Completed. 

```cpp
 ADC_ResetCalibration(ADC1);  // reset calibration configuration
while(ADC_GetResetCalibrationStatus(ADC1) == FlagStatus::SET); // wait for calibration
ADC_StartCalibration(ADC1);
```

在软件进行转换时, 需要使软件触发转换之后, 等待EOC置一之后读取数据寄存器即可。 读取的函数是GetStatus (实际操作SR寄存器), 另外根据手册, cleared by software or reading the ADC_DR 即**获取时会自动清除**, 不需要手动清除。

其中参考手册, EOC = 0是not complete 因此使用while (EOC == RESET)即可
```cpp title:状态寄存器标志位
/*    
  *	@arg ADC_FLAG_AWD: Analog watchdog flag       // 模拟看门狗标志位
  *     @arg ADC_FLAG_EOC: End of conversion flag      // 规则转换组完成标志位 (规则组或注入组都会置一) 
  *     @arg ADC_FLAG_JEOC: End of injected group conversion flag   // 注入组转换完成标志位
  *     @arg ADC_FLAG_JSTRT: Start of injected group conversion flag  // 注入组开始转换标志位
  *     @arg ADC_FLAG_STRT: Start of regular group conversion flag     // 规则组开始转换标志位 
*/
```
转换时间为13.5 + 12.5 = 26周期, 即1/12M * 26周期(2us)
等待完成之使用ADC_GetConversionValue即可。 

### (3) 完整代码 

```cpp
#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"

using namespace std;

void ADC1_Init(){
        // use ADC12_IN0 for AD convert 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, FunctionalState::ENABLE); // ADC are all device on APB2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, FunctionalState::ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);   // configure as 6 divide frequency 

    GPIO_InitTypeDef* GPIO_InitStruct = new GPIO_InitTypeDef();
    GPIO_InitStruct->GPIO_Mode = GPIO_Mode_AIN; 
    GPIO_InitStruct->GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct->GPIO_Speed = GPIOSpeed_TypeDef::GPIO_Speed_50MHz;
    
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_13Cycles5); // sample time is 1.5 ADCCLK cycle

    ADC_InitTypeDef* ADC1_InitStruct = new ADC_InitTypeDef();
    ADC1_InitStruct -> ADC_Mode = ADC_Mode_Independent; // two ADC work independently
    ADC1_InitStruct -> ADC_DataAlign = ADC_DataAlign_Right;
    ADC1_InitStruct -> ADC_ContinuousConvMode = FunctionalState::DISABLE;
    ADC1_InitStruct -> ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  // use TRGO for convert
    ADC1_InitStruct -> ADC_ScanConvMode = FunctionalState::DISABLE;
    ADC1_InitStruct -> ADC_NbrOfChannel = 1; // only use 1 channel 
    ADC_Init(ADC1, ADC1_InitStruct);
}

void ADC1_Start(){
    ADC_Cmd(ADC1, FunctionalState::ENABLE);  // enable ADC1 
    // use calibration for ADC1
    ADC_ResetCalibration(ADC1);  // reset calibration configuration 
    while(ADC_GetResetCalibrationStatus(ADC1) == FlagStatus::SET); // wait for calibration 
    ADC_StartCalibration(ADC1); 
    while(ADC_GetCalibrationStatus(ADC1) == FlagStatus::SET);
}

uint16_t AD_Get_Convert_Value(void){
    uint16_t res;
    ADC_SoftwareStartConvCmd(ADC1, FunctionalState::ENABLE);
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == FlagStatus::RESET);
    res = ADC_GetConversionValue(ADC1);

    return res;
}

int main(){
    uint16_t v = AD_Get_Convert_Value();
	OLED_ShowNum(1,1, v, 5);
	float Voltage = (float)v/4095 * 3.3;
	OLED_ShowString(2,1, "Voltage:0.00V");
	OLED_ShowNum(2,9, Voltage, 1);
	OLED_ShowNum(2,11,(uint16_t)(Voltage * 100)% 100, 2);
	Delay_ms(100);
}
```

需要说明的是, 由于AD值在判断域附近来回跳变, 导致输出产生抖动, 为了保证判断条件等加在ADC中不会产生抖动(例如判断阈值并产生某一报警); 这个可以**采用迟滞比较的方法完成**(例如滞回比较器等等) `->` 参考[[💻softwares/⚡Circuit/🎮STM32教程/一、基础使用部分/3. 通用IO口输出以及简单实验|滞回比较器]]部分。
方法1是大于上阈值和小于下阈值工作方法;
方法2是进行均值滤波, 采用平均值作为滤波AD值。 
裁剪分辨率即可。
### (4) 连续转换的修改使用
对于连续转换, 只需**先将连续转换修改为ENABLE**, 然后最开始触发一次就可以了。
首先在主循环前面调用ADC_SoftwareStartConvCmd(ADC1, FunctionalState::ENABLE);
然后也不需要Start和等待完成了， 只需return 即可
```cpp 
#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"

using namespace std;

void ADC1_Init(){
        // use ADC12_IN0 for AD convert 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, FunctionalState::ENABLE); // ADC are all device on APB2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, FunctionalState::ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);   // configure as 6 divide frequency 

    GPIO_InitTypeDef* GPIO_InitStruct = new GPIO_InitTypeDef();
    GPIO_InitStruct->GPIO_Mode = GPIO_Mode_AIN; 
    GPIO_InitStruct->GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct->GPIO_Speed = GPIOSpeed_TypeDef::GPIO_Speed_50MHz;
    
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_13Cycles5); // sample time is 1.5 ADCCLK cycle 
    ADC_InitTypeDef* ADC1_InitStruct = new ADC_InitTypeDef();
    ADC1_InitStruct -> ADC_Mode = ADC_Mode_Independent;   // two ADC work independently
    ADC1_InitStruct -> ADC_DataAlign = ADC_DataAlign_Right;  // 
    ADC1_InitStruct -> ADC_ContinuousConvMode = FunctionalState::ENABLE;
    ADC1_InitStruct -> ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  // use TRGO for convert 
    ADC1_InitStruct -> ADC_ScanConvMode = FunctionalState::DISABLE; 
    ADC1_InitStruct -> ADC_NbrOfChannel = 1; // only use 1 channel 
    ADC_Init(ADC1, ADC1_InitStruct); 
}

void ADC1_Start(){
    ADC_Cmd(ADC1, FunctionalState::ENABLE);  // enable ADC1 
    // use calibration for ADC1
    ADC_ResetCalibration(ADC1);  // reset calibration configuration 
    while(ADC_GetResetCalibrationStatus(ADC1) == FlagStatus::SET); // wait for calibration 
    ADC_StartCalibration(ADC1); 
    while(ADC_GetCalibrationStatus(ADC1) == FlagStatus::SET);
}

uint16_t AD_Get_Convert_Value(void){
    return ADC_GetConversionValue(ADC1);
}

int main(){
    OLED_Init();
    ADC1_Init();
    ADC1_Start();
    ADC_SoftwareStartConvCmd(ADC1, FunctionalState::ENABLE); 
    while(true){
        uint16_t v = AD_Get_Convert_Value();
        OLED_ShowNum(1,1, v, 5);
        float Voltage = (float)v/4095 * 3.3;
        OLED_ShowString(2,1, "Voltage:0.00V");
        OLED_ShowNum(2,9, Voltage, 1);
        OLED_ShowNum(2,11,(uint16_t)(Voltage * 100)% 100, 2);
        Delay_ms(100);
    }
}
```

### (5) AD多通道采集的修改使用
例如采用PA0-PA2的通道问题, 往往可以使用DMA进行转换。 
扫描模式手动转运困难。 使用<mark style="background: transparent; color: red">单次转换非扫描实现多通道方案</mark>。 

在GetValue中调用ChannelConfig方法, 即可实现每一次调用一个通道 
注意, 其中Continuous Mode一定要谨慎使用(此处应当使用Disable), **Continuous Mode 必须配合DMA进行, 否则会导致数据覆盖。** 
```cpp fold title:修改方法非常简单,完整代码如下:
#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"

using namespace std;

void ADC1_Init(){
        // use ADC12_IN0 for AD convert 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, FunctionalState::ENABLE); // ADC are all device on APB2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, FunctionalState::ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);   // configure as 6 divide frequency 

    GPIO_InitTypeDef* GPIO_InitStruct = new GPIO_InitTypeDef();
    GPIO_InitStruct->GPIO_Mode = GPIO_Mode_AIN; 
    GPIO_InitStruct->GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2 ; // initialize 3 ports 
    GPIO_InitStruct->GPIO_Speed = GPIOSpeed_TypeDef::GPIO_Speed_50MHz;
    
    ADC_InitTypeDef* ADC1_InitStruct = new ADC_InitTypeDef();
    ADC1_InitStruct -> ADC_Mode = ADC_Mode_Independent; // two ADC work independently
    ADC1_InitStruct -> ADC_DataAlign = ADC_DataAlign_Right;
    ADC1_InitStruct -> ADC_ContinuousConvMode = FunctionalState::DISABLE; // *** caution ***
    ADC1_InitStruct -> ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  // use TRGO for convert
    ADC1_InitStruct -> ADC_ScanConvMode = FunctionalState::DISABLE;
    ADC1_InitStruct -> ADC_NbrOfChannel = 1; // only use 1 channel -> once a time 
    ADC_Init(ADC1, ADC1_InitStruct);
}

void ADC1_Start(){
    ADC_Cmd(ADC1, FunctionalState::ENABLE);  // enable ADC1 
    // use calibration for ADC1
    ADC_ResetCalibration(ADC1);  // reset calibration configuration 
    while(ADC_GetResetCalibrationStatus(ADC1) == FlagStatus::SET); // wait for calibration 
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1) == FlagStatus::SET);
}

uint16_t AD_Get_Convert_Value(uint8_t Channel){
    ADC_RegularChannelConfig(ADC1, Channel, 1, ADC_SampleTime_13Cycles5); // sample time is 1.5 ADCCLK cycle
    // firstly, change the port number
    ADC_SoftwareStartConvCmd(ADC1, FunctionalState::ENABLE);
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == FlagStatus::RESET); // wait 
    uint16_t res = ADC_GetConversionValue(ADC1);
    return res;
}

int main(){
    OLED_Init();
    ADC1_Init();
    ADC1_Start();
    while(true){
        OLED_ShowNum(1,1, AD_Get_Convert_Value(ADC_Channel_0), 5);
        OLED_ShowNum(2,1, AD_Get_Convert_Value(ADC_Channel_1), 5);
        OLED_ShowNum(3,1, AD_Get_Convert_Value(ADC_Channel_2), 5);
        Delay_ms(100);
    }
}
```

# 四、DMA 直接存储器读取
### (1) DMA的基本概念与配置
DMA(Direct Memory Access) 为直接存储器读取。如果使用DMA进行数据转运， AD转换的数据就会自动转运到我们需要的数组中， 只要调用OLED显示就可以了。 

DMA<b><mark style="background: transparent; color: blue">往往使用于刷新高分辨率的屏幕， 高带宽的外设通信</mark></b>。 
**DMA 可以提供外设和存储器或者存储器和存储器之间的高速数据传输，无须CPU干预，节省了CPU的资源**
对于存储器和存储器, 一般指运行内存SRAM和程序存储器Flash, 用于存储变量数组和程序代码。 
在转运过程中无需DMA参与。

STM32系列的DMA具有<b><mark style="background: transparent; color: blue">12个可以独立配置的DMA通道</mark></b>, DMA1有7个通道, 而DMA2有5个通道, 多通道之间可以**快速转运而互不干扰**。而F103c8t6系列的DMA**仅有DMA1通道**。(高密度有DMA2通道) 
每一个DMA通道, 都可以支持**软件触发**以及**特定的硬件触发**， 例如将SRAM中的部分转运到Flash, 则需要进行软件触发。
- 如果是DMA完成**从SRAM转运到Flash, 则转运可以以最快的速度一次性全部转运完成**, 因此<b><mark style="background: transparent; color: red">存储器到存储器的触发，一般使用软件触发</mark></b>;
- 如果是完成外设到存储器的转运, 就不能一次性最快地转发, **而是需要在ADC每个通道AD转运完成之后等等，执行一次硬件触发**。 因此<b><mark style="background: transparent; color: red">外设到存储器， 一般使用硬件触发</mark></b>。
每一个外设的触发源，触发的通道是不同的;  对于**使用某个外设的触发源，必须使用其连接的通道**， 而不能任意选择通道。
DMA的内容主要对应数据手册13章。

### (2) STM32的存储器映像
计算机系统的5大组成部分 : **\[运算器， 控制器](CPU)， 存储器**，输入和输出设备
计算机的关键部分是CPU和存储器, 而存储器的安排地址入下(数据手册p34Memory Mapping): 

![[attachments/Screenshot_20240213_121447_com.microsoft.skydrive.jpg|1000]]
如上图可以获取入下的存储器结构:

| 类型 | 起始地址 | 存储器 | 用途 |
| ---- | ---- | ---- | ---- |
| Flash Memory | 0x0800 0000 | 程序存储器Flash(ROM) | 存储C语言编译后的程序代码 |
| System Memory | 0x1FFF F000 | 系统存储器(ROM) | **存储BootLoader，用于串口下载** |
| Option Bytes | 0x1FFF F800 | 选项字节(ROM) | 存储一些独立于程序代码的配置参数 |
| SRAM | 0x2000 0000 | 运行内存SRAM(RAM) | 存储运行过程中的临时变量 |
| 也是SRAM | 0x4000 0000 | 外设寄存器(SRAM) | 存储各个外设的配置参数 |
|  | 0xE000 0000 | 内核外设寄存器 | 存储内核各个外设的配置参数 |

STM32 特别指定了转运数据的存储器。 

芯片的BootLoader部分(System Memory)是出厂自动写入且一般不允许修改。  
选项的字节中主要存放 Flash 的读保护， 写保护与看门狗等等的配置。
SRAM 区存储变量, 数组, 结构体等等部分。(变量的内存都是以2000开头的)
内核外设寄存器即 Cortex-M3 Internal Peripherals 部分, 用于存储内核寄存器的外设参数(<mark style="background: transparent; color: red">内核外设即为NVIC和SysTick</mark>)
CPU的寻址寄存器范围是32位的范围。 (最大支持4GB容量的存储器, **这也是32位支持最大4GB运行内存ROM的原因**) 
0x0000 地址部分是别名到Flash或者系统寄存器(根据BOOT引脚的值, 即BOOT0和BOOT1的值), 可以在Flash 区执行， 也可以从系统存储器运行BootLoader, 

其中外设寄存器区 0x4000 部分的展开部分已经在右边说明。 

### (3) DMA 基本结构
![[Excalidraw/四、ADC与DMA存储器转运 2024-02-13 14.18.49|700]]
寄存器是特殊存储器, 本质上也是SRAM , 除了CPU可以对内存进行读写以外, 每一位背后都有导线可以用于控制外设电路的状态; 
寄存器可以**控制引脚电平， 导通开关和切换数据选择器**等等, 也可以**当做寄存器和计数器**等等。

利用<b><mark style="background: transparent; color: blue">总线矩阵Bus Matrix</mark></b> 来实现寄存器的高效访问。
主动单元中**内核有DCode和系统总线**来访问存储器, 其中Flash由DCode 专门访问;
DMA总线对Flash和SRAM也有访问权。 但是**注意Flash是只读的, 不可写入(不能将转运目的写为Flash)**(除非使用Flash接口寄存器按页擦除再写入)

另外, 以太网也有自己私有的DMA线(Ethernet MAC)。上述DMA和DMA2已经标出, 可以设置转运数据的源地址和目的地址。 
<b><mark style="background: transparent; color: blue">注意, 所有的通道只能分时复用一条DMA总线如果DMA和CPU需要访问同一个地址，则DMA会停止CPU的访问从而防止冲突</mark></b>,但是<b><mark style="background: transparent; color: red">总线仲裁器(放置在BusMatrix中)</mark></b>会保证CPU的一半带宽使其正常工作。

AHB Slave即<b><mark style="background: transparent; color: blue">AHB从设备, 即DMA自身的寄存器</mark></b>, 并与Bus Matrix 连接, <mark style="background: transparent; color: red">除此以外许多外设也可以发送DMA请求</mark>,  用于**作为硬件触发源**。

DMA包括: 
- Arbiter 是仲裁器, 用于调度通道来防止冲突
- AHB 从设备: 配置DMA 参数 
- DMA请求: 用于硬件触发DMA数据转运。 

> [!NOTE] 补充
> 另外对于外设寄存器, 有的是只读的, 有的是只写的。而我们主要使用的数据寄存器都是可以正常读写的; 具体可以参考手册。 

DMA内部结构图如下: 其中数据转运的站点包括外设寄存器和Flash > SRAM;
DMA可以按照黄色箭头方向转运; **且不能完成Flash > Flash 和SRAM > Flash的数据转运**。
![[attachments/Pasted image 20240213151600.png|800]]
#### 1.  外设寄存器和存储器 
在实际编程中， **外设站点和存储器站点各自有3个参数**: 
1. 起始地址
2. 数据宽度 <b><mark style="background: transparent; color: red">(有字节byte(8位, 一次转运一个uint8_t), 半字harfword(一个uint16_t大小), 字word(uint32_t大小)) </mark></b> 例如DMA参数转运时, 一次转运数据为uint16_t, 即半字大小
3. 地址是否自增 (是否转运之后地址自增 > **外设地址是ADC_DR寄存器, 显然不用自增, 但是存储器是需要自增的, 否则就会导致覆盖**) 

也可以在外设站点写入存储器的地址。
#### 2. 传输计数器
**传输计数器**: 用于指定总需要转运的次数， 是一个自减计数器。每转运一次， 传输计数器-1， 当减到0之后, DMA就不会再进行数据转运了。 
**自动重装器**: 当<mark style="background: transparent; color: red">传输计数器减到0之后， 设置是否恢复到初始值</mark>(如果使用则DMA可以多次)，<b><mark style="background: transparent; color: blue">决定重装是单次模式还是循环模式</mark></b>(如果是ADC + DMA循环扫描模式, 必须配合使用循环转运)。

#### 3. DMA触发控制
DMA的触发控制是传输计数器下方的M2M以及01部分, 决定转运的时机。
触发源包括**硬件和软件触发**。并由M2M(Memory to Memory) 进行选择。  

1. M2M = 1时, 选择软件触发(<b><mark style="background: transparent; color: red">应用于存储器到存储器的转运的情况</mark></b>): 注意<b><mark style="background: transparent; color: blue">软件触发的执行逻辑是: 以最快的速度连续不断触发DMA, 争取早日将传输计数器清零来早日完成传输。 </mark></b>(即连续触发，这与ADC一次触发转换一次不同) 
> [!NOTE] 注意
> 需要注意, <b><mark style="background: transparent; color: blue">软件模式和循环触发不能同时使用</mark></b>， 软件模式是需要将传输计数器清零, 如果使用重装则会导致循环。
2. M2M = 0 时，使用硬件触发。 其触发可以使用定时器, ADC, 串口等等。 往往与外设相关。
3. DMACmd控制
在启动时需要使用DMAcmd打开DMA来启动数据传输
但是要<b><mark style="background: transparent; color: blue">进行下一次数据传输时</mark></b>, 必须<b><mark style="background: transparent; color: red">先使用DMAcmd关闭DMA， 然后写传输计数器， 使能DMAcmd, </mark></b>然后给触发信号来传输。 不能在DMA开启时进行。 
> [!summary] DMA转运的条件
> 1. 开关控制， DMA_Cmd 必须使能。
> 2. 传输计数器 > 0
> 3. 触发源, 必须有触发信号
### (4) DMA请求映像
参考数据手册p280 
![[attachments/Pasted image 20240213154933.png|600]]
每一个通道的数据选择器是单独的, 进行每一个通道的触发使能。
每个事件的触发通道也在上图左边写出。**每个通道使用的硬件触发源不同; 例如使用TIM1_CH1触发只能选择通道2**; 而软件触发每个通道都支持。 

左侧选择哪个作为触发源是**根据对应的外设是否开启了DMA输出决定的**。例如对于ADC, 需要**单独地开启**以下关键函数: 
```cpp
ADC_DMAcmd // 用于启动对应ADC 的 DMA输出控制 

TIM_DMAcmd // 启动对应TIM的DMA输出控制
```
另外固件触发有默认优先级， 按照右侧来确定优先级。 

### (5) 数据宽度与对齐
当转运的两个站点(<b><mark style="background: transparent; color: red">外设寄存器和内部数据</mark></b>)**数据宽度一致时， 可以进行单个转运**， 但是PSIZE(Peripherical)和MSIZE(Memory)不同时， 需要参考p278的**可编程数据传输宽度和大小操作**。
![[Excalidraw/四、ADC与DMA存储器转运 2024-02-13 16.12.03|800]]
如上图, 其中第一列是**源端宽度(Source)**, 当**第二列为目标转运的宽度** 
显然, 当源端8位 目标为16位时, 则源端读B0, 目标写00B0, 即在目标多出来的数据部分补0 
当目标端比源端数据宽度小，则读B1B0时仅写入B0, 即丢失高位数据。
### (6) DMA 的工作原理
#### 1. DMA外设地址
先配置外设和存储器的三个参数; (**外设给DataA的首地址, 存储器给DataB的首地址**)
此时, **两个站点的地址都应当自增,** 来移动到下一个数据位置。 

转运次数 = 数组长度 
**软件转运**(相当于复制)
转运完需要使用Cmd给失能位。
#### 2. ADC扫描 + DMA转运
基本转运结构如下: 
![[Excalidraw/四、ADC与DMA存储器转运 2024-02-13 16.24.06|800]]
其中， **7个通道依次进行AD转换,转换结果都放到ADC_DR外设地址里面**，然后 
> [!NOTE] 说明
> 我们所需要做的, 就是在**每一个单独的通道之后，进行一次DMA转运**, 且**初始地址为ADC数据寄存器地址(ADC_DR)， 不需要自增**；而结尾地址为数组地址，进行自增。
> 
> 同时， 为了能够及时转运，需要**使用ADC硬件触发进行触发**。需要**和单个通道转换完成同步**
> 两端均选用uint16_t数据长度
> 
> ADC为连续扫描时, DMA设置为自动重装即可。 

对于ADC在多通道模式下, 虽然单个通道转换完成之后， 不产生任何标志位; 但是**会产生DMA请求**。因此**ADC和DMA结合使用最为常见**。

在p52 Bit-Banding **位段**中, 介绍了**存储器包含2个位段区**, 即**将所有的单个位**都分配了一个地址, **操作此地址相当于操作寄存器中的某一单个位**。 从而可以保证不仅能整体操作也能位操作

闪存编排可以在p53找到。 闪存接口寄存器也叫Flash Memory Interface Register是一个对闪存进行读写的外设。 
系统存储器和选择字节统称为信息块; 

另外在传输过程中， 有**传输过半， 传输完成和传输错误的中断**；

# 五、 DMA软件编程 
### (1) DMA实现数组转运
地址可以使用 uint32_t(&a) 来表示 
如果是const 常量则存储在Flash 中, 往往是不需要更改的; 
外设寄存器的地址都是固定的; 并且都可以从手册查到。 获取某个地址可以查手册, 也可以直接显示。
访问的方法是结构体访问寄存器, 即使用基地址+偏移获取寄存器的实际地址。 而偏移只需要使用指针进行即可。
```cpp
void DMA_DeInit(DMA_Channel_TypeDef* DMAy_Channelx);
void DMA_Init(DMA_Channel_TypeDef* DMAy_Channelx, DMA_InitTypeDef* DMA_InitStruct);
void DMA_StructInit(DMA_InitTypeDef* DMA_InitStruct);
void DMA_Cmd(DMA_Channel_TypeDef* DMAy_Channelx, FunctionalState NewState);
void DMA_ITConfig(DMA_Channel_TypeDef* DMAy_Channelx, uint32_t DMA_IT, FunctionalState NewState);

void DMA_SetCurrDataCounter(DMA_Channel_TypeDef* DMAy_Channelx, uint16_t DataNumber);  // 设置当前数据寄存器, 用于给传输计数器写数据
uint16_t DMA_GetCurrDataCounter(DMA_Channel_TypeDef* DMAy_Channelx);  // 获取传输计数器的值

// 状态函数和中断
FlagStatus DMA_GetFlagStatus(uint32_t DMAy_FLAG);
void DMA_ClearFlag(uint32_t DMAy_FLAG);
ITStatus DMA_GetITStatus(uint32_t DMAy_IT);
void DMA_ClearITPendingBit(uint32_t DMAy_IT);
```

需要说明, DMA_Init 方法的参数: 
```cpp
void DMA_DeInit(DMA_Channel_TypeDef* DMAy_Channelx); // 其中Channel 是选择通道,y,x表示对应

void DMA_Init(DMA_Channel_TypeDef* DMAy_Channelx, DMA_InitTypeDef* DMA_InitStruct); // 第一个参数是通道, 根据解释, 可以通过y选择组而x选择通道值  
  * @param  DMAy_Channelx: where y can be 1 or 2 to select the DMA and
  *   x can be 1 to 7 for DMA1 and 1 to 5 for DMA2 to select the DMA Channel. (DMA1-1 - DMA1-7或者DMA2-1-DMA2-5)
```

对于存储器到存储器的转运, 任意通道均相同。 
```cpp title:InitStruct取值
DMA1_InitStruct->DMA_PeripheralBaseAddr = (uint32_t)addrA; 
DMA1_InitStruct->DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;// bit per data ( uint8 )
DMA1_InitStruct->DMA_PeripheralInc = DMA_PeripheralInc_Enable; 

DMA1_InitStruct->DMA_MemoryBaseAddr = (uint32_t)addrB; // convert pointer to uint32
DMA1_InitStruct->DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; // bit per data (uint8) 
DMA1_InitStruct->DMA_MemoryInc = DMA_MemoryInc_Enable;

DMA1_InitStruct -> DMA_Mode = DMA_Mode_Normal;       // choose normal (signle) or circular
DMA1_InitStruct -> DMA_DIR = DMA_DIR_PeripheralSRC; // from Peripheral to Memory
DMA1_InitStruct -> DMA_M2M = DMA_M2M_Enable;                     // enable software trigger  
DMA1_InitStruct -> DMA_BufferSize = length;  // equal to DMA 
DMA1_InitStruct -> DMA_Priority  = DMA_Priority_High;
```
其中， 前三项Perippheral部分和Memory部分分别是Peripheral和Memory的三个参数(一般要选择从外设)
对于数组的地址, 可以使用 $(uint32\_t) arr$ 来获取(32位都是32位地址)  

其二， DMA_DIR参数是指定数据传输方向, 包括外设作为DST部分和外设作为SRC部分; 
DMA_Mode是指定是否重装。 (注意不能与软件触发同时使用)  
另外说明， 有一个**缓冲器大小**参数; 即**以数据单元指定缓存区大小**。 缓冲器参数的传输单元大小**根据传输方向等于外设DataSize或者MemoryDataSize的大小**; 数据单元指定缓存区大小; 即
BufferSize即<b><mark style="background: transparent; color: blue">传输计数器</mark></b>, 即<b><mark style="background: transparent; color: blue">传输数据的个数</mark></b>， 等于源端站点的数据总长度(对应每个数据单元对应源端的DataSize)  `->` 注意**取值范围是0-65535** 
另外在当使用多个通道时， 可以分别通过Priority指定通道的优先级;
> [!CAUTION] 注意
> 在转运函数中， 不要将Peripheral 和Memory参数设置搞混淆, 否则可能出现宽度和数据自增乱的情况。 

<b></b><b><mark style="background: transparent; color: red">另外, 如果希望重复转运，必须在转运完毕之后设置成DISABLE</mark></b>
数组转运完整代码如下（DMA_Cmd即直接进行了软件触发) 
```cpp
#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"
#include <algorithm>

using namespace std;

void DMA1_Init(uint32_t addrA, uint32_t addrB, uint16_t length){ 
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, FunctionalState::ENABLE);
    
    DMA_InitTypeDef* DMA1_InitStruct = new DMA_InitTypeDef();
    DMA_StructInit(DMA1_InitStruct);

    DMA1_InitStruct->DMA_PeripheralBaseAddr = (uint32_t)addrA; 
    DMA1_InitStruct->DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;// bit per data ( uint8 )
    DMA1_InitStruct->DMA_PeripheralInc = DMA_PeripheralInc_Enable;

    DMA1_InitStruct->DMA_MemoryBaseAddr = (uint32_t)addrB; // convert pointer to uint32
    DMA1_InitStruct->DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; // bit per data (uint8) 
    DMA1_InitStruct->DMA_MemoryInc = DMA_MemoryInc_Enable;

    DMA1_InitStruct -> DMA_Mode = DMA_Mode_Normal;       // choose normal (signle) or circular
    DMA1_InitStruct -> DMA_DIR = DMA_DIR_PeripheralSRC; // from Peripheral to Memory
    DMA1_InitStruct -> DMA_M2M = DMA_M2M_Enable;                     // enable software trigger  
    DMA1_InitStruct -> DMA_BufferSize = length;  // equal to DMA 
    DMA1_InitStruct -> DMA_Priority  = DMA_Priority_High;

    DMA_Init(DMA1_Channel1, DMA1_InitStruct); // use DMA1
    // then we can use software trigger
}

void DMA1_Start(){
    DMA_Cmd(DMA1_Channel1, FunctionalState::ENABLE);
}


int main(){
    uint8_t DataA[] = {0x01, 0x02, 0x03, 0x04};
    uint8_t DataB[] = {0,0,0,0};
    OLED_Init();
    DMA1_Init((uint32_t)DataA, (uint32_t)DataB, 4);
    for (int i = 0; i < 4; i++){
        OLED_ShowHexNum(1, 3*i + 1, DataA[i], 2);
    }
    for (int i = 0; i < 4; i++){
        OLED_ShowHexNum(2, 3*i + 1, DataB[i], 2);
    }
    Delay_s(5);
    DMA1_Start();
    for (int i = 0; i < 4; i++){
        OLED_ShowHexNum(3, 3*i + 1, DataA[i], 2);
    }
    for (int i = 0; i < 4; i++){
        OLED_ShowHexNum(4, 3*i + 1, DataB[i], 2);
    }
    while (true){
    }
}

```

```cpp
// give a new value to Buffer
void DMA_Transfer (void){
    DMA_Cmd(DMA1_Channel1, DISABLE);
    DMA_SetCurrDataCounter();
}
```
DMA_Start函数的规范写法:
首先需要启动转运，同时必须等待转运完成。 
参考TCIFx的数据手册, (p283) SET表示转运完成;

我们说明DMA_GetFlag的取值的部分: 主要分为4种标志位:
- DMA_FLAG_GL: 全局标志位 : 
- DMA_FLAG_GL: 转运结束标志位 
- DMA_FLAG_HT: 转运过半标志位 
- DMA_FLAG_TE: 转运错误标志位  
注意读取完毕之后需要进行手动清除标志位;

```cpp title:Start函数代码
// if use another value of buffer, 
void DMA1_Start(uint16_t size){
    DMA_Cmd(DMA1_Channel1, FunctionalState::DISABLE);
    DMA_SetCurrDataCounter(DMA1_Channel1, size);
    DMA_Cmd(DMA1_Channel1, FunctionalState::ENABLE);
    while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == FlagStatus::RESET);
    // wait for transfer complete 
    DMA_ClearFlag(DMA1_FLAG_TC1);
}
```

```cpp fold title:完整代码(存储器到存储器的DMA转运)
#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"
#include <algorithm>

using namespace std;

void DMA1_Init(uint32_t addrA, uint32_t addrB, uint16_t length){ 
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, FunctionalState::ENABLE);
    
    DMA_InitTypeDef* DMA1_InitStruct = new DMA_InitTypeDef();
    DMA_StructInit(DMA1_InitStruct);

    DMA1_InitStruct->DMA_PeripheralBaseAddr = (uint32_t)addrA; 
    DMA1_InitStruct->DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;// bit per data ( uint8 )
    DMA1_InitStruct->DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA1_InitStruct->DMA_MemoryBaseAddr = (uint32_t)addrB; // convert pointer to uint32
    DMA1_InitStruct->DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; // bit per data (uint8) 
    DMA1_InitStruct->DMA_MemoryInc = DMA_MemoryInc_Enable;

    DMA1_InitStruct -> DMA_Mode = DMA_Mode_Normal;       // choose normal (signle) or circular
    DMA1_InitStruct -> DMA_DIR = DMA_DIR_PeripheralSRC; // from Peripheral to Memory
    DMA1_InitStruct -> DMA_M2M = DMA_M2M_Enable;                     // enable software trigger  
    DMA1_InitStruct -> DMA_BufferSize = length;  // equal to DMA 
    DMA1_InitStruct -> DMA_Priority  = DMA_Priority_High;
		
    DMA_Init(DMA1_Channel1, DMA1_InitStruct); // use DMA1
    // then we can use software trigger
}

// if use another value of buffer, 
void DMA1_Start(uint16_t size){
    DMA_Cmd(DMA1_Channel1, FunctionalState::DISABLE);
    DMA_SetCurrDataCounter(DMA1_Channel1, size);
    DMA_Cmd(DMA1_Channel1, FunctionalState::ENABLE);
    while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == FlagStatus::RESET);
    // wait for transfer complete 
    DMA_ClearFlag(DMA1_FLAG_TC1);
}


int main(){
    uint8_t DataA[] = {0x0A, 0x0B, 0x0C, 0x0D};
    uint8_t DataB[] = {0,0,0,0};
    OLED_Init();
    
    OLED_ShowString(1,1, "DataA");
    OLED_ShowString(3,1, "DataB");
    OLED_ShowHexNum(1,8, (uint32_t)DataA, 8);
    OLED_ShowHexNum(3,8, (uint32_t)DataB, 8);
    
    for (int i = 0; i < 4; i++){
        OLED_ShowHexNum(2, 3 * i + 1, DataA[i], 2);
        OLED_ShowHexNum(4, 3 * i + 1, DataB[i], 2);
    }

    DMA1_Init((uint32_t)DataA, (uint32_t)DataB, 4);

    while (true){
        uint8_t temp = DataA[0];
        for (int i = 0; i < 4-1; i++){
            DataA[i] = DataA[i+1];
        }
        DataA[3] = temp;
        for (int i = 0; i < 4; i++){
            OLED_ShowHexNum(2, 3 * i + 1, DataA[i], 2);
            OLED_ShowHexNum(4, 3 * i + 1, DataB[i], 2);
        }
        Delay_ms(500);
        DMA1_Start(4);

        for (int i = 0; i < 4; i++){
            OLED_ShowHexNum(2, 3 * i + 1, DataA[i], 2);
            OLED_ShowHexNum(4, 3 * i + 1, DataB[i], 2);
        }
        Delay_ms(500);
    }
}

```

## (2) DMA + ADC多通道
由于要求是数组中的自增， 而循环获取时，仅使用软件触发即可。

注意: 其中不使用软件触发， 而是使用ADC1进行硬件触发的，为了使用硬件ADC进行触发, 触发通道必须是指定的(参考[[#3. DMA触发控制]]和[[#(4) DMA请求映像]], **ADC1的触发是使用通道1来进行的, 因此只能使用通道1进行硬件触发** 
注意一个重要的事情 : 在ADC使能之后， <b><mark style="background: transparent; color: blue">必须开启ADC到DMA的输出</mark></b>, 即调用 
```cpp title:开始输出重要条件
ADC_DMACmd(ADC1, FunctionalState::ENABLE);
```

最后， 每一次软件调用ADC_Cmd, 都可以使得ADC自动触发DMA转运到对应的数组中。

每一次ADC_Cmd之后，记得将DMA的转运标志位置1
三个硬件触发源, 使用哪一个就开始哪一个的ADC通道。 

注意: 硬件触发不要使用 `DMA_Cmd( DMA1_Channel1, DISABLE);`

由于DMA也是正常的单次模式， 因此在循环前需要再次SetCurrentDataCounter一次
```cpp
DMA_Cmd(DMA1_Channel1, FunctionalState::DISABLE);
DMA_SetCurrDataCounter(DMA1_Channel1, size);
DMA_Cmd(DMA1_Channel1, FunctionalState::ENABLE);
```

注意：此时任何一个都不需要等待, 也不需要ClearFlag函数
```cpp title:两个均为单次代码
#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"
#include <algorithm>

using namespace std;

uint16_t res[] = {0, 0, 0, 0}; // storge the result of all ADC channel 

void ADC1_Init(){
        // use ADC12_IN0 for AD convert 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, FunctionalState::ENABLE); // ADC are all device on APB2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, FunctionalState::ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);   // configure as 6 divide frequency 

    GPIO_InitTypeDef* GPIO_InitStruct = new GPIO_InitTypeDef();
    GPIO_InitStruct->GPIO_Mode = GPIO_Mode_AIN; 
    GPIO_InitStruct->GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_6;
    GPIO_InitStruct->GPIO_Speed = GPIOSpeed_TypeDef::GPIO_Speed_50MHz;

    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_7Cycles5); // sample time is 1.5 ADCCLK cycle
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_7Cycles5); // sample time is 1.5 ADCCLK cycle
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_7Cycles5); // sample time is 1.5 ADCCLK cycle
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 4, ADC_SampleTime_7Cycles5); // sample time is 1.5 ADCCLK cycle

    ADC_InitTypeDef* ADC1_InitStruct = new ADC_InitTypeDef();
    ADC1_InitStruct -> ADC_Mode = ADC_Mode_Independent; // two ADC work independently
    ADC1_InitStruct -> ADC_DataAlign = ADC_DataAlign_Right;
    ADC1_InitStruct -> ADC_ContinuousConvMode = FunctionalState::DISABLE; // continuous mode or single mode 
    ADC1_InitStruct -> ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  // use TRGO for convert
    ADC1_InitStruct -> ADC_ScanConvMode = FunctionalState::ENABLE;    
    ADC1_InitStruct -> ADC_NbrOfChannel = 4; // use 4 Channel  
    ADC_Init(ADC1, ADC1_InitStruct);
}

void DMA1_Init(uint32_t addrA, uint32_t addrB, uint16_t length){ 
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, FunctionalState::ENABLE);
    DMA_InitTypeDef* DMA1_InitStruct = new DMA_InitTypeDef();
    DMA_StructInit(DMA1_InitStruct);

    DMA1_InitStruct->DMA_PeripheralBaseAddr = addrA; 
    DMA1_InitStruct->DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;// bit per data ( uint8 )
    DMA1_InitStruct->DMA_PeripheralInc = DMA_PeripheralInc_Disable;

    DMA1_InitStruct->DMA_MemoryBaseAddr = addrB; // convert pointer to uint32 
    DMA1_InitStruct->DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; // bit per data (uint8) 
    DMA1_InitStruct->DMA_MemoryInc = DMA_MemoryInc_Enable;

    DMA1_InitStruct -> DMA_Mode = DMA_Mode_Normal;       // choose normal (signle) or circular
    DMA1_InitStruct -> DMA_DIR = DMA_DIR_PeripheralSRC;  // from Peripheral to Memory
    DMA1_InitStruct -> DMA_M2M = DMA_M2M_Disable;    //  **** note : we use ADC for trigger here ****
    DMA1_InitStruct -> DMA_BufferSize = length;       
    DMA1_InitStruct -> DMA_Priority  = DMA_Priority_High;

    DMA_Init(DMA1_Channel1, DMA1_InitStruct); //** can only use DMA1 -> note that ADC1 is in Channel1
    // then we can use software trigger
    DMA_Cmd(DMA1_Channel1, FunctionalState::ENABLE); //**CMD must be Enabled while hardware trigger
    
    ADC_DMACmd(ADC1, FunctionalState::ENABLE);

    ADC_Cmd(ADC1, FunctionalState::ENABLE);  // enable ADC1;
    // use calibration for ADC1
    ADC_ResetCalibration(ADC1);  // reset calibration configuration 
    while(ADC_GetResetCalibrationStatus(ADC1) == FlagStatus::SET); // wait for calibration 
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1) == FlagStatus::SET);
}
// ** note : it has been started; when ADC1 is working, it will work automatically

void ADC1_GetValue(uint16_t size){
    DMA_Cmd(DMA1_Channel1, FunctionalState::DISABLE);
    DMA_SetCurrDataCounter(DMA1_Channel1, size);
    DMA_Cmd(DMA1_Channel1, FunctionalState::ENABLE);
    ADC_SoftwareStartConvCmd(ADC1, FunctionalState::ENABLE);
    DMA_ClearFlag(DMA1_FLAG_TC1); // reset the DMA signal
}

int main(){
    OLED_Init();
    ADC1_Init();
    DMA1_Init((uint32_t)&ADC1->DR, (int32_t)res, 4); // use DR as the DataRegister
 
    while (true){
        ADC1_GetValue(4);
        for (int i = 0; i < 4; i++){
            OLED_ShowNum(i + 1, 1, res[i], 5);
        }
    }
}

```

此时只要修改49行, 即成为单次扫描加循环转运 
```cpp
DMA1_InitStruct -> DMA_Mode = DMA_Mode_Circular;       // choose normal (signle) or circular
// 然后修改这个函数 
void ADC1_GetValue(uint16_t size){
    ADC_SoftwareStartConvCmd(ADC1, FunctionalState::ENABLE);
}
```

对于连续扫描 + 循环转运模式, 注意最后一行必须加入ADC_Cmd, 来 
(如果错位，则可能没有放在最后一行)

```cpp
ADC1_InitStruct -> ADC_ContinuousConvMode = FunctionalState::ENABLE; // continuous mode or single mode
DMA1_InitStruct -> DMA_Mode = DMA_Mode_Circular;       // choose normal (signle) or circular 
ADC_Cmd(ADC1, FunctionalState::ENABLE);  // 记得在Init函数末尾加上触发, 必须触发一次!
```

