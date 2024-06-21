# 一、定时器 Timer 的概念
### (1) STM32定时器简介 
定时器是获取时序中最为强大功能的接口。 使用定时器中断的编码器接口， 可以更加方便地读取例如旋转编码器的正交波形， 以及进行

定时器基本功能: <b><mark style="background: transparent; color: blue">定时器可以对输入的时钟进行计数，并在计数值达到设定值时触发中断</mark></b>, 即输入一个准确可靠的基准时钟(以系统的主频(72MHz)作为输入基准时钟)，并对此进行计数。例如需要延时1us时, 可以使用计时72次来进行。 

定时器的核心(**时基单元**): 含有<mark style="background: transparent; color: red">16位计数器, 预分频器(可以对计数器时钟进行分频), 自动重装寄存器(计数的目标值)</mark>等等, 72MHz计数时钟下可以实现**最大59.65s的定时**($1/72M * 65536^2$)。 如果这个时间不够长, 则STM32支持级联的模式 (选择一个定时器主模式输出更新信号到 TRGO -> 另一个定时器选择从模式 -> 从模式使用外部时钟1)

除了基本的定时中断功能以外，还包含<mark style="background: transparent; color: red">内外时钟源选择、输入捕获、输出比较、编码器接口、主从触发模式</mark>等多种功能(即扩展功能)

|     | 定时器的主要功能                             |
| --- | ------------------------------------ |
| 1   | 定时中断和内外时钟源选择(如定时器外部时钟， 比一般Delay精确一些) |
| 2   | 输入捕获和输出比较(作为计数器,  用于统计引脚上的电平变化次数。)   |
| 3   | 编码器接口                                |
| 4   | 主从触发模式等等                             |
![[attachments/Pasted image 20240204145207.png]]
STM32根据复杂度和应用场景分为了高级定时器、通用定时器、基本定时器三种类型, 具体而言有如下的功能(其中<mark style="background: transparent; color: red">STM32F103C8T6仅有TIM1-TIM4</mark>, 但高密度的有TIM8), <mark style="background: transparent; color: red">不同的定时器所连接的总线也是不同的</mark> 

| 类型    | 编号                  | 总线   | 功能                                                                                                               |
| ----- | ------------------- | ---- | ---------------------------------------------------------------------------------------------------------------- |
| 高级定时器 | TIM1、TIM8           | APB2 | 拥有通用定时器全部功能，并额外具有重复计数器、死区生成、互补输出、刹车输入等功能(<mark style="background: transparent; color: red">主要是为了驱动三相无刷电机</mark>) |
| 通用定时器 | TIM2、TIM3、TIM4、TIM5 | APB1 | 拥有基本定时器全部功能，并额外具有**内外时钟源选择、输入捕获、输出比较、编码器接口、主从触发模式等功能**                                                           |
| 基本定时器 | TIM6、TIM7           | APB1 | 拥有定时中断、主模式触发DAC的功能(和DAC联合使用)                                                                                     |
### (2) 定时器的内部结构
基本定时器包括一个一个TIMCLK, 连到触发控制器和 DAC 上,  连接到内部时钟CK_INT上, 来源一般都是主频72MHz
![[Excalidraw/二、STM32定时器中断的使用 2024-02-04 14.44.21|700]]
首先，预分频器为0时, 输出频率 = 输入频率 = 72MHz, 而预分频器为1时, 二分频即输入32MHz 
如果写2, 为3分频, 即输出24MHz， 实际分频=预分频值+1, 最大16位65536分频 

计数器自增过程中, 会在CNT处产生中断并进行自动重装。计数过程如下: 
<b><mark style="background: transparent; color: blue">首先计数器从0开始计数, 计数到自动重装寄存器的值， 然后再清零开始下一次计数</mark></b> 

上述图中的<mark style="background: transparent; color: red">上箭头表示会产生中断, 而下箭头表示会产生事件</mark>， 对应"更新事件"部分。

另外, 主模式触发DAC功能，<mark style="background: transparent; color: red">能够让硬件在不受程序控制的条件下自动运行</mark>。即在DAC使用时, 可以用DAC输出一段波形例如类似的正弦波, 可以**不使用让主程序进行频繁中断的方法**。  主模式可以让定时器的更新事件映射到TRGO (Trigger Out)位置, 让TRGO 直接去触发DAC。

而高级定时器即
![[Excalidraw/二、STM32定时器中断的使用 2024-02-04 15.18.56|800]]
对于高级定时器, <mark style="background: transparent; color: red">不仅包括向上计时器, 也包含向下和中央对齐模式</mark>。其中向下为<b><mark style="background: transparent; color: blue">从重装值开始, 自减到0, 申请中断并回到重装值</mark></b> , 而中央对齐是<b><mark style="background: transparent; color: blue">从0开始, 向上增到重装值申请一次, 再向下自减到0申请一次</mark></b> 

除了**内部时钟源**以外: 
1. 还可以**选择TIMx_ETR引脚上的外部时钟(External Clock)**, 例如$TIM2\_ CH1\_ ETR$均在PA0口。此时, 可以在PA0上配置时钟, 上面的也叫**外部时钟模式2**, (也是首选模式)
2. 可以选择TRGI作为时钟源(用于触发输入, 可以触发定时器从模式)， 另外TRGI也可以作为外部时钟， 即**外部时钟模式1** `->` 此时，可以
	1. 以ETR作为时钟输入源(即ETR可以走ETRF, 也可以走TRGI) ; 
	2. 另外也可以走ITR信号, 这个信号是来自其他定时器的， 用于级联定时器(具体连接参考p343, 以及第15章)
	3. TIF_ED输入部分, 连接输入捕获部分的TIMx_CH1, 上升和下降沿均有效, 而<mark style="background: transparent; color: red">这个时钟也可以从TI1FP1和TI2FP2进行获得</mark>, 即可以从CH1引脚和CH2引脚进行输入。
![[attachments/Pasted image 20240204154319.png]]
![[attachments/Pasted image 20240204154817.png]]

> [!NOTE] 说明
> 下面的部分是考虑到输入捕获和频率测量的情况下的应用场景进行设计的。

3. 另外在Encoder Interface 中提供了编码器的接口**即编码模式**, 而右边成为定时器的主模式输出。 例如将更新事件映射到TRGO用于触发DAC等等。
触发输出可以触发其他定时器, DAC或者ADC等等。
下面的是输出比较电路, 连接到CH1-CH4， <mark style="background: transparent; color: red">用于输出PWM波形和作为电机驱动</mark>。 而输入捕获电路对应CH1-CH4引脚, <mark style="background: transparent; color: red">可以用于测量输入方波的频率等等</mark>。
Capture/Compare Register 是输入捕获和输出比较电路共用的寄存器。 但是引脚是共用的, 不能将两者同时使用。


> [!NOTE] 高级定时器的独有结构
> 利用Repetition Counter  , 可以实现定时器计数多次仅触发一次更新数据和更新中断。也是16位的重复计数器(可以再乘65536)
> 对于原始的电路加上了DTG Register(死区生成电路) , 可以输出一对互补的PWM波, 往往可以在四轴飞行器的三相无刷电机上进行使用。 (三相无刷电机需要6个大功率开关管, 对应前三路DTG的互补输出) 而死区生成电路用于防止直通现象 
> 下面的Clock failure from clock controller 和CSS(Clock Security System)用于提供电机驱动安全保障。 当BRK (即BKIN)产生刹车信号时, 进行<mark style="background: transparent; color: red">刹车输入</mark>, 控制电路自动切断电机输出。

定时器产生中断之后，首先中断信号在状态寄存器里置一个中断标志位,  而通过中断输出控制输入到NVIC模块而申请中断。 

### (3) 几个重要时序 
#### 1. 预分频器从0变到1时的时钟情况
CK_PSC是预分频器的输入时钟,  一般为72MHz 
CNK_EN 高电平计数器使能, 低电平计数器停止。
后半段, 预分频器的系数为2(按照预分频计数器看), 而计数器和寄存器是跟随时钟的上升沿进行不断自增的。
在更改预分频计数器频率时，计数频率仍然保持我原来的频率，直到本轮计数完成下一轮计数时起作用。

![[attachments/Pasted image 20240204164023.png|800]]
有关系: 
$$CK\underline{ }CNT  = CK\underline{}PSC / (PSC+1)$$
其中CK_CNT为定时器计数频率, PSC是预分频缓冲器。

