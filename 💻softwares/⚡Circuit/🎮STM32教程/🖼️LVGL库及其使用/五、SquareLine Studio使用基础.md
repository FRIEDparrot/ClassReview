注意: SquareLine 个人版最多使用 150Widgets, 多了需要Upgrade License
## 一、基本内容
### (1) 基础控件      
在画布上任意处右键可以选择选中的部分层级 
主要的面板包括Widgets, Hierarchy, Inspector 部分
注意: Hierarchy 中, parent 层级的属性变化也会导致child 发生相应的变化
在Inspector 中用于编辑属性, 可以添加自定义的样式(styles)和事件(events)

位置属性中, 可以设置px或者百分比的显示方法(需要注意的是%是按照parent计算得到的), 而为了在一般的屏幕大小中也能够适配, 往往采用 Percent 作为布局的依据。
![[attachments/Pasted image 20240606002749.png]]
类似Unity, 也使用了Assets面板作为控件的部分, 最大支持 2028 x 2028 大小的png图片。

在lvgl中, 所有的部分均使用了一个共同的 theme 来决定其 style, 同时可以单独改变每个组件的style. 
例如, 设置一个 slide bar 的样式时, 对应的gradient选项启用时, 则其颜色会有家变显示。

### (2) 事件的响应设置
首先打开State状态栏, 在未点击任何一种State时, 按钮是一种状态; 然后点击 Pressed State, 则自动切换到 Pressed 状态。
![[attachments/Pasted image 20240606004848.png|800]]

此时, 在Style Settings下方启动 Pressed 状态, 
![[attachments/Pasted image 20240606005228.png|800]]
注意:增加发光效果的方法是 Add Shadow, 而只需要设置一个浅色的阴影即可。在Play Mode 下仍然可以设置对应的Style属性。

event 用于例如在屏幕间进行切换操作时的操作。通过new screen 可以新建屏幕, 实现在不同屏幕间的切换效果。
![[attachments/Pasted image 20240606010141.png]]
对应地在Events 下方可以选择事件的类型, 例如call function等等。点击添加即可得到相应的回调函数属性了。
![[attachments/Pasted image 20240606010511.png]]
初始的事件在 Screens > Initial_action 中进行设置

### (3) 动画属性的配置
首先在 animation 面板中, 首先通过 animation name 进行add animation. 
注意animation仅显示最新创建的动画, 

注意relative一般是从0开始的, 表示这个值初始时和默认值相同。
对于Dleay部分,通过设置 Time, Delay 实现按顺序播放动画。
![[attachments/Pasted image 20240606102024.png|850]]

对于 Animation 的播放, 可以将其绑定到 event, 方法是Event > Play Animation 
需要说明没有搞懂之前尽量少使用relative, 否则可能出现错误。

### (4) 导出设置
首先, 从 File > Project Settings 中, 设置 Board Properties, 以及对应的 UI export Path等等, 相应的文件包括 Project Template 和 UI Files 两部分, 需要分别设置导出路径。
![[attachments/Pasted image 20240606105711.png]]
首先需要创建模板工程(Template Project), 然后Export UI files
![[attachments/Pasted image 20240606105711.png]]
注意修改 lv_config.h 使得对应部分支持相应的功能。

对于SquareLine不支持的相关设备, 可以仅导出UI Files, 这些是相对于工程独立的 UI 和代码相关文件。(实际上是对应的UI控制代码文件,不需要附加其他依赖项)此时我们可以将lvgl源码直接导入对应的工程中, 在 MCUXpresso IDE 中可以直接Import examples from SDK 

首先, 在原本的工程中新建一个UI Folder, 然后将 UI Files 导出到 UI Folder 中, 最后, 对于在初始示例代码中的 lv_demo_widgets()文件注释掉
包含 `#include ../ui/ui.h`, 然后调用函数  ui_init(); 
即可直接运行相应的代码, 获得对应的结果文件。(注意:SquareLine仅支持8.3.11以下版本,而最新的1.4.1增加了对lvgl9.1的支持)

### (3) Open Board Platform(OBP)
对于任何开发板, 需要有相应支持的情况下, 我们需要保证能获取一个支持其运行的工程模板。

可以通过template project 和json description file来直接将工程粘贴到SquareLineStudio文件夹下,从而可以快速获取到相应的， 直接的工程文件模板。

