
The basic survival skills for the deep learning: 

1.  Storing and Manipulating data 
2. libraries for ingesting and preprocessing data from a variety of sources. 
3. knowledge of the basic linear algebraic operations which would be applied to the high-dimensional data elements. 

The name of Pytorch Library is torch, and the datatype in the pytorch is tensor, which is n-dimension array. 
The definition of torch is: 
```python
import torch
x = torch.arange(12, dtype = torch.float32)
```

note that the starting index is included in the tensor array, but the end index is included in the  array, but at the same time the end index is not included.  

for the example above, the  result is : 
`[0 - 11]`

the element number of $x$ can be reached by: 
`x.numel()`
from the property `shape`, we can reach the size of the tensor.

basic operation
`torch.zeros((2,3,4))` 
defines a zero tensor for torch. 
also, `ones, zeros` and `torch.arange(3).reshape((3,1))`

the saving memory of the  parameter is using the identity of the parameter, which can be reached by `id(x)` command 

also, a important command is to <mark style="background: transparent; color: yellow">convert the array in numpy to the tensor in torch</mark> or <mark style="background: transparent; color: yellow">convert tensor to numpy array</mark> using the following commands: 

```python
X = torch.tensor([1,2])
A = X.numpy() # convert torch tensor to the numpy array

B = torch.from_numpy(A) # convert numpy array to torch tensor
```

