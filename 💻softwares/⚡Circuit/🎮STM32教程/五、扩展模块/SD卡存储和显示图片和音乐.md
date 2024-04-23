# 一、SD卡通信和指令相关简介
### (1) SD卡基本简介
主要的SPI通信方法参考[[💻softwares/⚡Circuit/🎮STM32教程/三、串口与通信协议/4. SPI 通信协议与W25Q64 Flash芯片|SPI 通信协议]]部分; SD卡官方的通信协议是SDIO通信协议。因此也同时兼容stm32的SDIO外设。
比较完整的命令可以参考 http://www.chlazza.net/sdcardinfo.html 
通信资料参考 http://www.openedv.com/docs/boards/stm32/zdyz_stm32f103_miniV4.html 
示例程序参考 https://blog.csdn.net/LaiYiFei25/article/details/122083456 
参考手册: 搜索SD Card Physical Layer Specification 

典型的SPI模式是通过CPOL和CPHA进行控制的, 参考 [[💻softwares/⚡Circuit/🎮STM32教程/三、串口与通信协议/4. SPI 通信协议与W25Q64 Flash芯片#(3) SPI 通信时序|SPI 通信时序]] 部分。
SD卡一般有9个引脚, 包括VDD, VSS,(SCLK, DI, DO, CS) CDN, DATA1, DATA2, 其中DATA1和DATA2是在SDIO模式下使用的。
![[attachments/Pasted image 20240420084041.png]]
其中SPI Mode下, CMD线是MOSI, 而DATA0是MISO线。 DATA3是CS线, 在SD Module 中均可以用来传输数据。

![[attachments/Pasted image 20240420001244.png]]

SD卡的<mark style="background: transparent; color: red">通信格式是 MSB first 高位先行</mark>, 仅**支持1byte每次传输的位宽**。当片选信号CS拉低时, 则开始通信;
SPI有默认512byte 的操作扇区, 因此往往定义一个512字节的数组作为缓冲区进行存储。

```cpp fold title:使用如下函数进行初始化
#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"

#define SD_Write_CS(x) GPIO_WriteBit(GPIOA, GPIO_Pin_1, (BitAction)(x))

uint8_t SDCard_Buffer[512];   // define the SD card Buffer

/* Use SPI1 PA5 to PA7 as Data line, also use PA1 as CS -> use SPI DMA for SD card communication */
void SDCard_GPIOInit(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, FunctionalState::ENABLE);
    
    /* for SPI1: PA5: SCK , PA7: MOSI */
    GPIO_InitTypeDef *GPIO_InitStruct = new GPIO_InitTypeDef();
    GPIO_InitStruct -> GPIO_Mode = GPIOMode_TypeDef::GPIO_Mode_AF_PP;
    GPIO_InitStruct -> GPIO_Speed = GPIOSpeed_TypeDef::GPIO_Speed_50MHz;
    GPIO_InitStruct -> GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_Init(GPIOA, GPIO_InitStruct);

    GPIO_InitStruct->GPIO_Mode = GPIOMode_TypeDef::GPIO_Mode_IPU;
    GPIO_InitStruct->GPIO_Pin  = GPIO_Pin_6;
    GPIO_Init(GPIOA, GPIO_InitStruct);

    /* PA1: CS */
    GPIO_InitStruct->GPIO_Mode = GPIOMode_TypeDef::GPIO_Mode_Out_PP;
    GPIO_InitStruct->GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOA, GPIO_InitStruct);
    SD_Write_CS(1);   // firstly not select SD card
}

/* Init SPI1 as the SPI Peripheral for communication */
void SDCard_SPIInit(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, FunctionalState::ENABLE);
    SPI_I2S_DeInit(SPI1);
    SPI_InitTypeDef* SPI_InitStruct = new SPI_InitTypeDef();
    SPI_StructInit(SPI_InitStruct);
    SPI_InitStruct->SPI_Mode= SPI_Mode_Master;         // ** don't forget! 
    SPI_InitStruct->SPI_CPHA = SPI_CPHA_1Edge;          // bit capture: 1st edge
    SPI_InitStruct->SPI_CPOL = SPI_CPOL_Low;            // clock polarity 
    SPI_InitStruct->SPI_DataSize = SPI_DataSize_16b;     // transfer 8 bit at once
    SPI_InitStruct->SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
    SPI_InitStruct->SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct->SPI_FirstBit  = SPI_FirstBit_MSB;  // use MSB mode
    SPI_InitStruct->SPI_NSS = SPI_NSS_Soft;            // use software NSS control
    SPI_Init(SPI1,  SPI_InitStruct);
    SPI_Cmd(SPI1, FunctionalState::ENABLE);
}

/* exchange a byte using SPI */
uint16_t SDCard_ExchangeByte(uint16_t byte){
    SD_Write_CS(0);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != FlagStatus::SET); // wait for send 
    SPI_I2S_SendData(SPI1, byte);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)!= FlagStatus::SET); 
    SPI_I2S_ClearFlag(SPI1, SPI_I2S_FLAG_RXNE);
    uint16_t byte_res = SPI_I2S_ReceiveData(SPI1);      // start receive data when RX not Empty
    SD_Write_CS(1);
    return byte_res;
}
```

根据容量大小，分为SD, SDHC和SDXC卡
![[attachments/Pasted image 20240421170547.png]]


### (2) SD卡寄存器介绍
SD卡往往有如下5个基本的寄存器，具体如下(参考p160):
![[Excalidraw/SD卡存储图片和音乐 2024-04-20 09.03.51|700]]
初始化时, 需要至少发送74个时钟, 且初始化时的 CLK 时钟不能超过400kHz (可以先设置Prescaler为256分频)

对于SD卡的**所有命令都是和0x40相合并然后发送**的,**在发送命令之前需要等待SD卡闲置, 方法是发送0xFF直到收到的回复为0xFF**则SD卡闲置。

```cpp fold title:检测SD卡的测试
#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"

uint8_t SDCard_DATA_Buffer[512];   // define the SD card Buffer

#define SDCard_Write_CS(x) GPIO_WriteBit(GPIOA, GPIO_Pin_1, (BitAction)(x))
#define spi_timeout 100
bool SDCard_error_flag = false;

#define SDCard_CMD0 0                                // SDCard Reset 
#define SDCard_CMD8 8                                // SEND_IF_COND
#define SDCard_CMD9 9                                // read CSD data 
#define SDCard_CMD12 12                              // stop data transmission 
#define SDCard_CMD13 16                              // set the sector size 
#define SDCard_CMD17 17                              // read sector 
#define SDCard_CMD18 18                              // read multiple sectors 
#define SDCard_CMD23 23                              // erase sector before write 
#define SDCard_CMD24 24                              // write sector
#define SDCard_CMD25 25                              // write multiple sectors
#define SDCard_CMD41 41                              // return 0x00
#define SDCard_CMD55 55                              // return 0x01
#define SDCard_CMD58 58                              // read OCR Information 
#define SDCard_CMD59 59                              // Enalbe or disable CRC

#define SDCard_RESPONSE_NO_ERROR               0x00
#define SDCard_RESPONSE_IN_IDLE_STATE          0x01
#define SDCard_RESPONSE_ERASE_RESET            0x02
#define SDCard_RESPONSE_ILLEGAL_COMMAND        0x04
#define SDCard_RESPONSE_COM_CRC_ERROR          0x08
#define SDCard_RESPONSE_ERASE_SEQUENCE_ERROR   0x10
#define SDCard_RESPONSE_ADDRESS_ERROR          0x20
#define SDCard_RESPONSE_PARAMETER_ERROR        0x40
#define SDCard_RESPONSE_FAILURE                0xFF

#define SDCard_RESPONSE_DATA_OK                0x05
#define SDCard_RESPONSE_DATA_CRC_ERROR         0x0B
#define SDCard_RESPONSE_DATA_WRITE_ERROR       0x0D
#define SDCard_RESPONSE_DATA_OTHER_ERROR       0xFF

/// @brief wait for spi respond (use global timeout as timeout)
/// @param SPIx 
/// @param SPI_I2S_FLAG 
/// @param state 
void SDCard_WaitSPIFlag(SPI_TypeDef* SPIx,uint16_t SPI_I2S_FLAG, FlagStatus state){
    uint32_t temp = spi_timeout;
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG) != state){
        temp--;
        Delay_ms(1);
        if (temp==0){
            SDCard_error_flag = true;
            break;
        }
    }
}

/// @brief Use SPI1 PA5 to PA7 as Data line, also use PA1 as CS -> use SPI DMA for SD card communication
void SDCard_GPIOInit(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, FunctionalState::ENABLE);
    /* for SPI1: PA5: SCK , PA7: MOSI */
    GPIO_InitTypeDef *GPIO_InitStruct = new GPIO_InitTypeDef();
    GPIO_InitStruct -> GPIO_Mode = GPIOMode_TypeDef::GPIO_Mode_AF_PP;
    GPIO_InitStruct -> GPIO_Speed = GPIOSpeed_TypeDef::GPIO_Speed_50MHz;
    GPIO_InitStruct -> GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_Init(GPIOA, GPIO_InitStruct);

    GPIO_InitStruct->GPIO_Mode = GPIOMode_TypeDef::GPIO_Mode_IPU;
    GPIO_InitStruct->GPIO_Pin  = GPIO_Pin_6;
    GPIO_Init(GPIOA, GPIO_InitStruct);

    /* PA1: CS */
    GPIO_InitStruct->GPIO_Mode = GPIOMode_TypeDef::GPIO_Mode_Out_PP;
    GPIO_InitStruct->GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOA, GPIO_InitStruct);
    SDCard_Write_CS(1);   // firstly not select SD card
}

/// @brief Init SPI1 as the SPI Peripheral for communication 
void SDCard_SPIInit(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, FunctionalState::ENABLE);
    SPI_I2S_DeInit(SPI1);
    SPI_InitTypeDef* SPI_InitStruct = new SPI_InitTypeDef();
    SPI_StructInit(SPI_InitStruct);
    SPI_InitStruct->SPI_Mode= SPI_Mode_Master;         // ** don't forget! 
    SPI_InitStruct->SPI_CPHA = SPI_CPHA_1Edge;          // bit capture: 1st edge
    SPI_InitStruct->SPI_CPOL = SPI_CPOL_Low;            // clock polarity 
    SPI_InitStruct->SPI_DataSize = SPI_DataSize_8b;     // transfer 8 bit at once
    SPI_InitStruct->SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;  // 256 as prescaler firstly
    SPI_InitStruct->SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct->SPI_FirstBit  = SPI_FirstBit_MSB;  // use MSB mode
    SPI_InitStruct->SPI_NSS = SPI_NSS_Soft;            // use software NSS control
    SPI_InitStruct->SPI_CRCPolynomial = 10;            // CRC settings
    SPI_Init(SPI1,  SPI_InitStruct);
    SPI_Cmd(SPI1, FunctionalState::ENABLE);         
}

/// @brief  exchange a byte using SPI1 
/// @param byte the data to send to 
/// @return 
uint16_t SDCard_ExchangeByte(uint16_t byte){
    SDCard_WaitSPIFlag(SPI1, SPI_I2S_FLAG_TXE, FlagStatus::SET);
    SPI_I2S_SendData(SPI1, byte);
    SDCard_WaitSPIFlag(SPI1, SPI_I2S_FLAG_RXNE, FlagStatus::SET);
    SPI_I2S_ClearFlag(SPI1, SPI_I2S_FLAG_RXNE);
    uint16_t byte_res = SPI_I2S_ReceiveData(SPI1);  // start receive data when RX not Empty
    return byte_res;
}

/// @brief Send Command to SD card 
/// @param cmd 
/// @param param 
/// @param crc 
/// @return 
uint16_t SDCard_SendCmd(uint8_t cmd, uint32_t param, uint8_t crc){
    uint8_t response;
    SDCard_Write_CS(1);
    Delay_ms(1);
    SDCard_Write_CS(0);
    while (true){
        uint8_t r = SDCard_ExchangeByte(0xFF);
        if (r == 0xFF) break;  // wait until SD card not busy
    }
    SDCard_ExchangeByte(cmd | 0x40);   // reset byte
    SDCard_ExchangeByte((uint8_t)(param >> 24));
    SDCard_ExchangeByte((uint8_t)(param >> 16));
    SDCard_ExchangeByte((uint8_t)(param >> 8));
    SDCard_ExchangeByte((uint8_t)(param));
    SDCard_ExchangeByte(crc);
    if (cmd == SDCard_CMD12) SDCard_ExchangeByte(0xFF);
    do{
        response = SDCard_ExchangeByte(0xFF);
        OLED_ShowHexNum(2,1, response, 4);
    }while(response & 0x80);
    SDCard_Write_CS(1);
    return response;
}

void SDCard_Init(){
    SDCard_GPIOInit();
    SDCard_SPIInit();
    /*************** SD card start sequence  ****************/
    /* 1. send at least 74 clock pulse with CS set to 0 (not exceed 400kHz)*/
    SDCard_Write_CS(1);
    for (uint16_t i = 0; i < 10; i++){
        SDCard_ExchangeByte(0xFF);  // 80 clock 0xFF send for sync
    }
    
    SDCard_SendCmd(SDCard_CMD0, 0, 0x95);
    
    OLED_ShowString(3,1, "Send Cmd Suc");
    // uint8_t res;
    // do {
    //     /* send 0xFF data to SD card until the response highest bit is 0*/
    //     res = SDCard_ExchangeByte(0xFE);
    //     OLED_ShowHexNum(3,1, res, 2);
    // }while(res & 0X80);

    
}

int main(){
    OLED_Init();
    SDCard_Init();   
    while (1)
    {
        /* code */
        SDCard_ExchangeByte(0x0F0F);
        Delay_ms(100);
    }
}
```

> [!caution] 寄存器部分位置
> 具体的寄存器相关在p116中有相应位解释， 而整体寄存器在p178部分, 可以查看每一个寄存器的位数以及代表的相关内容

### (3) SD卡相关指令讲解
#### 1. 指令格式
SD卡的命令格式如下(**所有命令都是和0x40相合并然后发送为01xxxxxx的格式**): 
![[Excalidraw/SD卡存储图片和音乐 2024-04-20 14.33.46|900]]
在发送完之后, 发送FF等待响应。参考p237 Detailed Command Description, 有详细的指令解释。

其中, **SD卡的SPI指令包括标准指令CMD和APP指令ACMD两种**, 参考p241, 其中, CMD是直接发送的指令, **ACMD是在指令前必须发送一个CMD55(APP_CMD)来作为前缀的**。即CMD55会将下一个指令置为APP指令而非标准指令。
![[attachments/Pasted image 20240421152402.png|800]]

SD卡**命令的响应返回类型**有R1, R2, R3, R7(CMD8)类型等, 一般对于SD卡返回的R1类型, 例如0x09是CRC校验错误;SDIO的Response具体可以在p116找到; 而SPI的响应在p253找到:
![[attachments/Pasted image 20240421110640.png]]

#### 2. CRC校验计算
指令的CRC校验码可以通过调用以下函数直接获取。但是一般直接用计算器获取
```cpp
RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, FunctionalState::ENABLE); //计算之前必须先开启CRC时钟
CRC_CalcBlockCRC(buff, buff_size);  // 计算CRC块校验码
```

需要说明的是, 实际上SPI模式下, CRC校验位虽然是必须发送的， 但是SD卡读到之后会自动忽略(**SPI默认不开启CRC校验, 而SDIO是开启的, 除此之外, CMD0, CMD8的校验是启用的**); **CRC校验码位中, 前7位为CRC校验位, 而停止位为0**; SPI的CRC校验可以通过CMD59进行配置。

CRC具体参考p95的CRC校验部分。常用的21个标准CMC模型如下(其中SD卡或MMC使用的是CRC-7/MMC模型，多项式为1000 1001,舍弃最高位即POLY = 0x09POLY):
![[attachments/Pasted image 20240421093955.png|1000]]
实际求解数据的CRC校验码, 我们使用CRC计算器, 以 0x40 0x00 0x00 0x00, 0x00为例, 获取的4A是CRC校验值, 则CRC部分使用0x4A << 1 + 1 = 0x95 获得对应的CRC校验码;
![[attachments/Pasted image 20240421104424.png]]
发送CMD8时, 参数为: 0x48 0x00 00 01 AA, 得到0x87为CRC校验码

#### 3. 初始化命令
SD卡的初始化流程如下:
1. 在CS为高情况下, 以 < 400kHz 速度发送74个CLK, 等待同步
2. 卡复位 (发送CMD0进行SD卡复位操作, 如果成功返回0x01)
3. 发送CMD8, 检测卡的类型是否为USB2.0 如果返回非法命令, 则为其他卡(之后的CMD58是虚线，可以不发送(Not Mandatory to send)) 
4. 对于SD2.0卡，参考[链接](https://blog.csdn.net/xiaolong1126626497/article/details/127985503)，需要**发送ACMD41**, 即**循环发送CMD55, CMD41直到返回0x00**
5. 发送CMD58，**获取OCR 寄存器**,

4. 如果不是SD2.0卡(不支持CMD8, 不返回0x01), 若返回值不为0x01，则进一步判断是V1.0卡还是MMC卡：先发送循环命令CMD55+CMD41进行复位，如果复位不成功则考虑是MMC卡，如果复位成功，则为V1.0卡; 
5. 如果是SD 1.0 卡, 则
具体流程在参考手册p229: 
![[attachments/Pasted image 20240421111115.png|850]]


----
CMD8部分
CMD8的参数设置是0x1AA(实际上是check pattern), 参考p118:
![[attachments/Pasted image 20240421131545.png]]
注意: 指令格式是参考p245的SPI回应指令的, 显然**对于R7指令, 是以R1指令开头的**，因此在R1指令之后, 8-31bit包含了voltage accepted信息, 因此**后面是没有CRC校验码的**。只需接收4字节。
下面是Cmd8的用法格式是两个字节的空白, 一个字节supply voltage(仅前3位设置有用), 而返回的
![[attachments/Pasted image 20240421131647.png]]
不同Voltage accepted参数如下:
![[attachments/Pasted image 20240421110146.png]]

另外需要注意的是, 原始程序在函数`int SD_sendcmd(uint8_t cmd,uint32_t arg,uint8_t crc)`中, 最后并没有加上取消片选即SD_CS(0)的操作, 这可能之后会导致部分的问题;

因此, 我们在接收CMD8的应答R7时, 需要恢复片选, 使用下面代码:
```cpp
SDCard_Write_CS(0);        
for (uint8_t i = 0; i< 4; i++) buf[i] = SDCard_ExchangeByte(0xFF); // throw the first 3 bits
SDCard_Write_CS(1);
```

需要说明: 由于不查验SPI模式下的CRC校验, 所以最后的校验码可以使用0x01代替

--- 
ACMD41:
ACMD41 (APP_Cmd) 是用于**初始化主机是否支持高容量卡**的指令。
![[attachments/Pasted image 20240421152132.png|800]]

因此, 为了支持高容量HC卡, 我们需要使用0x40000000作为第二个参数;
```cpp
for (uint16_t retry = 0x0FFF, uint8_t r3 = 0x01; (retry >0 && r3!=0x00); retry--){
                SDCard_SendCmd(SDCard_CMD55, 0x00, 0x01);
                r3 = SDCard_SendCmd(SDCard_CMD41, 0x40000000, 0x01);  // support High Capacity Card
                /*exit when r3 = 0x00*/
            }
```

----
CMD58 获取OCR寄存器
其中， CCS bit is assigned to OCR[30], 而对于支持 2.0 协议的卡, CCS位用于表示为SDHC或者SDSC;

R3对应的回复(245)如下:
![[attachments/Pasted image 20240421145953.png]]
OCR寄存器分布如下:
![[attachments/Pasted image 20240421153829.png]]
显然15-23表示不同的支持范围, 我们需要2.7-3.6V的电压, 所以一般需要的是0xFF回应, 其中比较重要的是29, 30位;
29位表示是否支持UHS-II 通信
30位为CCS,（**注意: 当SD卡忙状态时, 则CCS无效**）

在CMD41之后, 实际上就已经初始化完成了, 而对于SD2.0卡, 还要加一个 CMD58.
```cpp fold title:SD卡初始化代码示例如下
#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"
#include <stdlib.h>
#include "SDCard.h"

#define MSD_BLOCKSIZE			 512
uint8_t SDCard_DATA_Buffer[MSD_BLOCKSIZE];   // define the SD card Buffer

#define SDCard_Write_CS(x) GPIO_WriteBit(GPIOA, GPIO_Pin_1, (BitAction)(x))
#define spi_timeout 100
bool SDCard_error_flag = false;


typedef struct{
    uint8_t VoltageSupport;         /* Voltage Support (2.7 - 3.6)*/
    uint8_t UHS_II_Support;         /* If support UHS-II, set to 1*/
}sd_infomation;

/*********** Declaration for functions*************/
void SDCard_RCCconfig(void);
void SDCard_GPIOconfig(void);
void SDCard_SPIconfig(void);
void SDCard_SetPrescaler(SPI_TypeDef *SPIx, uint16_t SPI_baudrateprescaler);
void SDCard_WaitSPIFlag(SPI_TypeDef* SPIx,uint16_t SPI_I2S_FLAG, FlagStatus state);
uint16_t SDCard_ExchangeByte(uint16_t byte);
uint16_t SDCard_SendCmd(uint8_t cmd, uint32_t param, uint8_t crc);
void SDCard_ReadBytes(uint8_t* &data_buff, uint32_t length);

/// @note for card 1.0 and MMC, the code may be not correct
/// @brief Initialization Function for SD card, tested for SD card
/// @return 
uint8_t SDCard_Init(){
    uint8_t sd_type=SDCard_TYPE_ERR;  // record the SD card Type
    
    /*************** Peripheral Configurations **************/
    SDCard_RCCconfig();  // clock 
    SDCard_GPIOconfig(); // GPIO
    SDCard_SPIconfig();  // SPI
    
    /*************** SD card start sequence  ****************/
    /* 1. send at least 74 clock pulse with CS set to 0 (not exceed 400kHz)*/
    SDCard_SetPrescaler(SPI1, SPI_BaudRatePrescaler_256);
    SDCard_Write_CS(1);
    for (uint16_t i = 0; i < 10; i++){
        SDCard_ExchangeByte(0xFF);  // 80 clock 0xFF send for sync
    }
    SDCard_SetPrescaler(SPI1, SPI_BaudRatePrescaler_2); 
    
    /* 2. Go to Idle State Command */
    uint8_t r1 = 0x00;
    for (uint32_t retry = 0xFFF; r1!=0x01 ; retry--){
        r1 = SDCard_SendCmd(SDCard_CMD0, 0, 0x95);   // wait for 0x01 response > 
    }
    // OLED_ShowString(2,1, "idle suc");
    
    /* 3. Check the SD Card Type (v2.0 or later)*/
    if (r1!= 0){  // must return 0x00 after busy -> or not valid card
        uint8_t r2 = SDCard_SendCmd(SDCard_CMD8, 0x000001CD, 0x55);   // use 0xCD for check pattern
        if (r2 == 0x01){
            // check if the SD card support 2.7v to 3.6V
            uint8_t* buf = new uint8_t(4);
            SDCard_ReadBytes(buf, 4);
            if(buf[2]==0X01&&buf[3]==0XCD) // SD 2.0 card detected 
            {
                /* wait for SD2.0 initialize successful */
                for (uint16_t retry = 0x0FFF, r = 0x01; (retry && r) ; retry--){
                    SDCard_SendCmd(SDCard_CMD55, 0x00, 0x01);
                    r = SDCard_SendCmd(SDCard_CMD41, 0x40000000, 0x01);  // support High Capacity Card
                    /*exit when r3 = 0x00*/
                }
                /**** read OCR Register for test the SD2.0 Version ******/
                uint8_t r3 = SDCard_SendCmd(SDCard_CMD58, 0x00,0x01);
                if (r2 && r3 == 0x00){
                    uint8_t *buf2 = new uint8_t(4);  // receive OCR reg data 
                    SDCard_ReadBytes(buf2, 4);
                    if (buf2[0] & 0x40){
                        sd_type = SDCard_TYPE_V2HC;  // OLED_ShowString(3,1,"SD_HC Card");
                    }else{
                        sd_type = SDCard_TYPE_V2;    // OLED_ShowString(3,1,"SD Card");
                    }
                    /* reserve: 
                    voltage_support = buf2[1];
                    if (buf2[0] & 0x20){
                        uhs_support = 1;             // OLED_ShowString(2,1,"UHS support");
                    }else{
                        uhs_support = 0;             // OLED_ShowString(2,1,"UHS not support");
                    } 
                    */
                }
            }
            // else sd_type = SDCard_TYPE_ERR; -> return voltage not support error
            //  not support voltage 2.7 to 3.3V
        }else if (r2!=0xFF){
            // CMD 8 not support -> SD V1 or the MMC card
            uint8_t r3 = 0x00; uint16_t retry = 0x0FFF;

            SDCard_SendCmd(SDCard_CMD55, 0x00, 0x01);
            r3 = SDCard_SendCmd(SDCard_CMD41, 0x00, 0x01);

            if(r3 <= 0x01){                                      // for SD card only return 0x00 and 0x01
                sd_type = SDCard_TYPE_V1;                        // MMC Card not support the CMD
                for (uint8_t r = 0x01; (retry && r) ; retry--){  //  wait for ACM41 ready (0x00);
                    SDCard_SendCmd(SDCard_CMD55, 0x00, 0x01);
                    r = SDCard_SendCmd(SDCard_CMD41, 0x00, 0x01);
                }  
            }else{ // MMC card, which not support  
                sd_type = SDCard_TYPE_MMC;
                for (uint8_t r = 0x01; (retry && r) ; retry--){ // wait for MMC card to exit IDLE
                    r = SDCard_SendCmd(SDCard_CMD1, 0x00, 0x01);
                }
            }
            if (retry == 0 || SDCard_SendCmd(SDCard_CMD16, 512, 0x01)!= 0x00) sd_type = SDCard_TYPE_ERR;  // ERROR card 
        }
    }
    // return 0 if succeed
    if (sd_type) return 0;
    else return 1;  // 1 error 
}

int main(){
    OLED_Init();
    SDCard_Init();   
    while (1)
    {
        /* code */
        SDCard_ExchangeByte(0x0F0F);
        Delay_ms(100);
    }
}

/*****************  Private Function Declarations *********************/
/// @brief change the prescaler during different 
/// @param SPIx 
/// @param SPI_baudrateprescaler 
void SDCard_SetPrescaler(SPI_TypeDef *SPIx, uint16_t SPI_baudrateprescaler){
    SPI_Cmd(SPI1, FunctionalState::DISABLE);    
    SPI_InitTypeDef* SPI_InitStruct = new SPI_InitTypeDef();
    SPI_StructInit(SPI_InitStruct);
    SPI_InitStruct->SPI_Mode= SPI_Mode_Master;         // ** don't forget! 
    SPI_InitStruct->SPI_CPHA = SPI_CPHA_1Edge;          // bit capture: 1st edge
    SPI_InitStruct->SPI_CPOL = SPI_CPOL_Low;            // clock polarity 
    SPI_InitStruct->SPI_DataSize = SPI_DataSize_8b;     // transfer 8 bit at once
    SPI_InitStruct->SPI_BaudRatePrescaler = SPI_baudrateprescaler;  // 256 as prescaler firstly
    SPI_InitStruct->SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct->SPI_FirstBit  = SPI_FirstBit_MSB;  // use MSB mode
    SPI_InitStruct->SPI_NSS = SPI_NSS_Soft;            // use software NSS control
    SPI_InitStruct->SPI_CRCPolynomial = 10;            // CRC settings
    SPI_Init(SPI1,  SPI_InitStruct);
    SPI_Cmd(SPI1, FunctionalState::ENABLE);
}

/// @brief 
/// @param  
void SDCard_RCCconfig(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, FunctionalState::ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, FunctionalState::ENABLE);
}

/// @brief wait for spi respond (use global timeout as timeout)
/// @param SPIx 
/// @param SPI_I2S_FLAG 
/// @param state 
void SDCard_WaitSPIFlag(SPI_TypeDef* SPIx,uint16_t SPI_I2S_FLAG, FlagStatus state){
    uint32_t temp = spi_timeout;
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG) != state){
        temp--;
        Delay_ms(1);
        if (temp==0){
            SDCard_error_flag = true;
            break;
        }
    }
}

/// @brief Use SPI1 PA5 to PA7 as Data line, also use PA1 as CS -> use SPI DMA for SD card communication
void SDCard_GPIOconfig(void){
    /* for SPI1: PA5: SCK , PA7: MOSI */
    GPIO_InitTypeDef *GPIO_InitStruct = new GPIO_InitTypeDef();
    GPIO_InitStruct -> GPIO_Mode = GPIOMode_TypeDef::GPIO_Mode_AF_PP;
    GPIO_InitStruct -> GPIO_Speed = GPIOSpeed_TypeDef::GPIO_Speed_50MHz;
    GPIO_InitStruct -> GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_Init(GPIOA, GPIO_InitStruct);

    GPIO_InitStruct->GPIO_Mode = GPIOMode_TypeDef::GPIO_Mode_IPU;
    GPIO_InitStruct->GPIO_Pin  = GPIO_Pin_6;
    GPIO_Init(GPIOA, GPIO_InitStruct);

    /* PA1: CS */
    GPIO_InitStruct->GPIO_Mode = GPIOMode_TypeDef::GPIO_Mode_Out_PP;
    GPIO_InitStruct->GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOA, GPIO_InitStruct);
    SDCard_Write_CS(1);   // firstly not select SD card
}

/// @brief Init SPI1 as the SPI Peripheral for communication 
void SDCard_SPIconfig(void){
    SPI_I2S_DeInit(SPI1);
    SPI_InitTypeDef* SPI_InitStruct = new SPI_InitTypeDef();
    SPI_StructInit(SPI_InitStruct);
    SPI_InitStruct->SPI_Mode= SPI_Mode_Master;         // ** don't forget! 
    SPI_InitStruct->SPI_CPHA = SPI_CPHA_1Edge;          // bit capture: 1st edge
    SPI_InitStruct->SPI_CPOL = SPI_CPOL_Low;            // clock polarity 
    SPI_InitStruct->SPI_DataSize = SPI_DataSize_8b;     // transfer 8 bit at once
    SPI_InitStruct->SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;  // 256 as prescaler firstly
    SPI_InitStruct->SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct->SPI_FirstBit  = SPI_FirstBit_MSB;  // use MSB mode
    SPI_InitStruct->SPI_NSS = SPI_NSS_Soft;            // use software NSS control
    SPI_InitStruct->SPI_CRCPolynomial = 0x09;          // CRC settings
    SPI_Init(SPI1,  SPI_InitStruct);
    SPI_Cmd(SPI1, FunctionalState::ENABLE);
}

/// @brief  exchange a byte using SPI1 
/// @param byte the data to send to 
/// @return 
uint16_t SDCard_ExchangeByte(uint16_t byte){
    SDCard_WaitSPIFlag(SPI1, SPI_I2S_FLAG_TXE, FlagStatus::SET);
    SPI_I2S_SendData(SPI1, byte);
    SDCard_WaitSPIFlag(SPI1, SPI_I2S_FLAG_RXNE, FlagStatus::SET);
    SPI_I2S_ClearFlag(SPI1, SPI_I2S_FLAG_RXNE);
    uint16_t byte_res = SPI_I2S_ReceiveData(SPI1);  // start receive data when RX not Empty
    return byte_res;
}

/// @brief Send Command to SD card 
/// @param cmd 
/// @param param 
/// @param crc 
/// @bug may cause infinite loop 
uint16_t SDCard_SendCmd(uint8_t cmd, uint32_t param, uint8_t crc){
    uint8_t response;
    SDCard_Write_CS(1);
    Delay_ms(1);
    SDCard_Write_CS(0);  // ENABLE CS 
    while (true){
        uint8_t r = SDCard_ExchangeByte(0xFF);
        if (r == 0xFF) break;  // wait until SD card not busy
    }
    SDCard_ExchangeByte(cmd | 0x40);   // reset byte
    SDCard_ExchangeByte((uint8_t)(param >> 24));
    SDCard_ExchangeByte((uint8_t)(param >> 16));
    SDCard_ExchangeByte((uint8_t)(param >> 8));
    SDCard_ExchangeByte((uint8_t)(param));
    SDCard_ExchangeByte(crc);
    if (cmd == SDCard_CMD12) SDCard_ExchangeByte(0xFF);
	
    do{
        response = SDCard_ExchangeByte(0xFF);
    }while(response & 0x80); // wait until the highest bit is 0
		
    /* @bug : to be fixed, may cause infinite loop*/    
    SDCard_Write_CS(1);
    return response;
}

/// @brief Read several bytes from SD card, use data buff (at least of size length)
/// @note use 0xFF for exchange bytes 
/// @param data_buff 
/// @param length 
void SDCard_ReadBytes(uint8_t* &data_buff, uint32_t length){
    SDCard_Write_CS(0);
    for (uint8_t i = 0; i< length; i++) data_buff[i] = SDCard_ExchangeByte(0xFF); // throw the first 3 bits 
    SDCard_Write_CS(1);
}
```
其中定义的头函数文件如下
```cpp title:SDCard.h fold
#ifndef __SDCard_H
#define __SDCard_H

// SD card type
#define SDCard_TYPE_ERR     	    0x00
#define SDCard_TYPE_MMC				0x01
#define SDCard_TYPE_V1				0x02
#define SDCard_TYPE_V2				0x04
#define SDCard_TYPE_V2HC			0x06

// SD card Command 
#define SDCard_CMD0 0                                // SDCard Reset
#define SDCard_CMD1 1                                // MMC card wait 
#define SDCard_CMD8 8                                // SEND_IF_COND
#define SDCard_CMD9 9                                // read CSD data
#define SDCard_CMD10 10                              // read CID data
#define SDCard_CMD12 12                              // stop data transmission 
#define SDCard_CMD16 16                              // set the sector size 
#define SDCard_CMD17 17                              // read sector 
#define SDCard_CMD18 18                              // read multiple sectors 
#define SDCard_CMD23 23                              // erase sector before write 
#define SDCard_CMD24 24                              // write sector
#define SDCard_CMD25 25                              // write multiple sectors
#define SDCard_CMD41 41                              // return 0x00
#define SDCard_CMD55 55                              // return 0x01
#define SDCard_CMD58 58                              // read OCR Information 
#define SDCard_CMD59 59                              // Enalbe or disable CRC

/* SD Card Response Definition */
#define SDCard_RESPONSE_NO_ERROR               0x00
#define SDCard_RESPONSE_IN_IDLE_STATE          0x01
#define SDCard_RESPONSE_ERASE_RESET            0x02
#define SDCard_RESPONSE_ILLEGAL_COMMAND        0x04
#define SDCard_RESPONSE_COM_CRC_ERROR          0x08
#define SDCard_RESPONSE_ERASE_SEQUENCE_ERROR   0x10
#define SDCard_RESPONSE_ADDRESS_ERROR          0x20
#define SDCard_RESPONSE_PARAMETER_ERROR        0x40
#define SDCard_RESPONSE_FAILURE                0xFF

#define SDCard_RESPONSE_DATA_OK                0x05
#define SDCard_RESPONSE_DATA_CRC_ERROR         0x0B
#define SDCard_RESPONSE_DATA_WRITE_ERROR       0x0D
#define SDCard_RESPONSE_DATA_OTHER_ERROR       0xFF


typedef struct               /* Card Specific Data */
{
  uint8_t  CSDStruct;            /* CSD structure */
  uint8_t  SysSpecVersion;       /* System specification version */
  uint8_t  Reserved1;            /* Reserved */
  uint8_t  TAAC;                 /* Data read access-time 1 */
  uint8_t  NSAC;                 /* Data read access-time 2 in CLK cycles */
  uint8_t  MaxBusClkFrec;        /* Max. bus clock frequency */
  uint16_t CardComdClasses;      /* Card command classes */
  uint8_t  RdBlockLen;           /* Max. read data block length */
  uint8_t  PartBlockRead;        /* Partial blocks for read allowed */
  uint8_t  WrBlockMisalign;      /* Write block misalignment */
  uint8_t  RdBlockMisalign;      /* Read block misalignment */
  uint8_t  DSRImpl;              /* DSR implemented */
  uint8_t  Reserved2;            /* Reserved */
  uint32_t DeviceSize;           /* Device Size */
  uint8_t  MaxRdCurrentVDDMin;   /* Max. read current @ VDD min */
  uint8_t  MaxRdCurrentVDDMax;   /* Max. read current @ VDD max */
  uint8_t  MaxWrCurrentVDDMin;   /* Max. write current @ VDD min */
  uint8_t  MaxWrCurrentVDDMax;   /* Max. write current @ VDD max */
  uint8_t  DeviceSizeMul;        /* Device size multiplier */
  uint8_t  EraseGrSize;          /* Erase group size */
  uint8_t  EraseGrMul;           /* Erase group size multiplier */
  uint8_t  WrProtectGrSize;      /* Write protect group size */
  uint8_t  WrProtectGrEnable;    /* Write protect group enable */
  uint8_t  ManDeflECC;           /* Manufacturer default ECC */
  uint8_t  WrSpeedFact;          /* Write speed factor */
  uint8_t  MaxWrBlockLen;        /* Max. write data block length */
  uint8_t  WriteBlockPaPartial;  /* Partial blocks for write allowed */
  uint8_t  Reserved3;            /* Reserded */
  uint8_t  ContentProtectAppli;  /* Content protection application */
  uint8_t  FileFormatGrouop;     /* File format group */
  uint8_t  CopyFlag;             /* Copy flag (OTP) */
  uint8_t  PermWrProtect;        /* Permanent write protection */
  uint8_t  TempWrProtect;        /* Temporary write protection */
  uint8_t  FileFormat;           /* File Format */
  uint8_t  ECC;                  /* ECC code */
  uint8_t  CSD_CRC;              /* CSD CRC */
  uint8_t  Reserved4;            /* always 1*/
}MSD_CSD;

typedef struct				 /*Card Identification Data*/
{
  uint8_t  ManufacturerID;       /* ManufacturerID */
  uint16_t OEM_AppliID;          /* OEM/Application ID */
  uint32_t ProdName1;            /* Product Name part1 */
  uint8_t  ProdName2;            /* Product Name part2*/
  uint8_t  ProdRev;              /* Product Revision */
  uint32_t ProdSN;               /* Product Serial Number */
  uint8_t  Reserved1;            /* Reserved1 */
  uint16_t ManufactDate;         /* Manufacturing Date */
  uint8_t  CID_CRC;              /* CID CRC */
  uint8_t  Reserved2;            /* always 1 */
}MSD_CID;

typedef struct
{
  MSD_CSD CSD;
  MSD_CID CID;
  uint32_t Capacity;              /* Card Capacity */
  uint32_t BlockSize;             /* Card Block Size */
  uint16_t RCA;
  uint8_t CardType;
  uint32_t SpaceTotal;            /* Total space size in file system */
  uint32_t SpaceFree;      	      /* Free space size in file system */
} MSD_CARDINFO, *PMSD_CARDINFO;



#endif // !__SDCard_H

```

# 二、基于库函数的SD卡SPI模式通信开发 
### (1) 基本函数
在 StdPeriph_Lib 中， 可以找到STM32_EVAL库, 这个是非常常用的库(这个已经放在模板文件> Porting中)
![[attachments/Pasted image 20240421201519.png]]
移植其的stm32_eval.c , stm32_eval.h, 和两个spi_sd的相关文件并将其加入到工程中。 除此之外， 也把stm32f10b_eval.h也加进去;
![[attachments/Pasted image 20240421202407.png|500]]
但是之后我们会删除到只用一个spi_sd.c和.h文件进行SD卡读取和写入操作

修改头spi_sd.c文件中的Include ， 删除对stm32_eval.h的依赖，然后加入`#include  "stm32f10x.h"` , 如下图:
![[attachments/Pasted image 20240421205227.png]]
此时进行编译, 我们可以看到缺失大量宏定义, 这些是
![[attachments/Pasted image 20240421205230.png]]

此时我们在头文件中加入如下的宏定义即可:
```cpp title:相关引脚定义
#define SD_SPI                           SPI1
#define SD_SPI_CLK                       RCC_APB2Periph_SPI1
#define SD_CS_PIN                        GPIO_Pin_4                  /* PA.04 */
#define SD_CS_GPIO_PORT                  GPIOA                       /* GPIOA */
#define SD_CS_GPIO_CLK                   RCC_APB2Periph_GPIOA
#define SD_SPI_SCK_PIN                   GPIO_Pin_5                  /* PA.05 */
#define SD_SPI_SCK_GPIO_PORT             GPIOA                       /* GPIOA */
#define SD_SPI_SCK_GPIO_CLK              RCC_APB2Periph_GPIOA
#define SD_SPI_MISO_PIN                  GPIO_Pin_6                  /* PA.06 */
#define SD_SPI_MISO_GPIO_PORT            GPIOA                       /* GPIOA */
#define SD_SPI_MISO_GPIO_CLK             RCC_APB2Periph_GPIOA
#define SD_SPI_MOSI_PIN                  GPIO_Pin_7                  /* PA.07 */
#define SD_SPI_MOSI_GPIO_PORT            GPIOA                       /* GPIOA */
#define SD_SPI_MOSI_GPIO_CLK             RCC_APB2Periph_GPIOA

/* detect when the SD card is pluged in */
#define SD_DETECT_PIN                    GPIO_Pin_8                  /* PA.08 */
#define SD_DETECT_GPIO_PORT              GPIOA                       /* GPIOA */
#define SD_DETECT_GPIO_CLK               RCC_APB2Periph_GPIOA           
```

宏定义添加完成后， 还缺少两个函数
```cpp 
void SD_LowLevel_DeInit(void);
void SD_LowLevel_Init(void);
```
这两个在stm3210b_eval.h函数中有， 直接复制粘贴到spi_sd.h中即可;

其中Low_Level_Init 是初始化引脚和时钟的， 

更改源代码中 SD_LowLevel_Init 中的CPOL和 CPHA选项:
```cpp
SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; //  源代码为2Edge
```

SD_GetResponse 一般在命令中使用, 并代替了初始代码中的这一句:
```cpp
do{ 
	response = SDCard_ExchangeByte(0xFF);
}while(response & 0x80); // wait until the highest bit is 0
```
Read Byte 是发送0xFF读取数据的函数
WriteByte 会返回一个返回值, 写命令一般先用Write Byte, 再用GetResponse, 而等待退出使用 WriteByte; 或者发送0xFF时, SD_ReadByte; 

> [!NOTE] 说明
> SD_WriteByte和SD_ReadByte完全相同， 只是SD_ReadByte发送的是0xFF;

第三, 由于库函数中的初始化代码是给 MCC使用的， 所以没有加入CMD8的命令, 我们将其加上， 
```cpp
#define SD_CMD_SEND_IF_COND           8   /*!< CMD8 = 0x48 */
#define SD_CMD_APP_CMD                55  // return 0x01
#define SD_CMD_GEN_CMD                56  // DataBlock Operation
#define SD_CMD_READ_OCR               58  // read OCR Information
#define SD_CMD_CRC_ON_CMD             59  // Enalbe or disable CRC
```

另外， 还需要辛苦地修改原先的初始化代码;
此外,  相应的

### (2) FATFS 文件系统移植
http://elm-chan.org/fsw/ff/00index_e.html
[官方文档网址](http://elm-chan.org/docs/fat_e.html)
[CSDN移植教程](https://blog.csdn.net/qq_39778488/article/details/107129578)  [知乎移植教程](https://zhuanlan.zhihu.com/p/37897938)  [新版本CSDN移植教程](https://blog.csdn.net/Oushuwen/article/details/112469919)

[stm32f4移植示例](http://stm32f4-discovery.net/2014/07/library-21-read-sd-card-fatfs-stm32f4xx-devices/)

相较于以前的版本, FaTFS0.15版本删除了options 和 integer.h, 并且删除了以下的三个文件
![[attachments/Pasted image 20240422110734.png]]

将源码添加到工程文件中直接编译, 会出现很多错误如下:
![[attachments/Pasted image 20240422112328.png]]
上面这些函数就是FatFs接口函数，需要使用者去定义，所以这些函数没有定义。

除了把东西都注释掉以外,在 ff.h 的第360行有一个RTC函数, 是需要进行用户自定义的, 这是FatFs时间戳功能，通常需要MCU里的RTC时钟来支持，这里不使用时间戳。
```cpp title:ff.h360行
/* RTC function (provided by user) */
#if !FF_FS_READONLY && !FF_FS_NORTC
DWORD get_fattime (void);   /* Get current time */
#endif
```
修改：找到ffconf.h中的FF_FS_NORTC，将其改为1，如下：
```cpp title:ffconf.h
#define FF_FS_NORTC 1 
```
在disco.c文件中添加SD的宏定义， 然后修改对应的.c中的相应函数:
```cpp 
/* Definitions of physical drive number for each drive */
#define DEV_RAM     0   /* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC    1   /* Example: Map MMC card to physical drive 1 */
#define DEV_USB      2   /* Example: Map USB MSD  to physical drive 2 */
#define DEV_SD        3   /* Example: Map SD  card to physical drive 3  */   // 这个自己添加
// 共需要在对应的部分修改4个初始化接口函数, 略去
```

将ffconf.h文件中的FF_VOLUMES参数改成设备总的个数: 
```cpp title:假设此时定义了4个设备,则定义FF_VOLUMES为4
/*---------------------------------------------------------------------------/
/ Drive/Volume Configurations
/---------------------------------------------------------------------------*/

#define FF_VOLUMES		4
/* Number of volumes (logical drives) to be used. (1-10) */
```


另外需要修改ffconf.h文件下面几个定义, 其中FF_FS_RPATH是 FatFs相对路径的功能, USE_FIND是文件搜索, USE_MKFS是文件创建功能
```cpp
#define FF_FS_RPATH		1
/* This option configures support for relative path.
/   0: Disable relative path and remove related functions.
/   1: Enable relative path. f_chdir() and f_chdrive() are available.
/   2: f_getcwd() function is available in addition to 1.
*/
#define FF_USE_FIND		1
/* This option switches filtered directory read functions, f_findfirst() and
/  f_findnext(). (0:Disable, 1:Enable 2:Enable with matching altname[] too) */

#define FF_USE_MKFS		1
/* This option switches f_mkfs() function. (0:Disable or 1:Enable) */
```


然后, 根据上面提示的返回值，添加接口函数的定义, 例如 SD_disk_read 返回的是DRESULT对象, 则返回值也相应为DRESULT; 
```cpp title:SD_disk_read函数接口 fold
DRESULT SD_disk_read(BYTE* buff, LBA_t sectorm, UINT count);
/// @param prdv Physical drive nmuber to identify the drive */
/// @param buff Data buffer to store read data */
/// @param sector Start sector in LBA
/* Number of sectors to read */
DRESULT disk_read (BYTE pdrv,BYTE *buff,LBA_t sector,UINT count)
{
	DRESULT res;
	int result;

	switch (pdrv) {
		case DEV_SD :
			// translate the arguments here
			result = SD_disk_read(buff, sector, count);
			return res; 
	}
	return RES_PARERR;
}

DRESULT SD_disk_read(BYTE* buff, LBA_t sectorm, UINT count){
	return RES_OK;
}
```

在ff.c中, 3600行左右的 FATFS API 部分是常用的函数, 另外已经准备了一个用于移植SD卡的并把函数定义换成了习惯使用的部分在模板文件中。 
```cpp title:ff.c中的FATFS对象定义
static FATFS *FatFs[FF_VOLUMES];    /* Pointer to the filesystem objects (logical drives) */
static WORD Fsid;                   /* Filesystem mount ID */
```

```cpp
/* Get physical drive size */
disk_ioctl()  // 用于获取物理内存, 查看物理内存是否足够
```
### (2) f_mount原理
为了能够将文件系统绑定到 SD 卡(logical drive number = 3), 首先调用ff.c中的f_mount函数, 具体操作如下: 
```cpp
uint8_t res = f_mount(&fs, "3:", 1);
// 其中3是路径
```

在 f_mount 中会调用 mount_volume , 自动调用get_ldnumber获得vol=3, 然后判断这个 volume 是否**已经被挂载(mount)过**, 如果是加载过且没有开启写保护, 则返回FR_OK; 

如果是还没有被mount, 则会调用一次`stat = disk_initialize(fs->pdrv);  /* Initialize the volume hosting physical drive */` 判断能否初始化完成, 如果成功初始化, 则有可能会返回 NoSystem

### (3) 输入输出控制函数disk_ioControl
对于 `DRESULT SD_disk_ioctl(void);` 函数, 里面必须要根据Cmd进行定义许多函数的返回结果,其中ioctrl函数对应的命令已经在diskio.h中给出了定义, 具体如下:
```cpp
DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void* buff);
/* Command code for disk_ioctrl fucntion */
/* Generic command (Used by FatFs) */
#define CTRL_SYNC			0	/* Complete pending write process (needed at FF_FS_READONLY == 0) */
#define GET_SECTOR_COUNT	1	/* Get media size (needed at FF_USE_MKFS == 1) */
#define GET_SECTOR_SIZE		2	/* Get sector size (needed at FF_MAX_SS != FF_MIN_SS) */
#define GET_BLOCK_SIZE		3	/* Get erase block size (needed at FF_USE_MKFS == 1) */
#define CTRL_TRIM			4	/* Inform device that the data on the block of sectors is no longer used (needed at FF_USE_TRIM == 1) */

/* Generic command (Not used by FatFs) */
#define CTRL_POWER			5	/* Get/Set power status */
#define CTRL_LOCK			6	/* Lock/Unlock media removal */
#define CTRL_EJECT			7	/* Eject media */
#define CTRL_FORMAT			8	/* Create physical format on the media */

/* MMC/SDC specific ioctl command */
#define MMC_GET_TYPE		10	/* Get card type */
#define MMC_GET_CSD			11	/* Get CSD */
#define MMC_GET_CID			12	/* Get CID */
#define MMC_GET_OCR			13	/* Get OCR */
#define MMC_GET_SDSTAT		14	/* Get SD status */
#define ISDIO_READ			55	/* Read data form SD iSDIO register */
#define ISDIO_WRITE			56	/* Write data to SD iSDIO register */
#define ISDIO_MRITE			57	/* Masked write data to SD iSDIO register */
```

注意: 上面部分的 Generic Command 是一般都需要定义的, 而定义时要看后面给出的使用条件(例如FF_USE_MKFS == 1时需要GET_BLOCK_SIZE函数)

第二组部分和第三组部分都是不被FatFs使用的函数, 因此可以直接不定义。
