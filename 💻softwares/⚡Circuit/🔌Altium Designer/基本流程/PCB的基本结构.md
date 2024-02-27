# PCB绘制部分
PCB的绘制包括**起名, 放置引脚, 放置丝印和放置PCB外框**四个步骤
MSOP8 
![[attachments/Pasted image 20231124174336.png|600]]

TOP overlay丝印层

X, Y控制镜像对称
芯片的去耦电容离芯片的引脚越近越好, 且容值越小应当离管脚越近。

原理图绘制完成之后，应当检查(右键compile) 

网表传递 : **器件**的编号, 封装, **网络**的网名, 包含的引脚, 模型和属性(**参数**)都可以包含到网表中
称为Design `->` netlist for project `->` protel

通过annotate进行芯片部分的自动重命名

绘制完毕原理图Schematic之后, 可以使用Design-> update to PCB document 
注意导入是否成功



导入时绿色点表示有错误产生


使用Design > Rules 可以更改PCB的设计规则, 
例如线距 0.254 可以更改为0.1左右  

有时via -> track的间距需要增大Routine Via hole size (0.2mm)
Routine Via style  preferred Minimum 0.45 

L 键可以使用板的层和颜色以及层的显示的部分 All Off

高速信号线需要尽量短, 而模拟信号线也需要尽量短。
布线时需要<b><mark style="background: transparent; color: blue">优先布高速线和模拟信号线</mark></b>
shift + 点击某个器件可以高亮某个引脚的所有布线


ctrl + shift + 滚轮 换层 

地线可以 

过孔塞树脂之后，即使在焊盘上， 也不会吸焊锡的
电路板有高速需求时，地层要电源层大一点 
只要对铜层有任何修改, 则

Design -> Redesign Body shape




#  PCB 基本结构部分
PCB基本结构是两层铜加一层FR4纤维, 实际是2层板
对于多层的PCB使用半预制片隔 开

铜板上覆盖一层油漆阻焊层， 可以保护电路板和焊接层
丝印层(通过)

C, R, U(集成芯片), J插件, L电感, X等等
使用$FeCl_{3}$进行蚀刻, 沉铜等。

基础的工艺指标(需要注意板的极限)
![[attachments/Pasted image 20231124165546.png|400]]
孔内径0.2, 外径0.45 
另外线到焊盘和孔外径的间距为0.15mm左右
铜厚一般是1oz(2oz可以用于大电流, 也可以开窗等等)

PCB在PCBlab (schematic lab)中得到, 有Pads(焊盘)和芯片外框
![[attachments/Pasted image 20231124170241.png]]
![[attachments/Pasted image 20231124170248.png|600]]
芯片外框

布局主要由前后板的连接和信号的走向确定的。

在速度比较高时, 走线的长度应当相等。比较重要的是米皮线(GHz)

## 叠层设计
往往使用1,3层作为信号层, 2层板一般不需设计

对于**4层板**(最容易), 1为信号层, 2地层, 3电源层, 4地层 
整个PCB的元件是由原理图确定的, 

网名(net label) : 我们将IO35_L_N等将多根线连在一起, 则我们将几束线连在一起, 并起一个相同的网名, 例如VCC3V3中, 1,2,3,4是接在一起的(蓝色是注释)
![[attachments/Pasted image 20231124171013.png|500]]
MD1 米皮线
schmatic lab原理图库中找到, 最重要的是引脚pin number, 和PCB库的原理图是一定对应的。

其中方框中的引脚**和PCB库中的引脚1,2,3分别要必须对应**
库包括**原理图和PCB库**, 而在PCB得到之后需要进行贴片和调试。 需要先**确定系统指标**
.brd, .pcbdoc没有保密特性, 而Gerber文件则有保密特性

PCB绘制分为**建库， 绘制原理图和绘制电路图**三大步骤


