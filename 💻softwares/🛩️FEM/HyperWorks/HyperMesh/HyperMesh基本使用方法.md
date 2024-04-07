HyperMesh 是广泛应用的有限元前处理平台。 并可以与CAD, CAE等等有对应的接口。
HyperLife: 疲劳分析工具 
HyperMesh具有最为灵活和高效的网格生成和编辑功能。一般要求导入， 前处理，求解和后处理(后处理往往可以使用HyperView或者HyperGraph获取), 其中HyperView往往是创建动画， 而HyperView往往是创建曲线。

前处理包括: 
1. 几何模型的清理和简化 -> 获取相应的几何模型
2. 拓扑划分和网格划分

HyperWorks 是HyperMesh的新的风格界面, （实际上也是HyperMesh）

shift + 右键可以绕点旋转。 
alt + 中键  circle zoom
选中和取消选择往往是通过Ctrl 和Shift来进行的。

### 基本菜单文件
对于HyperMesh, 其主要的文件包括：
```text title:HyperMesh设置文件
1. command.tcl  命令历史文件 (用户操作日志文件)
2. hmmenu.set  菜单设置文件。 保存关于用户偏好和面板等等选项设置, 图形设置等等, 以及穿透检查， 单元检查等设置信息。
3. hmsettings.tcl 浏览器， 界面布局， 快捷键配置， 导入和导出, 最近的文件等等。 
4. hwsetting.xml 
```
上述部分的hwsettings.xml 可以从document文件夹中找到, 而command.tcl可以从工作目录里面找到(一般是workpack中，预设的)

### UI 基本认识
由于HyperWorks 有不同的求解目标， 则最初需要指定目标求解器, 更改方法是Solver-> Interface, 选择下一步用于求解的软件即可。
![[attachments/Pasted image 20240329234640.png]]
在HyperMesh中， 当前的工作可以有多个页面， 而使用右上角的箭头切换不同的页面。
![[attachments/Pasted image 20240329234856.png]]
View 中用于设置快捷键绑定以及命令窗口的显示等等部分内容。**其中, HWC Console 实际上就是HyperMesh的命令行**, 也可以执行对应的命令(即command.tcl中的部分)。
![[attachments/Pasted image 20240329235156.png|250]]
在不同的部分中进行快速切换使用HyperMesh选项卡, 可以在 HyperMesh 和其他部件之间集成式切换。
![[attachments/Pasted image 20240330010251.png|400]]
左上角部分集成了**过滤选择器**和确认， 取消操作栏框。
![[attachments/Pasted image 20240330005739.png]]
在模型左下的部分用于**调节视角以及视图**， 进行截图等等操作。
![[attachments/Pasted image 20240329235656.png|600]]
通过配合快捷键, hyperMesh能够极大提升划分网格的效率。

# 保存， 打开和导入文件 
HyperMesh 提供了保存为.hm 文件的方法
Save Session, Save Model 等选项， 一般保存Model 即可。

对于打开Open ，一次只能同时打开一个文件， 打开新的. hm 覆盖原始的。
### (1) 导入文件
而导入可以一次导入多个模型,  即一个场景中有多个模型，与前面导入的共存。导入往往用于合并模型等等。 
注意导入多次时， 不要多次导入相同的文件，否则Import 获得不同重名的Component. 
在导入时可以选用导入Solver Deck 即求解器的求解文件。 (例如abaqus 的inp文件, Ansys 的.cdb 文件等等)  
![[attachments/Pasted image 20240330142613.png]]

需要注意的是, 导入Solver Deck 时, 对应的配置模板(Solver Interface) 为对应的模型。即将Reader更改为例如Abaqus部分。 
![[attachments/Pasted image 20240330143211.png]]
BOM表示装备的目录树即层级关系。 例如UG的asm文件, catia 的 product 文件等等。
Connectors 用于导入焊点文件， 往往表示焊点位置， 
LoadResult 用于在后处理时， 绘制云图， 曲线等等做后处理等等。 

