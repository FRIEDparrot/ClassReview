LVGL 是国内最主流的常用图形库 , 可以使用UI编辑器SquarLine Studio 进行简化开发

最早的界面是MCGUI, 之后是emwin 部分对应(stmwin) 但是stmwin底层不开源(只提供lib库)
LVGL 是完全开源免费的 GUI 系统. 

## 一、示例程序运行方式
Unknow Target Connected 解决方法: 
1. 首先进行STLink 固件升级
注意使用STLink Utility 中的STLINK-> Upgrade, Keil 自带的版本太低了, 可以升级到J37
![[attachments/Pasted image 20240404162951.png]]

2.  修改配置 
首先将Device改为STM32F103C8, 然后找到Debug > 改为STLink Debugger > 并设置Reset And Run 
![[attachments/Pasted image 20240404115000.png]]
然后进入Settings > Debug , 将Port 由JTAG 更改为SW
![[attachments/Pasted image 20240404115127.png|800]]
然后即可正常进行测试。 测试效果如下:
![[attachments/IMG_20240404_115602.jpg|500]]
## 二、LVGL简介
LVGL全称为 Light and Versatile  Graphics Library, 提供轻量级, 并提供了LVGL相关的模拟器 
<mark style="background: transparent; color: red">SquareLine Studio 可以快速搭建优秀的界面</mark>，是非常常用的LVGL图形构建库

LVGL 提供了文本框， 按钮， Slide, 表格,  菜单等等。 

最低移植需求为16MHz时钟, >64kb 内存(推荐256-512kb), 

原理: 只需要一个简单驱动程序函数就可以将像素阵列移植到显示屏中进行显示。 
下面的部分均可以进行LVGL移植: 
![[attachments/Pasted image 20240406001907.png]]
lvgl 支持CJK 写系统， 即支持中文输入;

## 三、 基于Visual Studio 的LVGL模拟器环境配置
原始项目github地址为 https://github.com/lvgl/lv_port_pc_visual_studio
按照README 获取 lv_port_pc_visual_studio 项目
```git
git clone --recursive https://github.com/lvgl/lv_port_pc_visual_studio.git
```

然后打开LvglWindowsSimulator下的 LvglWindowsSimulator.vcxproj 文件, 注意切换为x64平台
![[attachments/Pasted image 20240406014237.png]]
设置为当前工程并编译。

对于 visual Studio 的文件, LVGL库函数的源码在  lv_port_win_visual_studio\LvglPlatform\lvgl\demos  中

其中, **examples, src文件夹 和lv_conf_template.h , lvgl.h 是移植 lvgl 关键性的文件夹**。并使用demo 进行测试使用。 

emamples 是对应的显示屏和LVGL输入设备输入文件, 而scripts是与micropython相关的文件部分。
其中, lv_conf_template.h 是lvgl的裁剪文件，即裁剪其中实际开发不需要使用的功能。 
而 lvgl.h 是对应包含的头文件。 

examples 和src 文件夹中的内容如下, 其中porting 表示输入设备驱动,  文件系统驱动和显示器驱动。 
![[attachments/Pasted image 20240407164920.png]]
src 主要是源码,  解码库和


