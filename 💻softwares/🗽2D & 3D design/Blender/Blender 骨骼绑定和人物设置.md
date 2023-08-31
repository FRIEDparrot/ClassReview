# 人物模型修复和层次设置
通过shift + ctrl 在右边选中整个Player 部分
![[attachments/Pasted image 20230821172917.png|800]]
对于Player选项， 往往会由于贴图上画有眼睛导致一系列问题， 从而我们可以通过
![[attachments/Pasted image 20230821172537.png]]
此时需要通过编辑贴图进行修改
![[attachments/Pasted image 20230821172730.png]]

方法是在texture Paint 栏找到对应的贴图，覆盖掉原有的眼睛等结构即可
![[attachments/Pasted image 20230821173412.png|700]]
在涂抹完毕之后，选取save `->` reload 进行重新加载
![[attachments/Pasted image 20230821173712.png]]

通过 可以移动对应的层， 而如果第二层不需要， 可以删除
![[attachments/Pasted image 20230821173938.png]]

对于如图的第二层头部， 可以使用Edit Mode进行切割。此时使用
![[attachments/Pasted image 20230821174353.png]]
Edit Mode -> 在MCprep中找select Alpha Faces `->` 使用 x键删除
![[attachments/Pasted image 20230821174739.png]]
删除之后效果如图所示， 可以使用e键进行拉伸面
![[attachments/Pasted image 20230821175223.png|300]]

# 骨骼绑定和动作技巧
骨骼绑定(rig) 是在人物的arma 部分
![[attachments/Pasted image 20230821175713.png|350]]

在选中对应的骨骼之后，改用Pose Mode进行编辑动作(ctrl + tab) 
![[attachments/Pasted image 20230821180148.png|800]]
选中左边的Move 图标，就可以使用GRS控制身体部位的大小和旋转了
![[attachments/Pasted image 20230821180657.png|700]]
注意使用
![[attachments/Pasted image 20230821181504.png]]
此处说明Inverse kinematics 的使用方法
通过G可以移动虚拟控制中的游标， 从而实现对于腿的控制
![[attachments/Pasted image 20230821183135.png|350]]

# 物品的生成

在生成物品之后， 可以通过此键进行调整， 也可以调整对应的大小，也可以通过右侧边设置面板进行调整
![[attachments/Pasted image 20230822182656.png|800]]

Improve UI 选项可能可以解决贴图错误问题

通过更改resources pack, 可以方便加载大量贴图。
![[attachments/Pasted image 20230822184200.png]]

将一个物品绑定到人物身上的步骤：
首先选取对应的物品， 然后通过 **ctrl `->` 选取骨骼部分**,  使用shift + tab 进入pose mode, 选中相应的骨骼
![[attachments/Pasted image 20230822184535.png|800]]
然后使用 ==ctrl + p==, 选取bone , 即将物品绑定到了骨骼上， 此时移动对应的部分， 物品就会和骨骼一起移动了。
![[attachments/Pasted image 20230822185534.png|600]]

# 人物服装的更换方法
初始配置如图， 只需点击neitherite 前面的图纸，选中，然后删除即可
![[attachments/Pasted image 20230822200158.png|1000]]
在之后只需进行绑定和