#### 2. 计数器溢出时序
![[attachments/Pasted image 20240204170149.png|800]]
其中除了产生更新时间UEV以外还会<mark style="background: transparent; color: red">将更新中断标志UIF置1</mark>, 然后计数器立即置零并重新开始计时。
我们设<mark style="background: transparent; color: red">重装载寄存器的值</mark>为arr, 则计数器的溢出频率等于:
$$CK\underline{}CNT\underline{}OV = CK\underline{}CNT / (ARR+1)$$
此时, 溢出时间为:
$$T\underline{}OV  = \frac{1}{CK\underline{}CNT\underline{}OV}$$
### (4) 更改计数寄存器的缓冲寄存器作用
另外， 预分频器为了防止中途更改数值而造成错误, 设计了<b><mark style="background: transparent; color: blue">缓冲寄存器</mark></b>, 带有缓冲机制的寄存器包括**预分频器, 自动重装寄存器, 捕获比较寄存器**。 (阴影部分) 
![[attachments/Pasted image 20240204171340.png|400]]
具有缓冲寄存器时， 则对应的<b><mark style="background: transparent; color: blue">自动加载寄存器更改时， 直到这个计数器寄存器溢出， 自动加载影子寄存器</mark></b>才会被更改。 目的是<mark style="background: transparent; color: red">让值的变化和更新事件同步发生， 从而防止中途更改产生错误。 </mark> 
![[attachments/Pasted image 20240204171637.png|700]]

### (5) RCC时钟树结构图 
用于STM32产生和配置时钟以及将配置时钟发送到各个外设的系统 ， 时钟树可以参考手册pdf, p92的部分。 
首先， 在主函数之前， 会自动执行一个 SystemInit 函数， 
![[Excalidraw/二、STM32定时器中断的使用 2024-02-04 17.31.39|1000]]
上图中， 有**四个振荡源**， 包括
1. 内部高速8MHz HSI Oscillator Clock (high speed internal oscillator clock)
2. 外部 4-16MHz HSE oscillator Clock   (high speed external )
3. 外部 LSE (32.768kHz) crystal 晶振 (驱动实时时钟电路)
4. 内部低速 (40kHz) > 给看门狗提供时钟 
需要说明: 外部的LSE石英振荡电路比内部RC电路更加稳定。 

在System Init 函数中, **先(1)启动内部的8MHz时钟, 让系统按8MHz运行**, 然后**配置外部时钟2进入锁相环(PLLMUL)实现倍频,  并让8MHz倍频得到72MHz的时钟, 最后等到锁相环稳定之后，选择锁相环的72MHz时钟为系统运行时钟**。


> [!NOTE] 上述设计方法的原理 
> 当外部晶振出问题时, 会导致系统时钟无法按照72MHz运行而是使用8MHz运行。 
> CSS是Clock Security, 即**时钟安全系统**。 可以监测外部时钟的运行状态。 **一旦外部时钟失效， 则自动将外部时钟切换为内部时钟**。

72MHz系统时钟进入AHB总线之后，**APB1总线分频系数为2, 即最大时钟为36MHz**, 而**APB2总线分频系数为1, 最大时钟72MHz**, 
需要说明, **TIM2,3,4是接在APB1总线上的， 但是经过倍频， 所以所有的定时器均为72MHz**, 注意不要修改System Init 的默认值。APB2PherphicalClockCmd 即修改对应的与门控制， 从而实现输出给外设。 

而对应FSMC, SDIO的是直接接入72Mhz主频的。 

# 二、内外时钟源选择过程以及重要函数
### (1) 基本步骤
对于定时器的初始化, 可以放在System中作为常用函数

![[Excalidraw/二、STM32定时器中断的使用 2024-02-04 20.56.12|900]]
初始化定时器主要分为以下几个步骤:
1. 开启RCC内部时钟 
2. 选择时钟源 > 对于定时中断，选用内部时钟源即可。 
3. 对于时基单元的配置 (包括预分频器, 计数器, 自动重装器)
4. 配置中断输出控制 
5. 配置NVIC 并分配优先级 
6. <mark style="background: transparent; color: red">运行控制: 模块配置完毕之后需要使能计数器</mark> 

其中比较重要的定时器配置有如下的几类 : 
```cpp
void TIM_DeInit(TIM_TypeDef* TIMx);  // 重置
// 时基单元使能
void TIM_TimeBaseInit(TIM_TypeDef* TIMx, TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct);
void TIM_TimeBaseStructInit(TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct);   // 时基单元模式初始化(包括计数模式)
void TIM_Cmd(TIM_TypeDef* TIMx, FunctionalState NewState);   // 使能计数器  
void TIM_ITConfig(TIM_TypeDef* TIMx, uint16_t TIM_IT, FunctionalState NewState);  // 使能中断输出信号

// 时钟选择对应的函数 
void TIM_InternalClockConfig(TIM_TypeDef* TIMx);  // 选择内部时钟
void TIM_ITRxExternalClockConfig(TIM_TypeDef* TIMx, uint16_t TIM_InputTriggerSource); // 选择级联ITRx的其他ITRX时钟
void TIM_TIxExternalClockConfig(TIM_TypeDef* TIMx, uint16_t TIM_TIxExternalCLKSource,  // 第二个参数是引脚
                                uint16_t TIM_ICPolarity, uint16_t ICFilter);  // 选择TIx(外部引脚)捕获通道的时钟, ICPolarity 为输入极性, ICFilter 为滤波器
void TIM_ETRClockMode1Config(TIM_TypeDef* TIMx, uint16_t TIM_ExtTRGPrescaler, uint16_t TIM_ExtTRGPolarity,
                             uint16_t ExtTRGFilter);     // 选择ETR通过**外部时钟模式1**所输入的时钟 
void TIM_ETRClockMode2Config(TIM_TypeDef* TIMx, uint16_t TIM_ExtTRGPrescaler,
                             uint16_t TIM_ExtTRGPolarity, uint16_t ExtTRGFilter);  // 选择ETR通过**外部时钟模式2**所输入的时钟 
// 上面的输入都有分频器参数(ExtPrescaler) 可以继续分频, 而
void TIM_ETRConfig(TIM_TypeDef* TIMx, uint16_t TIM_ExtTRGPrescaler, uint16_t TIM_ExtTRGPolarity,
                   uint16_t ExtTRGFilter);   // 单独用来配置 ETR 引脚的预分频器, 极性和滤波器
// 另外加上NVIC的两个函数
void NVIC_PriorityGroupConfig(uint32_t NVIC_PriorityGroup); // 对于`NVIC_PriorityGroupConfig`, 用来中断分组, 传入指定的 
void NVIC_Init(NVIC_InitTypeDef* NVIC_InitStruct);  // 初始化NVIC 
```

除此以外, 初始化结构体中有其他部分函数， 例如**定时器重装模式**和**定时器自动重装初值确定等等**,  以及**预分频值**。 

```cpp
void TIM_PrescalerConfig(TIM_TypeDef* TIMx, uint16_t Prescaler, uint16_t TIM_PSCReloadMode); // 用于单独修改预分频值 (这个模式是更新模式, 包括 -> 一种是在更新事件生效，另一种是写入后手动产生一个更新事件来让这个值立刻生效) 
void TIM_CounterModeConfig(TIM_TypeDef* TIMx, uint16_t TIM_CounterMode);  // 选择新的计数器模式 
void TIM_SelectCOM(TIM_TypeDef* TIMx, FunctionalState NewState); // 自动重装功能配置  -> 选择是否有预装功能
void TIM_SetCounter(TIM_TypeDef* TIMx, uint16_t Counter);           // 手动设置当前计数值
void TIM_SetAutoreload(TIM_TypeDef* TIMx, uint16_t Autoreload); // 设置自动重装器的值

uint16_t TIM_GetCounter(TIM_TypeDef* TIMx); // 获取当前的计数器值
uint16_t TIM_GetPrescaler(TIM_TypeDef* TIMx); // 获取预分频值

// 定时器中断标志位设置 
FlagStatus TIM_GetFlagStatus(TIM_TypeDef* TIMx, uint16_t TIM_FLAG);
void TIM_ClearFlag(TIM_TypeDef* TIMx, uint16_t TIM_FLAG);
ITStatus TIM_GetITStatus(TIM_TypeDef* TIMx, uint16_t TIM_IT);
void TIM_ClearITPendingBit(TIM_TypeDef* TIMx, uint16_t TIM_IT);
```


