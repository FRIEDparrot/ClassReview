## 1. Linear Regression 

> [!NOTE]Torch Linear Regression
> for the linear Regression in pytorch, refer to [torch documentation](https://pytorch.org/docs/stable/generated/torch.nn.Linear.html#torch.nn.Linear)

In the linear  Regression Problem, we  will use $n$ to denote the number of  examples, where $x^{(i)}$ denotes the $i$-th example, and $x_{j}$ denotes $j-th$  coordinate. 
the linear regression assign  each an index and express  the prediction $y$ 
$$\hat{y} = \omega_1 x_1 + \omega_2 x_2 + .....+ \omega_n x_n$$
which can be written to:
$$\hat{y}= \omega^Tx+b$$
the prediction $\hat{y} \in R$ can be expressed  via matrix vector product. we expected to find a real-world datasheet of $n$ examples. The instruments  were used to observe <mark style="background: transparent; color: red"> features</mark> $X$ and <mark style="background: transparent; color: red">labels</mark> $Y$ may might  have  small measurement error. The squared error is  given by: 

- The **Squared Error** (SE) is  defined  as : 
$$l^{(i)}(w,b) = \frac{1}{2}(\hat{y}^{(i)} -y^{(i)})^2$$
note this error **encourages the model to avoid large error,ti also lead to excessive sensitivity to anomalous data**. 
to measure the quality of data, we use the average of loss: 
$$\boxed {L = \frac{1}{n} \sum^{n}_{i=1} l^{(i)}(w, b) = \frac{1}{n} \sum^{n}_{i=1} \frac{1}{2} \left( w^{T} x^{(i)} + b - y^{(i)}\right)^{2}}$$
where $w, b$ are to minimize the loss, that is:
$$(\omega^*, b^*) = \text{argmin}_{w,b}L(w, b)$$
note analytical solution is $[w, b] =  (X^{T} X)^{-1} X^{T} y$ (refer to [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/Dive into Deep Learning(Pytorch)/Derivations/Derivation of Linear Regression.pdf|Derivation of Linear Regression.pdf]]) 

> [!caution] Full-Rank Condition
> the analytical solution force the $(X^T X)$ must be a full-rank matrix.
> if the column vector X has full rank $n$, by $rank(AB) = rank(A) rank(B)$ we prove that matrix above is full rank. 
> note the <mark style="background: transparent; color: red">features must be linear independent</mark> for X, which provide full usage of all features. 

## 2. Minibatch Stochastic gradient descent(SGD)
### (1) Basic method
- gradient descent: The technique for optimizing the deeplearning model is to <mark style="background: transparent; color: red">reduce the error by updating  the parameters in the direction that incrementally lowers the loss function</mark>.  

The derivate method of average of small batch can be slow. But the SGD method. note processing one sample at a time is much longer than a full batch(size at power of 2).

for minibatch  $B_{t}$, which consists of a **fixed number of $|B|$ of training examples**. and set learning rate is $\eta$, then each time we renew the $w$ and $b$ as follows:
$$(w, b) \overset{}{\longrightarrow}  (w,b) - \frac{|\eta|}{|B|} \sum_{i \in B_{t}} \partial_{w,b}  l^{(i)}   (w, b)$$
where 
$$l^{(i)} =  \frac{1}{2} \left( w^{T} x^{(i)} + b - y^{(i)}\right)^{2} $$
and $\partial_{w} = x, \partial_{b} = 1$, hence for SGD function :
$$\Large \begin{cases}
w = w -  \frac{|\eta|}{|B|}\sum^{n}_{i \in B_{t}} x^{(i)} (w^{T}x^{(i)} + b - y^{(i)})^{2} \\
b = b - \frac{|\eta|}{|B|}\sum^{n}_{i \in B_{t}} (w^{T}x^{(i)} + b - y^{(i)})^{2}
\end{cases}$$
> [!caution] Normalization of Data
> Torch didn't provide a function for normalize vector. so we need to write the normalize function  
```python
def normalize(x:torch.Tensor):  
    return (x - x.mean(dim=0))/x.std(dim=0)
```

the following code gives a brief example for building the model of Boston house Price. 

