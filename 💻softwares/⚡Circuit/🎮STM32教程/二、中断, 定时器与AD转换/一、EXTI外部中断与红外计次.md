# 一、使用模板和Vscode进行编辑
首先在Keil中新建工程, 将模板文件除了HardWard(如果用OLED那就加上)全部复制粘贴到下面。
![[attachments/Pasted image 20240202174116.png]]
新建一个main.cpp放到User里， 并配置包含文件和include 路径(注意将md.s也包含上)， 如图所示: 
![[attachments/Pasted image 20240202174705.png]]
在C/C++ > define中添加:
```cpp
USE_STDPERIPH_DRIVER
```
注意也可以使用下面这个方法添加:(可以一并把后面的STM32D10X_MD也加上)
![[attachments/Pasted image 20240223120106.png]]
如图， 添加C/C++ 的Misc Controls `--cpp11` (这个要加上不然OLED导入会报错) 
![[attachments/Pasted image 20240203005523.png|500]]

设置Debug为STLink > Reset and Run 
![[attachments/Pasted image 20240202175109.png]]
写入测试程序并编译`->` 0 错误
```cpp
#include "stm32f10x.h"

int main(){
	while (1){
	}
}

```
然后关闭Keil, 打开VSCode 并导入。 首先修改stm32f10h中的定义(#define)
![[attachments/Pasted image 20240202175419.png]]
此时可以正常编译， 但是不能烧录。此时更改Flasher Configurations, 使用STLink即可。 
![[attachments/Pasted image 20240202180547.png]]
如果还不行， 找到Configure ToolChain > Keil MDK设置为 TOOLs.INI 
![[attachments/Pasted image 20240202180337.png|600]]
此时即可正常运行和烧录。 

对于调试不起作用的情况， 提示没有outdebug , launch.json加上如下代码
```cpp title:调试不起作用launch.json加上代码
{
  "cwd": "${workspaceRoot}",
  "type": "cortex-debug",
  "request": "launch",
  "name": "stlink",
  "servertype": "openocd",
  "executable": "build\\Target 1\\ext_and_ired_test.elf",  // 更改为对应elf可执行文件
  "runToEntryPoint": "main",
  "configFiles": [
	  "interface/stlink.cfg",
	  "target/stm32f1x.cfg"
  ]
}
```
另外， 以本机目前的配置: 只要将Flasher Configurations 改为OpenOcd, 然后调试Debug上面选项使用 stlink即可进行调试
![[attachments/Pasted image 20240204204709.png]]
书签使用:
![[attachments/Pasted image 20240204212201.png]]


> [!NOTE] 说明
> 最快的方式是直接Copy Paste整个文件夹并修改工程名


ctrl + D可以从@ref处直接搜索到对应的group

# 二、STM32中断简介
### (1) STM32 的中断组成
stm32的中断用于对射式红外传感器计次和旋转编码器计次等等。中断概念， 优先级以及嵌套等等, 参考[[💻softwares/⚡Circuit/💻51单片机教程/4. 定时器, 中断, 串口通信与温度传感器/外部中断系统概念与其实现|51单片机外部中断]] 
STM32单片机具有<mark style="background: transparent; color: red">68个可屏蔽的中断通道</mark>(不包含第六代Cortex -M3)中断线，其中有16 个可编程的中断优先级序列。<b><mark style="background: transparent; color: blue">常用的外部中断有EXTI, TIM, ADC, USART, SPI, I2C, RTC等等</mark></b>。(其中68是F1系列最多的中断数量, 因具体型号而不同, 以数据手册为准), 几乎所有的模块都可以申请中断。

使用 NVIC (Nested vectored interrrupt controller)进行统一管理中断， 每一个中断通道都具有16个中断优先等级。 也可以设置优先级分组<mark style="background: transparent; color: red">实现抢占优先级和响应优先级的设置</mark>。
同时, STM32带有7个定时器,  三个16bit定时器, 一个16bit的PWM电机控制计时器(具有紧急停机功能), 2个看门狗,  以及24为的Systick。

参考数据手册(p197),  stm32常用的中断如下图所示(不完全), 其中高亮部分为内核中断 
![[attachments/Pasted image 20240202200145.png|800]]

| **重要中断名称** | 简介 |
| ---- | ---- |
| Reset |  |
| NMI | **不可屏蔽中断**, 由硬件失效, 存储管理, 总线错误与应用错误等等 触发 |
| WWDG | 窗口中断函数, 检测是否程序卡死等等, 可以用于正常 |
| PVD | 电源电压监测(Power Voltage Detection)。供电电压不足时产生中断; 可以用于监测电池和保存重要数据 |
| EXTI0 - EXTI4, EXTI15_10 | 外部中断资源  |
|  |  |
需要说明的是, 中断函数的地址是由编译器分配的， 不是固定值; 而在内存中定义了一个地址列表。(**中断地址列表也叫中断向量表**), 先跳转到中断向量表对应的位置， 然后再跳转到对应的函数地址。
### (2) NVIC简介
NVIC结构如图, 是stm32的CPU内核外置的外设(微处理器8259) 
![[attachments/Pasted image 20240202202023.png|600]]
NVIC 可以对中断按照优先级进行分组。每个**优先级寄存器共有4位(0-15), 由此确定中断的优先级顺序**。
<b><mark style="background: transparent; color: red">同时这个4位可以切分为高n位的抢占优先级和低4-n位的响应优先级</mark></b>

对于**响应优先级**, 是在**CPU目前执行的做完之后**进行判断并执行**响应优先级**较高的中断任务。
而**抢占优先级是对于更为紧急的事物**, 可以直接**打断目前执行的中断程序**直接执行本中断。
**抢占优先级高的可以中断嵌套，响应优先级高的可以优先排队，抢占优先级和响应优先级均相同的按中断号排队(中断号在数据手册中给出)**

**优先级值越小, 则优先级越高, 0为最高优先级**, 其中也有优先级为负的部分, 是否可设置可参考p203是否有settable 标志。

| 分组方式 | 抢占优先级 | 响应优先级 |
| ---- | ---- | ---- |
| 分组0 | 0位，取值为0 | 4位，取值为0~15 |
| 分组1 | 1位，取值为0~1 | 3位，取值为0~7 |
| 分组2 | 2位，取值为0~3 | 2位，取值为0~3 |
| 分组3 | 3位，取值为0~7 | 1位，取值为0~1 |
| 分组4 | 4位，取值为0~15 | 0位，取值为0 |

其中分组分为0-4, 是可以在程序中进行自定义选择的。此时**在程序中选好优先级之后， 就需要注意抢占优先级和响应优先级的取值范围设定**， <mark style="background: transparent; color: red">不能超出上表设定的规定范围</mark>。 

### (3) EXTI 中断 
EXTI 通过检测对应GPIO口的电平信号值，<mark style="background: transparent; color: red">当电平变化时立即向NVIC发出中断申请</mark>, 其对应的 IO 口是可以通过程序软件进行指定的。 经过NVIC后实现对主程序的中断。

可以通过软件设置上升沿，下降沿和双边沿触发， 也可以软件触发(引脚上可以不变化即可代码触发中断)。 
同时<mark style="background: transparent; color: red">支持所有的GPIO口的中断</mark>, 但是相同的Pin 不能同时触发中断(**即PA0, PB0, PC0不能有任意两个同时存在**)

如果有多个中断引脚， 必须选用不同的Pin引脚作为中断。

外部GPIO 可占用通道包括:  多达16个GPIO_Pin,  <mark style="background: transparent; color: red">外加PVD 输出, RTC闹钟, USB 唤醒与以太网唤醒</mark>等等。
- 外加PVD输出部分用于电源从电压过低状态下, 可以从低功耗的停止模式下唤醒 STM32 。 
- 对于RTC闹钟 ， 为了省电时， 在RTC设定一个闹钟之后,  STM32会进入停止模式， 而等闹钟时间到之后再进行借助外部中断唤醒。而USB和以太网等作用原理相同。

外部中断的触发相应方式包括<b><mark style="background: transparent; color: blue">中断响应</mark></b>和<b><mark style="background: transparent; color: blue">事件响应</mark></b>, 即除了中断触发外, 也可以通过触发事件进行(需要注意: <mark style="background: transparent; color: red">如果触发事件， 则外部中断的信号不会流向CPU,  而是流向其他外设而用于触发其他外设的操作</mark>, 例如DAC和触发DMA等等) 

#### 1. GPIO结构
下图为 GPIO 的基本结构图, <b><mark style="background: transparent; color: blue">每个GPIO 有16个通道, 而由于EXTI 只有16个引脚</mark></b>， 所以中间使用了AFIO中断引脚选择， 选择三个GPIO中的一个连接到EXTI中, 即<b><mark style="background: transparent; color: blue">外部中断中， 相同的 Pin 不能同时触发中断</mark></b>, 但是所有的GPIO都能够触发中断
![[attachments/Pasted image 20240202212012.png|700]]
16个引脚和下面的PVD , RTC, USB, ETH 共有20路中断的输出, <b><mark style="background: transparent; color: red">其中EXTI9_5和EXTI15-10是连接在一起的</mark></b>, 即外部中断9-5和15-10触发的是同一个中断函数。<b><mark style="background: transparent; color: blue">此时需要通过软件在中断函数中判断标志位区分中断是哪一个</mark></b>; 另外还有20条流到其他外设。

#### 2.  AFIO 复用 IO口的外部中断线映射。 
AFIO 复用IO口的整体结构图如下: 通过EXTI\[3:0] 
![[attachments/Pasted image 20240202213430.png]]

为了使用AFIO 复用IO口中断映射, <mark style="background: transparent; color: red">首先应当开启GPIO对应的AFIO时钟</mark>(即用于选择和重定义)
```cpp
// 配置APB2总线上的外部中断进入开启状态
RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN, FunctionalState::ENABLE);
```

AFIO主要用于引脚的功能是<mark style="background: transparent; color: red">复用功能引脚重映射和中断引脚选择</mark> 
- 对于复用功能引脚重映射， 可以<b><mark style="background: transparent; color: blue">将默认复用功能的引脚切换到重定义功能</mark></b> (参考[[💻softwares/⚡Circuit/🎮STM32教程/一、基础使用部分/1. 基础以及STM32简介#(3) 引脚定义|STM32引脚定义]]), 可以由AFIO 完成。 
- 中断引脚选择, 可以通过EXTI的位选用对应输入中断引脚

#### 4. EXTI 框图
![[Excalidraw/一、EXTI外部中断的使用 2024-02-02 22.48.19|700]]
选择寄存器选择上升或者下降沿触发, 和软件中断寄存器进行或逻辑共同控制中断， 
在输入或门之后通过mask 寄存器进行事件的触发, 之后再进行一次Mask 

> [!summary] 外部中断配置步骤总结 
> 1. 设置20条线的中断屏蔽寄存器(Mask bits) EXTI_IMR   
> 2. 设置中断线的触发器选择位（EXIT_RTSR和 EXTI_FTSR)
> 3. 配置NVIC的使能和覆盖位, 从而使得线上的中断可以被正确识别。


中断函数需求使用: 对于<mark style="background: transparent; color: red">想要获取的驱动信号是外部驱动的快速突发信号
</mark>, 且往往是突发事件,  并对准确接受波形有较高的要求，此时即可使用stm32的外部中断。 

例如旋转编码器， 红外接受等等都推荐使用中断进行获取。 
但是按键不推荐（需要进行按键检测 ）: 如果有长期不用不影响CPU时, <mark style="background: transparent; color: red">可以考虑定时器中断读取的方式</mark>，可以做到后台读取不阻塞主程序， 也能很好地处理按键抖动和松手检测的问题。

对应的寄存器部分可以参考  编程手册 > 内核外设> NVIC , 另外中断分组配置寄存器在编程手册 > SCB 部分
SCB_AIRCR 中, 配置中断分组
另外寄存器也可以参考对应的编程手册等。
# 三、 旋转编码器
旋转编码器：用来测量位置、速度或旋转方向的装置，当其旋转轴旋转时，其<b><mark style="background: transparent; color: blue">输出端可以输出与旋转速度和方向对应的方波信号</mark></b>，读取**方波信号的频率和相位信息**即可得知旋转轴的速度和方向

包括==机械触点式， 霍尔传感器式和光栅式传感器==等等。 

==光栅式旋转编码器==是通过对射式红外传感配合光栅编码盘测速， 需要注意的是, 红外+ 光栅编码盘的办法只能测量位置和速度但不能测量旋转方向

而下图的旋转编码器是==机械触点式==编码器。
其中外侧的部分, 左边触点接在A引脚, 右侧连接在B引脚。
![[Excalidraw/一、EXTI外部中断的使用 2024-02-02 23.16.01]]
由于金属盘的位置经过设计， <mark style="background: transparent; color: red">两侧触点的通断会产生一个90度的相位差</mark>, 从而通过AB的先后判断对应的旋转方向。 需要说明的是, 也有的旋转编码器通过一个引脚输出转速, 另一个引脚通过输出高低电平代表旋转方向。 

霍尔传感器式是通过两个错开的霍尔传感器输出正交方波信号
此外还有独立编码元件等

其中机械式不适用于电机测速功能。 

旋转编码器的内部原理图如下: 上面是可按下的按键。需要说明的是， 按钮是悬空的,  即不接入电路中。 
![[attachments/Pasted image 20240202233431.png|500]]
通过上拉电阻, 在没有接触情况下， 输出的是高电平。 

# 四、软件编程
### (1) 中断函数初始化配置部分
硬件电路: 旋转编码器的A. B 分别接入B4, B5 两个口, 而对射式红外传感器的DO接入B6口

实现要求, 通过旋转编码器和对射式红外共同控制显示屏数字,  旋转编码器顺时针增加，逆时针减小,  而对射红外计数一次增加一次。 
![[attachments/Pasted image 20240202212012.png|700]]

> [!NOTE] 配置外部中断方法
> 1.  配置RCC设置GPIOB 和AFIO的Clock 为开启状态
> 2. 配置GPIO, 设置GPIO模式
> 3. 配置AFIO, 将中断口设置为B口
> 4. 配置EXTI, 选择触发条件和触发方式
> 5. 配置NVIC, 给外部中断设定合适的优先级

其中, EXTI不需要开启时钟, 而 NVIC 是内核外设, 时钟一指都是开启的。 
对于外设采用中断时的各个引脚配置, 可以看参考手册 -> GPIO 和 AFIO 部分(p165)
![[attachments/Pasted image 20240203015224.png]]
可以查到， 显示都可以, 因此可以选择pull-up作为输入电平。 

1. 补充以下的 GPIO 函数讲解: 
```cpp 
void GPIO_AFIODeInit(void); // AFIO 复位函数 
void GPIO_PinLockConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);  // 锁定GPIO配置, 放止意外更改 
// 下面两个函数用于配置AFIO的事件输出功能, 使用不多
void GPIO_EventOutputConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource); 
void GPIO_EventOutputCmd(FunctionalState NewState);

// ***** 重要函数 ********
void GPIO_PinRemapConfig(uint32_t GPIO_Remap, FunctionalState NewState);  // GPIO的重映射函数, 其中第一个选择重映射的方式, 第二个参数是新的状态 ->  用于映射引脚的外设 
void GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource);  // ***用于配置外部中断， 可以配置AFIO 的选择器,  选择需要的中断引脚****
void GPIO_ETH_MediaInterfaceConfig(uint32_t GPIO_ETH_MediaInterface); // 与以太网有关的外设配置。
```

3. 需要说明: `GPIO_ EXTILineConfig` 实际上是操作AFIO的寄存器, 实际山是AFIO的函数操作。 用于选择GPIOPinSource作为中断线。其中GPIO_PortSource设置为GPIOB即可(由于是在B口进行操作)
```cpp
void GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource); 
```

其中GPIO_PinSource是<mark style="background: transparent; color: red">指定对应的外部中断线</mark>, 代表GPIOx 的第 GPIO_PinSource 中断线路， 
此时将 GPIO_PinSource 改为中断引脚即可。例如此处对射红外传感改为6。
```cpp
GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6); // 配置PB6口为中断口
```

4. 然后是配置 EXTI 电路, 相关的函数在 stm32f10x_exti.h 中可以找到 , 下面是一些中断线的定义。在没有配置优先级时， 默认是按照端口 0 > 1 > 2 > .... 来设定优先级的。
```cpp
#define EXTI_Line0       ((uint32_t)0x00001)  /*!< External interrupt line 0 */
#define EXTI_Line1       ((uint32_t)0x00002)  /*!< External interrupt line 1 */
#define EXTI_Line2       ((uint32_t)0x00004)  /*!< External interrupt line 2 */
#define EXTI_Line3       ((uint32_t)0x00008)  /*!< External interrupt line 3 */
#define EXTI_Line4       ((uint32_t)0x00010)  /*!< External interrupt line 4 */
#define EXTI_Line5       ((uint32_t)0x00020)  /*!< External interrupt line 5 */
#define EXTI_Line6       ((uint32_t)0x00040)  /*!< External interrupt line 6 */
#define EXTI_Line7       ((uint32_t)0x00080)  /*!< External interrupt line 7 */
#define EXTI_Line8       ((uint32_t)0x00100)  /*!< External interrupt line 8 */
#define EXTI_Line9       ((uint32_t)0x00200)  /*!< External interrupt line 9 */
#define EXTI_Line10      ((uint32_t)0x00400)  /*!< External interrupt line 10 */
#define EXTI_Line11      ((uint32_t)0x00800)  /*!< External interrupt line 11 */
#define EXTI_Line12      ((uint32_t)0x01000)  /*!< External interrupt line 12 */
#define EXTI_Line13      ((uint32_t)0x02000)  /*!< External interrupt line 13 */
#define EXTI_Line14      ((uint32_t)0x04000)  /*!< External interrupt line 14 */
#define EXTI_Line15      ((uint32_t)0x08000)  /*!< External interrupt line 15 */
#define EXTI_Line16      ((uint32_t)0x10000)  /*!< External interrupt line 16 Connected to the PVD Output */
#define EXTI_Line17      ((uint32_t)0x20000)  /*!< External interrupt line 17 Connected to the RTC Alarm event */
#define EXTI_Line18      ((uint32_t)0x40000)  /*!< External interrupt line 18 Connected to the USB Device/USB OTG FS
```

EXTI 的库函数包括: 
```cpp
// 模板函数部分(包括清除, 初始化和赋默认值)
void EXTI_DeInit(void);    // 清除EXTI 数据, 恢复为上电默认状态的 EXTI 设置
void EXTI_Init(EXTI_InitTypeDef* EXTI_InitStruct);            // 根据结构体参数配置EXTI外设 
void EXTI_StructInit(EXTI_InitTypeDef* EXTI_InitStruct);  // 将参数的结构体变量赋一个默认值
// 中断， 定时器, ADC和串口等等都是使用结构体初始化的

void EXTI_GenerateSWInterrupt(uint32_t EXTI_Line);       // 用于软件触发外部中断 > 程序中需要 

// 模板函数 > 用于获取标志位
// 首先在外设运行过程中会产生一些状态标志位, 当外部中断来时， 由挂起寄存器置一个标志位, 而对于EXTI9_5和EXTI15-10是连接在一起的 , 可以利用中断标志位进行判断。
// 串口和ADC, 定时器等都会置标志位, 而标志位都是放在状态寄存器的, 
FlagStatus EXTI_GetFlagStatus(uint32_t EXTI_Line);        // 用于获取指定标志位是否被置1, 用于主程序中查看标志位
void EXTI_ClearFlag(uint32_t EXTI_Line);                           // 对置1的标志位进行清除， 用于主程序中清除标志位
ITStatus EXTI_GetITStatus(uint32_t EXTI_Line);                // 获取中断标志位是否被置1 > 用于在中断中查看标志位
void EXTI_ClearITPendingBit(uint32_t EXTI_Line);             // 清除中断挂起标志位  > 用于在中断中清除标志位
// 下面的两个函数仅读写与中断有关的标志位, 而上面两个不论能否触发均可以读取
```


```cpp
EXTI_InitTypeDef* EXTI_InitStruct = new EXTI_InitTypeDef();
EXTI_InitStruct->EXTI_Mode = EXTIMode_TypeDef::EXTI_Mode_Interrupt;  // use interruption , 设置需要的模式
EXTI_InitStruct->EXTI_Line = EXTI_Line6;  //  设置
EXTI_InitStruct->EXTI_LineCmd = FunctionalState::ENABLE; // enable the 
EXTI_InitStruct->EXTI_Trigger = EXTITrigger_TypeDef::EXTI_Trigger_Falling;  // use falling edge trigger 
```

5. 配置EXTI 完毕之后对NVIC进行配置: 
NVIC由于是内核函数, 主要部分在misc.h文件中。 

下面是NVIC 的设置函数: 
```cpp
void NVIC_PriorityGroupConfig(uint32_t NVIC_PriorityGroup); // 对于`NVIC_PriorityGroupConfig`, 用来中断分组, 传入指定的 
void NVIC_Init(NVIC_InitTypeDef* NVIC_InitStruct);  // 初始化NVIC 
// 下面三个用的不多
void NVIC_SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset); // 设置中断向量表
void NVIC_SystemLPConfig(uint8_t LowPowerMode, FunctionalState NewState);
// 这个是Systick 配置函数 
void SysTick_CLKSourceConfig(uint32_t SysTick_CLKSource);
```
用于设置优先级, 可以用来进行中断分组等操作。 其中NVIC_InitTypeDef有如下四个参数 : 
```cpp
	NVIC_InitTypeDef* NVIC_InitStruc = new NVIC_InitTypeDef();
	NVIC_InitStruc->NVIC_IRQChannel = IRQn_Type::EXTI9_5_IRQn; // use channel 6 -> in channel 5-9
	NVIC_InitStruc->NVIC_IRQChannelCmd = FunctionalState::ENABLE;
	NVIC_InitStruc->NVIC_IRQChannelPreemptionPriority = ;
	NVIC_InitStruc->NVIC_IRQChannelSubPriority = ; 
```
对于不同的 NVIC_IQRChannel, 有如下的设置: 
```cpp
  ADC1_2_IRQn                 = 18,     /*!< ADC1 and ADC2 global Interrupt                       */
  USB_HP_CAN1_TX_IRQn         = 19,     /*!< USB Device High Priority or CAN1 TX Interrupts       */
  USB_LP_CAN1_RX0_IRQn        = 20,     /*!< USB Device Low Priority or CAN1 RX0 Interrupts       */
  CAN1_RX1_IRQn               = 21,     /*!< CAN1 RX1 Interrupt                                   */
  CAN1_SCE_IRQn               = 22,     /*!< CAN1 SCE Interrupt                                   */
  EXTI9_5_IRQn                = 23,     /*!< External Line[9:5] Interrupts                        */
  TIM1_BRK_IRQn               = 24,     /*!< TIM1 Break Interrupt                                 */
  TIM1_UP_IRQn                = 25,     /*!< TIM1 Update Interrupt                                */
  TIM1_TRG_COM_IRQn           = 26,     /*!< TIM1 Trigger and Commutation Interrupt               */
  TIM1_CC_IRQn                = 27,     /*!< TIM1 Capture Compare Interrupt                       */
  TIM2_IRQn                   = 28,     /*!< TIM2 global Interrupt                                */
  TIM3_IRQn                   = 29,     /*!< TIM3 global Interrupt                                */
  TIM4_IRQn                   = 30,     /*!< TIM4 global Interrupt                                */
  I2C1_EV_IRQn                = 31,     /*!< I2C1 Event Interrupt                                 */
  I2C1_ER_IRQn                = 32,     /*!< I2C1 Error Interrupt                                 */
  I2C2_EV_IRQn                = 33,     /*!< I2C2 Event Interrupt                                 */
  I2C2_ER_IRQn                = 34,     /*!< I2C2 Error Interrupt                                 */
  SPI1_IRQn                   = 35,     /*!< SPI1 global Interrupt                                */
  SPI2_IRQn                   = 36,     /*!< SPI2 global Interrupt                                */
  USART1_IRQn                 = 37,     /*!< USART1 global Interrupt                              */
  USART2_IRQn                 = 38,     /*!< USART2 global Interrupt                              */
  USART3_IRQn                 = 39,     /*!< USART3 global Interrupt                              */
  EXTI15_10_IRQn              = 40,     /*!< External Line[15:10] Interrupts                      */
  RTCAlarm_IRQn               = 41,     /*!< RTC Alarm through EXTI Line Interrupt                */
  USBWakeUp_IRQn              = 42      /*!< USB Device WakeUp from suspend through EXTI Line Interrupt */
```
其中重要的是`EXTI9_5_IRQn`  和 `EXTI15_10_IRQn` 
```
NVIC_IRQChannelPreemptionPriority
```

NVIC_IRQChannelPre-emptionPriority<mark style="background: transparent; color: red">用来设置抢占优先级</mark>, 而NVIC_IRQChannelSubPriority <mark style="background: transparent; color: red">用于设置响应优先级 </mark>, 需要说明的是, 设置优先级时需要参考下表确定优先级设置范围;  对于2的部分, 可以均设为0-3。

 |分组方式|抢占优先级|响应优先级|
|---|---|---|
|分组0|0位，取值为0|4位，取值为0~15|
|分组1|1位，取值为0~1|3位，取值为0~7|
|分组2|2位，取值为0~3|2位，取值为0~3|
|分组3|3位，取值为0~7|1位，取值为0~1|
|分组4|4位，取值为0~15|0位，取值为0|

中断程序的整个示例程序如下: (启用B6号口的中断配置)
```cpp
void CounterInit(){
	// firstly enable the GPIOB clock , and make configuration for GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, FunctionalState::ENABLE);
	GPIO_InitTypeDef* GPIO_InitStruct = new GPIO_InitTypeDef();
	GPIO_InitStruct->GPIO_Mode = GPIOMode_TypeDef::GPIO_Mode_IPU;
	GPIO_InitStruct->GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6; // make these as input pin
	GPIO_InitStruct->GPIO_Speed = GPIOSpeed_TypeDef::GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, GPIO_InitStruct);  // use struct for init;
	GPIO_SetBits(GPIOB, GPIO_Pin_6);

	// Enable AFIO RCC clock
	GPIO_AFIODeInit();  // reset the AFIO Pherphical 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, FunctionalState::ENABLE);
	// Note that for EXTI and NVIC (Core Periphical), the clock of them are open all time 
	
	// use B6 as interrupt for B6 as interruption for Infrared Sensor 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);
	
	// Initialize the EXTI Peripheral by Struct 
	EXTI_InitTypeDef* EXTI_InitStruct = new EXTI_InitTypeDef();
	EXTI_InitStruct->EXTI_Mode = EXTIMode_TypeDef::EXTI_Mode_Interrupt;  // use interruption 
	EXTI_InitStruct->EXTI_Line = EXTI_Line6; 
	EXTI_InitStruct->EXTI_LineCmd = FunctionalState::ENABLE; // enable the 
	EXTI_InitStruct->EXTI_Trigger = EXTITrigger_TypeDef::EXTI_Trigger_Falling;  // use falling edge trigger 
	EXTI_Init(EXTI_InitStruct); 

	// Configuration for NVIC 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // configure the group of the interruption
	NVIC_InitTypeDef* NVIC_InitStruc = new NVIC_InitTypeDef();
	NVIC_InitStruc->NVIC_IRQChannel = IRQn_Type::EXTI9_5_IRQn; // use channel 6 -> in channel 5-9
	NVIC_InitStruc->NVIC_IRQChannelCmd = FunctionalState::ENABLE;
	NVIC_InitStruc->NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruc->NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(NVIC_InitStruc);
}
```


### (2) 中断函数的写法
在stm32中， 中断函数具有固定的名字, 可以从startup_stm3210x_md.s中找到。
在Vector Table 中的就是中断向量表。 
![[attachments/Pasted image 20240203144428.png]]
下图为提供的外部中断函数句柄， 此处我们**只需要用到 EXTI9_5_IRQHandler 即可**,  只需要使用固定的名字， 不需要使用 interrupt  
![[attachments/Pasted image 20240203144559.png|800]]

而在中断函数中判断标志位, 可以在 stm32f10x_exti.h 中找到。 首先获取中断标志位的函数是
```cpp 
ITStatus EXTI_GetITStatus(uint32_t EXTI_Line);  // 参数是EXTI_Line 的线号
```
需要注意, 这个返回值是ITStatus类型， 所以查看是否为ITStatus:: SET或者RESET 
<b><mark style="background: transparent; color: blue">在中断函数中需要执行清除中断位的操作， 只要中断标志位置一, 则会导致一直申请中断</mark></b> 

```cpp
// Interruption function 
void EXTI9_5_IRQHandler(void){ 
	// judge the signal location 
	if (EXTI_GetITStatus(EXTI_Line6) == ITStatus::SET){
		// check if EXTI_Line6 is 1, triggered interruption 6
		rotate_num++;
		EXTI_ClearITPendingBit(EXTI_Line6);  // clear the interruption bit
	}
}

```

其中对于对射式红外传感器， 如果中间不放物品， 则输出为0，否则输出为1。

下面是c语言代码(不支持c++)
主函数
```cpp
#include "stm32f10x.h"
#include "OLED.h"
#include "CounterS.h"

int rotate_num = 0;
int test_num = 0;

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

/// @brief Display Number function 
/// @param num Number to Display on OLED
void Display_Num(int num){
	int length = get_length(num);
	if (num >=0) OLED_ShowNum(1,1,(uint32_t)num, length);
	else {
		OLED_ShowChar(1,1, '-');
		OLED_ShowNum(1,2, -(uint32_t)num, length);
	}
}

int main(){
	OLED_Init();
	CounterInit(); 
	while (1){
		Display_Num(GetCounterNum());
	}
}

```

```cpp
#ifndef Counter_H
#define Counter_H

#include "stm32f10x.h"

void CounterInit(void);
int GetCounterNum(void);

#endif
```

```cpp
#include "CounterS.h"

int Counter_Number = 0;

// Init The Counter Function 
void CounterInit(void){
	// firstly enable the GPIOB clock , and make configuration for GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // not use deinit 

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6; // make these as input pin
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);  // use struct for init;

	// Note that for EXTI and NVIC (Core Periphical), the clock of them are open all time 
	// use B6 as interrupt for B6 as interruption for Infrared Sensor
	 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);
	
	// Initialize the EXTI Peripheral by Struct 
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line6;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE; // enable the 
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;  // use interruption 
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;  // use falling edge trigger 
	EXTI_Init(&EXTI_InitStruct);

	// Configuration for NVIC 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // configure the group of the interruption
	
	NVIC_InitTypeDef NVIC_InitStruc;
	NVIC_InitStruc.NVIC_IRQChannel = EXTI9_5_IRQn; // use channel 6 -> in channel 5-9
	NVIC_InitStruc.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruc.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruc.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruc);
}

// Interruption function
void EXTI9_5_IRQHandler(void){
	// judge the signal location
	if (EXTI_GetITStatus(EXTI_Line6) == SET){
		// check again
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)==0){
			// check if EXTI_Line6 is 1, triggered interruption 6
			Counter_Number++ ;
		}
		EXTI_ClearITPendingBit(EXTI_Line6);  // clear the interruption bit
	}
}

int GetCounterNum(void){
    return Counter_Number;
}

```

### (3) 关于中断对C++的支持与extern C的用法与中断调试
> [!caution] 中断函数的使用注意要点
> 如果在使用中断时， 采用--cpp11编译环境， 则会导致程序完全跳进中断之后出不来，
> 解决方法1是全盘采用--cpp99作为编译， 但是这样会直接导致无法使用C++语法

下面着重讲解使用extern "C"让编译中断兼容C++的做法

关于STM32 在使用 C++ 进行编程时无法进行中断(触发中断就死循环)的原因分析: 
- 其实这个问题说透了也很简单,就是因为 C++ 和 C 语言编译结果不同导致的 因为C++ 是允许同名函数的,也就是通过不同的参数进行重载, <mark style="background: transparent; color: red">这就会导致函数名称和编译后的不相同</mark> 比如EXTI4_IRQHandler()这个函数,放在 C++ 里面编译后指不定就变成了EXTI4_IRQHandler_void 这跟.s 文件里面预先声明的函数不能说一点不相干,只能说完全不一样 加上extern "C"以后,编译器就会按照 C 语言的标准编译,就可以正确的被中断调用到了

解决方法是用extern "C" {}把中断函数包起来, <mark style="background: transparent; color: red">仅对于CounterSensor.c中的中断函数</mark>， <b><mark style="background: transparent; color: blue"></mark>强制使用C语言进行编译</b>, 这时就可以保证在编译完毕之后中断不会改变了 

这样**只需要在中断函数中使用C语言格式就行了**。 

```cpp
// 就像这样  
extern"C"{  
	voidEXTI4_IRQHandler(){  
		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);  
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_4);  
	} 
}
```

另外， 如果执行条件编译， 也可以这样使用extern "C", 参考 https://blog.csdn.net/weixin_40593838/article/details/122474117 

```cpp
#ifdef __cplusplus
extern "C"{
#endif

中间放部分代码
#ifdef __cplusplus
}
#endif
```



下面是使用extern "C"的完整代码
main.cpp 
```cpp
#include "stm32f10x.h"
#include "OLED.h"
#include "CounterS.h"

using namespace std;

int rotate_num = 0;
int test_num = 0;

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

/// @brief Display Number function 
/// @param num Number to Display on OLED
void Display_Num(int num){
	int length = get_length(num);
	if (num %2 == 0){
		GPIO_SetBits(GPIOA, GPIO_Pin_3);
	}
	else {
		GPIO_ResetBits(GPIOA, GPIO_Pin_3);
	}
	OLED_ShowNum(1,1,(uint32_t)num, length);
}

int main(){
	OLED_Init();
	CounterInit();
	// use port A-3 for LED Debug
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, FunctionalState::ENABLE);
	// Init GPIO Configuration
	GPIO_InitTypeDef *GPIO_InitStruct = new GPIO_InitTypeDef(); // define the GPIO Struct
	GPIO_InitStruct->GPIO_Mode = GPIOMode_TypeDef::GPIO_Mode_Out_PP;
	GPIO_InitStruct->GPIO_Pin =  GPIO_Pin_3;  // can initialize multiple 
	GPIO_InitStruct->GPIO_Speed = GPIOSpeed_TypeDef::GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, GPIO_InitStruct);       // **important -> call first**
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
	
	while (1){
		Display_Num(GetCounterNum());
	}
}

```

CounterS.h
```cpp
#ifndef Counter_H
#define Counter_H

#include "stm32f10x.h"

void CounterInit(void);
int GetCounterNum(void);

#endif

```

CouunterS.cpp
```cpp
#include "CounterS.h"
using namespace std;

int Counter_Number = 0;

// Init The Counter Function 
void CounterInit(void){
	// firstly enable the GPIOB clock , and make configuration for GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, FunctionalState::ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, FunctionalState::ENABLE); // not use deinit 

	GPIO_InitTypeDef* GPIO_InitStruct = new GPIO_InitTypeDef();
	GPIO_InitStruct->GPIO_Mode = GPIOMode_TypeDef::GPIO_Mode_IPU;
	GPIO_InitStruct->GPIO_Pin = GPIO_Pin_6; // make these as input pin
	GPIO_InitStruct->GPIO_Speed = GPIOSpeed_TypeDef::GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, GPIO_InitStruct);  // use struct for init;

	// Note that for EXTI and NVIC (Core Periphical), the clock of them are open all time 
	// use B6 as interrupt for B6 as interruption for Infrared Sensor
	 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);
	
	// Initialize the EXTI Peripheral by Struct 
	EXTI_InitTypeDef *EXTI_InitStruct = new EXTI_InitTypeDef();
	EXTI_InitStruct->EXTI_Line = EXTI_Line6;
	EXTI_InitStruct->EXTI_LineCmd = FunctionalState::ENABLE; // enable the 
	EXTI_InitStruct->EXTI_Mode = EXTIMode_TypeDef::EXTI_Mode_Interrupt;  // use interruption 
	EXTI_InitStruct->EXTI_Trigger = EXTITrigger_TypeDef::EXTI_Trigger_Falling;  // use falling edge trigger 
	EXTI_Init(EXTI_InitStruct);

	// Configuration for NVIC 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // configure the group of the interruption
	
	NVIC_InitTypeDef *NVIC_InitStruc = new NVIC_InitTypeDef();
	NVIC_InitStruc->NVIC_IRQChannel = EXTI9_5_IRQn; // use channel 6 -> in channel 5-9
	NVIC_InitStruc->NVIC_IRQChannelCmd = FunctionalState::ENABLE;
	NVIC_InitStruc->NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruc->NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(NVIC_InitStruc);
}

extern "C"{
    // Interruption function must be  written in C
    void EXTI9_5_IRQHandler(void){
        // judge the signal location
        if (EXTI_GetITStatus(EXTI_Line6) == SET){
            // check again
            if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)==0){
                // check if EXTI_Line6 is 1, triggered interruption 6
                Counter_Number++ ;
            }
            EXTI_ClearITPendingBit(EXTI_Line6);  // clear the interruption bit
        }
    }
}

int GetCounterNum(void){
    return Counter_Number;
}

```

此时， 只要在中断函数中加断点，然后使用F5做Debug (先编译), 之后只要挡光， 就会跳到中断函数中来。断点被触发。 
![[attachments/Pasted image 20240204084444.png]]

# 五、 旋转编码器计次编程
首先， 由于旋转编码器只要错过一点，就会错过很多波形， 此时我们为了将其和对射红外兼容，<mark style="background: transparent; color: red"> 可以给旋转编码设置更高的抢占优先级</mark>。

其中PB0接入A部分， 而PB1接入旋转编码器的C脚。

首先， 对于旋转编码器而言， 需要初始化两个中断, 即AB相都触发中断。
其中<mark style="background: transparent; color: red">初始化两个中断的方法是</mark>用一个下降沿作为中断, 同时读取另一个的电平。 

判断正转和反转的方法: 
![[Excalidraw/一、EXTI外部中断的使用 2024-02-04 09.26.38|700]]

不能进行合并的部分: 
```cpp
GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1); // not we shoudn't combine here
```
为了方便配置优先级， 尽量不用5-9的部分


在实际编程时需要注意: 由于A的下降沿会存储并重新请求中断， 因此需要在前后加上延时函数进行延时。 但是， <mark style="background: transparent; color: red">实际项目中应当尽可能不使用delay</mark>. 

```cpp
#include "stm32f10x.h"
#include "OLED.h"
#include "CounterS.h"
#include "Delay.h"

using namespace std;
int rotate_num = 0;

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

// Use PB5 as A and PB7 as C for interruption
void Encoder_Init(void){
		// firstly enable the GPIOB clock , and make configuration for GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, FunctionalState::ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, FunctionalState::ENABLE); // not use deinit 

	GPIO_InitTypeDef* GPIO_InitStruct = new GPIO_InitTypeDef();
	GPIO_InitStruct->GPIO_Mode = GPIOMode_TypeDef::GPIO_Mode_IPU;
	GPIO_InitStruct->GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; // make these as input pin
	GPIO_InitStruct->GPIO_Speed = GPIOSpeed_TypeDef::GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, GPIO_InitStruct);  // use struct for init;

	// Note that for EXTI and NVIC (Core Periphical), the clock of them are open all time 
	// use B6 as interrupt for B6 as interruption for Infrared Sensor
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1); // not we shoudn't combine here
	
	// Initialize the EXTI Peripheral by Struct 
	EXTI_InitTypeDef *EXTI_InitStruct = new EXTI_InitTypeDef();
	EXTI_InitStruct->EXTI_Line = EXTI_Line0|EXTI_Line1;
	EXTI_InitStruct->EXTI_LineCmd = FunctionalState::ENABLE; // enable the 
	EXTI_InitStruct->EXTI_Mode = EXTIMode_TypeDef::EXTI_Mode_Interrupt;  // use interruption 
	EXTI_InitStruct->EXTI_Trigger = EXTITrigger_TypeDef::EXTI_Trigger_Falling;  // use falling edge trigger 
	EXTI_Init(EXTI_InitStruct);

	// Configuration for NVIC 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // configure the group of the interruption
	
	NVIC_InitTypeDef *NVIC_InitStruc = new NVIC_InitTypeDef();
	NVIC_InitStruc->NVIC_IRQChannel = EXTI0_IRQn; // use channel 6 -> in channel 5-9
	NVIC_InitStruc->NVIC_IRQChannelCmd = FunctionalState::ENABLE;
	NVIC_InitStruc->NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruc->NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(NVIC_InitStruc);
	NVIC_InitStruc->NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStruc->NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruc->NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(NVIC_InitStruc);
}

extern "C"{
	void EXTI0_IRQHandler(void){
		Delay_ms(10);
		if (EXTI_GetITStatus(EXTI_Line0) == SET){
			// give a short delay
			if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0){
				rotate_num--;
				Delay_ms(10); 
			}
			EXTI_ClearITPendingBit(EXTI_Line0);
		}
	}

	void EXTI1_IRQHandler(void){
		Delay_ms(10);
		if (EXTI_GetITStatus(EXTI_Line1) == SET){
			if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0) == 0){
				rotate_num++;
				Delay_ms(10);
			}
			EXTI_ClearITPendingBit(EXTI_Line1);
		}
	}
}

/// @brief Display Number function 
/// @param num Number to Display on OLED
void Display_Num(int num, int line){
	int length = get_length(num);
	if (rotate_num %2 == 0){
		GPIO_SetBits(GPIOA, GPIO_Pin_3);
	}
	else {
		GPIO_ResetBits(GPIOA, GPIO_Pin_3);
	}
	if (num >= 0){
		OLED_ShowNum(line,1,num, length);
		for (int i = length+1; i <= 5; i++){
			OLED_ShowChar(line, i, ' ');
		}
	}else{
		OLED_ShowChar(line,1,'-');
		OLED_ShowNum(line, 2,(-num), length);
		for (int i = length+2; i <= 5; i++){
			OLED_ShowChar(line, i, ' ');
		}
	}
}

int main(){
	OLED_Init();
	CounterInit();
	Encoder_Init();
	// use port A-3 for LED Debug
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, FunctionalState::ENABLE);
	// Init GPIO Configuration
	GPIO_InitTypeDef *GPIO_InitStruct = new GPIO_InitTypeDef(); // define the GPIO Struct
	GPIO_InitStruct->GPIO_Mode = GPIOMode_TypeDef::GPIO_Mode_Out_PP;
	GPIO_InitStruct->GPIO_Pin =  GPIO_Pin_3;  // can initialize multiple 
	GPIO_InitStruct->GPIO_Speed = GPIOSpeed_TypeDef::GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, GPIO_InitStruct);       // **important -> call first**
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
	
	while (1){
		Display_Num(GetCounterNum(),1);
		Display_Num(rotate_num, 2);
	}
}

```

另外实际上<mark style="background: transparent; color: red">可以按照定时器中断的方式进行初始化旋转编码器</mark>。 此时即可
https://blog.csdn.net/weixin_45935114/article/details/132323761

说明: 尽量中断和主程序里不要处理相同的事件。
