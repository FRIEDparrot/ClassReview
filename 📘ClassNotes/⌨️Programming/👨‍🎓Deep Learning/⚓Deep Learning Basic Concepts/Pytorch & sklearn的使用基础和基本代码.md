# 1. Sklearn 库的使用 
sklearn 是最常用的机器学习库, 其包含分类, 回归算法, 聚类算法, 维度化简, 模型选择, 交叉验证, 数据预处理等等功能. 例如岭回归， 支持向量机, KNN , 朴素贝叶斯, 决策树, 特征选择, 保序回归等等算法

相比于 Pytorch,`sklearn` 中的模型（例如 `AdaBoostClassifier`）并不直接支持通过 `torch.DataLoader` 进行多次输入和训练，因为 `sklearn` <mark style="background: transparent; color: red">模型通常是基于批量训练的</mark>，而不像 PyTorch 那样采用逐批（mini-batch）训练的方式。从而提供更好的方法。
##  基础部分
### (1) sklearn 中的 Bunch 类
Bunch 是 sklearn 中最常用的结构, 类似于字典，具体参考 [[📘ClassNotes/⌨️Programming/🐍Python/🌟Python 基础部分/2. Python 基本数据结构和可视化方法|2. Python 基本数据结构和可视化方法]] 
```python 
from sklearn.datasets._base import Bunch
```
一般地, 可以通过
```python
print(bunch.keys())
```
获取其中的keys键, 对于bunch的keys部分均可以通过 `.` 进行访问。

> [!CAUTION] scipy 中的 toarray 方法
> 需要注意的是, scipy.spare.toarray() 和 numpy.array 并不相通, 稀疏矩阵可以通过 toarray() 方法进行转换为标准的 numpy 数组
### (2) 数据集训练和测试部分
sklearn 可以直接获取大量的学习数据集， 同时有分割训练集和测试集功能, 下面简介
<mark style="background: transparent; color: red">train_test_split 分割数据集</mark> 和 <mark style="background: transparent; color: red">StandardScaler 标准化的方法</mark> 

```python 
from sklearn.dataset import load_iris 
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler   # 标准化器类型 

X_train, X_test, y_train, y_test =  train_test_split(iris_data, iris_target, test_size=0.2, random_state=None)  
stdsca = StandardScaler.fit(X_train)
X_train_new = stdsca.transform(X_train)
X_test_new = stdsca.transform(X_test)
```