### (2) 导出文件
默认HyperMesh (.hm文件)包含了几何和有限元对象。所导出SolverDeck 的求解文件仅包含有限元信息, Geometry仅包含几何信息。
![[attachments/Pasted image 20240330144616.png]]


# 实体组织策略, 结构树和几何体
首先选择Sketch> new Sketch > 然后选择Create 进行创建草图。
![[attachments/Pasted image 20240329230339.png|700]]

在草图界面，我们每次绘制完毕之后， 通过右键点击然后向左滑动即可快速退出(和Altium Design操作有点像)

Component 是最基本的对象， 实际上是一种容器，可以包含几何和网格对象（Geometry, Elements）。 在Model树中， **每一个小图标都可以控制对象, 网格和几何的隐藏和显示** 。
![[attachments/Pasted image 20240330145536.png|700]]
利用 Model  > Create 可以创建Part 或者 Part Assembly, 其中<b><mark style="background: transparent; color: blue">Part是Component的上级, Part Assembly 是Part的上级</mark></b>
![[attachments/Pasted image 20240330153720.png]] 
首先， 通过2D拓扑显示和3D拓扑显示可以区分实体和壳
一般2D的曲面会在2D拓扑显示时显示成黄色而3D显示成蓝色

往往是通过 特性Peoperties来区分不同的Component的， 例如零件的材料，壳体的厚度等等

从Component > Empty 可以找到目前为空的Component， 可以选择并进行删除。

# 偏好设置Preference 与容差调整
在Preference中， 可以设置方块中每一次旋转的角度。
![[attachments/Pasted image 20240330152237.png|800]]
另外， 在视角控制中， 也可以在方块上右键并且选取Align to the Screen, 并可以通过左下角的相机标识保存某个特定需要的视角, 同时，可以通过ctrl + 0-9 可以切换不同的视角。
第二个选项是特定Component控制， 而后面的一个可以全局控制显示对应的网格显示方式。


清理容差包括几何清理容差(Geometry Clenup Tolerance)和网格清理容差(Mesh Cleanup Tolerance),
![[attachments/Pasted image 20240330153014.png]]
在Meshing选项中， 可以设置默认的元素大小(Element Default Size)，默认值是10.0; 需要注意的是， 网格是存在几何自动清理容差的， 这个是在 Node Cleanup Tolerance 中(一般是自动的, 也可以手动调节)。

FE Topology Revision 用于控制<mark style="background: transparent; color: red">网格和几何的关联性</mark>。一般默认为Rebuid即网格默认修改贴合几何操作
![[attachments/Pasted image 20240330152724.png]]


# 显示控制和Mask显示和隐藏 
### (1) 基本显示控制操作
通过调整右侧的颜色和对应模型属性下的 FE Style, Geometry Style 可以调整模型的显示方法, 例如只显示网格等等。 
![[attachments/Pasted image 20240330145902.png|900]]
另外也可以通过下面导航栏控制显示
![[attachments/Pasted image 20240330150200.png]]
基本的显示方法如下图， 通过取消Automatic 可以<b><mark style="background: transparent; color: blue">选择根据材料, 厚度, 子系统以及拓扑结构等等显示面的颜色</mark></b>。 
![[attachments/Pasted image 20240330162815.png|400]]
相邻显示:  
<mark style="background: transparent; color: red">Show Adjacent 快捷键是比较有用的快捷键，用于扩展隐藏相邻的对象， 可以通过Show Adjacent 扩充网格来修补难以修补的区域, 其快捷键是J</mark> 
![[attachments/Pasted image 20240330163444.png|400]]
我们以下面两张图为例， Show Adjacent 是 J , 而 Selected Adjacent 是Ctrl + J 
![[attachments/Pasted image 20240330191354.png|200]]
选中相邻的部分之后， 效果如图:
![[attachments/Pasted image 20240330191501.png|200]]

