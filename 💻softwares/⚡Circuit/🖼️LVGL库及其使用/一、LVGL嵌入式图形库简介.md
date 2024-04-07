LVGL 是国内最主流的常用图形库 , 可以使用UI编辑器SquarLine Studio 进行简化开发


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