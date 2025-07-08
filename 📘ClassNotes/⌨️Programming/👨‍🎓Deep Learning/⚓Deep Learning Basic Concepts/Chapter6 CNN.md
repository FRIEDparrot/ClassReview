---
~
---
LeNet is the first convolutional   network  that successfully deployed. 
firstly, for the  fully conneted layers to convolutions,  it is still a   structure-less network. also the  input dimension would grow unwieldly when the input dimension is large.  

## 1. Introduction 
### (1) Basics 
the images exhibit  a rich structure. So convolutional neural network is good  structure in natural images learning. 

Consider : 
1. The object recognition not rely on its location. so we may sweep the image with a detector. -> CNN systematize the <b><mark style="background: transparent; color: orange">spatial invariance</mark></b>. so we can learn it with fewer parameters. 
2. translation  invariance  
3. should able to capture longer-range features of the image 

Firstly, we can Constrain the MLP method, if we use a tensor for it, and  output a $i\times  j$  image from a $k  \times  l$ image, then we have : 
$$H_{i, j} = U_{i,j} + \sum_{k} \sum_{l} W_{i,j, k,l} X_{k,l}$$
so we can re-write that into : 
$$H_{i,j} = U_{i,j} + \sum_{a} \sum_{b}  W_{i, j,i +a, j+ b} X_{i + a, j +b} = U_{i,j} +  \sum_{a} \sum_{b}  V_{i,j, a,b}X_{i + a, j +b}$$
for fully connected layer,  $a$ is from $-i$ to $k-i$,  consider the  shit invariance, $U, V$ don't actually depend on $i,j$, so the 
$$\boxed{\Large H_{i,j} = u + \sum_{a} \sum_{b} V_{a,b}X_{i + a, j +b}}$$
since V needs  much less parameters,  also consider the locality,  we can just use  
$$H_{i,j} = u  +  \sum_{a =  - \Delta }^{\Delta} \sum_{b=- \Delta}^{\Delta}  [V]_{a,b} X_{i + a, j +b}$$
which need $4 \Delta^2$  parameters to work. 

we note here about the [[📘ClassNotes/📐Mathmatics/ℹ️Complex Variables/第七章 积分变换预备知识|integral definition]] of convolution (the about is just a  2-dimension discretization): 
$$\int^{ + \infty}_{-\infty} f_1(x) f_2(t - x) dx$$
note here the convolution theorem :
$$\boxed{\Large \mathcal{F}^{-1} [\mathcal{F}(f) \mathcal{F}(g)] =  f   * g}$$

