## 一、Kriging 代理模型基本原理
基于 Kriging 自适应代理模型的可靠性学习函数; 可以将 Monte Carlo 方法**求得的失效概率样本中的少量样本, 代入功能函数中得到相应功能的样本值**;
通过利用输入-输出模型，建立 Kriging 代理模型, 可以<mark style="background: transparent; color: red">通过U等学习函数在备选样本池中挑选出下一步所需的更新样本点， 从而对当前的Kriging代理模型实现相应的更新</mark>; 直到**自适应学习过程收敛**; 

对于自适应的 Kriging 代理模型, 可以在<mark style="background: transparent; color: red">满足给定精度的要求下准确判断样本的失效或者安全状态</mark>， 从而代替原始的功能函数计算局部可靠性与可靠性灵敏度; 

对于Kirging 代理模型， 可以直接和有限元模型进行结合来实现可靠性以及可靠性局部灵敏度分析; 

## 二、Kriging 代理模型的主要内容
工程领域常用的 Kriging 代理模型包括多项式响应面模型， 人工神经网络模型和 Kriging 代理模型等等; 
其中， 多项式响应面模型难以准确拟合高维和强非线性问题； 而人工神经网络模型需要的试验次数过多, 因此我们呢万钢使用 kriging 模型作为<mark style="background: transparent; color: red">方差最小的模型无偏估计</mark>, 具有<mark style="background: transparent; color: red">全局近似和局部误差相结合的特点，且其有效性不依赖于随机误差的存在</mark>, 从而能够取得<mark style="background: transparent; color: red">对于强非线性和局部响应突变函数的良好拟合效果</mark>; 

Kriging 模型可以近似为**一个随机分布函数和一个多项式之和**:
$$g_{K} (X) = \sum^{P}_{i=1}f_{i}(X) \beta_{i} + z(X)$$
其中$[f_1(x), f_2(x), \dots f_n(x)]$ 为随机向量$X$的基函数, 可以提供空间内的全局近似模型; 而$\beta$ 为回归待定系数; 而$z(x)$是随机过程, 可以用于创建期望为0且方差为$\sigma^2$的局部偏差; 而对应的协方差矩阵可以表示为: 
$$\text{cov} \left[ z(x^{(i)}, x^{(j)})\right] = \sigma^{2} [R(x^{(i)}, x^{(i)})]$$
其中, R表示相关矩阵， 而R