对于BaseInitTypeDef 参数有如下重要解释: 
```cpp 
TIM_TimeBaseInitTypeDef* TIM2Base_InitStruct = new TIM_TimeBaseInitTypeDef();
TIM2Base_InitStruct->TIM_ClockDivision = TIM_CKD_DIV1; // actually is 0
TIM2Base_InitStruct->TIM_CounterMode = TIM_CounterMode_Up; // counter in upper mode 
TIM2Base_InitStruct->TIM_Period = 0x0000; // loaded into the active Auto-Reload Register at the next update event;   Period 为ARR自动重装器的值(也即周期数);  
TIM2Base_InitStruct->TIM_Prescaler = 0x0000; //  Prescaler 是 PSC 预分频器的值
TIM2Base_InitStruct->TIM_RepetitionCounter = ; // 重复计数器的值 (TIM2没有重复计数器, 仅T1有)
TIM_TimeBaseInit(TIM2, TIM2Base_InitStruct);  
```
滤波器通过固定时钟频率$f$对信号进行采样, 如果电平相同， 则输入信号稳定。则输出采样值, 否则保持输出或者输出低电平。 其中采样频率越低, 则滤波效果好但信号延迟也越大。采样参数输入频率<mark style="background: transparent; color: red">由标准72MHz脉冲的某一个分频获取</mark>, 一般取TIM_CKD_DIV1即可

例如, 定时 1s 的操作可以使用: 
$$定时 =  (重装定时器 + 1) \times (预分频 + 1) \times \frac{初值}{72MHz} \times  重复计数器$$
其中预分频从0开始, 直接取重装定时器为0(由于没有重复), 预分频为10000, (重装定时器 + 1为7200), 即可
两者取值都要在0-65535之间 

### (2) 定时器中断的初始化代码以及应用

定时器2初始化为1s 的定时时间的代码如下: 

需要注意的是, 中断函数在初始化完成之后就立刻进入了一次, 这个是由于TimeBaseInit 强行载入初值，立刻生效导致中断产生。 其解决方法是TIM_ITConfig之前手动调用TIM_ClearFlag(TIM2); 即加上`TIM_ClearFlag(TIM2, TIM_FLAG_Update);` 来清除定时器初值。

```cpp
void TIMER2_INIT(){
	// use APB1 bus Clock  
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM2EN, ENABLE); 

	// firsly choose the internal clock -> use internal clock for drive 
	TIM_InternalClockConfig(TIM2);  // choose internal clock (not necessary -> default)
	
	TIM_TimeBaseInitTypeDef* TIM2Base_InitStruct = new TIM_TimeBaseInitTypeDef();
	TIM2Base_InitStruct->TIM_ClockDivision = TIM_CKD_DIV1; // actually is 0
	TIM2Base_InitStruct->TIM_CounterMode = TIM_CounterMode_Up; // counter in upper mode 
	TIM2Base_InitStruct->TIM_Prescaler = 10000 - 1;        // prescaler 
	TIM2Base_InitStruct->TIM_Period = 7200 - 1; 		         // Auto-Reload Register set 
	TIM2Base_InitStruct->TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, TIM2Base_InitStruct);     
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);  //******** avoid enter interruption after initialization ********
	TIM_ITConfig (TIM2, TIM_IT_Update, FunctionalState::ENABLE); // enable the interruption 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef* NVIC_InitStruct = new NVIC_InitTypeDef();
	NVIC_InitStruct->NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct->NVIC_IRQChannelCmd = FunctionalState::ENABLE;
	NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct->NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(NVIC_InitStruct);
}
```
另外还需要加上定时器的启动函数
```cpp
TIM_Cmd(TIM2, FunctionalState::ENABLE);   // 启动定时器2
```

最终的代码如下: 
```cpp
#include "stm32f10x.h"
#include "OLED.h"

int get_length(int num){
	int length;
	if (num == 0) length = 1;
	else{
		int n = num;
		for (length = 0; n!= 0; n/=10){
			length++;
		};
	}
	return length;
}

int Cnt = 0;

void TIMER2_INIT(){
	// use APB1 bus Clock  
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM2EN, ENABLE); 

	// firsly choose the internal clock -> use internal clock for drive 
	TIM_InternalClockConfig(TIM2);  // choose internal clock (not necessary -> default)
	
	TIM_TimeBaseInitTypeDef* TIM2Base_InitStruct = new TIM_TimeBaseInitTypeDef();
	TIM2Base_InitStruct->TIM_ClockDivision = TIM_CKD_DIV1; // actually is 0
	TIM2Base_InitStruct->TIM_CounterMode = TIM_CounterMode_Up; // counter in upper mode 
	TIM2Base_InitStruct->TIM_Prescaler = 10000 - 1;        // prescaler 
	TIM2Base_InitStruct->TIM_Period = 7200 - 1; 		   // Auto-Reload Register set 
	TIM2Base_InitStruct->TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, TIM2Base_InitStruct);     
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);  // avoid enter interruption after initialization 
	TIM_ITConfig(TIM2, TIM_IT_Update, FunctionalState::ENABLE); // enable the interruption 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef* NVIC_InitStruct = new NVIC_InitTypeDef();
	NVIC_InitStruct->NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct->NVIC_IRQChannelCmd = FunctionalState::ENABLE;
	NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct->NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(NVIC_InitStruct);
}

void TIMER2_START(){
	TIM_Cmd(TIM2, FunctionalState::ENABLE);
}

// interruption function 
extern "C"{
	void TIM2_IRQHandler(void){
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET){
			// note that we all use Update interruption for interrupt 
			Cnt++;
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		} 
	}
}

int main(){
	OLED_Init();
	TIMER2_INIT();
	TIMER2_START();
	while(1){
		OLED_ShowNum(1,1, Cnt, 4);
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 从0到7200 开始迅速增加
	}
}
```

### (3) 使用定时器外部使用模式2进行计时
方法是通过ETR的外部时钟模式2配置时钟2, 此时需要使用如下函数:(只要将internal clock Config 修改为如下函数即可) 
```cpp
void TIM_ETRClockMode2Config(TIM_TypeDef* TIMx, uint16_t TIM_ExtTRGPrescaler,
                             uint16_t TIM_ExtTRGPolarity, uint16_t ExtTRGFilter);  // 选择ETR通过**外部时钟模式2**所输入的时钟 
```
这个可以配置触发的方式(低电平或下降沿/高电平或上升沿), 触发滤波器(0x00-0xff)的大小, 

滤波器具体可以参考p405进行控制寄存器配置。 滤波器在p406ETF \[3:0] 部分

另外, 尽可能不要选择浮空输入, 浮空输入的适用范围是信号的功率很小导致内部上拉电阻可能影响输入信号， 此时可以考虑浮空输入。 

另外, 我们这里使用Timer1和 PA1_ETR即PA12进行中断配置(<b><mark style="background: transparent; color: blue">注意: 只有带有ETR的才能进行外部输入, TIM3, TIM4没有此功能</mark></b>), 由于每个定时器的外部端口是指定的， 不要相互搞混(Timer2的是PA0), 同时使用如下的初始化函数:
```cpp
RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, FunctionalState::ENABLE);
```
另外中断函数和触发部分也要换: TIM1_UP_IRQHandler(void) 
```cpp 
#include "stm32f10x.h"
#include "OLED.h"

void TIMER1_Init(){
	// Init P0_1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, FunctionalState::ENABLE); // use PA0 as TIM2_CH1_ETR
	GPIO_InitTypeDef *GPIO_InitStructure = new GPIO_InitTypeDef();
	GPIO_InitStructure->GPIO_Mode = GPIOMode_TypeDef::GPIO_Mode_IPU;
	GPIO_InitStructure->GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure->GPIO_Speed = GPIOSpeed_TypeDef::GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, GPIO_InitStructure);
	// use APB1 bus Clock 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, FunctionalState::ENABLE);
	// use external Clock
	TIM_ETRClockMode2Config(TIM1, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x0F);
	
	TIM_TimeBaseInitTypeDef* TIM1Base_InitStruct = new TIM_TimeBaseInitTypeDef();
	TIM1Base_InitStruct->TIM_ClockDivision = TIM_CKD_DIV1; // actually is 0
	TIM1Base_InitStruct->TIM_CounterMode = TIM_CounterMode_Up; // counter in upper mode 
	TIM1Base_InitStruct->TIM_Prescaler = 1 - 1;        // prescaler 
	TIM1Base_InitStruct->TIM_Period = 5 - 1; 		   // Auto-Reload Register set 
	TIM1Base_InitStruct->TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, TIM1Base_InitStruct); 
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);  // avoid enter interruption after initialization 
	TIM_ITConfig(TIM1, TIM_IT_Update, FunctionalState::ENABLE); // enable the interruption

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef* NVIC_InitStruct = new NVIC_InitTypeDef();
	NVIC_InitStruct->NVIC_IRQChannel =  TIM1_UP_IRQn;
	NVIC_InitStruct->NVIC_IRQChannelCmd = FunctionalState::ENABLE;
	NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct->NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(NVIC_InitStruct);
}

void TIMER1_START(){
	TIM_Cmd(TIM1, FunctionalState::ENABLE);
}

// interruption function 
extern "C"{
	void TIM1_UP_IRQHandler(void){
		if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET){
			TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
		}
	}
}

int main(){
	OLED_Init();
	TIMER1_Init();
	TIMER1_START();
	while(1){
		OLED_ShowNum(1,1, Cnt, 4);
		OLED_ShowNum(2,1, TIM_GetCounter(TIM2), 5);
		OLED_ShowNum(3,1, TIM_GetCounter(TIM1), 5); 
	}
}
```

