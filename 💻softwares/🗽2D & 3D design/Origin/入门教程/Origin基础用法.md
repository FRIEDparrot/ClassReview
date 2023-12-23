# 一、基本界面部分
项目管理器(book为表格, 而可以在上方选用graph文件
![[attachments/Pasted image 20231120000935.png]]
object Manager中显示了所有的图层， 方便
![[attachments/Pasted image 20231121203612.png|300]]

![[attachments/Pasted image 20231120001142.png]]

![[attachments/Pasted image 20231120001443.png]]
通过先新建图然后可以进行plot setup,  可以在画布中右键创建坐标轴
在Plot setup 中,yEr 表示曲线的误差, 而L表示是否添加数据标签(legend)
在不同的图层中可以添加不同的曲线，只需选中然后添加应用即可

论文插图为eps类型, 可以在导出时选取相应的类型 

帮助和部分教程可以从learning center中找到 
![[attachments/Pasted image 20231121203349.png]]
在一般情况下, 绘制图像时需要选中表格之后点击绘制即可。
在图像为最大化时， 切换回去只要选右上角即可
![[attachments/Pasted image 20231214210651.png|600]]

在画面中平移可以使用Tools中的手型工具， 除此之外, Tools 面板还提供了加入箭头, 直线, 文字工具等等说明。
![[attachments/Pasted image 20231214212350.png|600]]

# 二、手动绘制图像的基本方法
### 1. 在画布上新建图层
首先在右侧的空白处右键选择New Layer 
![[attachments/Pasted image 20231214210249.png|700]]

### 2. 调节空白和坐标轴的位置分布
双击坐标框, 如图, Size中有调节的方法, 
![[attachments/Pasted image 20231214211819.png|800]]
### 3. 在图表中插入数据
首先在图中找到plot setup 的部分, 
![[attachments/Pasted image 20231214212717.png|400]]
在弹出的部分中没有数据， 选用show plot Designation 
![[attachments/Pasted image 20231214212953.png|800]]
使用Show Available Data 再次展开上述数据, 则得到
![[attachments/Pasted image 20231214213154.png|700]]
最终的绘图设置区域如图所示, 然后按下图则有数据显示出(一般也可以选择为Worksheet in Project直接选工程中的所有数据表)。
![[attachments/Pasted image 20231214213904.png|500]]

将数据绘制成图的操作有如下的三步: 
![[attachments/Pasted image 20231214214313.png|800]]

### 4. 坐标轴, 刻度线和网格线设置
其中Major Ticks 是主刻度步进长度, 而Minor Ticks 是次刻度的步进长度
![[attachments/Pasted image 20231214215316.png|900]]
网格线可以在坐标轴的部分找到
![[attachments/Pasted image 20231214220021.png|700]]
对于整个图是否有刻度线, 在Line and Ticks选项中。在左边设置四个坐标轴, 从而获得轴线和边框。
![[attachments/Pasted image 20231214220548.png|800]]
对于不需要刻度出现的纯边框, 只需要设置Style即可。
![[attachments/Pasted image 20231214220813.png|800]]

# 三、其他常用绘图工具
Object 工具栏可以增加颜色条, 重构Legend标签(根据图线内容重构标签), 和增加表等内容。
![[attachments/Pasted image 20231214221422.png|700]]

# 四、绘图样式与颜色设置

对于柱形图, 可以按照如下的方法设置渐变色, 从而实现更加适合的绘制
![[attachments/Pasted image 20231214222621.png|900]]

