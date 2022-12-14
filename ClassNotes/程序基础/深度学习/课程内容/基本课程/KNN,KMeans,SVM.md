- 要求： 类内距离小，类间距离大
- 聚类的目标是未知的

一、KNN算法（识别，分类算法）
1. 求解数据点到其余各个点的距离
2. 求解对应的最邻近的K个点
3. 判断其中最多的点的类别

二、K-Means算法（K均值算法）
1. 随机取K个点作为初始的聚类中心，代表各个聚类(一般选取数据点(注意是不同的数据点)，有时随机)
2. 计算求解各个样本相对于各个数据的距离。并将点归类到与其距离最近的点
3. 重新求解新类别的均值，再次调整中心位置
4. 重新求解，迭代至中心点近似不动为止

- K-Means算法没有训练样本数据，因此为无监督学习
- 影响: 初始中心，输入数据K的选择，距离的度量
- 改进：二分K-Means算法--> 避免收敛于局部的问题   划分的标准是最大限度降低SE值

三、支持向量机(Suppported Vector Machine, SVM)
	 十大数据挖掘算法
	 ![[Pasted image 20221115195839.png|300]]
支持向量机属于十大数据挖掘算法之一，属于无监督式的学习算法

线性分类机
![[Pasted image 20221115200507.png|300]]

![[Pasted image 20221115201124.png]]
1. 方法: 
	1. 使用Logistic函数或者Sigmoid函数，将数据映射到（0,1）上
	2. Logistic回归的目的是学习出一个（0,1）分类模型
	3. 通过Logistic函数映射，将自变量映射到（0,1）上，映射后的值被认为是属于 y = 1 的概率。

2. 判别公式: 

$$\begin{matrix} 
P(y=1,x;\theta) = h_\theta(x) \\
P(y=0,x;\theta) = 1 - h_\theta(x)
\end{matrix}$$
其中，$h_\theta(x)$为映射函数, 有
$$h_\theta(x) = g(\theta^Tx) = \frac{1}{1+e^{-\theta^Tx}}$$
其中，$h_\theta(x)$仅和$\theta^T x$有关，当$\theta^T x>0$时，$h_\theta(x) > 0.5$

Logistic回归模型目标是学习得到$\theta$使得正例的特征$\theta^T x$远大于0,负例的特征远小于0，则得到的大于0和小于0的部分可以进行分类(注意在SVM中，是使用-1和1来作为标签的)

![[Pasted image 20221121234540.png|500]]
一、最大边缘超平面(Maximal Margin Hyperplane)

![[Pasted image 20221121204905.png|300]]
在n维的数据空间中寻找一个超平面(Hyperplane)
对于如图的超平面，$B_1,B_2$每个$B_i$都对应着一对平行的平面
则$B_1$是两个训练样本中最大边缘的一个（追求结构风险最小化）
![[Pasted image 20221121205137.png|500]]
![[Pasted image 20221121232109.png|400]]

支持向量机分为
- 线性支持向量机 
- 非线性支持向量机 - 核方法

![[Pasted image 20221121232346.png|500]]
对于超平面$$f(x) = w^T x + b$$
其中$x$是n维特征向量。
其决策边界可以写成：
$$w^T x + b = 0$$
其中$w = (w_1;w_2;....;w_m)$为垂直于超平面的法向量,$b$为位移量
其中$w$决定决策边界方向，$b$决定决策边界与原点之间的距离
决策边界由参数$\omega,b$确定

[超平面方程的由来详解](https://blog.csdn.net/dengheCSDN/article/details/77313758)
[[KNN,KMeans,SVM 2022-11-22 00.25.42.excalidraw]]

![[Pasted image 20221121233127.png|400]]
![[Pasted image 20221121233616.png|400]]
[[KNN,KMeans,SVM 2022-11-22 00.41.02.excalidraw]

<a href="https://blog.csdn.net/v_JULY_v/article/details/7624837">
支持向量机通俗导论
</a>

- 支持向量机去求解二类问题，目标是求一个特征空间的超平面，而超平面分开的两类对应于超平面的函数值的符号是刚好相反的；基于上述两种考虑，为了使问题足够简单，我们取y的值为1和-1

[[KNN,KMeans,SVM 2022-11-22 01.04.05.excalidraw]]

因此，定义函数间隔$$\hat{\gamma} = y(w^T x + b) = yf(x)$$
则超平面$(w,b)$关于训练数据集T的间隔：
$$\hat{\gamma} = min \hat{\gamma_i}$$
即定义为最近的两点之间的平面距离
线性支持向量机的学习是寻找满足约束条件的参数$\omega$和$b$，使得获取最大化的超平面距离,即几何距离
![[Pasted image 20221122012636.png|300]]
这是一个凸二次优化问题，可以直接使用现成的优化计算包进行求解，或者使用拉格朗日乘子法求解
![[Pasted image 20221122012752.png|400]]
![[Pasted image 20221122012922.png|400]]
![[Pasted image 20221122013615.png|400]]
![[Pasted image 20221122014716.png|400]]
![[Pasted image 20221122015602.png|400]]

三、非线性支持向量机

![[Pasted image 20221122015945.png|400]]
通过核函数kernal来推广到非线性的情况:

![[KNN,KMeans,SVM 2022-11-22 02.30.07.excalidraw]]

有几个常用的核函数kernal:
1. 多项式核

![[Pasted image 20221122025829.png|600]]
![[Pasted image 20221122025940.png|300]]
![[Pasted image 20221122025956.png|300]]
![[Pasted image 20221122030018.png|300]]
![[Pasted image 20221122030644.png|300]]
![[Pasted image 20221122030731.png|300]]
![[Pasted image 20221122030801.png|500]]
![[Pasted image 20221122030826.png|500]]



