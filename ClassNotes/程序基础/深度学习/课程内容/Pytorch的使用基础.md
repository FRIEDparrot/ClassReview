OpenVINO加速pytorch的工作原理
![[0_s3DTtiZ5T03zpTMH.webp|400]]
```
pip install torch-ort
python -m torch_ort.configure
```

使用openvino和对应的onnx runtime (目前由于onnx runtime未推出过Windows对应的onnix runtime, 鸽。。。。)

### 增加维度或者减少维度
```python
print(x.shape)
x = x.unsqueeze(0) # 在第一维增加
print(x.shape)
print(x)
```

另外，pytorch中大多数的操作都支持 `inplace` 操作，也就是可以直接对 tensor 进行操作而不需要另外开辟内存空间，方式非常简单，一般都是在操作的符号后面加`_`


torch进行反向传播算法求导

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

n.backward(torch.ones_like(n)) # 将 (w0, w1) 取成 (1, 1)
print(m.grad)
# tensor([[ 4., 27.]], grad_fn=<CopySlices>)
```
将上面的式子写成数学公式，可以得到 
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

#### 多次自动求导
通过调用 backward 我们可以进行一次自动求导，如果我们再调用一次 backward，会发现程序报错，没有办法再做一次。这是因为 PyTorch 默认做完一次自动求导之后，计算图就被丢弃了，所以两次自动求导需要手动设置
```python
x = torch.tensor([3], dtype=torch.float, requires_grad=True)
y = x * 2 + x ** 2 + 3
print(y)
y.backward(retain_graph=True) # 设置 retain_graph 为 True 来保留计算图
print(x.grad)
```


### 三、多项式回归模型

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


#### Logistic 回归模型
```python
def logistic_regression(x):
    return torch.sigmoid(torch.mm(x, w) + b)

y_pred = logistic_regression(x_data)
# 计算loss, 使用clamp的目的是防止数据过小而对结果产生较大影响。
def binary_loss(y_pred, y):
    logits = ( y * y_pred.clamp(1e-12).log() + \
              (1 - y) * (1 - y_pred).clamp(1e-12).log() ).mean()
    return -logits
```
只需要将loss改为这个即可

