
```python
pip install faiss-cpu 
```

```python
import faiss
import numpy as np

# 1. 生成一些随机向量 (1000 个 128 维向量)
d = 128  # 向量维度
nb = 1000  # 数据库向量数量
np.random.seed(1234)  # 保持结果一致
data = np.random.random((nb, d)).astype('float32')

# 2. 创建索引 (IndexFlatL2 适用于 L2 距离)
index = faiss.IndexFlatL2(d)  # L2 距离索引
print("是否是训练好的索引:", index.is_trained)  # True

# 3. 添加向量到索引
index.add(data)  # 添加数据库向量
print("数据库向量数量:", index.ntotal)

# 4. 查询最近邻 (5 个查询向量)
nq = 5  # 查询向量数量
queries = np.random.random((nq, d)).astype('float32')

# 搜索最近的 4 个向量
k = 4
distances, indices = index.search(queries, k)  # 返回距离和索引

# 5. 输出结果
print("查询结果的索引:\n", indices)
print("查询结果的距离:\n", distances)
```

此外, 如果采用 gpu, 可以采用 index_cpu_to_gpu 将索引转移到 gpu 实现加速.
```python
res = faiss.StandardGpuResources()  # 初始化 GPU 资源
index_gpu = faiss.index_cpu_to_gpu(res, 0, index)  # 将索引从 CPU 转移到 GPU
```