对于测试准确度, 可以采用 [sklearn.metrics 模块](https://scikit-learn.org/stable/api/sklearn.metrics.html)部分: 
```python
from sklearn.metrics import accuracy_score, classification_report 
from sklearn.metrics import precision score, recall_score 
from sklearn.metrics import f1_score  # F1标准, 参考sklearn 部分
```

[`accuracy_score`](https://scikit-learn.org/stable/modules/generated/sklearn.metrics.accuracy_score.html#sklearn.metrics.accuracy_score "sklearn.metrics.accuracy_score") ,[`average_precision_score`](https://scikit-learn.org/stable/modules/generated/sklearn.metrics.average_precision_score.html#sklearn.metrics.average_precision_score "sklearn.metrics.average_precision_score"), [`balanced_accuracy_score`](https://scikit-learn.org/stable/modules/generated/sklearn.metrics.balanced_accuracy_score.html#sklearn.metrics.balanced_accuracy_score "sklearn.metrics.balanced_accuracy_score"), [`brier_score_loss`](https://scikit-learn.org/stable/modules/generated/sklearn.metrics.brier_score_loss.html#sklearn.metrics.brier_score_loss "sklearn.metrics.brier_score_loss") , 
此外, 测试 MSE, MAE 也可以采用 sklearn,metrics 中直接import:

| [`mean_squared_error`](https://scikit-learn.org/stable/modules/generated/sklearn.metrics.mean_squared_error.html#sklearn.metrics.mean_squared_error "sklearn.metrics.mean_squared_error")                 | Mean squared error regression loss.(MSE)        |
| --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------- |
| [`mean_squared_log_error`](https://scikit-learn.org/stable/modules/generated/sklearn.metrics.mean_squared_log_error.html#sklearn.metrics.mean_squared_log_error "sklearn.metrics.mean_squared_log_error") | Mean squared logarithmic error regression loss. |
| [`mean_absolute_error`](https://scikit-learn.org/stable/modules/generated/sklearn.metrics.mean_absolute_error.html#sklearn.metrics.mean_absolute_error "sklearn.metrics.mean_absolute_error")             | Mean absolute error regression loss.(MAE)       |

### (3) 特征提取和分类 
```python
from sklearn.linear_model import LogisticRegression
```

如果生成分类数据示例,  可以采用 `from sklearn.datasets import make_classification`  得到
```python
from sklearn.ensemble import AdaBoostClassifier
from sklearn.datasets import make_classification
from sklearn.model_selection import train_test_split
from sklearn.metrics import classification_report

# 生成示例数据 (1000 个样本, 每个样本 20 个特征)
X, y = make_classification(
    n_samples=1000,    # 样本数量
    n_features=20,     # 特征数量
    n_informative=15,  # 有用特征数量
    n_redundant=5,     # 冗余特征数量
    random_state=42
)

# 划分数据为训练集和测试集
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42)

# 初始化 AdaBoostClassifier
clf = AdaBoostClassifier(n_estimators=50, random_state=42)

# 训练模型
clf.fit(X_train, y_train)

# 预测测试集
y_pred = clf.predict(X_test)

# 输出分类报告
print(classification_report(y_test, y_pred))
```

#### 1. 字符串标签编码(LabelEncoder)
Label 用于进行字符串标签编码为数字, 例如对于如下决策树表格(其中第一列不需要编码): 
![[attachments/Pasted image 20240912111152.png]]
其中采用 apply_map 的方法, 去除了每个字符串元素的空格; 同时采用strip去除了每个标签的空格。  
```python
import pandas as pd  
from sklearn.preprocessing import LabelEncoder  # encoder labels  
  
data_raw = pd.DataFrame(pd.read_excel("seals_data.xlsx"))  
data_proceed = pd.DataFrame()  
label_encoder = LabelEncoder()  
  
# eliminate all the white space 
data_raw = data_raw.applymap(lambda x: x.strip() if isinstance(x, str) else x)  
data_proceed['计数'] = data_raw.iloc[:,0]  

# 编码年龄, 收入, 学生, 信誉, 是否购买几列 
for column in data_raw.columns[1:]:
    data_proceed[str(column).strip()] = label_encoder.fit_transform(data_raw[column])  
print(data_proceed)
```
得到决策树编码结果如下:
![[attachments/Pasted image 20240912113356.png|200]]
#### 2. 分类器: 特征选择, 提取和主成分分析
参考 [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/👨‍🎓机器学习算法(sklearn)/1.机器学习算法和文本分类挖掘(Naive Bayes)|机器学习算法和文本分类挖掘(Naive Bayes)]], 特征提取可以通过 `from sklearn import feature_extraction` 得到
其中特征提取中有 image, text 等等几个模块; 常用到 tf-idf 模型等文本 tf-idf 特征向量计算模型。
![[attachments/Pasted image 20240910160939.png|300]]

```python
# 特征选择和特征提取部分
from sklearn import feature_extraction
from sklearn import feature_selection 
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.feature_extraction.text import TfidfTransformer 
vectorizer.fit_transform(contents_dictionary)
```

对于多标签分类部分, 需要采用多输出
```python
from sklearn.multioutput import MultiOutputClassifier, MultiOutputEstimator
# 将一个部分转换为多输出:
model = MultiOutputClassifier(MultinomialNB(alpha=0.01))
```

2. 主成分分析(PCA模块) 参考[[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/👨‍🎓机器学习算法(sklearn)/3. 推荐系统和需求搜寻算法(CF,PCA,SVD)|3. 推荐系统和需求搜寻算法(CF,PCA,SVD)]] 部分
```python 
from sklearn.decomposition import PCA
```

3. KNN, KMeans 分类器
```python
from sklearn.neighbors import KNeighborsClassifier
from sklearn.cluster import KMeans
```


## 2. PipeLine 的使用
PipeLine 是 sklearn 中的一个很方便的容器类,  可以直接在 PipeLine 中定义 Steps, 然后进行 fit 操作。 (类似的是 `torchvision.transforms.v2` 的 `v2.Compose` 和 `nn.Sequential`)
而通过 set_params 方法可以对 pipeline 中的某个名称对象属性进行设置, 具体方法是采用 `__` 进行命名区分: 
即 `set_params(name__param = new_param)` 

需要分类边界显示时, 则可以采用  sklearn.inspection 中的 DecisionBoundaryDisplay 方法。
xlabel 和 ylabel 选项和绘图 plt 设置完全相同。
下图采用 pipeline 进行分类边界显示
```python
from sklearn.pipeline import Pipeline
from sklearn.inspection import DecisionBoundaryDisplay

clf = Pipeline(  
    steps=[("scaler", StandardScaler()), ("kmeans", KMeans(n_clusters = 3))]  
)
clf.set_params(kmeans__max_iter = max_iter).fit(X_train, y_train)
disp = DecisionBoundaryDisplay.from_estimator(  
    clf,  
    X_test,  
    response_method="predict",  
    plot_method="pcolormesh",  
    xlabel=iris.feature_names[0],  
    ylabel=iris.feature_names[1],  
    shading="auto",  
    alpha=0.5,  
    ax=ax,  
)
```

需要说明,  PipeLine中的每个部分必须接受 x,y 作为 fit_predict 的参数, 因此当 fit 等仅接受一个函数 x 时, 需要重定义类型 (重定义 SOM 类), 参考[[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/👨‍🎓机器学习算法(sklearn)/5. BP 神经网络, SOM 神经网络和 Boltzmann机#(2) 采用第三方库建立 SOM 网络|5. BP 神经网络, SOM 神经网络和 Boltzmann机]] 
```python
from sklearn_som.som import SOM
clf = Pipeline(steps=[  
    ('scaler', StandardScaler()),  
    ('som', CustomSOM(m=1, n=3, dim=4))  
]) 
```

### 3. 超参数调优问题方法

使用GridSearchCV和KFold进行交叉验证
```python
from sklearn.neighbors import KNeighborsClassifier
from sklearn.model_selection import GridSearchCV, KFold
from sklearn.datasets import load_iris
 
# 加载示例数据集（这里以鸢尾花数据集为例）
data = load_iris()
X = data.data
y = data.target
 
# 定义K最近邻模型
knn = KNeighborsClassifier()
 
# 定义参数网格
param_grid = {
    'n_neighbors': [3, 5, 7],
    'weights': ['uniform', 'distance']
}
#weights：指定在预测时如何对邻居的权重进行加权，常见取值有uniform（所有邻居权重相等）和distance（权重与距离成反比）。
 
# 使用GridSearchCV和KFold进行交叉验证
kf = KFold(n_splits=5, shuffle=True, random_state=0)
grid_search = GridSearchCV(knn, param_grid, cv=kf, scoring='accuracy')
grid_search.fit(X, y)
 
# 输出最佳参数和最佳得分
print("最佳参数：", grid_search.best_params_)
print("最佳得分：", grid_search.best_score_)
```

# 2. PyTorch  
## 1. 数据类型转换
取 tensor 值采用  item(): 
`label_item = torch.Tensor(label).item()`

一般空网络采用如下方法定义:
```python
net = nn.Sequential(); 
net.add(nn.Dense(256, activation='relu'), nn.Dense(10))  
```

```python
# detach 取消求导连接,  而 numpy 转换为 numpy 数组
y.detach.numpy(); 
```

## 2. 基本数据操作
#### 1. unsqueeze,squeeze和reshape使用
增加维度或者减少维度: 
torch.squeeze(input,dim=0) **移除某个维度**
torch.unsqueeze(input,dim=0) **在某个维度上增加维度** 
reshape 的使用: 在 pytorch 等中, 我们常常使用 reshape 将数据集转换为指定的形状, 例如对于 (60000, 28,28) 的数据集, 我们会使用 `x_train.reshape(x_train.shape[0], x_train.shape[1], x_train.shape[2], 1)` 得到 (60000,28,28,1) 的4d数据集, 即将最后的28个中, 每个均作为输入拆分开;

```python title:squeeze使用示例
train[1].unsqueeze(0).shape
Out[12]: torch.Size([1, 28, 28])
A = train[1].unsqueeze(0).unsqueeze(0)
A.shape
Out[13]: torch.Size([1, 1, 28, 28]) 
A.squeeze(0).shape
Out[17]: torch.Size([1, 28, 28])
A.squeeze(0).squeeze(0).shape
Out[19]: torch.Size([28, 28])
A.squeeze([0,1]).shape
Out[22]: torch.Size([28, 28])
B.unsqueeze(dim=2).shape
Out[28]: torch.Size([28, 28, 1])
```

另外，pytorch中大多数的操作都支持 `inplace` 操作，也就是可以直接对 tensor 进行操作而不需要另外开辟内存空间，方式非常简单，一般都是在操作的符号后面加`_` 

#### 2. 常用函数集和运算 
**torch.nn.functional** 包含了**绝大多数的常用函数**: 例如 relu， softmax, sigmoid 等等。

```python
def SoftmaxRegression(nn.Module):
	self.w = torch.normal(0,simga, size =(num_inputs, num_outpus), requires_grad=True)
	self.b = torch.zeros(num_outputs, requires_grad = True)
	self.optim = torch.optim.SGD(self.parameters(), lr = 0.01)
	
	 def parameters(self) : 
		return [self.w, self.b]
	 def forward(self):
		 return softmax(torch.matmul(X.reshape((-1,  self.W.shape[0])),  self.W) + self.b)
```

对于 minist 数据集, 由于x的维度为 784, num_inputs = 784, num_outputs = 10. 即 w 为一个 784 x 10 的权重矩阵。

torch.gather(): 取某个tensor中指定下标的元素, 但是需要使用如下方法指定:
```Python
For a 3-D tensor the output is specified by: 
out[i][j][k] = input[index[i][j][k]][j][k]  # if dim == 0
out[i][j][k] = input[i][index[i][j][k]][k]  # if dim == 1
out[i][j][k] = input[i][j][index[i][j][k]]  # if dim == 2
```

#### 3. 取某些行或者某些列的元素(torch.gather)
例如gather(dim = 0) 是按行取元素, 
```python
a = tensor([[ 1,  2,  3,  4,  5], [ 6,  7,  8,  9, 10]])
a.gather(0, torch.tensor([[0, 1],[0,1]]))   # 第一列进行gather, 
# tensor([[1, 7], [1, 7]]) 
a.gather(1, torch.tensor([[0, 1],[0,1]]))
```
当 dim = 0 时, 取法为取指定行相应列的元素
$$\left[\begin{matrix}
0  &|&1 \\  0&  |  & 1
\end{matrix}\right]$$
dim = 0 时, 取法为取指定列相应行的元素。第一列取 0 行, 0行, 第二列取1行, 1行; 而
$$\left[\begin{matrix}
0  &1   \\   -  &  -  \\  0  & 1
\end{matrix}\right]$$
此时得到的是`[[1,2],[6,7]]`。 
**说明: 最常见的应用是在交叉熵的计算中, 获取到的概率矩阵 y_pred(例如784* 10), 正确目标是离散的点 y(0,1,2,3..9), 此时需要y_pred中,y所对应列的部分**, 即可采用:
```python 
y_pred.gather(1, y.view(-1, 1)) 
```

但实际上用的多的还是如下的方法: 
```python
criterion =  nn.CrossEntropyLoss()
y_pred = model(X)
loss = criterion(y_pred, y)
```

## 3. 基本求导方法
#### 1. 反向传播算法求导
以下为基本的求导方式代码, 实际上仅需要 torch.tensor 中定义 `requires_grad = True` 之后, 计算并采用 backward 求导 :  
```python
m = torch.tensor([[2, 3]], dtype=torch.float, requires_grad=True) # 构建一个 1 x 2 的矩阵
# 注意：[[]]是定义行向量，而[]定义列向量
n = torch.zeros(1, 2) # 构建一个相同大小的 0 矩阵
print(m)
print(n)
# 通过 m 中的值计算新的 n 中的值
print(m[0,0])
n[0, 0] = m[0, 0] ** 2
n[0, 1] = m[0, 1] ** 3
print(n)

n.backward(torch.ones_like(n)) # 将 (w0, w1) 取成 (1, 1), 进行自动求导
print(m.grad)
# tensor([[ 4., 27.]], grad_fn=<CopySlices>)
```
将上面的式子写成数学公式，可以得到: 
$$
n = (n_0,\ n_1) = (m_0^2,\ m_1^3) = (2^2,\ 3^3) 
$$
下面我们直接对 `n` 进行反向传播，也就是求 `n` 对 `m` 的导数。
这时我们需要明确这个导数的定义，即如何定义
$$
\frac{\partial n}{\partial m} = \frac{\partial (n_0,\ n_1)}{\partial (m_0,\ m_1)}
$$
$$
\frac{\partial n}{\partial m_0} = w_0 \frac{\partial n_0}{\partial m_0} + w_1 \frac{\partial n_1}{\partial m_0} = 2 m_0 + 0 = 2 \times 2 = 4
$$
$$
\frac{\partial n}{\partial m_1} = w_0 \frac{\partial n_0}{\partial m_1} + w_1 \frac{\partial n_1}{\partial m_1} = 0 + 3 m_1^2 = 3 \times 3^2 = 27
$$
#### 2. 多次自动求导
通过调用 backward 我们可以进行一次自动求导，<mark style="background: transparent; color: red">如果我们再调用一次 backward，会发现程序报错，没有办法再做一次。这是因为 PyTorch 默认做完一次自动求导之后，计算图就被丢弃了，所以两次自动求导需要手动设置, 即设定 backward 方法中 retain_graph = True 保留计算图</mark> 
```python
x = torch.tensor([3], dtype=torch.float, requires_grad=True)
y = x * 2 + x ** 2 + 3
print(y)
y.backward(retain_graph=True) # 设置 retain_graph 为 True 来保留计算图
print(x.grad)
```

> [!CAUTION] 梯度归零
> 注意：如果是循环迭代求解梯度的情况下, 我们希望每一次计算并仅求解一次梯度，则求导完毕之后, 需要使用grad.data.zero_()来归零梯度，否则将会使梯度累加。具体如下: 
>```python
>while (1) : 
>	w.requires_grad_(True);  # ask for the space for gradient.  
>	b.requires_grad_(True);  #  
>	try:  
>	    w.grad.zero_()  
>	    b.grad.zero_()  
>	except:  
>	    pass
>```

## 二、基本回归模型和训练步骤
### (1) 基本模型训练步骤
在 Pytorch 中, 最基本的部分包括如下几步(参考[[📘ClassNotes/⌨️Programming/🐍Python/🌟Python 基础部分/6. nn 库使用 SGD 训练MINIST 数据集|6. nn 库使用 SGD 训练MINIST 数据集]]): 

对于训练数据集, 一般需要转换为 4 维张量， 如 (5000, 3, 32, 32),  分别对应 (N, C, W, H)  
N：这一维通常代表批量大小 (batch size)，即一次性输入到网络中的样本数量。在这个例子中，一次输入 5000 个样本。
C：这一维表示输入通道数。在图像处理中，输入通道通常对应于颜色通道，`3` 表示 RGB 图像（红、绿、蓝三种颜色)
W, H：这两个维度分别表示图像的高度和宽度。 
#### 1. 定义网络模型
1. 继承继承 `nn.Module`, 并调用 `super().init()` 
2. **定义变换 (forward) 函数**
一般在 forward 函数中定义每一层变换,  而 
self.net 等一般也在   forward 中被调用, 即可以不使用 net, 而采用 forward 得到结果, 实际上都是一个输入 X 
```python
class CIFIAR_CNN(nn.Module):  
    def __init__(self, learning_rate = 0.02):  
        super(CIFIAR_CNN, self).__init__()  
  
        self.net = nn.Sequential(  
            nn.Conv2d(3, 32, kernel_size=3, padding=1),   # 首先, 由于是 RGB 数据, 输入通道数为 3, 尺寸为 (batch_size, 3, 32, 32)            nn.Conv2d( 32, 64, kernel_size=3, padding=1), # 形成 64 *32 * 32 的输出  
            # nn.MaxPool2d(kernel_size=(2,2), stride=2),  # n * 64 * 16 * 16, or use the following:  
            nn.AdaptiveMaxPool2d((16, 16)) ,                # n * 64 * 16 * 16 target size  
            nn.AvgPool2d(kernel_size=(2,2), stride=(2,2)),  # n * 64 * 8 * 8  
            nn.Flatten(1, -1),    # 将 64 * 8 * 8 的张量展平  
            nn.Linear(64 * 8 * 8, 512),  
            nn.ReLU(),         # 放在 Linear 之后, 激活函数, 用于实现非线性学习, 得到更复杂的模型  
            nn.Linear(512, 10),  # CIFAR-10 has 10 classes, so output is 10 dimensions  
            nn.Softmax(dim=1)  # Softmax 激活函数：将输出转化为概率分布  
        )  
  
    def forward(self,X):  
        return self.net(X)
```
#### 2. 加载数据和训练
1. **初始变换和加载数据集**
方法是定义一个函数或者 torchvision.Compose() 容器, 然后在 transform 中指定参数， 然后
> [!NOTE] 说明
> 在加载数据和训练的类中,  我们直接将相应的 `self.model` 指定为刚刚定义的类

**需要说明**:  `transform` 只会影响在 `DataLoader` 中批量加载数据时的图像。如果您想**查看经过缩放处理后的图像，您应该查看 `train_loader` 中的数据，而不是 `train_dataset.data`**。`train_loader` 批量加载数据时，会应用 `transform`。
```python
t = v2.Compose([  
    v2.ToDtype(torch.float32, scale=True),  
    v2.ToTensor(),  
    v2.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225]),  
])  
train_dataset = CIFAR10( root='./cifar10', train=True, download=True, transform=t)
```

2. **定义一个 DataLoader 迭代器**
```python
self.train_loader = DataLoader(train_dataset, batch_size=self.train_batch_size, shuffle=True)
```

3. **定义损失函数 (criterion 或者 loss) 和优化器(optim)** 
```python
def criterion(y_true, y_pred):
	return nn.CrossEntropyLoss(y_true, y_pred)
```

如果分类方法依赖于梯度, 则需要定义 optim 优化器: 
`optim` 的作用包括:
- **梯度更新**：基于模型参数的当前梯度调整参数值。
- **控制学习率**：学习率（`learning_rate`）是优化器的关键超参数，决定更新步伐大小。
```python
self.optim = torch.optim.SGD(model.parameters(), lr=0.01, momentum=0.9)  # 常用: SGD, Adam, RMSprop, AdaGrad 等等, 
# 其中 momentum 为用于加速收敛的动量设置项
```
需要说明,  如果不使用 optim 优化器, 可以手动进行参数更新:
```python
with torch.no_grad():  # 禁止计算图构建，直接更新参数
    for param in model.parameters():
        param -= learning_rate * param.grad  # 手动更新参数
```

4. 进行训练:  
其中， 调用 self.model.train 获取到
```python
def train_model(self, max_epoch = 100):  
    self.model.train()    # 只需要调用 train() 函数即可  
    # 初始时模型是未训练的模型  
    for epoch in range(max_epoch):  
        train_loss = 0.0  
        for data, labels in self.train_loader:  
            self.optim.zero_grad()   # 在每次小批量训练时, 都需要清零梯度  
            loss = self.criterion(self.model(data), labels)  
            loss.backward()         # 反向传播  
            self.optim.step()       # 利用反向传播得到的梯度, 进行参数更新  
            train_loss += loss.item() * data.size(0)    # 计算总和的误差
```


MaxPool 池化的参数指定方法: 
```python
# target output size of 5x7
m = nn.AdaptiveMaxPool2d((5, 7))
input = torch.randn(1, 64, 8, 9)
output = m(input)
# target output size of 7x7 (square)
m = nn.AdaptiveMaxPool2d(7)
input = torch.randn(1, 64, 10, 9)
output = m(input)
# target output size of 10x7
m = nn.AdaptiveMaxPool2d((None, 7))
input = torch.randn(1, 64, 10, 9)
output = m(input)
```


### (1)多项式回归模型
下面更进一步尝试一下多项式回归，下面是关于 x 的多项式：
$$
\hat{y} = w_0 + w_1 x + w_2 x^2 + w_3 x^3 
$$
这样就能够拟合更加复杂的模型，这里使用了 $x$ 的更高次，同理还有多元回归模型，形式也是一样的，只是除了使用 $x$，还是更多的变量，比如 $y$、$z$ 等等，同时他们的 $loss$ 函数和简单的线性回归模型是一致的。 
```python
import numpy as np  
import torch
import matplotlib.pyplot as plt
# 定义一个多变量函数
w_target = np.array([0.5, 3, 2.4]) # 定义参数
b_target = np.array([0.9]) # 定义参数

f_des = 'y = {:.2f} + {:.2f} * x + {:.2f} * x^2 + {:.2f} * x^3'.format(
    b_target[0], w_target[0], w_target[1], w_target[2]) # 打印出函数的式子

print(f_des)
```

我们下面以 w0 + w1 + w2 + b = y_sample 的式子, 以 randn 作为 w 初始值, 0 作为b 初始值进行小批量梯度算法求解获取结果(定义的两个函数中, 一个是模型, 一个是损失函数): 
```python
import numpy as np  
import matplotlib.pyplot as plt  
import torch  
  
fig = plt.figure()  
epochNum = 80  
lr = 0.001  

x_sample = np.arange(-3, 3.1, 0.1)    # 定义绘画数组区间  
y_sample = b_target[0] + w_target[0] * x_sample + w_target[1] * x_sample ** 2 + w_target[2] * x_sample ** 3  
# plt.plot(x_sample,y_sample)  
# ----------------------------------------------------------  
# 构建数据 x 和 y# x 是一个如下矩阵 [x, x^2, x^3]# y 是函数的结果 [y]x_train = np.stack([x_sample ** i for i in range(1, 4)], axis=1)  # 构建从相应的数据取得的自变量范围矩阵  
x_train = torch.from_numpy(x_train).float() # 转换成 float tensory_train = torch.from_numpy(y_sample).float().unsqueeze(1) # 转化成 float tensor  
# 定义参数 w和b  
w = torch.randn((3, 1), dtype=torch.float, requires_grad=True)  # 每一次run得到的初始fit曲线不同  
b = torch.zeros((1), dtype=torch.float, requires_grad=True)  

# 定义模型
def multi_linear(x):  
    return torch.mm(x, w) + b   # 注: mm可以使用matmul来进行代替  
  
def loss_func(y_, y):  # 使用平方函数作为损失函数  
    return torch.mean((y_ - y) ** 2)  
y_pred = multi_linear(x_train) # 构建多项式函数  
  
ax1 = fig.add_subplot(121)  
ax1.plot(x_train.data.numpy()[:, 0], y_pred.data.numpy(), label='curve_prefit', color='r')  
ax1.plot(x_train.data.numpy()[:, 0], y_sample, label='real curve', color='b') 

# 画出更新之前的模型  
# ============ 训练数据集 ====================
for epoch in range (epochNum):  
    loss = loss_func(y_pred, y_train)  
    try:  
        w.grad.zero_()  
        b.grad.zero_()  
    except:  
        pass  
    loss.backward()  
    w.data = w.data - lr * w.grad.data  
    b.data = b.data - lr * b.grad.data  
    y_pred = multi_linear(x_train)  
  
ax2 = fig.add_subplot(122)  
ax2.plot(x_train.data.numpy()[:, 0], y_pred.data.numpy(), label='fitting curve-', color='r')  
ax2.plot(x_train.data.numpy()[:, 0], y_sample, label='real curve', color='b')  
  
plt.legend()  
plt.show()
```
### (2) Logistic 回归模型
对于经典的$(0,1)$模型，使用交叉熵损失函数
$$loss = -[y * log(\hat{y}) + (1-y) *log(1-y)]$$
```python
def logistic_regression(x):
    return torch.sigmoid(torch.mm(x, w) + b)
## mm: matmul

y_pred = logistic_regression(x_data)
# 计算loss, 使用clamp的目的是防止数据过小而对结果产生较大影响。
def binary_loss(y_pred, y):
    logits = ( y * y_pred.clamp(1e-12).log() + \
              (1 - y) * (1 - y_pred).clamp(1e-12).log() ).mean()
    return -logits
```
只需要将loss改为这个即可。

其中，pytorch提供了相关的函数，重要的是 torch.optim.SGD 可以直接采用小批量梯度回归, 另外每一次需要调用 optimizer.zero_grad()清零优化器梯度即可。
```python title:最简单的小批量梯度下降回归代码(外部定义模型y_pred和损失binary_loss)
# 使用 torch.optim 更新参数
from torch import nn
import time
# use the neural network model for training
# 定义优化参数
w = nn.Parameter(torch.randn(2, 1))
b = nn.Parameter(torch.zeros(1))

# 优化器的设定以及相关的使用
optimizer = torch.optim.SGD([w, b], lr=0.1) # 使用SGD算法

for e in range(1000):
    # 前向传播
    y_pred = logistic_regression(x_data)  # 这个y_pred和loss是自己定义函数计算的
    loss = binary_loss(y_pred, y_data)      # 计算 loss
    # 反向传播
    optimizer.zero_grad() #-----使用优化器将梯度归 0
    loss.backward()
    optimizer.step() # *** 在这一步中，更新参数被封装，使用优化器来更新参数 **** 
    # 计算正确率
    mask = y_pred.ge(0.5).float()
    acc = (mask == y_data).sum().item() / y_data.shape[0]
```


## 三、PyTorch提供的损失函数 
前面使用了自己写的 loss函数，其实 PyTorch 已经提供了一些常见的 loss函数，比如线性回归里面的 loss 是 `nn.MSE()`，而 Logistic 回归的二分类 loss 在 PyTorch 中是 `nn.BCEWithLogitsLoss()`，关于更多的 loss，可以查看[文档](https://pytorch.org/docs/stable/nn.html#loss-functions)

PyTorch 实现的 loss函数有两个好处：第一是方便使用，不需要重复造轮子；第二就是其实现是在底层 C++ 语言上的，所以速度上和稳定性上都要比自己实现的要好。

另外，PyTorch 出于稳定性考虑，将模型的 Sigmoid 操作和最后的 loss 都合在了 `nn.BCEWithLogitsLoss()`，所以我们使用 PyTorch 自带的 loss 就不需要再加上 Sigmoid 操作了
```python
# 使用自带的loss
criterion = nn.BCEWithLogitsLoss() # 将 sigmoid 和 loss 写在一层，有更快的速度、更好的稳定性
w = nn.Parameter(torch.randn(2, 1))
b = nn.Parameter(torch.zeros(1))

def logistic_reg(x):
    return torch.mm(x, w) + b
    
optimizer = torch.optim.SGD([w, b], 1.)

y_pred = logistic_reg(x_data)
loss = criterion(y_pred, y_data)  # 这样定义损失函数
# 之后调用
for ......
	loss.backward()
	optimizer.step()_
print(loss.data)
```

常见的损失函数：
浅层： LR，SCM, ELm
深层：CNN ..

## 四、神经网络池化层的概念简介
池化层也是一个类似二维的层;

池化层在CNN中,用于减少空间维数进行降维使用, 实际上是使用一个feature map, 其宽高分别为 nh 和 nw, 类似于卷积过程, 在 map 的指定范围内进行扫描; 然后过滤出符合要求的部分。
一般池化需要指定 feature map 大小和 stride, 示例如下: 
![[attachments/1_fhdK1xJND_m1Rdmr1m_9PQ.webp]]
例如16x16的输入在2x2窗, 步进为(2x2)池化后成为 8x8 的特征; 常见的池化有均值池化和最大值池化等等; 


# 图像处理专题
### (1) 图片缩放代码 (Pytorch, sklearn 和  cv2) 
#### 1.  通过 skimage 库进行图形缩放 
一般地,  对于 32 x 32 的图像数据集, 输入是一个 (50000, 32, 32, 3) 的向量 , 可以采用 `cv2.resize` 或者 skimage.transform.resize 进行批量调整
```sh
pip install scikit-image
```

以下是 skimage 的
```python
import numpy as np
from skimage.transform import resize

# 假设输入的图像数据是 (50000, 32, 32, 3)
images = np.random.rand(50000, 32, 32, 3)

# 使用 resize 批量调整大小
resized_images = np.zeros((images.shape[0], 28, 28, 3))  # 初始化一个新的数组，目标尺寸是 (50000, 28, 28, 3)
for i in range(images.shape[0]):
    resized_images[i] = resize(images[i], (28, 28), mode='reflect', preserve_range=True)

print(resized_images.shape)  # 输出 (50000, 28, 28, 3)
```
处理 大批量图像时, 也可以采用
```python
import numpy as np
import cv2

# 假设输入的图像数据是 (50000, 32, 32, 3)
images = np.random.rand(50000, 32, 32, 3)

# 使用 cv2.resize 批量调整大小
resized_images = np.zeros((images.shape[0], 28, 28, 3), dtype=np.float32)
for i in range(images.shape[0]):
    resized_images[i] = cv2.resize(images[i], (28, 28))
print(resized_images.shape)  # 输出 (50000, 28, 28, 3)
```

#### 2. 对于  Pytorch 小批量训练
首先我们通过 `train_dataset = CIFAR10( root='./cifar10', train=True, download=True, transform=t)  ` 得到了实际的数据集,  而实际上
`self.train_dataset.data.shape` 是 (50000, 32, 32, 3) 大小的数据集(包含 RGB 数据), 而将其转换为 28 * 28 图像只需 `transforms.Resize((28, 28)),  # 将图像缩放到 28x28`  


其中, 参考 [Flatten](https://pytorch.org/docs/stable/generated/torch.nn.Flatten.html#flatten) 部分, 如果得到的输出需要变换为一维输出，则可以在 net 中插入一个 nn.Flatten 层,  会将后面的三维展开成一维, 
```python
input = torch.randn(32, 1, 5, 5)
# With default parameters
m = nn.Flatten()
output = m(input)
output.size()  # torch.Size([32, 25])
# With non-default parameters
m = nn.Flatten(start_dim =0, end_dim=2) # 将 0-2 维展平得到 (32 * 1 * 5 , 5)的形状
output = m(input)
output.size() # (160, 5)
```


### (2) 激活函数专题
在深度学习中，激活函数（如 `ReLU`, `Softmax` 等）通常用于添加非线性变换，使得神经网络能够学习到更复杂的模式。你可以将激活函数插入到 `nn.Linear` 层之后，通常是在输出层之前或某些中间层之后进行非线性变换。

常用的激活函数如下:
#### 1. **ReLU 激活函数**
$$ \text{ReLU} (x) = \max(0,x)$$
![[Excalidraw/Pytorch & sklearn的使用基础和基本代码 2024-12-21 12.24.08|250]]
`ReLU` 是常用的激活函数，通常用于 **隐藏层**，它会对输入进行非线性转换，防止梯度消失问题。一般来说，`ReLU` 放在 `nn.Linear` 层之后。
例如，在 `nn.Linear` 之后添加 `ReLU` 激活函数的写法：

```python
self.net = nn.Sequential(
	nn.Conv2d(3, 32, kernel_size=3, padding=1),   # 输入：batch_size, 3, 32, 32
	nn.Conv2d(32, 64, kernel_size=3, padding=1),  # 输出：batch_size, 64, 32, 32
	nn.AdaptiveMaxPool2d((16, 16)),               # 输出：batch_size, 64, 16, 16
	nn.AvgPool2d(kernel_size=(2, 2), stride=2),   # 输出：batch_size, 64, 8, 8
	nn.Flatten(),                                 # 展平：batch_size, 64 * 8 * 8
	nn.Linear(64 * 8 * 8, 512),                   # 输入：batch_size, 64 * 8 * 8，输出：batch_size, 512
	nn.ReLU(),                                   # ReLU 激活函数
	nn.Linear(512, 10)                            # 输出层：10个类别
)
```

- 在 `nn.Linear(64 * 8 * 8, 512)` 后插入 `nn.ReLU()`，帮助网络学习更复杂的表示。

#### 2. **Sigmoid 激活函数**
参考[[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Chapter3 Linear Neural  Networks for regression(back Propagation)#(3) Sigmoid function|Sigmoid function]], 有公式关系:
$$\text{Sigmoid}(x) =  \frac{1}{1 + e^{-x}}$$
用于将 wx + b 部分投影到 (0,1) 之间, 基本图像如下: 
![[Excalidraw/Pytorch & sklearn的使用基础和基本代码 2024-12-21 12.07.40|250]]
直接采用如下访问:
```python
torch.sigmoid(x);
```

**什么时候使用 Sigmoid**:

- **输出层**: 如果需要概率输出（如二分类问题），`Sigmoid` 通常用于最后一层。
- **特定任务**: 在某些需要严格约束输出范围为 `[0, 1]` 的任务中，比如生成器网络（GANs）或图像处理任务中像素值的归一化。

#### 3. tanh 激活函数
$$\tanh  (x) = \frac{1 - \exp(-2x)}{1 + \exp(-2x)}  = 1 - \frac{2\exp (-2x)}{1+ \exp (-2x)}$$
其导数为:
$$\boxed{\tanh '(x) = 1 - \tanh^{2} x}$$

![[Excalidraw/Pytorch & sklearn的使用基础和基本代码 2024-12-21 12.12.08|250]]
#### 4. **Softmax 函数** 
softmax 实际上不是激活函数, 是一个归一化函数, 通常放在 **输出层**，用于多分类任务的最终输出层，以将网络的输出转换为概率分布。`Softmax` 将每个类的输出转换为 `[0, 1]` 之间的概率，并确保所有类的概率和为 1(参考[[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/👨‍🎓机器学习算法(sklearn)/补充知识/9. softmax函数和交叉熵损失CrossEntropy|softmax函数和交叉熵损失]]部分). 

在多分类问题中，`Softmax` 主要用于 **最后一层**，用于输出每个类别的概率。( 如果是二分类问题， 则采用 `Sigmoid`，或者多标签问题)  
示例 : 
但是需要注意, 当采用 CrossEntropyLoss 时,  我们不需要使用 Softmax, 因为实际上已经施加了 `log_softmax` 部分; 
```python
self.net = nn.Sequential(
	nn.Conv2d(3, 32, kernel_size=3, padding=1),   # 输入：batch_size, 3, 32, 32
	nn.Conv2d(32, 64, kernel_size=3, padding=1),  # 输出：batch_size, 64, 32, 32
	nn.AdaptiveMaxPool2d((16, 16)),               # 输出：batch_size, 64, 16, 16
	nn.AvgPool2d(kernel_size=(2, 2), stride=2),   # 输出：batch_size, 64, 8, 8
	nn.Flatten(),                                 # 展平：batch_size, 64 * 8 * 8
	nn.Linear(64 * 8 * 8, 512),                   # 输入：batch_size, 64 * 8 * 8，输出：batch_size, 512
	nn.ReLU(),                                   # ReLU 激活函数
	nn.Linear(512, 10),                           # 输出层：10个类别
	nn.Softmax(dim=1)                             # Softmax 激活函数：将输出转化为概率分布
)
```
- `Softmax` 放在 `nn.Linear(512, 10)` 后，通常会在训练时使用交叉熵损失 (`nn.CrossEntropyLoss`)，这个损失函数会自动应用 `Softmax`。
- **ReLU** 通常应用于 **隐藏层**，位于每个 `nn.Linear` 后，用于引入非线性。
- **Softmax** 通常应用于 **输出层**，用于多分类任务，将网络的输出转化为概率分布。`Softmax` 应放在最后的输出层之后。

激活函数的一般规则:
- **隐藏层：** `ReLU`, `LeakyReLU`, `Tanh`, `Sigmoid` 等（根据任务和需求选择）。
- **输出层（分类任务）：** `Softmax`（多分类问题）或 `Sigmoid`（二分类问题，或者多标签问题）。

### (3) 损失函数专题 
损失函数实际上是一种经验风险最小化的思路， 但是需要注意[[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Chapter4 Linear Neural Networks for Classification#1. Basic Concept|经验风险和实际风险的区别]]. 常见的损失函数有: 0-1损失函数，绝对值损失函数，平方损失函数，Log对数损失函数，指数损失函数，交叉熵损失函数，Hinge损失函数等等;

#### 1. MSE, MAE 损失函数
MSE : 均方误差损失函数(MSE，mean square error), 一般用于回归问题(most commonly used loss function for regression tasks.) 
$$L = \frac{1}{n} \sum^{n}_{i=1} (\hat{y}_i -y_i)^2$$
```python
nn.MSELoss()
```

**Mean Absolute Error (MAE)** 
$$\text{L1Loss} = \frac{1}{n} \sum_{i=1}^n |\hat{y}_i - y_i|$$
其调用方法是
```python
nn.L1Loss() 
```

**Huber Loss** 
对 Huber Loss 实际上是 MSE 和 MAE 的组合,   具体公式如下 : 
$$L_{\delta}(a) =
\begin{cases} 
\frac{1}{2}a^2 & \text{if } |a| \leq \delta, \\
\delta (|a| - \frac{1}{2} \delta) & \text{otherwise.}
\end{cases}$$
```python
nn.HuberLoss
``` 

#### 2. 交叉熵损失函数 
交叉熵损失函数 (常用于分类问题)
$$H(y^{(i)}, \hat{y}^{(i)}) = -\overset{q}{\underset{j = 1}{\sum}}y_j^{(i)}\log(y_j^{(i)})$$
其中, $\hat{y}_j^{(i)}$非0即1  为分类标签;

一般地, 为了防止模型过拟合， 往往会加入<b><mark style="background: transparent; color: blue">正则化参数</mark></b>， 实际上是权重损失的概念, 参考[[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/👨‍🎓机器学习算法(sklearn)/9. 正则化方法, 概率图模型和贝叶斯网络|正则化方法, 概率图模型和贝叶斯网络]] 部分;

### 3.  Logarithmic MSE 误差 
对于房价等等较大的数据, 一般我们更加关心的是相对于原始价格的准确程度, 
$$\sqrt{\frac{1}{n} \sum_{i = 1}^{n} (\log_{} y_{i}  - \log_{} \hat{y}_{i})^{2}}$$
具体参考 [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Chapter5 Essential Concepts & Heuristics#(1) Logarithmic MSE Loss|Logarithmic MSE Loss]] 部分

Log-Cosh 误差, 可以采用:
$$\text{Log-Cosh}(x) = \sum_{i=1}^{n} \log(\cosh(\hat{y}_{i} - y_{i}))$$

### 验证集和k折交叉验证
- 方法是将训练数据集分割成k个不同的子数据集，每一次使用1个数据集验证，k-1个训练 
- 做k次模型训练和验证，并将k次的训练误差和验证误差分别求平均值
参考  [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Chapter5 Essential Concepts & Heuristics#(2) K-fold Cross-Validation|K-fold Cross-Validation]] 部分.


### 定义训练的层 
需要说明,  Pytorch  中, 支持自定义不同的层, 仍然可以采用 nn.Module 进行定义; 仍然需要继承 nn.Module 类型,  并且重写 forward 方法; 

forward 必须采用 `def forward(self, X):`  为定义, 如果有 forward, 则可以视为一层;

```python
import torch
import torch.nn as nn

class EpochDropout(nn.Module):
    def __init__(self, p=0.5):
        super(EpochDropout, self).__init__()
        self.p = p
        self.mask = None

    def forward(self, x):
        if self.training:
            # 如果 mask 为空，则生成 mask
            if self.mask is None or self.mask.shape != x.shape:
                self.mask = torch.bernoulli(torch.full(x.shape, 1 - self.p)).to(x.device) / (1 - self.p)
            return x * self.mask
        else:
            return x  # 测试阶段不使用 Dropout

    def reset_mask(self):
        """在每个 epoch 开始时重置 mask"""
        self.mask = None
```

```python
self.dropout = EpochDropout(p=0.5)
# 或者直接添加到 net 中 
```


### Batch Norm  
Add `nn.BatchNorm2d` after convolutional layers to stabilize learning and improve convergence speed.

`nn.BatchNorm2d` 是一种常见的神经网络层normalization技术,主要用于卷积神经网络中。它通常被添加在卷积层之后,起到以下作用:

1. **加速训练收敛**:
   - 通过标准化输入数据,可以有效缓解内部协变移移(internal covariate shift)的问题,加快模型的训练收敛速度。

2. **提高泛化能力**:
   - BatchNorm能够使得模型对输入数据的分布变化更加鲁棒,从而提高模型的泛化性能。

3. **降低对初始化的依赖**:
   - 由于BatchNorm能够对数据进行动态标准化,因此模型的训练过程对初始化参数的依赖性大大降低。

4. **防止梯度消失/爆炸**:
   - BatchNorm能够有效地缓解梯度消失或爆炸的问题,为网络的训练提供更加稳定的环境。

**一般情况下, `nn.BatchNorm2d` 层会被添加在卷积层和激活函数层之间, 这样可以在特征提取和特征变换之后对特征进行归一化**,从而获得更加稳定和鲁棒的特征表示。


## 常用的层部分
nn.GroupNorm,  nn.BatchNorm1d, nn.BatchNorm2d  等等

**此外, `nn.BatchNorm2d` 还有一些其他的变体,如 `nn.BatchNorm1d` 用于全连接层, `nn.InstanceNorm2d` 用于图像生成等任务**。实际应用中需要根据具体问题和网络结构来选择合适的normalization技术。

 `@torch.no_grad()` 提供了将整个函数不进行梯度的控制修饰器。

ResNet 


有一些比 `AdaptiveMaxPool2d` 更加优越的池化方法,具体取决于实际问题和模型需求。以下是一些对比和建议:

1. **GlobalAveragePooling2d**:
   - 这种方法将特征图上每个通道的特征值取平均,得到一个全局特征向量。
   - 相比 `AdaptiveMaxPool2d`，GlobalAveragePooling2d 能够更好地保留特征信息,不会丢失重要的特征。
   - 在一些任务上,如图像分类,GlobalAveragePooling2d 表现更优。

2. **Spatial Pyramid Pooling (SPP)**:
   - SPP 利用多尺度的池化操作,能够更好地捕捉不同尺度的特征信息。
   - 相比固定大小的池化,SPP 能够适应不同大小的输入,提高模型的鲁棒性。
   - SPP 模块可以作为卷积网络的一部分,与其他层一起端到端训练。

3. **Dilated Convolution**:
   - Dilated Convolution 可以在不降低分辨率的情况下扩大感受野,捕获更丰富的上下文信息。
   - 相比池化,Dilated Convolution 不会丢失特征信息,能够更好地保留原始的语义特征。

关于您提到的GPU足够的情况下,是否应该使用池化层:

- 如果 GPU 资源足够充裕,一般来说不使用池化层训练效果会更好。这是因为池化会丢失一些有价值的特征信息。
- 但如果GPU资源受限,使用池化层可以有效降低计算量和内存占用,从而能够训练更深更大的模型。这种情况下,池化的优势可能会大于其带来的信息损失。

综合考虑,如果GPU资源充足,可以尝试使用 GlobalAveragePooling2d 或 Dilated Convolution 等更高级的池化/感受野扩张技术。同时也要根据具体任务和模型需求进行实验验证,找到最合适的方案。


### GPU 设备数据传输 
一般采用 `.cuda()` , `.cpu` , `.to(self.device)`  和 `device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')`  结合使用进行 gpu 数据移动。

对于多 GPU 训练的模型， 可能涉及数据不同 GPU 之间的传递 
```python
print(torch.cuda.device_count())

num = 1
data.to(torch.device(f'cuda:{num}'))   # 将数据放在某个gpu 上
```

### 保存和加载模型 
一般对于任意 nn.Module 模块， 可以通过存储 state_dict 获取其中所有参数信息。 
```python
torch.save(self.model.state_dict(), 'cnn_models.pth')
```

然后采用 load_state_dict 得到原始模型参数
```python
model = EasyModule()
model.load_state_dict(torch.load('cnn_models.pth')) 
```



##  模型显示方法: 
一般对于  nn.Module 类型继承的模型， 可以直接使用 
`print(model)` 打印模型输入和输出部分， 

或者 module.named_parameters() 打印
```python
for name, param in model.named_parameters():
    print(f"Layer: {name}, Size: {param.size()}")
```

此外可以采用 `torchsummary`  库获取更加详细的模型结构. 
```python
from torchsummary import summary

summary(model, input_size=(10,))
```

### Lazy 网络 

> [!HINT] 为何采用 Lazy 网络
> 常用的 Lazy 网络包括 LazyLinear,  LazyConv2d 等, 其输入数量是根据前一层的输出确定的，因此可以灵活处理不同形状的输入数据， 因此能够更好地迁移到不同的数据集上来  
