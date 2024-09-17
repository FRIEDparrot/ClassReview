# 1. Sklearn 库的使用 
sklearn 是最常用的机器学习库, 其包含分类, 回归算法, 聚类算法, 维度化简, 模型选择, 交叉验证, 数据预处理等等功能. 例如岭回归， 支持向量机, KNN , 朴素贝叶斯, 决策树, 特征选择, 保序回归等等算法。
### (1) sklearn 中的 Bunch 类
Bunch 是 sklearn 中最常用的结构, 类似于字典，具体参考 [[📘ClassNotes/⌨️Programming/🐍Python/2. Python 基本数据结构和可视化方法|2. Python 基本数据结构和可视化方法]] 
```python 
from sklearn.datasets._base import Bunch
```
一般地, 可以通过
```python
print(bunch.keys())
```
获取其中的keys键, 对于bunch的keys部分均可以通过 `.` 进行访问。

> [!CAUTION] scipy 中的 toarray 方法
> 需要注意的是, scipy.spare.toarray() 和 numpy.array 并不相通, 稀疏矩阵可以通过 toarray() 方法进行转换为标准的 numpy 数组


### (2) 针对训练集和测试集常用部分
sklearn 可以直接获取大量的学习数据集， 同时有分割训练集和测试集功能
```python 
from sklearn.dataset import load_iris 
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler   # 标准化器类型 

X_train, X_test, y_train, y_test =  train_test_split(iris_data, iris_target, test_size=0.2, random_state=None)  
```

