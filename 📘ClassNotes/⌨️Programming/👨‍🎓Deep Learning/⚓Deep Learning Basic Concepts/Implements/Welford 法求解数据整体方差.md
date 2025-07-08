## **Welford's Online Algorithm** 递归方差计算方法
### (1) 算法推导 
全局方差定义为 : 
$$\sigma_{n}^{2} =  \frac{1}{n}\sum_{i = 1}^{n} (x_{i} - \mu_{n})^{2}$$
其中,  均值可以计算为 : 
$$\mu_{n} = \frac{1}{n} \sum_{i = 1}^{n}  x_{i} = \frac{1}{n}\left( \sum_{i = 1}^{n-1 }  x_{i}  + x_{n} \right) = \frac{1}{n}((n-1)\mu_{n-1}  + x_{n})$$
$$\Large \boxed{= \mu_{n-1} + \frac{x_{n} - \mu_{n-1}}{n}}$$
其中可取 $\mu_{0} = 0$ 计算 $\mu_{1}$ 仍然成立; 


对于整体的方差计算中， 考虑前 n 项和 $S_n$ : 
$$S_{n} = \sum_{i = 1}^{n}(x_{i} - \mu_{n})^{2}$$
则上式可推导为 : 
$$= \sum_{i = 1}^{n} ((x_{i} - \mu_{n-1}) - (\mu_{n} - \mu_{n-1}))^{2}$$
$$=  \sum_{i = 1}^{n} (x_{i} - \mu_{n-1})^{2} - 2 (\mu_{n} - \mu_{n-1})   \sum_{i = 1}^{n}(x_{i}- \mu_{n-1} ) + n(\mu_{n} - \mu_{n-1})^{2}$$
其中 : 
- $\sum_{i=1}^n (x_i - \mu_{n-1}) = n(\mu_n - \mu_{n-1})$（由均值递推公式可得）
- $\sum_{i=1}^n (x_i - \mu_{n-1})^2 = S_{n-1} + (x_n - \mu_{n-1})^2$  
代入得到 : 
$$
S_n = S_{n-1} +  (x_{n} - \mu_{n-1})^{2} - n (\mu_n - \mu_{n-1})^2
$$
由于 $\mu_n - \mu_{n-1} = \frac{x_n - \mu_{n-1}}{n}$，所以 : 
$$
\Large \boxed{S_{n} = S_{n-1}  +n (n-1)  (\mu_{n} - \mu_{n-1})^{2} = S_{n-1 } + \frac{n-1}{n}(x_{n} - \mu_{n-1})^{2}}
$$
而由于 : 
$$\begin{align*}
(x_n - \mu_{n-1})(x_n - \mu_n) &= (x_n - \mu_{n-1}) \left( x_n - \mu_{n-1} - \frac{x_n - \mu_{n-1}}{n} \right) \\
&= (x_n - \mu_{n-1})^2 \left( 1 - \frac{1}{n} \right) \\
&= \frac{(x_n - \mu_{n-1})^2 (n-1)}{n}
\end{align*}$$
得到递推公式为 : 
$$\Large \boxed{S_{n} = S_{n-1}  +n (n-1)  (\mu_{n} - \mu_{n-1})^{2} = S_{n-1 } + (x_{n} - \mu_{n-1}) (x_{n} - \mu_{n})}$$
其中 $S_{1} = (x_{1}-\mu_{1})^{2}$ , 需要注意不能够取 $S_{0} = 0$ 进行计算;  

优点 : 
- **数值稳定**：避免直接计算平方和时的数值误差（传统方法 $\sum x_i^2 - n \mu^2$ 可能因大数相减导致精度损失）。
- **在线计算**：适用于流式数据，无需存储所有历史数据。
- **高效**：每步仅需 $O(1)$ 计算。

### (2) 算法实现示例  
以计算 CIFAR-10 的均值和方差为例， 注意 :  

```python fold title:CIFAR10-std-calculation
import torch  
import torchvision  
from torch import nn  
from torchvision import transforms  
from tqdm import tqdm  
  
# firstly, transform the dataset to tensor  
# in ToTensor(), the pixel value range is [0, 255] and divide by 255 automatically  
trans = transforms.Compose([transforms.ToTensor()])  
  
train_dataset_raw = torchvision.datasets.CIFAR10(  
    root='./data', train=True, download=True, transform=trans  
)
mean = torch.zeros(3)  
M2 = torch.zeros(3, 32, 32)  # use an M2 matrix to store the variance of each pixel  

# Welford Algorithm to calculate the variance  
i = 0  # iter num  
  
test_img, _ = train_dataset_raw[0]  
num_pixels = test_img.shape[1] * test_img.shape[2]  # number of pixels in the image  
  
for img, _ in tqdm(train_dataset_raw):  
    delta = (img - mean.unsqueeze(1).unsqueeze(2)).sum(dim=(1, 2)) / num_pixels  # x_n - mu_{n-1}  
    mean_new = mean + delta / (i + 1)  # update mean  
  
    mu_n = mean.unsqueeze(1).unsqueeze(2)  
    mu_n1 = mean_new.unsqueeze(1).unsqueeze(2)  
  
    if i == 0:  
        M2 = (img - mu_n) ** 2  # initialize M2  
    else:  
        M2 += (img - mu_n) * (img - mu_n1)  
    mean = mean_new  
    i += 1  
  
std = torch.sqrt(torch.sum(M2, dim=(1, 2)) / (num_pixels * len(train_dataset_raw)))  
print(mean, std)
```

result is : `tensor([0.4914, 0.4822, 0.4465]) tensor([0.2470, 0.2435, 0.2616])` 

We can use the  following code for checking the above result : 
```python fold title:check-for-the-above
# load all the train dataset into memory  
data_tot = torch.zeros(len(train_dataset_raw), 3, 32, 32)  
for i in range(len(train_dataset_raw)):  
    data_tot[i] = train_dataset_raw[i][0]  
  
mean = torch.mean(data_tot, dim=(0, 2, 3))  
std = torch.std(data_tot, dim=(0, 2, 3))  
print(mean, std)
```