## ▶️ Get started
This list will guide you to get started with LVGL step-by-step.
**Get Familiar with LVGL** 
1. Check the [Online demos](https://lvgl.io/demos) to see LVGL in action (3 minutes)
2. Read the [Introduction](https://docs.lvgl.io/master/intro/index.html) page of the documentation (5 minutes)
3. Get familiar with the basics on the [Quick overview](https://docs.lvgl.io/master/get-started/quick-overview.html) page (15 minutes)
**Start to Use LVGL**
4. Set up a [Simulator](https://docs.lvgl.io/master/integration/ide/pc-simulator.html#simulator) (10 minutes)
5. Try out some [Examples](https://github.com/lvgl/lvgl/tree/master/examples)
6. Port LVGL to a board. See the [Porting](https://docs.lvgl.io/master/porting/index.html) guide or check the ready to use [Projects](https://github.com/lvgl?q=lv_port_)
**Become a Pro** 
7. Read the [Overview](https://docs.lvgl.io/master/overview/index.html) page to get a better understanding of the library (2-3 hours)
8. Check the documentation of the [Widgets](https://docs.lvgl.io/master/widgets/index.html) to see their features and usage
**Get Help and Help Others**
9. If you have questions go to the [Forum](http://forum.lvgl.io/)
10. Read the [Contributing](https://docs.lvgl.io/master/CONTRIBUTING.html) guide to see how you can help to improve LVGL (15 minutes)

# LVGL 移植基本方法
参考[blog.csdn.net/qq_59953808/article/details/126445608](https://blog.csdn.net/qq_59953808/article/details/126445608) 
首先， 新建工程时添加 lvgl 文件夹, 并复制粘贴这四个文件: 
![[attachments/Pasted image 20240408005546.png]]

把 lv_conf_template.h文件重命名为lv_conf.h
在lvgl/examples/porting文件夹中把三个.h和.c文件开头的#if 0改为#if 1; 
lv_port_disp_template.c 注释掉后面的两个示例部分;
![[attachments/Pasted image 20240408020419.png]]
在lv_conf.h 的include 后面加:
```cpp title:屏幕大小定义代码
#define MY_DISP_HOR_RES 160
#define MY_DISP_VER_RES  128
```

lvgl 中: 加入的文件夹有: 
1. 标红的全部加上子路径加入lvgl中 (stdlib除了micropython都加)
![[attachments/Pasted image 20240408015323.png]]
2. Driver  > Display > 选择自己的型号，我的是st7789驱动
![[attachments/Pasted image 20240408015413.png]]
3. libs > bin_decoder > lv_bin_decoder.c
![[attachments/Pasted image 20240408015517.png]]
4. osal > lv_os_none.c  选操作系统的 (定义了lv_draw_sw_mask)
![[attachments/Pasted image 20240408015548.png]]
添加到 include 路径的部分只有 lvgl, lvgl/src, lvgl/examples/porting 即可; 

不希望输出警告可以按照如图的方法进行: 
![[attachments/Pasted image 20240408020619.png]]

然后在工程下新建st7789并且包含如下的代码, 做好相应配置即可: 
![[attachments/Pasted image 20240408021420.png]]
对于 lv_anim 的 identifier "INT32_MIN" is undefined,  需要在对应文件下添加
```cpp
#include <limits.h>
// 将INT32_MIN更改为  INT_MIN, 即-2147483648
INT_MIN 
```
此时， 由于LED_font.h 数组过大导致内存RAM溢出， 此时, 我们可以取消include oled_font.h 并注释掉 ShowChar 和 ShowHanzi 
# 四、LVGL轻量化部分
### (1) 文件的添加(修正部分)
先将LVGL_conf.h 中的 if 0 更改为1
为了减少相应的内存占用， lvgl> examples 中,除了 porting 以外， 其余都可以删除

一般在**文件管理中，在工程下分别建立UI，其下面建立GUI文件夹和GUIAPP文件夹**
**而GUI下一般放入lvgl文件夹, GUIAPP用于存储相应的应用的UI界面文件**

此时可以使用如下的形式进行声明
```cpp
#include  ../../lvgl.h
```

工程文件中可以使用如下的形式:
![[attachments/Pasted image 20240410001422.png]]
1. core 保持全部添加, 并加上 lv_init 文件
2. draw路径下: 仅添加 c 文件和 sw 下的文件部分(sw中二级文件夹不用添加)
![[attachments/Pasted image 20240410002119.png]]
3. 使用 themes 文件存储layout, themes, bin > decoder 的文件 
4. 使用 diplay 文件夹
5. 使用 widgets 文件夹存储

注意: 通过add source folder > normal folder 可以简化操作流程 
最终简化后添加的文件夹如图所示 (其中需要单独使用lvgl_init和lvgl_port两个部分):
![[attachments/Pasted image 20240410011254.png]]
其中, core, display 全部添加, draw包含如下所示， 除了sw 的部分和内部 blend 部分, 其余部分全部不用添加; 
![[attachments/Pasted image 20240410011600.png]]
另外stdlib 中禁用micropython部分和 rtthread 部分

移植过程中， 主要修改的是porting部分, 其他基本不使用。 lv_disp已经指定了路径。

通过在 Builder Options 中设置 C/C++ Compiler > --cpp11 即可支持 c++11 语法了
![[attachments/Pasted image 20240410022623.png|900]]


### (2) 工程文件的选用和修改
#### 1. LVGL 时基单元修改 
首先在 oled 自带文件中 写一个1ms的定时器中断, 然后新增include "lvgl.h", 
添加函数`lv_tick_inc();` 这里暂时使用在delay_ms中, 每一次中断调用, 具体timer2.cpp 代码如下

```cpp title:timer2.cpp fold
#include "timer2.h"
#include "lvgl.h"

void TIMER2_INIT(){
	// use APB1 bus Clock  
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM2EN, ENABLE);

	// firsly choose the internal clock -> use internal clock for drive 
	TIM_InternalClockConfig(TIM2);  // choose internal clock (not necessary -> default)
	
	TIM_TimeBaseInitTypeDef* TIM2Base_InitStruct = new TIM_TimeBaseInitTypeDef();
	TIM2Base_InitStruct->TIM_ClockDivision = TIM_CKD_DIV1; // actually is 0
	TIM2Base_InitStruct->TIM_CounterMode = TIM_CounterMode_Up; // counter in upper mode 
	TIM2Base_InitStruct->TIM_Prescaler = 100 - 1;        // prescaler
	TIM2Base_InitStruct->TIM_Period = 720 - 1; 		     // Auto-Reload Register set
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
            lv_tick_inc(1);
		}
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

```

在主函数最前面调用如下代码, 保证 timer2 被打开: 
```cpp 
TIMER2_INIT();
TIMER2_START();
Lcd_Init();
```

`````ad-seealso
title: 补充
collapse: close
- 在计时器或任务中每 `x` 毫秒调用一次 [lv_tick_inc](https://lvgl.100ask.net/9.0/API/tick/lv_tick.html#_CPPv411lv_tick_inc8uint32_t "lv_tick_inc")(x) (`x` 应该在 1 到 10 之间)。 LVGL 的内部时序需要它。或者，使用 [`lv_tick_set_cb()`](https://lvgl.100ask.net/9.0/API/tick/lv_tick.html#_CPPv414lv_tick_set_cb16lv_tick_get_cb_t "lv_tick_set_cb") 注册 `tick_get_cb` ，以便LVGL可以直接检索当前时间。

也可以使用系统时钟的 Handler, 做法是直接修改 stm32xxxx_it.c 文件中的 SysTick_Handler 即可
```cpp
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

    HAL_SYSTICK_IRQHandler();
    lv_tick_inc(1);
    #ifdef USE_RTOS_SYSTICK
      osSystickHandler();
    #endif

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}
```

`````

然后修改oled.h 文件, 保证其与 cpp 兼容,  这样include "oled.h不会报错"
```cpp 
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
} /*extern "C"*/
#endif
```

#### 2. 修改lv_port_fs_templ 文件
```cpp fold title:原始的lv_port_fs_templ.c代码部分
/**
 * @file lv_port_fs_templ.c
 *
 */

/*Copy this file as "lv_port_fs.c" and set this value to "1" to enable content*/
#if 1
/*********************
 *      INCLUDES
 *********************/
#include "../../lvgl.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void fs_init(void);

static void * fs_open(lv_fs_drv_t * drv, const char * path, lv_fs_mode_t mode);
static lv_fs_res_t fs_close(lv_fs_drv_t * drv, void * file_p);
static lv_fs_res_t fs_read(lv_fs_drv_t * drv, void * file_p, void * buf, uint32_t btr, uint32_t * br);
static lv_fs_res_t fs_write(lv_fs_drv_t * drv, void * file_p, const void * buf, uint32_t btw, uint32_t * bw);
static lv_fs_res_t fs_seek(lv_fs_drv_t * drv, void * file_p, uint32_t pos, lv_fs_whence_t whence);
static lv_fs_res_t fs_size(lv_fs_drv_t * drv, void * file_p, uint32_t * size_p);
static lv_fs_res_t fs_tell(lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p);

static void * fs_dir_open(lv_fs_drv_t * drv, const char * path);
static lv_fs_res_t fs_dir_read(lv_fs_drv_t * drv, void * rddir_p, char * fn, uint32_t fn_len);
static lv_fs_res_t fs_dir_close(lv_fs_drv_t * drv, void * rddir_p);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_fs_init(void)
{
    /*----------------------------------------------------
     * Initialize your storage device and File System
     * -------------------------------------------------*/
    fs_init();

    /*---------------------------------------------------
     * Register the file system interface in LVGL
     *--------------------------------------------------*/

    static lv_fs_drv_t fs_drv;
    lv_fs_drv_init(&fs_drv);

    /*Set up fields...*/
    fs_drv.letter = 'P';
    fs_drv.open_cb = fs_open;
    fs_drv.close_cb = fs_close;
    fs_drv.read_cb = fs_read;
    fs_drv.write_cb = fs_write;
    fs_drv.seek_cb = fs_seek;
    fs_drv.tell_cb = fs_tell;

    fs_drv.dir_close_cb = fs_dir_close;
    fs_drv.dir_open_cb = fs_dir_open;
    fs_drv.dir_read_cb = fs_dir_read;

    lv_fs_drv_register(&fs_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your Storage device and File system.*/
static void fs_init(void)
{
    /*E.g. for FatFS initialize the SD card and FatFS itself*/

    /*You code here*/
}

/**
 * Open a file
 * @param drv       pointer to a driver where this function belongs
 * @param path      path to the file beginning with the driver letter (e.g. S:/folder/file.txt)
 * @param mode      read: FS_MODE_RD, write: FS_MODE_WR, both: FS_MODE_RD | FS_MODE_WR
 * @return          a file descriptor or NULL on error
 */
static void * fs_open(lv_fs_drv_t * drv, const char * path, lv_fs_mode_t mode)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    void * f = NULL;

    if(mode == LV_FS_MODE_WR) {
        /*Open a file for write*/
        // f = ...         /*Add your code here*/
    }
    else if(mode == LV_FS_MODE_RD) {
        /*Open a file for read*/
        // f = ...         /*Add your code here*/
    }
    else if(mode == (LV_FS_MODE_WR | LV_FS_MODE_RD)) {
        /*Open a file for read and write*/
        // f = ...         /*Add your code here*/
    }

    return f;
}

/**
 * Close an opened file
 * @param drv       pointer to a driver where this function belongs
 * @param file_p    pointer to a file_t variable. (opened with fs_open)
 * @return          LV_FS_RES_OK: no error or  any error from @lv_fs_res_t enum
 */
static lv_fs_res_t fs_close(lv_fs_drv_t * drv, void * file_p)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /*Add your code here*/

    return res;
}

/**
 * Read data from an opened file
 * @param drv       pointer to a driver where this function belongs
 * @param file_p    pointer to a file_t variable.
 * @param buf       pointer to a memory block where to store the read data
 * @param btr       number of Bytes To Read
 * @param br        the real number of read bytes (Byte Read)
 * @return          LV_FS_RES_OK: no error or  any error from @lv_fs_res_t enum
 */
static lv_fs_res_t fs_read(lv_fs_drv_t * drv, void * file_p, void * buf, uint32_t btr, uint32_t * br)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /*Add your code here*/

    return res;
}

/**
 * Write into a file
 * @param drv       pointer to a driver where this function belongs
 * @param file_p    pointer to a file_t variable
 * @param buf       pointer to a buffer with the bytes to write
 * @param btw       Bytes To Write
 * @param bw        the number of real written bytes (Bytes Written). NULL if unused.
 * @return          LV_FS_RES_OK: no error or  any error from @lv_fs_res_t enum
 */
static lv_fs_res_t fs_write(lv_fs_drv_t * drv, void * file_p, const void * buf, uint32_t btw, uint32_t * bw)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /*Add your code here*/

    return res;
}

/**
 * Set the read write pointer. Also expand the file size if necessary.
 * @param drv       pointer to a driver where this function belongs
 * @param file_p    pointer to a file_t variable. (opened with fs_open )
 * @param pos       the new position of read write pointer
 * @param whence    tells from where to interpret the `pos`. See @lv_fs_whence_t
 * @return          LV_FS_RES_OK: no error or  any error from @lv_fs_res_t enum
 */
static lv_fs_res_t fs_seek(lv_fs_drv_t * drv, void * file_p, uint32_t pos, lv_fs_whence_t whence)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /*Add your code here*/

    return res;
}
/**
 * Give the position of the read write pointer
 * @param drv       pointer to a driver where this function belongs
 * @param file_p    pointer to a file_t variable
 * @param pos_p     pointer to store the result
 * @return          LV_FS_RES_OK: no error or  any error from @lv_fs_res_t enum
 */
static lv_fs_res_t fs_tell(lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /*Add your code here*/

    return res;
}

/**
 * Initialize a 'lv_fs_dir_t' variable for directory reading
 * @param drv       pointer to a driver where this function belongs
 * @param path      path to a directory
 * @return          pointer to the directory read descriptor or NULL on error
 */
static void * fs_dir_open(lv_fs_drv_t * drv, const char * path)
{
    void * dir = NULL;
    /*Add your code here*/
    // dir = ...           /*Add your code here*/
    return dir;
}

/**
 * Read the next filename form a directory.
 * The name of the directories will begin with '/'
 * @param drv       pointer to a driver where this function belongs
 * @param rddir_p   pointer to an initialized 'lv_fs_dir_t' variable
 * @param fn        pointer to a buffer to store the filename
 * @param fn_len    length of the buffer to store the filename
 * @return          LV_FS_RES_OK: no error or  any error from @lv_fs_res_t enum
 */
static lv_fs_res_t fs_dir_read(lv_fs_drv_t * drv, void * rddir_p, char * fn, uint32_t fn_len)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /*Add your code here*/

    return res;
}

/**
 * Close the directory reading
 * @param drv       pointer to a driver where this function belongs
 * @param rddir_p   pointer to an initialized 'lv_fs_dir_t' variable
 * @return          LV_FS_RES_OK: no error or  any error from @lv_fs_res_t enum
 */
static lv_fs_res_t fs_dir_close(lv_fs_drv_t * drv, void * rddir_p)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /*Add your code here*/

    return res;
}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
```

对于 lv_port_Indev_template.c 文件,  其中是有 touchpad_init 等为触摸屏部分提供的选项。 
首先， 如果使用触摸屏， 则include "touch.h", 
然后触摸屏有的话需要使用 lv_port_indev_init的方法; 中, 修改tochpad_init();

另外还需要注册鼠标输入和键盘输入设备, 编码器设备等等。以及按钮(button_init())中的部分。
另外， static bool touchpad_is_pressed等等是检测触摸屏是否被按下的扫描函数。
如果是不需要使用触摸屏， 可以直接不包含 lv_port_indev_template


### 3. RAM 和 ROM 不足的问题解决方法

第一、裁剪RAM的使用， 首先在 lv_config 文件中,  更改自己的内存使用, 其中LV_MEM_SIZE 是占用运行内存， 建议使用10kb (c8t6是20kb的RAM, 然后用一点作为缓冲) 
```cpp title:更改lv_conf.h,设置最大运行内存的方式
#define LV_MEM_SIZE (6 * 1024U)          /*[bytes]*/
```


> [!NOTE] 注意
> 一般这个值不能太小, 如果设置到4kb以及以下往往会出现显示不出的问题并且报错。

第二,  修改 startup_stm32f10x_md.s中最大堆栈的数量: 
```cpp 
Stack_Size      EQU     0x00000400 // 800就够
```

第三、在 lv_config 文件中的480行左右， 禁用不需要的部分， 例如animating,  只需要设置为0;
![[attachments/Pasted image 20240411151638.png]]
禁用第120行的复杂渲染工具， 可以节省20-30k左右的ROM内存;
![[attachments/Pasted image 20240411152542.png]]

```cpp title:120行
/*The target buffer size for simple layer chunks.*/
#define LV_DRAW_SW_LAYER_SIMPLE_BUF_SIZE    (20 * 1024)   /*[bytes]*/
```

```
// lv_color_t buf[BUF_W * BUF_H];
// lv_color_t * buf_p = buf;
// void test(){
//     uint16_t x, y;
//     for(y = 0; y < BUF_H; y++) {
//         lv_color_t c = lv_color_mix(RED, YELLOW, (y * 255) / BUF_H);
//         for(x = 0; x < BUF_W; x++){
//             (*buf_p) =  c;
//             buf_p++;
//         }
//     }
//     lv_area_t a;
//     a.x1 = 10;
//     a.y1 = 40;
//     a.x2 = a.x1 + BUF_W - 1;
//     a.y2 = a.y1 + BUF_H - 1;
//     disp_flush(NULL, &a, buf);
// }
```

由于240 * 240 比较大， 初始化1/10部分的RAM是不够的， 所以我们只使用左上的160 * 160 空间进行测试;

## 五、LVGL移植基本过程和遇到的问题

基本的移植颜色转换函数: 
```cpp 
uint16_t colordata = 0x0000;
colordata |= (uint16_t)(color_p->red) << 11;
colordata |= (uint16_t)(color_p->green)<< 6;
colordata |= (uint16_t)(color_p->blue);
// 上式四句和以下的一句等价 
uint16_t colordata = lv_color_to_u16(*color_p);
```


对于STM32, 颜色深度应当设置为16位， 否则会出现花屏现象(虽然在lv_conf.h中): 
```cpp
// 1. lv_conf.h中, 22行部分注意设置为16色深
/*Color depth: 8 (A8), 16 (RGB565), 24 (RGB888), 32 (XRGB8888)*/
#define LV_COLOR_DEPTH 16
```

> [!caution] 核心问题
> 在display.h文件中， 我们可以看到如下的定义代码: 从下面代码可以看出, px_map 是uint8_t类型的， 因此我们在定义lv_flush_cb_t 对应的myflash函数时, 也应当考虑到数组是8位的问题。

```cpp 
typedef void (*lv_display_flush_cb_t)(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map);
typedef void (*lv_display_flush_wait_cb_t)(lv_display_t * disp);
```

正确的flush代码如下:
```cpp

```