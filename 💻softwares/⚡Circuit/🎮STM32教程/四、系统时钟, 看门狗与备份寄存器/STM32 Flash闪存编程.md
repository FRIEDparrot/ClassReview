# 一、STM32 Flash 简介
在程序存储器中存储的程序， 可以保证掉电不丢失的特性。本工程主要**利用程序读写存储程序的存储器**。
包括
1. 利用内部的 Flash 空间来进行掉电不丢失的保存。
2. 在程序中， 直接读写芯片内存区域修改程序本身, 这个称为IAP(In- Application Programming), 有时也有OTA，
3. ICP(In-Circuit Programming -ICP) 通过JTAG, SWD更新程序, 或者使用Bootloader串口通信更新程序全部内容。 
4.  IAP 可以实现在程序后方不会覆盖的部分写一个引导程序，在主程序中可以通过程序进行下载更新。 

往往适用于少量参数可以直接存入芯片的情况。
首先需要一个Flag判断在存储之前是否存储过程序，如果没有存储过, 就将掉电数据存储进去。 

同时， 在某个位置可以直接读取到 stm32 的ID号，可以使用指针进行读取。 

Flash 包含**程序存储器，系统存储器， 选项字节三个部分**。 
一般<b><mark style="background: transparent; color: blue">将选项字节用于存储相关配置参数</mark></b> 
![[attachments/Pasted image 20240217101703.png]]

利用闪存存储器接口，可以**对程序存储器和选项字节进行擦除和编程**
参考手册p53, 选项字节(Option Bytes)具有16 K的空间

