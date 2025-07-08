# 网格的多模块共享方法
对于在某个分析中,  选择网格并右击即可导出为STL文件; 这个是导出网格文件部分
![[attachments/Pasted image 20240308213401.png]]
参考 [reuse mesh in different workbench files](https://forum.ansys.com/forums/topic/reuse-mesh-in-different-workbench-files/), 得到: 
**==首先在建立工程时，应当先选用 Mechanical Model 模块， 然后在这个里面划分网格==** , 而不是直接建立力学模块; 
只要将Model进行共享即可实现网格共享， 如下图: 
![[attachments/Pasted image 20240308224032.png|1000]]
最终例如使用模态和瞬态分析共享网格， 如下图所示: 
![[attachments/Pasted image 20240308224802.png]]

# 网格公共拓扑的设置
在Ansys中, 我们往往会为了网格划分较好可能会将某个几何体划分为两部分, 但是直接划分为两部分会导致两者之间的面变成接触分析导致产生很大的接触应力; 

注意: 一般使用split 而不使用split Body, 这样会导致一个完整的体被分为两个部分, split 之后多余的只需直接删除即可; 

而为了保证连续性我们往往使用公共拓扑(Shared Topology) 和公共节点(Shared Nodes)
在公共节点中， 往往有Shared Topology 和Bounded Contact 两种, 需要注意的是，Bounded Contact 是将两者粘合到一起，而两个部分是分别进行网格化 Mesh 的; **注意使用 Bounded Contact  时， 两侧的自由度并不是连续的, 因此可能会导致连接处的部分不连续现象**。

首先， 我们设置网格过程中， 选取过滤器在图形上方(放到了左侧部分):
![[attachments/Pasted image 20240306115822.png]]
# 网格算法说明

### (1) 网格划分方法部分(Method)
这一部分主要是基于 Mesh > Insert > Method 方法; 
在网格的算法(Method > Algorithm)中， 主要有协调分片(Patch Conforming)和独立分片(Patch Independent)算法, 其中协调分片算法是基于Tgrid 算法先划分面网格再划分体网关的方法，并在多体部件中，生成混合四面体，棱柱等。

独立分片算法(Patch Independent)是基于ICEM CFD的自顶向下划分网格方法， 先生成体网格,  更适合对质量差的CAD模型划分网格。

> [!NOTE] 说明
> 对于不同情况, Mechanical 更适合使用协调分片的网格算法， 而电磁分析和流体分析适合用协调分片或独立分片方法，显示动力学适合使用带有虚拟拓扑的协调分片或者独立分片算法。 

六面体主导为 Hex Dominant 方法

Mesh > Show > Sweepable Bodies 可以显示可扫掠对象

多区域网格划分(MultiZone) 方法，用于**将几何体分解成为映射区域和自由区域， 然后判断区域并生成纯六面体网格**。


### (2) 全局网格尺寸控制和质量检查
点击Mesh 即可出现如图的网格信息控制
![[attachments/Pasted image 20240324092413.png]]
其中可以选用Use Adaptive Sizing, 全局的网格大小在Span Angle Center中，有Fine, Medium, Coarse 三个选项。
Transition 设置过渡的快慢选项
![[attachments/Pasted image 20240324094507.png]]

在划分完网格之后， 使用Mesh > Display Style 可以显示其中某些部分的**网格质量和单元质量信息** 
![[attachments/Pasted image 20240324092855.png|800]]
也可以通过如图方法检查网格质量 (可以在Mesh Metric下设置Element Quality 来检查)
可通过 Smoothing 来进行平滑网格，
![[attachments/Pasted image 20240324093434.png|900]]
在Quality > Mesh Metric 中， 可以观察处于不同状态的网格分布情况, 如图:
![[attachments/Pasted image 20240324122951.png]]


> [!NOTE] 说明
> 一般地， 应当保证单元的Aspect Ratio(纵横比) 不大于20(否则网格质量差)
> 雅各比值应尽可能避免小于0的部分出现。
> 同时Skewness 不得超过0.9(超过0.9则网格质量差) 而0.5-0.75为中等可接受的范围, 一般大部分在0-0.25为宜

### (3) 膨胀层设置
> [!NOTE] 笔记
> 膨胀层往往是使用在CFD中在边界处部分划分的几层细密网格

全局膨胀层的设置如下: 包括膨胀层膨胀的方式控制，全局的最大层数和增长率。
![[attachments/Pasted image 20240324095500.png|400]]

例如， 在CFD中， 为了计算壁面的网格更加精确， 我们往往会将流体的壁面加上一个膨胀层
![[attachments/Pasted image 20240324112336.png|900]]
添加好膨胀层的效果如下， 可以在壁面处更加精细的划分网格， 用于之后的CFD分析。
![[attachments/Pasted image 20240324112900.png]]
对于希望看到内部的网格, 使用 Home > Section Plane 即可
![[attachments/Pasted image 20240324114451.png]]
### (4) 其他实用工具
常用的有 Pinch (修剪小的特征) 
Match Control  > <mark style="background: transparent; color: red">用于循环对称和 2D 对称边上的网格划分</mark>, 可以保证划分后的对应面网格匹配

虚拟拓扑:
插入方法如下:
![[attachments/Pasted image 20240324113813.png|800]]

通过虚拟拓扑， 可以自动删除模型中的小边和小面等等。
![[attachments/Pasted image 20240324113948.png|700]]

# 网格编辑部分
可以在模型中插入 Mesh Edit 来编辑网格。

Contact Group 是用于创建接触对匹配的, 即在接触对上， 利用指定的容差来融合两个部分的节点。




# 叶轮网格划分
首先导出文件, 如图: 
先设置 Model >  Model Units 为 mm, 
![[attachments/Pasted image 20240324154335.png]]
然后导出一次 TurboGrid Input Files, 路径如下图所示
![[attachments/Pasted image 20240324152916.png]]
在TurboGrid中, 选择 Import 将其导入， 如果是在一个工程中， 也可以直接拖拽到 TurboGrid 模块, 如图所示:
![[attachments/Pasted image 20240324154734.png]]
首先选中 Display All Instances 
![[attachments/Pasted image 20240324160134.png|800]]

不选Target Maximum Expansion Data 之后， 可以正常划分网格，
![[attachments/Pasted image 20240324161710.png]]
检查网格质量时， 可以使用 Mesh Analysis 检查， 并使用Mesh Limits进行修改， 如下图所示:
![[attachments/Pasted image 20240324161841.png|800]]
