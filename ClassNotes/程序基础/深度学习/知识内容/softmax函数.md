#### 1. softmax函数
softmax函数可以进行归一化，根据三个数值的相对大小,将其变为和为1的概率
$$S_{ij} = \frac{e^{z_i}}{\sum_k e^{z_k}}$$
-  𝑆𝑖是经过softmax的类别概率输出
-  𝑧_𝑘是神经元的输出

不同的值反映出取不同的部分的概率

#### 2. 交叉熵损失函数

$$C = -\underset{i}{\sum} y_iln\hat{y}_i$$
