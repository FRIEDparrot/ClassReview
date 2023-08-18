首先在左边选择component mode 之后点击P按钮，然后找到需要的元件
双击添加对应的元件到电路

在左侧的 terminals mode 中可以找到对应的input和output选项， 得到整个元件的仿真图

![[attachments/Pasted image 20230802122138.png|600]]

重要的设置以及快捷键配置可以在System选项中找到
![[attachments/Pasted image 20230802122534.png|250]]
可以通过Edit Key Mapping `->` 如图的方法进行快捷键的配置
![[attachments/Pasted image 20230802122722.png|700]]
F8全屏

缓冲器(Inverter)和反相器(Buffer),需要注意的是, NOT 输出的总是空，而我们需要高低电平来实现逻辑控制
因此使用Buffer和Inverter来进行逻辑控制， 使用Inverter替代原始的非门部分

用POWER进行模拟电源

一般的开关是switch, 而 SPDT, DPDT 等表示开关分别是单刀双掷开关和双刀双掷开关，

“D Flip-Flop”（D触发器）可以实现输出的高低电平设计
对于这些触发器元件可以存储一个比特位（即0或1），并在接收到时钟信号时切换其状态
结合SPDT可以实现高低电平的切换模拟， 这样就可以实现最简单的逻辑门设计： 
![[attachments/Pasted image 20230802134256.png|1000]]

失去程序焦点时按下delete可以解决
