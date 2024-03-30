---
~
---

```MATLAB title:AFOSM_solu.m
function [x_i, beta_res, Pf] = AFOSM_solu(mu_,sigma_, g)
%AFOSM_SOLUTION AFOSM方法, 改进一次二阶矩方法求解可靠度 beta 以及设计点坐标x_i
%   必须都是正态分布的抽样, 且最好是线性的失效函数;
x_i = mu_;                       % 初始化迭代点
sigma_d = sqrt(diag(sigma_))';   

beta_pre = 0;   % 实际上是以均值为设计点计算的beta值

% 首先求解设计点, 由于是线性函数, 使用的是AFOSM方法求解可靠度
for epoch = 1:2000
    dg_dx = diff_function(g,x_i);
    lambda_ = - (dg_dx .* sigma_d)/ sqrt(sum(dg_dx.^2 .* sigma_d.^2));% 求解对应的lambda
    G =  @(beta_) g(mu_ + sigma_d .* lambda_ .* beta_ );   % 根据 lambda 求解 beta
    % 原变量空间满足 g(X1, X2, ... Xn) = 0;
    beta_temp = fzero(G, beta_pre);     % 计算一个新的beta值
    if abs(beta_temp - beta_pre) < 1e-4  % 迭代直到beta_收敛
        break
    else
        beta_pre = beta_temp;
        x_i = mu_ + sigma_d .*  lambda_ .*beta_temp;
    end
end

beta_res = beta_temp;
Pf = normcdf(-beta_res);

if epoch == 2000
    warning("reach the maximum epoch 2000, the result may be not correct");
end

end
```


```matlab title:diff_function.m
function dg_dx = diff_function(g, x_i, delta)
if nargin == 2
    delta = 1e-8; % 求导步进
end
% 求解一个函数g在x_i(行向量)处关于各个变量的导数, 注意输入x_i必须为行向量;

m = size(x_i, 1); % 允许多行进行输入, 则返回多行的导数值
n = size(x_i, 2);
dg_dx = zeros(m, n);

for i = 1:m
    cur_xi = x_i(i,:);
    for j = 1:n
        x_delta = zeros(1,n); x_delta(j) = delta;
        dg_dx(i, j) = (g(cur_xi + x_delta) - g(cur_xi))/delta;
    end
end

end
```