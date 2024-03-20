# 常见的建模工具与使用方法
常用的有SpaceClaim, DesignModeler ,  BladeModeler 
往往DesignModeler 与CAD接口建模较好, 而Blade Modeler承担**旋转叶轮叶片的建模等等功能**; 

## 常见操作认识 
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
