# 一、定时器 Timer 的概念
### (1) STM32定时器简介 
定时器是获取时序中最为强大功能的接口。 使用定时器中断的编码器接口， 可以更加方便地读取例如旋转编码器的正交波形， 以及进行

定时器基本功能: <b><mark style="background: transparent; color: blue">定时器可以对输入的时钟进行计数，并在计数值达到设定值时触发中断</mark></b>, 即输入一个准确可靠的基准时钟(以系统的主频(72MHz)作为输入基准时钟)，并对此进行计数。例如需要延时1us时, 可以使用计时72次来进行。 

定时器的核心(**时基单元**): 含有<mark style="background: transparent; color: red">16位计数器, 预分频器(可以对计数器时钟进行分频), 自动重装寄存器(计数的目标值)</mark>等等, 72MHz计数时钟下可以实现**最大59.65s的定时**($1/72M * 65536^2$)。 如果这个时间不够长, 则STM32支持级联的模式, 此时可以实现很长的

除了基本的定时中断功能以外，还包含<mark style="background: transparent; color: red">内外时钟源选择、输入捕获、输出比较、编码器接口、主从触发模式</mark>等多种功能(即扩展功能)

|  | 定时器的主要功能 |
| ---- | ---- |
| 1 | 定时中断和内外时钟源选择(如定时器外部时钟， 比一般Delay精确一些) |
| 2 | 输入捕获和输出比较(作为计数器,  用于统计引脚上的电平变化次数。) |
| 3 | 编码器接口 |
| 4 | 主从触发模式等等 |
![[attachments/Pasted image 20240204145207.png]]
STM32根据复杂度和应用场景分为了高级定时器、通用定时器、基本定时器三种类型, 具体而言有如下的功能(其中<mark style="background: transparent; color: red">STM32F103C8T6仅有TIM1-TIM4</mark>, 但高密度的有TIM8), <mark style="background: transparent; color: red">不同的定时器所连接的总线也是不同的</mark> 

| 类型 | 编号 | 总线 | 功能 |
| ---- | ---- | ---- | ---- |
| 高级定时器 | TIM1、TIM8 | APB2 | 拥有通用定时器全部功能，并额外具有重复计数器、死区生成、互补输出、刹车输入等功能(<mark style="background: transparent; color: red">主要是为了驱动三相无刷电机</mark>) |
| 通用定时器 | TIM2、TIM3、TIM4、TIM5 | APB1 | 拥有基本定时器全部功能，并额外具有**内外时钟源选择、输入捕获、输出比较、编码器接口、主从触发模式等功能** |
| 基本定时器 | TIM6、TIM7 | APB1 | 拥有定时中断、主模式触发DAC的功能(和DAC联合使用) |

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
具有缓冲寄存器时， 则对应的自动加载寄存器更改时， 直到这个计数器寄存器溢出， 自动加载影子寄存器才会被更改。 目的是<mark style="background: transparent; color: red">让值的变化和更新事件同步发生， 从而防止中途更改产生错误。 </mark> 
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

## 二、内外时钟源选择过程以及重要函数
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
void TIM_SetCounter(TIM_TypeDef* TIMx, uint16_t Counter);  // 手动设置当前计数值  
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


## 三、定时器中断的初始化代码以及应用

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
	TIM2Base_InitStruct->TIM_Period = 7200 - 1; 		   // Auto-Reload Register set 
	TIM2Base_InitStruct->TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, TIM2Base_InitStruct);     
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);  //******** avoid enter interruption after initialization ********
	TIM_ITConfig(TIM2, TIM_IT_Update, FunctionalState::ENABLE); // enable the interruption 

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

# 四、使用定时器外部使用模式2进行计时
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
