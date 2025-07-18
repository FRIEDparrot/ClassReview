下面的两部分代码给出了最为简单的使用 Linear 层定理的 MINIST  分类方法 : 
```python
import matplotlib.pyplot as plt  
import numpy as np  
import torch  
from torchvision.datasets import mnist # 导入 pytorch 内置的 mnist 数据  
from torch import nn 
from torch.utils.data import DataLoader  

# 使用内置函数下载 mnist 数据集  
train_set = mnist.MNIST('../data/mnist', train=True,  download=True)  
test_set  = mnist.MNIST('../data/mnist', train=False, download=True)  

# 这个是定义预处理函数， 只需采用一个参数 x 即可 
def data_tf(x):  
    x = np.array(x, dtype='float32') / 255  
    x = (x - 0.5) / 0.5 # 标准化，这个技巧之后会讲到  
    x = x.reshape((-1,)) # 拉平成一维向量  
    x = torch.from_numpy(x)  
    return x
  
# 载入数据集，申明定义的数据变换  
train_set = mnist.MNIST('../data/mnist', train=True,  transform=data_tf, download=True)  
test_set  = mnist.MNIST('../data/mnist', train=False, transform=data_tf, download=True)  
# show a image of the dataset  
  
# 使用 pytorch 自带的 DataLoader 定义一个数据迭代器  
train_data = DataLoader(train_set, batch_size=64,  shuffle=True)  
test_data  = DataLoader(test_set,  batch_size=128, shuffle=False)  
  
# 使用 Sequential 定义 4 层神经网络  
net = nn.Sequential(  
    nn.Linear(784, 400),  
    nn.ReLU(),  
    nn.Linear(400, 200),  
    nn.ReLU(),  
    nn.Linear(200, 100),  
    nn.ReLU(),  
    nn.Linear(100, 10)  
)

# 定义 loss 函数 
criterion = nn.CrossEntropyLoss()  
optimizer = torch.optim.SGD(net.parameters(), 1e-1) # 使用随机梯度下降，学习率 0.1  
# 开始训练  
losses = []  
acces = []  
eval_losses = []  
eval_acces = []  

for e in range(20):  
    train_loss = 0  
    train_acc = 0  
    net.train()   # 模型改为训练模式
    for im, label in train_data: 
	    # print(label.data.shape)  # 64        # 前向传播   
        # print(im.data.shape)     # 64*784  
        out = net(im)  
        loss = criterion(out, label)   # out是64 * 10和64的比较  
        # 反向传播  
        optimizer.zero_grad()  
        loss.backward()  
        optimizer.step()  
        # 记录误差  
        train_loss += loss.item()  
        # 计算分类的准确率  
        _, pred = out.max(1)  
        num_correct = float((pred == label).sum().item())  
        acc = num_correct / im.shape[0]  
        train_acc += acc  
  
    losses.append(train_loss / len(train_data))  
    acces.append(train_acc / len(train_data))  
    # 在测试集上检验效果  
    eval_loss = 0  
    eval_acc = 0  
    net.eval()  # 将模型改为预测模式  
    for im, label in test_data:  
        im = Variable(im)  
        label = Variable(label)  
        out = net(im)  
        loss = criterion(out, label)   # 记录误差  
        eval_loss += loss.item()  # 记录准确率  
        _, pred = out.max(1)  
        num_correct = float((pred == label).sum().item())  
        acc = num_correct / im.shape[0]  
        eval_acc += acc  
        
    eval_losses.append(eval_loss / len(test_data))  
    eval_acces.append(eval_acc / len(test_data))  
    print('epoch: {}, Train Loss: {:.6f}, Train Acc: {:.6f}, Eval Loss: {:.6f}, Eval Acc: {:.6f}'  
          .format(e, train_loss / len(train_data), train_acc / len(train_data),  
                  eval_loss / len(test_data), eval_acc / len(test_data)))
```

下面是采用一个 Module 继承 nn.Module , 采用两个 Linear 和 一个 ReLU 激活函数作为网络， 这样就不需要使用任何 autograd.Variable 来将某些部分标明为变量， 并且也可以把 train 之类写在网络中 :   

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
                break        plt.tight_layout()  
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
