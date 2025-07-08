---
~
---

> [!NOTE] HyperMesh设置整体重置办法
> E:\HyperWorks2023\hwdesktop\utility\HWCleanup\找重置的.bat 文件,  此时可以重置一切设置
# 几何体的创建
### (1) 基本菜单流程
在HyperMesh中, **几何创建是在Geometry菜单中进行的, 但是2023中没有Geometry菜单, 而是将其集成到了Topology 菜单中**。 

几何处理包括:
1. 几何导入与几拓扑关系
2. 进行几何清理,  简化和去特征，清除标签等等 
3. 进行网格划分,  
4. 创建和修正中面 

导入时， 一般可以导入.x_t格式的文件，一般不会出错，如果直接.prt文件可能会导致无法导入的问题。
另外， 在2023中， 也添加了 Inspire Parameter 的支持, 即随CAD文件一起导入的参数可以在Hyperworks 中进行更新。

例如, 在 Points/Nodes 中选择建立Points,  然后选择Lines中创建直线。 此时即可得到对应的直线。 
![[attachments/Pasted image 20240401194737.png|300]]
对于特征的删除 ， 直接在初始界面使用选择过滤器即可进行选中和删除操作(注意拓扑不能删除).

### (2) 拓扑关系
拓扑关系是面与面之间的连接关系，边仅属于一个面时， 为自由边(红色)
另外，共享边为两个面连接(绿色), 还有T型连接(non-manifold edge)(黄色

在HyperWorks 中， 可以直接创建几何， 也可以使用Sketch工具创建几何对象; 另外， 可以 
Topology > Split 分割对应的边， 事实上Split下面的部分是选定 Tools 工具的
![[attachments/Pasted image 20240401201408.png]]
对于选用Plane进行切割的情形, 点击之后不用指定某个表面， 直接移动到线段上就可以创建分割面了。 

Stitch 中， 可以通过拖动将某些边和点移动到另一部分从而实现曲面的缝合操作。 除此之外， 还可以通过Supress 抑制某些边。
![[attachments/Pasted image 20240401201842.png]]
抑制的边不会影响网格的划分，且显示为虚线， 如图所示: 
![[attachments/Pasted image 20240401201948.png|400]]
对于丢失的曲面，可以选择四条相应的曲边来, 再到surface > Patch 中进行修复对应的几何。 使用PatchAll进行对应的面的
![[attachments/Pasted image 20240401212043.png|650]]

小边间隙修补方法: 
stitch > 
![[attachments/Pasted image 20240401212604.png|350]]
FE Geometry 选项中， 用于选中实体网格等等， 其中From CAD可以分别选取关联CAD和没有关联CAD的单元。Update 用于更新已经与CAD关联的几何单元。
Ruled工具可以通过线性或者平面创建曲面和实体等等。 

# Features 特征检测和创建
在Topology > Features 中启动Features 的创建界面
![[attachments/Pasted image 20240402002954.png]]
只需要输入查找的特征类型, 注意需要选用合适的容差。 
另外也可以手动创建上述特征。 

在检测中， 注意调整合适的容差, 则检测到之后会出现Feature Manager, 检测出的圆角如图所示: 
![[attachments/Pasted image 20240402094606.png|1100]]
可以选中任何一个特征 `->` 右键Review, 将特征高亮显示, 如下图:
![[attachments/Pasted image 20240402094823.png|700]]
可以通过 Defeatures 来移除几何中的部分特征，例如Holes, Fillets, Logos等等 (通过 三横线设置容差)
![[attachments/Pasted image 20240402095929.png|600]]

### 批处理修复圆角等特征
Defeature > Batch 用于执行批处理修复特征。注意要进行 parameters 和 criteria 的编辑操作。
![[attachments/Pasted image 20240402104222.png|400]]
其中Parameter定义了清除各种特征的大小和半径检查 
![[attachments/Pasted image 20240402104605.png]]
Criteria主要是设置网格的质量进行相关设置。

### 曲面修复部分
曲面修复集成到了2D > Surface Repair 中, 
![[attachments/Pasted image 20240402105253.png]]
Validate 是检查错误使用的命令, Edge 显示可见状态曲面上的自由边。
Geometry > Cleanup > 可以建设

对于小的圆角可以使用Cuts/Edge Fillets来进行清除
![[attachments/Pasted image 20240402111846.png|500]]
也可以使用Edges来进行查找和修复对应的边
![[attachments/Pasted image 20240402112639.png|400]]
Clean Up
# Offset 偏移
用于偏移某些面的操作， 曲面的拓扑关系在偏移过程中保持不变。 
![[attachments/Pasted image 20240402100400.png]]
对于线段, 可以进行均匀偏移或者非均匀偏移, 设置首端尾端偏移程度
![[attachments/Pasted image 20240402100725.png]]

# 草图操作
草图中， 可以通过dimension为草图添加约束。 
![[attachments/Pasted image 20240402100921.png]]
**草图中可以对特定约束创建相应的变量**， 首先需要创建约束Constraints, 然后进行Create Variable
![[attachments/Pasted image 20240402101030.png]]
变量的部分可以在变量编辑器找到， 并编辑， 创建和删除变量:
![[attachments/Pasted image 20240402101142.png|600]]
创建的参数化变量如图所示: 
![[attachments/Pasted image 20240402102244.png|650]]
在草图中, Sketch > Project 用于将邻近的几何投影到草图平面上作为参考。 投影后构造线将是虚线， 而为了将构造线转换为实线，可以
![[attachments/Pasted image 20240402102508.png]]
投影方法如图所示: 投影之后
![[attachments/Pasted image 20240402102811.png|500]]
在构造线上右键选择CreateCurve之后即可将其创建为草图曲线。 
![[attachments/Pasted image 20240402103313.png|300]]
Intersect 是用于草图横穿平面获取相交曲线的。 
另外， 在Preference > Sketching > Realize as 中设置为草图创建完毕之后，则默认生成何种几何图形
![[attachments/Pasted image 20240402103853.png]]


![[attachments/Pasted image 20240402104456.png]]

# 几何拓扑匹配操作
修复和缝合边和顶点的间隙使得几何面与模型拓扑关系一致。 Interface Check 用于查找几何曲面和实体之间的交叉和穿透， 
![[attachments/Pasted image 20240402113008.png]]
方法是便捷对其和边界未对齐的操作
![[attachments/Pasted image 20240402113128.png|900]]

# Midsurface 中面抽取 
中面在网格划分中往往是比较重要的，  可以在MidsurfaceMethod 中指定中面抽取方法
![[attachments/Pasted image 20240402114328.png]]
抽取中面效果如下:
![[attachments/Pasted image 20240402114017.png|400]]
Edit Plates 中可以查看各个中面抽取的类型,  
![[attachments/Pasted image 20240402114923.png|400]]
也可以更改某个中面抽取的计算方法， 例如可以将右侧平面的抽取改为Sweep等等
![[attachments/Pasted image 20240402115552.png|650]]
改为扫掠后可能获取到更为合适的中面划分， 如图所示:
修复前:
![[attachments/Pasted image 20240402121809.png]]
修复为Sweep之后会修复部分连续问题。 另外可以直接通过surface来进行中面的编辑和修改， 以获取到更好的中面连接

其中， 中面包括如下的类型:
![[attachments/Pasted image 20240402115048.png]]
使用Part 组件进行创建平面的方法 : 
![[attachments/Pasted image 20240402115336.png|800]]