```python fold title:Boston_House_Price_Linear_Regression
import deeplearning_util_functions  
import pandas as pd  
import torch  
from torch.utils.data import DataLoader, TensorDataset  
  
def LossFunc(y_pred:torch.Tensor, y_true:torch.Tensor):  
    return 0.5 * torch.sum((y_pred - y_true)**2)  
  
def Prediction(w:torch.Tensor, x:torch.Tensor, b:torch.Tensor):  
    return torch.mm(x, w) +  b  
  
def normalize(x:torch.Tensor):  
    return (x - x.mean(dim=0))/x.std(dim=0)  
# Boston housing problem  
data = pd.read_csv("../datasheets/housing.csv")  
data_tensor = torch.tensor(data.values, dtype=torch.float32)  
  
features = data_tensor[:, 0:3]  
targets  = data_tensor[:,3].unsqueeze(1)   # add a dimension to it  
n = features.shape[1]  
  
# ----------- parameters for create the linear regression minibatch stochastic gradient descent --------------  
eta = 0.01    # learning rate  
w = torch.ones(n).unsqueeze(1)     # column vector  
b = torch.tensor(0,dtype=torch.float32)                         # initial value  
num_epoch_max = 90                                                   # maximum number of epoch  
# ---------------------------------------------------  
  
X = normalize(features)  
Y = normalize(targets)  
  
DataLoader1 = DataLoader(torch.cat((X,Y), 1),batch_size=5,shuffle=True)  
  
train_x = torch.tensor([])  
train_y = torch.tensor([])  
  
epoch = 0  
for minibatch in DataLoader1:  
    epoch = epoch +1  
    sample_x = minibatch[:, 0:3]  
    sample_y = minibatch[:,3].unsqueeze(1)  
  
    train_x = torch.cat((train_x, sample_x), dim=0)  
    train_y = torch.cat((train_y, sample_y), dim=0)  # 2 dim tensor  
  
    w.requires_grad_(True);  # ask for the space for gradient.  
    b.requires_grad_(True);  #  
    try:  
        w.grad.zero_()  
        b.grad.zero_()  
    except:        pass    # record the following calculation of w and b  
  
    y_pred = Prediction(w, sample_x,b)  
    y_true = sample_y  
    # calculate the loss by loss function  
    Loss = LossFunc(y_pred, y_true)  
    Loss.backward()      # derive the differentation of Loss function.  
  
    # use item to get the number    print("epoch: ", epoch, "Loss:", Loss.item())  
    if Loss.item() < 0.1 or epoch > num_epoch_max:  
        break    else:        # when the variables are changed must use torch,no_grad to not record the result.  
        with torch.no_grad():  
            w = w - eta * w.grad    # refresh the w (note!!!! if the gradient didn't cleared, it would be none)  
            b = b - eta * b.grad    # refresh the b  
        # clear the grad after derive it  
print("final loss: ",Loss, ".\n", "w:", w.detach().numpy(), ".\n b:", b.detach().numpy())
```

> [!caution] Autograd and Torch.no_grad()
> note **if the variables need to be refreshed, but gradient retains, the refresh process would be recorded. but if variable changed, the grad would be None type**. hence we need to use `torch.no_grad` for change the variable in the code above.

> [!check] Tips
> for the houses prices and the  stock price,  the <mark style="background: transparent; color: red">log regression may be better</mark>. 
> distributions over counts (Pisson Distribution, etc.) can describe the discrete number distribution of the target.

Linear Regression is a single-layer fully connected neural network. 
```python title:code_for_generate_the_linear_samples
def generate_sample(W:torch.Tensor, b:torch.Tensor, noise_amp = 0.01, sample_number = 100) -> tuple[torch.Tensor, torch.Tensor]:  
    num_inputs = torch.tensor(W.shape)[0]  # 3 input numbers  
    num_outputs = torch.tensor(b.shape)  
    X = torch.rand((sample_number, num_inputs), requires_grad=False)  
    y = torch.mm(X, W) + b + 0.01 * torch.rand((sample_number, 1))  
    return X, y
```
### (2) Normal Distribution 
if we assume that the observations arise from noisy measurements, the noise is $\varepsilon \sim \mathcal{N} (0, \sigma^{2})$,thus  the likelihood of each $y$ is 
$$P(y | x) = \prod^{n}_{i=1} p (y^{(i)} | x^{(i)}) = \prod^{n}_{i=1}\frac{1}{\sqrt{2 \pi  \sigma^{2}} } \exp \left(- \frac{1}{2\sigma^{2}}( y^{(i)} - w^{T} x^{(i)}  - b)^{2}\right) $$
since all pairs are independent, <mark style="background: transparent; color: red">the principle  of  maximum likelihood</mark> are  called <mark style="background: transparent; color: red">maximum likelihood estimators</mark>, since $p(y|x)$ is hard to calculate maximum, this can be reached by minimize the minus logarithm. then :
$$- \log  P(y| \boldsymbol{x}) =  -\frac{1}{2} \log (2\pi \sigma^{2})  + \frac{1}{2 \sigma^{2}}(y^{(i)} - w^{T}x^{(i)} - b)^{2}$$
since $\sigma$ is fixed, for minimize $\frac{1}{2 \sigma^{2}}(y^{(i)} - w^{T}x^{(i)} - b)^{2}$, we return to the problem of
$$X^{T }Y = X^{T} X w$$

