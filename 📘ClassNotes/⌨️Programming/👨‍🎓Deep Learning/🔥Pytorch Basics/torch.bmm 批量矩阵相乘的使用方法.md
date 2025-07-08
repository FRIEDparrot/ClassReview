When using `torch.bmm`,  it calculates the  multiplication of different matrix concurrently (more efficient).  

### (1) 对于三维向量 
参考 [documentation](https://pytorch.org/docs/stable/generated/torch.bmm.html) 部分 : 
一般地， 需要将 batch_size  放在第一维即可 : 
If `input` is a $(b×n×m)$ tensor, `mat2` is a $(b×m×p)$ tensor, `out` will be a (b×n×p) tensor. 

### (2) Multi-Head 多头注意力机制 
首先一般先进行一次  reshape 将后面两维拆为多头 : 
```python fold title:
q = self.w_q(queries).reshape(B, T, h, D//h)  # [B, T, h, D//h]
k = self.w_k(keys).reshape(B, T, h, D//h)     # [B, T, h, D//h]
v = self.w_v(values).reshape(B, T, h, D//h)   # [B, T, h, D//h]
```

从而得到， 在 multi-head 的注意力机制中, 其输入一般为 :  
- **Q (Query)**: `(batch_size, seq_len, num_heads, head_dim)`
- **K (Key)**: `(batch_size, seq_len, num_heads, head_dim)`
- **V (Value)**: `(batch_size, seq_len, num_heads, head_dim)` 

此时， **需要调整维度顺序：拆分维度之后， 将 num_heads 移到第1维（批量维度）**
```python fold title:transpose 
Q = Q.transpose(1, 2)  # (batch, num_heads, seq_len, head_dim)
K = K.transpose(1, 2)  # (batch, num_heads, seq_len, head_dim)
V = V.transpose(1, 2)  # (batch, num_heads, seq_len, head_dim) 
```

由于 torch.bmm 仅支持 $n \times  b \times  a$ 矩阵， 需要<b><mark style="background: transparent; color: orange">将batch 维度和 num_heads 维度进行合并</mark></b>， 得到如下的矩阵部分 : 
```python fold title:
# 合并 batch 和 num_heads 维度
batch_size, num_heads, seq_len, head_dim = Q.shape
Q = Q.reshape(batch_size * num_heads, seq_len, head_dim)  # (batch*num_heads, seq_len, head_dim)
K = K.reshape(batch_size * num_heads, seq_len, head_dim)  # (batch*num_heads, seq_len, head_dim)
V = V.reshape(batch_size * num_heads, seq_len, head_dim)  # (batch*num_heads, seq_len, head_dim)
```

此时即可通过 `bmm` 计算 $Q, K$ 的点积和相应的 $V$ 乘积。 
