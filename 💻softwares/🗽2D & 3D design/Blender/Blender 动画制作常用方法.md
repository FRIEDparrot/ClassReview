插入关键帧时，在对应的部位使用右键或者直接开启自动关键帧模式
或者在位置和缩放属性上使用 i 键， 可以插入部位关键帧
![[attachments/Pasted image 20230822222655.png|600]]

在动画栏Animation tab 中， 比较重要的是Dope Sheet栏， 通过此面板可以控制每一个部位的关键帧部分
![[attachments/Pasted image 20230822222757.png|1200]]


先设置一个相应的姿势， 然后按 i 键， 选取whole character 或者下面的bones only

![[attachments/Pasted image 20230822223629.png]]

下方和上方的两个时间轴， 选中之后，==g键进行平移，s键进行缩放==
![[attachments/Pasted image 20230822224237.png]]

另外可以使用graph Editor 来进行插值曲线编辑部分

# 循环动画的制作
对于循环的走路等动画制作， 

首先shift + a 添加动画的底盘, 并
![[attachments/Pasted image 20230823114522.png|800]]
自动关键帧在动画的下方
![[attachments/Pasted image 20230823114646.png|600]]

在关键帧中， 打到第一帧和第11帧分别采用两个不同的动画
点击动画部分右上角的箭头， 仅显示左脚部分
按左边的箭头进入==位置移动模式==， 并使用==shift + D==进行复制左脚位置, 将原本第1帧的位置部分粘贴到第11帧上， 保证行走过程中， 左脚的位置不发生变化
![[attachments/Pasted image 20230823121424.png]]

![[attachments/Pasted image 20230823121825.png]]

注意使用Key中的Copy KeyFrames 可以将姿势进行复制粘贴操作    
![[attachments/Pasted image 20230823124137.png|700]]

如果是制作出在同一位置的动画时， 可以选中下方的盘进行拖动创建底盘的关键帧，从而实现在循环跑步动画的过程中身体的移动
![[attachments/Pasted image 20230823124903.png|1000]]
另外注意的是， 往往需要采用线性的插值方式
![[attachments/Pasted image 20230823125251.png|1100]]

效果如图所示: 
![[attachments/Pasted image 20230823125323.png|600]]

# NLA 编辑器笔记

即使用Nonlinear Animation进行编辑动画
![[attachments/Pasted image 20230823125613.png]]