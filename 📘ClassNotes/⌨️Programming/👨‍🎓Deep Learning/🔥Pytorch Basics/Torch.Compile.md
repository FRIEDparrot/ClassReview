### (1) Torch.Compile 
Torch.Compile 可以使用 TorchDynao 后端对相应模型进行优化，用以将 Pytorch 模型编译为更加高效的版本, 以提升运行速度。 具体参考 https://docs.pytorch.org/docs/stable/generated/torch.compile.html#torch.compile 

实际使用往往采用
```python fold title:实际使用
torch.nn.Module.compile() 
```

```python fold title:
torch.compile(
    model, 
    *,
    mode="default",  # 编译模式
    dynamic=None,    # 是否启用动态形状(一般直接默认即可)
    fullgraph=False, # 是否要求完整图编译
    backend="inductor",  # 使用的编译器后端
    options=None     # 后端特定选项
)
```

最常用的参数是 **mode**: 编译优化级别 
- `"default"`: 平衡编译时间和运行性能
- `"reduce-overhead"`: 减少框架开销，适合小模型或者 small batches 
- `"max-autotune"`: 最大程度优化，但编译时间长  
- `”max-autotune-no-cudagraphs”` is a mode similar to “max-autotune” but without CUDA graphs 

编译会在第一次输入时进行 : 
```python fold title:使用示例
import torch
import torchvision.models as models

# 加载预训练模型
resnet50 = models.resnet50(weights='IMAGENET1K_V2').cuda()

# 编译模型 - 使用最大优化
compiled_resnet = torch.compile(resnet50, mode="max-autotune")

# 准备输入
input = torch.randn(64, 3, 224, 224).cuda()

# 首次运行会较慢，因为需要编译
output = compiled_resnet(input)

# 后续运行会很快
for _ in range(10):
    output = compiled_resnet(input)
```

