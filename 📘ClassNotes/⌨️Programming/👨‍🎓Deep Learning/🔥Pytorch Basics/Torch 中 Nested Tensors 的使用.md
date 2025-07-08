对于  Nested tensors, 

```python fold title:
nt = torch.nested.nested_tensor([torch.arange(10), torch.arange(5), torch.arange(8)], dtype=torch.float32, layout=torch.jagged)
print(nt) 
# NestedTensor(size=(3, j2), offsets=tensor([ 0, 10, 15, 23]), contiguous=True) 
```

实际作用是代替 padded_sentence, 例如: 

sentences = [["goodbye", "padding"],
             ["embrace", "nested", "tensor"]]
vocabulary = {"goodbye": 1.0, "padding": 2.0,
              "embrace": 3.0, "nested": 4.0, "tensor": 5.0}
padded_sentences = [torch.tensor](https://docs.pytorch.org/docs/stable/generated/torch.tensor.html#torch.tensor "torch.tensor")([[1.0, 2.0, 0.0],
                                 [3.0, 4.0, 5.0]])
nested_sentences = [torch.nested.nested_tensor](https://docs.pytorch.org/docs/stable/nested.html#torch.nested.nested_tensor "torch.nested.nested_tensor")(torch.tensor([1.0, 2.0]),
			   torch.tensor([3.0, 4.0, 5.0])])
print(f"{padded_sentences=}")
print(f"{nested_sentences=}")

一般**实际也可以采用填充或者掩码进行代替** (更加常见)
