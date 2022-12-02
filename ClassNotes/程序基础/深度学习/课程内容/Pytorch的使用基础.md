OpenVINO加速pytorch的工作原理
![[0_s3DTtiZ5T03zpTMH.webp|400]]
使用openvino和对应的

### 增加维度或者减少维度
```python
print(x.shape)
x = x.unsqueeze(0) # 在第一维增加
print(x.shape)
print(x)
```

另外，pytorch中大多数的操作都支持 `inplace` 操作，也就是可以直接对 tensor 进行操作而不需要另外开辟内存空间，方式非常简单，一般都是在操作的符号后面加`_`
