Inspire 是使用进行拓扑优化和结构优化设计等等进行优化设计的, 仅从力学的方面考虑问题, 进行相应的拓扑优化和 
绝大多数 Optistruct 能够执行的拓扑优化流程都可以从 Inspire 中直接完成。 

Inspire 只能够通过位置， 角度调整和捕捉来实现零件的移动和装配,  因此应当尽可能在CAD软件中进行装配。
通过右下角的部分选择移动过滤表: 
![[attachments/Pasted image 20240406105434.png|550]]

如图， 在零件部分， 可以调整材料的 Transoarency 等等部分，
![[attachments/Pasted image 20240406105612.png|800]]
选用Structure > Contacts 可以显示重合面。 
![[attachments/Pasted image 20240406110010.png|800]]

# 拓扑优化基本流程
以实例: 桥梁的拓扑优化 
首先建立一个简易的桥梁模型， 如图：
![[attachments/Pasted image 20240406110456.png]]

进行布尔运算创建桥的整体结构(略) , 注意的是
### (2) 创建新 Part 的步骤
在拉伸时， 选择Addto New Part > Create New Part, 如图所示。 
![[attachments/Pasted image 20240406113442.png|600]]
这样才能够拉伸成功， 否则拉伸将会不成功。 
构建出的基本的桥梁模型如下:
![[attachments/Pasted image 20240406113809.png|400]]
### (3) 设计空间和非设计空间, 形状控制 
我们在进行拓扑优化过程中， 需要指定设计空间和非设计空间。 其中， 可以优化的称为设计空间， 不能优化的为非设计空间。 
将某一个零件设置为设计空间， 只需要右键零件， 然后勾选 Design Space 即可。 
![[attachments/Pasted image 20240406114039.png|400]]
通过Contant > Bonding 中的绑定连接， 实际情况中的螺栓或者焊接都可以近似认为是绑定接触。
不设定接触 > 相互之间没有接触力, 会相互穿透 (很少用到)。 

形状控制即提前给软件提要求使得获取的拓扑优化结果尽可能对称等等。基本方法是Structure > Shape Controls, 包括 Draw Direction(拔模方向) 和 Symmetric Controls (对称控制); 
![[attachments/Pasted image 20240406120115.png]]
Bead Patterns 用于设置拉延筋模式， 主要是针对形貌优化（钣金和筋条布置方式等）

### (4) 材料属性设置
![[attachments/Pasted image 20240406115821.png]]
### (5) 定义一端固支， 另一端简支的条件
如图， 在图中双击固定约束，会弹出如图的三个自由度设定。此时,  只需要将某一个箭头设为绿色，即释放对应的自由度。 
![[attachments/Pasted image 20240406114849.png]]
### (5) 优化
直接点击 Optimize 即可进行优化分析。 
其中<mark style="background: transparent; color: red">厚度约束是生成空间最小特征的厚度， 这个厚度 设置越小， 精度越高， 会生成更多细小特征</mark>， 网格大小划分也会越密集。 
另外， 厚度越小计算时间也将会越长。 另外通常在优化时， 是不考虑结构自重的。 注意在Load Cases中， 要包含一组约束(包含固定约束以及相应的载荷)
![[attachments/Pasted image 20240406121222.png|400]]
此时点击 Run 可以直接进行拓扑优化分析。 
在Preference > Run Options 中可以进行Number of CPU Cores 的设置(本机是4核8线程, 则可以选择为8)
![[attachments/Pasted image 20240406121708.png]]
拓扑优化结果如图， 其中$\star$表示的位置为比较接近于优化目标结果的位置。 
![[attachments/Pasted image 20240406122534.png|800]]
在Model树下右键Mass > Apply 
![[attachments/Pasted image 20240406123021.png]]
显示拓扑优化后的 Part1 部件为5.785kg， 如下图: 
![[attachments/Pasted image 20240406123344.png|950]]


