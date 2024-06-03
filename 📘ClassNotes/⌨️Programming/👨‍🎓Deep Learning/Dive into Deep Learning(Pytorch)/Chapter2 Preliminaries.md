### 1. Basics of Torch 
The name of Pytorch Library is torch, and the **datatype in the pytorch is tensor**, which is n-dimension array. 
```python
import torch
x = torch.arange(12, dtype = torch.float32)   # create a tensor by torch
x.reshape(3,4);    #  transform it to a 3-line 4-column data. 
x.numel();             # the element number of x
x.shape();             # the size of the tensor
torch.zeros((2,3,4))  # defines a zero tensor for torch.  (also use ones)
torch.arange(3).reshape((3,1))  #
torch.randn(3,4)  

torch.tensor([[2,1,4,3], [1,2,3,4], [4,3,2,1]])   # create tensor by initiate the element 
```

note that the <mark style="background: transparent; color: red">starting index is included in the tensor array, but at the same time the end index is not included. </mark> which means 0-11 will be created.

> the saving memory of the  parameter is using the identity of the parameter, which can be reached by `id(x)` command 

### 2. Convert Numpy to torch 
also, a important command is to <mark style="background: transparent; color: red">convert the array in numpy to the tensor in torch</mark> or <mark style="background: transparent; color: red">convert tensor to numpy array</mark> using the following commands: 
```python
X = torch.tensor([1,2])
A = X.numpy() # convert torch tensor to the numpy array 
B = torch.from_numpy(A) # convert numpy array to torch tensor
```

for reading the Dataset, we just need the os package,also, load data using pandas is available. 
```python
import os 
os.makedirs(os.path.join('..', 'data'), exist_ok = True);
datafile = .....csv 
with open(data_file, 'r') as f:
	# ......
```

for concatenate the matrix or tensor, we can use:
```python
torch.cat(x1, x2, dim = 1) # for dim = 1, it cat the row
# for broadcasting the matrix, we use:
x.repeat(3,1)
```

also, if convert to scalar is needed, use:
```python
print(r[0].item())
```