选择过滤器往往是通过快捷键进行切换的， 具体参考 Entity Selector 部分和[[💻softwares/🛩️FEM/HyperWorks/HyperMesh常用快捷键|HyperMesh常用快捷键]]
截面显示在Section Cuts 中, 并且通过按钮切换方向。 
![[attachments/Pasted image 20240330164034.png|500]]

从Model 结构树中点击Components可以进入Components面板。
![[attachments/Pasted image 20240330164335.png]]
### (2) HyperMesh 列表树与显示控制
主要讲解的三个栏目包括Session, Model, Component三个部分;
![[attachments/Pasted image 20240330170448.png]]
其中Model栏包含了所有的部件信息,  且每个部分都可以单独双击展开。 
另外，Model中可以创建大量的内容， 例如Properties等等。 
![[attachments/Pasted image 20240330172016.png|350]]

在多个Component中，最新创建的 Component 会被设为Current Component并加粗显示, 也可以在Components 中选中任意一个进行 make current. 此时, 之后的几何体都会建立在这个Component中，和这个Component作为一个整体。
![[attachments/Pasted image 20240330214259.png]]

对于不同的Component, 可以右键选择Review, 此时其余的将被透明显示， 如下图: 
![[attachments/Pasted image 20240330170929.png|300]]
每个Component 都有对应的元素个数信息, 材料信息等, 均可以在Component界面进行右拉展开，也可以通过下方的部分进行修改。 
![[attachments/Pasted image 20240330171353.png|300]]
当进行批量编辑时， 可以在图中选中对应的对象， 然后选用Edit Component/
![[attachments/Pasted image 20240330171901.png]]

对于属性编辑器， 都是从Model中双击Properties进入的。 同样的， 也可以使用 Edit  Properties 选项
![[attachments/Pasted image 20240330172435.png|800]]
另外， 也可自定义显示参数中的内容， 例如在下拉列表中任意一项右键 Add Column, 可以将对应的属性添加到显示栏中
![[attachments/Pasted image 20240330172822.png]]
如图所示: 
![[attachments/Pasted image 20240330172939.png]]
Model > Close Entity View 关闭所有次级窗口，另外也可以在Configure中设置为Show Under Subfolders。 
![[attachments/Pasted image 20240330173118.png]]
在Properity中可以设置面壳体的厚度等等, 同时可以赋予材料。
![[attachments/Pasted image 20240330213828.png|500]]

## Mask 控制
Mask用于控制不同的部分的显示与隐藏。
![[attachments/Pasted image 20240330155606.png]]

在选取相应的节点时 , 往往使用Select > by ID 等选项， 另外也有Select by material 等等方法, 用于根据不同的标准来进行过滤选择。 


# 工具栏的使用
除了Measure， 还提供了Calculate 功能可以
![[attachments/Pasted image 20240330173857.png]]
工具的所有下拉栏项都可以使用来获取更多工具选项。 
![[attachments/Pasted image 20240330174136.png|550]]

对于有多个选项的， 可以<mark style="background: transparent; color: red">先选中 Target 之后再直接按中键可以跳到下一个选择选项</mark>。
![[attachments/Pasted image 20240330190356.png|600]]
另外，带有视频符号可以直接观看视频示例。
对于在 Topology 中更改的部分, 可以通过Nodes, lines 等等自适应更改拓扑结构的划分, 而之后划分的网格会自适应分布到对应的 lines 和  nodes 上。需要说明的是， 自适应网格划分行为可以在 Preference >  Mesh > CAD topoloy Revision 中进行自定义
![[attachments/Pasted image 20240330192022.png|600]]
注意: 如果上述选项中的 Keep  mesh along matched topology 被勾选， 则删除对应的线段之后， 拓扑不会恢复原状。但是

