对于 Adam 部分，参考 https://nvidia.github.io/apex/optimizers.html 
需要说明，  **Apex 主要针对 Linux 系统优化，尤其是在使用 NVIDIA GPU 进行深度学习时的性能**。 官方不支持 Windows 

需要时, 可以在 Linux 装 Pytorch 加速 Adams 并行计算。  

```python fold title:
import torch  
import torch.optim as optim

# Example model and data

model = YourModel()  
data = YourData()

# Using Adam optimizer

optimizer = optim.Adam(model.parameters(), lr=0.001)

# For fused Adam in a CUDA environment (if supported) 
# This may require a specific version of PyTorch and installation of apex for mixed-precision. 
from apex.optimizers import FusedAdam

fused_optimizer = FusedAdam(model.parameters(), lr=0.001)

for input, target in data:  
optimizer.zero_grad()  
output = model(input)  
loss = loss_function(output, target)  
loss.backward()  
optimizer.step()
```
