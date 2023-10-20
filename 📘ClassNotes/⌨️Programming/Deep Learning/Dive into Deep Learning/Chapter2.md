
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


for reading the Dataset, we just need the os package 
```python
import os 
os.makedirs(os.path.join('..', 'data'), exist_ok = True);
datafile = .....csv 
with open(data_file, 'r') as f:
	......
```

also load data using pandas is available

### 2 data preparation

```python
inputs, targets = data.iloc[:, 0:2], data.iloc[:, 2]

inputs = pd.get_dummies(inputs, dummy_na=True)print(inputs)

inputs = inputs.fillna(inputs.mean())

print(inputs)
```

Data visualization tools such as seaborn, Bokeh, or matplotlib can help you to manually inspect the data and develop intuitions about what problems you may need to address.


### 3 Reduction 
Invoking the sum function reduces a tensor along all of the axes, and eventually produce a scalar.
To sum along the rows, we need to specify the axis as `axis = 0` in sum;

if we specify that the `axis=1` , 

### 4. Non-Reduction Sum 

for keeping the number of axes unchanged, when invoking the function for calculating the sum 
`sum_A = A.sum(axis =1,keepdims = True)` -> this will keep the two axes after summing each row. 

`A / sum(A)`

### 5. cumsum function 
The cumsum function is for calculate the <mark style="background: transparent; color: yellow">cumulative sum of elements</mark> 

```python
A = torch.arange(3, dtype = torch.float32)
A.cumsum()

>> tensor([1.,3.,6.])
```


### 6. Dot product and vector product 

```python 
# for dot product 
torch.dot(x,y);

# for vector product 
torch.mv(A,x); 

# matrix-matrix multiplication 
torch.mm(A,B)
```

```python
torch.norm(u)
```

- scalars, vectors, matrices and the tensors are the basic mathmatical objects used in the linear algebra 
- Comon vector norms include the $l_1$ norm and $l_2$ normm and the common matrix norm include the spectral and Frobenius norm. 