> [!summary] conclusion
> minimize the mean squared error is equivalent to maximum the  likelihood estimation of the linear model.

> [!note] MAE Loss function 
> For MAE loss function to find 
> $$\min \sum^{n}_{i=1} |x_{i} - b|$$
> the solution of best $b$ is median of all the $x$ (can be proved by mathatical induction) 
> caution that $|x_i - b|$ has no derivate when $x_i = b$, hence this can't be derivated. 

### (3) Sigmoid function
The Sigmoid Function is defined as:
$$\text{Sigmoid}(x) =  \frac{1}{1 + e^{-x}}$$
```python
import torch  
from deeplearning_util_functions import plot_figure  
# from torch.distributions.multinomial import Multinomial  
from matplotlib import pyplot as plt
def sigmoid(x):  
    return 1/(1 + np.exp(-x))
# print(Multinomial(100, torch.tensor([0.5, 0.5])).sample())  
X = torch.arange(-7, 7, 0.01)  
Y = torch.sigmoid(X)  
plot_figure(X, Y)  
plt.show()
```
which is as follows:
![[attachments/Pasted image 20240609090321.png]]

## 3. Base class of neural network in pyTorch
### (1) Objective Programming in torch
The base class of the neural network in pytorch is `nn.Module` 
[Introduction of torch.nn reference](https://blog.csdn.net/weixin_52067875/article/details/125921057) 

```python
import torch.nn as nn
nn.Module
```

The nn [container](https://pytorch.org/docs/stable/nn.html#containers) includes:
```python
Module 
Sequential 
ModuleList
ModuleDict
ParameterList
ParameterDict
```

for Load small batch data, use `from torch.utils.data import DataLoader`, 
generate random number to initialize the weight vector:
```python
self.w = torch.normal(mu_,sigma_,size=(1, num_inputs), requires_grad=True)
```
if we need to use MSE loss function,just call: 
```python
loss = nn.MSELoss()
input = torch.randn(3, 5, requires_grad=True)
target = torch.randn(3, 5)
output = loss(input, target)
output.backward()
```
if we need for train a Linear Regression Module in pytorch, use `nn.Linear` for building linear module(where weights and bias would be initialized randomly). 
```python
from torch.optim import SGD 
criterion = nn.MSELoss()
```

also note <mark style="background: transparent; color: red">if we want to load the train samples and test samples at the same time by "DataLoader", we must use Type "TensorDatasheet"</mark>, it will make a datasheet contain different variables. 

also <mark style="background: transparent; color: red">torch.data.randomsplit()</mark> will do the work to split the dataset into train data and test data. 

the following code shows the process of linear regression: 
```python fold title:small_batch_linear_regression_by_torch
import matplotlib.pyplot as plt  
import torch  
from torch import nn  
from torch.optim import SGD  
from torch.utils.data import DataLoader, TensorDataset, random_split  
from deeplearning_util_functions import plot_figure  
  
# w must be column vector  
def generate_sample(W:torch.Tensor, b:torch.Tensor, noise_amp = 0.01, sample_number = 100) -> tuple[torch.Tensor, torch.Tensor]:  
    num_inputs = torch.tensor(W.shape)[0]  # 3 input numbers  
    num_outputs = torch.tensor(b.shape)  
    X = torch.rand((sample_number, num_inputs), requires_grad=False)  
    y = torch.mm(X, W) + b + 0.01 * torch.rand((sample_number, 1))  
    return X, y  
  
class LinearRegression:  
    def __init__(self, X:torch.Tensor , y:torch.Tensor, mu_:float=0, sigma_:float = 0.01, learning_rate:float = 0.01):  
        num_inputs = int(torch.tensor(X.shape)[1])  
        num_outputs = int(torch.tensor(1))  
        # print(num_inputs, num_outputs)  
        self.X = X  
        self.y = y  
        # in nn.Linear, w and b will be initialize automatically  
        self.model = nn.Linear(num_inputs, 1)   # specify input and output argument numbers  
        self.criterion = nn.MSELoss()                       # loss criterion  
        self.optimizer = SGD(self.model.parameters(),lr=learning_rate)  # use SGD module  
        # note: **the parameters of the model was transfer to the optimizer here**, and later we use optimizer.step() for optimization 
  
    # data_sheet include X and y data 
    def calculate(self, batch_size=10,  max_epoch_num: int = 500, target_loss=0.1):
        epoch = 0  
        data_sheet = TensorDataset(self.X, self.y)  
        data_loader = DataLoader(data_sheet, batch_size=batch_size, shuffle=True)  
        for X_batch, y_batch in data_loader:  
            epoch += 1  
            outputs = self.model(X_batch)  
            loss = self.criterion(outputs, y_batch)  
            self.optimizer.zero_grad()  
            loss.backward()  
            self.optimizer.step()   # backward  propagation and optimization  
            print("epoch:", epoch, "loss:", loss.item())  
            if loss < target_loss or epoch > max_epoch_num:  
                break    def test_accuracy(self, X_test, y_test):  
        y_pred = self.model(X_test)  
        loss = self.criterion(y_pred, y_test)  
        print("loss result:", loss)  
        plot_figure(y_test.detach().mT.numpy(), y_pred.detach().mT.numpy(), fmts= '.')  
        plt.show()

if __name__ == "__main__":  
    train_size = 7000; test_size = 3000  
    W = torch.tensor([1, 2, 3], dtype=torch.float32).unsqueeze(1)  
    b = torch.tensor(5, dtype=torch.float32)  
    X, y = generate_sample(W, b, 0.01,  sample_number= train_size+test_size)  
  
    # split  
    X_train = X[0:train_size]; X_test = X[train_size:]  
    y_train = y[0:train_size]; y_test = y[train_size:]  
  
    A = LinearRegression(X, y)  
    A.calculate(target_loss=0.01)  
    A.test_accuracy(X_test, y_test)
```

> [!summary] The Basics in the deep learning model building process
> 1. Data Loader (include  random split and DataLoader iteration)
> 2. model (nn.Linear) 
> 3. optimization procedure (torch.optim.SGD) 
> 4. iteration train loop 
> 5. visualization process 
> the goal is discover general patterns.  

> [!cite] Overfitting and Regularization
> The phenomenon of fitting  closer to the training data but failed to fit test data is called overfitting.  and the technique for combatting overfitting is called regularization. 

### (2) Lazy Functions
Also we introduce the <mark style="background: transparent; color: red">lazy modules</mark>(see [lazyModuleMixin](https://pytorch.org/docs/stable/generated/torch.nn.modules.lazy.LazyModuleMixin.html#torch.nn.modules.lazy.LazyModuleMixin)) such as `net = nn.LazyLinear(1)` has no  initialize parameters. and we can use `net.weight.data.normal_(0,0.01)` and `net.bias.data.fill_(0)` to specify the parameter initialized. 
```python
net = nn.LazyLinear()  
net.weight.data.normal_(0,0.01)  
net.bias.data.fill_(0)
```

nn.Lazy provides the following Lazy functions:
![[attachments/Pasted image 20240615093517.png]]
`torch.nn.ReLU` gives the **ReLU activate function**. Hence we must <mark style="background: transparent; color: red">perform a dry run before specify optimizer</mark> and `initialize_parameters()` to initialize the network's lazy modules(may take no effect?).

the example code for build a 2-layer module (10 hidden unit, 1 output) (<mark style="background: transparent; color: red">must super the torch.nn.Module to speciy it's a network Module class:</mark>) 
```python
class LazyMLP(torch.nn.Module):
   def __init__(self):
       super().__init__()
       self.fc1 = torch.nn.LazyLinear(10)
       self.relu1 = torch.nn.ReLU()
       self.fc2 = torch.nn.LazyLinear(1)
       self.relu2 = torch.nn.ReLU()
   def forward(self, input):
       x = self.relu1(self.fc1(input))
       y = self.relu2(self.fc2(x))
       return y
# constructs a network with lazy modules
lazy_mlp = LazyMLP()
# transforms the network's device and dtype
# NOTE: these transforms can and should be applied after construction and before any 'dry runs'
lazy_mlp = lazy_mlp.cuda().double()    # torch.Module  class  
lazy_mlp
# performs a dry run to initialize the network's lazy modules
lazy_mlp(torch.ones(10,10).cuda())
# after initialization, LazyLinear modules become regular Linear modules
lazy_mlp
# attaches an optimizer, since parameters can now be used as usual
optim = torch.optim.SGD(mlp.parameters(), lr=0.01)
lazy_mlp = LazyMLP()
# The state dict shows the uninitialized parameters
print(lazy_mlp.state_dict())
```

using `pip install gymnasium` api, it provides the following environments:
![[attachments/Pasted image 20240615102718.png]]
for CartPole-v1 task, see [this link](https://pytorch.org/tutorials/intermediate/reinforcement_q_learning.html)

### (3) Train Error and  Generalization  Error
Training error $R_{emp}$ is the error is the error between model prediction and train dataset. 
$$R_{emp}[X, y, f] = \frac{1}{n}  \sum^{n}_{i=1}  l (\boldsymbol{x}^{(i)}, y^{(i)},  f(\boldsymbol{x})^{(i)})$$
The <mark style="background: transparent; color: red">generalized error</mark> is  expressed as  the integral:
$$ \mathcal{R}[p, f] = E_{(x,y)\sim P}[l(\boldsymbol{x}, y, f(\boldsymbol{x} ))]  = \int \int l(\boldsymbol{x}, y, f(\boldsymbol{x} )) d\boldsymbol{x} dy$$
not the distribution could vary from P and density function $p(\boldsymbol{x},y)$ is not known.
for generalization error, this is the result that applied to the  independent <mark style="background: transparent; color: red">test set</mark>. we expect that the training error is close to the population error. 

note that when we work with more  complex models and few examples, we expext the training error decrease and  generalization error grow. if <mark style="background: transparent; color: red">model complexity</mark>  is enough, and arbitary data can be fittted. 

one notion of the complexbility is controlled by  the tange of values that parameters can take. Also,when introduce <mark style="background: transparent; color: red">weight decay (a practical regularization technique)</mark>. 
when  the model is too simple, the  model is unable to  reduce train error, called **underfitting** and when training error is  significantly lower than validation error, called **overfitting**. 
![[Excalidraw/Chapter3 Linear Neural  Networks for regression 2024-06-15 13.21.29|300]]
### (4) Cross Validation 
One Popular solution for the overfitting of the model is  cross validation method.   which  is <mark style="background: transparent; color: red">K-fold cross validation</mark>. which means validation execute K times and   train on $K-1$ subsets and  validating  on subset not used in training. 

more data oftenly leads to better generalization. 

### (5) Norms and Weight decay
for the polynomial regression problem, given the training data consisting a single feature $\boldsymbol{x}$ and a corresponding real-valued label $y$.

$$\hat{y} =  \sum^{d}_{i=0} x^{i} w_{i}$$
Since we can always  mitigate overfitting by collecting more data, we can limit the model's capacity by  tweaking the dregree of polynomial. 

The natural  extensions of polynomials  to multivariate data are <mark style="background: transparent; color: red">monomials</mark>($x_1x_2^2$). since the term $d$ with degree d blows up rapidly as it grows larger (which $k$ multichoose in $d$ have $\left( \begin{matrix}   k - 1 + d\\ k -1  \end{matrix}\right)$ )

weight decay may be <mark style="background: transparent; color: red">the most widely used  technique  for regularizing parametric machine leaning models</mark>. 
**We can measure the complexity of the model by the distance between a function and zero. Which the functional analysis and theory of  Banach spaces gives the answer**. note simply tossing aside features can reduce overfitting, but not  a wise choose. 

for $f(\boldsymbol{x}) = w^{T} \boldsymbol{x}$, the distance of it can be measured by the weight vector $||w||^{2}$,  which is $l_2$ norm (see [[📘ClassNotes/📐Mathmatics/📈Advanced Mathematics/补充部分/n维空间中的距离向量定义|distance in n-dimension space]]).  

we restrict the size of $||w||$, the loss is given by:
$$L(w, b) =  \frac{1}{n}  \sum^{n}_{i=1} \frac{1}{2} \left( w^{T} \boldsymbol{x}^{(i)}  + b - y^{(i)}\right)^{2}$$
To penalize the size of weight vector,  we <mark style="background: transparent; color: red">add ||w||^2  to this loss function</mark>, and **the model trade off the standard loss for additive penalty by the regularization constant $\lambda$** (where $\lambda$ is the regularization constant <b><mark style="background: transparent; color: blue">( 'lambda' here is `weight_decay` or `wd` parameter  in SGD parameters))</mark></b>:  
$$\boxed{L(w,b)  +  \frac{\lambda}{2} ||w||^{2}}$$
refer and compare it to [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/深度学习算法原理(sklearn)/补充知识/5.SVM支持向量机|supported vector machine]], the goal is **low model complexity + low loss**, which similar to the priciple of **minimize both loss and the plane distance in SVM**. if $\lambda >0$, we restrict the size of $\boldsymbol{w}$.  (the reason to use square is easy to compute the derivate)  

also, $l_2$-regularized linear model constitute the **classic ridge  regression algorithm**. ($l_1$-regularized or sum of abs values is **lasso regression**), and $l_2$ method places an outsize panalty on large components of the  weight vector.(more robust to measurement error in a single varibale)

the minibatch **stochastic gradient descent(SGD)** updates for $l_2$ regularized  regression as:
$$w \quad \overset{\text{iteration}}{\longrightarrow}\quad  (1-  n \lambda ) w - \frac{\eta}{|\mathcal{B}|}  \boldsymbol{x}^{(i)}   (w^{T} x^{(i)} + b - y^{(i)})$$
for use weight decay, we must **redefine the loss function in torch. firstly we inhert the nn.Module class**. the 

for **ReLU function and so on, we can use**:
```python
import torch.nn.functional as F 
```

and since **we need to modify the criterion, we refer to the source code of MSELoss, and the core is define the forward function**: 
```python
def forward(self, input: Tensor, target: Tensor) -> Tensor: 
	return F.mse_loss(input, target, reduction=self.reduction)
```
hence we can self-define the Loss criterion as follows:
```python
class Weightdecay_MSELoss(nn.Module):  
    def __init__(self):  
        super().__init__()  
    # define the forward function (special function) for change the criterion  (input must be Linear function)  
    def forward(self, input:torch.Tensor, target:torch.Tensor, model:nn.Linear, lambda_:float = 1) -> torch.Tensor:  
        mse_loss = F.mse_loss(input, target)  
        l2_penalty = 1/2 * torch.sum(model.weight ** 2)  
        return mse_loss + l2_penalty
```

> [!NOTE] forward Method
> for the `nn.Module` class, the method "**forward**" is a special method, which defines the forward spread logical process 

hence, the entire code is listed as follows: 
in this function we use `TensorDatasheet` for create datasheet, and use the `DataLoader` to generate data from it. 

```python
import matplotlib
import matplotlib.pyplot as plt  
import torch  
import numpy as np  
import torch.nn as nn  
import torch.nn.functional as F  
from torch.utils.data import TensorDataset, DataLoader  
from torch.optim import SGD

class LinearRegression:  
    def __init__(self, X:torch.Tensor , y:torch.Tensor, mu_:float=0, sigma_:float = 0.01, learning_rate:float = 0.01):  
        num_inputs = int(torch.tensor(X.shape)[1])  
        num_outputs = int(torch.tensor(1))  
        # print(num_inputs, num_outputs)  
        self.X = X  
        self.y = y  
        # in nn.Linear, w and b will be initialize automatically  
        self.model = nn.Linear(num_inputs, 1)   # specify input and output argument numbers  
        self.criterion = nn.MSELoss()                       # loss criterion  
        self.optimizer = SGD(self.model.parameters(),lr=learning_rate)  # use SGD module  
  
    # data_sheet include X and y data    def calculate(self, batch_size=10,  max_epoch_num: int = 500, target_loss=0.1):  
        epoch = 0  
        data_sheet = TensorDataset(self.X, self.y)  
        data_loader = DataLoader(data_sheet, batch_size=batch_size, shuffle=True)  
        for X_batch, y_batch in data_loader:  
            epoch += 1  
            outputs = self.model(X_batch)  
            loss = self.criterion(outputs, y_batch)  
            self.optimizer.zero_grad()  
            loss.backward()  
            self.optimizer.step()   # backward  propagation and optimization  
            print("epoch:", epoch, "loss:", loss.item())  
            if loss < target_loss or epoch > max_epoch_num:  
                break    def test_accuracy(self, X_test, y_test):  
        y_pred = self.model(X_test)  
        loss = self.criterion(y_pred, y_test)  
        print("loss result:", loss)  
        plot_figure(y_test.detach().mT.numpy(), y_pred.detach().mT.numpy(), fmts= '.')  
        plt.show()  
  
    def get_param(self):  
        print(self.model.state_dict())  
  
def generate_sample(W:torch.Tensor, b:torch.Tensor, noise_amp = 0.01, sample_number = 100) -> tuple[torch.Tensor, torch.Tensor]:  
    num_inputs = torch.tensor(W.shape)[0]  # 3 input numbers  
    num_outputs = torch.tensor(b.shape)  
    X = torch.rand((sample_number, num_inputs), requires_grad=False)  
    y = torch.mm(X, W) + b + 0.01 * torch.rand((sample_number, 1))  
    return X, y  
  
def set_axis(axes:matplotlib.axes.Axes, xlabel:str, ylabel:str,  title:str, xlim:list, ylim:list, xscale, yscale, legend:list):  
    axes.set_xlabel(xlabel);    axes.set_ylabel(ylabel)  
    axes.set_title(title);  
    plt.xlim(xlim)  
    if (legend):  
        axes.legend(legend)  
    axes.grid()  
  
def plot_figure(X:torch.Tensor, Y:torch.Tensor, title:str = None , xlabel=None,fmts = ('-', 'm--'), ylabel=None, xlim = None,  
                ylim = None, xscale = None, yscale = None,  
                legend = None, axes:matplotlib.axes.Axes=None):  
    # test if the input is a 1-dimension data  
    def Is1Axis(X):  
        if (hasattr(X, "ndim") and X.ndim == 1) or isinstance(X, list) and not hasattr(X[0], "__len__"):  
            return True    if Is1Axis(X): X = [X]  
    if Y is None: Y = [[]] * len(X)  
    if Is1Axis(Y): Y = [Y]  
    if axes is None:  
        fig, axes = plt.subplots()  
  
    axes.cla()  # clear the  
    for x,y, fmt in zip(X,Y,fmts):  
        if len(x) != 0:  
            axes.plot(x,y, fmt)  
        else:            axes.plot(y,fmt)  
    set_axis(axes, xlabel, ylabel,title,xlim, ylim,xscale, yscale,legend)  
  
class Weightdecay_MSELoss(nn.Module):  
    def __init__(self):  
        super().__init__()  
    # define the forward function (special function) for change the criterion  (input must be Linear function)  
    def forward(self, input:torch.Tensor, target:torch.Tensor, model:nn.Linear, lambda_:float = 1) -> torch.Tensor:  
        mse_loss = F.mse_loss(input, target)  
        l2_penalty = 1/2 * torch.sum(model.weight ** 2)  
        return mse_loss + l2_penalty  

class Weightdecay_Linear_regression(LinearRegression):  
    def __init__(self, X:torch.Tensor , y:torch.Tensor, learning_rate:float = 0.01):  
        super().__init__(X,y,learning_rate = learning_rate)  
        self.validation = False  
        self.criterion = Weightdecay_MSELoss()  
  
    def split_sapmle(self,train_size) -> None:  
        X = self.X  
        y = self.y  
        self.X = X[0:train_size]  
        self.X_test = X[train_size:]  
        self.y = y[0:train_size]  
        self.y_test = y[train_size:]  
        self.validation = True  
        self.loss_arr = np.array([])  
        self.loss_val_arr = np.array([])  
  
    def calculate(self, batch_size=10,  max_epoch_num: int = 500, target_loss=0.01):  
    # redefine the calculate function and use the new criterion  
        epoch = 0  
        data_sheet = TensorDataset(self.X, self.y)  
        data_loader = DataLoader(data_sheet, batch_size=batch_size, shuffle=True)  
        for X_batch, y_batch in data_loader:  
            epoch += 1  
            outputs = self.model(X_batch)  
            loss = self.criterion(y_batch,outputs, self.model,0)  
  
            self.loss_arr = np.append(self.loss_arr, loss.item())  
            print("epoch:", epoch, "loss:", loss.item())  
            if self.validation:  
                loss_val = self.criterion(self.model(self.X_test), self.y_test, self.model, 0)  
                self.loss_val_arr = np.append(self.loss_val_arr, loss_val.item())  
                print("epoch:", epoch, "loss_val:", loss_val.item())
            self.optimizer.zero_grad()  
            loss.backward()  
            self.optimizer.step()   # backward  propagation and optimization  
            if loss < target_loss or epoch > max_epoch_num:  
                break        self.epoch_number = epoch  
  
    def plot_loss_figure(self):  
        if not hasattr(self, "epoch_number"):  
            raise NotImplementedError("The model haven't been build!")  
        if self.validation:  
            plot_figure([np.arange(1, self.epoch_number+1), np.arange(1, self.epoch_number+1)], [self.loss_arr, self.loss_val_arr], fmts=['b-', 'r--'])  
        else:            plot_figure(np.arange(1, self.epoch_number+1), self.loss_arr, fmts = 'b-')  
        #plot_figure(np.arange(1, self.epoch_number+1), , fmts='r--')  
        plt.show()  

if __name__ == "__main__":  
    # w = 0.01 * torch.ones((200,1), dtype=torch.float32)  # for d = 200, but very small number of samples 20  
    # b = torch.tensor([0.05], dtype=torch.float32)  
    w = torch.tensor([1, 2, 3, 5, 3, 10], dtype=torch.float32).unsqueeze(1)  
    b = torch.tensor(5, dtype=torch.float32)  
  
    X, y = generate_sample(w,b,noise_amp=1.0, sample_number=13000)  #  
    # split into    A = Weightdecay_Linear_regression(X,y)  
    A.split_sapmle(12000)  
    A.calculate(batch_size=10,target_loss=0.01, max_epoch_num=1200)  
    A.plot_loss_figure()
```

this figure shows the loss function: 
![[attachments/Pasted image 20240901182734.png|500]]
in the torch.optim.SGD, we can set the different weight decay options, like 

```python
optimizer = SGD(params=model.parameters() ,lr=0.01, weight_decay=3)
#note : model is a class defined by super nn.Module
```

note that by defaut, pytorch decay both weights and biases, when we disable the weight decay, the model may be overfitting 

> [!caution] Hint
> weight_decay parameter in SGD is set to 0 as default, hence we often set it to a reasonable value manually.

`````ad-note
title: Weight decay
collapse: open

Weight decay, also known as **L2 regularization**, is a technique used in deep learning to prevent overfitting by penalizing large weights in the model. Here’s a detailed introduction to weight decay:

### What is Weight Decay?

Weight decay is a regularization technique that adds a penalty term to the loss function, which is proportional to the sum of the squared weights of the model. [This penalty term discourages the model from assigning too much importance to any single feature, thereby promoting simpler models that generalize better to unseen data](https://d2l.ai/chapter_linear-regression/weight-decay.html)[1](https://d2l.ai/chapter_linear-regression/weight-decay.html)[2](https://machinelearningmastery.com/how-to-reduce-overfitting-in-deep-learning-with-weight-regularization/).

### How Does Weight Decay Work?

The primary loss function (L_{\text{original}}(w)) is modified to include the weight decay term, resulting in a new loss function (L_{\text{new}}(w)):

$$L_{\text{new}}(w) = L_{\text{original}}(w) + \lambda \sum_{i} w_i^2$$

Here, (w) represents the weights of the model, and (\lambda) is a hyperparameter that controls the strength of the regularization. [The larger the value of (\lambda), the more the weights are penalized](https://d2l.ai/chapter_linear-regression/weight-decay.html)[3](https://paperswithcode.com/method/weight-decay).

### Why Use Weight Decay?

Weight decay helps in:

1. [**Reducing Overfitting**: By penalizing large weights, weight decay prevents the model from fitting the noise in the training data, leading to better generalization on new data](https://d2l.ai/chapter_linear-regression/weight-decay.html)[2](https://machinelearningmastery.com/how-to-reduce-overfitting-in-deep-learning-with-weight-regularization/).
2. [**Improving Model Stability**: Smaller weights lead to a more stable model that is less sensitive to small changes in the input data](https://d2l.ai/chapter_linear-regression/weight-decay.html)[1](https://d2l.ai/chapter_linear-regression/weight-decay.html).

### Implementing Weight Decay

Weight decay can be implemented in various deep learning frameworks. For example, in Keras, you can add weight regularization to a layer using the `kernel_regularizer` argument:

```python
from keras.layers import Dense
from keras.regularizers import l2

model.add(Dense(32, kernel_regularizer=l2(0.01)))
```

[This code snippet adds an L2 regularizer with a penalty factor of 0.01 to a dense layer](https://d2l.ai/chapter_linear-regression/weight-decay.html)[2](https://machinelearningmastery.com/how-to-reduce-overfitting-in-deep-learning-with-weight-regularization/).

### Conclusion 
Weight decay is a powerful and widely-used regularization technique in deep learning. By adding a penalty term to the loss function, it helps in reducing overfitting and improving the generalization of the model. Understanding and effectively using weight decay can significantly enhance the performance of your deep learning models.
`````

