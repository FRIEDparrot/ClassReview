参考[[💻softwares/🛩️FEM/Workbench/Geomrtry & Materials/Workbench几何操作#BladeModeler的使用|BladeModeler的使用]]配置 Design Modeler 
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
### (1) 形状和点位置调整
使用 Radial Impeller 进行径流式压气机建模

![[attachments/Pasted image 20240323154717.png|800]]
子午面实际上是如下的投影面:
![[attachments/Pasted image 20240323163747.png|700]]
在线段上， 可以任意添加插值点和删除插值点， 此时可以点击线段对对应的值进行修改， 增加和删除部分操作; 
![[attachments/Pasted image 20240323154826.png|600]]

### (2) 叶片角的调整
在左下图像中， 通过右边的两个点可以**切换叶片罩层(Shroud)是否激活**或者**轮毂层(Hub)激活**。
![[attachments/Pasted image 20240323155901.png|600]]
在**叶片角度窗口右键选用 Adjust Blade Angles**, 轮毂层和罩层完全相同
![[attachments/Pasted image 20240323155256.png|600]]
通过类似的对话框， **分别调整轮毂层和罩层和叶片角**即可。
![[attachments/Pasted image 20240323160322.png]]

### (3) 叶片属性设置
如图表示了
![[attachments/Pasted image 20240323160625.png]]
### (4) 喉部面积探测
喉部面积探测方法是使用Show Throat(s), 
![[attachments/Pasted image 20240323161055.png|600]]
**准正交面积按钮**可以在右边按钮组找到, 
![[attachments/Pasted image 20240323161316.png]]
**表面积分析**可以在页面如图的按钮上找到
![[attachments/Pasted image 20240323161803.png|1100]]
单击即可获取到**喉部面积信息**(其中Throat Surface Area 是喉部表面积)
![[attachments/Pasted image 20240323161833.png|700]]

如果希望全部显示叶片的三维模型， 可以在 Replication 中选用 All Replicates, 另外也可以在Display中选用
![[attachments/Pasted image 20240323162957.png|800]]
### (5) 几何导入方法
通过Properities 中，选中Create Hub 将会创建轮毂, 而Create All Blades 会**将模型中的所有叶片都显示(否则只是一个叶片)**
![[attachments/Pasted image 20240323164253.png|1100]]
注意Design Modeler 的 BladeEditor 的 License 必须是开启状态
然后在对应的 BladeGen 中， 选用Create Bodies -> All 和Ceate Hub, 得到如下的图像， 导入成功: 
![[attachments/Pasted image 20240323164705.png|900]]
