任务管理器 > 服务 >  SolidWorks Flexnet Server  > Start （可以不开启 LicensingService)
![[attachments/Pasted image 20240313161932.png]]

## 一、草图操作
### (1) 基本视角操作
对于绘制任意一个草图, 都是需要添加约束使其获得
方法是 smart dimension > 添加对应的约束, 当变为黑色时, 则已经完全约束成功。
在确定时, 可以从零件右上角窗口选中确定进行确认拉伸
![[attachments/Pasted image 20240706160413.png|800]]
对于定位到不同的视角, 可以通过View Orientation 来进行, 
![[attachments/Pasted image 20240706160635.png|650]]
对于将视图调整到某个面的法向, 可以选中面然后右键选择normal to，则得到相应的法向面
![[attachments/Pasted image 20240706163256.png|550]]
在每一个草图对象中,均可以添加 Horizontal，Vertical 和 Fix 约束, 而约束会显示在图像上, 
![[attachments/Pasted image 20240706164821.png]]
此时可以设置每个约束选项。

对于圆弧， 按住shift然后转圈， 即可找到希望绘制的圆弧方向。
### (2) 转换为参考线,  隐藏草图
对于某个线, 选中之后，<mark style="background: transparent; color: red">选择For Construction，即可将其转换为参考线</mark>, 此时得到的
![[attachments/Pasted image 20240706165158.png|1000]]
隐藏的方法是右键找 Hide 和 Show, 
![[attachments/Pasted image 20240707092229.png]]

### (3) 草图几何关系的编辑,裁剪和延伸
对于常见的草图工具, 包括重合, 中点， 相切, 平行, 对称, 共线， 相等等等, 
对于草图的编辑功能, 包括裁剪实体, 转换, 等距实体, 按实体引用等等。

例如, 我们同时选取三条直线, 选择Parallel, 则为平行, 
![[attachments/Pasted image 20240706173322.png]]
注意: 如果选择对象有两条直线, 则除了竖直(Vertical) 还有垂直 (perpendicular)

对于同时选择两个圆再选中一条直线, 则可以添加关系Symmetric  
![[attachments/Pasted image 20240706174902.png]]
对于裁剪实体, 有如下几个功能, 包含 Power Trim, Corner 等等, 如果想延伸实体, 则可以按shift进行裁剪, 此时可以延伸实体。
![[attachments/Pasted image 20240706175113.png]]
等距实体即Offset, 进行实体偏移, 
### (4) 转换实体引用
转换实体引用可以将几何中已有的线段提取到正在绘制的草图中, 也可以**视为投影和映射操作**, 
例如将某个圆弧投影到当前的草图平面上:
![[attachments/Pasted image 20240706175955.png]]
也可以通过转换实体引用实现继续拉伸功能, 

另外, 转换实体功能也可以通过选中草图, 可以在另一个面上生成相同的草图。

### (5) 基本设计模板 Template 讲解
在SolidWorks 中, 对于标准, 注解均需要按照某些标准进行, 比如文本大小, 字体等等, 可以通过 Settings > Default Template 进行设置, 
![[attachments/Pasted image 20240706181331.png]]
每一次在System Option 更改字体时, 均会产生一个新的模板文件，具体标注的标准可以在Document Properties 中进行更改, 
![[attachments/Pasted image 20240706181727.png]]
.. - fixed, 如果希望这个尺寸保存下来,  可以在Drafting Standard 中进行保存和修改, 然后使用Load From External File来进行获取相应的绘图标准。
![[attachments/Pasted image 20240706181835.png]]

**另外也可以将某个文件存储为 .prtdot 或者 .asmdot 格式, 即为使用的文件模板了。此时可以直接从Default Templates 中进行选取模板**。

### (6) 草图阵列和镜像
对于草图的阵列和阵列, 有 Mirror Entities 和 Linear Sketch Pattern 两个方面, 

![[attachments/Pasted image 20240707090445.png]]

镜像可以在下方的部分进行选取(点击Mirror About两个框可以切换)
![[attachments/Pasted image 20240707090941.png|800]]
  
对于草图的阵列, 可以通过如图的方法, 修改实际阵列的角度以及距离。
![[attachments/Pasted image 20240707091337.png|900]]
另外, 也可以方形阵列, 具体方法是修改Y方向阵列的个数
![[attachments/Pasted image 20240707091533.png]]
另外, 也可以使用圆周草图阵列功能, 如下:
![[attachments/Pasted image 20240707091820.png]]
## 二、基本实体成型
### (1) 基本创建与拉伸
在 From 中选用 Offset 可以实现设定起始和拉伸的距离
![[attachments/Pasted image 20240707103231.png]]

