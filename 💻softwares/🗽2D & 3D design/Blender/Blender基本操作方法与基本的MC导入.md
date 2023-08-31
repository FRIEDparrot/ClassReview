中间平移, shift + 中键旋转
选中物体之后, GRS分别是平移，旋转和缩放

按T和N键可以打开左右侧边栏(右侧边栏里面有mcprep插件部分), B选取
![[attachments/Pasted image 20230817182009.png|500]]
如图， 每个窗口左上角都有对应的其他窗口， 点击可以切换到不同的窗口上， 通过在窗口边缘进行右键点击可以进行分割，从而适应不同的窗口操作
![[attachments/Pasted image 20230817183007.png|900]]

alt + H可以在侧边栏将选中的隐藏物品显示出来
shift + C重新聚焦焦点 
shift + ~ 可以切换为飞行模式， 此时可以通过WASDQE控制前后左右移动和上升下降

在Mineways中==右键拖拽==可以实现选取对应的导出范围, 并使用Export for rendering 进行导出为obj类型文件
![[attachments/Pasted image 20230817185429.png|280]]

地图也可以通过Jmc2Obj来导出为.jar文件， 注意需要勾选Create a separate object for each material 和Don't allow duplicate vertexes 选项，并可以关闭Render word sides & bottom选项
![[attachments/Pasted image 20230821160952.png|600]]

在导入世界之后，使用a选中所有， 并使用alt + a取消选择
选中之后使用prep materials并更改对应的选项，可以在对应部分显示贴图
![[attachments/Pasted image 20230821162558.png|500]]
通过MCprep Materials,的如下设置，可以对材料进行显示设置
![[attachments/Pasted image 20230821162737.png|600]]

基本生物的生成：
首先选取页面左边的3D光标，并将3D光标设置在场景中的相应位置, 在左上选取Add ->MCprep `->` Load Spawner`->` 再找一次(shift + a)，即可生成对应的生物
![[attachments/Pasted image 20230821164428.png|650]]
生物生成之后，使用左下角的mob spawner 可以选择Clear Pose 
![[attachments/Pasted image 20230821170216.png]]

$z$更改渲染部分 

需要说明， 我们往往使用Simple Player 来进行导入
如果需要使用带有表情的脸， 则使用Player , 否则使用Simple Player 

切换视角使用`键
![[attachments/Pasted image 20230821175007.png]]

移动， 旋转等等都可以通过shift + space 弹出工具栏进行选择
需要注意：通过上方的磁铁和格点可以进行类似于3ds Max的捕捉设置， 如果开启捕捉，则移动过程中会进行自动捕捉
![[attachments/Pasted image 20230822181039.png|500]]

也可以通过如图的方法进行选取捕捉到何位置
![[attachments/Pasted image 20230822181758.png|600]]
在选择模式下， 使用w可以切换游标状态


12345用于全局隐藏和显示
shif + s 用于进行捕捉
![[attachments/Pasted image 20230822204320.png]]

shift + o 用于进行设置插值方法
![[attachments/Pasted image 20230822204532.png]]

z : 调整渲染
![[attachments/Pasted image 20230822205726.png|500]]
b ： 进行框选

edit 模式下: 使用p是seperate 

i 键可以显示某个按钮上的帮助
