点击右上角的摄像机可以切换成摄像机的视角
![[attachments/Pasted image 20230822194653.png|500]]
点击物品右侧的组件部分， 然后即弹出对应的属性，此时即可对摄像机的属性进行修改，如调节焦距等等
![[attachments/Pasted image 20230822195024.png|1000]]

将右侧边栏切换成 Render properties, 然后即可对Render Engine等等进行调整
![[attachments/Pasted image 20230822195247.png|700]]

其中 Eevee 和 Cycles 的优缺点如下: （Cycles需要选取GPU compute) 
![[attachments/Pasted image 20230822200813.png|1000]]

在切换渲染引擎之后， 需要全选所有物体并重新在Material Prep 中再点击一下OK。 
在右上角选中第四个Viewport Shading 进行渲染预览。
![[attachments/Pasted image 20230822201128.png]]
如图在右下角选中出图按钮， 即可对场景的渲染进行相应的摄像机渲染操作。
![[attachments/Pasted image 20230822201916.png|1000]]
通过菜单进行渲染图片(或者使用F11是预览渲染， 重新渲染使用F12)
![[attachments/Pasted image 20230822202119.png]]
需要说明对应的线程数的选择的问题
![[attachments/Pasted image 20230822202746.png|500]]

注意也可以从上方选择render animation, 从而得到对应的动画
注意在渲染之前要在动画条右上角选取对应的帧的范围
![[attachments/Pasted image 20230822203211.png|400]]
