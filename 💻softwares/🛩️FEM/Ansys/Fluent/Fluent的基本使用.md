
# 三维fluent流场计算
### (1) 工程创建与基础设置 
首先工程选项选择 Fluid Flow (Fluent)
![[attachments/Pasted image 20240322202410.png|800]]
对于三维的物品，可以ICEM CFD中划分网格， 也可以使用自带的工具。划完网格之后， 需要添加 Named Selection， 然后将
入口和出口==分别命名为 Inlet 和 outlet, 另外也把墙体全部选中命名为wall==  
![[attachments/Pasted image 20240322203943.png|600]]

==在其他软件中划分好网格后导入模型到 Fluent==， 然后在进入Fluent时，选用双精度即可。 
![[attachments/Pasted image 20240322205046.png]]
在CFD中， 使用的是**压力基础的, SST  $k-\omega$ 模型**

多处理器求解的设置:   Using dynamic process spawning now requires that you manually specify the interconnect as -pethernet for shared memory or distributed memory without InfiniBand, and -pib.ofed for distributed memory over InfiniBand. 

设置稳态和瞬态求解器(如果需要动态求解， 则选用瞬态求解器)
![[attachments/Pasted image 20240322210526.png|400]]
下面可以选择是否添加重力选项。 
![[attachments/Pasted image 20240322210933.png|900]]
### (2) 多相流模型选择
在多相流中提供了如下的选择: 
1. **Volume of Fluid (VOF)模型**：  
   - 用于追踪两种或多种不相容流体的界面位置。  
   - 需要定义表面张力。  
   - 设置库朗数（Courant Number）以控制时间步长，保证计算的稳定性。  
  
2. **Mixture模型**：  
   - 适用于相间存在相互渗透现象的模拟，如液体中的气泡或气体中的液滴。  
   - 可以考虑质量交换。  
   - 不能用于无粘计算或模拟凝固和融化。  
  
3. **Eulerian模型**：  
   - 适用于各种场合，尤其是颗粒悬浮、流化床等。  
   - 可以模拟气泡柱、上浮等现象。  
   - 有一定的使用限制，如不能用于无粘流动或模拟凝固和融化。

> [!hint] 说明
> 如果是设置多种材料界面， 则设置完材料之后， 再回到多相流VOF选项设置每一相(上方Phase)的属性， 另外还需要设置相界面属性(Phase Intersection)(指定不同的几个相)， 一般也需要设置表面张力模型

![[attachments/Pasted image 20240322211428.png|1200]]
湍流模型的设置如下:
![[attachments/Pasted image 20240322212217.png|900]]

# 二、材料和网格化
### (1) Fluent 材料数据库的选用
我们以在材料中添加水为例， 选择材料Create 右侧的数据库选项
![[attachments/Pasted image 20240322213620.png]]
在右侧选用流体或者固体， 并找到水的一项。然后复制， 关闭即可。此时即可自动添加水的分析。
![[attachments/Pasted image 20240322213749.png|800]]
第三是<b><mark style="background: transparent; color: blue">在Cell Zone Conditions 设置每一块的材料属性</mark></b>， 即将材料赋给某一个区域, 默认为 air , ==如果设置为其他则会导致错误== 
![[attachments/Pasted image 20240323104330.png]]
### (2) 网格显示
对于显示网格， 只需使用网格工具即可。 
![[attachments/Pasted image 20240322215632.png]]
# 三、入口， 出口与边界条件

对于入口和出口类型， 需要在对应的面上右键设置，
![[attachments/Pasted image 20240322215931.png]]
另外， 一般使用**湍流强度和湍流粘度模型作为湍流模型** 


> [!NOTE] hydraulic - diameter (水力直径) 模型
> 对于此模型， 其重要解释如下: 
> 在 Fluent 中，入口湍流选项中的水力直径（Hydraulic Diameter）模型是用于描述<mark style="background: transparent; color: red">非圆形管道或者流体并没有充满管道内部的情况</mark>。以下是一些关于如何在 Fluent 中设置湍流参数的基本步骤：
> 1. 湍流强度（Turbulence Intensity）：湍流强度被定义为脉动速度的均方根与平均流速的比值。对于内部流动，气流充分发展时，可以通过雷诺数来估算湍流强度。例如，当雷诺数为50000时，湍流强度为4%1。
> 2. 湍流长度尺度（Turbulence Length Scale）：湍流长度尺度与涡流的大小有关。在完全发展的管内流动中，湍流长度尺度与管道的物理尺寸之间存在近似关系。对于非圆形管道，特征长度应为其水力直径1。
> 3. 湍流粘度比（Turbulent Viscosity Ratio）：湍流粘度比是湍流粘度与流体动力粘度的比值。一般湍流粘度比在1-10之间，默认值为10。对于低湍流水平如外部自由流，湍流粘度比可设置为1。对于中等湍流水平，湍流粘度比可设置为10。而对于高湍流水平，这个数值最大可以设置为1001。
> 在某些情况下，湍流参数的设置会影响收敛速度，甚至会影响计算结果。因此，正确地设置湍流参数是非常重要的。希望这些信息能够帮助你更好地理解和使用 Fluent 中的入口湍流选项水力直径模型。

# 求解设置
### (1) 残差设置

![[attachments/Pasted image 20240322221328.png]]



### (2) 计算自动保存设置
![[attachments/Pasted image 20240322222617.png]]

![[attachments/Pasted image 20240322223340.png]]


### (3) 切片显示
首先 result > Create 一个平面 
然后在结果显示中， 选中这个平面,  