常见的成型命令如下: 
![[attachments/Pasted image 20240707103731.png|1300]]
对于Lofted， 可以通过<mark style="background: transparent; color: red">定义两个端面来得到最终的实体</mark>, 
![[attachments/Pasted image 20240707151044.png]]
对于有多个对象的草图， 可以更改 Selected Contours, 来修改哪一个部分需要拉伸
筋条创建使用Rib，注意草图必须有一个开放的边界。
![[attachments/Pasted image 20240707105430.png]]
可以直接按F切换为fit视角, 此时可适应
### (2) 参考平面, 参考轴和参考坐标的创建
该部分在 Reference Geometry 中可以找到, 
![[attachments/Pasted image 20240707110839.png]]

在创建面时，必须指定到这个平面的距离（下面设定为5）
![[attachments/Pasted image 20240707111022.png]]

螺旋线可以先建立一个圆的草图, 然后选用 Curves > helix and Spiral, 即可将圆转换为相应的螺线。
![[attachments/Pasted image 20240707111443.png|850]]
可以直接使用扫描中的圆形轮廓方法生成弹簧等等
旋转中心线一般是可以使用不封闭草图， 并将旋转中心线定义为构造线的。
### (3) 螺纹线的显示
Insert > annotation > Cosmetic Thread 
![[attachments/Pasted image 20240707113147.png]]
注意更改View Top Annotations 类型, 可以保证显示
![[attachments/Pasted image 20240707114419.png|400]]
另外如果需要显示贴图， 勾选Document Properties > Detailing  > Shaded cosmetic threads
![[attachments/Pasted image 20240707114949.png]]
选择过滤器使用右键 > Select Other, 具体如图:
![[attachments/Pasted image 20240707115852.png|350]]
另外， 也可以使用直接右键找过滤器图标, 如图: 
![[attachments/Pasted image 20240707185545.png]]

对于对称中心面的对齐操作, 可以在定义面时，选用多个参考,并且使用 mid plane 来施加中面约束。
一般可以添加重合 (Coincident), 平行等等约束。
![[attachments/Pasted image 20240707131037.png]]
通过剖视图 Section View 可以先选择基准平面， 并获取剖切视角
![[attachments/Pasted image 20240707131610.png]]
选中Full Preview 时，则显示圆角的切割预览。
![[attachments/Pasted image 20240707132708.png]]

### (4) 异形孔向导
孔一般使用 GB 标准, 常见的有以下几个标准:
![[attachments/Pasted image 20240707142400.png]] 
GB/T 5782-2000,  GB/T 5781-2000,  其中 Hex head bolts 表示六角头螺栓
Slotted cheese head screws 为开槽圆柱头螺钉
socket head 为圆柱头螺钉

注意: 主要不在圆孔修改其直径和深度, 而是在Size 中修改型号(一般使用M2等等类型)
![[attachments/Pasted image 20240707143531.png]]

对于螺纹孔, 一般选用 Straignt Tap 即为螺纹孔, 即打孔时会自带螺纹
![[attachments/Pasted image 20240707143933.png]]
注意在 Options 中，需要选中 Cosmetic Thread 来保证螺纹孔符合要求:
![[attachments/Pasted image 20240707144301.png]]

### (5) 实体的阵列与镜像
对于实体的阵列和镜像, 如下图:
![[attachments/Pasted image 20240707144741.png]]
在SW中, 连接在一起的实体部分不需要进行布尔运算, 自动会合成一个Body, 并且放在Solid Bodies 选项栏中, 可以单独选中
![[attachments/Pasted image 20240707150003.png]] 

需要注意: 对于镜像实体, 如果是放在Feature 中是不允许的, 因此必须放在Bodies to Mirror 中才能进行成功的镜像。 
![[attachments/Pasted image 20240707145740.png]]


> [!NOTE] 快捷键补充
> 如果模型构建后突然消失, 通过ctrl + B 可以重建模型, 来重新显示模型
> ctrl + 8重定向视图

## 三、常用功能的部分
### (1) 草图图片功能和样条曲线
寻找 Tools > Sketch Tools >  Sketch Picture , 可以设置透明度并沿着轮廓绘制图形。
![[attachments/Pasted image 20240707151943.png|550]]

样条曲线在草图功能栏中就有
### (2)材料的指定和重量计算
首先右键材料, 编辑属性, 但是一般用于3D打印时, 往往需要自定义密度等等。
选用 Matrial > Edit Material, 常见的比如 **POM, PLA (聚乳酸), ABS 塑料**, 亚克力材料, 尼龙铝粉，陶瓷粉末,  竖直等等
![[attachments/Pasted image 20240707153235.png]]

