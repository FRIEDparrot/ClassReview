# 1. Basic Knowledges
## (1) introduction 
the basic process is still : 
1. load data and pass it to model  
2. generating output 
3. calculating the loss  
4. take gradients with respect to weights  
5. update the model  

for discrete non-negative random variable, including (1) hard assignments (2) soft assignments (posibility of each category)

for classification, we can divide it into 3 category like {1,0,0}, {0,1,0} and {0,0,1}. that is the one-hot encoding:
$$y \in \{\{1,0,0\}, \{0,1,0\} , \{0,0,1\} \} $$

for single-layer neural network, the input and output model is : 
$$\left[\begin{matrix}
o_{1} = x_{1}w_{11} + x_{2}w_{12} + \dots  + x_{n}w_{1n} + b_{1} \\ 
o_{2} = x_{1}w_{21} + x_{2}w_{22} + \dots  + x_{n}w_{2n}  + b_{2}\\ 
\dots  \\ 
o_{n} = x_{1}w_{n1} + x_{2}w_{n2} + \dots  + x_{n}w_{nn} + b_{n}
\end{matrix}\right]$$
this construct a single-layer neural network: 
![[Excalidraw/Chapter4 Linear Neural Networks for Classification 2024-09-02 09.04.51|650]]

## (2) Softmax Classification Loss
since there's no guarantee that outputs $o_i$ sum up to 1 or even nonnegative. so we may corrupt y by adding noise $\varepsilon$ like  $y = o + \varepsilon$, where $\varepsilon \in N(0, \sigma^{2})$, but it  doesn't works well compared to softmax, which use :
$$\hat{y} = \frac{\exp(o_{i})}{\sum^{n}_{j=1}  \exp(o_{j})}$$
hence the maximum of $y_i$ gives the maximum probablity. that is:
$$\text{argmax} \space \hat{y}_{i}  = \text{argmax}\space o_{j}$$
also, all the element would be positive and sum up to 1 after this step.
we can use  vectorization and obtain: 
$$O = XW + b \qquad  \hat{Y} = softmax (O)$$

## (3) Loss Function 
Since we have mapping from x to probability $\hat{y}$,  we will rely on the maximum likelihood estimation. (use maximum-likelihood estimation) but <mark style="background: transparent; color: red">we use Log-Likeihood method here</mark>.

