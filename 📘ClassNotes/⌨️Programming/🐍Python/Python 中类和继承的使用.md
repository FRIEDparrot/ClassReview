下面讲解了继承 torch.nn.Module 类的方法, 首先需要在类名后面括号加上继承的类, 并且使用 super() 调用父类的 \_\_init\_\_()函数 
```python
import torch  
import numpy as np  
import time  
import torch.nn as nn  
from deeplearning_util_functions import plot_figure  
from matplotlib import pyplot as plt  
  
class Regression_Module(nn.Module):   # if class is in bracket, it will inherit this class  
    def __init__(self, learning_rate:float=0.001) :  
        # for the  method inherit from other method: use:  
        super().__init__()  # inherit the  init method of the parent  
  
        self.learning_rate = learning_rate  
        self.wrapper('learning_rate', 0.2)  
        print(self.learning_rate)  
        print(self.cpu())  
        print(torch.cpu.current_device())  
    def wrapper(self, attr:str, value):  
        setattr(self, attr, value)  
        assert hasattr(self, 'learning_rate') # refer to https://blog.csdn.net/qq_42269354/article/details/89476880  
    def __call__(self, *args, **kwargs):  
        print("executing the method")  
  
a = Regression_Module(0.01)   # create a new class  
a()  # call the __call__ method
```

[assert 用法](https://blog.csdn.net/qq_42269354/article/details/89476880) 即后面布尔必须为真,如果为假则触发异常。

注意: python 允许将一个类像一个实例一样调用, 实际上是调用了其中的 `__call__` 方法, 
```python
assert hasattr(u.name, '__call__')    # 判断是否可以调用
u()   # 调用其中的 __call__ 方法
```






