# 1. SpaceClaim 梁单元建模
在Workbench 的SpaceClaim 中，梁单元总会自动转化为面片单元, 可以使用如下的方法建立梁单元
在从CAD导入杆单元之后，效果如图
![[attachments/Pasted image 20230528150042.png|500]]
Link1 二维杆单元
Link8 三维杆单元
Link10 三维仅受拉伸或者压缩单元
如图， 使用Prepare中的Offset 选项，为梁赋予截面即可
![[attachments/Pasted image 20230528150601.png|800]]
如图，赋予截面之后，导入的部分在Mechanical 中如图所示，只需要在左侧的section中定义对应的截面属性即可
![[attachments/Pasted image 20230528151029.png|700]]


# DesignModel 中的梁单元建立

Concepts > Cross Section > Circular 添加一个横截面
![[attachments/Pasted image 20240323121935.png]]
然后选用Concepts > Lines From Sketches 从草图创建线体
![[attachments/Pasted image 20240323122224.png|700]]
最后在 Part, Body 中选用创建的 Line Body , 然后将横截面关联即可
![[attachments/Pasted image 20240323122440.png|800]]
显示横截面时， 选用 View > Cross Section Solids 
![[attachments/Pasted image 20240323122635.png]]
