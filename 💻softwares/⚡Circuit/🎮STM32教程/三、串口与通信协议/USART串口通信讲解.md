# 一、STM32 通信接口简介
STM32的通信模块提供了以下的通信接口: 包括USART, I2C, SPI, CAN, USB等等; 且C8T6支持以下的全部通信接口。
STM32通过通信极大地扩展了硬件系统; 

| 名称(数据线数量) | 引脚 | 双工 | 时钟 | 电平 | 设备 |
| ---- | ---- | ---- | ---- | ---- | ---- |
| USART(2) | TX、RX | 全双工 | 异步 | 单端 | 点对点 |
| I2C (1) | SCL、SDA | 半双工 | 同步 | 单端 | 多设备 |
| SPI(2) | SCLK、**MOSI、MISO**、CS | 全双工 | 同步 | 单端 | 多设备 |
| CAN(1) | CAN_H、CAN_L (使用两个引脚表示差分数据) | 半双工 | 异步 | 差分 | 多设备 |
| USB(1) | DP、DM  (差分数据脚) | 半双工 | 异步 | 差分 | 点对点 |

STM32有3个USART接口, 其中USART1的引脚为: 
PA8(USART1_CK), PA9(USART1_TX) , PA10(USART1_RX), 以及USART1_CTS, USART1_RTS, 
接线方法需要接入TX(结CH340G_RX), RX(接入CH34G_TX)以及GND。(TX(transmit exchange), RX) 
仍然使用CH340G 芯片, 可以使用串口调试助手来进行接收和发送数据。 

MOSI (Master Output Slave Input)主机输出数据脚  MISO 主机输入数据脚

CAN 和USB都是半双工通信； 时钟的特性分为同步和异步； 其中I2C和SPI均有单独的时钟线， 是同步接收的; 而USB, CAN, 串口都需要双方指定一个频率进行接收(即异步通信-> 同时为了采样位置对齐需要加上帧头和帧尾)