# 配置方法
在Proteus 中， 不添加任何外加电路, 直接将Program File改为对应的.hex或者.lsf文件, 然后<mark style="background: transparent; color: red">将晶振频率更改为72MHz</mark>, 即配置完成, 可以实现编译。

仿真选用Target中的 .hex 或者 .elf 文件均可。
![[attachments/Pasted image 20240206134609.png]]

![[attachments/Pasted image 20240206135023.png]]

首先, 对于GPIO_Set和Reset 部分, 可以直接获取对应的结果, 但是对于按键的识别， 往往在开发板能够在LED端口配置成功， 但是如图的方法进行仿真却不能使用按钮(即按钮失效)

解决的方法是不接入上拉电阻， 直接接入按钮: 这样就可以实现按钮读取结果。
![[attachments/Pasted image 20240206152139.png]]

# CubeMX 基础使用

对于STM32, 一般拥有7个端口即GPIOA`->` GPIOG, 

首先当具有通信时, 更改System Core, 
![[attachments/Pasted image 20240206114703.png|900]]
配置RCC部分:
需要说明， 如果是使用外部晶振, 则配置为如下端口, 而不使用外部晶振时, 可以
![[attachments/Pasted image 20240206120200.png]]

最后将 HCLK 设置为 72MHz 
![[attachments/Pasted image 20240206122148.png]]

常见的库函数
```cpp
HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState); // GPIO_PIN_SET
HAL_GPIO_TooglePin(GPIOx, GPIO_Pin); 

```


GPIO的设置方法如图: 其中可以通过PA5 Configuration 可以配置上电之后的
![[attachments/Pasted image 20240206125940.png|1000]]

其中GPIO_output level 是设置刚上电时的GPIO输出(默认为低)
配置完毕之后就可以选择Project > ToolChain IDE
方法一是改为 MDK ARM V5, 此时会生成对应的 Keil MDK 文件， 选用创建.c, .h文件
![[attachments/Pasted image 20240206131714.png]]
选Generate Code后Open Project即可
![[attachments/Pasted image 20240206131213.png]]



读取按键并Delay的函数如下: 
```cpp
  while (1)
  {
    /* USER CODE END WHILE */
		HAL_GPIO_WritePin (GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    /* USER CODE BEGIN 3 */
  }
```