where $X$ is the feature, and $Y$ is one-hot encoding. and also $\hat{y}$ (that function gives) **is the conditional probabilities of each class**.)
$$\hat{y}_1 = P(y = class1 | x) \qquad \hat{y}_{2}= P (y = class2 | x) \qquad  \dots  $$
in a dataset to train with sample number $n$, and each sample contain its features $\boldsymbol{x}^{(i)}$ and targets $\boldsymbol{y}^{(i)}$,we can <b><mark style="background: transparent; color: blue">compare the estimates with reality by checking how probable the actual classes are according to the model</mark></b>. so in the whole model(n samples), we have : 
$$\boxed{P(Y|X) = \prod^{n}_{i=1} P(\boldsymbol{y}^{(i)} | \boldsymbol{x}^{(i)} )}$$
since maximizing the product of terms is awkward, we take the negative logarithm, and <mark style="background: transparent; color: red">maximizing the likelihood turn into minimizing the negative log-likelihood</mark>. hence <u>minimize loss function is to maximum P(Y|X) (the correct classification probalblity) in the entire datasheet</u>. so: 
$$- \log P(Y|X) =  \sum^{n}_{i=1} - \log P (y^{(i)} | x^{(i)})  = \sum^{n}_{i=1} l (y^{(i)} , \hat{y}^{(i)})$$
where $l$ is loss function, **we assume $\hat{y}$ have $q$ classes**, refer to the [[📘ClassNotes/📐Mathmatics/🎣Probability Theory/第七章 参数估计#(2) 最大似然估计法|maximum similarity estimation]], the probability of $\sum^{n}_{i=1} - \log P (y^{(i)} | x^{(i)})$ is the maximum value that can be reached in all possible $\hat{y}$. 

we know  that the loss function is <b><mark style="background: transparent; color: orange">cross-entropy loss</mark></b>, where $y$ is **one-hot encoding vector**, also note we <b><mark style="background: transparent; color: orange">only consider the correct classified samples, so loss less than 0 only when truth</mark></b> $y_{j} =  1$ <b><mark style="background: transparent; color: orange">, or is correctly classified</mark></b>.
$$\boxed{l(y, \hat{y}) =  - \sum^{q}_{j=1} y_{j} \log \hat{y}_{j}}\qquad  (\hat{y}_{j} \rightarrow P(y^{(i)} | x^{(i)}))$$
> [!hint] Construct method of loss function
> the method for constructing loss function only aim to maximize the probility to get maximum probility when prediction is right. but doesn't consider the case where prediction is wrong. (also in this case $\log \hat{y}_i = - \infty$)

the problem finally turns into: 
$$minimizing: \boxed{- \sum^{n}_{i=1} \sum^{q}_{j=1} y_{j}^{(i)} \log  \hat{y}_{j}^{(i)}}\qquad \text{ i for every sample}$$
where $y_j$ is the true class, and $\hat{y}_{j}$ is the predicted probablity.

> [!caution] Note
> minimizing the "softmax" Loss is the core mind of the softmax loss classification method

<b><mark style="background: transparent; color: blue">this loss function is commonly called cross-entropy loss</mark></b>, note here $l(y, \hat{y}) = 0$ only if we predict the actual label with certainty($\hat{y} = 1$). 

## (4) Softmax and Cross-Entropy loss Relation
The softmax function is commonly used with cross entropy loss, we get it by substituting the $\hat{y}$ with $o$ from the neural network output, then
$$\hat{y}_{j} = \text{softmax} (o_{j}) = \frac{\exp(o_{j})}{ \sum^{q}_{k=1} \exp(o_{k})}$$
hence:
$$l(y, \hat{y}) = - \sum^{q}_{j=1} y_{j}  \log \frac{\exp(o_{j})}{ \sum^{q}_{k=1} \exp(o_{k})}$$

> [!hint] Log partition function 
> The function $g(x) = \log \sum_{i} \exp x_{i}$ is also referred to as log-partition function. 

with simple derivation, we get: 
$$l(y, \hat{y}) = \sum^{q}_{j=1} y_{j} \left[ \log \left(\sum^{q}_{k=1} \exp (o_{k}) \right) - o_{j} \right]$$
note that $y_j$ <mark style="background: transparent; color: red">only take 1 at true result</mark>, hence the front term becomes 1, then the loss function become:
$$\Large\boxed{l(y, \hat{y}) =  \log\sum^{q}_{k=1} \exp (o_{k}) - \sum^{q}_{j=1} y_{j} o_{j}}$$
that is called **cross-entropy loss function** in classification. also the most common loss function used in classification problems. 

note we <mark style="background: transparent; color: red"> derivate is difference between the estimated probability and observation result </mark>: 
$$\frac{\partial l(y, \hat{y})}{\partial o_{j}} = \text{softmax}(o_{j})- y_{j}$$
also, when we observe a entire distribution over outcomes like (0.1, 0.2, 0.7), the loss function above still works fine.

## (5)  entropy concept of Information theory 
for <mark style="background: transparent; color: red">quantify the amount of infomation contained in data</mark>, for a distribution $P$ the entropy is defined (see [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/👨‍🎓深度学习算法原理(sklearn)/2. 决策树和回归树#(2) 信息熵测度|decision tree algorithm]]): 
$$\boxed{H[P] = \sum _{j}  - P(j) \log P(j)}\tag{4.1.11}$$
One of the Fundamental theorems is, <mark style="background: transparent; color: red">in order to encode the  data drawn randomly from distribution P, we need at least H[P] "nats" to encode it</mark>. 

the "nat" here is equal to a bit with base e(rather than 2) $= \frac{1}{\log 2}\approx 1.44bit$

Shannon settled on $\log_{} \frac{1}{P(j)} = - \log_{} P(j)$ to quantify the suprisal when observing the event $j$. in this case, equation 4.1.11 is the <mark style="background: transparent; color: orange">expected suprisal when one assigned the correct probablities that truly atc the data generating process. </mark>

we compare the cross-entropy and the **entropy here. the cross-entropy from P to Q is** 
$$H(P, Q) \xlongequal{def}  \sum_{j} - P(j) \log_{} Q (j)$$
note the entropy $H(P,P) = H[P]$ <mark style="background: transparent; color: orange">is the lowest possible cross entropy that is achieved</mark>, which mean the lowest surprisal. hence cross-entropy classificaition is <b><mark style="background: transparent; color: orange">both maximizing the likelihood but also minimizing the surprisal</mark></b> (the number of bits required to communicate the labels)

for any connected layer with d inputs and q outputs, the parameterization and computational cost is $O(dq)$ , but  the cost of transforming <mark style="background: transparent; color: orange">inputs into outputs can be reduced through approximation and compression</mark>. we can use Quaternion-like decompositions to reduce the cost to $O\left(\frac{dq}{n}\right)$, 

> [!HINT] RealSoftMax 
> RealSoftMax(a; b) = log(exp(a)+ exp(b)). since exp > 0, it's apperantly that $RealSoftMax(a,b)> \max (a, b)$ also , when  $\lambda \rightarrow \infty$, $\lambda^{-1}\text{RealSoftMax}(\lambda a,\lambda b) \rightarrow \max(a,b)$


> [!CAUTION] Parameters
> Actually, since we all define the parameter of the network <b><mark style="background: transparent; color: orange">by inheriting nn.Module</mark></b>, we can access the all parameter by `model.parameters`, for example :
>```python
>model = nn.Module() # inherited class (define network)
>torch.optim.SGD(model.parameters(), lr = 0.01)
>```

## (6) Image Classification  
the mnist dataset can be generated by `from torchvision.datasets import FashionMNIST`. 
In order to script the transformations, please use `torch.nn.Sequential` instead of [`Compose`](https://pytorch.org/vision/0.9/transforms.html #torchvision.transforms.Compose "torchvision.transforms.Compose"). 
```python
transforms = torch.nn.Sequential(
    transforms.CenterCrop(10),
    transforms.Normalize((0.485, 0.456, 0.406), (0.229, 0.224, 0.225)),
)
scripted_transforms = torch.jit.script(transforms)
``` 
we can use `next(iter(dataloader))` for generating the data continuously, the following is an example of the <b><mark style="background: transparent; color: orange">usage of pytorch DataLoader</mark></b> :
```python
self.train_dataloader = torch.utils.data.DataLoader(dataset = self.train, batch_size = 64, shuffle = True)

def train_loader(self):  
    x,y = next(iter(self.train_dataloader))  
    print(y)

# use the following in main function : 
a = MNIST_Module()  
a.train_loader()  
a.train_loader()
```
for image plot in pytorch. it must be reshaped into a 4 dimension vector. which size is $[1, 1, w, h]$ for convolution(use squeeze for add dimension).
```python
a.show_data_img(fig_data1.squeeze(0))    # must be 4 dimensions for image display

def show_data_img(self, fig_data):  
    plt.imshow(fig_data.numpy())
```
for **the basic classification Model**, we can also reference [[📘ClassNotes/⌨️Programming/🐍Python/🌟Python 基础部分/6. nn 库使用 SGD 训练MINIST 数据集|the simplest nn lib usage]]. 
```python 
self.optim = torch.optim.SGD()
self.net = nn.Sequential(  
	nn.Conv2d(in_channels=1, out_channels=32, kernel_size=3, stride=1, padding=1),  
)
self.criterion = nn.CrossEntropyLoss()
```

# 2. Softmax Regression Scratch 
### (1) MINIST Classification practice
for computing the softmax function, we need to calculate the mean value of exponentiation of each sample and also:
$$\text{softmax} (X)_{ij}  =  \frac{\exp(X_{ij})}{\sum_{k} \exp X_{ik }}$$
the denominator($\sum_{k} \exp X_{ik}$) is called partition function (can be log for log-partition function).
```python
m = nn.Softmax(dim=1)
input = torch.randn(2, 3)
output = m(input)
```

note : <b><mark style="background: transparent; color: blue">relu, softmax, sigmoid are all in the torch.nn.functional module</mark></b>, and the softmax function and be directly called as follows : 
```python
import torch
import torch.nn.functional as F
X = torch.tensor([[1,2,3],[4,5,6]],dtype=torch.float32)
F.softmax(X)
#tensor([[0.0900, 0.2447, 0.6652],[0.0900, 0.2447, 0.6652]]) 
partition = torch.sum(X.exp(), dim =1, keepdim=True)
res = X.exp()/partition 
#tensor([[0.0900, 0.2447, 0.6652],[0.0900, 0.2447, 0.6652]]) 
```

for  cross entropy calculation, we can use (for more calculation details see [nn.CrossEntropyLoss](https://pytorch.org/docs/stable/generated/torch.nn.CrossEntropyLoss.html#torch.nn.CrossEntropyLoss) and [calculation process of cross entropy](https://blog.csdn.net/qq_36892712/article/details/130143714))
```python
a = tensor([0.5000, 1.0000])
b = tensor([1.0000, 0.5000])
- torch.sum(b *torch.log(F.softmax(a, dim=0)))  # 1.2111 
F.cross_entropy(a,b)  # 1.2111
```

also we may use
```python
self.criterion = nn.CrossEntropyLoss()  # this would not apply softmax 
```

so in summary, the whole program code is :
```python
import time
import torch  
import torch.nn as nn  
import torchvision  
from torchvision import transforms  
from torch.utils.data import dataset, DataLoader  
import matplotlib.pyplot as plt  
from torchvision.datasets import FashionMNIST  
from sklearn.metrics import accuracy_score  
import torch.nn.functional as F  

# initial transform defined by compose
trans = transforms.Compose([  
    transforms.ToTensor(),  
    transforms.Resize((28, 28)),  
    transforms.Normalize((0.5), (0.5))  
])

class MNIST_Module(nn.Module):  
    def __init__(self, batch_size = 256):  
        super(MINIST_Module, self).__init__()  
        self.train = FashionMNIST(root='data', train=True, download=True, transform=trans)  
        # use "trans" parameter for fist transform `
        self.test   = FashionMNIST(root='data', train=False, download=True, transform=trans) 
        self.train_dataloader = DataLoader(dataset = self.train, batch_size = batch_size, shuffle = True)  
        self.test_dataloader  = DataLoader(dataset = self.test, batch_size = batch_size, shuffle = True)  
  
        self.net = nn.Sequential(  
            nn.Linear(784,128, bias = True),  
            nn.ReLU(),   # not use softmax for train  
            nn.Linear(128, 10, bias=True),  
            # nn.Softmax(dim=1)     # used when not use  nn.cross-entropy loss
        )  
	
    def forward(self, x):  
        x = x.view(-1, 28*28) # reshape to 1-dim tensor  
        return self.net(x)  
  
    def train_loader(self):  
        X,y = next(iter(self.train_dataloader))  
        return X,y  
  
    def test_loader(self):  
        X,y = next(iter(self.test_dataloader))  
        return X,y  
  
    def loss(self, y_pred, y_true):  
        return F.cross_entropy(y_pred,y_true)  
  
    # if the indices is listed as a list, it will return a list of labels  
    def get_labels(self, indices):  
        self.labels = ['t-shirt', 'trouser', 'pullover', 'dress', 'coat', 'sandal', 'shirt', 'sneaker', 'bag', 'ankle boot']  
        return [self.labels[int(i)] for i in indices]  

    def show_data_imgs(self, fig_data, labels, n_nums):  
        print("show image data of shape: ", fig_data.shape)  
        fig, axes = plt.subplots( 1, n_nums, figsize=(35, 35))  
        fig_labels = self.get_labels(labels.numpy())  
        num = 0  
        for ax,fig,label in zip(axes, fig_data, fig_labels):  
            num += 1  
            ax.imshow(fig.view([28, 28]).numpy())  
            ax.set_title(label)  
            ax.axis('off')  
            if (num == n_nums):  
                break  
        plt.tight_layout()  
        plt.show()  

if __name__ == "__main__":  
    model = MNIST_Module()  
    num_epoch = 1500  
    optim = torch.optim.SGD(model.parameters(), lr=0.005)  
    criterion =  nn.CrossEntropyLoss()  
  
    for epoch in range(num_epoch):  
        X, y = model.train_loader()  
        optim.zero_grad()  
        y_pred = model(X)  
        loss = criterion(y_pred, y)  
        loss.backward()  
        optim.step()  
        if epoch % 100 == 0:  
            print(f'Epoch: {epoch}, Loss: {loss.item()}')  
  
    correct = 0  
    total = 0  
    with torch.no_grad():  
        data, target = model.test_loader()  
        outputs = model(data)  
        _, predicted = torch.max(outputs.data, 1)  
        total += target.size(0)  
        correct += (predicted == target).sum().item()  
    print(f'Accuracy: {100 * correct / total:.2f}%')  
  
    # show the last image groups  
    model.show_data_imgs(data, target, 12)  
    model.show_data_imgs(data, torch.argmax(outputs, dim=1), 12)
```

**this program would not use softmax**, but would use ReLU as activate function. and it display the following two images as true example and predicted example (can reach about 78.52% accuracy):
![[attachments/Pasted image 20240914161430.png]]
predicted result:
![[attachments/Pasted image 20240914161511.png]]

### (2) Softmax Revisited
we note that in the softmax function it gives the expression 
$$\hat{y}_{j} =  \frac{\exp o_{j}}{\sum_{k = 1}^{n} \exp o_{k}}$$
but if consider the float overflow (exceed 10^38 for float32), the result would be unstable. hence 1 solution is to subtract this from each term:
$$\overline{o} = \max_{k} o_{k}$$
and take their log and then we have:
$$\log_{}\hat{y}_{j} =  \log_{}\frac{\exp  (o_{j} - \overline{o})}{ \sum_{k = 1}^{n} \exp (o_{j} - \overline{o})} = 
o_{j} - \overline{o} - \log_{} \left(\sum_{k = 1}^{n} o_{j} - \overline{o} \right)$$
since all the terms of $o_{j } - o$ is < 0, and log part is > 0, so this both avoid overflow and underflow.

# 3. Classification generalization
### (1) Test Set and error calculation
since the goal is to learn the  general patterns, the test set is needed.
we can often guarantee generalization a priori : 

1. for many models, and desired upper bound on the <mark style="background: transparent; color: red">generalization gap</mark> $\varepsilon$ , we can determine some <b><mark style="background: transparent; color: orange">required number of n, such that if contains at least n samples</mark></b>, then the empirical error  will lie within $\varepsilon$ of the true error.

firstly, we often suppose **a fresh dataset of examples** $D = (x_i, y_j)$, where we can calculate the <mark style="background: transparent; color: red">empirical error</mark> is :
$$\varepsilon_{D}(f) = \frac{1}{n} \sum_{i = 1}^{n} 1 (f(x^{(i)}) \neq   y^{(i)})$$
we can get the empirical error from test dataset. $x^{(i)}$ are from test dataset.

and the <mark style="background: transparent; color: red">population error</mark> is given by:`
$$\varepsilon(f) = E_{(x,y) \sim P }1(f(x)\neq  y)= \int\int 1 (f(x) \neq  y) p(x,y) dxdy$$
**population error is what we finally care about**. by repre2senting, we  can view $\varepsilon_D(f)$ as a statistical estimator of $\varepsilon$. (note since the sample average $\mu$ tends to the center of the normal distribution with the test error with standard deviation $\frac{\sigma}{\sqrt{n}}$, then the approach is at the rate $O(1/\sqrt{n})$. 

also, in the Bernoulli Distribution $p(1-p)$, the <mark style="background: transparent; color: red">variance</mark> $\sigma^2$ of a  Bernoulli depends on $\varepsilon(f)$ according to $\varepsilon(f) (1-\varepsilon(f))$. hence <mark style="background: transparent; color: red">the standard deviation of the</mark> $\varepsilon_D(f)$ is not bigger than $\sqrt{0.25/n}$.  for example, if we need desired accuracy (test error to fit population error) $\delta = 0.01 = \sqrt{\frac{0.25}{n}}$, then it need 2500 samples(accuracy within one standard deviation). and for 2 variables -> 10000 samples. 

> [!NOTE] 
> a fixed classifier (always outputs the same decision regardless of the input data) is useless, since  it  model fits neither the  training data nor the test data. <mark style="background: transparent; color: red">The Central problem is tradeoff between more flexible(higher variance) but risk overfitting;  or a more rigid model class but risk underfitting.</mark>

also note **when the same test set is used to evaluate multiple models, controlling for false discovery can be difficult**. 

For example, see [[📘ClassNotes/📐Mathmatics/🎣Probability Theory/重要定理/重要分布表整理|standard normal distribution table]], $1\sigma$ range can reach 0.84 confidence, if we need 99.5% confidence, $2.58 \sigma$ is needed, and estimate error gap is 0.01 then : 
$$2.58 \cdot \sqrt{\frac{0.25}{n}}\leq  0.01$$
hence we need 16,641 samples for estimate. if 99.5% confidence is needed.

### (2) VC-dimension Introduction
For developing the  mathematical analysis to quantify where a model sits along the spectrum, and  provide the  associated guarantees,<b><mark style="background: transparent; color: orange">Vapnik and  Chervonenkis extended</mark></b> <mark style="background: transparent; color: red">the theory on the convergence of relative frequencies</mark> to <mark style="background: transparent; color: red">more general classes of functions</mark>, which gives the definition of <b><mark style="background: transparent; color: orange">Vapnik-Chervonenkis (VC)dimension</mark></b> 

VC dimension gives a measure of the <b><mark style="background: transparent; color: orange">complexity or flexibility of a model class</mark></b>. the key results is to bound the error as a function of VC dimension : 
$$P(R[p, f] - R_{emp}[X, Y, f] < \alpha )\geq  1- \delta \quad  \text{for}  \quad  \boxed{\alpha \geq  c \sqrt{\frac{VC - \log_{}  \delta }{ n}}}$$
where $\delta$ is the probability that bound is violated. $\alpha$ is the <mark style="background: transparent; color: red">upper bound on  the generalization-gap</mark>. $n$ is dataset size.

The VC dimension **quantifies the largest number of data points for which we can assign any arbitrary labeling and for each find some model $f$ in the class that agrees with that labeling**. see [[📘ClassNotes/👨‍🔧Mechanics/🖥️Computational_Mechanics/🚧结构可靠性设计基础/第八章 响应面方法和支持向量机#(1) VC维的概念|Conception of the VC dimension]]

For example, **linear models on d-dimensional input have VC dimension d+1**; (can perfectly fit d+1 item). 

### (3) Environment and Distribution Shift 
Sometimes, the model appears to **perform marvelously as measured by test set accuracy but failed catastrophically in deployment**. (<mark style="background: transparent; color: red">in the deployment, the distribution of the data suddenly shifts</mark>)

The distribution  shift include following types:
1. <b><mark style="background: transparent; color: orange">Covariate Shift</mark></b> : the labeling function $P(y|\boldsymbol{x})$ not change. but <b><mark style="background: transparent; color: orange">the distribution of the features</mark></b> $P(x)$ changed. For example, **when the training set only consists of photos , but test set contains only cartoons,** they have substantially  different characteristics.  
2. <mark style="background: transparent; color: red">Label Shift</mark> : We assume label marginal $P(y)$ can change but <mark style="background: transparent; color: red">the class-conditional distribution</mark> $P(\boldsymbol{x}|y)$ remain fixed. For example, since the <b><mark style="background: transparent; color: orange">symptoms are caused by diseases</mark></b>. label shift is reasonable here. **in the deployed cases, the edge distribution of $y$ changed while $x$ is not changed a lot**. 
3. <mark style="background: transparent; color: red">Concept Shift</mark>: this arise <b><mark style="background: transparent; color: orange">when the definitions of labels can change</mark></b>. For example, the soft drink has different concept in different zone. 

<b><mark style="background: transparent; color: orange">we may consider the above assumption to adjust our model to be more robust</mark></b>. 

> [!HINT] Implementation
> The Covariate Shift is the natural assumption that to make where we believe $\boldsymbol{x}$ cause $y$  
> The Label Shift is the natural assumption that to make where we believe $y$ cause x

firstly, we assume the data was sampled from the distribution $p_S(x,y)$, and the test data consist of examples with distribution $p_{T}(x,y)$, 

principled  algorithms can detect shift and do adapt to improve the  accuracy of the original classifier. 

for example, when predict the cancer,  it's considerably more difficult to obtain blood samples from healthy men than sick ones. in this sampling procedure, since test subjects <mark style="background: transparent; color: red">different in age hormone levels, etc</mark>(unlikely to be case with real patients),  if we only use the healthy university students as healthy implement, **we could expect extreme covariant shift**. 

also, for spam that has been trained we may have good classifier accuracy, but when the spammers create new message that unlike trained, the filter would failed. 

also if  the distribution of the dataset changes slowly, the recommendation system may works well in winter but failed in summer.  

also, consider building the web search engine in US but deploy in UK, the covariant shifts would happen.

### (4) Correction Strategies of distribution Shift 
#### 1. Basic Concept 
We know in the above cases many have different training and test distributions. 

so <mark style="background: transparent; color: red">if we can't get  models work considering the  covariance, label or concept shift</mark>, we can <b><mark style="background: transparent; color: orange">use the principled strategies to cope with shift</mark></b>. 

The Empirical risk and the risk are different. In train process, we largely minimize the loss on the training as <mark style="background: transparent; color: red">empirical risk expression</mark>: 
$$\text{minimize}_{f}\frac{1}{n}  \sum_{i = 1}^{n} l (f(x_{i}),  y_{i})$$
the risk is <b><mark style="background: transparent; color: red">the expectation of the loss over the entire population</mark></b> of the loss over data drawn from the **true distribution** $p(x,y)$;
$$E_{p} (x, y) [ l (f(x), y)] = \int\int l (f(x), y) p(x,y) d\boldsymbol{x} dy$$
we hope to minimize the risk rather than empirical risk. 

#### 2. Covariance Shift Correction 
first if we want to estimate the dependency $P(y|x)$ for which we have labeled data $(x_i, y_i)$, but the  ==observations $\boldsymbol{x}_{i}$ are drawn from the source distribution $q(\boldsymbol{x})$, but the truth distribution of $x$ is $p(x)$== . since $p(x,y) = p(y|x) p(x)$, in the following equation,we assume the conditional distribution not change($p(y|x) = q(y|x)$ since $x$ is the cause of $y$). thus we have :
$$\int\int l (f(x), y) p (y|\boldsymbol{x}) p (\boldsymbol{x}) dx dy = \int\int  l (f(x), y)q(y|x)  q(x) \frac{p(x) }{q (x)}  dxdy $$
we can **add the weight ratio** $\beta_{i} = \frac{p(x)}{q(x)}$, then the target is:
$$\text{minimize}_{f} \frac{1}{n}\sum_{i = 1}^{n} \beta_{i} l (f(x_{i}), y_{i})$$
however, in most cases, we don't know the ratio $\beta_{i}$.  so we need features $x\sim p(\boldsymbol{x})$; 

so if we have training set $\{(x_1,y_1), \dots (x_n, y_n)\}$ then a unlabeled  test set $\{u_1, \dots u_{m}\}$ are generated from the test distribution. then we can firstly, 
1. generate a <b><mark style="background: transparent; color: orange">binary classification training set</mark></b>, which <mark style="background: transparent; color: red">train set with label -1 and test set with label 1</mark> : 
$$\left\{ (x_{1}, -1), \dots  (x_{n},  -1), (u_{1}, 1), \dots (u_{m}  , 1)\right\}$$
we can now write:  
$$P(z =  1 |x) = \frac{p(x)}{p(x) + q(x)} ,  \quad  P(z= -1 |x) = \frac{q(x)}{p(x) + q(x)}$$
then we have : 
$$\beta_{i} =\frac{P (z = 1| x)}{P(z = -1 | x)} $$
we can <b><mark style="background: transparent; color: orange">build a new binary classifier using logistic regression approach</mark></b>, for mapping the $w^T x + b$  to the probability range $\{-1, 1\}$, we can construct the following equation by [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Chapter3 Linear Neural  Networks for regression(back Propagation)#(3) Sigmoid function|Sigmoid function]]: 
$$\boxed{\Large P(z = 1| x) = \frac{1}{1 + \exp (-  h(x ))}\qquad h(x) = w^{T}x+ b}$$
> [!HINT] 
> Sigmoid function is a general mapping function used in the 2-type classification.

then we get the best $h(x)$ and also consider $P(z =-1|x) = 1- P(z=1|x)$, we get:
$$\beta_{i}  = \frac{\frac{1}{1 + \exp (-h(x))}}{\frac{\exp(-h(x))}{1 +  \exp(-h(x)) }} = \frac{1}{ \exp  (-h(x))} = \exp  (h(x))$$
so we can get the best $\beta_{i}$ against $x_{i}$ or $\beta_i = \min(\exp(h(x_i), c))$ for constant $c$ and substitute it to estimate the true error. 

#### 3. Label Shift Correction 
for a k-categories classification task, we can assume that <mark style="background: transparent; color: red">the distribution of the labels shifts over time, that is</mark>, $q(y) \neq  p(y)$; but we still consider the conditional distribution is same (since y determine $x$, we have $p(x|y)=q(x|y)$) 
$$\int\int l (f(x), y) p (\boldsymbol{x}|y) p (y) d\boldsymbol{x} dy = \int\int  l (f(x), y)q(x|y) q(y) \frac{p(y) }{q (y)}  dxdy $$
then we get similar : 
$$\beta_{i} = \frac{p(y)}{q(y)}$$
we take the good  off-the-shelf classifier.

for estimating the  target label distribution, we can <mark style="background: transparent; color: red">compute the confusion matrix</mark> $C$ using validation set. 
$$Cp(y) = \mu (\hat{y})$$
and we can get the solution  
$$p(y) = C^{-1 }\mu(y)$$
also, $q(y)$ is easy to estimate, so $\beta_i$ can be calculated at the same time.

Also if the task keeps on changing slowly, the predictor, etc,may become gradually out of date. so <mark style="background: transparent; color: red">we may use the existing network weights  and simply perform a few update steps with new data</mark> to prevent Concept Shift.

### (5) Common Machine-Learning Concepts 
**Batch Learning**: only train once and the model never update again later
**Online Learning**: every time add a estimate and observation, and update the model in this step. 
Bandits 
Control (PID)  
Reinforcement Learning:  simulate the case that environment cooperate or environment try to cause accident. 
Feedback Loops