### 3. data preprocessing & pandas to tensor
if use pandas, how to read files can be found at [pandas-io-tutorial](https://pandas.pydata.org/docs/user_guide/io.html ), details can be found at general functions.

```python
inputs, targets = data.iloc[:, 0:2], data.iloc[:, 2]
inputs = pd.get_dummies(inputs, dummy_na=True) # use dummy for encode the data into dummy, also add the  column to indicate dummies. -> 
print(inputs)
inputs = inputs.fillna(inputs.mean())   # fill NaN value  by the mean of each column 
print(inputs)
```
Data visualization tools such as seaborn, Bokeh, or matplotlib can help you to manually inspect the data and develop intuitions about what problems you may need to address.

> [!NOTE] pd.get_dummies 
> for pd.get_dummies,  if the category is used, every category will be seprated in a unique column.

> [!caution] Indexing data 
> Pandas Indexing  documentation https://pandas.pydata.org/pandas-docs/stable/user_guide/indexing.html  
> but for basics, 
> `df.loc[row_indexer,column_indexer]`
> `df.iloc[row_indexer,column_indexer]`
> should be remembered, Any of the axes accessors may be the null slice `:`

if we convert the pandas to tensor, use 
```python
torch.tensor(inputs.values)
```

for practice, we can use the appendicitis datasheet by the following code: 
```python 
from ucimlrepo import fetch_ucirepo
# fetch dataset  
regensburg_pediatric_appendicitis = fetch_ucirepo(id=938)
X = regensburg_pediatric_appendicitis.data.features  
y = regensburg_pediatric_appendicitis.data.targets
```

also 
```python
len(x)
```
is available for 1-d tensor 

### 4. Linear Algebra 
##### (1) sum and Non-Reduction Sum 
We also note that for 1-d tensor, the data form is row vector. for transpose of the tensor is column vector 
```python
A.mT
print(x.sum(axis=0))   # sum or 
```

To sum along the rows, we need to specify the axis as `axis = 0` in sum;
if we specify that the `axis=1`,it would sum along columns  

Invoking the sum function reduces a tensor along all of the axes, and eventually produce a scalar.

use *keepdims* property for keeping the number of axes unchanged, when invoking the function for calculating the sum `sum_A = A.sum(axis =1,keepdims = True)` -> this will keep the two axes after summing each row. 

then  we can perform the calculation by the matrix without dim reduction 

##### (2) dot product and matrix multiply 

> [!caution]  
> torch.dot, torch.mv and torch.mm is different two operation 

```python 
x = torch.tensor([[1,2,3,4]],dtype=torch.float32)  
y = torch.tensor([range(4)], dtype=torch.float32)  
print(torch.mm(x, y.mT))  # reach 1 scalar
print(torch.mm(x.mT, y))  # 4 x4 

# for dot product 
torch.dot(x,y);     # sum of vector 

# matrix to vector product
torch.mv(A,x);     

# matrix-matrix multiplication 
torch.mm(A,B)
```

##### (3) distance of vector and  cumsum function 
for get the distance of vector, we just use 
```python
torch.norm(x)
```
which calculate $l_2$ norm  for the matrix.also note for matrix the $l_2$ norm is
$$||X||_{F}= \sqrt{\sum^{m}_{i=1} \sum^{n}_{j=1}  x_{ij}^{2}}$$

The cumsum function is for calculate the <mark style="background: transparent; color: yellow">cumulative sum of elements</mark> 
```python
A = torch.arange(3, dtype = torch.float32)
A.cumsum()

>> tensor([1.,3.,6.])
```

## 5. Derivates and Differentation 
### (1) concepts 
for partial derivative of the y with respect to X 
$$\frac{\partial y}{\partial x_{i}} =  \frac{\partial f}{\partial x_{i}} =  \partial_{x_{i}} f  = D_{x_{i} } f$$
we note that in the deeplearning, since we work with deeply nested function, <mark style="background: transparent; color: red">chain rules</mark> which is 
$$\frac{\partial f}{\partial x_{j}} = \sum_{i}\frac{\partial f}{\partial u_{i}}\frac{\partial u_{i}}{\partial x_{j}}$$
also applies. hence: $\nabla_{x} y = A \nabla_{u} y$, A contains $\frac{\partial u_{i}}{\partial x_{j}}$
the<mark style="background: transparent; color: red"> backpropagation</mark> is built on the basis of the derivates. 

### (2) Automatic Differentation (gradient)
the detailed description of tensor can be referenced from https://pytorch.org/docs/stable/tensors.html# 

> [!warning] Clone and detach usage 
> When working with tensors prefer using [`torch.Tensor.clone()`](https://pytorch.org/docs/stable/generated/torch.Tensor.clone.html#torch.Tensor.clone "torch.Tensor.clone"), [`torch.Tensor.detach()`](https://pytorch.org/docs/stable/generated/torch.Tensor.detach.html#torch.Tensor.detach "torch.Tensor.detach"), and [`torch.Tensor.requires_grad_()`](https://pytorch.org/docs/stable/generated/torch.Tensor.requires_grad_.html#torch.Tensor.requires_grad_ "torch.Tensor.requires_grad_") for readability. Letting t be a tensor, `torch.tensor(t)` is equivalent to `t.clone().detach()`, and `torch.tensor(t, requires_grad=True)` is equivalent to `t.clone().detach().requires_grad_(True)`.

`torch.autograd` is  the autograd function , for the target we need to derive the gradient, we use the following method for derive the gradient:
```python
import torch
x = torch.arange(4,10, dtype=torch.float32,requires_grad=True)   # x.requires_grad_(True)  
print(x.grad)   # default value is None  
  
y = 2 * torch.dot(x,x)   # y = 2 * x^T x  
# firstly we calculate x to y  
y.backward()    # use backward method for derive the gradient of y  
print(x.grad) # the y derive for x (i.e.  4 * x), tensor([16., 20., 24., 28., 32., 36.])
```

> [!warning] torch.as\_tensor
> [`torch.tensor()`](https://pytorch.org/docs/stable/generated/torch.tensor.html#torch.tensor "torch.tensor") always copies `data`. If you have a Tensor `data` and just want to change its `requires_grad` flag, use [`requires_grad_()`](https://pytorch.org/docs/stable/generated/torch.Tensor.requires_grad_.html#torch.Tensor.requires_grad_ "torch.Tensor.requires_grad_") or [`detach()`](https://pytorch.org/docs/stable/generated/torch.Tensor.detach.html#torch.Tensor.detach "torch.Tensor.detach") to avoid a copy. If you have a numpy array and want to avoid a copy, use [`torch.as_tensor()`](https://pytorch.org/docs/stable/generated/torch.as_tensor.html#torch.as_tensor "torch.as_tensor"). 
> 
> note torch.as_tensor would not copy data but share the same storge with the numpy array.

since Pytorch don't reset the buffer for the gradient, if we need to clear the gradient for x, use:
```python
x.grad.zero_()
```

Also, for non-scalar variables, the natural <mark style="background: transparent; color: red">interpolation of the derivative of vector y respect to vector x</mark> is a <b><mark style="background: transparent; color: blue">Jacobian Matrix</mark></b>. 

since the **gradient of non-scalar would elicit error**, but we can <mark style="background: transparent; color: red">reduce the object to a scalar</mark>. we can use the following technique: 

Parameter for [torch.Tensor.backward](https://pytorch.org/docs/stable/generated/torch.Tensor.backward.html) : 
- **gradient** ([_Tensor_](https://pytorch.org/docs/stable/tensors.html#torch.Tensor "torch.Tensor") _or_ _None_) – Gradient w.r.t. the tensor. If it is a tensor, it will be automatically converted to a Tensor that does not require grad unless `create_graph` is True. None values can be specified for scalar Tensors or ones that don’t require grad(treated as scalar tensor). If a None value would be acceptable then this argument is optional.

```python title:change_the_tensor_to_scalar
y = x * x   # also is dot(x,x) use a scalar  
print("y=", y)  
y.backward(gradient=torch.ones(len(y)))  # Invoking the backward on a non-scalar elicits,  
# i.e. don’t require grad(i.e. reduce it to a scalar), this can also use  y.sum().backward();  
print(x.grad)
```

> [!NOTE] Manual gradient layouts 
If you need manual control over `.grad`’s strides, assign `param.grad =` a zeroed tensor with desired strides before the first `backward()`, and never reset it to `None`. 3 guarantees your layout is preserved as long as `create_graph=False`. 4 indicates your layout is _likely_ preserved even if `create_graph=True`.

### (3) Detach gradient  
if  we need to track another variable named z, which is  u * y, where `u = y.detach()`, we can firstly <mark style="background: transparent; color: red">retain the calculation graph of the middle variables</mark>. 

```python 
y = x * x   # also is dot(x,x) use a scalar  
y.backward(gradient=torch.ones(len(y)),retain_graph=True)
x.grad.zero_()   # clean the grad calculation result 
u = y.detach()   # make a copy without tracking the gradient  
z = u * y  
z.sum().backward()   # start tracking the  gradient of z  
print(x.grad)
```

We also note that if we use `autograd` function, <mark style="background: transparent; color: red">the benifit of automatic differentation is even  if building the graph of the function required, the  gradient is still calculated</mark>. This is called a <b><mark style="background: transparent; color: blue">dynamic control flow</mark></b>. 

```python 
def f(x):  
    y = torch.zeros(len(x))  
    for i in range(len(x)):  
        if x[i] < 6:  
            y[i] = x[i] + 9  
        else:  
            y[i] = -x[i] - 9  
    return y  
  
x0 = torch.randn(3,dtype=torch.float32) * 10  
x0.requires_grad_(True)  
print("x0: ", x0)  
  
y0 = f(x0)  
y0.sum().backward()  
print(x0.grad)
```

## 6. Probability Static Knowledges 
Note that the target of deeplearning is not only predict a value but also to quantify its uncertainty. The  unknown is called **==Target==** andthe  known is called ==**Features**==. 

Also note we can reach moltinomial distribution by 
```python
from torch.distributions.multinomial import Multinomial

print(Multinomial(100, torch.tensor([0.5, 0.5])).sample())
# randomly sample from two probabilitys, like tensor([47, 53])
```

The distributions documentations are listed at https://pytorch.org/docs/stable/distributions.html# 
![[attachments/Pasted image 20240603224930.png|450]]


