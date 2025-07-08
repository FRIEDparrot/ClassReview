### (1) 基本
对于 SpaceClaim 中， 基本的参数化建模可以在 Design > Blocks > Blocks Recording 中找到, 

如下， 可以选择对应的 Parameters 
![[attachments/Pasted image 20250522100440.png]]

将 Import Options 中的参数设置选用为 All,  然后展开对应的参数部分，如下图， 选中所有导入参数。 
![[attachments/Pasted image 20250522100717.png|300]]
初始时， 对应参数不会修改， 而修改后， 会出现如下图的闪电图标， 表示需要更新原始 Attached 的几何模型。 
![[attachments/Pasted image 20250522111910.png|200]]
仅有 Discovery 和 Design Modeler 支持参数化， 而 SC 无法参数化。 其中 Discovery 操作与上下完全相同。 

对于边中点的提取， 只需使用 Shift 寻找中点， 选中之后进行 Create Point, 这样创建的就是中点的对应点。 需要说明， 圆角部分无法提取中点。 
![[attachments/Pasted image 20250522105334.png|400]]
选中点并 Create Origin，默认创建的， 在插入坐标系之后， 可以通过 Move Tool 旋转对应方向, 以使得实际建模方向满足要求。 
![[attachments/Pasted image 20250522101417.png|650]]


实际需要将裂纹的坐标系部分设置为如下图满足实际要求 : 
![[attachments/Pasted image 20250522120908.png]]
实际裂纹坐标系部分如下图所示 : 
![[Excalidraw/SpaceClaim 参数化建模和基本坐标系设置 2025-05-22 11.58.14|300]]
例如， 我们通过上述办法建立如下的坐标系，  同时 z 轴相对竖直方向倾斜 10度 : 
![[attachments/Pasted image 20250522121854.png|300]]
在进入 Model 界面时， 实际上会自动导入选定的坐标系。因此我们只需导入即可 : 
此时， 将对应坐标系设为 Origin_crack 
![[attachments/Pasted image 20250522122300.png|850]]

选择 Insert > Fracture 创建裂纹, 并**创建 Elliptical Crack 椭圆形裂纹， 注意默认为全局坐标系, 需要修改为  Origin_Crack 的裂纹坐标系**。 

![[attachments/Pasted image 20250522122700.png]] 

常用设置 : 

Fracture Affected Zone (裂纹影响区域) : 可手动定义影响区域高度和宽度
Largest Contour Radius **最大绕线长度,  不能大于最大影响区域高度的一半** (一半时， 刚好相切， 一般也不能产生)  


此外， 对于带有裂纹的基体部分， 应当**使用四面体网格进行划分**， 以保证实际兼容性。 因此一般在网格划分方法中强制部分采用四面体网格。 

划分 Mesh 完成之后， 选择 Genrate  Fracture 生成裂纹部分 : 
![[attachments/Pasted image 20250522123931.png|300]]
需要注意的是, 有时 hex dominant 无法产生相应裂纹，可换用 Tet 方法 
![[attachments/Pasted image 20250522124036.png]]

需要说明的是， 裂纹深度 (minor Radius 部分) 不能太深， 否则容易导致无法生成。 生成如下自动加密的网格 : 
![[attachments/Pasted image 20250522125037.png|300]]
对于小尺寸结构， 工程裂纹 大约为 0.3mm, 而航空中往往可能给 1.2 mm 部分 (minor 轴)

### (2) 热-机械联合作用下的应力强度因子计算   
往往采用如下的方法实现稳态热分析和静态结构耦合 : 
![[attachments/Pasted image 20250522143652.png|450]]
常用的分析设置包含 
1. 热流设定 (Heat Flow) 
2. 对流换热边界 (用于和外部进行对流换热)  

![[attachments/Pasted image 20250522144524.png]]

需要指定换热边界和对流系数 : 
![[attachments/Pasted image 20250522144902.png|500]]

对于温度场部分， 在下一个静态分析中， 在 Imported Loads 中, <b><mark style="background: transparent; color: orange">导入温度场分析结果</mark></b>。 
![[attachments/Pasted image 20250522152725.png]]
需要说明的是， 对应的载荷不能太小， 否则和实际结构不符，并且最大载荷出现在角点处， 不符合实际。 

得到实际的裂纹热应力分析所得结果如下图所示 : 
![[attachments/Pasted image 20250522163702.png]]

同时也可以添加对应的 Fatigue Tool, 求解得到结果中 :  
1. SIFS (K1) 是 1 型应力强度因子 
2. K2 对应  2 型裂纹强度因子 

需要说明的是， 一般舍去第一条并认为是不准确的。 实际结果中可能有多条曲线， 取应力强度因子为最接近下面的部分 (裂纹中的积分更加均匀) 

![[attachments/Pasted image 20250522165913.png]]



![[attachments/Pasted image 20250522164141.png]] 


> [!warning] 注意
> 不论是椭圆型裂纹还是半椭圆型裂纹， 均需要将对应的网格划分形状选为 Tet 形状, 也可能由于此导致相应问题。 

通过 Result > Edges > No WireFrame 隐藏单元网格部分; 
![[attachments/Pasted image 20250522170752.png|700]]

也可以通过 Fracture Tool > J integral 查看得到的  J 积分值部分。 如下图所示 :  
![[attachments/Pasted image 20250522171727.png]] 

一般而言，  J 基本应该是如下的图像部分 : 实际上是按照网格对应的环向进行积分的得到的结果。 
![[attachments/Pasted image 20250522171822.png|700]]
另外， 可以利用无摩擦支撑 (Frictionless Support) 代替对应的 Displacement 约束。  
![[attachments/Pasted image 20250522172255.png]] 
