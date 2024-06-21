## (1) SU-03T管脚定义
![[attachments/Pasted image 20240503214728.png]]
在使用时, SU-03T的部分, SPK+, SPK-接入喇叭, M-, M+ 接入咪头, 另外还需要接入CH340的USB转串口的模块;

### 2. SU-03T烧录注意事项
在进行烧录时， 必须是**串口TXD接入B6, RXD接入B7(不要错搞成B0, B1)**, <mark style="background: transparent; color: red">按下烧录键时必须断电</mark>(拔掉5V 线)然后上电(接上5V线)自动烧录。
![[attachments/Pasted image 20240502193458.png|600]]
具体接线图如下图所示(只需要CH340转串口部分):
![[attachments/IMG_20240502_195405.jpg|400]]
在烧录时一定要断电, 点击烧录之后上电, 下载完成如图所示:
![[attachments/Pasted image 20240502202040.png|700]]

## (2) SU-03T Linux 二次开发环境搭建
### 1. 基本虚拟机安装
参考 SU03T二次开发环境搭建配套文档（外部版本)
首先， 参考[VMWare安装教程](https://blog.csdn.net/weixin_52799373/article/details/124324077) 在电脑上安装VMWare虚拟机 (Ubuntu参考 [Ubuntu配置](https://blog.csdn.net/Javachichi/article/details/131572766)) 注意: 我配置的是18版本, 20,22版本的相同;

然后参考 [跨虚拟机拖拽文件](https://blog.csdn.net/yakamoz423/article/details/128817915) 并使用nano编辑器删除WayLandEnable前面的注释, ctrl + O保存
```shell
# 先安装open-vm-tools
sudo apt install open-vm-tools
sudo nano /etc/gdm3/custom.conf   # 使用gedit 也可以
# 然后删除下面的WayLandEnable前面的注释, 保证是false
```
在 Settings 中,使用Shared Folders, 设置为 Always Enabled 
![[attachments/Pasted image 20240504095757.png|700]]
最后， 一定要安装open-vm-desktop这一选项,
```shell 
sudo apt-get install open-vm-tools-Desktop
# 然后重启
reboot
```
然后就可以跨虚拟机移动和复制粘贴文件了
![[attachments/Pasted image 20240504100057.png]]

我们把对应的 Andestech.tart.gz 放到 /opt 文件夹下, 然后解压:
```shell
sudo tar -xzvf Andestech.tart.gz
```
解压完毕后保留Andestech文件夹, 如下图:
![[attachments/Pasted image 20240504102256.png]]

### 2. VMAwre 中系统目录和用户目录的访问
在文件夹中, Home默认是用户目录， 并使用 ~ 代替, 例如 ls -l ~/opt 是返回用户目录下的内容
![[attachments/Pasted image 20240504123036.png|700]]
但是我们需要使用系统的目录, 方法是打开Other locations > Computer, 在这里的才是根目录, 需要将Andestech交叉编译文件夹放置到这里并配置环境变量
![[attachments/Pasted image 20240504123227.png]]
我们可以使用sudo权限, 例如**将用户下的opt/Andestech移动到系统的opt/Andestech**:
```
sudo mv ~/opt/Andestech /opt/
```
然后在系统的opt文件夹里面就有Andestech了, 此时再输入 /opt/Andestech, 提示Is a Directory,配置成功!
![[attachments/Pasted image 20240504123720.png]]
### 3. 使用vim配置环境变量
```
sudo apt-get install vim # 安装vim编辑器 
sudo vi /etc/profile         # 使用vim打开
03、按住键盘Shift+i使文本可以编辑：按Enter进行换行，在文档最后添加如下语句：               

（（I 为插入的意、思），此时鼠标在第一行的第一个位置；shift+a 和 shift+i 的区别是：i 为光标前插入，a 为光标后插入）
```

![[attachments/Pasted image 20240504102635.png]]
加入这一句:
```
export PATH=$PATH:/opt/Andestech/BSPv422/toolchains/nds32le-elf-mculib-v3s/bin
```
然后更新etc/profile:
```
source /etc/profile  
是一个 shell 命令，用于重新加载系统环境变量文件 /etc/profile，以便使修改后的环境变量生效。
在 Linux 系统中，环境变量是指在整个系统中都可以使用的全局变量，用于存储一些常用的路径、命令别名、默认设置等。环境变量文件 /etc/profile 是系统级别的环境变量配置文件，它包含了系统全局的环境变量定义，通常被所有的登录用户所共享。
当我们需要修改系统级别的环境变量时，可以通过编辑 /etc/profile 文件来进行修改。但是，修改后的环境变量并不会立即生效，需要使用 source 命令重新加载 /etc/profile 文件，以便使修改后的环境变量立即生效。
```

### 3. 获取root权限并安装make
```cpp
su  root # 使用root权限 -> 报错 , sudo passwd root 输入两次密码
su root   # su friedparrot 切换回来
```
最终获得了root权限如下图:
![[attachments/Pasted image 20240504105816.png]]

安装make如下:
```
sudo apt-get install gcc automake autoconf libtool make  # 安装make 
```

### 4. 编译sdk
首先在桌面等随便位置新建一个su03t文件夹。 因为前面环境变量已经配置好了
所以直接在智能公元上下载SDK, 最终cd到unione_lite_app_hb_m文件夹
![[attachments/Pasted image 20240504104319.png|800]]
编译指令如下:
```shell
./build.sh update
```
编译成功:
![[attachments/Pasted image 20240504124519.png]]
最后在自己创建的文件夹中, 打开可以看到编译生成的output文件夹， 里面就有对应的bin文件了, 编译成功!
![[attachments/Pasted image 20240504124705.png]]

由于我们是在Linux虚拟机上开发SU-03T, 所以需要下载vscode来作为开发工具。

为了能够在linux虚拟机上有更好的编译体验, 可以在Linux机上的.vscode下找到tasks.json，然后添加如下的config:
```json title:用tasks.json代替 ./build.sh update 
{
	"label": "Build with build.sh",
	"type": "shell",
	"command": "./build.sh",
	"args": ["update"],
	"group": {
		"kind": "build",
		"isDefault": true
	},
	"presentation": {
		"reveal": "always",
		"panel": "shared"
	}
}
```

**使用ctrl + shift + B 进行构建**(我还配置了f8作为快捷键)

如果随便添加一点错误代码（例如在main.c前面稍微加一点）, 则显然能够得到报错:
Building file: ../middleware/rtos/rtos_api/rtos_api.c
Building file: ../src/app/src/main.c
../src/app/src/main.c:57:1: error: expected '=', ',', ';', 'asm' or '__attribute__' before 'static'

## (3) 基于SU-03T的二次开发
对于初始的SDK的部分, 其编译好的bin文件在output文件夹中(release update bin是通过B6, B7烧录的文件)。
对于SU-03T的二次开发, 宏定义在 user/inc/user_config.h 中
```cpp title:GPIO宏定义
/** @defgroup uni_config_gpio_macro GPIO相关宏定义
GPIO settings
@{*/
#define GPIO_OUT_DEF_VAL      1       ///< 0: default is low, 1: default is high, when GPIO set to output mode
/** @}*/
```

对于二次开发所用的相关手册, 均已经在开发指导手册中给出，具体如下:
![[attachments/Pasted image 20240601114420.png]]
对于.h文件不高亮的问题,在右下角更改文件类型， 选择.c或者.cpp文件即可
![[attachments/Pasted image 20240601114635.png]]
基本结构在user > inc 文件下,如下图结构所示, 包括user定义的文件和.h文件等等:
![[attachments/Pasted image 20240601115016.png]]

对于SU-03T提供了很多面向用户的demo文件, 且最初默认是AUTO_GPIO的DEMO文件
```cpp 
#define USER_RUN_DEMO_SELECT  USER_DEMO_AUTO_GPIO ///< select a demo to run
```

Demo使用方法：
1.要使能Demo功能，先将 USER_RUN_DEMO 设置为1  
2.再通过配置USER_RUN_DEMO_SELECT 定义为需要展示的示例程序宏  
3.重新编译固件，烧录后即可体验demo运行效果

具体的示例文件位置是 user/src/examples 

基本的库函数都在src/hal/inc中, 包括 uni_hal_audio_player.h 和 uni_hal_audio.h, 以及uni_hal_decoder.h

对于源文件的main.c中, 包含了如下的user部分的头文件:
```cpp
#include "user_config.h"
#include "user_main.h"
#include "user_player.h"
```

```cpp title:除此以外包含的相关文件
#include "uni_iot.h"
#include "uni_log.h"
#include "uni_timer.h"
#include "uni_event_route.h"
#include "uni_event_list.h"
#include "uni_recog_service.h"
#include "uni_media_player.h"
#include "uni_session_manage.h"
#include "uni_wakeup_session.h"
#include "uni_setting_session.h"
#include "uni_watchdog_session.h"
#include "uni_user_meeting.h"
#include "uni_black_board.h"
#include "idle_detect.h"
#include "uni_version.h"
#include "uni_record_save.h"
#include "uni_config.h"
#include "uni_hal_reset.h"
#include "uni_hal_uartcmd.h"
#include "uni_pcm_default.h"
#include "uni_cust_config.h"
#include "uni_hal_audio.h"
#include "uni_hal_uart_upgrade.h"
#include "uni_hal_factory.h"
```

### 2. 软件架构认识
对于SU-03T而言, 其主要的软件架构如图所示:
![[Excalidraw/SU-03T语音识别控制模块 2024-06-01 16.18.31|950]]
为了能够编写一个main函数, 首先在SU-03T的main.h中包含头文件
```cpp
#include "unione.h"
```
这个一般在user_main.h中就已经写好了,语音识别功能是在 src -> sdk -> vui 实现的;

而一个重要的文件夹是 src > utils， 这个层提供常用的工具集代码，包括如：事件队列、Ring Buffer、List、状态机、Hash运算、Json解析等，供SDK及应用层使用； 包含了 unisound 层的所有逻辑， 里面的各个层都有一个 inc和一个src文件夹, 包含了include头文件和源代码文件; 

在 user_uart.h 中， 会有`#include "uni_uart.h"` 
修改unione.h中的include的文件， 即可在user层引用, 而我们只需要写 User 层以上的文件, 首先新建一个函数, 而运算函数可以从uni_ 前缀进行获得 
```cpp 
#include "user_config.h"
#include "user_event.h"
#include "user_gpio.h"
#include "user_player.h"
#include "user_pwm.h"
#include "user_timer.h"
#include "user_uart.h"
```

在Uni_log.h中包含了不同的Log定义(D,T,W,E,R):
```cpp
#define LOGD(tag, fmt, ...) LOG(N_LOG_DEBUG, tag, fmt, ##__VA_ARGS__)
#define LOGT(tag, fmt, ...) LOG(N_LOG_TRACK, tag, fmt, ##__VA_ARGS__)
#define LOGW(tag, fmt, ...) LOG(N_LOG_WARN, tag, fmt, ##__VA_ARGS__)
#define LOGE(tag, fmt, ...) LOG(N_LOG_ERROR, tag, fmt, ##__VA_ARGS__)
#define LOGR(tag, fmt, ...) LOG(N_LOG_RAW, tag, fmt, ##__VA_ARGS__)
```
在app层接口中(src/app), 


对于唤醒词操作, 是存储在 inc > uni_nlu_content.h 中的, 并有如下定义:
```cpp
typedef struct {
  uni_u32 key_word_hash_code; /* 存放识别词汇对应的hashcode */
  uni_u8  nlu_content_str_index; /* 存放nlu映射表中的索引，实现多个识别词汇可对应同一个nlu，暂支持256条，如果不够换u16 */
  char    *hash_collision_orginal_str /* 类似Java String equal，当hash发生碰撞时，赋值为识别词汇，否则设置为NULL */;
} uni_nlu_content_mapping_t;

// 一部分简单的对应关系定义如下:
const char* const g_nlu_content_str[] = {
[eCMD_wakeup_uni] = "{\"asr\":\"小智小智\",\"cmd\":\"wakeup_uni\",\"pcm\":\"[103, 104]\"}",
[eCMD_LearnMatchAC] = "{\"asr\":\"匹配空调\",\"cmd\":\"LearnMatchAC\",\"pcm\":\"[107, 108, 109, 110, 111]\"}",
[eCMD_NightLampOn] = "{\"asr\":\"打开小夜灯\",\"cmd\":\"NightLampOn\",\"pcm\":\"[112]\"}"
}
```

### 2. GPIO 复用定义
具体的GPIO定义如下, 复用功能已经给出;
```cpp
/** @brief GPIO号*/
typedef enum {
  GPIO_NUM_A25 = 0,   ///<可用于SPI-MISO / ADC / DMIC-DATA
  GPIO_NUM_A26,       ///<可用于SPI-CLK / ADC / DMIC-CLK
  GPIO_NUM_A27,       ///<可用于SPI-MOSI / PWM / ADC
  GPIO_NUM_A28,       ///<已占用，PA使能控制（喇叭功放静音）
  GPIO_NUM_B0,        ///<已占用，SW-CLK（烧录器接口）
  GPIO_NUM_B1,        ///<已占用，SW-DATA（烧录器接口）
  GPIO_NUM_B2,        ///<可用于UART1-TX / PWM / I2C-SCL
  GPIO_NUM_B3,        ///<可用于UART1-RX / PWM / I2C-SDA
  GPIO_NUM_B6,        ///<UART1-RX（外设串口通信接收脚），不使能UART时可用做GPIO
  GPIO_NUM_B7,        ///<UART1-TX（外设串口通信发送脚），不使能UART是可用做GPIO
  GPIO_NUM_B8,        ///<已占用，虚拟Software UART-TX（Log输出引脚，波特率115200）
  GPIO_NUM_MAX
}GPIO_NUMBER;
```

### (3) 语音识别原理详解 
对于语音控制教程, 参考 hb_asr_control.c, 对应的语音控制接口在 user_asr.h 中; 具体函数只有几个, 如下:
```cpp fold title:语音识别相关指令
/**
* 进入后将只会识别唤醒词
*@brief 进入睡眠状态
*@retval 0  操作成功
*/
int user_asr_goto_sleep(void);
/**
* 进入后将只会识别命令词
*@brief 进入唤醒状态
*@retval 0  操作成功
*/
int user_asr_goto_awakend(void);

/**
* mic开始接收数据
*@brief 使能语音识别
*@retval 0  操作成功
*/
int user_asr_recognize_enable(void);

/**
* mic被关闭，停止拾音
*@brief 关闭语音识别
*@retval 0  操作成功
*/
int user_asr_recognize_disable(void);

/**
* 启用识别词
*@brief 启用识别词
*@param word 要启用的识别词
*@retval 0  操作成功
*/
int user_asr_word_enable(const char *word);

/**
* 禁用识别词
*@brief 禁用识别词
*@param word 要禁用的识别词
*@retval 0  操作成功
*/
int user_asr_word_disable(const char *word);

/**
* 禁止超时自动退出识别模式
*@brief 禁止超时自动退出识别模式
*@retval 0  操作成功
*/
int user_asr_timeout_disable(void);

/**
* 使能超时自动退出识别模式
*@brief 使能超时自动退出识别模式
*@retval 0  操作成功
*/
int user_asr_timeout_enable(void);
```

对于语音识别线程, 其基本的创建方法如下:
```cpp
uni_pthread_create(&pid, &param, _hb_asr_control_task, NULL);
```

其中， `_hb_asr_control_task` 是函数句柄, 并且在其中添加 while(1) 函数作为主线程来进行
```cpp
static void _hb_asr_control_task(void *args) {
  while (1) {
    //进入睡眠模式，此时将只能通过唤醒词唤醒，说命令词无效
    user_asr_goto_sleep();
    LOGT(TAG, "Now is into sleep mode, you can awake it");
    uni_sleep(10);//延迟10秒
    //进入唤醒模式，此时可以识别命令词
    user_asr_goto_awakend();
    LOGT(TAG, "Now is into awake mode, you can say cmd word ");
    uni_sleep(10);
    //仍然在唤醒模式，执行关闭语音控制
    user_asr_recognize_disable();
    LOGT(TAG, "asr recognize is disabled, nothing will be recongnized");
    uni_sleep(10);
    //重新打开语音控制
    user_asr_recognize_enable();
    LOGT(TAG, "asr recognize is enabled");
    //延迟10秒
    uni_sleep(10);
  }
}
```

uni_hash 是哈希函数, 
Blacklist 是用于设置免唤醒即可使用的词语, 如果词语在 BlackList中, 则可以直接唤醒, 具体可以使用word_enable 和 word_disable 来进行实现;
```
int user_asr_word_enable(const char *word) {
  return uni_user_meeting_remove_asr_blacklist(word);
}

int user_asr_word_disable(const char *word) {
  return uni_user_meeting_append_asr_blacklist(word);
}
```

对于最终的语音识别函数, 是通过观察 uni_recog_control.c 中的 ENGINE_EVENT_START 事件来进行实现的, 并使用:
```cpp 
rc = _engine_start_internal(param->vui, param->scene->asr_mode,
							param->scene->timeout);

VuiRecognStart
```

对于每个语音识别服务, 使用的是 recog_service 系统, 而其参数如下
```cpp
static struct {
  VuiHandle     *vui;
  Scene         *cur_scene;
  uni_s32       twice_judge_last_time;
  uni_bool      is_work;
} g_recog_service; 
```



找到sevice.msc, 然后重启VMWare net service 
在uni_log.c 中的 LogInitialize 函数keyi 
uname -a


在Unisound中使用BlackBoard来进行引脚的详细操作
```cpp title:BlackBoard机制讲解
/**
 * write key:value into black board
 * @return return write success or fail
 * @retval 0 presents success, -1 presents failed
*/
int BbWrite(BLACK_BOARD_KEY key, int value);

/**
 * read value with of key from black board
 * @return return read success or fail
 * @retval 0 presents success, -1 presents failed
*/
int BbRead(BLACK_BOARD_KEY key, int *value);

/**
 * init black board
 * @return return init success or fail
 * @retval 0 presents success, -1 presents failed
*/
int BbInit(void);

/**
 * destroy black board
 * @return
 * @retval 0 presents success, -1 presents failed
*/
void BbFinal(void);
```

## (4) SU-03T 二次开发程序编写
在函数程序编写之前, 主要了解两个缩写:
asr (automatic speech recognition) 自动语音识别 
vui (voice  user interface) 语音用户界面  

UDP平台上用户定义脚本中的回复语列表，在 tools /scripts/pcm_map.txt 中可以找到对应关系, 对应的部分可以在User_event .h中找到, 

主要的两个定义语音文件和回复语的部分均在 super_cmd_map.txt 和 pcm_map.txt 中, 且回复语必须在scrips/tones 中有对应的语音wav文件, 输入部分在 input.txt 中有相应的定义;

```python
"""
1. PCM文件命名规则:
    所有文件PCM名字必须以纯数字命名。
    1 ~ 100 预留给系统特殊用途的PCM文件
    101 ~ 用命名配置产生的PCM文件。是根据配置文件cmd_content_total生成的PCM文件

2. cmd_content_total文件规则
    2.1 格式
        "="之前是命令名称，之后是命令词；@之后是回复语，没有回复语时也没有‘@’字符；
        多条命令词用"|"隔开；多条回复语用"|"隔开。

        命令    命令词        回复语（可无，没有回复语时也没有‘@’字符）
        xxx_cmd=命令词1|命令词2@回复语1|回复语2
        xxx_cmd=命令词1|命令词2
    2.2
        cmd_content_total文件应包含所有命令词。
        以”uni“为后缀的命令词是系统默认处理的命令词。
        asr_timeout_uni是虚拟命令词，用于关联识别超时回复用
"""
```


### (1) 延时函数和串口打印调试方法
延时函数如下: 需要说明的是, 在延时过程中, 仍然可以调用语音进行唤醒等操作。
```cpp title:延时函数
uni_sleep() ;    // 秒延时函数
uni_msleep();
uni_usleep();
```

```cpp title:UART常用User层函数
user_uart_init(_user_uart_recv cb_recv);   // 传入的参数是回调函数的句柄 
user_uart_send(buf,sizeof(buf));
```

```cpp
// 其中, 核心还是编写一个 _hb_uart_send_task 函数, 然后在while(1) 中进行回调
uni_pthread_create(&pid, &param, _hb_uart_send_task, NULL);

/* 接收到串口数据后，该接口将被触发回调 */
static void _hb_uart_recv(char *buf, int len); // 这个是回调函数的编写

static void _hb_uart_send_task(void *args); // 这个是主thread的函数编写 
```

典型的串口打印办法:
```cpp 
static void _hb_uart_send_task(void *args) {
  char buf[6] = {1, 2, 3, 4, 5, 6};
  int ret;

  /* 每隔2秒发送一次 */
  while (1) {
    LOGT(TAG, "hb uart send[1, 2, 3, 4, 5, 6]");
    ret = user_uart_send(buf, sizeof(buf));
    LOGT(TAG, "hb uart send done, ret=%d", ret);
    uni_msleep(3000);
  }
}
```
注意工程路径, 因此需要找到 home/friedparrot/(从这个往上一级, 不在Desktop之下, 目前已经将正确的工程移动到Desktop下)
![[attachments/Pasted image 20240602004210.png]] 
调试成功的串口函数显示如下:
![[attachments/Pasted image 20240602004638.png]] 

### (2) 简单的GPIO控制
#### 1. 基本函数与返回值获取方法
基本的函数如下所示: 
```cpp
user_gpio_get_value
int user_gpio_set_value(GPIO_NUMBER num, int val);
int user_gpio_get_value(GPIO_NUMBER num);
int user_gpio_interrupt_enable(void);   // GPIO 中断使能
int user_gpio_interrupt_disable(void);
int user_gpio_clear_interrupt(void);
int user_sw_timer_gpio_pulse(void);  // 输出定时脉冲
// 示例代码 
user_gpio_set_value(GPIO_NUM_B0, 0);

```

首先, 事件的控制和通过事件控制语音是通过 user_subscribe_event 来进行
```cpp
static void _register_event_callback(void) {
  user_event_subscribe_event(USER_CUSTOM_SETTING, _custom_setting_cb);
}

```

> [!NOTE] 函数指针与返回值的定义
> 需要说明的是, subscribe_event 中传入 `_custom_setting_cb` 的函数类型以及返回值查看方法: 参考[[📘ClassNotes/⌨️Programming/🌳Data Structure/C++基础知识/4. 函数指针的使用|4. 函数指针的使用]], 则有:

```cpp title:.h文件中的函数指针定义
int user_event_subscribe_event(USER_EVENT_TYPE event,_user_event_cb cb);  // 第二个是用于回调函数的函数指针
// 其中_user_event_cb 如下:
/** @brief 事件回调函数，当用户订阅事件发生时会被调用
* @param event 事件类型
* @param context 事件信息
*/
typedef void (*_user_event_cb)(USER_EVENT_TYPE event,
                                    user_event_context_t *context);
// 返回值为 void,  传入两个event 和 context 参数
```
一般而言, 在程序中可能会传入空指针, 如果不使用回调直接传入NULL即可;

user_event.h 中列出了主要的可定义的事件对象:
```cpp
int user_event_subscribe_event(USER_EVENT_TYPE event,_user_event_cb cb);  // 第二个是用于回调函数的函数指针
int user_event_clear_observers(void);  // 清空事件订阅队列 

// 可以订阅的事件类型如下: 
USER_AUDIO_PLAY_START,  ///< 音频开始播放时发送
USER_AUDIO_PLAY_END,    ///< 音频播放完毕或被停止时发送
USER_CUSTOM_SETTING,    ///< 识别到客户自定义的识别词时发送
USER_VOLUME_SETTING,    ///< 识别到音量调节指令时发送
USER_GOTO_SLEEPING,     ///< 进入待唤醒状态时发送
USER_GOTO_AWAKENED,     ///< 进入识别状态（已唤醒）时发送
```

```cpp
/** @brief 对应USER_CUSTOM_SETTING事件*/
typedef struct {
  char            *cmd;         ///< 命令词意图，对应UDP平台上用户定义脚本中的action
  char            *word_str;    ///< 识别到的命令词
  char            *reply_files; ///< 回复语列表[1, 2, 3]，对应UDP平台上用户定义脚本中的回复语列表，在pcm_map.txt中可以找到对应关系
}event_custom_setting_t;
```

#### 2. 实现自己增加一个程序并实现功能
自己实现程序,在user/src 下新建一个user_files, 用于存储自己的工程
首先修改 user_config.h 增加宏定义
```cpp
#define USER_DEMO_TEST1       18      /// user demo added by user 
#define USER_RUN_DEMO_SELECT  USER_DEMO_TEST1 ///< select a demo to run 
```
然后在自己定义的 test1_tutorial 中使用如下代码, 其中 user_config 和 user_event 必须使用;

```cpp 
#include "user_config.h"
#include "user_event.h"
#include "user_uart.h"
#include "user_gpio.h"
#include "user_asr.h"   // automatic speech recognition
#include "user_player.h"
#include "user_pwm.h"

int test1_tutorial(void){

}
```

修改 user_main.h 中的部分
```cpp 
extern int test1_tutorial(void); // 放在上面

// 下面加上18的判断
#elif (USER_RUN_DEMO_SELECT == USER_DEMO_TEST1)
  return test1_tutorial();
```

最后一步是转到 build 中的 makefile, 里面包含了工程路径的部分: 
![[attachments/Pasted image 20240603111020.png]]
其中的 user/src/examples/subdir.mk 为
```
-include user/src/examples/subdir.mk
```

首先需要修改 build/sources.mk 为工程中包含的文件 

```cmake title:source.mk文件添加(包含头文件)
INCLUDE_DIR    +=../user/src/user_files
# SUBDIRS :=  后面加上
user/src/user_files
```
修改makefile文件如下:
```cmake title:makefile文件添加
-include user/src/user_files/subdir.mk
```
在build > user 下创建 user_files > subdir.mk, 并在其中加入代码:

> [!caution] 注意构建的书写事项 
> 其中C_SRCS是源文件路径， <mark style="background: transparent; color: red">必须使用两个点..</mark>, 表示的是初始文件夹下的内容
> 而OBJS和 C_DEPS 是 build 下的文件路径, <mark style="background: transparent; color: red">前面使用一个点.</mark> ,表示build下对应生成的东西
> 后面的 $(CROSS_COMPILE)gcc $(CCFLAGS) -MF 之类是指定编译规则, 保证编译能够正常进行

如果出现`No rule to make target 'user/src/user_files/test1_tutorial.c', needed by 'uni_app.adx'`类似的错误， 一般是拼写错误了, 此时检查拼写和每个后缀名 
```CMake title:添加的正确的subdir.mk文件书写如下
################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# added by FriedParrot 
# Add inputs and outputs from these tool invocations to the build variables 

C_SRCS += \
../user/src/user_files/test1_tutorial.c

OBJS += \
./user/src/user_files/test1_tutorial.o

C_DEPS += \
./user/src/user_files/test1_tutorial.d

# Each subdirectory must supply rules for building sources it contributes  
user/src/user_files/%.o: ../user/src/user_files/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
```

> [!NOTE] 说明
> 之后在其中添加路径时, 也需要对应修改build/.../user_files 下对应的subdir.mk文件

在makefile中会自动包含对应的部分: 
![[attachments/Pasted image 20240603132247.png]]

makefile 编写参考 [makefile 教程](https://blog.csdn.net/ruglcc/article/details/7814546/) 

使用如下串口调试程序进行测试即可
```cpp title:test1_tutorial.c
#include "user_event.h"
#include "user_gpio.h"
#include "user_asr.h"   // automatic speech recognition
#include "user_uart.h"
// #include "user_player.h"
// #include "user_pwm.h"

#define TAG "test1_tutorial"

// note 
static void _hb_uart_recv(char* buf, int len){
    uni_msleep(500);  // wait for receive data 
    user_uart_send(buf,sizeof(buf));
} 

int test1_tutorial(void){
    // it is triggered when receive the recognition word 
    int res = user_uart_init(_hb_uart_recv);
    if (res!=0) { // check if initialize succeesfully 
        return -1;
    }
    user_event_subscribe_event(USER_CUSTOM_SETTING,_hb_asr_function);  // subscribe the event 
    
    user_asr_word_disable("退下");
    user_asr_word_disable("再见");
}
```

对于简单的gpio控制, 均需要使用 uni_strcmp() 函数，基本示例代码如下(注意: 其中BH, BL为指令符号, 并且在编译固件时生成, 在scripts文件夹下的txt文件有描述):
```cpp
static void _hb_asr_function(USER_EVENT_TYPE *event, 
                            user_event_context_t *context){
    int res = user_uart_send("receive the message successfully",33);
    if (context){
        event_custom_setting_t *setting = &context->custom_setting;
        if (uni_strcmp(setting, "BH") == 0){
        }
        elif(uni_strcmp(setting, "BL") == 0){
        }
        user_player_reply_list_num(setting->reply_files, 2);
    }
}
```

### (2) 自定义播放文件内容
为了能够回复自定义语句, 逻辑等等, 需要调用 语音控制部分, SU-03T底层的语音系统是通过播放 .mp3 文件进行的, 具体文件在 tones 中, 而通过调用 user_player 相关函数可以实现直接的语音回复以及某个确定文件的语音回复。

主要的demo在 user_player.c  和 user_player.h 中,
```cpp title:user_player具体用法的示例代码
user_player_set_volume_min();
LOGT(TAG, "set min vol");
/*播放9.mp3文件*/
user_player_play(AUDIO_PLAY_REPLY, "9"); //9.mp3
uni_sleep(10);

user_player_set_volume_mid();
LOGT(TAG, "set mid vol");
/*播放列表中的101.MP3文件*/
user_player_reply_list_num("[9,101,102]", 1);
uni_sleep(10);

user_player_set_volume_max();
LOGT(TAG, "set max vol");
/*随机播放列表中的某个文件*/
user_player_reply_list_random("[9,101,102]");
uni_sleep(10);

/*顺序播放列表中的所有文件*/
user_player_reply_list_in_order("[9,101,102]");
uni_sleep(10);
```

需要注意的是, 使用 user_player_reply_list_in_order 会创建一个线程, 一般地， 如果是在命令事件注册中想要得到相应回复， 则必须<mark style="background: transparent; color: red">在命令的回调函数中使用如下代码</mark> 
```cpp 
user_player_reply_list_random(setting->reply_files) // 参考 autogpio 的demo 
// 通过这个可以指定回复的
```
否则得到的命令将不会得到回复。


## 语音资源文件使用构建说明 
### (1) 相关文件 
根据 build.sh文件, 构建时只需先生成对应的 scripts, 
```shell title:build.sh 
cp -raf user/src/examples/res_housekeeper/* tools/scripts/
cp -f user/src/examples/inc_housekeeper/* include/
```


即首先将在res源文件下的tones部分(存储mp3回复语), 以及sucp_cmd_map.txt文件和两个json, 两个.dat文件
![[attachments/Pasted image 20240605175638.png]] 
然后将头文件拷贝到include路径下即可
其中 aik_release.json 文件存储了对应的唤醒语

在其中,tools/scripts/asrifx.dat 为语音支持 .dat 文件, 其中的部分
```shell 
cp tools/scripts/asrfix_en.dat tools/scripts/asrfix.dat
cp tools/scripts/asrfix_cn.dat tools/scripts/asrfix.dat
```


此外就是存储语法的文件 grammar.dat, 这个文件用于实现。


- 指令的阈值配置文件名为aik_debug.json或aik_release.json，前者对应的是debug固件，修改后者对应的是release固件。 当设备类型为“通用”时，文件路径为tools/scripts下；当设备类型为“智能语音红外控制器”时，文件路径为user/src/examples/res_housekeeper下；其他品类的阈值配置文件请到user/src/examples/res_xxx路径下对应修改。


语音播放有一个特点， 就是后面播放的语音会中断正在播放的语音， 包括在基础

www.aimachip.com

信息中设置的回复语播放， 所以， 如果一个行为中有多个语音播放控制， 则需要在后面播放的语音控制中设置延时播放， 否则只会听到一条语音播放。

