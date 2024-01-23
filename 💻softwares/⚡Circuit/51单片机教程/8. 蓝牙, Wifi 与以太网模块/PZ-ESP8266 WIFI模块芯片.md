- 常见的类似扩展模块有PZ-HC05蓝牙模块, PZ-CH1921以太网转串口模块等等, 

由于WiFI模块芯片是使用3.3V的部分作为电源的， 在开发板上使用AMS 1117-3.3 V进行转换, 并接到P3接口上, 供3.3V引脚使用。
![[attachments/Pasted image 20240122122153.png]]

下面的是ESP8266及其引脚图。 
![[attachments/Pasted image 20240122123624.png|350]] 
其中RST为复位引脚,  由于是高电平有效所以可以不接。 

CH_PD为使能管脚(高电平有效)

# 电路与连接
实际连接中我们可以借用NRF24L01中的D3.3V + 杜邦线进行连接(扩展一个3.3V电源的IO口)
其余按照数据手册连接即可
![[attachments/Pasted image 20240122144636.png]]
在实际中, 使用NRF24L0扩展一个3.3V口, 连线如下: 
![[attachments/IMG_20240122_150630.jpg|300]]

由于刚上电之后， 仅是串口中会发送一些乱码数据， 因此将Wifi模块进行升级, 利用自带的固件烧录工具进行。 得到的AP-MAC和STA-MAC如下图: (需要重启之后，进行烧录)
![[attachments/Pasted image 20240122144559.png]]
烧写成功 
![[attachments/Pasted image 20240122144944.png]]

## 一、串口调试与波特率修改
在Wifi芯片中, 厂家给的Wifi模块的波特率是115200, 通过115200波特率发送
![[attachments/Pasted image 20240122150133.png]]
下图中显示了调试波特率成功的部分，有设备连接时， 会显示设备的地址。 
![[attachments/Pasted image 20240122150711.png]]
为了让其正常通信， 写入命令修改为9600波特率: 
```c
AT+UART_DEF=9600,8,1,0,0
```

![[attachments/Pasted image 20240122151330.png]]
此时Wifi模块的波特率已经被设置为9600

# 使用单片机串口与模块通信
串口的指令集分为:
1. 基础AT指令
2. Wifi功能性AT指令 
3. TCP/IP 工具箱
4. AT 命令 


| 指令 | 格式 |  |
| ---- | ---- | ---- |
| 测试命令 | AT + \<x\>=? | 用于查询或者设置命令或者内部程序的设置参数以及取值范围 |
| 查询 | AT + \<x\>? | 返回当前参数的值 |
| 设置命令 | AT + \<x\>=<...> | 设置用户自定义参数值 |
| 执行命令 | AT + \<x\> | 执行受模块内部程序控制的变参数不可变的功能  |

多数的AT + 参数值均具有上述指令。

### 基础AT指令: 
```c
AT // 测试启动
AT+UART_DEF=9600,8,1,0,0 // 波特率设置
AT+RST      // 重启
AT+ GMR    // 版本信息
/*
AT version:1.7.1.0(Jul 15 2019 16:58:04)
SDK version:3.0.1(78a3e33)
compile time:Feb 14 2020 09:19:42
*/
```

```c
AT+CWMODE // 选择WIFI应用(AP)模式
/*
AT+CWMODE?
+CWMODE:2
OK
*/
AT+CWJAP     // 加入AP
AT+CWLAP     // 列出挡片可用的AP
AT+CWQAP     // 退出与AP连接
AT+CWSAP     // 设置AP模式下的参数 
AT+CWLIF       // 查看已经接入设备的IP
```

例如设置Wifi 热点名称为FriedParrot's Wifi, 需要使用: 
```c
AT+CWSAP="FriedParrot's Wifi","parrot12345",1,0 // 最后参数为0不设置密码, 2设置密码
```

其中: ESP8266-WIFI 模块支持STA/AP/STA+AP三种模式: 

|  | 参数 |
| ---- | ---- |
| AT + CWMODE =  | 1: station 模式: ESP8266通过路由器连接互联网， 手机和电脑可以通过互联网进行远程控制。<br>2: AP 模式: 以PZ_ESP82666作为热点， 手机电脑可以直接与模块通信和局域网无线控制。<br>3: 可以互相切换互联网控制和局域网控制。   |
|  |  |

另外有一个比较重要的概念是AP, 可以通过AP+CWJAP来加入AP , 并通过AP+CWLAP来查询
```c
AT+CWJAP?
No AP
OK
```

本地的IP地址为: 
```c
AT+CIFSR
+CIFSR:APIP,"192.168.4.1"
+CIFSR:APMAC,"be:dd:c2:49:1b:33"
OK
```

在实际中，由于单片机的RXD(P30) 和TXD(P31)为串口引脚, 因此可以通过Wifi连接串口, 而Wifi模块读取到的数据会存储在SBUF中供读取, 同时使用串口中断4 进行读取数据, 过程可以参考[[💻softwares/⚡Circuit/51单片机教程/4. 定时器, 中断, 串口通信与温度传感器/串口通信讲解|串口通信讲解]] 

手机发送的数据格式为: 
+IPD, ID号, 发送数据长度: 数据

最后通过手机上的APP即可控制LED灯的亮灭了。 
