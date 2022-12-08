在前面学习线性回归和逻辑回归模型的基础上，本节学习如何利用PyTorch实现多层神经网络。
1. 神经网络(Neural Network)

线性回归的公式是 $y = w x + b$， Logistic 回归的公式是 $y = Sigmoid(w x + b)$，其实它们都可以看成单层神经网络，其中 Sigmoid 被称为激活函数。

```python
# 模型的可视化工具：Netron
# 在cmd中输入netron   
# 然后访问localhost:8080即可使用netron 来可视化访问本地的模型
```

example code: 
```python
import torch  
import numpy as np  
from torch import nn  
from sklearn import datasets  
import matplotlib.pyplot as plt  

## -------generate sample data and plot the figure that didn't classified ------  
np.random.seed(0)  
data_x, data_y = datasets.make_moons(200, noise=0.20)  
# plot data  
plt.scatter(data_x[:, 0], data_x[:, 1], c=data_y, cmap=plt.cm.Spectral)  
plt.show()  
  
# ---------- use torch to train the module ---------  
x = torch.from_numpy(data_x).float()  
y = torch.from_numpy(data_y).float().unsqueeze(1)  
# since the y vector (the output) should be a vector for training --> which should be unsqueed as a one-column tensor  
  
# initialize the parameter randomly at the begin of the training  
  
# the first layer (input 2 and output 4)  
w1 = nn.Parameter(torch.randn(2, 4) * 0.1)  # 4 means the columns, which is the number of the hidden neuron  
b1 = nn.Parameter(torch.zeros(4))  
# the second layer (input 4 and output 1)  
w2 = nn.Parameter(torch.randn(4, 1) * 0.1)  
b2 = nn.Parameter(torch.zeros(1))  
  
# define the SimpNetWork module  
def SimpNetwork(x):  
    x1 = torch.mm(x, w1) + b1  
    x1 = torch.sigmoid(x1)          # 使用 PyTorch 自带的 sigmoid 激活函数  
    x2 = torch.mm(x1, w2) + b2  
    return x2                       # BCEWithLogitsLoss 已经带了sigmoid，所以此处不需要  
  
optimizer = torch.optim.SGD([w1, b1, w2, b2], 0.1)  
criterion = nn.BCEWithLogitsLoss()  
  
# obtain the data of the shape of x and y  
# for i in [x, y]:  
#    print(i.shape, type(i), type(i.numpy()), i.numpy().shape)  
  
# 训练 1000 次  
for e in range(1000):  
    out = SimpNetwork(x)  # 1. caculate output  
    loss = criterion(out, y)  # 2. caculate loss  
    optimizer.zero_grad()   # ***** note that use this to zero the grad of the optimizer  
    loss.backward()         # use the gradient algorithm  
    optimizer.step()  
    if (e + 1) % 100 == 0:  
        print('epoch: {}, loss: {}'.format(e+1, loss.item()))  
  
y_res  = torch.sigmoid(SimpNetwork(x))    # caculate the sigmoid of the function  
# y_pred = np.argmax(y_res, axis=1)  
y_pred = (y_res > 0.5)*1  
  
# plot data  
plt.scatter(x[:, 0], x[:, 1], c=y, cmap=plt.cm.Spectral)  
plt.title("ground truth")  
plt.show()  
  
plt.scatter(x[:, 0], x[:, 1], c=y_pred, cmap=plt.cm.Spectral)  
plt.title("predicted")  
plt.show()
```

对于前面的线性回归模型、 Logistic回归模型和神经网络，在构建的时候定义了需要的参数。这对于比较小的模型是可行的，但是对于大的模型，比如100 层的神经网络，这个时候再去手动定义参数就显得非常麻烦，所以 PyTorch 提供了两个模块来帮助我们构建模型，一个是`Sequential`，一个是 `Module`。

Sequential 允许我们构建序列化的模块，而 Module 是一种更加灵活的模型定义方式，下面分别用 `Sequential` 和 `Module` 来定义上面的神经网络。
