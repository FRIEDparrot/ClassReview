默认情况下， 展平范围是 dim=1 到 dim=-1
可以通过 `start_dim, end_dim` 控制范围，注意从 0 开始 : 

给出一个简单的带参数的示例如下: 
```python fold title:
import torch import torch.nn as nn

# 创建一个 4D 张量（模拟图像批次数据）

input_tensor = torch.randn(2, 3, 4, 5) # 形状: [batch_size, channels, height, width] print("输入张量形状:", input_tensor.shape)

# 1. 默认行为（展平第1维到最后）

flatten_default = nn.Flatten() 
output_default = flatten_default(input_tensor) 
print("默认展平后的形状:", output_default.shape) # [2, 3_4_5] = [2, 60]

# 2. 自定义参数（仅展平第2维和第3维）

flatten_custom = nn.Flatten(start_dim=2, end_dim=3) output_custom = flatten_custom(input_tensor) print("自定义展平后的形状:", output_custom.shape) # [2, 3, 4*5] = [2, 3, 20]
```

