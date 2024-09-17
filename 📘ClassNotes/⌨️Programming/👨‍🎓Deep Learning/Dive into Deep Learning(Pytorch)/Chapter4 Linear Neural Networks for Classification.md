## 1. Basics of classification 
### (1) introduction 
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
### (2) Softmax 
since there's no guarantee that outputs $o_i$ sum up to 1 or even nonnegative. so we may corrupt y by adding noise $\varepsilon$ like  $y = o + \varepsilon$, where $\varepsilon \in N(0, \sigma^{2})$, but it  doesn't works well compared to softmax, which use : 
$$\hat{y} = \frac{\exp(o_{i})}{\sum^{n}_{j=1}  \exp(o_{j})}$$
hence the maximum of $y_i$ gives the maximum probablity. that is:
$$\text{argmax} \space \hat{y}_{i}  = \text{argmax}\space o_{j}$$
also, all the element would be positive and sum up to 1 after this step. 

 we can use  vectorization and obtain: 
$$O = XW + b \qquad  \hat{Y} = softmax (O)$$

### (3) Loss Function 
Since we have mapping from x to probablity $\hat{y}$,  we will rely on the maximum liklihood estimation. (use maximum-likelihood estimation) but <mark style="background: transparent; color: red">we use Log-Likeihood method here</mark>.

