## 一、Kriging 代理模型基本原理
基于 Kriging 自适应代理模型的可靠性学习函数; 可以将 Monte Carlo 方法**求得的失效概率样本中的少量样本, 代入功能函数中得到相应功能的样本值**;
通过利用输入-输出模型，建立 Kriging 代理模型, 可以<mark style="background: transparent; color: red">通过U等学习函数在备选样本池中挑选出下一步所需的更新样本点， 从而对当前的Kriging代理模型实现相应的更新</mark>; 直到**自适应学习过程收敛**; 

对于自适应的 Kriging 代理模型, 可以在<mark style="background: transparent; color: red">满足给定精度的要求下准确判断样本的失效或者安全状态</mark>， 从而代替原始的功能函数计算局部可靠性与可靠性灵敏度; 

对于Kirging 代理模型， 可以直接和有限元模型进行结合来实现可靠性以及可靠性局部灵敏度分析; 

> [!CAUTION] 变异系数的定义
> 变异系数实际上是均值和标准差的比值,  因此可以**通过均值 * 变异系数获取每个数据的标准差**

## 二、Kriging 代理模型的主要内容
### (1) Kriging 模型的建立与一般公式
工程领域常用的 Kriging 代理模型包括多项式响应面模型， 人工神经网络模型和 Kriging 代理模型等等; 
其中， 多项式响应面模型难以准确拟合高维和强非线性问题； 而人工神经网络模型需要的试验次数过多, 因此我们呢万钢使用 kriging 模型作为<mark style="background: transparent; color: red">方差最小的模型无偏估计</mark>, 具有<mark style="background: transparent; color: red">全局近似和局部误差相结合的特点，且其有效性不依赖于随机误差的存在</mark>, 从而能够取得<mark style="background: transparent; color: red">对于强非线性和局部响应突变函数的良好拟合效果</mark>; 

Kriging 模型可以近似为**一个随机分布函数和一个多项式之和**, 即**使用多重线性模型拟合对应的模型值， 与神经网络中的函数类似**; 
$$\boxed{g_{K} (X) = \sum^{P}_{i=1}f_{i}(X) \beta_{i} + z(X)}\tag{7.2.1}$$
其中$[f_1(x), f_2(x), \dots f_n(x)]$ 为随机向量$X$的基函数, 可以提供空间内的全局近似模型; 而$\beta$ 为回归待定系数; **而$z(x)$是随机过程**, 可以用于创建期望为0且方差为$\sigma^2$的局部偏差; 而对应的协方差矩阵可以表示为: 
$$\text{cov} \left[ z(x^{(i)}, x^{(j)})\right] = \sigma^{2} \boldsymbol{R}[R(x^{(i)}, x^{(i)})]$$
其中, $\boldsymbol{R}$**表示相关矩阵**， 而R为任意两个样本点的相关函数; 而 $i,j = 1,2, \dots m$, 其中$m$为训练样本集中的数据个数, 其中$R(x^{(i)}, x^{(j)})$可以选择多种模式，**我们以高斯型相关函数的表达式为例**:
$$R(x^{(i)}, x^{(j)}) = \exp \left( - \sum^{m}_{i=1} \theta_{k}|x_{k}^{(i)} -  x_{k}^{(j)}|^{2}\right)$$
其中, $\theta_{k}(k = 1, 2, \dots m)$为**相关的未知参数**; 

> [!NOTE] 说明
> 在DACE库中， R是一个含有参数$\theta, w, x$的矩阵,  即有
> $$E[z_{l}(w) z_{l}(x)] =  \sigma_{l}^{2} \mathcal{R}(\theta, w, x)$$
> 其中预测的协方差在$z(w), z(x)$之间;
> 但是实际值可以写成:
> $$y_l(x) = \mathcal{F}(\beta_{,l}, x) + \alpha(\beta_{:,l}, x)$$
> 其中 $\alpha$ 是近似误差

