### print格式控制
```python
x = 1
y = 2
print(f'x = {x:.3f}, z = {x+y:.3f}')
```
其中第二个显示 z = 3.00, 即通过string 中的 f 实现了格式控制

## 函数的主要使用方法
### (1)指定函数输入类型
在函数编写时, 可以指定输入的格式类型, 具体方法如下:

```python 
def set_axis(axes:matplotlib.axes.Axes, xlabel:str, ylabel:str, xlim, ylim, xscale, yscale, legend):  
    axes.set_xlabel(xlabel);    axes.set_ylabel(ylabel)  
    axes.set_title(title);
```

检查某个类对象是否具有某种属性: hasattr
```python
hasattr(X, "ndim")
```

检查某个对象是否是某个类:isinstance 
```python 
isinstance(X, list)
```

### (2) 指定函数返回类型
方法是在 def 后加上 `-> 类型` 指定返回类型, 如果没有返回类型, 则认为是 None.
```python
class A:
	def __init__(self) -> None:
		A.name = "wreg"
		pass
	def sum(a:int, b:int) -> int:
		return a + b
```

如果是返回多个指定类型的参数, 则可以使用:
```python
def generate_sample(self, W, b) -> tuple [int, int]:
    # 你的代码
    return X, y
```

### (3) 函数中自定义迭代器的方法
基本的方法是使用 yield 进行迭代返回, 
```python
def get_dataloader():
	indics = list(range(0, 100))
	batch_indices =   torch.tensor( indices[i : i + self.batch_size] )  # if batch size = 10,  return the first 10 element at first. 
	yield self.x[batch_indices], self.y[batch_indices]

# 使用方法如下:
X,y = next(iter(data.get_dataloader()))
```

## pandas 常用操作
### 1. 取行或者取列
取某几行或者某几列:
```python
features = data.iloc[0:2]                    # 取某几行
features = data[data.columns[0:2]]  # 取某几列
features.shape[1]         # 获取某些方向上的尺寸
data_tensor = torch.tensor(data.values)  # 转换pd.dataframe 到 torch.tensor
```


### 2. 转换 pandas 数据到torch张量
在 torch 中最为常用的是 torch.utils.data 包,
```python
from torch.utils.data import dataloader
```
在 pytorch 中, unsqueeze 用于增加张量的维数, 例如将一个一维张量转换为二维, 则使用:
```
targets  = data_tensor[:,3].unsqueeze(1)   # add a dimension to it 
```

最为简单的小批量依次输出的方法如下, 需要注意 `data_tensor = torch.tensor(data.values, dtype=torch.float32) ` 将 DataFrame 转换为 float32 数据类型, 如果默认, 则是double 数据类型，会产生问题

```python 
import deeplearning_util_functions  
import pandas as pd  
import torch  
from torch.utils.data import DataLoader, TensorDataset  
  
# Boston housing problem  
data = pd.read_csv("../datasheets/housing.csv")  
data_tensor = torch.tensor(data.values, dtype=torch.float32)  # note from 
  
features = data_tensor[:, 0:3]  #  
targets  = data_tensor[:,3].unsqueeze(1)   # add a dimension to it  to keep 2 dims
n = features.shape[0] 

# create the linear regression minibatch stochastic gradient descent  
w = torch.ones(n)  
b = torch.zeros(n)  
  
X = features  
Y = targets  
  
DataLoader1 = DataLoader(torch.cat((X,Y), 1),batch_size=20,shuffle=True)  
  
for minibatch in DataLoader1:  
    print(minibatch)
```


### 3. 迭代器对象和next() 函数用法
`next()` 用于迭代器中获取下一个元素, 如果达到末尾则抛出 StopIteration 异常 
```python 
# 获取下一个元素
numbers = [1, 2, 3, 4, 5]
numbers_iter = iter(numbers)
print(next(numbers_iter))  # 输出：1
print(next(numbers_iter))  # 输出：2
```

如果是迭代器对象, 则必须实现 `__iter__()` 和 `__next__()` 两个方法
- `__iter__()`方法：返回迭代器对象本身。
- `__next__()`方法：返回迭代器对象的下一个数据元素，如果没有元素可迭代，则抛出`StopIteration`异常。例如定义 MyIterator 迭代器对象 
```python
class MyIterator:
    def __init__(self, data):
        self.data = data
        self.index = 0

    def __iter__(self):
        return self

    def __next__(self):
        if self.index >= len(self.data):
            raise StopIteration
        result = self.data[self.index]
        self.index += 1
        return result
numbers = [1, 2, 3, 4, 5] 
my_iter = MyIterator(numbers) 

for num in my_iter:  
	print(num)   # 实际上是从next 迭代器方法中获取到的num对象
```

### yield 关键字
作用是保存当前程序执行状态, 虽然可以理解为 return 操作, 但是yield执行时还保存了当前的执行内容, 再一次调用这个函数时，他会找到你在此函数中的yield关键字，然后从yield的下一句开始执行。
```python
def generator():
    for i in range(10):
        yield i * i   # 需要单个逐一返回 
gen = generator()
print(gen)
```


## Collections 容器类
collection 提供了包括双端队列(deque)等多种数据类型
```python
from colloection import namedtuple,deque
```
参考 [python 官方文档](https://docs.python.org/3/library/collections.html)



```
def forward(self, input: Tensor, target: Tensor) -> Tensor: return F.mse_loss(input, target, reduction=self.reduction)
	
```