这样就得到了计次到4自动重置的对射红外传感器

# 三、定时器的输出比较功能
### (1) PWM 简介 
输出比较功能(Output Compare, OC)是用于输出指定脉宽PWM波形的功能。 PWM只能在惯性系统中使用。 
通过STM32可以<mark style="background: transparent; color: red">任意控制电机旋转的方向以及速度</mark>。

对于输出比较器， 可以<mark style="background: transparent; color: red">通过 CNT 和 CCR 寄存器的关系</mark>, 实现输出电平的置零， 置一等操作， 从而输出一定频率和占空比的波形部分。 可以**等效为数模转换功能**。

对于每个通用定时器， 都有4个输出的比较通道, 且高级定时器的三3个通道拥有死区生成与互补输出的功能。 共用一个CCR寄存器。 

CNT是单元计数器, 而CCR为下方4个捕获比较寄存器， 包括输入捕获和输出比较。 
![[attachments/Pasted image 20240206154447.png]]


1.  PWM波频率越大, 等效的模拟量就越平稳， 但是性能开销也增加。
$$占空比 = \frac{高电平}{周期}\qquad  分辨率 = 占空比变化步距$$
一般1%的分辨率就足够使用了。

### (2) 输出比较通道的比较原理
通用输出比较通道原理如图: 
首先, CNT 和CCR1相互比较， 当CNT > CCR1或者CNT = CCR1时, 给输出模式的控制器传入一个信号。

传入信号之后， <mark style="background: transparent; color: red">输出模式的控制器改变对应输出OC1REF的电平</mark>。 并可以将REF映射到主模式的TRGO输出上(即主模式控制器)。 

另外， OC1REF的信号会接到<mark style="background: transparent; color: red">输出使能电路（TIMx_CCER）上</mark>,  如果给==CC1P寄存器==写入0 则信号电平翻转(右侧的部分), 而写入1时, 则将选取取非路径, 此时输出使能电路得到的是信号的反转信号。即==CC1P起到了极性选择的作用==。 
CC1P可以在p417查到, 而
![[Excalidraw/二、STM32定时器中断的使用 2024-02-06 16.15.32|600]]

控制模式器的输入是CNT和CCR的大小关系, 而OC1REF的输出可以通过控制OC1M寄存器进行控制， 而OC1M寄存器在CCMR(CC模式寄存器1) 中可以找到(p412), 获取到OC1M的模式设置如下: 
![[attachments/Pasted image 20240206163710.png|700]]
例如000是冻结模式, 可以用于暂停输出和产生时基，捕获保持某个时刻电平等等。

1. 可以使用010, 即与TIMx_CCR1所设置的值相同时, 设置为**无效电平**(需要说明有效电平可以是低电平, 外部可以通过CC1P控制是否反相)。 但是置低之后, 由于不会恢复, 则好需要001来置高电平， 但是多次操作往往耗费机时; <mark style="background: transparent; color: red">因此不适用于输出连续变化的波形</mark> 
2. 匹配电平翻转可以用于生成占空比50%的波形(设置为0) 
3. 最为重要的是PWM 模式1(110) 和 PWM模式2(111),  用于<mark style="background: transparent; color: red">输出频率和占空比都可调的PWM波形</mark>, 即在CNT < CCR 为有效而 CNT > CCR 为无效。 (这是向上计数模式的情况下) 

### (3) 输出 PWM 的基本结构框图 
#### 1. 基本定时器的输出比较电路
![[Excalidraw/二、STM32定时器中断的使用 2024-02-04 15.18.56|800]]
基本运行如图: 参考上方图片, 输入CNT的实际上是讲过PSC分频之后的TRGI信号, 因而可以通过配置PSC和Auto Reload Register实现不同的计数的时间长度和范围。
![[attachments/Pasted image 20240206170804.png|600]]

| 参数 | 计算公式 |  |
| ---- | ---- | ---- |
| PWM频率： | Freq = CK_PSC / (PSC + 1) / (ARR + 1) |  |
| PWM占空比： | Duty = CCR / (ARR + 1) | 注意应当是 ARR的数值+1 |
| PWM分辨率： | Reso = 1 / (ARR + 1) | 即CCR的范围, 每一次CCR取不同的值(例如自增1)时, 占空比变化的最小步距大小 |

#### 2. 高级定时器的输出结构原理图
高级定时器的1-3的输入原理图如图所示: 通过死区电压
![[Excalidraw/二、STM32定时器中断的使用 2024-02-06 17.27.34|800]]
外部电路往往需要一个大功率电子开关(MOS管) 控制**类似于推挽输出的电路**， 而**两个推挽电路可以形成 H桥电路**， 从而控制电机的正转和反转。  而**三个推挽电路可以驱动三相无刷电机**。 

显然两个管不能同时导通, 因此**两个MOS的输出应当是互补的**。 相应地， OC1, OC1N为**两个互补的输出端口**， 从而控制导通和关闭。为了**防止上下管同时导通， 加入了死区产生电路设计**(通过上/下管关闭时的延时保证上管和下管有小段时间的均不导通控制)。

捕获/比较电路的主要电路结构如图所示:
![[attachments/Pasted image 20240206162847.png|800]]

### (4) 软件编程 
1. 开启外部时钟, 将TIM外设(TIM1)以及GPIO外设的相关时钟(GPIOA时钟)打开
2. 配置定时器时基单元 
3. 配置输出比较单元 (其中包括CCR的值, 输出比较模式, 极性选择, 输出使能函数) // 注意: 不用配置中断NVIC 
4. 配置GPIO, 将PWM对应的GPIO口配置为复用的推挽输出 
5. 运行控制 `->` 打开计数器

相关函数: 
首先是初始化相关输入比较函数。 
```cpp
void TIM_TimeBaseInit(TIM_TypeDef* TIMx, TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct); // 初始化定时器 
void TIM_OC1Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct); // 用结构体进行初始化 OC (Output Compare) 比较功能
void TIM_OC2Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
void TIM_OC3Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
void TIM_OC4Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
void TIM_OCStructInit(TIM_OCInitTypeDef* TIM_OCInitStruct);  // 用于给 OCStruct 赋值一个默认值

void TIM_ForcedOC1Config(TIM_TypeDef* TIMx, uint16_t TIM_ForcedAction);  // 配置强制输出模式->用于暂停和强制输出高低电平 
void TIM_OC1PreloadConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload);    // 配置CCR寄存器的预装功能设置(影子寄存器功能) 
void TIM_OC1FastConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCFast); // 配置快速使能 -> 用的不多, 参考手册单脉冲模式(非重点) 
void TIM_ClearOC1Ref(TIM_TypeDef* TIMx, uint16_t TIM_OCClear); // 外部事件时清除(非重点)


// 单独进行输出比较极性配置  ->  其中N是互补通道的配置
void TIM_OC1PolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPolarity);
void TIM_OC1NPolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCNPolarity); 
// 配置定时器的**输出使能参数** 
void TIM_CCxCmd(TIM_TypeDef* TIMx, uint16_t TIM_Channel, uint16_t TIM_CCx);
void TIM_CCxNCmd(TIM_TypeDef* TIMx, uint16_t TIM_Channel, uint16_t TIM_CCxN);
// ****选择输出比较模式**** -> 即OC1M寄存器设置函数 
void TIM_SelectOCxM(TIM_TypeDef* TIMx, uint16_t TIM_Channel, uint16_t TIM_OCMode)
// **** 用于单独更改 CCR 寄存器的值 -> 用于更改比较值, 即更改输出信号的占空比 **********
void TIM_SetCompare1(TIM_TypeDef* TIMx, uint16_t Compare1);
void TIM_SetIC1Prescaler(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC);
```


