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

```


对于前面的线性回归模型、 Logistic回归模型和神经网络，在构建的时候定义了需要的参数。这对于比较小的模型是可行的，但是对于大的模型，比如100 层的神经网络，这个时候再去手动定义参数就显得非常麻烦，所以 PyTorch 提供了两个模块来帮助我们构建模型，一个是`Sequential`，一个是 `Module`。

Sequential 允许我们构建序列化的模块，而 Module 是一种更加灵活的模型定义方式，下面分别用 `Sequential` 和 `Module` 来定义上面的神经网络。
