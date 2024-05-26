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
其中， 多项式响应面模型难以准确拟合高维和强非线性问题； 而人工神经网络模型需要的试验次数过多, 因此我们往往使用 kriging 模型作为<mark style="background: transparent; color: red">方差最小的模型无偏估计</mark>, 具有<mark style="background: transparent; color: red">全局近似和局部误差相结合的特点，且其有效性不依赖于随机误差的存在</mark>, 从而能够取得<mark style="background: transparent; color: red">对于强非线性和局部响应突变函数的良好拟合效果</mark>;

Kriging 模型可以近似为**一个随机分布函数和一个多项式之和**, 即**使用多重线性模型拟合对应的模型值， 与神经网络中的函数类似**, 其中 $X = (x_{1}, x_{2}, \dots x_{n})^{T}$ 
$$\boxed{g_{K} (X) = \sum^{P}_{i=1}f_{i}(X) \mu_{i} + z(X)}\tag{7.2.1}$$
其中$f(X) = [f_1(x), f_2(x), \dots f_n(x)]$ 为随机向量$X$的基函数, 可以提供空间内的全局近似模型; 而$\mu_{i}$为**回归待定系数**, 实际上是一个中间变量; 
> [!NOTE] $z(x)$ 的表达
>  由于实际上的预测值应该是前面的一部分， **$z(x)$是一附加在该预测值上的随机过程**, 可以用于创建期望为0且方差为$\sigma^{2}$的局部偏差; 
>  其中可以==取$z$的协方差矩阵为样本的归一化相关系数矩阵==， 即有:
>  $$\text{Cov}[z(x_{i}, z(x_{j}))] = \sigma^{2} R(x_{i}, x_{j})$$
>  其中 $R(x_{i}, x_{j})$ 为样本的相关系数矩阵, $\sigma^{2}$ 为方差的预测值(实际上可以使用下面的$s^2$来代替)， 而在实际预测中， 我们使用条件概率最大时对应的值为相应的预测值(对应参考文献中的$\partial L = 0$)

> [!hint] 归一化相关系数矩阵引入
> 变量标准正态化后的相关系数矩阵