另外， 根据Kriging理论, **未知点$x$处的响应估计值**可以通过下式得到, 即将式(7.2.1中的**随机过程$z$函数部分**换成右侧的$r, R^{-1}$相乘的部分:) 
我们使用 $l$ 表示**单位列向量**, 而$l^{T}$为**单位行向量**,  则:
$$\Large\boxed{g_{K} (x) = f(x) \hat{\beta} + r^{T}(x) R^{-1} ( g  - l \hat{\beta})}\tag{7.2.2}$$
其中, $\hat{\beta}$为$\beta$的估计值, 而g为**训练样本数据得到的响应值构成的列向量**，而$r(x)$为**样本点与预测点之间的相关函数向量**， 并可以表达为:
$$r^{T}(x) = (R(x, x^{(1)}),  R(x, x^{(2)}), \dots , R(x, x^{(m)}))$$
并有估计值$\hat{\beta}$ 以及方差估计值$\hat{\sigma}^{2}$的计算方法:
$$\hat{\beta} = (l^{T} R^{-1} l)^{-1} l^{T}R^{-1} g$$
$$\hat{\sigma}^{2} = (g - l\hat{\beta})^{T} R^{-1} (g - l\hat{\beta})/m$$
而相关参数$\theta$可通过求解**极大似然估计的最大值进行获取**， 因此有: 
$$\boxed{\max F(\theta) = - \frac{m \ln (\hat{\sigma}^{2}) + \ln |\boldsymbol{R}|}{2}\qquad \theta_{k}\geq 0 (k = 1,2 \dots m)}\tag{7.2.3}$$
上式是**求解Kriging代理模型的方程式**, 而只需经故上式进行求解，得到的Kriging 模型即为拟合精度最优的代理模型;

我们考虑一个未知的输入量$x$与响应值$g(x)$，且响应值$g_K(x)$服从高斯分布， 则有$g_{k}(x)\sim N(\mu_{gk} (x), \sigma_{gk}^{2}(x))$, 则均值和方差的计算公式如下: 
$$\mu_{gk}(x)=l^{T}(x) \hat{\beta} + r^{T}(x) R^{-1}  (g - l\hat{\beta})$$
$$\sigma_{gk}^{2}(x) =\sigma^{2}\left\{1 - r^{T}(x) R^{-1} r(x) + [l^{T}R^{-1} r(x) -l(x)]^{T} (l^{T}R^{-1}l)^{-1} \left[l^{T}R^{-1}r(x) - l(x) \right] \right\}$$
其中， 对于$\mu_{gk}$和$\sigma_{gk}$的计算，MATLAB中有相应的工具箱(DACE)进行对应的求解; 

需要说明， Kriging 代理模型是一种**准确插值方法**(即必定在每个样本点处取得对应的值，或者$\mu_{gk}(x_{i}) = g(x_{i}), \sigma^{2}_{gk}(x_{i}) = 0)$, ==此时, $\sigma_{gk}^{2}(x)$ 表示$g_K(x)$和$g (x)$均方误差的最小值, 此时其他输入变量样本对应的功能函数预测值的方差一般不是0, 从而可以使用$\sigma_{gk}^{2}(x)$来评估代理模型在x为之的估计准确程度; 从而为更新Kriging 代理模型提供一个更一般的指标==; 


> [!caution] 说明
> 需要注意的是， 当某处的$\sigma_{gk} (x)$比较大时， 说明在$x$处的估计是不正确的，因此往往在此处增加点进行进一步求解; 

### (2) Kriging 代理模型的自适应学习函数方法与特性
首先,  对于Kriging 代理模型， 要求对于失效概率g(x) = 0 点的预估， 即利用代理模型建立准确的代理功能函数零的面更为重要。
自适应的主要过程是，首先根据少量样本点建立粗糙的Kriging代理模型，而**通过自适应学习函数从备选样本即中获取==符合要求的样本点==加入当前的样本训练集中**。最后更新Kriging代理模型直到满足收敛条件;

我们需要满足下列的自适应代理模型:
1. 可以取**随机输入变量分布密度较大的区域**进行增加 
2. **距离功能函数为0的面近,  即靠近极限状态面近,  符号误判的风险较大的部分**进行增加样本点, 这些样本点的特征是$\mu_{gx}$较小， 而对应的预测方差$\sigma_{gx}^{2}$较大;
应用广泛的学习函数包括 **EFF(Expected feasibility Function)学习函数**, **基于信息熵的H学习函数**以及**U学习函数** 

