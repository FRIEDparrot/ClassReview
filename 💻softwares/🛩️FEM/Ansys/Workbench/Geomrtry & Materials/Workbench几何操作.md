# 常见的建模工具与使用方法
常用的有SpaceClaim, DesignModeler ,  BladeModeler 
往往DesignModeler 与CAD接口建模较好, 而Blade Modeler承担**旋转叶轮叶片的建模等等功能**; 

# SpaceClaim 使用
## 常用操作
缝合几何体选用 Design > Combine 
对于分割面和融合面的操作
分割面在 Design >  Split Surface 中, 而缝合面在Repair > Merge Faces 中;

space claim 中的选择过滤器通过如图大的方法进行调出:
![[attachments/Pasted image 20240306155501.png|800]]
修剪删除功能:
![[attachments/Pasted image 20240306155937.png]]
投影到草图的功能:
![[attachments/Pasted image 20240306155749.png]]
反向工程部分在 Tools 选项卡中, 有:
创建工件(生成数控加工部分的工件)等等;
![[attachments/Pasted image 20240306162420.png|800]]
Relief  (用于创建圆角切口用于止裂槽和拟合间隙)和 Unroll 选项 
![[attachments/Pasted image 20240306161849.png]]


## ClaimSpace中的焊接建模
对于焊接点的建模， ClaimSpace中提供了点焊 Spot Weld 和焊接 Weld 两种选项; 
在Prepare 中提供了Weld和Spot Weld部分用于焊接建模 ， 同时Sharp Edges 可以修复尖角等等; 
![[attachments/Pasted image 20240306163913.png]]

## BladeModeler的使用
对于BladeModel, <mark style="background: transparent; color: red">包含 BladeGen, Meanline sizing tools , BladeEditor 三个具体的应用</mark>构成的旋转机械部件设计工具产品名称的总称;

首先在新建几何文件时， 使用DesignModeler Geometry; 
![[attachments/Pasted image 20240306170539.png]]
在Workbench 中选择 Tools > Addins > 
![[attachments/Pasted image 20240306175220.png]]
然后将Blade Editor 中的两个都选中
![[attachments/Pasted image 20240306175255.png]]
最后保证Tools > Options  > Addins 中的BladeModeler Licensing 处于开启状态
![[attachments/Pasted image 20240306175514.png]]

另外， 在Workbench中对应的 BladeGen 中也有对应的设计界面， 只需要新建即可; 
![[attachments/Pasted image 20240306180127.png|900]]
对于BladeGen, 主要有两种工作模式， 包括角度/厚度(ANG/THK)模式和初始压力面/吸力面定义(PRS/SCT)模式;
<b><mark style="background: transparent; color: blue">分别提供了径向和轴向叶片的设计环境</mark></b>, 可以通过Model > Mode 切换 
![[attachments/Pasted image 20240306180850.png]]

# 几何测量方法
在Measure中 ,  首先 ctrl 选择两点， 如图: 
![[attachments/Pasted image 20240306093727.png|950]]
对于线之间的距离测量，只需要类似选中线即可
![[attachments/Pasted image 20240306093846.png|700]]

# DesignModer 的使用


我们可以通过下图的<mark style="background: transparent; color: red">图形选项工具条</mark>进行图形显示选择,  其中
![[attachments/Pasted image 20240321214844.png]]
第一个提供 了面涂色的选项， 第二个提供了边涂色选项,  具体如下图: 例如可以使用共享拓扑进行涂色。或者使用固体和流体进行涂色。
![[attachments/Pasted image 20240321215327.png]]
注意: Black 是关闭面或者边的实体显示， 此时实体显示为黑色。
![[attachments/Pasted image 20240321215157.png]]
通过下图第二个进行new Section Plane 
![[attachments/Pasted image 20240321215852.png]]
而创建新的平面可以利用下面的按钮， 并可以选用下面的的类型
![[attachments/Pasted image 20240321220045.png]]
![[attachments/Pasted image 20240321220158.png]]

DesignModel 中通过如图的选项建立草图, (需要事先在Modeling树指定平面), 然后Generate即可
![[attachments/Pasted image 20240321213819.png]]

进入草图绘制之前， 首先可以设置网格显示选项， 这个在 草图Settings >  Grid 中， 可以设置建模时的自动捕捉。
![[attachments/Pasted image 20240321220951.png]] 
单位可以在上方的 Units 中进行设置
![[attachments/Pasted image 20240321222053.png]]
在绘制草图时， 对应的坐标会在右下角中进行显示: 
![[attachments/Pasted image 20240321222900.png]]
### (1) Design Modeler 中特征体建模
例如在XY平面上创建一个六边形 (删除需shift + 选中) 
![[attachments/Pasted image 20240321221855.png]]
从下面的框来创建体素特征:(包括使用Slice来进行体的分割和倒圆角Blend)
![[attachments/Pasted image 20240321223457.png]]
更多的体素特征可以从Create , Concept 和 Tools 中找到(包括Boolean等等)
![[attachments/Pasted image 20240321224853.png]]

### (2) 拓扑结构的合并
在拉伸草图时， 会有Merge Topology选项， 选中即可实现拓扑结构合并;
![[attachments/Pasted image 20240321223143.png|700]]
即使用Shared Topology 观察如下图: 
![[attachments/Pasted image 20240321223319.png|300]]
如果是对于任意两个相连对象， 希望其中不产生接触力， 可以使用拓扑结构合并操作Share Topology 
![[attachments/Pasted image 20240321224141.png]]
这个可以实现多个体的网格共节点行为(注意仍然会生成接触对)。

### (3) 几何体切分操作和接触面设置(便于网格划分)
几何体切分(Slice) 是重要的操作， 用于划分高质量的网格
划分出的体会自动分成两个体， 并自动冻结。(也可以指定使用surface切分)
![[attachments/Pasted image 20240321225129.png|700]]

> [!caution] 说明
> Slice 会导致所有体冻结， 注意此时新建的几何体将不会与接触的任何零件合并， 而是作为一个独立的零件存在， 且不会共享拓扑结构。
> 共享拓扑结构往往用于准确模拟接界面的行为，即使网格共节点， 也可能产生接触力， 方法一是生成接触对之后选用接触对类型(参考接触对类型的选择)。方法二是设计时就设计为一个整体。

具体Bonded 和接触的设置方法如下, Bonded 即连接为整体， 不会产生额外接触力。
![[attachments/Pasted image 20240321232814.png]]
### (4) 点特征的创建(焊接点等)
使用Point创建点特征
![[attachments/Pasted image 20240321225823.png]]
在点创建中可以定义焊接等行为；
### (5) 分离(Imprint Faces)和粘附(Imprint)
在Boolean中有Imprint  Faces 选项 
<mark style="background: transparent; color: red">Imprint Faces, 往往用于在一个面上创建一小块局部区域,  往往在特定区域施加载荷时有用，不会改变拓扑结构</mark>
![[attachments/Pasted image 20240321230204.png|500]]
另外， 如果选用Imprinted 则变成两体粘附的操作。
![[attachments/Pasted image 20240321230348.png]]
### (6) Face Split 方法
Face Split 的位置如下: 通过插入可以划分面网格， 使得总体划分更加精细

Face Split 不会改变几何体的数量， 也不会额外产生接触力。方便后期的载荷施加和网格划分。
![[attachments/Pasted image 20240329091320.png]]
