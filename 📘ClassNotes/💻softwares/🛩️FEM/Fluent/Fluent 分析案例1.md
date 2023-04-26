Fluent的基本的创建结构的几种模式，可以进行Results后处理， 并点击setup进入fluent界面

![[attachments/Pasted image 20230425170832.png|700]]

我们往往可以使用第三种方式(Fluent with Meshing) 通过这个可以建立网格， 可以通过上面的滑块调整截面位置

工作界面如下(其中，我们可以使用上面的report查看网格的数量，节点的数量等等)
![[attachments/Pasted image 20230425212034.png|700]]

最后单击左上角 Switch to solution mode可以进入求解界面
在求解部分配置完毕之后，直接在求解界面的通用设置中即可设置网格一般参数
<mark style="background: transparent; color: yellow">首先往往在scale中将网格单位设置为mm</mark>，其次，可以通过通用设置部分**检查网格质量**


对于**湍流模型**，我们可以==使用$k-\varepsilon$模型==进行计算

对于创建还是覆盖，可以在材料创建之后，可以选择是否覆盖或者不覆盖

可以在左侧的单元区域条件中增加源项

求解之前，需要在==Initialization==中点击初始化Initiate按钮， 可以将求解进行混合初始化。同时，如果需要<mark style="background: transparent; color: yellow">配置初始条件</mark>，可以双击左边的初始化List

在配置**Calculation Activities**中，可以设置每一次保存的迭代步数， 在求解前需要输入迭代的总的次数，然后进行执行计算
![[attachments/Pasted image 20230426010027.png|600]]

在进行结果处理时，使用result -> surface 上右键选择 add -> plane 即可在工作空间中创建平面， 而绘图部分是在下方的Graphics中进行实现的。 
![[attachments/Pasted image 20230426010907.png|250]]
通过上述绘制迹线图像可以得到如下绘制结果
![[attachments/result.png|500]]
可以在report中增加对表面积分和体积积分的求解和计算
![[attachments/Pasted image 20230426012028.png|500]]
