## 1. 卷积计算模块
卷积在 PyTorch 中有两种方式，一种是 `torch.nn.Conv2d()`，一种是 `torch.nn.functional.conv2d()`，这两种形式本质都是使用一个卷积操作。

这两种形式的卷积对于输入的要求都是一样的，首先需要输入是一个 `torch.autograd.Variable()` 的类型，大小是 `(batch, channel, H, W)`，其中 `batch` 表示输入的一批数据的数目，第二个是输入的通道数，一般一张彩色的图片是 3，灰度图是 1，而卷积网络过程中的通道数比较大，会出现几十到几百的通道数，`H` 和 `W` 表示输入图片的高度和宽度，比如一个 `batch` 是 32 张图片，每张图片是 3 通道，高和宽分别是 50 和 100，那么输入的大小就是 `(32, 3, 50, 100)`

例如导入torch的autograd模块：

```python
import numpy as np
import torch
from torch import nn
from torch.autograd import Variable
import torch.nn.functional as F
from PIL import Image
import matplotlib.pyplot as plt

im = Image.open('./cat.png').convert('L') # 读入一张灰度图的图片
im = np.array(im, dtype='float32') # 将其转换为一个矩阵
# 将图片矩阵转化为 pytorch tensor，并适配卷积输入的要求
im_t = torch.from_numpy(im.reshape((1, 1, im.shape[0], im.shape[1]))) # 将图片转换为toech表示的张量

# 可视化图片
fig = plt.imshow(im.astype('uint8'), cmap='gray')
fig.figure.savefig('fig-res-1-basic_conv_4_1.pdf')

# 使用 nn.Conv2d （in_channels, out_channels, kernel_size）
conv1 = nn.Conv2d(1, 1, 3, bias=False) # 定义卷积

# ---------------------定义卷积部分------
sobel_kernel = np.array(
    [[-1, -1, -1], 
     [-1, 8, -1], 
     [-1, -1, -1]], dtype='float32') # 定义轮廓检测算子(卷积核)
sobel_kernel = sobel_kernel.reshape((1, 1, 3, 3)) # 适配卷积的输入输出
conv1.weight.data = torch.from_numpy(sobel_kernel) # 给卷积的 kernel 赋值

edge1 = conv1(Variable(im_t)) # 将卷积作用在图片的pytorch张量上,获取新的卷积图
# ----------------使用conv1函数进行卷积-----
edge1 = edge1.data.squeeze().numpy() # 将输出转换为numpy数组格式(图片的格式)
# 这样就可以使用 plt.imshow 进行显示了
fig = plt.imshow(edge1, cmap='gray') # 显示边缘检测后的轮廓图
```

上述过程中，如果使用 `F.conv2d` 方法，可以将

```python
# 使用 F.conv2d
sobel_kernel = np.array([[-1, -1, -1], [-1, 8, -1], [-1, -1, -1]], dtype='float32') # 定义轮廓检测算子
sobel_kernel = sobel_kernel.reshape((1, 1, 3, 3)) # 适配卷积的输入输出
weight = Variable(torch.from_numpy(sobel_kernel))

edge2 = F.conv2d(Variable(im), weight) # 作用在图片上
edge2 = edge2.data.squeeze().numpy() # 将输出转换为图片的格式
```

不同的地方在于使用 `nn.Conv2d()` 相当于直接定义了一层卷积网络结构，而使用 `torch.nn.functional.conv2d()` 相当于定义了一个卷积的操作。

使用 `nn.Conv2d()` 会默认定义一个随机初始化的 weight，如果需要修改，那么取出其中的值对其修改，如果不想修改，那么可以直接使用这个默认初始化的值，非常方便。

**实际使用中我们基本都使用 `nn.Conv2d()` 这种形式**

## 2. 池化层
池化即为使图片大小减小的操作，

卷积网络中另外一个非常重要的结构就是 `池化`，这是利用了图片的下采样不变性，即一张图片变小了还是能够看出了这张图片的内容，而使用池化层能够将图片大小降低，非常好地提高了计算效率，同时池化层也没有参数。池化的方式有很多种，比如<mark style="background: transparent; color: yellow">最大值池化，均值池化等等，在卷积网络中一般使用最大值池化。</mark>

在 PyTorch 中最大值池化的方式也有两种，一种是 `nn.MaxPool2d()`，一种是 `torch.nn.functional.max_pool2d()`，他们对于图片的输入要求跟卷积对于图片的输入要求是一样的，就不再赘述，下面举例说明。

```python
# 使用 nn.MaxPool2d
pool1 = nn.MaxPool2d(2, 2)
print('before max pool, image shape: {} x {}'.format(im.shape[2], im.shape[3]))
small_im1 = pool1(Variable(im))
small_im1 = small_im1.data.squeeze().numpy()
print('after max pool, image shape: {} x {} '.format(small_im1.shape[0], small_im1.shape[1]))

fig = plt.imshow(small_im1, cmap='gray')
```

**跟卷积层一样，实际使用中，我们一般使用 `nn.MaxPool2d()`**

## 3. LeNet5 卷积模块

LeNet5 的架构的提出是基于如下的观点：==图像的特征分布在整张图像上，通过带有可学习参数的卷积，从而有效的减少了参数数量，能够在多个位置上提取相似特征==。

在LeNet5提出的时候，没有 GPU 帮助训练，甚至 CPU 的速度也很慢，因此，LeNet5的规模并不大。其包含<mark style="background: transparent; color: yellow">七个处理层，每一层都包含可训练参数（权重）</mark>，当时使用的输入数据是 32×3232×32 像素的图像。LeNet-5 这个网络虽然很小，但是它包含了深度学习的基本模块：==卷积层，池化层，全连接层==。它是其他深度学习模型的基础，这里对LeNet5进行深入分析和讲解，通过实例分析，加深对与卷积层和池化层的理解。

```python
import torch
from torch import nn
import torch.nn.functional as F

# 自定义一个LeNet5类
class LeNet5(nn.Module):
    def __init__(self):  # 定义方法是首先继承LeNet5中的类，然后在init中定义全连接层
        super(LeNet5, self).__init__()
        # 1-input channel, 6-output channels, 5x5-conv
        self.conv1 = nn.Conv2d(1, 6, 5)  # 1 x 6 x 5 中间层5个,输入为1,输出为6
        # 6-input channel, 16-output channels, 5x5-conv
        self.conv2 = nn.Conv2d(6, 16, 5) # 6-> 16 
        # 16x5x5-input, 120-output
        
        # 注意这个是Linear的层, 
        self.fc1 = nn.Linear(16 * 5 * 5, 120) 
        # 120-input, 84-output
        self.fc2 = nn.Linear(120, 84)
        # 84-input, 10-output
        self.fc3 = nn.Linear(84, 10)
		
	    # LeNet5中有 
    def forward(self, x):  # 定义前向池化层
        x = F.max_pool2d(F.relu(self.conv1(x)), (2, 2))
        x = F.max_pool2d(F.relu(self.conv2(x)), (2, 2))
        x = torch.flatten(x, 1) # 将结果拉升成1维向量，除了批次的维度
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = self.fc3(x)
        return x

net = LeNet5()
print(net)
```

---

```python 


```