```cpp title:初始化函数
TIM1_OCInitStruct->TIM_OCMode = TIM_OCMode_PWM1; // 从多种模式中选择

#define TIM_OCMode_Timing                  ((uint16_t)0x0000)
#define TIM_OCMode_Active                  ((uint16_t)0x0010)
#define TIM_OCMode_Inactive                ((uint16_t)0x0020)
#define TIM_OCMode_Toggle                  ((uint16_t)0x0030)
#define TIM_OCMode_PWM1                    ((uint16_t)0x0060)
#define TIM_OCMode_PWM2                    ((uint16_t)0x0070)
```

需要注意的是, 结构体每个变量都是要首先赋一个初始值的。由于是局部变量, 不赋初始值可能会导致初始化不确定和丢失的问题，因此首先需要使用StructInit 赋初值

```cpp title:极性的说明
TIM1_OCInitStruct->TIM_OCPolarity  // 是设置REF有效电平是否翻转 

// 参数如下:
#define TIM_OCPolarity_High            // 输出的REF高电平有效, 即不翻转 
#define TIM_OCPolarity_Low             // 输出的REF低电平有效, 翻转
#define IS_TIM_OC_POLARITY
```

```cpp title:下述三个值共同决定PWM的输出和占空比
    TIM1_BaseInitStruct->TIM_Prescaler = 1440 - 1;  // PSC
    TIM1_BaseInitStruct->TIM_Period = 1000 - 1;     // ARR
    TIM1_OCInitStruct->TIM_Pulse = 1000;        // (CCR setting) Specifies the pulse value to be loaded into the Capture Compare Register
```

需要说明的是， 我们是使用CH1复用端口来输出对应的呼吸灯的PWM波形的, 因此选用TIM1-> CH1端口, 此时端口为PA8, 也是LED需要接入的端口。

而对于CH1的配置是在TIM_OC1Init的部分的函数中进行初始化。 
> [!caution] 使用复用推挽输出的条件
> 首先， 对于普通的IO引脚, 输出数据控制权是输出寄存器；而**如果希望使用定时器控制引脚, 则需要使用复用开漏或者推挽输出模式**, 此时， 输出控制权转移给片上外设， 才能使用PWM和定时器进行输出
> 另外， 给出一个复用推挽的示例代码如下:
> 具体可以参考AFIO寄存器， 

另外, GPIO_Pin_8 的输出模式应该使用为复用推挽输出 `GPIO_AF_PP`  

我们在PA8上输出1kHz, 占空比为50% 的PWM波, 则
72 M/(PSC + 1)/(ARR+1) = 1000;
ARR = 100 - 1;
PSC = 720-1;
CCR = 50;

注意: TIM_CtrlPWMOutputs(TIM1, ENABLE); 需要在TIM1 使用DMA时额外加入 

```cpp title:定时器2PWM初始化代码(定时器1不能搬运)
#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"

using namespace std;
void PWM2_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		//GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	
	TIM_Cmd(TIM2, ENABLE);
}

int main(){
	PWM_Init();
	int mode = 0, num = 0;
	while (1){
		if (num > 95){
			mode = 1;
		}
		else if (num < 1) {
			mode = 0;
		}
		if (mode == 0) num ++;
		else num--;
		Delay_ms(20);
		TIM_SetCompare1(TIM2,num); //
	}
}

```

当主循环夹杂OLED_Show函数时,高电平时间稍长, 而不严格是50%的占空比,  而是相对50%偏大。
![[attachments/Pasted image 20240206234405.png]]

对于高精度的情况下, 定时器仍然难以精确输出, 但是在范围较大的情况下, 还是能够做到较为精确的输出: 
![[attachments/Pasted image 20240207000458.png]]

针对呼吸灯, 修改TIM_OCInitStructure.TIM_Pulse的函数是TIM_Set Compare函数。 
因此， 为了实现呼吸灯， 只要使用以下代码: 
```cpp
int mode = 0, num = 0;
	while (1){
		if (num > 80){
			mode = 1;
		}
		else if (num < 10) {
			mode = 0;
		}
		if (mode == 0) num ++;
		else num--;
		Delay_ms(5);
		TIM_SetCompare1(TIM2,num); //
	}
```

### (5) 端口的重映射
> [!NOTE] 端口的重映射
> 另外，如果复用端口使用冲突导致无法使用， 可以根据重定义的复用功能进行重映射。而重映射是使用AFIO完成的。例如TIM2_CH1的重映射, 可以从PA0移动接到PAPA15端口上来。

```cpp title:复用推挽的示例代码
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  // 首先开启AFTO时钟
GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);     // 其中第一个选项为重映射的方式 
GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  // 解除原先PA15端口的调试端口JTAG复用功能
```

参考手册p178, 重映射分为不同的重映射方式， 对应不同的引脚, 可以通过8位来控制4个寄存器 
一般是部分重映射1，部分重映射2和完全重映射。 对应`GPIO_PartialRemap1_TIM2`, 

对于初始化时， 就是使用为其他功能的端口, 例如PA15初始时是作为调试端口JDTI使用的, 为了关闭其调试功能, 可以使用AFIO先关闭端口的复用。

调试端口分为SWD, JTAG。可以通过AFIO 设置PB4取消NJRST的复用功能，<mark style="background: transparent; color: red">我们往往可以考虑解除JTAG的复用而保留SWD的复用功能</mark>; 一般复用仅需上面两句， 如果有调试功能， 则加上第三句。

需要注意: 不要调用这个(会解除SWJ和JTAG的一切调试功能 -> PA13 - PB4全部成为普通GPIO, 同时STLINK无法再下载程序，此时只能通过串口下载),
```cpp
@arg GPIO_Remap_SWJ_JTAGDisable  : JTAG-DP Disabled and SW-DP Enabled 
```

### (6) 驱动舵机和直流电机
需要说明的是, 对于共用一个CH端口的部分， 输出PWM波形的频率是一样的, 而占空比由各自的CCR决定。因此不同CH端口的频率一致, 但是占空比可以各自设定。

注意: 使用CH2时, 则应当改成: 
```cpp
TIM_SetCompare2
```


```cpp title:舵机模式选择代码
#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"

using namespace std;

void PWM2_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;		//GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1440 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  // OC2Init
	TIM_Cmd(TIM2, ENABLE);
}

#define Key1 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)

int main(){
	int mode = 0;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	GPIO_InitTypeDef* GPIO_InitStructure = new GPIO_InitTypeDef();
	GPIO_InitStructure->GPIO_Mode = GPIOMode_TypeDef::GPIO_Mode_IPU;
	GPIO_InitStructure->GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure->GPIO_Speed = GPIOSpeed_TypeDef::GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, GPIO_InitStructure);

	PWM2_Init();
	while (1){
		if (!Key1){
			Delay_ms(10);
			while(!Key1);
			mode = (mode + 1)%5;
			Delay_ms(10);
		}
		switch (mode){
			case 0:
				TIM_SetCompare2(TIM2, 25); // 0 degree
				break;
			case 1:
				TIM_SetCompare2(TIM2, 50);
				break;
			case 2: 
				TIM_SetCompare2(TIM2, 75);
				break;
			case 3: 
				TIM_SetCompare2(TIM2, 100);
				break;
			case 4: 
				TIM_SetCompare2(TIM2, 125);
				break;
			default:
				break;
		}
	}
}

```

直流电机驱动: 
首先: 除了VCC, GND以外, PWMA用于速度控制, 而

另外， 需要说明的是, 如果将定时器的频率调高(20kHz左右), 则电机堵转时就不会发出蜂鸣器的响声
例如(Period  = 100-1, Prescaler = 36-1), 如果是50Hz, 则可能发出蜂鸣器响声。

```cpp title:电机代码 fold
#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"

using namespace std;

void PWM2_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;		//GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1440 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  // OC2Init
	TIM_Cmd(TIM2, ENABLE);
}

#define Key1 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)

int main(){
	int mode = 0;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	GPIO_InitTypeDef* GPIO_InitStructure = new GPIO_InitTypeDef();
	GPIO_InitStructure->GPIO_Mode = GPIOMode_TypeDef::GPIO_Mode_IPU;
	GPIO_InitStructure->GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure->GPIO_Speed = GPIOSpeed_TypeDef::GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, GPIO_InitStructure);

	PWM2_Init();
	while (1){
		if (!Key1){
			Delay_ms(10);
			while(!Key1);
			mode = (mode + 1)%5;
			Delay_ms(10);
		}
		switch (mode){
			case 0:
				TIM_SetCompare2(TIM2, 0); 
				break;
			case 1:
				TIM_SetCompare2(TIM2, 400); // 0 degree
				break;
			case 2:
				TIM_SetCompare2(TIM2, 600);
				break;
			case 3: 
				TIM_SetCompare2(TIM2, 800);
				break;
			case 4: 
				TIM_SetCompare2(TIM2, 1000);
				break;
			default:
				break;
		}
	}
}

```