根据 SquareLine 的 [OpenBoard Platform 的相关文档](https://docs.squareline.io/docs/obp),  对应的文件在  SquareLine_Studio/boards 文件夹下 , 并且包括 .zip,.png和.slb 文件 
![[attachments/Pasted image 20240606123338.png]]
在png, slb和zip文件下, 需要入下的文件:
![[attachments/Pasted image 20240606125017.png]]
首先需要在自行创建的zip文件夹中必须有 ui 文件夹, 并删除原本的lvgl文件夹(自动会在导出时加入)
然后在stm32等的文件中加上include "ui/ui.h"

然后在.project中 [具体参考](https://www.youtube.com/watch?v=8D03rIzTz38)
最后在 ui 文件夹中加入README.md

slb 文件的创建方法: 使用json文件, 并且在 squareline 上有示例
最后修改group, title,keywords,"lvgl_export_path" 和"lvgl_include_path"
pattern_match_files 用于修改 tag 之后替换的文件(.project, 只用于修改相应的文件名)
然后修改 "supported lvgl version"部分, 得到最终的文件。

此时, 选择export ui和export project templates, 即可实现整个工程模板导出。

## 二、基础界面设计方法
### (1) Flex Layout 界面设计实现滚动菜单
对于每一个 Widget 对象, 可以通过 Layout > Flex 确定其横向分布或者纵向分布, 
![[attachments/Pasted image 20240606144632.png]]
通过wrap选项可以实现类似菜单的布局方案。
![[attachments/Pasted image 20240606144913.png]]
相应地, 使用Flex Layout 布局时, 往往采用Paddings 中修改Pad Row和Pad Column来修改间距。
注意其中可以使用fr为单位(free space), 但是注意, 修改其中一个对象时其余会连着修改。

对于其中的某个子项, 可以选中 Ignore Layout,此时中间的元素可以任意移动。
对于任何一个Text部件, 可以选中 To be translated  选项, 可以通过按钮设置调用翻译选项来切换语言设置。
![[attachments/Pasted image 20240606154617.png]]

### (2) Components 组件的设计
首先, Component 组件实际上是一系列 Widgets 的集合。通过在父对象上点击 Create Component 来实现组件的创建。

Components 是一个独立的组件, 并且已经保存在了项目的 Components 文件夹中, 非常容易在工程间进行复制粘贴。 

Create, Edit 和 Detach 方法可以创建, 修改和删除 Components (Detach用于将Components还原成组件), 另外其属性也可以进行单独自定义修改。


### (3) Object Naming Option 
可以通过Object Naming Option来实现给每个种类的 Widgets 加上任意的 prefix, 
这种方法有利于管理 Hierarchy 面板以及 Screens 面板

方法: 通过 File >  Project Settings >  Object Naming 可以更改其中的命名选项。
![[attachments/Pasted image 20240608090304.png]]
此时, 在Name下面显示区域即可得到对应的效果
![[attachments/Pasted image 20240608090620.png]]
对于 Property 也会自动加上相应的内容。

## 三、主题设置和快速切换方法
### (1) 全局定义颜色主题设置
全局定义颜色主题是在 Themes 中进行的， 同时也兼顾主题的切换工作。
下图显示了如何添加一个全局颜色 Light Blue 的教程,  
![[attachments/Pasted image 20240608094302.png|350]]
之后即可从Style 中直接选取 Light Blue, 方便颜色的选取过程。
![[attachments/Pasted image 20240608094442.png]]

### (2) 字体管理器的设置
字体管理器方法如下:
![[attachments/Pasted image 20240608101545.png|450]]
在新增字体时, 必须添加 .ttf(或者.otf)文件, 可以从 [常见网络字体库](https://www.iconfont.cn/fonts/index?spm=a313x.fonts_index.i1.2.6e133a81lsQO6G&type=3), https://www.fontsquirrel.com/fonts/list/popular 上进行下载获取到 .ttf 文件, 以 OpenSans-Regular 为例, 会自动生成一个bin和.fcfg文件。

bpp 是决定使用的 texture Filter 的部分(建议至少设置到4)
Range决定了 ascii 码对应范围, 而当symbol 有定义时, 则仅修改对应字符的字体
![[attachments/Pasted image 20240608103824.png]]
对应的部分只需在 Text Font 中修改即可。

对于DropDown, 如果想要设置其在选择时的属性, 则采用State > Checked 来进行设置。
indicator 是 Dropdown 中的小箭头符号。

## 四、琐碎组件
### (1) 键盘的设置方案
键盘有多种 Module， 其中包括 TEXT LOWER, TEXT UPPER 和NUMBER 等等。 

KeyBoard 主要的两组键,当使用Default时改变的是其中一组键的状态, **选用 Checked 状态时, 修改的是另外一组键的状态**。这样就能分别定义两组键的状态了。
![[attachments/Pasted image 20240608111733.png]]

对应的输入文本框是Text Area类型, 并且可以在 Target textarea 中进行设置。 
需要注意的是, 如果需要多个文本框同时编辑(一个键盘链接不同的文本框通过聚焦书写), 则需要调用Event进行设置。
屏幕1 Event > Focused > Keyboard set target > 设置Keyboard 和Textrea 
屏幕2 Event > Focused > Keyboard set target > 设置Keyboard 和Textrea
... 

### (2) 动画的添加
SquareLine 中的动画采用了自动识别文件的方案, 需要将动画排列成 png 文件, 然后自动识别 sequence 最后逐帧播放。
在 Animation 中添加新的动画 


### (3) 表格的插入方法
在Charts 中进行表格的插入, 
![[attachments/Pasted image 20240608091920.png|500]] 
通过Number of Points 控制显示的点的个数, 用于绘制温度等等的图表非常有用。
![[attachments/Pasted image 20240608092913.png]]

如图, 设置一个颜色为 White BG，并将相应的Dark Theme中的定义设置为黑色, 则直接切换Selected Theme 即可切换颜色。
![[attachments/Pasted image 20240608100736.png]]