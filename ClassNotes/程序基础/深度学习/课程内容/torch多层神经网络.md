在前面学习线性回归和逻辑回归模型的基础上，本节学习如何利用PyTorch实现多层神经网络。
1. 神经网络(Neural Network)

线性回归的公式是 $y = w x + b$， Logistic 回归的公式是 $y = Sigmoid(w x + b)$，其实它们都可以看成单层神经网络，其中 Sigmoid 被称为激活函数。

```python
# 模型的可视化工具：Netron
# 在cmd中输入netron   
# 然后访问localhost:8080即可使用netron 来可视化访问本地的模型
```
