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

```python
import matplotlib.pyplot as plt  
from torch import nn  
import numpy as np  
import torch  
from sklearn import datasets  
  
# The definition of the Sequential module  
seq_net = nn.Sequential(  
    nn.Linear(2, 4), # PyTorch 中的线性层，wx + b  
    nn.Tanh(),  # 激活函数层  
    nn.Linear(4, 1)  # 输出层  
)  
# 使用Sequential编写时，其所获得的结果更加准确  
# 注意: Sequential 是使用c++ 作为底层代码的，比python执行效率更高  
  
print(seq_net[0])  # 序列模块可以通过索引访问每一层  
# 打印出第一层的权重  
w0 = seq_net[0].weight  
print("weight of the first layer",w0)  
  
# -----define the boundary of the decision boundaey ---------  
def plot_decision_boundary(model, x, y):  
    # Set min and max values and give it some padding  
    x_min, x_max = x[:, 0].min() - 1, x[:, 0].max() + 1  # the padding is used to set the range of drawing  
    y_min, y_max = x[:, 1].min() - 1, x[:, 1].max() + 1  
    h = 0.01  # h is the step  
    # Generate a grid of points with distance h between them    xx, yy = np.meshgrid(np.arange(x_min, x_max, h), np.arange(y_min, y_max, h))  
    # Predict the function value for the whole grid .c_  
    Z = model(np.c_[xx.ravel(), yy.ravel()])   # ravel is similiar to "flatten"  
    # but if we ravel an array it will returns the reveal of the identity(that means, if we modify the value of the array,    # it will have an impact on the tensor itself)    # however,  "flatten" returns a copy of the array    Z = Z.reshape(xx.shape)  
    # Plot the contour and training examples  
    plt.contourf(xx, yy, Z, cmap=plt.cm.Spectral)  
    plt.ylabel('x2')  
    plt.xlabel('x1')  
    plt.scatter(x[:, 0], x[:, 1], c=y.reshape(-1), s=40, cmap=plt.cm.Spectral)  
  
# generate sample data  
np.random.seed(0)  
data_x, data_y = datasets.make_moons(200, noise=0.20)  
x = torch.from_numpy(data_x).float()  
y = torch.from_numpy(data_y).float().unsqueeze(1)  
  
# 设置损失函数  
criterion = nn.BCEWithLogitsLoss()  
# 通过 parameters 可以取得模型的参数  
param = seq_net.parameters()  
# 定义优化器  
optim = torch.optim.SGD(param, 0.1)  
  
for e in range(10000):  
    out = seq_net(x)  
    loss = criterion(out, y)  
    optim.zero_grad()  
    loss.backward()  
    optim.step()  
    if (e + 1) % 1000 == 0:  
        print('epoch: {}, loss: {}'.format(e+1, loss.item()))  
  
def plot_seq(x):  
    out = torch.sigmoid(seq_net(torch.from_numpy(x).float())).data.numpy()  
    out = (out > 0.5) * 1  
    return out  
  
plot_decision_boundary(lambda x: plot_seq(x), x.numpy(), y.numpy())  
plt.title('sequential')  
plt.show()
```



### 2.3 Module

下面再用 Module 定义这个模型，下面是使用 Module 的模板

```python
class 网络名字(nn.Module):
    def __init__(self, 一些定义的参数):
        super(网络名字, self).__init__()
        self.layer1 = nn.Linear(num_input, num_hidden)
        self.layer2 = nn.Sequential(...)
        ...
        
        定义需要用的网络层
        
    def forward(self, x): # 定义前向传播
        x1 = self.layer1(x)
        x2 = self.layer2(x)
        x = x1 + x2
        ...
        return x
```

注意的是，Module 里面也可以使用 Sequential，同时 Module 非常灵活，具体体现在 forward 中，如何复杂的操作都能直观的在 forward 里面执行


```python
class SimpNet(nn.Module):
    def __init__(self, num_input, num_hidden, num_output):
        super(SimpNet, self).__init__()
        self.layer1 = nn.Linear(num_input, num_hidden)
        
        self.layer2 = nn.Tanh()
        
        self.layer3 = nn.Linear(num_hidden, num_output)
        
    def forward(self, x):
        x = self.layer1(x)
        x = self.layer2(x)
        x = self.layer3(x)
        return x

mo_net = SimpNet(2, 4, 1)
# 访问模型中的某层可以直接通过名字

# 第一层
l1 = mo_net.layer1
print(l1)

# 打印出第一层的权重
print(l1.weight)
# 定义优化器
optim = torch.optim.SGD(mo_net.parameters(), 1.)

# 我们训练 10000 次
for e in range(10000):
    out = mo_net(x)
    loss = criterion(out, y)
    optim.zero_grad()
    loss.backward()
    optim.step()
    if (e + 1) % 1000 == 0:
        print('epoch: {}, loss: {}'.format(e+1, loss.item()))


```