where $X$ is the feature, and $Y$ is one-hot encoding. and also $\hat{y}$ (that function gives) **is the conditional probablities of each class**.)
$$\hat{y}_1 = P(y = class1 | x) \qquad \hat{y}_{2}= P (y = class2 | x) \qquad  \dots  $$
in a dataset to train with sample number $n$, and each sample contain its features $\boldsymbol{x}^{(i)}$ and targets $\boldsymbol{y}^{(i)}$,we can <b><mark style="background: transparent; color: blue">compare the estimates with reality by checking how probable the actual classes are according to the model</mark></b>. so in the whole model(n samples), we have : 
$$\boxed{P(Y|X) = \prod^{n}_{i=1} P(\boldsymbol{y}^{(i)} | \boldsymbol{x}^{(i)} )}$$
since maximizing the product of terms is awkward, we take the negative logarithm, and <mark style="background: transparent; color: red">maximizing the likelihood turn into minimizing the negative log-likelihood</mark>. hence <u>minimize loss function is to maximum P(Y|X) (the correct classification probalblity) in the entire datasheet</u>. so:
$$- \log P(Y|X) =  \sum^{n}_{i=1} - \log P (y^{(i)} | x^{(i)})  = \sum^{n}_{i=1} l (y^{(i)} , \hat{y}^{(i)})$$
where $l$ is loss function, we assume $\hat{y}$ have q classes, refer to the [[📘ClassNotes/📐Mathmatics/🎣Probability Theory/第七章 参数估计#(2) 最大似然估计法|maximum similarity estimation]], the probability of $\sum^{n}_{i=1} - \log P (y^{(i)} | x^{(i)})$ is the maximum value that can be reached in all possible $\hat{y}$. 

the loss function would be:
$$\boxed{l(y, \hat{y}) =  - \sum^{q}_{j=1} y_{j} \log \hat{y}_{j}}\qquad  (\hat{y}_{j} \rightarrow P(y^{(i)} | x^{(i)}))$$
> [!hint] Construct method of loss function
> the method for constructing loss function only aim to maximize the probility to get maximum probility when prediction is right. but doesn't consider the case where prediction is wrong. (also in this case $\log \hat{y}_i = - \infty$)

the problem turns into: 
$$minimizing: \boxed{- \sum^{n}_{i=1} \sum^{q}_{j=1} y_{j}^{(i)} \log  \hat{y}_{j}^{(i)}}\qquad \text{ i for every sample}$$
where $y_j$ is the true class, and $\hat{y}_{j}$ is the predicted probablity.

> [!caution] Note
> minimizing the "softmax" Loss is the core mind of the softmax loss classification method

<b><mark style="background: transparent; color: blue">this loss function is commonly called cross-entropy loss</mark></b>, note here $l(y, \hat{y}) = 0$ only if we predict the actual label with certainty($\hat{y} = 1$). 

### (4) Softmax and Cross-Entropy loss 
we subsititute the $\hat{y}$ with $o$ from the neural network output, then 
$$\hat{y}_{j} = \text{softmax} (o_{j}) = \frac{\exp(o_{j})}{ \sum^{q}_{k=1} \exp(o_{k})}$$
hence:
$$l(y, \hat{y}) = - \sum^{q}_{j=1} y_{j}  \log \frac{\exp(o_{j})}{ \sum^{q}_{k=1} \exp(o_{k})}$$

> [!hint] Log partition function 
> The function $g(x) = \log \sum_{i} \exp x_{i}$ is also referred to as log-partition function. 

with simple derivation, we get: 
$$l(y, \hat{y}) = \sum^{q}_{j=1} y_{j} \left[ \log \left(\sum^{q}_{k=1} \exp (o_{k}) \right) - o_{j} \right]$$
note that $y_j$ only take 1 at true result, hence 
$$\Large\boxed{l(y, \hat{y}) =  \log\sum^{q}_{k=1} \exp (o_{k}) - \sum^{q}_{j=1} y_{j} o_{j}}$$
that is called **cross-entropy loss function** in classification. also the most common  loss funciton used in classification problems. 
note the<mark style="background: transparent; color: red"> derivate is difference between the estimate probablity and observation result </mark>: 
$$\frac{\partial l(y, \hat{y})}{\partial o_{j}} = \text{softmax}(o_{j})- y_{j}$$
also, when we observe a entire distribution over outcomes like (0.1, 0.2, 0.7), the loss function above still works fine. 

### (5)  entropy concept of Information theory 
for <mark style="background: transparent; color: red">quantify the amount of infomation contained in data</mark>, for a distribution $P$ the entropy is defined : 
$$\boxed{H[P] = \sum _{j}  - P(j) \log P(j)}\tag{4.1.11}$$
One of the Fundamental theorems is, <mark style="background: transparent; color: red">in order to encode the  data drawn randomly from distribution P, we need at least H[P] "nats" to encode it</mark>
the "nat" here is equal to a bit with base e(rather than 2) $= \frac{1}{\log 2}\approx 1.44bit$

Shannon settled on $\log_{} \frac{1}{P(j)} = - \log_{} P(j)$ to quantify the suprisal when observing the event $j$. in this case, equation 4.1.11 is the <mark style="background: transparent; color: orange">expected suprisal when one assigned the correct probablities that truly atc the data generating process. </mark>

we compare the cross-entropy and the entropy here. the cross-entropy from P to Q is 
$$H(P, Q) \xlongequal{def}  \sum_{j} - P(j) \log_{} Q (j)$$
note the entropy $H(P,P) = H[P]$ <mark style="background: transparent; color: blue">is the lowest possible cross entropy that is achieved</mark>. hence cross-entropy classificaiton is both maximizing the likelihood but also minimize the suprisal(thus -> the number of bits required to communicate the labels)

for any connected layer with d inputs and q outputs, the parameterization ans computatioinal cost is $O(dq)$ , but  the cost of transforming <mark style="background: transparent; color: orange">inputs into outputs can be reduced through approximation and compression</mark>. we can use Quaternion-like decompositions to reduce the cost to O(dq/n), 

> [!HINT] RealSoftMax 
> RealSoftMax(a; b) = log(exp(a)+ exp(b)). since exp > 0, it's apperantly that $RealSoftMax(a,b)> \max (a, b)$ also , when  $\lambda \rightarrow \infty$, $\lambda^{-1}\text{RealSoftMax}(\lambda a,\lambda b) \rightarrow \max(a,b)$

### (4) Imamge Classification 

the mnist dataset can be generated by `from torchvision.datasets import FashionMNIST`. 
In order to script the transformations, please use `torch.nn.Sequential` instead of [`Compose`](https://pytorch.org/vision/0.9/transforms.html #torchvision.transforms.Compose "torchvision.transforms.Compose"). 
```python
transforms = torch.nn.Sequential(
    transforms.CenterCrop(10),
    transforms.Normalize((0.485, 0.456, 0.406), (0.229, 0.224, 0.225)),
)
scripted_transforms = torch.jit.script(transforms)
``` 
we can use `next(iter(dataloader))` for generating the data continuously, the following is an example : 
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

for **the basic classification Model**, we can also reference [[📘ClassNotes/⌨️Programming/🐍Python/5. 最简单的 nn 库使用方法|the simplest nn lib usage]].   
```python 
self.optim = torch.optim.SGD()
self.net = nn.Sequential(  
    nn.Conv2d(in_channels=1, out_channels=32, kernel_size=3, stride=1, padding=1),  
)  
self.criterion = nn.CrossEntropyLoss()
```

### (5) Softmax Regression Implementation 
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
  
trans = transforms.Compose([  
    transforms.ToTensor(),  
    transforms.Resize((28, 28)),  
    transforms.Normalize((0.5), (0.5))  
])  
  
class MNIST_Module(nn.Module):  
    def __init__(self, batch_size = 256):  
        super().__init__()  
        self.train = FashionMNIST(root='data', train=True, download=True, transform=trans)  
        self.test   = FashionMNIST(root='data', train=False, download=True, transform=trans)  
        self.train_dataloader = DataLoader(dataset = self.train, batch_size = batch_size, shuffle = True)  
        self.test_dataloader  = DataLoader(dataset = self.test, batch_size = batch_size, shuffle = True)  
  
        self.net = nn.Sequential(  
            nn.Linear(784,128, bias = True),  
            nn.ReLU(),   # not use softmax for train  
            nn.Linear(128, 10, bias=True),  
            # nn.Softmax(dim=1)  
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

