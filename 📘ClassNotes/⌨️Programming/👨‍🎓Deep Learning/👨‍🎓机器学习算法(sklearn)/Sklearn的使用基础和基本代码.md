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