Flash Memory Interface registers 部分是用于控制 Main memory 部分的擦除和编程过程。 即==擦除和编程是通过读写下方Flash Memory Interface Registers来进行实现的==。
![[attachments/Pasted image 20240406164222.png|700]]
参考[[💻softwares/⚡Circuit/🎮STM32教程/三、串口与通信协议/4. SPI 通信协议与W25Q64 Flash芯片#(3) Flash 的写入时的注意事项|4. SPI 通信协议与W25Q64 Flash芯片Flash 的写入]]， 芯片的Flash和W25Q64的Flash具有相同的特性。

但是这里只有按照页来划分(由于0x3FF是1023, 共有1024个空间即1024byte)。(W25Q64是x000 - xFFF共4096, 4kb)

对于F103C8T6板, 最后一个页的起始地址为 0800 FC00, 选项字节可以配置程序的读写保护。

# Flash Memory 接口寄存器操作方法
### (1) Flash 解锁和加锁
类似于W25Q64的写使能操作。
在FPEC键中写入三个键值: 
RDPRT: 0x000000A5
KEY1  : 0x45670123
KEY2  : 0xCDEF89AB

解锁方法: 在FLASH_KEYR先写入KEY1, 再写入KEY2， 解锁
加锁方法: 在操作完毕之后， 需要尽快将Flash进行重新加锁， 防止意外产生。 加锁操作是设置FLASH_CR中的LOCK位， 写入1即可。 

### (2) 指针访问存储器
```cpp
#define      __IO   volatile  // 这个已经在库函数中定义了

uint16_t Data = *((__IO uint16_t *)(0x08000000)); // 读指定地址的寄存器
*((__IO uint)(0x08000000)) = 0x1234;                    // 写指定地址的寄存器
/*
注意: 后面的部分是地址值, 而首先将地址值转化为 volatile uint16_t* 指针类型(将存储器转换为指针再取值)
然后取这个指针的值并且赋值给Data 
*/
```

> [!NOTE] volatile 关键字
> volatile 是保证安全， 防止**编译器优化**。即Keil有时利用编译优化， 可以占据更小的空间(例如删除空循环代码)，而volatile 用于防止变量被优化掉。 同时编译器也会利用缓存加速代码。但是在多线程内, 由于中断修改原始变量可能不会导致高速寄存器中的部分修改， 导致程序修改不同步的问题
> volatile 是告诉编译器这个变量是易变的， 因此每一次更改必须执行到位， 不要使用缓存优化。
> **编译器优化的voilate部分使用**: 在==无意义加减变量， 多线程更改变量，读写与硬件相关寄存器==时, 都需要加上volatile, 防止编译器优化造成的丢失。 

优化编译的配置方法如下:
![[attachments/Pasted image 20240407000935.png]]

注意: 由于闪存Flash在==运行时是只读的， 不能够轻易更改==，必须解锁并进行(3)中的操作

### (3) 程序存储器写入过程
1. 写入
	- 注意: ==STM32 首先会自动检测指定地址有没有擦除， 如果没有擦除， 则自动不执行写入操作(除非写入全是0)==
	- 先解锁
	- 置FlASH_CR -> PG = 1 
	- 指定地址**写入半字(16位), 注意只能以半字的方法写入** 
	- 等待BSY直到为0
1. 页擦除(全部置为1)
	- 解锁(参考全擦除)
	- 置FLASH_CR-> PER  = 1, 并在FLASH_AR指定要擦除的页， 置 FLASH_CR -> START = 1;
	- 等待BSY 
2. 全擦除(所有位置为1) :  
	- 读取Lock位, 如果为1, 则需要解锁(写KEY1,KEY2), 也可以直接解锁
	- 置 FLASH_CR -> MER = 1(MER=1表示全擦除), 然后置 FLASH_CR -> START = 1
	- 等待 BSY = 1, 直到BSY = 0



选项字节部分如下图所示: 具体作用参考闪存编程手册;
![[attachments/Pasted image 20240406183841.png]]
擦除选项字节流程（闪存手册p17）
1. 检查FLASH_SR-> BSY 
2. 解锁FLASH_CR->OPTWRE (先解锁Lock , 再解锁FLASH_CR->OPTWRE , 方法是在FLASH_OPTKEYR中先写入KEY1,再写入KEY2)
3. 设置FLASH_CR->OPTER = 1
4. 设置FlASH_CR ->  START = 1
5. 等待BSY 
写入如下: 
![[attachments/Pasted image 20240406184552.png]]

另外需要注意: 任何读写闪存的操作将会使得CPU暂停， 直到闪存读写编程结束。往往使用定时器中断等等的部分，会导致中断无法处理而暂停。 
### (4) 电子签名
闪存容量寄存器: 
基地址:0x1fff f7e0  ,  16字节

UID寄存器: 
0x1fff f7e8, 96位


# 三、 Flash编程
### (1) 基本库函数讲解
其中, ErasePage是前后都有等待的策略， 因此一般会返回4表示FLASH_COMPLETE
```cpp
void FLASH_SetLatency(uint32_t FLASH_Latency);
void FLASH_HalfCycleAccessCmd(uint32_t FLASH_HalfCycleAccess);
void FLASH_PrefetchBufferCmd(uint32_t FLASH_PrefetchBuffer);

void FLASH_Unlock(void);   // 解锁Flash写入流程 -> 即将Key1,Key2发送到
void FLASH_Lock(void);      // 上锁Flash写入 
/*进行闪存和选项字节的函数*/
FLASH_Status FLASH_ErasePage(uint32_t Page_Address);   // 擦除页
FLASH_Status FLASH_EraseAllPages(void);                             // 全部擦除 -> 注意:返回FLASH_Status 表示: 
FLASH_Status FLASH_EraseOptionBytes(void);
FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data);   // whole word 
FLASH_Status FLASH_ProgramHalfWord(uint32_t Address, uint16_t Data); // half word 
FLASH_Status FLASH_ProgramOptionByteData(uint32_t Address, uint8_t Data); 
FLASH_Status FLASH_EnableWriteProtection(uint32_t FLASH_Pages); 
FLASH_Status FLASH_ReadOutProtection(FunctionalState NewState); 
FLASH_Status FLASH_UserOptionByteConfig(uint16_t OB_IWDG, uint16_t OB_STOP, uint16_t OB_STDBY);
uint32_t FLASH_GetUserOptionByte(void);  // 用于获取获取用户OptionByte 
uint32_t FLASH_GetWriteProtectionOptionByte(void);  // 
FlagStatus FLASH_GetReadOutProtectionStatus(void);
FlagStatus FLASH_GetPrefetchBufferStatus(void);   // 获取预缓存区状态
void FLASH_ITConfig(uint32_t FLASH_IT, FunctionalState NewState);   // 中断使能
FlagStatus FLASH_GetFlagStatus(uint32_t FLASH_FLAG);
void FLASH_ClearFlag(uint32_t FLASH_FLAG);
FLASH_Status FLASH_GetStatus(void);
FLASH_Status FLASH_WaitForLastOperation(uint32_t Timeout);  // 等待忙, 一般不使用

/*------------ New function used for all STM32F10x devices -----*/
void FLASH_UnlockBank1(void);
void FLASH_LockBank1(void);
FLASH_Status FLASH_EraseAllBank1Pages(void);
FLASH_Status FLASH_GetBank1Status(void);
FLASH_Status FLASH_WaitForLastBank1Operation(uint32_t Timeout);
```

```cpp title:FLASH_Status含义代表
typedef enum
{ 
  FLASH_BUSY,
  FLASH_ERROR_PG,
  FLASH_ERROR_WRP,
  FLASH_COMPLETE,
  FLASH_TIMEOUT
}FLASH_Status;
```

实际编程时， 注意进行擦除和写入操作时， 先Unlock再Lock即可。
```cpp
uint32_t MCU_FlashRead(uint32_t addr_){
    return (uint32_t)*((__IO uint32_t*)(addr_)); 
} 
// 对于不同的数据存储和读出, 只要强制转换为8位和16位数据再读出即可。(也可修改前面强转)

uint32_t MCU_FlashEraseAllPages(){
    FLASH_Unlock();
    FLASH_EraseAllPages(); // **note** if we do more precisely, check the return value 
    FLASH_Lock();
}

```


最后一页地址为0x0800FC00, 需要注意的是， 一个字节为一个地址， 一个半字占据两个地址， 这样我们就可以根据大小进行写入。
另外， 一页有1kb数据, 因此一次写入一个半字可以
```cpp title:将某一页全写入0xff 
for (uint16_t i = 0; i < 512; i++){
	ProgramHarfWord(0xffff,  addr + i * 2);// 省去Lock, Unlock
}
```
另外， 可以先传入数组 ， 然后再依次写入。 
![[attachments/Pasted image 20240407011526.png]]
另外在Keil 中可以配置内存大小如下:
![[attachments/Pasted image 20240407012857.png]]

```cpp title:闪存测试代码
#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#define LASTPAGE_ADDR 0x0800fc00

using namespace std;
/* Flash Code */

void MCU_FlashWrite(uint32_t addr_, uint16_t data_){
    FLASH_Unlock();
    FLASH_ProgramHalfWord(addr_, data_); // "ProgramWord" is entire word 
    FLASH_Lock();
}

/* since the library didn't provide the Read of the Flash*/
uint32_t MCU_FlashRead(uint32_t addr_){
    return (uint32_t)*((__IO uint32_t*)(addr_));  // note 
}

void MCU_FlashEraseAllPages(){
    FLASH_Unlock();
    FLASH_EraseAllPages(); // **note** if we do more precisely, check the return value 
    FLASH_Lock();
}

void MCU_FlashErasePage(uint32_t addr_){
    FLASH_Unlock();
    FLASH_ErasePage(addr_);
    FLASH_Lock();
}


int main(){
    OLED_Init();
    MCU_FlashWrite(LASTPAGE_ADDR, 0x1234);  // comment this and 
    while (1)
    {
        OLED_ShowString(1, 1, "Hello, world");
        OLED_ShowHexNum(2, 1, MCU_FlashRead(LASTPAGE_ADDR), 8);
    }
}
```

另外, 每一次编译完成之后， 也可以查看相应的程序大小。
![[attachments/Pasted image 20240407013025.png]]
其中, Total ROM Size 是程序占用 Flash 的大小，而 Total RW Size 是占用SRAM的大小 (本机是128kb RAM)
![[attachments/IMG_20240407_014037.jpg|500]]
更改编译的方法是在 Target 中更改IROM1为0x80000000 , 0x20000 (其中0x10000是64kb(65536), 而0x20000为128kb); 然后在VSCode 中重新Import; 
![[attachments/Pasted image 20240407014604.png]]
之后编译的 ROM 会变为128 kb 内存; 
![[attachments/Pasted image 20240407015134.png]]

```cpp
OLED_ShowHexNum(2, 1, MCU_FlashRead(0x1ffff7e0), 8);  // ROM Size
OLED_ShowHexNum(3, 1, MCU_FlashRead(0x1ffff7e8), 8);  // Chip UID
OLED_ShowHexNum(3, 9, MCU_FlashRead(0x1ffff7e8 + 0x04), 8);
OLED_ShowHexNum(4, 1, MCU_FlashRead(0x1ffff7e8 + 0x08), 8);
```
