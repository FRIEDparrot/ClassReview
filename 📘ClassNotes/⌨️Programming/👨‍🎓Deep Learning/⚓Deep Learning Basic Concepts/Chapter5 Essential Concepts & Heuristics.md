## 1. Basics in Deep Neural Network 
### (1) Basic Concepts  
Multi-Layer Perception is the neural network with not more than 1 layer of  hidden layer.  

Kernel methods have been used for may decades to model the nonlinear dependencies. so we often overcome the  limitations of the linear models by <mark style="background: transparent; color: red">incorporating one or more layer of hidden layers</mark>.

It's easy to see that multi-layer of the linear model is equal to 1-layer linear model , see [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/👨‍🎓机器学习算法(sklearn)/9. 正则化方法, 概率图模型和贝叶斯网络#二、多层感知机|MultiLayer Perceptions]]. where we add <mark style="background: transparent; color: red">activations in the hidden layer</mark>.
$$O =\phi (XW_h + b_h)W_o + b_{o}$$
note **after computing the linear portion of the layer , we can calculate the activation without looking at the values taken by other hidden units**. 

See [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/👨‍🎓机器学习算法(sklearn)/6. 最小二乘, RBF, 岭回归和 Lasso 回归#(2) 径向基函数网络|Radial Basis Function]], if enough hidden nodes are given, any complex functions can be modeled. Note single-layer 

The often-used activation function -> See [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/👨‍🎓机器学习算法(sklearn)/Sklearn的使用基础和基本代码#(2) 激活函数专题|Most used Activation Functions]] 

### (2) Forward Propagation, Back Propagation and Computational Graphs
Forward Propagation <mark style="background: transparent; color: red">refers to the calculation and storage of intermediate variables</mark>. 
In the **forward propagation**,  when it come to calculate the gradients, we invoke the backpropagation function. 

we can calculate intermediate variable as:
$$\boxed{z = W^{(1)} x}$$
with activation function $\phi$, we  have:
$$\boxed{h= \phi(z) }$$
assuming the parameters  of the output layer only contains the weight $W^{(2)}$ 
$$o = W^{(2)} h$$
then given the hyper parameter $\lambda$, the **L2 regularization term** 
$$s = \frac{\lambda}{2} \left(||W^{(1)}||^{2} +  ||W^{(2)}||^{2} \right)$$
then minimize objective function :
$$J = l(o, y) +  s$$

for computational graph of forward propagation, it is like: 
![[Excalidraw/Chapter5 Multilayer Perception 2024-12-21 14.24.27|500]]
### (3) Back Propagation 
See [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Implements/1. 深度学习基础简介#(1) 正向传播和反向传播|Back Propagation]], back propagation use the chain rule of derivation, that is 
$$\frac{\partial Z}{\partial X} = \prod \left(\frac{\partial Z}{\partial Y}\frac{\partial Z}{\partial X}\right)$$
For higher dimensional tensors, with $J = s + L, L = (y - o)^{2}$ then:
$$\frac{\partial J}{\partial o} = \frac{\partial J}{\partial L} \frac{\partial L}{\partial o}  = 2 (y -  o)$$
considering that $s = \frac{\lambda}{2} \left(||W^{(1)}||^{2} +  ||W^{(2)}||^{2} \right)$,  we have: 
$$\frac{\partial s }{\partial W^{(1)}} = \lambda W^{(1)} \qquad  \frac{\partial s}{\partial W^{(2)}} = \lambda W^{(2)}$$
and also, we can calculate 
$$\frac{\partial J}{\partial W^{(2)}} = \frac{\partial J}{\partial o} \frac{\partial o}{\partial W^{(2)}} +  \frac{\partial  J}{\partial s} \frac{\partial s}{\partial W^{(2)}} = 2(y- o) h^{T}  + \lambda W^{(2)} $$
then similarly we get: 
$$\frac{\partial J}{\partial W^{(1)}} =  \frac{\partial J}{\partial h} \frac{\partial h}{\partial W^{(1)}} +  \frac{\partial  J}{\partial s} \frac{\partial s}{\partial W^{(1)}}  = \frac{\partial J}{\partial h} \frac{\partial h}{\partial W^{(1)}} +  \lambda W_{1}$$
where:
$$\frac{\partial J}{\partial h} = \left(\frac{\partial J}{\partial o} \frac{\partial o}{\partial h}\right) = W^{(2)T} 2(y - o)$$
$$\frac{\partial h}{\partial W_{1}} =  \phi'  x ^{T}$$
in the real computing process, we travel the computation graph in the direction of dependencies and compute all the variables on the path.

we note that the **hidden layer $h$ is a key point to calculate the derivation**. so we store the derivations of $\frac{\partial o}{\partial h}$ and $\frac{\partial h}{\partial W}$ to save the calculation time. 

usually, when we calculate the backward for a second time or for another variable, we would set `retain_graph=True` parameter.  
```python
loss.backward(retain_graph=True)
# if not need computation graph any more, use:
loss.grad.zero_()
```

Note : backpropagation reuse the **stored intermediate values** from forward propagation  to **avoid duplicate calculations**. so it takes significantly more memory, if the batch size is large, <mark style="background: transparent; color: red">it's easy to lead to out of memory error</mark>.


### (4) Numerical Stability and Initialization 
If we initialize the parameters according to pre-specified distribution, <mark style="background: transparent; color: red">the choice of the initialization scheme plays a significant role in neural network learning</mark>, and maintain numerical stability is crucial. also, <b><mark style="background: transparent; color: orange">the choice can be tied up  with the choice  of the nonlinear activation function. </mark></b> also, Which  function we choose can how we initialize the  parameter  determine ho quickly the optimization algorithm converges. 

<b><mark style="background: transparent; color: orange">poor choice can lead to exploding or vanishing gradients while training.</mark></b> so we would discuss some useful heuristics for improve numerical .

#### 1. Vanishing and exploding gradients 
When consider the deep network with $L$ layers, the hidden layer output is $h^{(l)}$. 

$$h^{(l)} =  f_{l} (h^{(l -1)})  \qquad o = f_{L} \circ \dots \circ  f_{1}(x)$$
we note that  if the hidden layer output and the input are vectors, we have the derivation for the layer $W^{(l)}$ is :
$$\partial_{W^{(l)}}o  =  \partial_{h(L-1)} h^{(L)} \partial_{h(L-2)}  h^{(L-1)}  \dots  \partial_{h^{(l)}}  h^{(l+1)}  \partial_{W^{(l)}}h^{(l)} $$
we define 
$$M^{(L)} = \partial_{h(L-1)} h^{(L)}  \qquad  \text{then} \qquad  \partial_{W^{(l)}}o  = M^{(L)} M^{(L- 1)} \dots M^{(l + 1)}  h^{(l)}$$
we notice that each $M$ may have wide variety of the eigenvalues, so the product would be  var large or very small. 

1. <b><mark style="background: transparent; color: blue">for vanish gradients</mark></b>, the  **frequent culprit for vanishing gradient problem is the choice of activation function $\phi$ after the linear layer**, Historically, the [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/👨‍🎓机器学习算法(sklearn)/Sklearn的使用基础和基本代码#2. **Sigmoid 激活函数**|sigmoid function]] was popular because it resembles a <b><mark style="background: transparent; color: blue">thresholding function.</mark></b> 

the <mark style="background: transparent; color: red">sigmoid function's  gradient vanishes both when the input are  large and small. </mark> that is,  **when the  inputs o many of the  sigmoid functions are close to zero**, the gradients of the overall product may vanish.  Hence <mark style="background: transparent; color: red">the ReLU function, which are more stable</mark>(but less neurally plausible) are emerged. 

2. for exploding gradients,  if 100 gaussian random matrices are multiplied, the result would be to huge to get a gradient  descent optimizer  to converge. 


#### 2. Code Example for detecting gradient problem 
in the code deployment, there are several tricks for detect the gradient vary process : 

use `.norm(2)` for l2 normalization. 
```python
# the function to monitor the gradient varies  
def monitor_gradients(model):
    grad_norms = {}
    for name, param in model.named_parameters():
        if param.grad is not None:
            grad_norms[name] = param.grad.norm(2).item()
	# if we need to plot the gradient, we use : 
	grads = [] 
	for param in model.parameters():
		if param.grad is not None:
			grads.append(param.grad.view(-1).cpu().numpy()) 
		# then plot the gradient in figure.  
    return grad_norms

# in the train process, we can :   
for epoch in range(10):
    optimizer.zero_grad()
    outputs = model(x)
    loss = criterion(outputs, y)
    loss.backward() 
	# get the gradient after calculate it by back propagation 
    grad_norms = monitor_gradients(model)
    print(f"Epoch {epoch}: Gradient Norms: {grad_norms}")
    optimizer.step()
```

or detect the maximum and minimum of the gradient :
```python
max_grad = max(max_grad, param.grad.max().item())
min_grad = min(min_grad, param.grad.min().item())
```
#### 3. Basic handling methods 
ReLU activation functions mitigate the vanishing gradient problem, and also accelerate the convergence. 

1. we can avoid the gradient explosion by Gradient Clipping : 
```python
torch.nn.utils.clip_grad_norm_(model.parameters(), max_norm=1.0)
```
this can be add after `loss.backward()` and before `optimizer.step()` to limit the gradient range. 

also, consider using sigmoid as the first layer to avoid the  gradient loss in the network. 

2. if the gradient  norm all close to and loss is not  decrease with training, (if just  few update steps and  the loss never decrease later,  may be gradient vanish happens)   
also if  the gradient vanish happens, we may use `LeakyReLU` instead of `ReLU` activation function or use **another sample initialization method (For example,  Xavier Initialization or Kaiming Initialization)**, which will be implemented later. 

## 2. Basic Regularization method
### (1)  Parameter Initialization
additional  care during the optimization and regularization would further enhance stability, for the default initialization, we use the  normal distribution to initialize the values of the weights. it often works well with moderate problem size. 
#### 1. Xavier Initialization
for we look at the distribution of  output $o_i$  the  layer of output is given by 
$$o_{i}=\sum_{j = 1}^{n}  w_{ij} x_{j}$$
we assume that the input layer has zero mean  and the variance $o_{i}$ 
$$E[o_{i}] = \sum_{j = 1}^{n} E[w_{ij}] E[x_{j}] = 0$$
$$\text{Var}[o_{i} ] = E[o_{i}^{2}] - (E[o_{i}])^{2} =\sum_{j = 1}^{n} E[w_{ij}^{2}] E[x_{j}^{2}] = n_{in}  \sigma^{2} \gamma^{2} $$
One of the way to keep the  variance fixed is  to set $n_{in} \sigma^2  = 1$, <mark style="background: transparent; color: red">we see that the gradient's variance can blow up unless</mark> $n_{out} \sigma^2 = 1$ ($n_{out}$ is number of output) but **we can't satisfy both condition**. so we try to satisfy :
$$\frac{1}{2} (n_{in} + n_{out}) \sigma^{2} = 1 \qquad \text{or} \qquad \sigma= \sqrt{\frac{2}{n_{in} +  n_{out}}}$$
so  the  practically beneficial of Xavier initialization is  that it  <mark style="background: transparent; color: red">samples weights from a Gaussian distribution with zero mean and variance</mark> $\sigma$ above, or 
$$\sigma^{2} = \frac{2}{n_{in} + n_{out}}$$
also if we deploy it in the uniform distribution, we get : 
$$U \left(-\sqrt{\frac{6}{n_{in} + n_{out}}}, \sqrt{\frac{6}{n_{in } + n_{out}}}\right)$$

note the reasoning above barely scratches the surfaces of modern approaches to parameter initialization. actually, other heuristics  such as  shared  parameters, super-resolution , sequence models, etc. 

note for generalization of error, it must deploy after regularization  either by <mark style="background: transparent; color: red">reducing the complexity of the model class or by applying a penalty. </mark> 

adding layers, nodes,  or training for a larger number of epochs also help for generalization. 

#### 2. Pytorch Example  
PyTorch provides the Xavier Initialization method in the `torch.nn.init` module. 

it provide  `xavier_normal_` as normal distribution and `xavier_uniform_` as uniform distribution. 

```python
import torch
import torch.nn as nn
import torch.nn.init as init

# deploy the xavier initialization in the layer weight initialization. 
layer = nn.Linear(in_features=128, out_features=64)
init.xavier_uniform_(layer.weight)  # or init.xavier_normal_(layer.weight) 

class MyModel(nn.Module):
    def __init__(self):
        super(MyModel, self).__init__()
        self.fc1 = nn.Linear(128, 64)
        self.fc2 = nn.Linear(64, 32)
        self.fc3 = nn.Linear(32, 10)

    def initialize_weights(self):
        for layer in self.modules():
            if isinstance(layer, nn.Linear):
                init.xavier_uniform_(layer.weight)
                if layer.bias is not None:
                    init.zeros_(layer.bias)

    def forward(self, x):
        x = torch.relu(self.fc1(x))
        x = torch.relu(self.fc2(x))
        x = self.fc3(x)
        return x

# Instantiate model and apply Xavier initialization
model = MyModel()
model.initialize_weights()
```

### (2) Early Stopping 
See [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/👨‍🎓机器学习算法(sklearn)/9. 正则化方法, 概率图模型和贝叶斯网络#1. Early Stop 正则化|Early Stop Regularization]], early stopping is a classic technique for regularization deep neural networks. 
and the most common way to determine  stop criteria is  to **monitor the validation error in training process.**

typically,  we check once validation error after each epoch. and cut off training when the validation error  has not decreased by more than some small amount $\varepsilon$ for  some number of epochs. This is called <mark style="background: transparent; color: red">patience criteria</mark>. 

[[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Chapter3 Linear Neural  Networks for regression(back Propagation)#(5) Norms and Weight decay|Weight Decay]] is also one of the classic regularization method.  (L1, L2 weight decay)

### (3) DropOut Regularization 
Another problem in network design  is  the symmetry inherit in the parametrization. If permute the weight of the first layer and the second layer obtain the same effect, then there is nothing special  differentiating the first hidden unit or second. (which means the permutation symmetry among the hidden units of each layer)

assume that we initialize all the parameter  with respect to $W^{(1)}$ as $W^{(1)} = c$ , where c is constant, then <mark style="background: transparent; color: red">the gradient of all the parameter gives the same value</mark>.  thus **==after gradient based iteration, the W still take same  value==**. so the iteration would never **break the  symmetry on its own.**  but  <b><mark style="background: transparent; color: orange">Drop Out regularization May Break the Symmetry In this case</mark></b>. 

classical generalization theory suggest to <mark style="background: transparent; color: red">close the gap between the  train and the performance </mark>, the  work drew a clear mathematical connection between the requirement that a function be smooth and  requirement that it be <mark style="background: transparent; color: red">resilient  to perturbations in the input</mark>. 

we may add the following layer in the network : 
```python
nn.Dropout(p=0.4)
```

the key challenge is how to<mark style="background: transparent; color: red"> inject the noise brought by co-Adaption </mark>.  

In the standard  dropout regularization, with dropout probability  $p$, each  intermediate activation h is replaced by  random  variable $h'$ as :
$$h' = \begin{cases}
0   \qquad  \text{with probablity  } p\\
\frac{h}{1-p}   \qquad  \text{otherwise}
\end{cases}$$
note that by design the  expectation not  changed, as : 
$$E [h'] =h $$
for example,  we can self-define a drop out layer module, like following format :  
```python
import numpy as np  
import torch  
from torch.optim import SGD  
from torch import nn

class MyDropOutLayer(nn.Module):  
    def __init__(self, p = 0.5):  
        super(MyDropOutLayer, self).__init__()  
        self.linear1 = nn.Linear(10, 5)  
        self.relu = nn.ReLU()  
        self.p = p  
        self.linear2 = nn.Linear(5, 2)   # simluate only 2 classes  
  
    def dropout_layer(self, X, p):  
        assert 0 <= p <= 1   # assert is to judge the expression must be true.  
        if p == 1:  
            return torch.zeros_like(X)    # if the dropout is 1, return 0  
        else :  
            # if rand > p, return 0, else return 1/(1-p) * X  
            mask = (torch.rand(X.shape) < p).float()   # if the dropout is not 1, return the mask  
            return mask * X / (1 - p)  
  
    def forward(self, X):  
        X = self.linear1(X)  
        X = self.relu(X)  
        X = self.dropout_layer(X, p=self.p)  
        X = self.linear2(X)  
        return X  
  
class self_Module(nn.Module):  
    def __init__(self):  
        super(self_Module, self).__init__() # call the parent class's constructor  
        self.forward_layer = MyDropOutLayer(p=0.5)  
  
    def forward(self, X):  
        X = self.forward_layer(X)   # call "forward" function  
        return X  
  
if __name__  == "__main__":  
    model = self_Module()  
    optimizer = SGD(params=model.parameters() ,lr=0.01, weight_decay=0.0001)  
    data = torch.rand(10, 10, dtype=torch.float32)  
    target = torch.tensor(torch.round(torch.rand(10)), dtype=torch.int64)  
    # use Long as the target type  
    criterion = nn.CrossEntropyLoss()   
# simulate the train process  
    for i in range(1000):  
        optimizer.zero_grad()  
        output = model(data) # call the forward function  
        loss = criterion(output, target)  
        loss.backward()  
        optimizer.step()  
        if (i+1) % 100 == 0:  
            print("Epoch: {}, Loss: {}".format(i+1, loss.item()))  
    print("predicted label", np.argmax(model(data).detach().numpy(), axis=1))  
    print("true label: ", target)
```

> [!HINT] 
> Although dropout can cause instability in the training process, it can help the model improve its generalization ability. By forcing the network to learn more robust feature representations, dropout can reduce the risk of overfitting, thereby improving the performance of the model on the test set.
> 
> In order to reduce the training instability caused by dropout, a common practice is to<mark style="background: transparent; color: red"> use the same dropout mode in each epoch</mark>, that is, to keep the position of the dropout neurons unchanged throughout the epoch. This ensures that the network sees the same "version" within the same epoch, making the training process more stable. In addition, the bias and variance of the model can be balanced by adjusting hyperparameters such as the dropout rate. 
> 
> Also note fixing the dropOut mask may lead to less random in epoch, which would introduce other bias, and also decrease the regularization of dropout.

several techniques for dropout layer are as follows: 
**Where to Add Dropout Layers:**
1. **After Linear (Fully Connected) Layers:**
   - Dropout is most commonly used after fully connected layers, as these layers tend to have a large number of parameters and are more prone to overfitting.
2. **After Convolutional Layers (Selective Use):**
   - Dropout can be applied after convolutional layers, but this is less common because convolutions involve spatial structure, and zeroing out activations might harm feature extraction. However, using small dropout rates here can sometimes be beneficial in deeper networks to improve generalization.
3. **After Pooling Layers:**
   - Dropout after pooling layers is less common but can still be effective in reducing overfitting in cases with small datasets.
4. **In the Last Layers of the Network:**
   - Adding dropout closer to the output layer, before the final classification, is a common practice to reduce overfitting while preserving feature-rich representations from earlier layers.

- **Common Values:**
  - **Fully Connected Layers:** Dropout rates of **0.2 to 0.5** are standard. A lower value (e.g., 0.2) is used for more complex models, while a higher value (e.g., 0.5) is used for simpler or overfitting-prone models.
  - **Convolutional Layers:** If applied, use smaller rates like **0.1 to 0.3**, as convolutional layers already benefit from weight sharing, which provides some regularization.

- **Adjust Based on Dataset Size:**
  - Larger datasets generally require smaller dropout rates because there is less risk of overfitting.
  - Smaller datasets may benefit from higher dropout rates to combat overfitting.

Also, Dynamic Dropout is optional, such as gradually reduce the dropout rate during training as the model converges, which can help maintain precision. 

##  3. Error Measure and K-Fold Cross Validation
### (1) Logarithmic MSE Loss 
Also note for the high house price with different level, we may consider use 
$$\frac{y - \hat{y}}{y}$$
than absolute error, also there may have significant outliners

we can  use a small value $\delta$  for $|\log_{} y  - \log_{} \hat{y}| \leq  \delta$ , which means $e^{- \delta} \leq \frac{\hat{y}}{y} \leq e^{\delta}$  
$$\sqrt{\frac{1}{n} \sum_{i = 1}^{n} (\log_{} y_{i}  - \log_{} \hat{y}_{i})^{2}}$$
this have good effect when $y > 0$ and have large discrepancy 

Note that in the house price,  we can integrate the Loss as a module of torch like the following class : 
```python
import torch
import torch.nn as nn

class LogErrorLoss(nn.Module):
    def __init__(self, epsilon=1e-8):
        super(LogErrorLoss, self).__init__()
        self.epsilon = epsilon

    def forward(self, y, y_hat):
        # Clamp values to avoid log(0)
        y = torch.clamp(y, min=self.epsilon)
        y_hat = torch.clamp(y_hat, min=self.epsilon)
        
        # Compute the logarithmic error
        log_diff = torch.log(y) - torch.log(y_hat)
        
        # Return the square root of the mean squared logarithmic error
        return torch.sqrt(torch.mean(log_diff ** 2))

```

### (2) K-fold Cross-Validation 
Firstly we need to slice out  $i^{th}$  segment of the  data in a K-fold cross-validation. We could do something much smarter  if the dataset is larger. 
the average  validation error returned when we train K times of K-fold cross-validation. and the basic process of K-fold cross validation is as follows : 
1. K-cross validation split the model to $k$ subset 
2. for each subset we use 1 fold as test set and other as train set  

this also have a good estimation of the <b><mark style="background: transparent; color: orange">generalization ability</mark></b> of model, which can be used with [[#(2) Early Stopping]] criterion for performance improvement. 

in the sklearn  module,  it has KFold can directly call as follows : 
```python
from sklearn.model_selection import KFold, cross_val_score 
kf = KFold(n_splits = 5, shuffle = True)

# execute the cross validation 
scores = cross_val_score(model, X, y, cv=kf) 
```

if in torch,  we can self define the KFoldCrossValidationTrainer Module : 
```python
import torch  
import torch.nn as nn  
from torch.utils.data import TensorDataset, DataLoader  
  
class KFoldCrossValidationTrainer:  
    def __init__(self, model, criterion, optimizer, k=5):  
        self.model = model  
        self.criterion = criterion  
        self.optimizer = optimizer  
        self.k = k  
  
    def forward(self, data, target):  
        n_samples = data.size(0)  
        k_fold_size = n_samples // self.k  
        train_losses = []  
        val_losses = []  
  
        for i in range(self.k):  
            # Split the dataset  
            val_start = i * k_fold_size  
            val_end = min((i + 1) * k_fold_size, n_samples)  
  
            x_val = data[val_start:val_end]  
            y_val = target[val_start:val_end]  
  
            x_train = torch.cat([data[:val_start], data[val_end:]], dim=0)  
            y_train = torch.cat([target[:val_start], target[val_end:]], dim=0)  
  
            # Training  
            self.model.train()  
            self.optimizer.zero_grad()  
            output = self.model(x_train)  
            train_loss = self.criterion(output, y_train)  
            train_loss.backward()  
            self.optimizer.step()  
            train_losses.append(train_loss.item())  
  
            # Validation  
            self.model.eval()  
            with torch.no_grad():  
                val_output = self.model(x_val)  
                val_loss = self.criterion(val_output, y_val)  
                val_losses.append(val_loss.item())  
  
        # Return average training and validation loss  
        avg_train_loss = sum(train_losses) / self.k  
        avg_val_loss = sum(val_losses) / self.k  
        return avg_train_loss, avg_val_loss
```

and in one roll of training, we can call the following process : 
```python
# Initialize model, criterion, and optimizer
model = SimpleModel()
criterion = nn.MSELoss()
optimizer = optim.SGD(model.parameters(), lr=0.01)
# Initialize K-Fold Trainer
kfold_trainer = KFoldCrossValidationTrainer(model, criterion, optimizer, k=5)
# Perform K-Fold Cross Validation
train_loss, val_loss = kfold_trainer.forward(data, target)
print(f"Average Training Loss: {train_loss:.4f}")
print(f"Average Validation Loss: {val_loss:.4f}")
```

Also, we can use the KFold function of sklearn in pytorch,  just use `kfold.split()`  for splitting the dataset. the thought of K-cross-validation is to train model K times in same network and finally get a more precise error. Thus ensure the <mark style="background: transparent; color: red">generalization ability of the network generalization ability of the specified network</mark>. 
```python
import torch 
from sklearn.model_selection import KFold 
from torch.utils.data import Dataset, DataLoader, Subset

k_folds = 5 
kfold = KFold(n_splits=k_folds, shuffle=True, random_state=42)
# initialize the criterion and optimizer  
criterion = nn.MSELoss() 
fold_train_losses = []
fold_val_losses = [] 
for fold, (train_idx, val_idx) in enumerate(kfold.split(dataset)):
    # create the dataset that needed 
    train_subset = Subset(dataset, train_idx)
    val_subset = Subset(dataset, val_idx)
    train_loader = DataLoader(train_subset, batch_size=16, shuffle=True)
    val_loader = DataLoader(val_subset, batch_size=16, shuffle=False) 
    
	model = SimpleModel() 
	optimizer = SGD(model.parameters(), lr=0.01)
	
	model.train()
	train_loss = 0.0
	for batch in train_loader:
		inputs, targets = batch
		optimizer.zero_grad()
		outputs = model(inputs)
		loss = criterion(outputs, targets)
		loss.backward()
		optimizer.step()
		train_loss += loss.item()
	train_loss /= len(train_loader)
    fold_train_losses.append(train_loss)

	model.eval()
    val_loss = 0.0
    with torch.no_grad():
        for batch in val_loader:
            inputs, targets = batch
            outputs = model(inputs)
            loss = criterion(outputs, targets)
            val_loss += loss.item()

    val_loss /= len(val_loader)
    fold_val_losses.append(val_loss) 
    print(f"FOLD {fold + 1}, Train Loss: {train_loss:.4f}, Validation Loss: {val_loss:.4f}")
```

### (3) Model Selection  
we assume that  we  pick  an untuned  set of the hyperparameters. With a large dataset, the normal sorts of  the hyperparameters, **K-fold cross validation tends to be reasonably resilient  against multiple testing**.  But note that **if we try unreasonable large number of options**, we would find our **validation performance is no longer representative of the true error**.  

## 4. Model Build and Self-defined Module
### (1) Introduction of Self-defined Module
this chapter focus on the model construction, parameter access and initialization. 
Firstly,  we have built the  linear models with single output or basic classification task. 
A single neuron take inputs, and also generate  corresponding scalar output; the associated  parameters can  be updated to optimize some objective function of interest.

for the network with multiple outputs, we leveraged the <mark style="background: transparent; color: red">vectorized arithmetic to characterize the entire layer of neurons</mark>.  when we worked through the softmax regression , a single layer was a model, but it's convenient to speak about the components that is larger than an individual layer but smaller than the entire model. (we use `nn.Module` layer) 

the ResNet-152 architecture implemented that the processed hundreds of layers so it's tedious if we only introduce 1 layer at a time. so we often <b><mark style="background: transparent; color: orange">arrange many layers in a module</mark></b> to train complex enough modules, or use `nn.Sequential` for many compact layers. 

```python
from torch import nn
net = nn.Sequential(
	nn.LazyLinear(256),
	nn.ReLU(),
	nn.LazyLinear(256)
)
```

we can custom  define a module,  and the basic  functionality of each module <b><mark style="background: transparent; color: orange">may provide the following functions</mark></b> :  
1. Ingest the input data  as argument to the forward ropagation method. 
2. Generate an output by forward propagation method retrn a value. (accessed by  `forward` method)
3. Calculate the gradient of the output respect to input,  which is defined by the backpropagation method. but in torch  
4. Store and  provide the access to the parameter of the layer,  which are necessary to execute the forward propagation method. 
5. Initialize the model parameter. 

### (2) Self-defined Sequence 
we often use functional part, see https://pytorch.org/docs/stable/nn.functional.html
```python
import torch.nn.functional as F
```

note <mark style="background: transparent; color: red">a key virtue of  the module is the abstraction is  the versatility.</mark>  the doc of module can be found  at https://pytorch.org/docs/stable/generated/torch.nn.Module.html#torch.nn.Module. 

for self-defined sequential  class, we can use  `add_module` method  of a basic module, and  use `self.children()`  :
```python
import torch  
from torch import nn  
  
class MySequential(nn.Module):  
    def __init__(self, *args):  
        super(MySequential, self).__init__()  
        for idx, module in enumerate(args):
           self.add_module(module)

	def forward(self, input):  
	    for module in self.children():  
	        input = module(input)  
	    return input

net = MySequential(
	nn.Linear(10, 5);
	nn.ReLU();
	nn.Linear(5, 100);
)
```

### (3) Parametric Layer
also, for example, <b><mark style="background: transparent; color: orange">if we want a layer with parameters, we can use </mark></b> `nn.Parameter` to make specified param a parameter of module. and use `layer.parameters` for access all the parameters of the layer. 

```python
import torch  
from torch import nn  
import torch.nn.functional as F  
  
class Layer1(nn.Module):  
    def __init__(self, input_num, output_num):  
        super(Layer1, self).__init__()  
        self.w = nn.Parameter(torch.rand(input_num, output_num))   # use nn.Parameter to make w a parameter  
        self.linear = nn.Linear(output_num, output_num)  
  
    def forward(self,X):  
        assert X.shape[-1] == self.w.shape[0], "Input dimension must match"  
        X = X @ self.w  
        X = self.linear(X)  
        X = F.relu(X)  
        X = self.linear(X)  
        return X  
	@torch.no_grad()  
	def extra_repr(self):  
	    pass 
	    
l1 = Layer1(10, 5)  
for param in l1.parameters():  
    print(param)  
  
input_test = torch.rand(10)  
print("output :", l1(input_test))
```

note in the Output, not only `self.w` but also linear layer would be printed. 

```python fold title:output
Parameter containing:
tensor([[5.7338e-01, 1.9158e-01, 9.3175e-01, 1.5499e-02, 2.4858e-01],
        [3.8877e-01, 2.8795e-01, 4.9835e-04, 6.1744e-01, 6.5033e-01],
        [2.1165e-01, 3.3831e-01, 7.5023e-01, 2.0996e-01, 1.9790e-01],
        [8.4896e-01, 1.5095e-01, 2.9802e-01, 5.7556e-01, 8.3422e-01],
        [8.0424e-01, 2.0123e-01, 4.5389e-01, 2.2344e-02, 9.5980e-01],
        [2.6045e-01, 9.3704e-01, 2.4308e-01, 2.9486e-01, 9.2768e-01],
        [1.9918e-02, 8.5719e-01, 8.3298e-01, 4.3065e-01, 9.1512e-01],
        [9.2998e-01, 1.7723e-01, 4.7651e-01, 6.9191e-01, 8.5374e-01],
        [4.5427e-01, 9.0022e-01, 8.8186e-02, 4.0458e-01, 1.8898e-02],
        [5.3395e-01, 5.4567e-01, 8.4909e-01, 8.4853e-01, 3.1498e-02]],
       requires_grad=True)   # self.w
Parameter containing:
tensor([[ 0.3482, -0.0466, -0.0054,  0.2345,  0.2712],
        [ 0.4353, -0.4004,  0.0870,  0.0835, -0.2961],
        [ 0.0252, -0.3695, -0.1715,  0.4114, -0.2974],
        [ 0.2081, -0.0064, -0.2362, -0.0545,  0.0734],
        [-0.0781, -0.0151,  0.0005,  0.3052, -0.4233]], requires_grad=True)  # linear.w 
Parameter containing:
tensor([ 0.1021,  0.0131,  0.4111,  0.0501, -0.3245], requires_grad=True)  # linear.b
tensor([ 0.9217,  1.0353,  0.4748,  0.5378, -0.5042], grad_fn=<ViewBackward0>)
```
### (4) Parameter Access and Initialization 
parameters are  complex objects, containing values, gradients,  and other information 

except `module.parameters`, we use named parameters for 
```python
for name, param in l1.named_parameters():  
    print(name, param)  # print the name and value of parameters

# w
# linear.weight
# linear.bias 
# we can also access the specified like layer1.linear.weight.data[0] 
```

note that we often define the parameter as `self.xxx` in a module. 

also we can <mark style="background: transparent; color: red">share the  parameters across multiple layers</mark>. we can just **use the same layer instance twice** thus they are same object, so the value are the same. but in this case, the gradient would be  added together during the back propagation. 

we can use `nn.init` for better parameter initialization, such as [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Chapter5 Essential Concepts & Heuristics#1. Xavier Initialization|Xavier Initialization]] : 
```python
def init_model(self):  
    nn.init.uniform_(self.w, -0.01, 0.01)  
    nn.init.normal_(self.b)  
    nn.init.xavier_normal(self.c)
	
	if isinstance(layer, nn.Linear):
		nn.init.xavier_uniform_(layer.weight)
```

<b><mark style="background: transparent; color: orange">Lazy initialization is used when we use the layer without specifying the input dimension</mark></b>. however we must run it once (call  "forward") for  initialize parameter. 

if we want to self-define a lazy module, **Use `torch.nn.UninitializedParameter`** , Declare parameters that are not initialized immediately as instances of `torch.nn.parameter.UninitializedParameter`. and replace the `UninitializedParameter` with a properly initialized `torch.nn.Parameter` once the shape is determined.

##  5. FASHION MINIST Practice
We give the example of the Optimization of the  Fashion MINIST dataset, both dropout,  regularization are used, and Adam Optimizer is used instead of SGD. 

```python
import os  
import torch  
import torch.nn as nn  
from torchvision import transforms  
from torch.utils.data import dataset, DataLoader  
import matplotlib.pyplot as plt  
from torchvision.datasets import FashionMNIST  
from sklearn.metrics import accuracy_score  
import torch.nn.functional as F  
  
device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')  
  
trans = transforms.Compose([  
    transforms.ToTensor(),  
    transforms.Resize((28, 28)),  
])  
  
os.chdir(os.path.dirname(os.path.abspath(__file__)))  
  
class MNIST_Module(nn.Module):  
    def __init__(self, batch_size=250):  
        super(MNIST_Module, self).__init__()  
        self.train = FashionMNIST(root='./data', train=True, download=True, transform=trans)  
        self.test = FashionMNIST(root='./data', train=False, download=True, transform=trans)  
        self.train_dataloader = DataLoader(dataset=self.train, batch_size=batch_size, shuffle=True)  
        self.test_dataloader = DataLoader(dataset=self.test, batch_size=batch_size, shuffle=True)  
        self.net = nn.Sequential(  
            # First Convolutional Block  
            nn.Conv2d(1, 16, kernel_size=3, stride=1, padding=1, bias=True),  # 28x28 -> 16 * 28x2  
            nn.LeakyReLU(),  # Retain minor activations for subtle features  
            nn.Conv2d(16, 64, kernel_size=3, stride=1 , padding=1, bias=True),  
            nn.LeakyReLU(), 
            # add ReLU here is very good for the feature detection of images, Leaky ReLU may retain some key features  
            # Strided Convolution for Downsampling, Replace Pooling with Strided Convolution for less feature loss            
            nn.Conv2d(64,128, kernel_size=3, stride=1, padding=1, bias=True),  
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
        self.to(device)  # deploy the model into gpu  
  
    def forward(self, x):  
        # x = x.view(-1, 28*28) # reshape to 1-dim tensor  
        return self.net(x)  
  
    def train_loader(self):  
        X, y = next(iter(self.train_dataloader))  
        return X.to(device), y.to(device)  
  
    def test_loader(self):  
        X, y = next(iter(self.test_dataloader))  
        return X.to(device), y.to(device)  
  
    def loss(self, y_pred, y_true):  
        return F.cross_entropy(y_pred, y_true)  
  
    # if the indices is listed as a list, it will return a list of labels  
    def get_labels(self, indices):  
        self.labels = ['t-shirt', 'trouser', 'pullover', 'dress', 'coat', 'sandal', 'shirt', 'sneaker', 'bag',  
                       'ankle boot']  
        return [self.labels[int(i)] for i in indices]  
  
    def get_precision(self, dataloader):  
        # self.eval()  # set the model to the evaluation mode  
        correct = 0  
        total = 0  
        with torch.no_grad():  
            # not use train_loader here for traveling the whole data set  
            for data, target in dataloader:  
                data, target = data.to(device), target.to(device)  
                outputs = self(data)  
                _, predicted = torch.max(outputs.data, 1)  
                total += target.size(0)  
                correct += (predicted == target).sum().item()  
        return 100 * correct / total  
  
    def show_data_imgs(self, axes, fig_data, labels, n_nums=10):  
        fig_data = fig_data.to('cpu')  # move the data to cpu for showing  
        labels = labels.to('cpu')  
        print("show image data of shape: ", fig_data.shape)  
        fig_labels = self.get_labels(labels.numpy())  
        num = 0  
        for ax, fig, label in zip(axes, fig_data, fig_labels):  
            num += 1  
            ax.imshow(fig.view([28, 28]).numpy())  
            ax.set_title(label)  
            ax.axis('off')  
            if (num == n_nums):  
                break        plt.tight_layout()  
  
if __name__ == "__main__":  
    model = MNIST_Module(batch_size=300)  
    num_epoch = 1000  
    # weight_decay shouldn't be too large.  
  
    # RMSprop and Adams can dynamically change the learning rate during training (use instead of SGD)    optim = torch.optim.Adam(model.parameters(), lr=0.001,  weight_decay=0.0001)  
  
    # use weight decay for more precision  
    criterion = nn.CrossEntropyLoss()  
  
    # train model  
    # model.train()    for epoch in range(num_epoch):  
        X, y = model.train_loader()  
        optim.zero_grad()  
        y_pred = model(X)  
        loss = criterion(y_pred, y)  
        loss.backward()  
        optim.step()  
  
        if epoch % 100 == 0:  
            print(f'Epoch: {epoch}, Loss: {loss.item()}')  
        torch.cuda.empty_cache()  # clear the GPU cache to avoid broken  
  
    # model.eval()    score_train = model.get_precision(model.train_dataloader)  
    print(f'Accuracy in train set: {score_train}%')  
    score_test = model.get_precision(model.test_dataloader)  
    print(f'Accuracy in test set: {score_test}%')  
  
    data, target = model.train_loader()  
    output = model(data)  
    # show the last image groups  
    n_nums = 10  
    figure, axes = plt.subplots(2, n_nums, figsize=(35, 35))  
    axes_group1 = axes[0]  
    axes_group2 = axes[1]  
    model.show_data_imgs(axes_group1, data, target, n_nums)  
    model.show_data_imgs(axes_group2, data, torch.argmax(output, dim=1), n_nums)  
    plt.show()  
  
    torch.save(model.state_dict(), './fashion_minist_model.pth')
```

with several optimization technique and more complex model,  the train set precision reach 94 %  and test precision  finally reach 91.75%.
![[attachments/2024-12-22 232229.png|400]]
also ResNet and etc. are recommended for better precision (95% or higher) 



