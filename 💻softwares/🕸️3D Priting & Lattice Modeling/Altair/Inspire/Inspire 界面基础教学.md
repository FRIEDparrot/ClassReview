Inspire 是使用**进行拓扑优化和结构优化设计等等进行优化设计**的, 仅从力学的方面考虑问题, **进行相应的拓扑优化和绝大多数 Optistruct 能够执行的拓扑优化流程都可以从 Inspire 中直接完成**。

视频教程 :  [Inspire get started](https://2024.help.altair.com/2024/inspire/en_us/topics/inspire/get_started/videos_gs_r.htm) 
详细文档 :  [Inspire 2024 docs](https://2024.help.altair.com/2024/inspire/en_us/)  

Inspire 只能够通过位置， 角度调整和捕捉来实现零件的移动和装配,  因此应当尽可能在CAD软件中进行装配。

## 一、操作基础
### (1) 基础界面操作 
通过左下角的 Show/Hide Tool, 左键将某个部分变为透明， 而**shift+ 左键恢复显示**。
![[attachments/Pasted image 20250505091735.png]]

在对应在 Hide Tool 状态下， 通过下方选取可**切换面或者体的 Show/Hidden 模式**。  

![[attachments/Pasted image 20250505092224.png|650]]

View Tool 如图所示， 用于存储和恢复视角 : 
![[attachments/Pasted image 20250505101347.png|550]]
在草图绘制中， 通过 Dimension Tool 来施加约束， 最终保证草图呈现白色 (fully defined) 
![[attachments/Pasted image 20250505104319.png|450]]

我们在进行拓扑优化过程中， 需要指定设计空间和非设计空间。 其中， 可以优化的称为设计空间， 不能优化的为非设计空间。 

将某一个零件设置为设计空间， 只需要右键零件， 然后勾选 Design Space 即可。 
![[attachments/Pasted image 20240406114039.png|400]]

在零件部分， 可以调整材料的 Transoarency 等等部分，
![[attachments/Pasted image 20240406105612.png|800]]
选用Structure > Contacts 可以显示重合面。 
![[attachments/Pasted image 20240406110010.png|800]]

### (2) 孔分割， 简化处理部分 
通过 Geometry > Partition 可以自动识别相应孔结构并进行分割， 
![[attachments/Pasted image 20250505113621.png|500]]
可以通过选择更改为 Simplify 选项部分，选用封堵孔 (Simplify > Plug) 
![[attachments/Pasted image 20250505114405.png|500]]
### (3) Structure 界面的载荷施加  
一般地， 针对 Supports 部分,  实际和 Loads 部分同一界面 
![[attachments/Pasted image 20250505120422.png]] 

### (4) 求解器设置 
按照 Preference > Run Options    设定求解器， 求解器包含  SimSolid 和 Optistruct,  
![[attachments/Pasted image 20250506082055.png]]
需要注意的是， **在 Load Cases 中， 需要将所有的载荷进行选中， 即进行所有载荷分析部分**。 

在分析结果中， 往往选择 Show Selected  Loads and Supports 或者 All 选项。 
![[attachments/Pasted image 20250506083324.png]] 

### (5) 基本后处理流程 
如图, 可以从右侧载荷显示部分拖动显示不同载荷范围情况。 
![[attachments/Pasted image 20250506084500.png|600]] 
需要说明， 默认仅选中  Vertical 部分， 而如果需要选中所有载荷， 需要更改为 Results Envelop 部分; 
![[attachments/Pasted image 20250506085542.png|200]]
### (6) 基础拓扑优化方法  
首先， **对于拓扑优化部分， 必须采用 Optistruct 作为求解器** 
对于拓扑优化约束施加， **采用 Structure > Shape Controls 作为施加拓扑优化载荷**。 

在Preference > Run Options 中可以进行Number of CPU Cores 的设置。 

Draw 是**制造脱模方向或者挤压方向**，在制造脱模方向上，对于 Single Draw 部分,  则会保留下方的脱模部分， 而也有双向脱模等部分。 
![[attachments/Pasted image 20250506101333.png]]

对于相应的  **Draw Directions** 约束部分,  实际上是制造工艺约束， 用于保证结构能够被制造出来，参考 [Single Draw](https://2024.help.altair.com/2024/inspire/en_us/topics/inspire/structure/draw_single_c.htm) 部分。
![[attachments/Pasted image 20250506090440.png|400]]
在 Optimize 部分进行拓扑优化选项， 
![[attachments/Pasted image 20250506102120.png|350]]

### (7) PolyNURBS  简述 
首先，可以使用 PolyNORBS 将复杂的拓扑优化结果进行拟合 Fit. 
实际上会生成一个新的 Part, 可以通过 Calculate 计算对应的重量。 
![[attachments/Pasted image 20250506105400.png|600]]
如下图，  可以调节拟合平滑度和使用面数量。  
![[attachments/Pasted image 20250506105942.png|600]]

> [!WARNING] 注意
> 一般直接采用 拓扑优化后结果的 Fit PolyNURBS, 这样界面部分会更加细致合理。  

在 History > PolyNURBS > Edit, 可以进行精细化和微调。 
![[attachments/Pasted image 20250506110203.png|380]]
删除面和修补面部分如下图所示 :  
![[attachments/Pasted image 20250506110536.png]]
常见的有 Split, Repair , 

在Model树下右键Mass > Apply 
![[attachments/Pasted image 20240406123021.png]]
显示拓扑优化后的 Part1 部件为5.785kg， 如下图: 
![[attachments/Pasted image 20240406123344.png|950]]