# 四、TIM 输入捕获功能讲解
### (1) 输入捕获的概念
1. 定时器输入捕获模式即IC(input capture), 包括用于测量频率和占空比等等。
在实际使用中， 我们使用定时器产生的信号接到TIM3_CH1进行测量，也可以使用其他发生器来发生当前的波形(仅使用)

如果是同时希望测量信号的频率和占空比， STM32的输入捕获设计了一个PWMI 模式即 PWM输入模式; <mark style="background: transparent; color: red">可以同时测量频率和占空比的大小</mark>。 

<mark style="background: transparent; color: red">输入捕获模块和输出比较模块的寄存器是共用的, </mark>即共用一个CCR寄存器。

原理: 输入捕获模式下， <mark style="background: transparent; color: red">当前通道的输入引脚出现电平跳变时</mark>， 则<mark style="background: transparent; color: blue">当前的CNT的值将被锁存到CCR中</mark>, 从而用于PWM波形频率， 占空比， 脉冲间隔， 电平持续时间等等的测量。 

另外, 也可以<mark style="background: transparent; color: red">配置为主从触发模式</mark>来<b><mark style="background: transparent; color: blue">实现硬件的全自动测量</mark></b> (此时， 需要测量时可以直接读取CCR寄存器来进行) 

### (2) 信号的测量原理
对于信号的频率测量, STM32只能测量数字信号的频率, 如果想要测量模拟量例如正弦波等的频率， 需要搭建信号预处理电路。

显然可以通过运放比较等等转换为数字信号传给STM32即可。 如果测量的电压比较大，可以利用隔离放大器， 电压互感器等等隔离高压和低压端，从而保证电路的安全。 


主要的测量方法包括: 
1. 测频法 : 在闸门时间T内，对上升沿计次，得到N，则频率
$$f_x=N / T$$
2. 测周法：两个上升沿内，以标准频率fc计次，得到N ，则频率
$$f_x=f_c  / N$$
(要求标准频率必须远大于原始信号的频率，标准频率越高越准确)
3. 中界频率方法(两种切换使用): 测频法与测周法误差相等的频率点
$$f_m=\sqrt{(f_c  / T)}$$
由于闸门时间与测周方法均有正负1误差，为了减小正负1的误差, 当对于一个频率， 同时使用测频法和测周法测得的数值相同时, 即
$$\frac{N}{T} = \frac{f_{c}}{N}\rightarrow  N = \sqrt{\frac{f_{c}}{T}}$$此时, 如果待测频率小于中介频率, 则测周法准确； 而待测频率大于中界频率， 则测频方法准确。
> [!NOTE] 适用范围
> 1. 测频方法适用于测量高频信号, 测周方法适用于测量低频信号; 
> 2. 由于测频方法测量的是闸门时间内的多个周期,  相当于自带一个均值滤波器，如果过程中的频率有变化， 则测得的是平均频率。 结果更新比较慢; 
> 3. 而测周法更新更快， 一般瞬时频率， 但是结果受到噪声影响， 容易结果不稳定

例如使用外部中断的方法，可直接修改外部中断代码来实现频率测量, 这个也是一般的测频方法， 比较容易； 


**三输入异或门的逻辑**: <mark style="background: transparent; color: red">任何一个输入发生翻转</mark>时, 则输出都发生一次翻转。
![[Excalidraw/二、STM32定时器中断的使用 2024-02-04 15.18.56|1000]]
根据如图电路: <mark style="background: transparent; color: red">如果使用通道1, 则输入应当是三个引脚的异或值</mark>; 而如果选用TI2或者TI3, 则为原始输入。 

实际上在STM32中， 我们往往使用测周方法确定信号的频率。 
> [!NOTE] 异或门的设计讲解
> 对于通道1的异或门， 是为三相无刷电机设计的;  由于无刷点击有3个霍尔传感器检测转子的位置。 并根据转子的位置换相;  
> 在前3个口接入无刷电机霍尔传感器， 此时可以作为接口定时器驱动无刷电机工作。 

### (3) 信号基本流向
1. 输入部分
另外 TI1FP1 (TI1 Filter Polarity 1)和 TI1FP2 分别输入TI1和TI2的电路; 这个是设置了两套滤波和两套极性选择电路部分。 因而可以选择进行交叉输入连接(TI1输入CC2, TI2输入CC1);  
其主要目的是可以<b><mark style="background: transparent; color: blue">将一个引脚的输入同时映射到两个捕获单元</mark></b> `->` 即为 PWMI 模式的经典结构: 其中**第一个使用上升沿触发, 捕获周期; 而第二个使用下降沿触发, 用于捕获占空比**。 
此外, 也可以选用TRC信号作为捕获部分的输入。
2. 预分频部分
利用Prescaler 对输入的信号进行预分频， 之后再进行捕获。 捕获瞬间也可以通过配置捕获中断来触发中断。 
3. 测周方法的原理
利用定时器的标准时钟(72MHz)进行计数;<mark style="background: transparent; color: red"> 通过CNT的数值记录上升沿的间隔</mark>; 从而可以测量对应的频率; 
<mark style="background: transparent; color: red">在一次捕获之后将CNT自动清零</mark>的步骤<b><mark style="background: transparent; color: blue">可以使用主从触发模式来进行自动完成</mark></b>。 
输入捕获阶段的基本原理图如下图所示: 
![[Excalidraw/二、STM32定时器中断的使用 2024-02-08 00.00.36|800]]
参考p350, 包括IC1F位(定义输入滤波器参数), 用于对 TI1 进行采样, 并且带有可以自定义N的事件计数器， 只有连续n个相同值的情况下, 才能对应设置(输出)出一个有效的电平。

如果波形的噪声较大， 可以将N稍微设置大， 从而更好地滤波。

在有效信号产生之后, CNT的值转运到CCR 中; 而硬件电路中, TI1FP1信号以及TI1的边沿信号各自作用于 slave mode controller (从模式控制器) , 只要设置用TI1FP1的<mark style="background: transparent; color: red">上升沿触发从模式</mark>,  而从模式可以进行自动清零。

### (4) 主从触发模式的原理 (重点)
![[attachments/Pasted image 20240208001527.png|600]]
主从触发模式， 包括**主模式, 从模式和触发源选择**三个部分。**主模式**可以**将定时器内部的信号映射到TRGO引脚**, 从而<mark style="background: transparent; color: red">触发别的外设</mark>;

从模式是**接受其他外设或者自身外设的信号**, 用于控制自身定时器的运行。而可以通过**触发源选择选取不同的控制信号触发源**。
过程是首先选用指定触发源 `->` 获得触发信号 TRGI `->` 执行右侧表中的某一个操作(置零Reset) 

具体寄存器可以在p403寻找; 从模式的触发信号在p406, 寄存器如下
![[attachments/Pasted image 20240208003120.png]]
具体而言，从模式可以通过控制如下的指令进行触发: 
![[attachments/Pasted image 20240208003418.png|800]]
通过如下方法选择Slave Mode 的操作(我们往往用到100的Reset操作) 
![[attachments/Pasted image 20240208003507.png|600]]

### (5) 输入捕获和PWMI的基本结构
**输入捕获基本结构**如下图:  
![[attachments/Pasted image 20240208004251.png]]
1. 配置时基单元, 启动定时器; 包括
2. 首先选择TI1FP1为上升沿触发, ；
3. 选择为从模式并配置从模式, 每一次先转运CNT到CCR, 再触发从模式来实现CNT的清零操作 (第一条由输入捕获自动执行, 而第二条由从模式自动执行)

如果**信号频率太低且分频少, 则CNT可能会溢出**。因此我们一般要设置合适的分频保证1s内不溢出。 

另外, 对于使用从模式自动清零CNT, 只能使用通道1,2; 而如果用3, 4只能在开启捕获中断之后手动清零即可。

**PWMI的基本结构**如下:  即通过TI1FP2配置下降沿触发，从而**实现在下降沿触发时存储一个CNT中的值， 即为高电平的时间；这个值除CCR1中的值， 即可得到占空比的大小**。注意需要配置两个通道同时捕获引脚1的输入。 
![[attachments/Pasted image 20240208004902.png]]

定时器同步(p401)中有对应的主从触发模式介绍