## 三、不同种类的 Kriging 学习函数的基本形式
### (1) EFF 学习函数
对于EFF学习函数, 其定义入下: 
$$EFF(x) = \int_{\overline{g}(x) - \varepsilon}^{\overline{g}(x) + \varepsilon} (\varepsilon - \left|\overline{g}(x) - g_{K}(x) \right|) f_{gk}( g_{k}(x)) dg_{k}(x)$$
其中$f_{gk}$为Kriging 模型中的$g_K(x)$的概率密度函数， 且**服从均值为$\mu_{gk} (x)$, 标准差为$\sigma_{gk}(x)$的正态分布**; 其中$\overline{g}(x) = 0$为失效边界; **而$\varepsilon$与$\sigma_{gx}(x)$成比例**; 一般取比例为$2\sigma_{gk}$我们可以**取g的左右部分分别为**:
$$g^{-}(x) = \overline{g}(x) - \varepsilon, \qquad g^{+}(x) = \overline{g}(x) + \varepsilon$$
可以得到公式:
![[attachments/Pasted image 20240301143014.png|700]]
### (2) H 学习函数
我们考虑**Shannon信息熵理论**， 由于$g_{k}(x)$的信息熵可以表示为:
$$h(x) = - \int f_{gk}(g_{K}(x)) \ln \left[ f_{gk} (g_{K}(x)) \right]dg_{K}(x)$$
其中, $h(x)$为$g_K(x)$的混乱等级, 且其定量确定了$g_{K}(x)$的不确定性; 对于**信息熵** $h(x)$ 当**其绝对值越小时, 对应预测值$g_{K}$的不确定性也越小**; 因而**直接将学习函数定义为不确定性**:
$$H(x) = |h(x)| = \left|  \int_{g^{-}}^{g^{+}}  f_{gk}(g_{K}(x))\ln \left[ f_{gk} (g_{K}(x)) \right]dg_{K}(x)\right|$$
其中上下限分别为:$g^{+} = 2 \sigma_{gk}(x), g^{-} = -2 \sigma_{gk}(x)$
上述公式成为:
![[attachments/Pasted image 20240301144032.png]]

### (3) U 学习函数
U学习函数定义为:
$$U(x) = \left| \frac{\mu_{gk}(x)}{\sigma_{gk}(x)}\right|$$
其中$\mu_{gk}(x), \sigma_{gk}(x)$两项分别为Kriging 代理模型的均值和标准差; 

其学习原理是，当估计值相同时， 估计值的**标准差越大， 则学习函数函数的值越小**。因此**每一次可以选择学习函数U值较小的点加入**到点集中， 从而实现Kriging 代理模型的自适应更新过程; 

## 四、可靠性与可靠性局部灵敏度分析的AK-MCS方法
### (1) 可靠性分析的AK-MCS方法 
AK-MCS 实际上是自适应 Kriging 代理模型结合 Monte Carlo 数字模拟的失效概率求解方案; 
1. 获取整体的 Monte Carlo 样本点$S_{MC}$ 假设其中样本点的个数为$N_{MC}$, 供更新使用; 
2. 产生训练的初始样本点，建立Kriging 代理模型， 假设选取了$N_1$ 个输变量的样本，并计算响应的代理模型函数值，  从而形成初始训练集$T_{MC}$;
3. 根据当前的$T_{MC}$建立Kriging代理模型$g_K(x)$; 依次迭代选择新样本点，即**每次将学习函数最小的部分加入**, 然后**判断收敛性**; 需要注意: 相关模型采用 Gaussian 过程模型, 而回归模型采用常数,  模型即为普通Kriging代理模型; 
4. 通过建立的代理模型估计失效概率, 
$$\hat{P}_{f} = \frac{N_{gK} \leq  0}{N_{MC}}$$
失效概率的变异系数的估计值为: 
$$\text{cov}(\hat{P}_{f})  = \sqrt{\frac{1 - \hat{P}_{f}}{(N_{MC} - 1) P_{f}}}$$
其中， 当**变异系数$\text{cov}(\hat{P}_{f})< 5\%$时，认为估计可以接受**; 如果不能接受， 则考虑更新Monte Carlo样本池; 方法一是扩充， 方法二是重新选择; 

