### (1) RET6 程序移植方法
补充: Stm32F103RET6 内存分布:
![[attachments/Pasted image 20240615225735.png]]
首先在Keil 中更改类型为 RET6
只要将启动文件改为 hd.s, 记得添加到工程。即可启动程序。

### (2) ESP8266 模块接法
- 常见的类似扩展模块有PZ-HC05蓝牙模块, PZ-CH1921以太网转串口模块等等,
由于WiFI模块芯片是使用3.3V的部分作为电源的， 在开发板上使用AMS 1117-3.3 V进行转换, 并接到P3接口上, 供3.3V引脚使用
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
![[attachments/IMG_20240122_150630.jpg|500]]

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

| 指令   | 格式               |                             |
| ---- | ---------------- | --------------------------- |
| 测试命令 | AT + \<x\>=?     | 用于查询或者设置命令或者内部程序的设置参数以及取值范围 |
| 查询   | AT + \<x\>?      | 返回当前参数的值                    |
| 设置命令 | AT + \<x\>=<...> | 设置用户自定义参数值                  |
| 执行命令 | AT + \<x\>       | 执行受模块内部程序控制的变参数不可变的功能       |
多数的AT + 参数值均具有上述指令。

### 基础AT指令: 
```c
AT // 测试启动
AT+UART_DEF=9600,8,1,0,0 // 波特率设置
AT+RST      // 重启
AT+GMR    // 版本信息
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

## 二、启动Wifi连接和互传的AT指令步骤
主要AT指令参考 [Expressif 文档](https://espressif-docs.readthedocs-hosted.com/projects/esp-at/zh-cn/release-v2.2.0.0_esp8266/AT_Command_Set/index.html), 以及[英文文档](https://docs.espressif.com/projects/esp-at/en/release-v3.4.0.0/esp32s2/Get_Started/index.html) 

### (1) 查看相关地址参数
> [!caution] 重点:连接IP地址
> 本地的IP地址为: AT+CIFSR

```c
AT+CIFSR
+CIFSR:APIP,"192.168.4.1"
+CIFSR:APMAC,"be:dd:c2:49:1b:33"
+CIFSR:STAIP,"0.0.0.0"
+CIFSR:STAMAC,"bc:dd:c2:49:1b:33"
OK
```

> [!caution] AP模式下的参数
> AT+CWSAP?
> `<ssid>, <pwd>,<chl>,<ecn>`
> ssid为接入点名称

> [!check] 开启连接教程
> 方法是必须将Wifi启动Server模式, 这需要首先关闭所有连接, 并配置 MUX = 1,

```shell
AT+CWMODE=3  # both satation and server, use AT+CWMODE?  for query 
AT+CIPMUX=1
# OK
AT+CIPSERVER=1,8080  #  开启server, 端口8080   Create a TCP server. 
AT+CIPSTA?                     # query the ip address f ESP8266
#AT+CIPSTA?
#+CIPSTA:ip:"192.168.43.190"
#+CIPSTA:gateway:"192.168.43.1"
#+CIPSTA:netmask:"255.255.255.0"
#  AT+CIPSTA=<"ip">[,<"gateway">,<"netmask">]
```

- 此时即可通过TCP控制单片机接收串口数据了,接收到一个设备会有对应的id号,
```shell
+IPD,0,5:hello
# 格式: +IPD,<id>,<len>:data
```
即接收到的 id 为0 的信息

- 为了让wifi模块给手机设备返回信息, 使用AT指令:
```shell
AT+CIPSEND=0,10     # 给 id 为0的
# 返回 OK, >
hellowreg
# SEND OK
```

关闭多路连接的AT指令方式如下
```shell
AT+CIPCLOSE=0   # 关闭和0连接
AT+CIPSERVER=0 # 关闭服务器模式
AT+CIPMUX=0       # 关闭多路连接
```


### (2) 使用 ESP8266 连接手机热点
首先设置 esp8266 为 station mode,
```shell 
AT+CWMODE=1   # set  the wifi mode to station mode 
# OK 
AT + CWJAP="HUAWEI P60","12345678"   # 密码必须和自己手机的对应
```

使用AT+CWJAP查询相应的可连接的热点
```shell 
AT+CWLAP
+CWLAP:(4,"TP-LINK_A859",-71,"18:f2:2c:87:a8:59",1,-21,0,4,4,7,0)
+CWLAP:(0,"NWPU-FREE",-75,"70:f9:6d:ca:b3:f0",1,-16,0,0,0,4,0)
+CWLAP:(0,"NWPU-FREE",-82,"74:50:4e:4a:d2:70",1,-34,0,0,0,4,0)
+CWLAP:(3,"HUAWEI P60",-22,"be:fb:e0:6c:24:cd",2,3,0,4,4,7,0)
+CWLAP:(0,"NWPU-FREE",-73,"74:50:4e:4b:53:b0",6,-26,0,0,0,4,0)
+CWLAP:(3,"AndroidAP_7451",-81,"c4:3c:b0:01:da:02",6,3,0,4,4,7,0)
+CWLAP:(3,"Enter",-47,"44:df:65:da:bb:de",9,-11,0,4,4,7,1)
+CWLAP:(0,"NWPU-FREE",-55,"74:50:4e:4a:d3:30",11,-29,0,0,0,4,0)
+CWLAP:(0,"NWPU-FREE",-70,"74:50:4e:4a:bf:50",11,-36,0,0,0,4,0)
+CWLAP:(4,"F**king Life",-71,"80:89:17:57:99:86",11,-7,0,4,4,7,0)
+CWLAP:(0,"NWPU-FREE",-75,"74:50:4e:4a:f7:f0",11,-41,0,0,0,4,0)
+CWLAP:(3,"",-87,"fc:87:43:63:57:ed",11,-34,0,4,4,7,1)
+CWLAP:(0,"NWPU-FREE",-69,"74:50:4e:4a:d2:50",11,-26,0,0,0,4,0)
```

```shell 
AT+CWQAP 
# OK
# WIFI DISCONNECT
```

```cpp fold title:常用指令
/******** AT command for control wifi module ********/ 
typedef enum {
    AT = 0,                    // Test AT startup
    AT_RST,                    // Restart module
    AT_GMR,                    // View version information
    AT_GSLP,                   // Enter Deep-sleep mode
    AT_RESTORE,                // Restore factory settings
    AT_UART_CUR,               // Set UART current temporary configuration, not saved to flash
    AT_UART_DEF,               // Set UART default configuration, saved to flash
    AT_SLEEP,                  // Set sleep mode
    AT_SYSRAM,                 // Query current remaining heap space and minimum heap space
    AT_SYSMSG,                 // Query/Set system prompt information
    AT_USERRAM,                // Operate user's free RAM
    AT_SYSFLASH,               // Query or read/write flash user partition
    AT_CWMODE,                 // Set Wi-Fi mode (Station/SoftAP/Station+SoftAP)
    AT_CWJAP,                  // Connect to AP
    AT_CWRECONNCFG,            // Query/Set Wi-Fi reconnection configuration
    AT_CWLAPOPT,               // Set AT+CWLAP command scan result properties
    AT_CWLAP,                  // Scan for available APs
    AT_CWQAP,                  // Disconnect from AP
    AT_CWSAP,                  // Configure ESP32 SoftAP parameters
    AT_CWLIF,                  // Query stations connected to ESP SoftAP
    AT_CWQIF,                  // Disconnect station from ESP SoftAP
    AT_CWDHCP,                 // Enable/Disable DHCP
    AT_CWDHCPS,                // Query/Set ESP SoftAP DHCP assigned IP address range
    AT_CWAUTOCONN,             // Power on auto connect to AP
    AT_CWAPPROTO,              // Query/Set SoftAP mode 802.11 b/g/n protocol standard
    AT_CWSTAPROTO,             // Set Station mode 802.11 b/g/n protocol standard
    AT_CIPSTAMAC,              // Query/Set ESP Station MAC address
    AT_CIPAPMAC,               // Query/Set ESP SoftAP MAC address
    AT_CIPSTA,                 // Query/Set ESP Station IP address
    AT_CIPAP,                  // Query/Set ESP SoftAP IP address
    AT_CWSTARTSMART,           // Start SmartConfig
    AT_CWSTOPSMART,            // Stop SmartConfig
    AT_WPS,                    // Set WPS function
    AT_MDNS,                   // Set mDNS function
    AT_CWJEAP,                 // Connect to WPA2 Enterprise AP (ESP32 Only)
    AT_CWHOSTNAME,             // Query/Set ESP Station hostname
    AT_CWCOUNTRY               // Query/Set Wi-Fi country code
} Wifi_ATCmd_t;
```

## 三、单片机控制ESP8266收发信息
### (1) 发送信息
首先初始化, 在发送信息时，以 \\r\\n 作为指令结尾
```cpp 
esp8266_Init();
Delay_ms(200);  // wait 200ms for uart to start
_esp8266_sendstring("AT\r\n");    // 发送AT指令
```

为了能够保证串口通信稳定, 参考[[💻softwares/⚡Circuit/🎮STM32教程/五、扩展模块/HC05蓝牙芯片的使用#(2) 与HC-05模块通信的上拉注意事项|HC05蓝牙芯片的使用]],  如果直接使用 TXD, 则无法使输出被 ESP8266检测, 采用类似方法增加上拉电阻到3.3V, 可保证通信稳定。
![[attachments/IMG_20240413_233432_edit_140546562116435.jpg|600]]

> [!caution] 上拉电阻的接法
> 注意必须在 PB10(TXD)部分接入上拉电阻, 而RXD不需要进行上拉。
> [**RX引脚**：大部分芯片内部已经自动集成了一个弱上拉电阻，一般不需要外部电阻上拉，只需要在软件配置时，配上上拉即可](https://blog.csdn.net/qq_22146161/article/details/122882581)[1](https://blog.csdn.net/qq_22146161/article/details/122882581)[3](https://blog.csdn.net/sun_shu_ying/article/details/135500436)[。这是为了避免RX无连接（或接触不良）下，不定电平的噪声导致输入乱码等问题](https://blog.csdn.net/iluzhiyong/article/details/51834475)[4](https://blog.csdn.net/iluzhiyong/article/details/51834475)。

通过如下代码, 获取UART发送信息的结构(如果收到\n则以@结尾,否则将不会输出\n),法线指令均以\n进行结尾。因此可以通过判断结尾的方法获取当前指令是否结束。

```cpp
for (int i = 1; i < 5; i++){
	for (int j = 1; j < 17; j++){
		char ch = ESP_Rx_buffer[index];
		index ++;
		if (ch == '\n'){
			OLED_ShowChar(i,j,'@');
			break; // change to next row
		}
		OLED_ShowChar(i,j, ch);
	}
	if (index >= strlen(ESP_Rx_buffer)) break;
}
```

```cpp title:主循环中,按照\n为结尾接收数据的函数编写
/// @brief called in main loop, copy message from ESP_Rx_buffer to ESP_Rx_buffer_shadow
/// @note  must receive "\n" as Stop Sign for each command 
/// @param 
/// @return 0: exit no error;
///         1: receive not complete; (still receive new cmd)
///         2: receive failed(due to transfer too fast);
///         3: receive failed(copy failed, may not happen);
/// @note   if this function return 1 exceed timeout. clear both buffer.
static uint8_t _esp8266_receive_msg_cb(void){
    if (!strlen(ESP_Rx_buffer)) return 0;  // exit normally
    uint8_t storge_full = 0;

    if (ESP_Rx_buffer[strlen(ESP_Rx_buffer)-1]!='\n' && strlen(ESP_Rx_buffer) < ESP8266_RX_BUFFER_SIZE){
        return 1;  // return not receive complete
    }
    // if storge string is full, throw off the msg received after last "\n"
    if (strlen(ESP_Rx_buffer) == ESP8266_RX_BUFFER_SIZE) {
        storge_full = 1;
        int i;
        for (i = ESP8266_RX_BUFFER_SIZE; i>0; i--){
            if (ESP_Rx_buffer[i-1] == '\n') break;
        }
        if (i==0) {
            _esp8266_resetRxBuffer();
            return 2;  // storge full and not receive "\n"
        }
        else{ // just receive the msg before last "\n"
            strncpy(ESP_Rx_buffer_shadow, ESP_Rx_buffer, i);
        }
    }
    else{
        strcpy(ESP_Rx_buffer_shadow,ESP_Rx_buffer);    // copy it to shadow buffer to excute 
    }
    // clear the buffer, also reset the Rx counter for next receive;
    memset(ESP_Rx_buffer, 0, strlen(ESP_Rx_buffer));
    buffR_idx = 0;

    /* test for ESP_Rx_buffer_shadow */
    if (ESP_Rx_buffer_shadow[strlen(ESP_Rx_buffer_shadow)-1]!='\n') {
        _esp8266_resetRxBuffer();
        return 3;  // copy failed 
    }
    if (storge_full) return 2;
    return 0;
}
```
需要说明的是, 串口机制采用了使用\\n进行结尾的传输机制, 因此可能接收到K, 但是绝对不可能是单独的O，此时,在调用完 receive_msg_cb之后, 则自动先将 RxBuffer 中信息复制到 Rx_shadow_buffer 中, 再通过动态内存分配的方式, 分配到**线性表 CmdList中**(这里没有使用动态内存)
![[attachments/Pasted image 20240618120922.png]]
```cpp
for (int i = 0; i < ESP_Cmd_List.cmd_buffer_length; i++){
	OLED_ShowString(i+1, 1, ESP_Cmd_List.cmd_buffer[i]);
}
```

采用的串口收发机制使用了指令+超时退出的机制, 其中在send_cmd之后, 会等待 `SP8266_RESPONSE_TIMEOUT` 时间,来接收所有数据, 包括应答, 如果接收到应答则直接返回, 否则等待超时后, 返回错误信息。 

### (2) 全新的接收AT指令架构
接收指令较为麻烦, 但好在每一条指令均是以 \\n 进行结尾的, 同时, 我们考虑以 115200 波特率进行通信的过程, 首先要求中断时间尽可能最小化, 同时要求

传统的架构是使用一个足够大的 buffer, 同时使用双缓冲结构, 在主循环中等待数据接收结束(判断strlen(buffer = '\\n'))时, 将数据转移到 buffer_shadow 中。然后将buffer_shadow 中的部分进行分割。分割后的数据转移到动态分配内存的线性表或者链表中(为了方便,我们使用线性表进行存储)

由于往往**使用 AT+CWLAP 等等会在瞬间接收大量信息**, 而我们接收指令的间隔是1ms, 在115200的波特率下, 1ms平均接收数据量为:
$$0.001 / (\frac{1}{115200} *9) \approx 12.8$$
因此理论上1ms判断和256大小的 buffer 是完全够的。但是,在连续接收中,很可能在主循环中, 得到最后一个字不是\\n, 则接收会继续下去, 直到 buffer 变满无法继续接收数据, 导致发生崩溃错误。

此处我们使用一种**全新的程序逻辑架构**:
![[Excalidraw/ESP8266 WIFI模块芯片 2024-06-19 10.05.26|750]]
由于中断函数会自动将RxBuffer部分按照\\n分割到，主函数会将 Rx_buffer_shadow 分割成指令, 

首先,这个架构也可能会产生新的问题,由于有可能在接收应答 OK 之前, 调用执行命令, 实际上只要返回没有执行完毕然后自动等待应答直到超时就可以了。 


### (3) 74880 波特率和Boot loader的设置 
这个问题可能与ESP8266的波特率设置有关。让我们来看看可能的原因和解决方案：

1. [**冷启动或硬件复位时的乱码**：初次上电或硬件复位时，ESP8266系统一定会输出乱码，除非你使用的固件设置了74880波特率。这是因为系统上电时会运行厂商芯片内部的Boot loader，而Boot loader会将整个SoC的波特率初始化为74880。如果你的串口软件不在这个波特率上，就会出现乱码。你可以尝试将串口波特率设置为74880，以查看这些乱码的含义](https://blog.csdn.net/qq_15181569/article/details/103567084)[1](https://blog.csdn.net/qq_15181569/article/details/103567084)。
    
2. **软件或看门狗复位的问题**：当你使用AT指令复位后，下次启动时仍然会执行厂商SDK的代码，该部分代码不开源，无法修改。此代码会将波特率设置为115200。如果你的波特率恰好是115200，那么在执行复位时将不会出现乱码。如果不是，那么在串口速率切换的过程中，你依然会看到乱码。

总之，冷启动的乱码是在不使用74880波特率的情况下无法避免的。如果你的波特率不是74880，你可以尝试设置为74880，或者查看其他波特率是否能够正常显示数据。如果问题仍然存在，可能需要检查硬件连接或考虑其他故障原因。希望这些提示对你有所帮助!

给出如下两个 wifi_init 和 wifi_debug 代码:
```c
uint8_t wifi_init(void){
    if (esp8266_Init()!=ESP_RES_OK) return 1;
    if (esp8266_cmd_wifi_set_softAPconfig("\"FriedParrot\"", "\"parrot12345\"", 1, 0, 1) != ESP_RES_OK)  return 1;
    esp8266_cmd_tcp_set_cipmux(1);
    esp8266_cmd_tcp_set_tcpServer(1, 8080);
    OLED_ShowString(1,1,"Wifi Init Suc");
    if (esp8266_cmd_tcp_cipsend("0", 17 , NULL, NULL, "Wifi Init Succeed")!=ESP_RES_OK) return 1;
    return 0;
}

uint8_t wifi_debug(char* str){
    uint8_t retry_time = 0;
    while (retry_time < 3){
        if (esp8266_cmd_tcp_cipsend("0",strlen(str), NULL, NULL, str)==ESP_RES_OK) {
            return 0;
        }
        Delay_ms(800);
        retry_time++;
    }
    return 1;
}

```

