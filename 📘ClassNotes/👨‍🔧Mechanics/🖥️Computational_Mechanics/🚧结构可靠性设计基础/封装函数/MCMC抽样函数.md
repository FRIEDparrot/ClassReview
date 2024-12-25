文档参考[[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/👨‍🎓深度学习算法原理(sklearn)/补充知识/6. 马尔科夫MCS抽样(MCMC)|马尔科夫MCS抽样(MCMC)]]和[[📘ClassNotes/👨‍🔧Mechanics/🖥️Computational_Mechanics/🚧结构可靠性设计基础/第四章 重要抽样法原理与应用#三、基于核密度估计的自适应重要抽样法|基于核密度估计的自适应重要抽样法]] 部分

```matlab
function [xp, gx] = MCMC_Sample(x0, l_i, f_X, g, b, num_MCMC, gx0_optional)
% @brief: 马尔科夫链抽样, x0 为抽样中心,l_i 为步长, f_X 为概率密度, g 失效函数,
% b: g(x) boundary,  num_MCMC 抽样个数, gx0_optional 初始点函数值, 用于减少运算量(可以不加)
% xp: 抽样获取到的点集 gx 对应函数值
% ------------------------------------------------------------- 
% for normal MCMC sample, b = 0, or if use SUS method, specify b value as boundary
% **ref empirical value**: l_i = 6 .* sigma_d .* num_MCMC .^(-1/(n+4));
% f_X: function handle, probability distribution : f_X = @(x)probability_func(x);
% transfer probability is f_X(x_i(i,:))/f_X(x_i(i-1,:)), use (q(i,j)  == 1)
    gx = zeros(num_MCMC, 1);
    if nargin == 6
        gx(1,:) = g(x0);
    else
        gx(1,:) = gx0_optional;
    end    
    n = length(x0);
    xp = zeros(num_MCMC, n);  xp(1,:) = x0;
    
    for i = 2:num_MCMC
        res = zeros(1, n);  % 存储抽样得到的点
        for j = 1:n
            range = [xp(i-1,j) - l_i(j)/2, xp(i-1, j) + l_i(j)/2];  % 马尔科夫链抽样范围
            res(j) = unifrnd(range(1),range(2),1, 1);  % 平均抽样方法获取下一个备选转移状态
        end
        temp = g(res);
        if temp > b  % 抽取到未失效点
            xp(i,:) = xp(i-1,:);   % 直接保持原状态;
            gx(i,:) = gx(i-1,:); % 对应的gx
            continue
        end
        % 对于其中的每一个变量, 求解转移概率
        r = f_X(res)/f_X(xp(i-1,:)); % 决定接收或者拒绝
        if ((rand(1) < min(r)) || (gx(i-1,:) > b)) % 用平均分布产生数据, random('Uniform', 1)
            xp(i,:) = res;
            gx(i,:) = temp;
        else
            xp(i,:) = xp(i-1,:);   % 直接保持原状态;
            gx(i,:) = gx(i-1,:);
        end
        clear temp 
    end
end
```


另外， 对于独立标准正态分布的变量，可以利用下式计算联合概率密度函数:
```matlab
% @brief: 假设每一个变量均为标准正态概率密度函数, 获取联合概率密度函数
% @caution: must called as f_X = @(x)NPfunc(x, mu_, sigma_d);
function f_X = NPfunc(x_i, mu_, sigma_d)
    f_X = prod(normpdf(x_i,mu_, sigma_d), 2);
end
```

求解正态分布的均值方差灵敏度部分
```matlab
% @brief: Normal loss Function: 对于正态分布变量,求解失效概率和均值方差灵敏度
function [Pf, Pf_mu, Pf_sigma] = NLfunc(xp, If, mu_, sigma_d, nums)
	Pf       = nnz(If)/nums;             % 初始时必定为p0(一开始<b的必定是p0)
	Pf_mu    = sum(If .* (xp - mu_)./ sigma_d.^2, 1)/nums;  
	Pf_sigma = sum(If .*(((xp - mu_)./sigma_d).^2 -1), 1)./sigma_d ./nums;
end
```