我们设<mark style="background: transparent; color: red">样本的协方差矩阵为</mark>$R$, 参考[[📘ClassNotes/📐Mathmatics/🎣Probability Theory/第四章 随机变量的数字特征#(4) 多维正态分布随机变量的概率密度以及协方差矩阵|第四章 随机变量的数字特征]]得到:
$$\boldsymbol{R}_{i,j} = \text{cov} \left[ z(x^{(i)}, x^{(j)})\right]$$
其中,$\sigma$是样本方差(每一个都是相同的), $\boldsymbol{R}$**表示相关矩阵**, 并且有**高斯型相关函数的表达式** (也可以选择其他多种模式)为: 
$$R(x^{(i)}, x^{(j)}) = \exp \left( - \sum^{m}_{i=1} \theta_{k}|x_{k}^{(i)} -  x_{k}^{(j)}|^{2}\right)$$
其中$\theta_{k}(k = 1, 2, \dots m)$为**相关的未知参数**(类似于相关系数);
我们参考[多元高斯概率分布函数的推导与理解](https://zhuanlan.zhihu.com/p/523214705)以及[克里金(Kriging)模型详细推导](https://zhuanlan.zhihu.com/p/377620800), 则得到预测的方法:
其中, 预测点的$g_{K}$估计值为:
$$\Large \boxed{\begin{cases}
{g}_{K}^{pre}(x) = f(x)^{T}\hat{\mu} +r^{T} R^{-1}(\hat{g} - l \hat{\mu})  \\
s_{K}^{2(pre)}(x) = \hat{\sigma}^{2} [1 - r^{T}C^{-1}r + \frac{(1- l^{T}C^{-1} r)^{2}}{l^{T} R^{-1}l}]
\end{cases}}$$
其中$f(x)^{T}$为回归模型，可以选择 0, 1, 2 次多项式。而此处的$\hat{\mu}$实际上是7.2.1中$\mu$的估计值, $l$为单位列向量， $\hat{g}$为样本响应数据构成的列向量 

> [!NOTE] 补充
> 实际上对于任意一个未知的$x$, $g_{K}(x)$预测值服从一个均值为$g_{K}^{pre}(x)$，方差为$s_{K}^{2(pre)}$的高斯分布。

而$\boldsymbol{r} = \boldsymbol{r}(x)$ 为<mark style="background: transparent; color: red">训练样本点和预测点之间的相关函数向量</mark>, 是一个列向量(表示了目标点与所有样本点之间的相关性)
$$r(x_{i}) = (R(x_{i}, x^{(1)}, R(x_{i}, x^{(2)}),  R (x_{i} , x^{(3)}), \dots R(x_{i}, x^{(m)}))$$
其中, 估计值$\hat{\mu}$ 以及方差估计值$\hat{\sigma}^{2}$的计算可以通过下式 : 
$$\Large\begin{cases}
\hat{\mu} = (l^{T} R^{-1} l)^{-1} l^{T}R^{-1} g \\ 
\hat{\sigma}^{2} = \frac{(g - l\hat{\mu})^{T} R^{-1}(g - l\hat{\mu}) }{m}
\end{cases}$$
而相关参数$\theta$可通过求解**极大似然估计的最大值进行获取**， 即有公式为 : 
$$\boxed{\max F(\theta) = \ln L = - \frac{m \ln (\hat{\sigma}^{2}) + \ln |\boldsymbol{R}|}{2}\qquad \theta_{k}\geq 0 (k = 1,2 \dots m)}\tag{7.2.3}$$
这个式子对应 $\ln L$, 其中$L$为概率密度$p$ , 超参数 $\theta$ 的选择是使得上式最大的 $\theta$ 值。
上式是**求解Kriging代理模型的方程式**, 而只需经故上式进行求解，得到的Kriging 模型即为拟合精度最优的代理模型;

另外,此时$\hat{s}_{k}^{2}(x)$ 表示$g_K(x)$和$g (x)$均方误差的最小值, 此时其他输入变量样本对应的功能函数预测值的方差一般不是0, 因而 $\hat{s}^{2(pre)}_{K}$的大小可以用于评估代理模型对$x$的估计准确程度; 从而为更新Kriging 代理模型提供一个更一般的指标; 当某处的$\hat{s}_{k}^{2(pre)} (x)$比较大时， 说明在$x$处的估计是不正确的，因此往往在此处增加点进行进一步求解;
> [!caution] 说明
> Kriging 代理模型是一种**准确插值方法**(即必定在每个样本点处取得对应的值，另外需要说明的是, Kriging模型预测点使用的实际上是使用最大似然函数进行点估计的。

> [!NOTE] 说明
> 在DACE库中， R是一个含有参数$\theta, w, x$的矩阵,  即有
> $$E[z_{l}(w) z_{l}(x)] =  \sigma_{l}^{2} \mathcal{R}(\theta, w, x)$$
> 其中预测的协方差在$z(w), z(x)$之间;
> 但是实际值可以写成:
> $$y_l(x) = \mathcal{F}(\beta_{,l}, x) + \alpha(\beta_{:,l}, x)$$
> 其中 $\alpha$ 是近似误差

### (2) Kriging 代理模型的自适应学习函数方法与特性
首先,  对于Kriging 代理模型， 要求对于失效概率g(x) = 0 点的预估， 即利用代理模型建立准确的代理功能函数零的面更为重要。
自适应的主要过程是，首先根据少量样本点建立粗糙的Kriging代理模型，而**通过自适应学习函数从备选样本即中获取==符合要求的样本点==加入当前的样本训练集中**。最后更新Kriging代理模型直到满足收敛条件;

我们需要满足下列的自适应代理模型:
1. 可以取**随机输入变量分布密度较大的区域**进行增加 
2. **距离功能函数为0的面近,  即靠近极限状态面近,  符号误判的风险较大的部分**进行增加样本点, 这些样本点的特征是$\mu_{gx}$较小， 而对应的预测方差$\sigma_{gx}^{2}$较大;
应用广泛的学习函数包括 **EFF(Expected feasibility Function)学习函数**, **基于信息熵的H学习函数**以及**U学习函数** 

> [!NOTE] 加点个数的选择 
> 一般选择 Kriging 模型的初始样本点个数与变量个数有关, 建议的初始样本量至少是变量个数的10倍以上。

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
其中$\mu_{gk}(x), \sigma_{gk}(x)$两项分别为 Kriging <mark style="background: transparent; color: red">代理模型的均值和标准差</mark>; 

其学习原理是，<mark style="background: transparent; color: red">当估计值相同时， 估计值的标准差越大， 则学习函数函数 U 的值越小。而当标准差相同时,  估计值越接近0, 则对应的学习函数越小</mark>。由于<mark style="background: transparent; color: red">越靠近失效面， 则对应的均值接近0且标准差增大</mark>， 因此**每一次可以选择学习函数U值较小的点加入**到点集中来更新 Kriging 代理模型， 从而实现Kriging 代理模型的自适应更新过程;

需要注意的是, 对于符号误判的风险大小, 实际上是U(x)的函数
$$P_{m}(x) = \Phi(-U(x))$$
原则上需要学习范围 U(x) < 2 范围内的一切抽样点，才能保证精度足够高。

## 四、可靠性与可靠性局部灵敏度分析的AK-MCS方法
### (1) 可靠性分析的AK-MCS方法 
AK-MCS 实际上是自适应 Kriging 代理模型结合 Monte Carlo 数字模拟的失效概率求解方案;
1. 获取整体的 Monte Carlo 样本点$S_{MC}$ 假设其中样本点的个数为$N_{MC}$, 供更新使用;
2. 产生训练的初始样本点，建立Kriging 代理模型， 假设选取了$N_1$ 个输变量的样本，并计算响应的代理模型函数值，  从而形成初始训练集$T_{MC}$;
3. 根据当前的$T_{MC}$建立Kriging代理模型$g_K(x)$; 依次迭代选择新样本点，即**每次将学习函数最小的部分加入**, 然后**判断收敛性**; 需要注意: 相关模型采用 Gaussian 修正模型, 而回归模型采用常数,  模型即为普通Kriging代理模型;
4. 通过建立的代理模型估计失效概率, 使用 $g_K(x^{(j)}) \leq  0$ 的样本点个数与总样本点个数的比值代替
$$\hat{P}_{f} = \frac{N_{gK} \leq  0}{N_{MC}}$$
失效概率的变异系数的估计值为: 
$$\text{cov}(\hat{P}_{f})  = \sqrt{\frac{1 - \hat{P}_{f}}{(N_{MC} - 1) P_{f}}}$$
其中， 当**变异系数$\text{cov}(\hat{P}_{f})< 5\%$时，认为估计可以接受**; 如果不能接受， 则考虑更新Monte Carlo样本池; 方法一是扩充， 方法二是重新选择; 

> [!caution] 计算过程说明
> 先构建样本池, 保证$U_{x} \geq 2$, 然后再使用

### (2) 可靠性局部灵敏度分析的AK-MCS方法 
首先，参考[[📘ClassNotes/👨‍🔧Mechanics/🖥️Computational_Mechanics/🚧结构可靠性设计基础/第二章 蒙特卡洛模拟方法|蒙特卡洛模拟方法]], 局部灵敏度可以定义为:
$$\frac{\partial P_{f}}{\partial \theta_{Xi}^{k}} = \int\dots\int _{F} \frac{\partial f_{X}}{\partial \theta_{X_{i}}^{(k)}} \frac{1}{f_{X}(x)} f_{X} (x) dx = \int\dots\int _{R^{n}} I_{F}(x) \frac{\partial f_{X}}{\partial \theta_{X_{i}}^{(k)}} \frac{1}{f_{X}(x)} f_{X} (x) dx$$
我们将上式使用均值表示， 公式为:
$$\boxed{\frac{\partial P_{f}}{\partial \theta_{X_{i}}} =E\left[ \frac{I_{F} (x)}{f_{X}(x)} \frac{\partial f_{X}(x)}{\partial \theta_{X_{i}}^{(k)}} \right]}$$
其中, 求解可靠性灵敏度**需要利用功能函数计算的只是失效域指示函数** $I_F(x)$, **可以采用AK-MCS方法计算失效概率时创建的Kriging模型**; 同时, 也可以利用AK-MCS方法求解局部灵敏度估值的变异系数。

而**输入变量为独立正态分布的情况**时, 有公式:
$$\frac{1}{f_{X}(x)}\frac{\partial f_{X}(x)}{\partial \mu_{X_{i}}} = \frac{1}{f_{X_{i}} (x_{i})} \frac{\partial f_{X_{i}}}{\partial \theta_{X_{i}}} = \frac{(x_{i} - \mu_{X_{i}})}{\sigma_{X_{i}}^{2} }$$
$$\frac{1}{f_{X}(x)}\frac{\partial f_{X}(x)}{\partial \sigma_{X_{f}}} = \frac{1}{f_{X_{i}}} \frac{\partial f_{X_{i}}(x_{i})}{\partial \theta_{X_{i}}^{(k)}} = \frac{(x_{i} - \mu_{X_{i}})^{2}}{\sigma_{X_{i}}^{3}} - \frac{1}{\sigma_{X_{i}}}$$
而对于基本过程, AK-MCS的局部灵敏度分析只需在迭代完成之后计算对应的$I_F$以及可靠性局部灵敏度即可; 

MATLAB 中的 Kriging 工具箱使用参考 https://zhuanlan.zhihu.com/p/612582292
而DACE工具箱文档参考 https://www.omicron.dk/dace/dace.pdf 
```matlab title:dacefit函数的使用格式
[dmodel, perf] = dacefit(X, y, regr, corr, theta0, lob, upb) 
% 其中：S为输入变量，Y为输出变量，regr为回归函数，包含regpoly0、regpoly1、regpoly2三种函数形式，corr为相关函数，包括corrcubic、correxp、correxpg、corrgauss、corrlin、corrspherical、corrspline共7种形式，theta0为输入theta的初值，可任意赋值, lob为theta的下限值，upb为theta的上限值
``` 

> [!summary] 自适应 Kriging 代理模型的原理
> 自适应代理模型的原理是，通过多次自适应更新Kriging 代理模型， 把原先抽样中， 所有的和极限状态面距离较近的部分都加入到 Kriging 代理模型中。

```matlab fold title:Kriging模型建立示例代码
%% 首先, 考虑航空工程中的无头铆钉的铆接应力, 通过控制铆接过程中的挤压应力, 使得
% 我们将铆钉的过程看成圆柱受压之后变形填充, 并假设:
% 1. 铆接过程中, 铆钉孔不扩大
% 2. 铆钉的体积变化忽略不计
% 3. 铆接结束后, 铆钉头部为圆柱状 
% 4. 各向同性材料
%%%%% 
clear,clc;
mu_ = [5, 20, 547.2, 5.1, 5];               % d(mm), h(mm), K(MPa), D0(mm), t(mm)
sigma_d = mu_.*[0.1, 0.02, 0.01, 0.2, 0.2]; % d(mm), h(mm), K(MPa), D0(mm), t(mm)-> variance cofficient
n_SHE = 0.15;                               % 铆钉材料的硬化指数
H_ = 2.2;                                   % 墩头高度
pressure_max = 580; % sigma_sq

% 定义功能函数为sigma_sq - sigma_max;
g =  @(x) pressure_max - x(:,3) .* (log((x(:,1).^2 .* x(:,2) - x(:,4).^2 .* x(:,5))./(2 .* H_ .* x(:,1).^2))).^n_SHE;
[Pf_mcs, Pf_mu_mcs, Pf_sigma_mcs] = MCS_solu(mu_, diag(sigma_d.^2), g, 1e7)     % -> 0.0472
clear Pf_mcs Pf_mu_mcs Pf_sigma_mcs

%% %%%%%%%%%%%%%%% 计算模型部分 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
num_MCS = 3e5;           % 蒙特拉罗法的抽样点数 -> 注意: 由于使用Kriging代理模型, 可以在更少的点下达到相同精度
num_Kriging_begin = 20;  % 从30个样本点开始(初始样本点个数) -> 从200个产生complex double
n = size(mu_, 2);
Cutting_OverFlow = 100;          % 定义一个 OverFlow 用于截断学习函数过大的样本
learning_rate = 1;               % 学习率, 每次加入几个样本点

%%%%%%%%%%%%%%%%% 进行抽样, 生成对应的样本点 (MCS生成的样本点需要较多)%%%%%%%%%%%%%%%%%%%%%%%
% 注意: 这个是进行标准均匀分布的sobol抽样;
xpp = sobolset(n,'Skip',1e5); xpp = xpp(1:num_MCS,:);       % 获取(0,1)中的MCS方法的抽样点
xp = norminv(xpp, mu_, sigma_d);                            % 通过反正态函数将均匀分布映射为正态分布
%%%%%%%%%%%%%%%% 方法一、Kriging 代理模型 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 首先初始化样本集 -----> 使用最初的几个作为样本点
X = xp(1:num_Kriging_begin, :);     % 初始 Kriging 模型的样本点
xp(1:num_Kriging_begin,:) = [];   
Y = g(X);                           % 计算初始样本值(5输入, 1输出) -> 实际上是模拟有限元过程
% 如果g(X) 是已知的, 可以考虑从中剔除距离失效域较远的部分;

%%%%%%%%%% 定义 Kriging 模型中的 超参数theta 迭代的初始值, 最大值和最小值 %%%
theta_0 = 0.01 .* ones(1,n);        % 初始定义为大小为0.01, 维数n的行向量
low_bnd = 1e-5 .*ones(1,n);         % theta 最小不超过5
upp_bnd = 20 .* ones(1,n);          % theta 最大不超过20;

min_value = 0; % 初始化最小学习函数
for epoch = 1:1000   % 进行自适应学习的Kriging模型迭代, 最多加入1000个点
    % 一阶回归方程, gauss 修正模型
    [dmodel, pref] = dacefit(X, Y, @regpoly0 , @corrgauss, theta_0, low_bnd, upp_bnd);  % 使用这些进行拟合点
    [mu_g_pred, sigma_g_pred] = predictor(xp, dmodel);                     % 将其余的全部样本值放入并进行预测
    % 获得的 mu_g_pred 是预测值, sigma_g_pred 是预测方差
    % for the opts parameter, since trial sites m > 1, return m-vector with estimated MSE (min square error) 
    
    % 对初始样本进行截断, 剔除U_x过大的点, 减小计算量
    U_x = U_LearningFunc(mu_g_pred, sigma_g_pred); % 使用U函数作为自适应学习函数, 取其从小到大作为排序;
    points_delete_ = (abs(U_x) > Cutting_OverFlow);
    xp(points_delete_,:) = [];
    U_x(points_delete_,:) = [];

    if (min_value >= 2)  % 终止条件是 U >= 2
        break;
    else
        % 每次加入学习函数值低的, 加入次数取决于 learning rate
        for i = 1:learning_rate
            [min_value,min_index] = min(U_x);    % 每一次找出其中最小的元素以及下标, 每一次加入对应的index_下标
            new_points= xp(min_index,:);
            xp(min_index,: ) = [];  % 删除xpp中的对应的行
            y_temp = g(new_points);
            if (imag(y_temp)==0)
                X = [X;new_points];      % 在x中加入对应的点
                Y = [Y;y_temp]; 
            end
        end
        sprintf("epoch: %d,U_x: %f", epoch * learning_rate ,min_value)
    end
    clear mu_g_pred sigma_g_pred y_temp
end
% 当做完之后, 整体的预测模型已经出来, 显然这时的mu_g_pred和sigma_g_pred是对应的结果, find(mu_g_pred <
% 0)为失效数量;
Pf_ref = size(find(mu_g_pred<0), 1)/size(mu_g_pred,1);  % 失效概率参考值
%% %%%%%%%%%%% 使用构建好的代理模型, 重新对原始数据进行估计对应的值 %%%%%%%%%%%%%%%%%%%%%
xp_pre = norminv(xpp, mu_, sigma_d);
y_pre  = predictor(xp_pre,dmodel);     
nums = length(y_pre);

I_f = y_pre < 0;                 % 存储失效点
Pf = length(find(I_f)) / nums;   
Pf_var = (Pf - Pf^2) / (nums-1);
Pf_cov = sqrt((1-Pf) / ((nums-1) * Pf));  % 计算局部灵敏度变异系数

%% %%%%%%%%%%%% 公式 3.3.3, 3.3.4  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
Pf_mu = 1./nums .* sum((xp_pre(I_f,:) - mu_) ./ sigma_d.^2, 1);   % 均值灵敏度
Pf_sigma = 1./nums .* sum( 1./sigma_d .*(((xp_pre(I_f,:)-mu_) ./ sigma_d).^2 - 1));  % 方差灵敏度

function res = U_LearningFunc(mu_g_pred, sigma_g_pred)
    res = abs(mu_g_pred)./sqrt(sigma_g_pred);
end
```

### (3) Kriging 自适应学习模型的收敛条件
我们考虑当 $\min_{x \in MC}U(x) \geq  2$ 时, 即符号误判风险较高的点已经全部加入样本中, 则可以认为误判部分已经全部加入了， 这个可以作为可靠性分析 AK-MCS 方法的收敛条件。 

但是<mark style="background: transparent; color: red">对于可靠性局部灵敏度分析而言</mark>, 我们往往以可靠性局部灵敏度和变异系数的估计值的收敛性来判定是否继续增加样本池容量。具体如下(参考3.3.1.1-3.3.1.3):
$$\frac{\partial P_{f}}{\partial \theta_{X_{i}}^{(k)}} = E\left[ \frac{\partial P_{f}}{\partial \theta_{X_{i}}^{(k)}} \right]= E\left[ \frac{I_{F}(x)}{f_{X} (x)} \frac{\partial f_{X}(x)}{\partial \theta_{X_{i}}^{(k)}} \right] = \frac{1}{N} \sum^{n}_{j=1}\left[  \frac{I_{F}(x_{j})}{f_{X} (x_{j})} \frac{\partial f_{X}(x_{j})}{\partial \theta_{X_{i}}^{(k)}}\right] $$
$$V\left[  \frac{\partial \hat{P}_{f}}{\partial \theta_{X_{i}}^{(k)}} \right] \approx \frac{1}{N -1}\left[\frac{1}{N} \sum^{n}_{j=1} \left(\frac{I_{F} (x_{j})}{f_{X}(x_{j})}  \cdot \frac{\partial f_{X}(x_{j})}{\partial \theta_{X_{i}}^{(k)}}\right)^{2} - \left( \frac{\partial P_{f}}{\partial \theta_{X_{i}}^{(k)}}\right)^{2}\right]$$
$$\text{Cov} \left[\frac{\partial \hat{P}_{f}}{\partial \theta_{X_{i}}^{(k)}}\right] = \frac{\sqrt{V\left[\frac{\partial \hat{P}_{f}}{\partial \theta_{X_{i}}^{(k)}}\right]}}{\left| E\left[\frac{\partial \hat{P}_{f}}{\partial \theta_{X_{i}}^{(k)}}\right]\right|}$$
判断 AKA-MCS 方法的收敛性时，只需要保证方差:
$$\text{cov}\left(\frac{\partial \hat{P}_{f}}{\partial \theta_{X_{i}}^{(k)}} \right) < 5\%$$
即可认为可靠性局部灵敏度的估计是可以被接受的。结束AK-MCS过程。

## 五、结合重要抽样的代理模型方法
### (1) AK-IS方法
AK-IS方法的可靠性分析主要思想为: 参考[[📘ClassNotes/👨‍🔧Mechanics/🖥️Computational_Mechanics/🚧结构可靠性设计基础/第三章 可靠性灵敏度分析的矩方法|第三章 可靠性灵敏度分析的矩方法]]，根据<mark style="background: transparent; color: red">改进一次二阶矩方法将原始的抽样密度中心平移到使用一次二阶矩方法求解得到的设计点处</mark>, 然后构造重要抽样密度概率函数 $h_{X}(x)$, 利用重要抽样密度函数构造重要抽样样本池, 采用 U 学习函数更新样本点。

需要说明的是, AK-IS方法是适用于功能函数已知而且方便求导的情况的， 但是也可以用于求解功能函数未知的情况(选用初始样本点即可);

失效概率计算等等参考[[📘ClassNotes/👨‍🔧Mechanics/🖥️Computational_Mechanics/🚧结构可靠性设计基础/第四章 重要抽样法原理与应用|重要抽样法原理与应用]]部分; 并需要使用 $h_{X}(x)$ 估计对应的失效概率值。

对应的可靠性灵敏度估计式为(4.2.1.1-4.2.1.4)

```matlab fold title:AK-IS代码示例
%% %%%%%%%%%%%%%%% AK-IS Kriging Model Building %%%%%%%%%%%%%%%%%%% 
% 屋架结构的可靠性分析 
clear, clc;

mu_ =     [2.9e7, 500, 1000];
sigma_d = [1.45e6, 100, 100];
sigma_ = diag(sigma_d.^2);

w = 2.4484;
t = 3.8884;
g = @(x)2.2 - 4.*100.^3./(x(:,1).* w .* t) .* sqrt((x(:,2)./w.^2).^2 + (x(:,3)./t.^2).^2);

num_AKIS_fst = 100;    % 使用50个样本点作为初始样本点
num_AKIS = 1e5;

%% %%%%%%%%%%% Kriging Model Building %%%%%%%%%%%%%%%%%%%%
[x_i, ~, ~]= AFOSM_solu(mu_, sigma_, g);
n = size(mu_,2);

%% %%%%%%%%%%% 求解重要抽样的中心抽样的概率密度 %%%%%%%%%%%%%%%%
fx_pdf = @(x)  joint_pdf(x, mu_, sigma_d);  % f_x 分布函数
hx_pdf = @(x) joint_pdf(x, x_i,sigma_d);    % 注意 h_x 的分布函数是关于x_i为中心的函数
% @note: 说明: 之后算式中使用的是 f(x)./h(x) 作为整体的函数值

xpp = lhsnorm(mu_, sigma_, num_AKIS_fst);
ypp = g(xpp);

lob = 1e-5 * ones(1,n);
upb = 20 * ones(1,n);
theta0 = 0.01 * ones(1, n);
% dmodel = dacefit(xpp, ypp, @regpoly0, @corrgauss, theta0, lob, upb);
% 这个在循环中已经调用了

%% %%%%%%%%%%% 仍然需要使用AFOSM方法构建设计点过程中的点或者使用初始选取点作为初始点构建 %%%%%%%%%%%%
xp = lhsnorm(x_i, sigma_, num_AKIS);   % 注意: 这些都是从 x_i 为中心重要抽样得到的点
% @note: 如果前面使用MCS方法后面使用IS方法, 会导致失效概率偏低, 因此预测时必须使用新获取的抽样点进行预测

cur_xp = []; cur_yp = [];
for epoch  = 1:1000
    dmodel = dacefit([xpp;cur_xp], [ypp;cur_yp], @regpoly0, @corrgauss, theta0, lob, upb);
    [mu_g, sigma_g] = predictor(xp, dmodel);  % 只是求解 xp 的预测值来计算参数
    
    U_x = U_LearningFunc(mu_g, sigma_g);
    [U_min, index_min] = min(U_x);
    if (U_min < 2)
        % 加点, 取 xp 中 U_x 最小的点加入
        sprintf("epoch: %d,min U_x: %d", epoch, U_min)
        cur_xp = [cur_xp; xp(index_min,:)];
        cur_yp = [cur_yp; g(xp(index_min,:))];
        xp(index_min,:) = [];
    else
        break;
    end 
end

%% %%%%%%%%%%%%%% 使用重要抽样计算灵敏度 %%%%%%%%%%%%%%%%%%%%%%
num_IMS = 2e4;

% 里面自带了AFOSM 求设计点的过程
% [Pf, Pf_mu, Pf_sigma,msc] = IMS_solu(mu_,sigma_,g,num_IMS);

xp_test = lhsnorm(x_i, sigma_, num_IMS);    % 用于计算失效概率
If = (predictor(xp_test, dmodel) < 0);  % 使用模型预测
Pf = sum(If.*fx_pdf(xp_test)./ hx_pdf(xp_test))  ./ num_IMS  % 失效概率
Pf_var = 1./(num_IMS -1) .* (1./num_IMS .* sum(If.* (fx_pdf(xp_test).^2 ./ hx_pdf(xp_test).^2), 1) - Pf.^2);
Pf_cov = sqrt(Pf_var)./Pf;
% 与上面的解一致即可

fX_mu = ((xp_test - mu_)./ sigma_d.^2) .* fx_pdf(xp_test);
fX_sigma = (((xp_test - mu_)./ sigma_d).^2 -1) .* fx_pdf(xp_test)./sigma_d;

Pf_mu = 1/num_IMS * sum(If .* fX_mu./hx_pdf(xp_test), 1)
Pf_sigma = 1/num_IMS * sum(If .* fX_sigma./hx_pdf(xp_test),1)

function U_x = U_LearningFunc(mu_g, sigma_g)
    U_x  = abs(mu_g)./sqrt(sigma_g);
end
% @brief: 求解多变量的联合概率密度函数(normpdf)
function f_x = joint_pdf(x, mu_,sigma_d)
    f_x = prod(1./sqrt(2 .* pi .* sigma_d.^2).* exp(-0.5 .* ((x - mu_)./(sigma_d)).^2), 2);
end


```

### (2) Meta-IS-AK 方法
全称元模型构造重要抽样代理模型方法(Metamodel Importance Sampling):
首先, **理论上最优重要抽样函数**为:
$$h_{opt}(x) = \frac{I_{F} (x) f_{X}(x)}{P_{f}}$$
但是由于功能函数在工程中往往是未知的, 因此**使用当前的 Kriging 代理模型预测函数g(X)获得的概率来构造重要抽样密度函数**; 此时式子成为: 
$$\boxed{h_{X}(x) = \frac{\pi(x) f_{X}(x)}{P_{f\varepsilon}}}\tag{9.5.2.1}$$
其中 $\pi(x)$ 是<mark style="background: transparent; color: red">概率分类函数,表示在该处样本点的函数值</mark> f(x) < 0 <mark style="background: transparent; color: red">对应的概率大小</mark>(当必定<0时, $\pi(x) = 1$), 可以计算为:
$$\boxed {\pi(x) = P\left\{ g_{K}(X)\leq 0\right\} = \Phi\left(- \frac{\mu_{g_{K1}}(x)}{\sigma_{g_{K1}}(x)}\right)}\tag{9.5.2.2}$$
而$P_{f\varepsilon}$为<mark style="background: transparent; color: red">归一化系数, 也称为扩展失效概率</mark> 并且定义为:
$$P_{f\varepsilon} = \int_{R_{n}}\pi(x) f_{X}(x) dx$$
**扩展失效概率和灵敏度**可以通过下面的公式进行计算:
$$\boxed {P_{f\varepsilon} = E (\pi (x)) = \frac{1}{N_\varepsilon } \sum^{N_\varepsilon}_{j=1}\pi(x^{(j)})}$$
$$\frac{\partial \hat{P}_{f\varepsilon}}{\partial\theta_{X_{i}}^{(k)}} = \frac{1}{N_\varepsilon}\sum^{N_\varepsilon}_{i=1} \frac{\pi(x_{f}^{(j)})}{f_{X}(x_{f}^{(j)})} \frac{\partial f _{X}(x_{f}^{(j)})}{\partial \theta_{X_{i}}^{(k)}}$$
此时,采用元模型构造的重要抽样密函数(参考9.5.2.1), 则容易计算出失效概率估计为:
$$\boxed {P_{f} = \int_{R^{n}} I_{F}(x) \frac{f_{X}(x)}{h_{X}(x)} h_{X}(x)dx = P_{f\varepsilon} \int_{R^{n}} \frac{I_{F}(x)}{\pi(x)} h_{X}(x) dx}$$
我们取 $\alpha_{corr}$ 为修正因子, 并写为:
$$\alpha_{corr} =  \int_{R^{n}} \frac{I_{F}(x)}{\pi(x)} h(x) dx \qquad  P_{f}  = P_{f\varepsilon} \alpha_{corr}$$

Meta-IS-AK方法构建 Kriging 代理模型的步骤为:
1. 首先由 Meta-IS 方法, **逐步逼近重要抽样函数的样本点, 迭代收敛之后，即可得到第一步的Kriging模型和相应的重要抽样样本点** 
2. 利用 AK-IS 方法更新Kriging 模型，具体步骤和5.1中相同。

其中，第一步的具体方法是: 在每一步先重要抽样, 然后<b><mark style="background: transparent; color: blue">对重要抽样样本进行K-Means聚类分析</mark></b>(参考[[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/Class Contents/基本课程/1.KNN算法和KMeans算法|1.KNN算法和KMeans算法]]), 获取K个形心, 然后**将K个形心以及对应的真实功能函数值加入训练样本集T中**, 并**使用$T$更新代理模型**。 

在更新代理模型之后, 使用**交叉验证方法**判定收敛性, 具体方法是<b><mark style="background: transparent; color: blue">计算修正因子的留一法估计值</mark></b> $\hat{\alpha}_{corrLOO}$:
$$\Large \hat{\alpha}_{corrLOO} = \frac{1}{m} \sum^{m}_{i=1}\frac{I_{F}(x_{T} ^{(i)})}{P\left\{ g_{K(T/x_{T}^{(i)})} (x_{T}^{(i)})\leq 0\right\}}$$
其中m为迭代更新Kriging模型的训练样本点个数,$I_F$ 由真实功能函数 而对应的功能函数值为:
$$T = \left\{ (x_{T}^{(1)},  g(x_{T}^{(1)})), \dots  (x_{T}^{(m)},  g(x_{T}^{(m)})) \right\}$$
需要说明的是$g_{K(T/x_T^{(i)})}$是样本中除去第i个数据之后构建的 Kriging 模型, 下面是对应的失效概率。需要说明的是, 一开始就估计修正因子，会浪费更多的计算量。

==显然, $\alpha$ 越接近于1, 则概率分类函数越接近失效域指示函数$I_F(x)$, 而为了第一步能够有一定的准确性==, 一般取初始_corr值为 $[0.1 ~ 10]$ 且训练样本数量大于规定的最小值(可以取30)时时，即停止第一次迭代。

实际上, 我们在Meta-IS-AK方法中，只是调用了一次相应的模型, 且模型只更新了一次。

### (3) Meta-IS-AK 方法的可靠性灵敏度计算方法
对于Meta-IS-AK方法的可靠性局部灵敏度, 计算为:
$$\frac{\partial P_{f}}{\partial \theta_{X_{i}}^{(k)}} = \frac{\partial \alpha_{corr}}{\partial \theta_{X_{i}}^{(k)}} P_{f\varepsilon} + \alpha_{corr} \frac{\partial P_{f\varepsilon}}{\partial \theta_{X_{i}}^{(k)}} $$
其中, 每一项可以计算为:
$$\frac{\partial P_{f\varepsilon}}{\partial \theta_{X_{i}}^{(k)}} = E \left[\frac{\pi(x)}{f_{X}(x)} \frac{\partial f_{X} (x)}{\partial \theta_{X_{i}}^{(k )}} \right]$$
$$\frac{\partial \alpha_{corr}}{\partial \theta_{X_{i}}^{(k)}} = E\left[\frac{I_{F}(x) }{\pi(x) f_{X}(x)} \frac{\partial f_{X} (x)}{\partial \theta_{X_{i}}} - \frac{ I_{F} (x) }{\pi(x) P_{f\varepsilon}} \frac{\partial P_{f\varepsilon}}{\partial \theta_{X_{i}}^{(k)}} \right]$$
`````ad-todo
title: 推导
collapse: open
`````

`````ad-caution
title: 编程注意事项
collapse: open
在求解predictor的预测值的方差时, 根据文档有如下说明，其中求解的是MSE方差作为$\sigma_g$， 根据文档有:
```matlab
% Output
% y    : predicted response at x.
% or   : If mx = 1, then or = gradient vector/Jacobian matrix of predictor
%        otherwise, or is an vector with mx rows containing the estimated
%                   mean squared error of the predictor
% Three or four results are allowed only when mx = 1,
% dy   : Gradient of predictor; column vector with  n elements
% mse  : Estimated mean squared error of the predictor;
% dmse : Gradient vector/Jacobian matrix of mse
```
按照上面说明, mse参数是当设计点仅有1个时的方差值, 而当多个设计点(训练点)对应的方差是从or参数获取的, 因此获取方差时, 一般需要考虑长度是否为1调用形式不同, 即
```matlab 
function [qx]=qxfun(x,dmodel ) %pi(x)的计算s
    if(size(x, 1)==1)
        [ypre,~,ysd]=predictor(x,dmodel);    % use min square error for the 
        qx=normcdf(-ypre./sqrt(ysd));     
    else
        [ypre,ysd]=predictor(x,dmodel);      % use or (m-vector with estimated MSE);
        qx=normcdf(-ypre./sqrt(ysd));
    end
end
```
`````


```
Z = hx([xx,yy]);
y = -4:0.1:4;
xx = reshape(X, [], 1); yy = reshape(Y, [], 1);
Z = hx([xx,yy]);
```



> [!hint] 补充：给定分布函数(概率密度函数)的抽样方法
> 任意的概率下的抽样转换方法,只需要进行坐标下的抽样, 然后利用分布函数构造反函数(即在$x = [0,1]$区间分布的某个函数，直接均匀抽样后即可利用分布函数投影到这个函数)


下面的代码使用 Meta-IS-AK 方法构建代理模型, 并使用IS方法求解了三种失效模式的失效概率局部灵敏度;经过检测， 在三个例题上都能够取得较好的代理模型建立效果:

```matlab fold title:Meta-IS-AK方法的示例代码
%% %%%%%%%%%%%%%%%%  9.6 具有串联失效模式的模型 %%%%%%%%%%%%%%%%%%%%% 
clear, clc;
mu_ = [0, 0];  sigma_d = [1, 1]; sigma_ = diag(sigma_d.^2);
c = 2;
g1 = @(x) c - 1 - x(:,2) + exp(- x(:, 1).^2 ./ 10) + (x(:,1)./5).^4;
g2 = @(x) c.^2 ./ 2 - x(:,1) .* x(:,2);
g = @(x) min([g1(x), g2(x)],[],2);
initial_point  = [-4, 4];             % 提供初始样本点(失效域内即可)
fx = @(x)joint_pdf(x, mu_, sigma_d);  % 提供原始的联合概率密度函数
%% %%%%%%%%%%%%%%%%  9.7 非线性串联系统 %%%%%%%%%%%%%%%%%%%%%%%
% mu_ = [5, 1];
% sigma_d = [1, 0.1];
% sigma_ = diag(sigma_d.^2);
% 
% g1 = @(x) 2* x(:,1).^2 - x(:,2) + 6;
% g2 = @(x) x(:,1) + x(:,2).^2 - 4;
% g = @(x) min([g1(x), g2(x)],[],2);
% initial_point  = [3, -1];                % 提供初始样本点
% fx = @(x)joint_pdf(x, mu_, sigma_d);     % 提供原始的联合概率密度函数
%% %%%%%%%%%%%%%%%  9.8 钢架结构的四个失效模式 %%%%%%%%%%%%%%%%
% mu_ = [2,2,2,1]; 
% sigma_d = [0.2, 0.2, 0.2, 0.25]; 
% sigma_ = diag(sigma_d.^2);
% 
% g1 = @(x) 2 .* x(:,1) + 2 .* x(:,3) - 4.5 .* x(:,4);
% g2 = @(x) 2 .* x(:,1) + x(:,2) + x(:,3) - 4.5 .* x(:,4);
% g3 = @(x) x(:,1) + x(:,2) + 2 .* x(:,3) - 4.5 .* x(:,4);
% g4 = @(x) x(:,1) + 2.* x(:,2) + x(:,3) - 4.5 .* x(:,4);
% g = @(x) min([g1(x), g2(x), g3(x), g4(x)],[],2);
% 
% initial_point  = [1, 1, 1, 1];           % 提供初始样本点
% fx = @(x)joint_pdf(x, mu_, sigma_d);     % 提供原始的联合概率密度函数
% [Pf, Pf_mu, Pf_sigma, msc] = MCS_solu(mu_, sigma_, g, 1e6);
%% %%%%%%%%%%%%%%%%   Meta-AK-IS 方法的参数 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
n = size(mu_, 2);
params.num_MKIS_init = 30;   	     % 初始样本点 m0 = 30
params.max_epoch1 = 10;              % 
params.nKMeans = 10;
params.num_MCMC = 1e4;               % 更新初始代理模型时每次抽取的点
params.a_LOSS_min = 0.1;             % 初始模型接受a_LOSS下界
params.a_LOSS_max = 10;

params.regr = @regpoly0;
params.corr = @corrgauss;
params.theta0 = ones(1,n);
params.lob  =  1e-5 .* ones(1,n);
params.upb  = 100 .* ones(1,n);

params.num_ISAK   = 2e3;         % ********* 注意: 这个会影响收敛性, 所以不要调特别高*****
params.max_epoch2 = 1000;        % ****** 从ISAK样本中最大添加样本数量(< num_ISAK) ****** 

%% %%%%%%%%%%%%% 构建初始抽样函数元模型, 迭代更新Kriging模型 %%%%%%%%%%%%%%%%%%%%%%
x_train = lhsnorm(mu_, sigma_, params.num_MKIS_init);    % 生成随机样本点
y_train = g(x_train);                   

% 抽样一次, 用于估计扩展失效概率
x_temp = slicesample(mu_, 2e4,"burnin",500, "pdf",fx, 'thin', 5);   % lhsnorm(mu_, sigma_, 2e4)

for epoch = 1:params.max_epoch1
	[dmodel, pref] = dacefit(x_train, y_train,params.regr,params.corr,params.theta0,params.lob,params.upb);
    % 可以首先通过MCS抽样确定扩展失效概率 Pf_epsilon 的大小
    Pf_epsilon = mean(fx_pred(x_temp, dmodel));         % 求解扩展失效概率 Pf_epsilon 的值;
	
    % 在求解过程中,  pi(x) -> 直接通过 fx_pred 求解得到
    fx_pdf = @(x) fx(x);                                % 原始变量的概率密度函数 
	px_pdf = @(x) fx_pred(x, dmodel);       		    % 修正项 pi(x)
	hx_pdf = @(x) px_pdf(x) .* fx_pdf(x) ./ Pf_epsilon;     % 新的重要抽样函数概率密度 h(x)
    
    % 以hx为概率密度 -> 根据经验取失效域中某一点为起始点进行 Marklov 抽样 (初始点影响不大)
	[xp_mcmc] = slicesample(initial_point,params.num_MCMC,"burnin",1000, "pdf",hx_pdf, "thin", 3);
	% 此处是以h(x)为密度函数抽样的, 同时把 predictor 代入到了h(x)中进行抽样
		
	% 用交叉验证方法(cross validation method)求解 \hat{a_corrLoo}判定是否收敛, 否则增加样本点进行迭代计算 
    a_LOSS = crossloss(x_train, y_train, dmodel);
    
    if (a_LOSS > params.a_LOSS_min && a_LOSS < params.a_LOSS_max)
        break;
    else
        fprintf("refine epoch:%d, alpha_Loss : %f\n", epoch,a_LOSS)
        % 通过kmeans算法,计算形心, 
        [~, C] = kmeans(xp_mcmc, params.nKMeans);
	    x_train = [x_train; C];
	    y_train = [y_train; g(C)];
    end
end
clear fx_pdf px_pdf C xp_mcmc x_temp;

if epoch < params.max_epoch1
	fprintf("\nSampling function meta-model build successfully\n")
    fprintf("total epoch for meta-model: %d\n", epoch)
    fprintf("alpha Loss is %.4f\n", a_LOSS)
    fprintf("Pf_epsilon = %f\n\n", Pf_epsilon)
else
	warning("reach maximum epoch %d, model may be not correct", params.max_epoch1)
    fprintf("alpha Loss is %.4f\n", a_LOSS)
end
clear epoch a_LOSS Pf_epsilon

%% %%%%%%%%%%% 使用 AK-IS 方法构建更加精细的 Kriging 模型 %%%%%%%%%%%%%%%%
x_test = slicesample(initial_point,params.num_ISAK,"burnin",1000, "pdf", hx_pdf, "thin", 5);
% 注意: 其中是以 hx_pdf 为重要抽样函数的

for epoch = 1:params.max_epoch2
	[dmodel, pref] = dacefit(x_train, y_train, params.regr, params.corr, params.theta0, params.lob, params.upb);
	[mu_g, sigma_g] = predictor(x_test, dmodel);  % -> 此时的模型已经具有一定的精度
	U_x = U_LearningFunc(mu_g, sigma_g);
	[U_min, index_min] = min(U_x);
    
    fprintf("total sample number: %d, min U_x: %f\n", size(x_train, 1), U_min)
    if (U_min < 2)
        % 加点, 取 xp 中 U_x 最小的点加入
        x_train = [x_train; x_test(index_min,:)];
        y_train = [y_train; g(x_test(index_min,:))];
        x_test(index_min,:) = [];
    else
        break;
    end
end

if epoch < params.max_epoch2
	fprintf("\nsubmodel build successfully\n")
    fprintf("total epoch number for refine the submodel: %d\n", epoch)
else
	warning("reach maximum epoch %d, the submodel may be not precise", params.max_epoch2)
end
clear epoch
%%%%%%%%%%%% 直接利用 IS 方法求解重要抽样计算模型失效概率以及局部灵敏度 %%%%%%%%%%%%%%%%%
x_pred = slicesample(initial_point,params.num_ISAK,"burnin",1000, "pdf", hx_pdf, "thin", 5);
y_pred = dacepredict(x_pred, dmodel);
If = (y_pred < 0);

Pf = sum(If .* fx(x_pred) ./ hx_pdf(x_pred)) ./ params.num_ISAK;
msc.Pf_var = 1./(params.num_ISAK -1) .* (1./params.num_ISAK .* sum(If.* (fx(x_pred).^2 ./ hx_pdf(x_pred).^2), 1) - Pf.^2);
msc.Pf_cov = sqrt(msc.Pf_var) ./ Pf;

fX_mu = ((x_pred - mu_)./ sigma_d.^2) .* fx(x_pred);
fX_sigma = (((x_pred - mu_)./ sigma_d).^2 -1) .* fx(x_pred)./sigma_d;

Pf_mu = 1/params.num_ISAK .* sum(If .* fX_mu./hx_pdf(x_pred), 1);
Pf_sigma = 1/params.num_ISAK .* sum(If .* fX_sigma./hx_pdf(x_pred),1);
clear fX_mu fX_sigma 
% [Pf_mcs, Pf_mu_mcs, Pf_sigma_mcs, msc_mcs] = MCS_solu(mu_, sigma_, @(x)dacepredict(x,dmodel), 1e4);

%% %%%%%%%%%%%%%%%%%%%%%%% functions %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% @brief: U 学习函数
function U_x = U_LearningFunc(mu_g, sigma_g)
    U_x  = abs(mu_g)./sqrt(sigma_g);
end

% @brief: 求解多变量的联合概率密度函数(normpdf)
function f_x = joint_pdf(x, mu_,sigma_d)
    f_x = prod(1./sqrt(2 .* pi .* sigma_d.^2).* exp(-0.5 .* ((x - mu_)./(sigma_d)).^2), 2);
end

% @brief: pi(x) 值的计算, 即返回各个项失效的概率
function px = fx_pred(xp, dmodel)
    [mu_g,sigma_g] = dacepredict(xp, dmodel); % 单个输入变量, 获取方差(用于提供hx_pdf作为抽样函数)
    px = normcdf(- mu_g ./sqrt(sigma_g));     % 直接通过分布函数取均值获取 pi(x) 的每一项
end

% @brief: 计算修正因子alpha_corr的留一法估计值
function a_corrLoo = crossloss(x_train, y_train, dmodel)
	m = size(x_train, 1);
	If = (y_train < 0);	
	a_i = zeros(m, 1);
	for i = 1:m
		If_temp = If; If_temp(i,:) = [];
		Pi_temp = fx_pred(x_train, dmodel); Pi_temp(i,:) = [];
		a_i(i) = sum(If_temp .* Pi_temp) ./ m;
	end
	a_corrLoo = mean(a_i);
end
```

