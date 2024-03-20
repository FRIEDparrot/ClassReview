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
