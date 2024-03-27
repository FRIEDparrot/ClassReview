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
```matlab title:dacefit函数的使用格式
[dmodel, perf] = dacefit(X, y, regr, corr, theta0, lob, upb) 
% 其中：S为输入变量，Y为输出变量，regr为回归函数，包含regpoly0、regpoly1、regpoly2三种函数形式，corr为相关函数，包括corrcubic、correxp、correxpg、corrgauss、corrlin、corrspherical、corrspline共7种形式，theta0为输入theta的初值，可任意赋值, lob为theta的下限值，upb为theta的上限值
``` 

> [!summary] 自适应 Kriging 代理模型的原理
> 自适应代理模型的原理是，通过多次自适应更新Kriging 代理模型， 把原先抽样中， 所有的和极限状态面距离较近的部分都加入到 Kriging 代理模型中。

```matlab title:Kriging模型建立示例代码
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