也可以选中实体进行修改:
![[attachments/Pasted image 20240707153439.png]]
通过 Evaluate > Mass of Properties  可以观察获得的零件质量,  
![[attachments/Pasted image 20240707154244.png|800]]
一般对于3D打印, 往往存储需要的格式是 STEP 格式, 或者 STP 格式(最中性文件) 

## 四、装配体
### (1) 基本装配体
对于任何一个装配体都是一个窗口, 并且支持打开多个窗口。此时, 可以通过右上角进行放大缩小窗口
![[attachments/Pasted image 20240707161251.png|850]]
在装配体中可以进行 Open Part, 从 Window 选项中可以获取不同的 Window 属性, 并且可以使用 ctrl + tab切换
![[attachments/Pasted image 20240707161501.png]]
可以通过ctrl + 左键拖动零件到另一个窗口, 也可以ctrl 再按住项目树中的零件, 即可复制零件并添加到窗口中。 
注意: 在装配体中， 第一个零件会默认加上 f, 表示 fixed ，即第一个零件无法移动。此时,为了后续不产生更多问题, 需要**使其原点与坐标原点进行重合**。

首先, 点击第一个零件, 右键选取 float 
![[attachments/Pasted image 20240707162410.png]]
选取两个原点, 右键选择 Mate, 即可将第一个零件的原点定位到坐标系的原点上(这个是添加约束的方式), 然后右键第一个零件选择Fix即可
![[attachments/Pasted image 20240707163851.png]]
### (2) 配合约束的使用
最为常用的约束就是配合 (Mate),  可以将一个面连接到另一个面上。包括Standard, Advanced, Analysis和 Mechanical 配合约束。
![[attachments/Pasted image 20240707172706.png]]

常见的有共面， 同心等等


### (3) 高级约束的施加
约束的高级模式包括对称约束， 宽度，距离和角度约束等等; 常见的机械配合包括凸轮，槽口，齿轮配合等等; 

对称约束需要指定 Symmetry Plane 和需要指定对称的平面
![[attachments/Pasted image 20240707221851.png]]
对于宽度约束, 往往约束使某物块两个面与槽的两个面距离相等, 此时方块将位于槽的正中间。
首先选择需要对齐的两个面, 然后选择对齐到中央的两个面(Tab Reference)
![[attachments/Pasted image 20240707222617.png|800]]
此时我们即可对齐对象, 并实现精确放置在中心。 
![[attachments/Pasted image 20240707222715.png|800]]
也可以通过两个对象, 选用距离配合，**可以分别指定最大和最小的间距距离**,这样可以<mark style="background: transparent; color: red">实现两个面之间进行距离限制;角度配合也是同理。例如将最小距离设置为0,即可实现碰撞检测</mark> 
![[attachments/Pasted image 20240707222954.png]]
角度配合在实现如下图的面旋转限制部分非常有用。
![[attachments/Pasted image 20240707223856.png]]
另外也可以直接选择两个并进行快捷添加约束，选取时会自动弹出:
![[attachments/Pasted image 20240707224256.png]]
对于机械配合中的凸轮(cam)配合，往往使用于不规则曲面接触, 即不规则形状凸轮顶某个确定的直面的运动过程模拟(先选择凸轮面，再选取目标面)。

**槽口约束的创建**需要指定**一个槽内圆柱面和槽的内表面**,如下图:
![[attachments/Pasted image 20240707231039.png]]
另外，在装配和零件树中，任何一个部件都可以选用 Supress ， 具体即略去这一步的操作;
![[attachments/Pasted image 20240707231720.png]]

另外， 也可以通过添加装配体特征, 增加切除和孔等等的功能部分; 注意切除可以作用到多个物体上。
![[attachments/Pasted image 20240707232130.png]]
零部件的阵列与镜像操作可以在 Linear Components Pattern 中找到(选用Mirror Components 之后注意右上角的箭头next图标):
![[attachments/Pasted image 20240707234533.png]]

赋予材质的方法: 首先, 每一个零件可以单独指定其独立显示的颜色(直接右键设置), 装配中的颜色会覆盖原有的颜色。
对于材质, <mark style="background: transparent; color: red">在右侧菜单中，将相应材质拖拽到零件上，然后在弹出菜单中选择赋予到哪一层即可</mark>。 
![[attachments/Pasted image 20240707235550.png]]