# 工具带 (Tool  Belt) 的使用
**使用 alt + 右键 可以 打开工具带，并通过Shift 切换另一个种类的工具带** 。 如图所示:
![[attachments/Pasted image 20240330194603.png|1500]]

同时， 快捷 tool Belt  支持自定义相应的内容， 只需将常用的部分拖进其中即可。 同时支持 Export 和 Import 对应的选项。
![[attachments/Pasted image 20240330201018.png|1300]]


# 面法向设置和物体的移动
normal : 设置面的法向; 

在移动工具中， 可以按照指定的坐标系移动部件， 其中包括全局坐标系， 局部坐标系和自定义坐标系。 
![[attachments/Pasted image 20240330203010.png]]
物体<mark style="background: transparent; color: red">移动时设置捕捉的方法</mark>:
首先， 如果需要使用捕捉， 首先要将坐标系的原点设置到物体中需要进行捕捉的部分上。
此时，首先使用 shift + 鼠标左键移动，移动坐标系的原点到需要进行捕捉的部位。 

以将球心捕捉到正方体顶点为例, 首先坐标系应当设在球心。
![[attachments/Pasted image 20240330203843.png|400]]
然后将坐标系直接移动到所需要捕捉的正方体顶点上即可。
![[attachments/Pasted image 20240330204221.png|400]]
另外拖拽其中的黑色点可以改变坐标系的方向。 
![[attachments/Pasted image 20240330204721.png|400]]

例如, 在非法线方向进行拉伸时， 必须指定 Align Vector, 此时<b><mark style="background: transparent; color: blue">可以定义拉伸的法线方向， 保证拉伸方向正确。</mark></b> 
![[attachments/Pasted image 20240330210720.png]]
创建如图的斜线方向拉伸:(其中第二项为 Move Tool, 允许自定义改变拉伸方向), 注意: 定义完毕拉伸方向之后， 直接按下Esc可以退出坐标界面， 此时不需要指定 Guides, 可以直接指定拉伸的距离大小即可按照下述方法进行拉伸操作。 
![[attachments/Pasted image 20240330212647.png|500]]
另外, 带有三角图标的部分可以展开, 例如Move下的Transformation Tool ， 可以参数化的阵列和移动对象
![[attachments/Pasted image 20240401113811.png]]

# Extension Manager 的使用
通过File > Extension Manager 可以获取不同的Extension设置, 用于添加不同的Toolbox等等
往往都可以使用二次开发等等， 关联到对应的脚本。


# 其他补充部分
在所有的Hyper Mesh 中, 可以通过右上角的方框更改窗口布局， 并且每个窗口都可以change Type, Drag, Maximum等等。
可以变换成TableView等等多种不同的选项。 

通过SessionBrower 可以管理窗口布局， 观察每个Session下的多个窗口。
![[attachments/Pasted image 20240401191639.png|200]]
另外， 点击右上角的Untitled 可以添加页面， 实现一个总的分析下多个界面的管理 
![[attachments/Pasted image 20240401191606.png]]
多个不同的point , 可以Part , point 通过按下 P 多次进行切换显示等等。 

G: 切换显示网格， 显示几何和网格几何都显示。 
Q: 显示选中的节点号和单元号
D: 隐藏或者显示

上一个视图: B 
Shift + A 显示整个模型
Shift + I 仅显示选定内容或者当前对象模型
Shift + H 隐藏并清除选定内容

Select 廂中， 往往使用Save 和Retrive操作，在选择对象时， 可以进行save 和Retrive 操作
右键 > Select > by path, plane, .... 
alt 可以配置相应的快捷选择。 

n ： 从选择单元切换到选择节点。
例如， 可以选用所有的nodes, 然后改点s, 则可以直接跳转到分配了system 的对应节点。
。即检查节点是否引用了某个坐标系。 
中键 + space 可以快速跳转到Advanced Selections
by config 
查看模型中的三角形数量: 
显示网格 -> 按照config选择选择三角形 CATIA6

Entity Editor 
