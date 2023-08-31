MeshSwap 用于创建不完整方块

在设置MeshSwap 中需要注意的是MeshSwap需要消耗大量性能，因此需要仅选取离镜头近的物体进行动画渲染
![[attachments/Pasted image 20230822204953.png|800]]

可以调整至z轴, 并进入Edit 模式。 并保证对应的选取是在Face Select 捕捉选取模式下

![[attachments/Pasted image 20230822205844.png]]

打开右上角的x-ray渲染模式，并使用B进行框选选中摄像头附近的内容(保证z轴上的所有部分均被框选进去)
![[attachments/Pasted image 20230822210008.png|800]]
然后在编辑模式下使用p进行分离， 选择separate selection.
![[attachments/Pasted image 20230822210259.png]]
然后就可以在Object 模式下进行单独选取对应的选区了， 

另外也可以通过拖拽的方法将对应的物品放进Meshswap Render 中, 进行Mesh Swap的渲染, 需要注意这个比较耗费性能
![[attachments/Pasted image 20230822211807.png|1000]]
使用MeshSwap的好处是可以渲染其他类似于具有浮雕的方块


# 更换自定义材质包的渲染方法

首先可以将材质包解压之后， 放在blender工程的相对路径之下。 

在全选世界中的部分之后， 选择 swap texture pack 选项
![[attachments/Pasted image 20230822214820.png|750]]

可以在相应的界面进行拖拽对应的材质包进这个界面进行添加材质包， 
![[attachments/Pasted image 20230822215017.png|800]]
另外可以使用PBR贴图纹理和法向贴图设置如下：设置完毕之后， 点击swap 即可
![[attachments/Pasted image 20230822215132.png|800]]

在shading 选项栏， 我们也可以选中不同的物体，对其渲染方式进行操作。
![[attachments/Pasted image 20230822215804.png|800]]
如果是对于特定的物品有特定材料情况，则可以在MCprep栏找到对应的item选项卡，只需应用之后再次点击Material即可


# 动态贴图的渲染和设置部分

对于水源和岩浆等等方块，可以从tools 选项卡中的Advanced 条中， 找到Animate textures 选项

![[attachments/Pasted image 20230822220642.png]]
全选场景并启用Animate texture 选项， 则改用动态贴图

动态贴图并不会在pack all into .blend 中进行打包

如果需要进行导出， 在此选项中，勾选Next to this blend file 来另行保存和导出。
![[attachments/Pasted image 20230822222107.png|800]]
