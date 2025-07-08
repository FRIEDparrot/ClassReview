对于将其中的两句 :  
```python fold title:
x = self.embed(x)  # (batch_size, seq_len, hidden_dim)  
x = x * (x.shape[-1] ** 0.5)  # scale embedding
```
合为一句时， 训练结果出现巨大差异 : 
```python fold title:

```


表面上

```python
x = self.embed(x)
x = x * (x.shape[-1] ** 0.5)
```

和

```python
x = self.embed(x) * (x.shape[-1] ** 0.5)
```

**似乎是一样的**，对吧？  
但是实际上，它们在 **PyTorch的内部执行细节上**，有很微妙的不同，尤其是跟**`self.embed`这层的参数梯度计算**有关！

---

## 关键区别

- **第一种写法：**
    
    ```python
    x = self.embed(x)
    x = x * (x.shape[-1] ** 0.5)
    ```
    
    这里：
    
    - `self.embed(x)` 先被计算成一个**中间变量**。
        
    - 然后你用中间变量 `x` 去乘 `(x.shape[-1] ** 0.5)`。
        
    - PyTorch 在反向传播时，会记录：
        
        > `x_after_embed = Linear(x)` `x_scaled = x_after_embed * const`
        
        换句话说，乘法和 Linear 是**两步分开**记录在 computational graph 里的。
        
- **第二种写法：**
    
    ```python
    x = self.embed(x) * (x.shape[-1] ** 0.5)
    ```
    
    这里：
    
    - `self.embed(x)` 直接包在乘法里，**一口气组合成一个新的操作**。
        
    - 在计算图中，PyTorch会优化，把乘常数的操作**融合进了 Linear 层**。
        
    - **这种融合**可能会让前向计算稍微快一点，而且在极小数值差异下，**数值稳定性（特别是初始化权重时）也会微妙不同。**
        

---

## 为什么影响了拟合效果？

- **如果分开写**，乘 `(x.shape[-1] ** 0.5)` 这一步发生在输出之后，**不会影响 embed 层的初始化参数**。
    
- **如果合在一起写**，PyTorch 有可能在初始化内部就考虑了这个scale，或者在反向传播中scale的传播路径也不同。
    

尤其是**小学习率 + 小初始权重**时，**这点数值差异**就能导致模型一开始训练更顺畅或者更卡住！

所以最终表现出的就是：

> 看似代码一样，实际训练过程稍有不同，拟合效果就拉开了差距。

---

## 总结

|写法|计算图|影响|
|:--|:--|:--|
|分开写（两步）|`Linear` → `Mul`|乘法不融合，数值传播直接|
|合在一起（一步）|`Mul(Linear(...))`|可能融合，初始时更平滑、传播更稳定|

---

## 额外

实际上，**标准Transformer论文（Attention is All You Need）**里的embedding，也是**在Linear或Embedding之后，立即scale**，就像你改的**"一行写法"**。所以你最后的那种写法才是**更标准、更合理的**！

---

要不要我顺便也画一张**两种写法的计算图小示意图**，这样你能更直观看到PyTorch里面到底差了什么？要的话告诉我 🔥。