# 五、TIM 输入捕获软件编程
### (1) 运行过程中调节频率的方法
首先:  我们给出可以通过按钮调节频率的方法, 初始化PA5作为按钮
由于PSC 和ARR都可以调节频率, 但是需要注意: <mark style="background: transparent; color: red">调节ARR会影响到占空比</mark>。 而占空比可以使用 `TIM_SetCompare` 来进行, 
首先回忆频率公式: 
$$freq  = \frac{72MHz}{(PSC + 1)(ARR + 1)} \qquad  Duty = \frac{CCR}{ARR  +  1}$$
例如， 如果希望分辨率更高, 则可以固定ARR为较高的数值

```cpp title:单独写入PSC
void TIM_PrescalerConfig(TIM_TypeDef* TIMx, uint16_t Prescaler, uint16_t TIM_PSCReloadMode);
// 库的命名规范是当有 TIM_PSCReloadMode 模式设置等参数时， 则称为Config 


TIM_PrescalerConfig(TIM2, Prescaler, TIM_PSCReloadMode_Update); // 其中第三个参数是设置是否用影子寄存器等待更新时重装。 
```

参考[[#(5) 输入捕获和PWMI的基本结构]], 输入捕获包括: 
1. 时钟设置(GPIO, TIM)和 GPIO配置 (上拉或者浮空输入)
2. 初始化定时器时基单元， 
3. 配置输入捕获单元， 包括滤波器， 边沿检测和极性选择， 以及分频器和捕获/比较器；
4. 选择<mark style="background: transparent; color: red">触发源选择为TI1FP1</mark>, 选取触发之后的操作为Reset操作
5. 配置完毕之后开启定时器 (TIM_cmd)

### (2) 输入捕获的初始化函数

```cpp
void TIM_ICInit(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct); // 用于初始化某个定时器为输入捕获模式(IC模式) 
void TIM_PWMIConfig(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct); // 初始化某个定时器为PWMI模式 (注意ICInit 是单一配置一个通道, 而PWMI Config可以快速配置两个通道) -> 此时将两个通道直接修改为PWMI 通道

void TIM_ICStructInit(TIM_ICInitTypeDef* TIM_ICInitStruct); // 给IC初始化结构体赋值一个初始值 
void TIM_SelectInputTrigger(TIM_TypeDef* TIMx, uint16_t TIM_InputTriggerSource); // 选择输入触发源 TRGI 
void TIM_SelectOutputTrigger(TIM_TypeDef* TIMx, uint16_t TIM_TRGOSource); // 选择输出触发源 TRGO 
void TIM_SelectSlaveMode(TIM_TypeDef* TIMx, uint16_t TIM_SlaveMode);  // 选择从模式 
void TIM_SetIC1Prescaler(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC);  // 分别配置 IC1, 2, 3, 4 的分频器 
uint16_t TIM_GetCapture1(TIM_TypeDef* TIMx);  // 分别读取四个通道的CCR  -> 注意: GetCpature1,2,3,4 分别是和SetCompare函数对应的, 都是操作所共用的 CCR寄存器的值。
```

其中TRGI 和 TRGO 分别为 从模式的输入触发源和主模式的输出触发源， 如下图所示。TRGI可以配置为TI1FP1,  而 TRGO可以选择为Enable 
![[Excalidraw/二、STM32定时器中断的使用 2024-02-09 23.43.01|700]]
```cpp
TIM3_ICInitStruct->TIM_ICFilter= 4; // make it bigger for more stable signal
TIM3_ICInitStruct->TIM_ICPolarity = TIM_ICPolarity_Rising; // 设置触发 -> Rising为上升沿触发
TIM3_ICInitStruct->TIM_ICPrescaler = TIM_ICPSC_DIV1;      // 预分频， 即几次触发记作一次有效-> DIV1时是每次触发都有效。 
TIM3_ICInitStruct->TIM_ICSelection = TIM_ICSelection_DirectTI; // 选择信号从哪个引脚进行输入->即配置数据选择器的。
```
通过配置TIM3_CH1引脚为输入引脚。 
```cpp title:其中TIM_ICSelection中包含如下几个参数 fold
#define TIM_ICSelection_DirectTI      // 直连通道
#define TIM_ICSelection_IndirectTI   // 交叉通道
#define TIM_ICSelection_TRC            // TRC 引脚
```
然后配置触发器的触发源和从模式
```cpp
// Configure The Trigger Settings 
    TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);  // 配置触发线触发源
    TIM_SelectSlaveMode(TTIM3, TIM_SlaveMode_Reset);  // 配置从模式 -> 实现全自动的测量
    // 另外的 3 个SlaveMode 参数是编码器使用的
```


### (3) 输入捕获的示波器代码
下面的代码使用PA5做控制引脚, 分别测量100, 200, 400, 1000Hz频率的部分
```cpp fold title:频率测量器
#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"
#include <algorithm>

using namespace std;
// call the function that modify PSC (Prescaler) 
void PWM_SetPrescaler(uint16_t Prescaler){
    TIM_PrescalerConfig(TIM2, Prescaler, TIM_PSCReloadMode_Update);
}

void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		//GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_InternalClockConfig(TIM2);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 7200 - 1;		//ARR, 100 Hz 
	TIM_TimeBaseInitStructure.TIM_Prescaler = 100 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 500;		//CCR
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_Cmd(TIM2, ENABLE);
}

#define Key1 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)

// use PA6 port as the IC 
void InputCapture_Init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, FunctionalState::ENABLE);
    GPIO_InitTypeDef * GPIO_InitStruct = new GPIO_InitTypeDef();
    GPIO_InitStruct->GPIO_Mode = GPIOMode_TypeDef::GPIO_Mode_IPU;
    GPIO_InitStruct->GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStruct->GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, GPIO_InitStruct);
    
    // Timer Configuration 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, FunctionalState::ENABLE);
    TIM_InternalClockConfig(TIM3);
    TIM_TimeBaseInitTypeDef* TIM3_TBInitStruct = new TIM_TimeBaseInitTypeDef();
    TIM3_TBInitStruct->TIM_ClockDivision = TIM_CKD_DIV1;
    TIM3_TBInitStruct->TIM_CounterMode = TIM_CounterMode_Up;
    TIM3_TBInitStruct->TIM_RepetitionCounter = 0;
    TIM3_TBInitStruct->TIM_Prescaler = 72 - 1; // 1M in 1s -> period is 0.000001s
    TIM3_TBInitStruct->TIM_Period = 65535;    // maximum : 0.0655s -> 15Hz lowest 
    TIM_TimeBaseInit(TIM3, TIM3_TBInitStruct);
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);

    // Init IC 
    TIM_ICInitTypeDef* TIM3_ICInitStruct = new TIM_ICInitTypeDef();
    TIM3_ICInitStruct->TIM_Channel = TIM_Channel_1;
    TIM3_ICInitStruct->TIM_ICFilter= 0x3; // make it bigger for more stable signal
    TIM3_ICInitStruct->TIM_ICPolarity = TIM_ICPolarity_Rising; // trigger at rising edge 
    TIM3_ICInitStruct->TIM_ICPrescaler = TIM_ICPSC_DIV1;       // every trigger is valid 
    TIM3_ICInitStruct->TIM_ICSelection = TIM_ICSelection_DirectTI; // Direct Connect 
    TIM_ICInit (TIM3, TIM3_ICInitStruct);
    
    // Configure The Trigger Settings 
    TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1); 
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
    // note that the above 3 parameters are used for the encoder 
    /*
      *     @arg TIM_TS_ITR0: Internal Trigger 0
	  *     @arg TIM_TS_ITR1: Internal Trigger 1
	  *     @arg TIM_TS_ITR2: Internal Trigger 2
	  *     @arg TIM_TS_ITR3: Internal Trigger 3
    */
}

void InputCapture_Start(){
    TIM_Cmd(TIM3, FunctionalState::ENABLE); 
}

int freq = 0; 
void CalculateFreq(void){ // call in main function 
    uint16_t res = TIM_GetCapture1(TIM3);
    freq = 1000000.0 / (float) res;
}

void DispFreq(void){
    OLED_ShowString(1,1, "Freq:");
    OLED_ShowNum(1, 6, freq, 5);
}

int main(){
    OLED_Init();
	PWM_Init();
    // Init PA5 port as btn 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, FunctionalState::ENABLE);
    GPIO_InitTypeDef* GPIO_InitStruct = new GPIO_InitTypeDef();
    GPIO_InitStruct->GPIO_Mode = GPIOMode_TypeDef::GPIO_Mode_IPU;
    GPIO_InitStruct->GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStruct->GPIO_Speed = GPIOSpeed_TypeDef::GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, GPIO_InitStruct); 

	int mode = 0;
    TIM_SetCompare1(TIM2,50); // set the Duty of TM2;

    InputCapture_Init();      // Init Input Capture function 
    InputCapture_Start();

	while (1){
        if (!Key1){
            Delay_ms(5);
            if (!Key1){
                while (!Key1);
                mode = (mode + 1)%4;
                switch (mode)
                {
                    // don't forget - 1 in prescaler 
                    case 0: PWM_SetPrescaler(100-1); break; // 100 Hz 
                    case 1: PWM_SetPrescaler(50-1);  break; // 200 Hz
                    case 2: PWM_SetPrescaler(25-1);  break; // 400 Hz
                    case 3: PWM_SetPrescaler(10-1);  break; // 1000Hz 
                    default: break;
                }
                Delay_ms(10);
            }
        }

        CalculateFreq();
        DispFreq();
	}
}

```

### (4) PWMI 模式的代码 

方法一是使用两个ICInit来初始化两个通道; 
```cpp
    // Init PWMI settings 
    TIM_ICInitTypeDef* TIM3_ICInitStruct1 = new TIM_ICInitTypeDef();
    TIM3_ICInitStruct1->TIM_Channel = TIM_Channel_1;
    TIM3_ICInitStruct1->TIM_ICFilter= 0x3; // make it bigger for more stable signal
    TIM3_ICInitStruct1->TIM_ICPolarity = TIM_ICPolarity_Rising; // trigger at rising edge
    TIM3_ICInitStruct1->TIM_ICPrescaler = TIM_ICPSC_DIV1;       // every trigger is a 
    TIM3_ICInitStruct1->TIM_ICSelection = TIM_ICSelection_DirectTI; // Direct Connect 
    TIM_ICInit (TIM3, TIM3_ICInitStruct1);

    TIM_ICInitTypeDef* TIM3_ICInitStruct2 = new TIM_ICInitTypeDef();
    TIM3_ICInitStruct2->TIM_Channel = TIM_Channel_2;
    TIM3_ICInitStruct2->TIM_ICFilter= 0x3; // make it bigger for more stable signal
    TIM3_ICInitStruct2->TIM_ICPolarity = TIM_ICPolarity_Falling; // trigger at rising edge
    TIM3_ICInitStruct2->TIM_ICPrescaler = TIM_ICPSC_DIV1;       // every trigger is a 
    TIM3_ICInitStruct2->TIM_ICSelection = TIM_ICSelection_InDirectTI; // Direct Connect 
    TIM_PWMIConfig(TIM3, TIM3_ICInitStruct2);
```

上式可以简化为如下代码 :
```cpp 
// Init PWMI settings 
    TIM_ICInitTypeDef* TIM3_ICInitStruct = new TIM_ICInitTypeDef();
    TIM3_ICInitStruct->TIM_Channel = TIM_Channel_1;
    TIM3_ICInitStruct->TIM_ICFilter= 0x3; // make it bigger for more stable signal
    TIM3_ICInitStruct->TIM_ICPolarity = TIM_ICPolarity_Rising; // trigger at rising edge
    TIM3_ICInitStruct->TIM_ICPrescaler = TIM_ICPSC_DIV1;       // every trigger is a 
    TIM3_ICInitStruct->TIM_ICSelection = TIM_ICSelection_DirectTI; // Direct Connect 
    TIM_PWMIConfig(TIM3, TIM3_ICInitStruct);  // auto-configure another port inversely
```


```cpp title:测量频率和占空比完整代码
#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"
#include "math.h"

using namespace std;
// call the function that modify PSC (Prescaler) 
void PWM_SetPrescaler(uint16_t Prescaler){
    TIM_PrescalerConfig(TIM2, Prescaler, TIM_PSCReloadMode_Update);
}

void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		//GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_InternalClockConfig(TIM2);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 7200 - 1;		//ARR, 100 Hz 
	TIM_TimeBaseInitStructure.TIM_Prescaler = 100 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 3600;		//CCR
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_Cmd(TIM2, ENABLE);
}

#define Key1 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)

// use PA6 port as the IC 
void InputCapture_Init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, FunctionalState::ENABLE);
    GPIO_InitTypeDef * GPIO_InitStruct = new GPIO_InitTypeDef();
    GPIO_InitStruct->GPIO_Mode = GPIOMode_TypeDef::GPIO_Mode_IPU;
    GPIO_InitStruct->GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStruct->GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, GPIO_InitStruct);
    
    // Timer Configuration 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, FunctionalState::ENABLE);
    TIM_InternalClockConfig(TIM3); 
    TIM_TimeBaseInitTypeDef* TIM3_TBInitStruct = new TIM_TimeBaseInitTypeDef();
    TIM3_TBInitStruct->TIM_ClockDivision = TIM_CKD_DIV1;
    TIM3_TBInitStruct->TIM_CounterMode = TIM_CounterMode_Up;
    TIM3_TBInitStruct->TIM_RepetitionCounter = 0;
    TIM3_TBInitStruct->TIM_Prescaler = 72 - 1; // 1M in 1s -> period is 0.000001s
    TIM3_TBInitStruct->TIM_Period = 65535 - 1;    // maximum : 0.0655s -> 15Hz lowest 
    TIM_TimeBaseInit(TIM3, TIM3_TBInitStruct);
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);

    // Init PWMI settings 
    TIM_ICInitTypeDef* TIM3_ICInitStruct = new TIM_ICInitTypeDef();
    TIM3_ICInitStruct->TIM_Channel = TIM_Channel_1;
    TIM3_ICInitStruct->TIM_ICFilter= 0x3; // make it bigger for more stable signal
    TIM3_ICInitStruct->TIM_ICPolarity = TIM_ICPolarity_Rising; // trigger at rising edge
    TIM3_ICInitStruct->TIM_ICPrescaler = TIM_ICPSC_DIV1;       // every trigger is a 
    TIM3_ICInitStruct->TIM_ICSelection = TIM_ICSelection_DirectTI; // Direct Connect 
    TIM_PWMIConfig(TIM3, TIM3_ICInitStruct);  // auto-configure another port inversely
    
    // Configure The Trigger Settings 
    TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
    // note that the 3 parameters are used for the encoder 
}

void InputCapture_Start(){
    TIM_Cmd(TIM3, FunctionalState::ENABLE);
}

int freq = 0, duty = 0;
void CalculateFreqDuty(void){ // call in main function 
    int res = TIM_GetCapture1(TIM3);
    int res2 = TIM_GetCapture2(TIM3); // CCR2 
    freq = 1000000.0 / (float) res;
    duty = ceil((float)res2 * 100 /(float)res); // integral ->  /100;
}

void DispFreq(void){
    OLED_ShowString(1,1, "Freq:");
    OLED_ShowNum(1, 6, freq, 5);
    OLED_ShowString(2,1, "Duty:");
    OLED_ShowNum(2, 6, duty, 5);
}

int main(){
    OLED_Init();
	PWM_Init();
    // Init PA5 port as btn 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, FunctionalState::ENABLE);
    GPIO_InitTypeDef* GPIO_InitStruct = new GPIO_InitTypeDef();
    GPIO_InitStruct->GPIO_Mode = GPIOMode_TypeDef::GPIO_Mode_IPU;
    GPIO_InitStruct->GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStruct->GPIO_Speed = GPIOSpeed_TypeDef::GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, GPIO_InitStruct); 
    TIM_SetCompare1(TIM2, 3600); // set the Duty of TM2;
    InputCapture_Init();      // Init Input Capture function 
    InputCapture_Start();

    int mode = 0;
	while (1){
        if (!Key1){
            Delay_ms(5);
            if (!Key1){
                while (!Key1);
                mode = (mode + 1)%4;
                switch (mode)
                {
                    // don't forget - 1 in prescaler 
                    case 0: PWM_SetPrescaler(100-1);TIM_SetCompare1(TIM2,3600); break; // 100 Hz, 50% duty
                    case 1: PWM_SetPrescaler(50-1); TIM_SetCompare1(TIM2,1800); break; // 200 Hz, 25% duty
                    case 2: PWM_SetPrescaler(25-1); TIM_SetCompare1(TIM2,1440); break; // 400 Hz, 20% duty
                    case 3: PWM_SetPrescaler(10-1); TIM_SetCompare1(TIM2,720); break; // 1000Hz, 10% duty
                    default: break;
                }
                Delay_ms(10);
            }
        }

        CalculateFreqDuty();
        DispFreq();
	}
}

```