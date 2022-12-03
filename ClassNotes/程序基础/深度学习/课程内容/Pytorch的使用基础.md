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