### (2) 可靠性局部灵敏度分析的AK-MCS方法 
首先，参考[[📘ClassNotes/👨‍🔧Mechanics/🖥️Computational_Mechanics/✈️航空发动机可靠性分析/第二章 蒙特卡洛模拟方法|蒙特卡洛模拟方法]], 局部灵敏度可以定义为:
$$\frac{\partial P_{f}}{\partial \theta_{Xi}^{k}} = \int\dots\int _{F} \frac{\partial f_{X}}{\partial \theta_{X_{i}}^{(k)}} \frac{1}{f_{X}(x)} f_{X} (x) dx = \int\dots\int _{R^{n}} I_{F}(x) \frac{\partial f_{X}}{\partial \theta_{X_{i}}^{(k)}} \frac{1}{f_{X}(x)} f_{X} (x) dx$$
我们将上式使用均值表示， 公式为: 
$$\frac{\partial P_{f}}{\partial \theta_{X_{i}}} =E\left[ \frac{I_{F} (x)}{f_{X}(x)} \frac{\partial f_{X}(x)}{\partial \theta_{X_{i}}^{(k)}} \right]$$
其中, 求解可靠性灵敏度**需要利用功能函数计算的只是失效域指示函数** $I_F(x)$, **可以采用AK-MCS方法计算失效概率时创建的Kriging模型**; 同时, 也可以利用AK-MCS方法求解局部灵敏度估值的变异系数。 

而**输入变量为独立正态分布的情况**时, 有公式:
$$\frac{1}{f_{X}(x)}\frac{\partial f_{X}(x)}{\partial \mu_{X_{i}}} = \frac{1}{f_{X_{i}} (x_{i})} \frac{\partial f_{X_{i}}}{\partial \theta_{X_{i}}} = \frac{(x_{i} - \mu_{X_{i}})}{\sigma_{X_{i}}^{2} }$$
$$\frac{1}{f_{X}(x)}\frac{\partial f_{X}(x)}{\partial \sigma_{X_{f}}} = \frac{1}{f_{X_{i}}} \frac{\partial f_{X_{i}}(x_{i})}{\partial \theta_{X_{i}}^{(k)}} = \frac{(x_{i} - \mu_{X_{i}})^{2}}{\sigma_{X_{i}}^{3}} - \frac{1}{\sigma_{X_{i}}}$$
而对于基本过程, AK-MCS的局部灵敏度分析只需在迭代完成之后计算对应的$I_F$以及可靠性局部灵敏度即可;

MATLAB 中的 Kriging 工具箱使用参考 https://zhuanlan.zhihu.com/p/612582292
而DACE工具箱文档参考 https://www.omicron.dk/dace/dace.pdf 
```erlang title:dacefit函数的使用格式
[dmodel, perf] = dacefit(X, y, regr, corr, theta0, lob, upb) 
% 其中：S为输入变量，Y为输出变量，regr为回归函数，包含regpoly0、regpoly1、regpoly2三种函数形式，corr为相关函数，包括corrcubic、correxp、correxpg、corrgauss、corrlin、corrspherical、corrspline共7种形式，theta0为输入theta的初值，可任意赋值, lob为theta的下限值，upb为theta的上限值
``` 

```erlang
function  [dmodel, perf] = dacefit(S, Y, regr, corr, theta0, lob, upb)
%DACEFIT Constrained non-linear least-squares fit of a given correlation
% model to the provided data set and regression model
%
% Call
%   [dmodel, perf] = dacefit(S, Y, regr, corr, theta0)
%   [dmodel, perf] = dacefit(S, Y, regr, corr, theta0, lob, upb)
% 
% Input
% S, Y    : Data points (S(i,:), Y(i,:)), i = 1,...,m
% regr    : Function handle to a regression model
% corr    : Function handle to a correlation function
% theta0  : Initial guess on theta, the correlation function parameters
% lob,upb : If present, then lower and upper bounds on theta
%           Otherwise, theta0 is used for theta
%
% Output
% dmodel  : DACE model: a struct with the elements
%    regr   : function handle to the regression model
%    corr   : function handle to the correlation function
%    theta  : correlation function parameters
%    beta   : generalized least squares estimate
%    gamma  : correlation factors
%    sigma2 : maximum likelihood estimate of the process variance
%    S      : scaled design sites
%    Ssc    : scaling factors for design arguments
%    Ysc    : scaling factors for design ordinates
%    C      : Cholesky factor of correlation matrix
%    Ft     : Decorrelated regression matrix
%    G      : From QR factorization: Ft = Q*G' .
% perf    : struct with performance information. Elements
%    nv     : Number of evaluations of objective function
%    perf   : (q+2)*nv array, where q is the number of elements 
%             in theta, and the columns hold current values of
%                 [theta;  psi(theta);  type]
%             |type| = 1, 2 or 3, indicate 'start', 'explore' or 'move'
%             A negative value for type indicates an uphill step
```

Cov_f_Kir =  
  
0.0083
  
P_f_Kir =  
  
0.0460