> [!note] Appication 
> Use [[📘ClassNotes/👨‍🔧Mechanics/🏃Dynamics & controls/🧪Vibration testing Techniques/第六章 频域分析及数字信号处理技术#(2) 快速傅里叶变换(FFT)|Fast FFT]] is  a normal technique for accelerate the convolution process, see [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Implements/FFT  Based Convolution|FFT  Based Convolution]]

for 3 channel like RGB, we <b><mark style="background: transparent; color: orange">specify the output channel in the hidden layer representation H as</mark></b> $d$ , then we got : 
$$\boxed{\Large H_{i,j} = u  +  \sum_{a =  - \Delta }^{\Delta} \sum_{b=- \Delta}^{\Delta}  [V]_{a,b, c,d} X_{i + a, j +b, c} }$$
so the convolution layer is really simple : 
```python

class Conv2d(nn.Module):
	def __init__(self, kernel_size): 
		super().__init__()
		self.w = 
		self.b = 
		.... 
	def corr2d(X, V): 
		w,h = V.shape   #  kernel shape 
		w_o = X.shape[0] -  w + 1, h_o = X.shape[1] - h + 1
		O = torch.zeos((w_o, h_o)) 
		for i in range (w_o): 
			for i in range (h_o):
				O[i,j] = ..... 
	
	def forward(x): 
		return self.corr2d(self.w, x) + self.b
```

in Conv2d function, we just use : 
```python
nn.conv2d(kerner_ize= (3,3), bias=False)
```

we note that the kernels are learned from the  data  
### (2) Padding and Stride  
Padding is the edge extension, and stride is the step the kernel to move 

padding: 
![[attachments/Pasted image 20250214221202.png]]

stride : 
![[attachments/Pasted image 20250214221244.png|500]]
output shape calculation is ease to comprehense : 
$$\text{output size}= \text{floor}\left(\frac{n_{h} - k_{h} + p_{h} + s_{h}}{s_{h}}\right) \times \text{floor}\left( \frac{n_{w} - k_{w} +  p_{w} + s_{w}}{s_{w}} \right)$$

### (3) Multi Input and Multi Output Channels 
For multi input channels,  if we build per kernal for each input, we can just add the result of different channels together as output. 

```python
def corr2d_multi_in(X, K):
	return sum(corr2d(x,k), for x,k in zip (X, K))

def corr2d_multi_in_out(X,K):
	return torch.stack([corr2d_multi_in(X,k) for k in K], 0) #   second param is dim 
```

for 3 channels input and  2 channels output, we need  6 kernels,  which means the following computation structure : 
![[attachments/Pasted image 20250214222715.png|400]]
### (4)  Pooling 
the most popular pooling methods is max pooling and average pooling, which has been declared in [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/👨‍🎓机器学习算法(sklearn)/7.  AdaBoost 图片分类和 CNN 卷积神经网络#2. 池化层|CNN convolutional basics]]. 


### (5) LeNet Concept 
LeNet is the most basic neural network` include the following layers : 
1. Multiple Convolutional Layers 
2. Pooling Layers (conv -> pool -> conv -> pool)
3. Linear Layers (dense Layer )

a simple LeNet-5 just consisits of 
1. a convolutional encoder consisting  2  convolutional layers.  
2. a dense block  consisits of fully connected layers; 

we have made a good example in [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Chapter5 Essential Concepts & Heuristics#5. FASHION MINIST Practice|FASHION MINIST Practice]], so we not do it vague here. 

## 2. Modern Convolutional  neural network

> [!CAUTION] Why use Lazy Networks  
Common Lazy Networks include LazyLinear, LazyConv2d, etc. Their input size is determined by the output of the previous layer, allowing them to flexibly handle different input shapes. This adaptability makes them better suited for transfer to different datasets.

### (1) AlexNet 
The basic  deep neural  network are as follows :
1.  <b><mark style="background: transparent; color: orange">AlexNet</mark></b>, which is deployed for large-scale vision challenge
2. VGG network 
3. Network in network  (NiN)
4. GoogleLeNet 
5. residual network (ResNet) 
6. DenseNet, ShiftMet  MobileNet  v3, etc 

AlexNet is 8-layer Convolutional Neural Network,  which have  5 convolutional  layers, also, <b><mark style="background: transparent; color: orange">AlexNet used the ReLU instead of sigmoid as the activation function</mark></b>.  
![[attachments/Pasted image 20250217095628.png|400]]


> [!HINT] Why ReLU  function makes more efficiency  
> the ReLU activation function makes model training easier when using different parameter initialization methods. This is because, when the output of the sigmoid activation function is close to  0 or 1, the gradient is almost 0, hence this part makes the backpropagation <mark style="background: transparent; color: red">can't continue to update such model  parameters</mark>.  


for AlexNet,  the network structure is : 
```python 
class AlexNet(nn.Module):
def __init__(self, lr=0.1, num_classes=10):
	super().__init__()
	self.net = nn.Sequential(
	nn.LazyConv2d(96, kernel_size=11, stride=4, padding=1),
	nn.ReLU(), nn.MaxPool2d(kernel_size=3, stride=2),
	nn.LazyConv2d(256, kernel_size=5, padding=2), nn.ReLU(),
	nn.MaxPool2d(kernel_size=3, stride=2),
	nn.LazyConv2d(384, kernel_size=3, padding=1), nn.ReLU(),
	nn.LazyConv2d(384, kernel_size=3, padding=1), nn.ReLU(),
	nn.LazyConv2d(256, kernel_size=3, padding=1), nn.ReLU(),
	nn.MaxPool2d(kernel_size=3, stride=2), nn.Flatten(),
	nn.LazyLinear(4096), nn.ReLU(), nn.Dropout(p=0.5),  # randomly dropout  
	nn.LazyLinear(4096), nn.ReLU(),nn.Dropout(p=0.5),
	nn.LazyLinear(num_classes))

def forward(self, X):
	return self.net(X)
```

Note compare to the network structure of the net of  [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Chapter5 Essential Concepts & Heuristics#5. FASHION MINIST Practice|Chapter5 Essential Concepts & Heuristics]], which gains 90%+ accuray on fashion MINIST, there is  more drop layer (regularization method),  but lack of flatten layer and batchnorm layer.  

also, <b><mark style="background: transparent; color: orange">weight_decay parameter</mark></b> (L2 normalization) parameter is recommended. 

```python 
self.net = nn.Sequential(  
	# First Convolutional Block  
	nn.Conv2d(1, 16, kernel_size=3, stride=1, padding=1, bias=True),  # 28x28 -> 16 * 28x2  
	nn.LeakyReLU(),  # Retain minor activations for subtle features  
	nn.Conv2d(16, 64, kernel_size=3, stride=1 , padding=1, bias=True),  
	nn.LeakyReLU(), 
	# add ReLU here is very good for the feature detection of images, Leaky ReLU may retain some key features  
	# Strided Convolution for Downsampling, Replace Pooling with Strided Convolution for less feature loss            nn.Conv2d(64,128, kernel_size=3, stride=1, padding=1, bias=True),  
	nn.BatchNorm2d(128),  
	nn.LeakyReLU(),  
	nn.Conv2d(128, 128, kernel_size=3, stride=2, padding=1, bias=True),  # 128x28x28 -> 128x14x14 (stride = 2 decrease the size of the image)  
	nn.BatchNorm2d(128),  
	nn.LeakyReLU(),  

	# Third Convolutional Block (optional for additional depth)  
	nn.Conv2d(128, 256, kernel_size=3, stride=1, padding=1, bias=True),  # 128x14x14 -> 256x14x14  
	nn.BatchNorm2d(256),  
	nn.LeakyReLU(), 
	nn.Flatten(),  
	nn.Linear(256 * 14 * 14, 1024, bias=True),  
	nn.Dropout(p=0.3),  
	nn.ReLU(),  # not use softmax for train  
	nn.Linear(1024, 256, bias=True),  
	nn.ReLU(),  
	nn.Linear(256, 10, bias=True),  
)  
```

### (2) VGG Network 
VGG (Visual Geomtry Group), which has the structure as follows : 

The key difference is that <b><mark style="background: transparent; color: orange">the convolutional layers are grouped in nonlinear transformations that leave the dimensonality unchanged, followed by a resolution-reduction step,</mark></b> as depicted in Fig.8.2.1. 
![[attachments/Pasted image 20250217104039.png|500]]  

**VGG blocks are structured as follows** : 
```python
class VGG(nn.Module):
def __init__(self, arch, lr=0.1, num_classes=10):
	super().__init__()
	conv_blks = []
	for (num_convs, out_channels) in arch:
		conv_blks.append(self.vgg_block(num_convs, out_channels))
	self.net = nn.Sequential(
		*conv_blks,
		nn.Flatten(),
		nn.LazyLinear(4096), nn.ReLU(), nn.Dropout(0.5),
		nn.LazyLinear(4096), nn.ReLU(), nn.Dropout(0.5),
		nn.LazyLinear(num_classes))
		self.net.apply(d2l.init_cnn)
	) 
def vgg_block(num_convs, out_channels):
	layers = []
	for _ in range(num_convs):
		layers.append(nn.LazyConv2d(out_channels, kernel_size=3, padding=1))
		layers.append(nn.ReLU())
		layers.append(nn.MaxPool2d(kernel_size=2,stride=2))
		return nn.Sequential(*layers)
# where vgg_block is initailized on the upper  field  

# then we can use 
VGG(arch=((1, 64), (1, 128), (2, 256), (2, 512), (2, 512))).layer_summary((1, 1, 224, 224))
```
in that network, the size change is  `64 x 112 x 112 -> 128 x56 x 56 -> 256 x28 x 28 -> 512  x14 x 14 -> 512 x 7x 7 -(flatten)-> 25088  ->  4096 -> 4096 -> 10`

### (3) NiN (Network in Network)
NiN (Network in Network) is  developed to <mark style="background: transparent; color: red">handle the tremendous memory usage  of the fully connected layer at the end of the architecture</mark>. 

For VGG-11 above, need  25088 x 4096 matrix for calculation, which occupies about 400MB of RAM space.  which is a significant impediment to computation. 

The strategy of  NiN is very simple, which use different block and has no fully connected layer at the end: 
1.  use <b><mark style="background: transparent; color: orange">1 × 1 convolutions to add local nonlinearities across the channel activations</mark></b> 
2. use **global average pooling to integrate across all locations in the last representation layer**. (format 7 *  7 to 1 * 1 to make a global pooling and reduce  the storge usage).

1 layer of NiN consists of 3 layers,  which different in the first input layer dimension.  
![[attachments/Pasted image 20250217113639.png|450]]
with the structure declared above, we give the following code for taining network: 
```python
class NiN(nn.Module):
def __init__(self, lr=0.1, num_classes=10):
	super().__init__()
	self.net = nn.Sequential(
		nin_block(96, kernel_size=11, strides=4, padding=0),
		nn.MaxPool2d(3, stride=2),
		nin_block(256, kernel_size=5, strides=1, padding=2),
		nn.MaxPool2d(3, stride=2),
		nin_block(384, kernel_size=3, strides=1, padding=1),
		nn.MaxPool2d(3, stride=2),
		nn.Dropout(0.5),
		nin_block(num_classes, kernel_size=3, strides=1, padding=1),
		nn.AdaptiveAvgPool2d((1, 1)),
		nn.Flatten())
	def nin_block(out_channels, kernel_size, strides, padding):
		return nn.Sequential( 
			nn.LazyConv2d(out_channels, kernel_size, strides, padding), nn.ReLU(),
			nn.LazyConv2d(out_channels, kernel_size=1), nn.ReLU(),
			nn.LazyConv2d(out_channels, kernel_size=1), nn.ReLU())
```

### (4) Multi-Branch GoogleeNet 
GoogleNet use a structure that combines the strength off NiN, repeated blocks and  a <mark style="background: transparent; color: red">cocktail of convolution kernels</mark>.  It is arguably also the first network that exhibits a clear distinction among the stem (data ingest), body (data processing), and head (prediction) in a CNN. 

![[Excalidraw/Chapter6 CNN 2025-02-17 11.54.28]]
The key contribution in GoogLeNet was the <b><mark style="background: transparent; color: orange">design of the network body. It solved the problem of selecting convolution kernels in an ingenious way</mark></b>, in googleNet used a <b><mark style="background: transparent; color: orange">multi-branch convolutions </mark></b>,  

firstly, we called the **convolutional block**  in GoogleLeNet is an <b><mark style="background: transparent; color: orange">Inception block</mark></b>. Note while other works tried to **identify which convolution, ranging from 1×1 to 11×11 would be best**, and original design included <b><mark style="background: transparent; color: orange">a number of tricks to stabilize training through intermediate loss functions</mark></b>. 

for <b><mark style="background: transparent; color: blue">inception  block</mark></b>, the data structure is like following: 
![[attachments/Pasted image 20250217120029.png|500]]
The 4 convolutional layers with different convolution  method extract different features from the input, and finally concatenate them together. 

> [!caution] Hyperparameters 
> The commonly tuned hyperparameters of the Inception block are **the number of** **output channels** per layer, i.e., how to allocate capacity among convolutions of different size, which should be specified mannually.  
> 
> the finally output channel number is the sum of all the convolutional layers. 

```python
import torch
from torch import nn
from torch.nn import functional as F
from d2l import torch as d2l

class Inception(nn.Module):
	# `c1`--`c4` are the number of output channels for each branch
	def __init__(self, c1, c2, c3, c4, **kwargs):
		super(Inception, self).__init__(**kwargs)
		# Branch 1
		self.b1_1 = nn.LazyConv2d(c1, kernel_size=1)
		# Branch 2
		self.b2_1 = nn.LazyConv2d(c2[0], kernel_size=1)
		self.b2_2 = nn.LazyConv2d(c2[1], kernel_size=3, padding=1)
		# Branch 3
		self.b3_1 = nn.LazyConv2d(c3[0], kernel_size=1)
		self.b3_2 = nn.LazyConv2d(c3[1], kernel_size=5, padding=2)
		# Branch 4
		self.b4_1 = nn.MaxPool2d(kernel_size=3, stride=1, padding=1)
		self.b4_2 = nn.LazyConv2d(c4, kernel_size=1)
	def forward(self, x):
		b1 = F.relu(self.b1_1(x))
		b2 = F.relu(self.b2_2(F.relu(self.b2_1(x))))
		b3 = F.relu(self.b3_2(F.relu(self.b3_1(x))))
		b4 = F.relu(self.b4_2(self.b4_1(x)))
		return torch.cat((b1, b2, b3, b4), dim=1)
```

also, actually the googleNet  use the  following structure is  as follows, <b><mark style="background: transparent; color: orange">the following structure contains 3 inspection block :</mark></b> 
![[attachments/Pasted image 20250218094315.png|600]]
for example, we just use : 
```python
@d2l.add_to_class(GoogleNet)
def b5(self):
	return nn.Sequential(Inception(256, (160, 320), (32, 128), 128),
						Inception(384, (192, 384), (48, 128), 128),
						nn.AdaptiveAvgPool2d((1,1)), nn.Flatten())
```

## 3. Batch Normalization 
### (1) Batch Normalization Concepts 
batch normalization can <b><mark style="background: transparent; color: orange">accelerate the converge of  deep networks</mark></b>. A secondary (serendipitous) benefit of batch normalization lies in its inherent regularization. 

It postulated that **drift in the distribution of such variables could hamper the convergence of the network**.

adaptive  normalization  is  to prevent  the  layer variabe activation to be too extreme. also note that Adaptive solvers such as AdaGrad (Duchi et al., 2011), Adam (Kingma and Ba, 2014), Yogi (Zaheer et al., 2018), or Distributed Shampoo (Anil et al., 2020) aim to address this from the viewpoint of optimization, e.g., by adding aspects of second-order methods 

we denote a minibatch by $B$, then  
$$\text{BN}(x) = \gamma\space  \odot \space  \frac{x - \hat{\mu}_{B}}{\hat{\sigma}_{B}} + \beta$$
where  $\gamma$ is  scale parameter,  $\beta$ is  shift parameter. 

we can calculate $\hat{\mu}_B$ and $\hat{\sigma}_B$ as : 
$$\hat{\mu}_B = \frac{1}{|B|} \sum_{x \in B} x \text\qquad   \hat{\sigma}^2_B = \frac{1}{|B|} \sum_{x \in B} (x - \hat{\mu}_B)^2 + \epsilon$$
and calcuate the new element as : 
$$\hat{x} \rightarrow  \text{LN} (x) = \frac{x - \hat{\mu}_{B}}{\hat{\sigma}_B}$$
Note that we add a small constant $\varepsilon > 0$ to the variance estimate to ensure that we never attempt division by zero, note that the  noiseness is a  benificial rather than problem. 

> [!NOTE] Best Minibatch size 
> The  batch normalize  works best for the moderate minibatches size in 50 to 100 range. 
> 1. the variance woeks as a art of regularization, larger  minibatch regularizes less due to more stable estimates, whereas tiny minibatches destroy useful signal due to high variance.  
### (2) Batch Normalization Layers 
### 1. Fully connected layers
for fully connected layers,  we use $\phi$ as actiavtion function, then we can use :   
$$h = \phi(BN(Wx + b))$$

secondly, assume that minibatches have  m examples,  and the outootut of the convolution  has height $p$ and  width $q$, which give size $m * p * q$ .  

we  apply the same mean and  variance  to normalize  the value. 

we give the batch norm layer example as follows: 
```python
class BatchNorm(nn.Module):
	# `num_features`: the number of outputs for a fully connected layer
	# or the number of output channels for a convolutional layer. `num_dims`:
	# 2 for a fully connected layer and 4 for a convolutional layer
	def __init__(self, num_features, num_dims):
		super().__init__()
		if num_dims == 2:
			shape = (1, num_features)
		else:
			shape = (1, num_features, 1, 1)
		# The scale parameter and the shift parameter (model parameters) are
		# initialized to 1 and 0, respectively
		self.gamma = nn.Parameter(torch.ones(shape))
		self.beta = nn.Parameter(torch.zeros(shape))
		# The variables that are not model parameters are initialized to 0 and

		self.moving_mean = torch.zeros(shape)
		self.moving_var = torch.ones(shape)
	def forward(self, X):
		# If `X` is not on the main memory, copy `moving_mean` and
		# `moving_var` to the device where `X` is located
		if self.moving_mean.device != X.device:
		self.moving_mean = self.moving_mean.to(X.device)
		self.moving_var = self.moving_var.to(X.device)
		# Save the updated `moving_mean` and `moving_var`
		Y, self.moving_mean, self.moving_var = batch_norm(
		X, self.gamma, self.beta, self.moving_mean,
		self.moving_var, eps=1e-5, momentum=0.1)
		return Y
```

## 4. ResNet (Residual Networks) and ResNeXt
### (1) Function Class 
we consider the function $\mathcal{F}$ that a specific network architecture can reach.  we got $\forall f \in  \mathcal{F}$, $\exists w, b \text{ (parameters)}$ which can be obtained  through training on a suitable dataset. 

firstu we want to find  $f^{*}$,  if this function is in $\mathcal{F}$ ,  we would instead find  $f_\mathcal{F}^*$ which is the best bet within $\mathcal{F}$.  also given a dataset with features $X$ and labels $y$, we  find 
$$f_{F}^{*} \xlongequal{def }  \text{arg} \min_{f}\space  \text{s.t.}  f \in  \mathcal{F}$$
regularization can control the complexity of  $\mathcal{F}$, 

we assume that  we expect to find  $\mathcal{F}'$ , which  we should arrive at a better outcome. the following picture  declared that if $F \subset  F'$, we can guarantee that it get closer to the truly function $f^*$ (in fig1, it may not get closer.)

![[attachments/Pasted image 20250218115418.png|600]]

in the residual network, the fundamental consideration is that <b><mark style="background: transparent; color: orange">every additional layer should be more easily  contain the identity functionas one of oites elements</mark></b>.   later it has  been added to recurrent networks,  (which **transformers used  to stack layers of networks**)

### (2) Residual Blocks 
we use the **residual block** for  build the ResNet, which use the $x$ as  add to  the result  $g(x)$, and call the mapping $f(x)$,  since this network has mapping  
$$g(x)  = f(x) - x$$
if $f(x) = x$  is  the desired  underlying  mapping,  it down to $g(x) = 0$, which must be included when $w= 0, b = 0$, which is thus eay to learn. 
![[attachments/Pasted image 20250218120919.png|500]] 
With the knowledge above,  we can **build a residual network by the following process**: 
```python
import torch 
from torch import nn
from torch.nn import functional as F 

class Residual(nn.Module):
	"""The Residual block of ResNet."""
	def __init__(self, num_channels, use_11conv=False, strides=1):
		super().__init__()
		self.conv1 = nn.LazyConv2d(num_channels, kernel_size=3, padding=1,
			stride=strides)
		self.conv2 = nn.LazyConv2d(num_channels, kernel_size=3, padding=1)
		
		if use_11conv:
			self.conv3 = nn.LazyConv2d(num_channels, kernel_size=1,
			stride=strides)
		else:
			self.conv3 = None
			self.bn1 = nn.LazyBatchNorm2d()
			self.bn2 = nn.LazyBatchNorm2d()
	
	def forward(self, X):
		Y = F.relu(self.bn1(self.conv1(X)))
		Y = self.bn2(self.conv2(Y))
		if self.conv3:
			X = self.conv3(X)
		Y += X
		return F.relu(Y)
```
which build the network structure as follows, including 2 conv & norm block with ReLU activation function inside. 
![[Excalidraw/Chapter6 CNN 2025-02-18 12.20.14|300]]
we note the input and output dimension can be resized by change the `stride` parameter,  for `stride=2`, we got reduced dimension <mark style="background: transparent; color: red">(note in such case, we often use more output channels to retain more features)</mark>

> [!NOTE] **Pre-activation Benefits**
> In ResNet v2, the order is **BN → ReLU → Conv**. This ensures the input to each Conv layer is normalized and activated, leading to:
> 
> - **Smoother gradients**: BN before Conv reduces internal covariate shift.
> - **Better generalization**: ReLU before Conv avoids non-linearity stacking, improving feature representation.
> - If we just use 1 conv layer, the model becomes weaker, which doesn’t guarantee smooth gradient flow if `F(x)` is too simple.

> [!HINT] For Deep Network
> - **Plain CNN** (Conv → BN → ReLU) lacks residual connections, making deeper networks harder to train due to gradient issues.
> - **ResNet** explicitly models residuals, enabling stable training of 100+ layers.

for ResNet-18 network, it has the following architecture : 
![[attachments/Pasted image 20250220091928.png|550]]

```python
class ResNet(nn.module):
	def __init__(self, arch, lr=0.1, num_classes=10):
		self.net = nn.Sequential(self.b1()) 
		
		# this part add the convolution layer to k
		for i, b in enumerate(arch):
			self.net.add_module(f'b{i+2}', self.block(*b, first_block=(i==0)))
			self.net.add_module('last', nn.Sequential(
				nn.AdaptiveAvgPool2d((1, 1)), 
				nn.Flatten(),
				nn.LazyLinear(num_classes))
			)
			self.net.apply(d2l.init_cnn)
	# the firstl layer, which ingest the data 
	def b1(self):
		return nn.Sequential(
			nn.LazyConv2d(64, kernel_size=7, stride=2, padding=3),
			nn.LazyBatchNorm2d(), 
			nn.ReLU(),
			nn.MaxPool2d(kernel_size=3, stride=2, padding=1)
		)
		
	def block(self, num_residuals, num_channels, first_block=False):
		"""
		in the following picture, first block have no  use_1x1conv layer 
		
		num_residual is number of residual layers
		num_channel is output channel 
		"""
		blk = []
		for i in range(num_residuals):
			if i == 0 and not first_block:
				# reduce the output channel 
				blk.append(Residual(num_channels, use_1x1conv=True, strides=2))
			else:
				blk.append(Residual(num_channels))
				return nn.Sequential(*blk)
	
class ResNet18(ResNet):
	def __init__(self, lr=0.1, num_classes=10):
		super().__init__(((2, 64), (2, 128), (2, 256), (2, 512)), lr, num_classes) 
```

### (3) Pretrained ResNet Model Building   
> [!NOTE] Fast Build of Pretrained  ResNet Model
> In PyTorch, you can build ResNet using existing modules in the `torchvision` library. `torchvision.models` provides pre-trained ResNet models such as `resnet18`, `resnet34`, `resnet50`, `resnet101`, and `resnet152`. If you just want to use these models, you can simply load them without writing them manually.

Here is a simple example showing how to load a pre-trained ResNet model: 
```python
import torch
import torchvision.models as models

# Load the pre-trained ResNet50 model
resnet50 = models.resnet50(pretrained=True)

# Put the model into evaluation mode
resnet50.eval()

# If you need to customize the model, you can modify it based on this
# For example, replace the last fully connected layer
num_classes = 10 # Assume there are 10 categories
resnet50.fc = torch.nn.Linear(resnet50.fc.in_features, num_classes)

# You can choose to use a model with custom weights
# model.load_state_dict(torch.load('your_model_path.pth'))
```

In addition, there are other open source libraries such as `timm` (PyTorch Image Models), which provide implementations of many models, including various variants of ResNet models, for choose and use.

> [!NOTE] ByPassing Path Unit 
> Prior to residual connections, bypassing paths with gating units were introduced to effectively train highway networks with over 100 layers
### (4) ResNext
for ResNet, one challenge of design is the trade-off between **nonlinearity and dimensionality within a given block.** we apply the idea of multiple independent groups to ResNet. 

considering that the computational cost of ingesting $c_i$ and emitting $c_o$ proportional to $O(c_i \times  c_o)$,  note that different from the smorgasbord of transformations in Inception,  ResNeXt <mark style="background: transparent; color: red">adopts the same transformation in all branches, thus minimizing the need for manual tuning of each branch</mark>. 

Note the <mark style="background: transparent; color: red">use of grouped convolution with g groups is g times faster than a dense convolution</mark>, the input and output channels are divided into $g$ groups. Each group operates independently,  reducing the number of connections to $\frac{C_{in}}{g}\times \frac{C_{out}}{g}$​​ for each group. so the computational cost reduce to 
$$O\left(\frac{C_{in}}{g}\times \frac{C_{out}}{g}\right) = O (c_{i} \times  c_{o} / g)$$

![[attachments/Pasted image 20250220094503.png|650]]


for ResNext Block, 
```python
class ResNeXtBlock(nn.Module): #@save  
    """The ResNeXt block."""  
    
    def __init__(self, num_channels, groups, bot_mul, use_1x1conv=False, strides=1):  
        super().__init__()  
        bot_channels = int(round(num_channels * bot_mul))  
        self.conv1 = nn.LazyConv2d(bot_channels, kernel_size=1, stride=1)  
        # 3x 3 conv, and use `groups` parameter for different groups 
        self.conv2 = nn.LazyConv2d(bot_channels, kernel_size=3,  
                                   stride=strides, padding=1,  
                                   groups=bot_channels // groups)  
        self.conv3 = nn.LazyConv2d(num_channels, kernel_size=1, stride=1)  
        self.bn1 = nn.LazyBatchNorm2d()  
        self.bn2 = nn.LazyBatchNorm2d()  
        self.bn3 = nn.LazyBatchNorm2d()  
        
        if use_1x1conv:  
            self.conv4 = nn.LazyConv2d(num_channels, kernel_size=1, stride=strides)  
            self.bn4 = nn.LazyBatchNorm2d()  
        else:  
            self.conv4 = None  

    def forward(self, X):  
        Y = F.relu(self.bn1(self.conv1(X)))  
        Y = F.relu(self.bn2(self.conv2(Y)))  
        Y = self.bn3(self.conv3(Y))  
        
        if self.conv4:  
            X = self.bn4(self.conv4(X))  
        
        return F.relu(Y + X)
blk = ResNeXtBlock(32, 16, 1)
```

> [!HINT] `groups` parameter in  `nn.Conv`
> - **`groups=1`**: Standard convolution, where all the input channels are connected to all the output channels.
> - **`groups=n`**: Divides the input and output channels into `n` groups, meaning each group operates independently on their corresponding subset of channels. For instance, if you have 4 input channels and set `groups=2`, it will divide the input channels into 2 groups of 2 channels each, and each group will produce its own set of output channels.

## 5. DenseNet
### (1) Dense Block and transition layer 
DenseNet (Densely Connected Networks)  is to some extent the logical extension of how to parametrize the  function. 

for the point $x = 0$, the [[📘ClassNotes/📐Mathmatics/📈Advanced Mathematics/第三章 微分中值定理和导数的应用#三、泰勒公式|taylor expansion]] can be written as : 
$$f(x) = f(0) +  x \cdot  \left[f'(0) + x \cdot \left[\frac{f''(0)}{2!}  + x \cdot   \left[\frac{f'''(0 )}{3!}\right] \right] \right]$$
the ResNet is a function that **decompose to** 
$$f(x) = x + g (x) $$
if we want to capture the further infomation  beyond  $x$ and $g(x)$ terms, we use DenseNet for 

![[attachments/Pasted image 20250220104359.png|550]]

**The key difference between ResNet and DenseNet is that outputs are <mark style="background: transparent; color: red"> concatenated rather than plused together</mark>**. so we can perform a mapping from $x$ to
$$x \rightarrow   [x , f_{1} (x),  f_{2} ([x, f_{1} (x)]), _{3}([x, f_{1} (x), f_{2}([x, f_{1}(x)])]), .... ]$$

all these functions  **are combined in MLP to reduce the number  of features again**, which gives the  following dependency graph: 
![[attachments/Pasted image 20250220110956.png|550]]

and we can gives the following build process example :
```python
import torch
from torch import nn

def conv_block(num_channels):
    return nn.Sequential(
        nn.LazyBatchNorm2d(), 
        nn.ReLU(),
        nn.LazyConv2d(num_channels, kernel_size=3, padding=1)
    )
    
class DenseBlock(nn.Module):
    def __init__(self, num_convs, num_channels):
        super(DenseBlock, self).__init__()
        layer = []
        for i in range(num_convs):
            layer.append(conv_block(num_channels))   # add mutliple dense block 
        self.net = nn.Sequential(*layer)

    def forward(self, X):
        for blk in self.net:
            Y = blk(X)
            # Concatenate input and output of each block along the channels
            X = torch.cat((X, Y), dim=1)
        return X

blk = DenseBlock(2, 10)
# we define a DenseBlock instance with 2 convolution blocks of 10 output channels. 
#       in that case When using an input with 3 channels, we will get an output with 3+10+10 = 23 channels.
```

The number of convolution block channels **controls the growth in the number of output channels relative to the number of input channels**. This is also referred to as the <b><mark style="background: transparent; color: orange">growth rate</mark></b>. 

- **Transition Layers**
considering that the  dense block  would quickly increase the  number of channels, , adding too many of them will lead to an excessively complex model. A **transition layer** is used to control the complexity of the model. 
```python
def transition_block(num_channels):
	# reduces the number of channels by using an 1 × 1 convolution. 
    return nn.Sequential(
        nn.LazyBatchNorm2d(), 
        nn.ReLU(),
        nn.LazyConv2d(num_channels, kernel_size=1),  # control output channels
        nn.AvgPool2d(kernel_size=2, stride=2)   #  reduce output size 
    ) 
```
### (2) DenseNet model building
For Building DenseNet model, we gives the following example to build the  DenseNet model: 
```python
import torch
from torch import nn
from d2l import torch as d2l

class DenseNet(nn.Module):
    def __init__(self, num_channels=64, growth_rate=32, arch=(4, 4, 4, 4), lr=0.1, num_classes=10):
        super(DenseNet, self).__init__()
        self.net = nn.Sequential(self.b1()) # add first branch 

		# define 4 x 4 x 4 x 4 convolution layer. which grows 4 * 32 + 64 = 192 channels 
        for i, num_convs in enumerate(arch):
            self.net.add_module(f'dense_blk{i+1}', DenseBlock(num_convs, growth_rate))
            # The number of output channels in the previous dense block
            num_channels += num_convs * growth_rate
            
            # Transition layer that halves the number of channels is added here  
            if i != len(arch) - 1:
                num_channels //= 2
                self.net.add_module(f'tran_blk{i+1}', transition_block(num_channels))  # 81 channels

		# use an MLP as the last layer constructure 
        self.net.add_module('last', nn.Sequential(
            nn.LazyBatchNorm2d(), nn.ReLU(),
            nn.AdaptiveAvgPool2d((1, 1)), nn.Flatten(),
            nn.LazyLinear(num_classes)
        ))
        self.net.apply(d2l.init_cnn) 
        
	def b1(self):
        return nn.Sequential(
            nn.LazyConv2d(64, kernel_size=7, stride=2, padding=3),
            nn.LazyBatchNorm2d(), nn.ReLU(),
            nn.MaxPool2d(kernel_size=3, stride=2, padding=1)
        )
# Create a DenseNet model
model = DenseNet(lr=0.01) 
# Set up the trainer (in actual application,  we define Adams or SGD as Trainer)
trainer = d2l.Trainer(max_epochs=10, num_gpus=1) 
# Load the FashionMNIST dataset
data = d2l.FashionMNIST(batch_size=128, resize=(96, 96)) 
# Train the model
trainer.fit(model, data)
```

> [!error] Fault
> unlike ResNet, where inputs and outputs are added together, DenseNet concatenates inputs and outputs on the channel dimension. Although these concatenation operations reuse features to achieve computational efficiency, unfortunately they lead to heavy GPU memory consumption. As a result, applying DenseNet may require more memory-efficient implementations that may increase training time

### (3) Summary
for a compare of different network performance, we can refer following picture tested on COVID-19 dataset, where DenseNet-169 followed closely with an accuracy of 93.15% :
![[attachments/Pasted image 20250220105927.png|500]]

## 6.  Automating  neural  network design
### (1) Design Space 
Design network design space explore the structure the aspect of  the network design from  initial <b><mark style="background: transparent; color: orange">AnyNet design space</mark></b>. 

![[attachments/Pasted image 20250220124144.png|550]] 
AnyNet assume  standard, fixed blocks like ResNext, 

in such parametric network, use stage for designing the inner blocks 

```python
import torch
from torch import nn
from d2l import torch as d2l

class AnyNet(nn.Module):
    def __init__(self, arch, stem_channels, lr=0.1, num_classes=10):
        super(AnyNet, self).__init__() self.save_hyperparameters()
	self.net = nn.Sequential(self.stem(stem_channels))
	for i, s in enumerate(arch):
		self.net.add_module(f'stage{i+1}', self.stage(*s))
		self.net.add_module('head', nn.Sequential( nn.AdaptiveAvgPool2d((1, 1)), nn.Flatten(), nn.LazyLinear(num_classes)))
		self.net.apply(d2l.init_cnn)

    def stage(self, depth, num_channels, groups, bot_mul):
        blk = []
        for i in range(depth):
            if i == 0:
                blk.append(d2l.ResNeXtBlock(num_channels, groups, bot_mul, use_1x1conv=True, strides=2))
            else:
                blk.append(d2l.ResNeXtBlock(num_channels, groups, bot_mul))
        return nn.Sequential(*blk)
```
then  for AnyNet it search the space of stage designation.  

### (2) Constraint of Design Space  
We constraint the Design Space with <mark style="background: transparent; color: red">Lower Error Distributions</mark> 

the error empirical distribution function $F(e)$ 
$$F(e) = \frac{1}{n} \sum_{i = 1}^{n} 1 (e_{i}< e)$$
![[attachments/Pasted image 20250220130724.png]]

### (3) RegNet 
RegNet is  a simple regular network explored by ANN desgin. 
The resulting AnyNetX_E design space consists of simple networks following easy-to-interpret design principles:

- Share the bottle network ratio $b_i = b$ for all stages $i$;
- Share the number of groups \( $g_i = g$ \) for all stages \( $i$ \);
- Increase network width across stages: \( $w_i \leq w_{i+1}$ \);
- Increase network depth across stages: \( $d_i \leq d_{i+1}$ \).

```python
class RegNet32(AnyNet):
    def __init__(self, lr=0.1, num_classes=10):
        stem_channels, groups, bot_mul = 32, 16, 1
        depths, channels = (4, 6), (32, 80)
        super().__init__(
            ((depths[0], channels[0], groups, bot_mul),
             (depths[1], channels[1], groups, bot_mul)),
            stem_channels, lr, num_classes)

RegNet32().layer_summary((1, 1, 96, 96))
model = RegNet32(lr=0.05)
trainer = d2l.Trainer(max_epochs=10, num_gpus=1)
data = d2l.FashionMNIST(batch_size=128, resize=(96, 96))
trainer.fit(model, data)
```