对于测试准确度, 可以采用 [sklearn.metrics 模块](https://scikit-learn.org/stable/api/sklearn.metrics.html)部分: 
```python
from sklearn.metrics import accuracy_score, classification_report 
from sklearn.metrics import precision score, recall_score 
from sklearn.metrics import f1_score  # F1标准, 参考sklearn 部分
```

[`accuracy_score`](https://scikit-learn.org/stable/modules/generated/sklearn.metrics.accuracy_score.html#sklearn.metrics.accuracy_score "sklearn.metrics.accuracy_score") ,[`average_precision_score`](https://scikit-learn.org/stable/modules/generated/sklearn.metrics.average_precision_score.html#sklearn.metrics.average_precision_score "sklearn.metrics.average_precision_score"), [`balanced_accuracy_score`](https://scikit-learn.org/stable/modules/generated/sklearn.metrics.balanced_accuracy_score.html#sklearn.metrics.balanced_accuracy_score "sklearn.metrics.balanced_accuracy_score"), [`brier_score_loss`](https://scikit-learn.org/stable/modules/generated/sklearn.metrics.brier_score_loss.html#sklearn.metrics.brier_score_loss "sklearn.metrics.brier_score_loss") , 

此外, 测试 MSE, MAE 也可以采用 sklearn,metrics 的如下包进行获取:

| [`mean_squared_error`](https://scikit-learn.org/stable/modules/generated/sklearn.metrics.mean_squared_error.html#sklearn.metrics.mean_squared_error "sklearn.metrics.mean_squared_error")                 | Mean squared error regression loss.(MSE)        |
| --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------- |
| [`mean_squared_log_error`](https://scikit-learn.org/stable/modules/generated/sklearn.metrics.mean_squared_log_error.html#sklearn.metrics.mean_squared_log_error "sklearn.metrics.mean_squared_log_error") | Mean squared logarithmic error regression loss. |
| [`mean_absolute_error`](https://scikit-learn.org/stable/modules/generated/sklearn.metrics.mean_absolute_error.html#sklearn.metrics.mean_absolute_error "sklearn.metrics.mean_absolute_error")             | Mean absolute error regression loss.(MAE)       |

sklearn 标签编码: 例如对于如下决策树表格, 其中第一列不需要编码:
![[attachments/Pasted image 20240912111152.png]]
其中采用 apply_map 的方法, 去除了每个字符串元素的空格; 同时采用strip去除了每个标签的空格。  
```python
import pandas as pd  
from sklearn.preprocessing import LabelEncoder  # encoder labels  
  
data_raw = pd.DataFrame(pd.read_excel("seals_data.xlsx"))  
data_proceed = pd.DataFrame()  
label_encoder = LabelEncoder()  
  
# eliminate all the white space  
data_raw = data_raw.applymap(lambda x: x.strip() if isinstance(x, str) else x)  
  
data_proceed['计数'] = data_raw.iloc[:,0]  
  
for column in data_raw.columns[1:]:  
    data_proceed[str(column).strip()] = label_encoder.fit_transform(data_raw[column])  
print(data_proceed)
```
得到决策树编码结果如下:
![[attachments/Pasted image 20240912113356.png]]

### (3) 特征提取和分类 
```python
# 特征选择和特征提取部分
from sklearn import feature_extraction
from sklearn import feature_selection 
```
其中特征提取中有 image, text 等等。
![[attachments/Pasted image 20240910160939.png]]
对于多标签分类部分, 需要采用多输出
```python
from sklearn.multioutput import MultiOutputClassifier, MultiOutputEstimator
# 将一个部分转换为多输出:
model = MultiOutputClassifier(MultinomialNB(alpha=0.01))
```

主成分分析:
```python 
from sklearn.decomposition import PCA
```

```python 
from sklearn.inspection import DecisionBoundaryDisplay 
```


# 2. Pytorch
## 一、基本数据操作
#### 1. unsqueeze,squeeze和reshape使用
增加维度或者减少维度: 
torch.squeeze(input,dim=0) **移除某个维度**
torch.unsqueeze(input,dim=0) **在某个维度上增加维度** 
reshape 的使用: 在 pytorch 等中, 我们常常使用 reshape 将数据集转换为指定的形状, 例如对于 (60000, 28,28) 的数据集, 我们会使用 `x_train.reshape(x_train.shape[0], x_train.shape[1], x_train.shape[2], 1)` 得到 (60000,28,28,1) 的4d数据集, 即将最后的28个中, 每个均作为输入拆分开;

```python title:squeeze使用示例
train[1].unsqueeze(0).shape
Out[12]: torch.Size([1, 28, 28])
A = train[1].unsqueeze(0).unsqueeze(0)
A.shape
Out[13]: torch.Size([1, 1, 28, 28]) 
A.squeeze(0).shape
Out[17]: torch.Size([1, 28, 28])
A.squeeze(0).squeeze(0).shape
Out[19]: torch.Size([28, 28])
A.squeeze([0,1]).shape
Out[22]: torch.Size([28, 28])
B.unsqueeze(dim=2).shape
Out[28]: torch.Size([28, 28, 1])
```

另外，pytorch中大多数的操作都支持 `inplace` 操作，也就是可以直接对 tensor 进行操作而不需要另外开辟内存空间，方式非常简单，一般都是在操作的符号后面加`_` 

#### 2. 常用函数集和运算 
**torch.nn.functional** 包含了**绝大多数的常用函数**: 例如 relu， softmax, sigmoid 等等。

```python
def SoftmaxRegression(nn.Module):
	self.w = torch.normal(0,simga, size =(num_inputs, num_outpus), requires_grad=True)
	self.b = torch.zeros(num_outputs, requires_grad = True)
	self.optim = torch.optim.SGD(self.parameters(), lr = 0.01)
	
	 def parameters(self) : 
		return [self.w, self.b]
	 def forward(self):
		 return softmax(torch.matmul(X.reshape((-1,  self.W.shape[0])),  self.W) + self.b)
```

对于 minist 数据集, 由于x的维度为 784, num_inputs = 784, num_outputs = 10. 即 w 为一个 784 x 10 的权重矩阵。

torch.gather(): 取某个tensor中指定下标的元素, 但是需要使用如下方法指定:
```Python
For a 3-D tensor the output is specified by: 
out[i][j][k] = input[index[i][j][k]][j][k]  # if dim == 0
out[i][j][k] = input[i][index[i][j][k]][k]  # if dim == 1
out[i][j][k] = input[i][j][index[i][j][k]]  # if dim == 2
```

#### 3. 取某些行或者某些列的元素(torch.gather)
例如gather(dim = 0) 是按行取元素, 
```python
a = tensor([[ 1,  2,  3,  4,  5], [ 6,  7,  8,  9, 10]])
a.gather(0, torch.tensor([[0, 1],[0,1]]))   # 第一列进行gather, 
# tensor([[1, 7], [1, 7]]) 
a.gather(1, torch.tensor([[0, 1],[0,1]]))
```
当 dim = 0 时, 取法为取指定行相应列的元素
$$\left[\begin{matrix}
0  &|&1 \\  0&  |  & 1
\end{matrix}\right]$$
dim = 0 时, 取法为取指定列相应行的元素。第一列取 0 行, 0行, 第二列取1行, 1行; 而
$$\left[\begin{matrix}
0  &1   \\   -  &  -  \\  0  & 1
\end{matrix}\right]$$
此时得到的是`[[1,2],[6,7]]`。 
**说明: 最常见的应用是在交叉熵的计算中, 获取到的概率矩阵 y_pred(例如784* 10), 正确目标是离散的点 y(0,1,2,3..9), 此时需要y_pred中,y所对应列的部分**, 即可采用:
```python 
y_pred.gather(1, y.view(-1, 1)) 
```

但实际上用的多的还是如下的方法: 
```python
criterion =  nn.CrossEntropyLoss()
y_pred = model(X)
loss = criterion(y_pred, y)
```


### (2) 基本求导方法
#### 1. 反向传播算法求导
```python
m = torch.tensor([[2, 3]], dtype=torch.float, requires_grad=True) # 构建一个 1 x 2 的矩阵
# 注意：[[]]是定义行向量，而[]定义列向量
n = torch.zeros(1, 2) # 构建一个相同大小的 0 矩阵
print(m)
print(n)
# 通过 m 中的值计算新的 n 中的值
print(m[0,0])
n[0, 0] = m[0, 0] ** 2
n[0, 1] = m[0, 1] ** 3
print(n)

n.backward(torch.ones_like(n)) # 将 (w0, w1) 取成 (1, 1), 进行自动求导
print(m.grad)
# tensor([[ 4., 27.]], grad_fn=<CopySlices>)
```
将上面的式子写成数学公式，可以得到: 
$$
n = (n_0,\ n_1) = (m_0^2,\ m_1^3) = (2^2,\ 3^3) 
$$
下面我们直接对 `n` 进行反向传播，也就是求 `n` 对 `m` 的导数。
这时我们需要明确这个导数的定义，即如何定义
$$
\frac{\partial n}{\partial m} = \frac{\partial (n_0,\ n_1)}{\partial (m_0,\ m_1)}
$$
$$
\frac{\partial n}{\partial m_0} = w_0 \frac{\partial n_0}{\partial m_0} + w_1 \frac{\partial n_1}{\partial m_0} = 2 m_0 + 0 = 2 \times 2 = 4
$$
$$
\frac{\partial n}{\partial m_1} = w_0 \frac{\partial n_0}{\partial m_1} + w_1 \frac{\partial n_1}{\partial m_1} = 0 + 3 m_1^2 = 3 \times 3^2 = 27
$$

#### 2. 多次自动求导
通过调用 backward 我们可以进行一次自动求导，如果我们再调用一次 backward，会发现程序报错，没有办法再做一次。这是因为 PyTorch 默认做完一次自动求导之后，计算图就被丢弃了，所以两次自动求导需要手动设置
```python
x = torch.tensor([3], dtype=torch.float, requires_grad=True)
y = x * 2 + x ** 2 + 3
print(y)
y.backward(retain_graph=True) # 设置 retain_graph 为 True 来保留计算图
print(x.grad)
```

> [!CAUTION] 梯度归零
> 注意：如果是循环迭代求解梯度的情况下, 我们希望每一次计算并仅求解一次梯度，则求导完毕之后, 需要使用grad.data.zero_()来归零梯度，否则将会使梯度累加。

```python
while (1) : 
	w.requires_grad_(True);  # ask for the space for gradient.  
	b.requires_grad_(True);  #  
	try:  
	    w.grad.zero_()  
	    b.grad.zero_()  
	except:  
	    pass
```

## 二、基本回归模型
### (1)多项式回归模型
下面更进一步尝试一下多项式回归，下面是关于 x 的多项式：
$$
\hat{y} = w_0 + w_1 x + w_2 x^2 + w_3 x^3 
$$
这样就能够拟合更加复杂的模型，这里使用了 $x$ 的更高次，同理还有多元回归模型，形式也是一样的，只是除了使用 $x$，还是更多的变量，比如 $y$、$z$ 等等，同时他们的 $loss$ 函数和简单的线性回归模型是一致的。 
```python
import numpy as np  
import torch
import matplotlib.pyplot as plt
# 定义一个多变量函数
w_target = np.array([0.5, 3, 2.4]) # 定义参数
b_target = np.array([0.9]) # 定义参数

f_des = 'y = {:.2f} + {:.2f} * x + {:.2f} * x^2 + {:.2f} * x^3'.format(
    b_target[0], w_target[0], w_target[1], w_target[2]) # 打印出函数的式子

print(f_des)
```

我们下面以 w0 + w1 + w2 + b = y_sample 的式子, 以 randn 作为 w 初始值, 0 作为b 初始值进行小批量梯度算法求解获取结果(定义的两个函数中, 一个是模型, 一个是损失函数): 
```python
import numpy as np  
import matplotlib.pyplot as plt  
import torch  
  
fig = plt.figure()  
epochNum = 80  
lr = 0.001  

x_sample = np.arange(-3, 3.1, 0.1)    # 定义绘画数组区间  
y_sample = b_target[0] + w_target[0] * x_sample + w_target[1] * x_sample ** 2 + w_target[2] * x_sample ** 3  
# plt.plot(x_sample,y_sample)  
# ----------------------------------------------------------  
# 构建数据 x 和 y# x 是一个如下矩阵 [x, x^2, x^3]# y 是函数的结果 [y]x_train = np.stack([x_sample ** i for i in range(1, 4)], axis=1)  # 构建从相应的数据取得的自变量范围矩阵  
x_train = torch.from_numpy(x_train).float() # 转换成 float tensory_train = torch.from_numpy(y_sample).float().unsqueeze(1) # 转化成 float tensor  
# 定义参数 w和b  
w = torch.randn((3, 1), dtype=torch.float, requires_grad=True)  # 每一次run得到的初始fit曲线不同  
b = torch.zeros((1), dtype=torch.float, requires_grad=True)  
# 定义模型  
def multi_linear(x):  
    return torch.mm(x, w) + b   # 注: mm可以使用matmul来进行代替  
  
def loss_func(y_, y):  # 使用平方函数作为损失函数  
    return torch.mean((y_ - y) ** 2)  
y_pred = multi_linear(x_train) # 构建多项式函数  
  
ax1 = fig.add_subplot(121)  
ax1.plot(x_train.data.numpy()[:, 0], y_pred.data.numpy(), label='curve_prefit', color='r')  
ax1.plot(x_train.data.numpy()[:, 0], y_sample, label='real curve', color='b')  
  
# 画出更新之前的模型  
# ============ 训练数据集 ====================for epoch in range (epochNum):  
    loss = loss_func(y_pred, y_train)  
    try:  
        w.grad.zero_()  
        b.grad.zero_()  
    except:  
        pass  
    loss.backward()  
    w.data = w.data - lr * w.grad.data  
    b.data = b.data - lr * b.grad.data  
    y_pred = multi_linear(x_train)  
  
ax2 = fig.add_subplot(122)  
ax2.plot(x_train.data.numpy()[:, 0], y_pred.data.numpy(), label='fitting curve-', color='r')  
ax2.plot(x_train.data.numpy()[:, 0], y_sample, label='real curve', color='b')  
  
plt.legend()  
plt.show()
```

### (2) Logistic 回归模型
对于经典的$(0,1)$模型，使用交叉熵损失函数
$$loss = -[y * log(\hat{y}) + (1-y) *log(1-y)]$$
```python
def logistic_regression(x):
    return torch.sigmoid(torch.mm(x, w) + b)
## mm: matmul

y_pred = logistic_regression(x_data)
# 计算loss, 使用clamp的目的是防止数据过小而对结果产生较大影响。
def binary_loss(y_pred, y):
    logits = ( y * y_pred.clamp(1e-12).log() + \
              (1 - y) * (1 - y_pred).clamp(1e-12).log() ).mean()
    return -logits
```
只需要将loss改为这个即可。

其中，pytorch提供了相关的函数，重要的是 torch.optim.SGD 可以直接采用小批量梯度回归, 另外每一次需要调用 optimizer.zero_grad()清零优化器梯度即可。
```python title:最简单的小批量梯度下降回归代码(外部定义模型y_pred和损失binary_loss)
# 使用 torch.optim 更新参数
from torch import nn
import time
# use the neural network model for training
# 定义优化参数
w = nn.Parameter(torch.randn(2, 1))
b = nn.Parameter(torch.zeros(1))

# 优化器的设定以及相关的使用
optimizer = torch.optim.SGD([w, b], lr=0.1) # 使用SGD算法

for e in range(1000):
    # 前向传播
    y_pred = logistic_regression(x_data)  # 这个y_pred和loss是自己定义函数计算的
    loss = binary_loss(y_pred, y_data)      # 计算 loss
    # 反向传播
    optimizer.zero_grad() #-----使用优化器将梯度归 0
    loss.backward()
    optimizer.step() # *** 在这一步中，更新参数被封装，使用优化器来更新参数 **** 
    # 计算正确率
    mask = y_pred.ge(0.5).float()
    acc = (mask == y_data).sum().item() / y_data.shape[0]
```


## 三、PyTorch提供的损失函数 
前面使用了自己写的 loss函数，其实 PyTorch 已经提供了一些常见的 loss函数，比如线性回归里面的 loss 是 `nn.MSE()`，而 Logistic 回归的二分类 loss 在 PyTorch 中是 `nn.BCEWithLogitsLoss()`，关于更多的 loss，可以查看[文档](https://pytorch.org/docs/stable/nn.html#loss-functions)

PyTorch 实现的 loss函数有两个好处：第一是方便使用，不需要重复造轮子；第二就是其实现是在底层 C++ 语言上的，所以速度上和稳定性上都要比自己实现的要好。

另外，PyTorch 出于稳定性考虑，将模型的 Sigmoid 操作和最后的 loss 都合在了 `nn.BCEWithLogitsLoss()`，所以我们使用 PyTorch 自带的 loss 就不需要再加上 Sigmoid 操作了
```python
# 使用自带的loss
criterion = nn.BCEWithLogitsLoss() # 将 sigmoid 和 loss 写在一层，有更快的速度、更好的稳定性
w = nn.Parameter(torch.randn(2, 1))
b = nn.Parameter(torch.zeros(1))

def logistic_reg(x):
    return torch.mm(x, w) + b
    
optimizer = torch.optim.SGD([w, b], 1.)

y_pred = logistic_reg(x_data)
loss = criterion(y_pred, y_data)  # 这样定义损失函数
# 之后调用
for ......
	loss.backward()
	optimizer.step()_
print(loss.data)
```

常见的损失函数：
浅层： LR，SCM, ELm
深层：CNN ..

## 四、神经网络池化层的概念简介
池化层也是一个类似二维的层;

池化层在CNN中,用于减少空间维数进行降维使用, 实际上是使用一个feature map, 其宽高分别为 nh 和 nw, 类似于卷积过程, 在 map 的指定范围内进行扫描; 然后过滤出符合要求的部分。
一般池化需要指定 feature map 大小和 stride, 示例如下: 
![[attachments/1_fhdK1xJND_m1Rdmr1m_9PQ.webp]]
例如16x16的输入在2x2窗, 步进为(2x2)池化后成为 8x8 的特征; 常见的池化有均值池化和最大值池化等等; 


