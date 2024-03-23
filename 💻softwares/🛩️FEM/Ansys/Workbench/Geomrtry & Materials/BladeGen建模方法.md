参考[[💻softwares/🛩️FEM/Ansys/Workbench/Geomrtry & Materials/Workbench几何操作#BladeModeler的使用|BladeModeler的使用]]配置 Design Modeler 
在基础的界面创建BladeGen对象,
![[attachments/Pasted image 20240322001537.png]]
BladeGen主要有两种工作模式， 包括角度/厚度(ANG/THK)模式和初始压力面/吸力面定义(PRS/SCT)模式;
![[attachments/Pasted image 20240322002019.png]]
其中在初始界面下就可以选择Ang/Thk或者Pre/Sct模式工作

设定数量方法如图: 
![[attachments/Pasted image 20240322002443.png]]
也可以从工具栏设置: 
![[attachments/Pasted image 20240322003706.png]]

根据diaplay group control, 可以更加完整的看到叶片的模型部分: 
![[attachments/Pasted image 20240322003130.png|500]]

只要将其导入Geometry 即可获取到对应的叶片设计, 其中高亮部分即为子午面
![[attachments/Pasted image 20240322005204.png|300]]
# BladeGen 保存说明
一般以.bgd保存在工作目录下, 

另外可以输出为 TurboGrid 可以识别的文件 (共有4个, .curve, .inf格式存在, 分别包含了叶片轮毂,  叶片罩，叶片轮廓和叶片信息) 方法是 File > Export -> Turbo Grid Input File 

# Vista CFD 的 Meanline sizing tools 建模
Meanline sizing tools 是<mark style="background: transparent; color: red">弧线设计工具之一</mark>,   其中具有弧线校准功能， 且可以为后序的叶片分析优化提供初始叶片的几何设计。
Meanline sizing tools 包括**离心式压缩机的 ANSYS Vista CCD, 径向涡轮设计使用的 Vista RTD , 离心泵设计的ANSYS Vista CPD 和用于轴流风扇的 ANSYS Vista AFD**

# 压气机径流式叶轮建模

使用 Radial Impeller 进行径流式