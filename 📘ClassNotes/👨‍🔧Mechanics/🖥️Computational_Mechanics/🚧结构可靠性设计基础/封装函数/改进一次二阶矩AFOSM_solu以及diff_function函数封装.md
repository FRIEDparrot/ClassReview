---
~
---

```MATLAB title:AFOSM_solu.m
function [x_i, beta_res, Pf, exitflag] = AFOSM_solu(mu_,sigma_, g)
%   AFOSM_SOLUTION AFOSM方法, 改进一次二阶矩方法求解可靠度 beta 以及设计点坐标x_i
%   必须都是独立正态分布的抽样, 且最好是线性的失效函数;
%   return 正常求解返回 1, -1求解错误, -2 获得的点不满足零点精度要求
    x_i = mu_;                       % 初始化迭代点
    sigma_d = sqrt(diag(sigma_))';   
    beta_pre = 0;   % 实际上是以均值为设计点计算的beta值
    exitflag = 1;   
    y0 = g(mu_);    %  ***** 修正: 首先计算在均值处的 g(x) 值, 用于修正 beta ********

    % 首先求解设计点, 由于是线性函数, 使用的是AFOSM方法求解可靠度 
    for epoch = 1:100
        dg_dx = diff_function(g,x_i);
        lambda_ = - (dg_dx .* sigma_d)/norm(dg_dx .* sigma_d); % 求解对应的lambda
        G =  @(beta_) g(mu_ + sigma_d .* lambda_ .* beta_ );   % 根据 lambda 求解 beta
        % 原变量空间满足 g(X1, X2, ... Xn) = 0;
        
        [beta_temp,~, exitflag_,~]  = fzero(G, beta_pre);      % 计算一个新的beta值(可能是 < 0的不正确值)
        if (exitflag_~=1)  % 错误处理
            warning("no zero point found, design point invalid\n");
            if g(mu_) < 0
                Pf = 1e-15;
                beta_res = -15;
            else
                Pf = 1;
                beta_res = 15;
            end
            x_i = mu_;
			exitflag = -1;
            return;   % 直接退出函数
        end
        
        if (abs(beta_temp - beta_pre) < 1e-4) % 迭代直到beta_收敛
            break
        else
            beta_pre = beta_temp; 
            x_i = mu_ + sigma_d .*  lambda_ .* beta_temp;
        end
    end
    beta_res = sign(y0) * abs(beta_temp); % 在此处使用g(mu_)值修正求解到的 beta,
    % 当有 g(mu) > 0 时, beta_temp > 50%,  beta > 0, 而 g(mu) < 0时beta_temp < 50% 
    Pf = normcdf(-beta_res);

    if (g(x_i) > 1e-5)
        warning("zero point find exceed the error 1e-5, solution failed");
        exitflag = -2; % 
    end 
end
```


```matlab title:diff_function.m
function dg_dx = diff_function(g, x_i, delta_)
    if nargin == 2
        delta_ = 1e-8 * ones(n); % 求导步进
    end
	n = size(x_i, 2);
    m = size(x_i, 1); % 允许多行进行输入, 则返回多行的导数值
    % 求解一个函数g在x_i(行向量)处关于各个变量的导数, 注意输入x_i必须为行向量;
    dg_dx = zeros(m, n);

    for i = 1:m
        cur_xi = x_i(i,:);
        for j = 1:n
            x_delta = zeros(1,n); x_delta(j) = delta_(j);
            dg_dx(i, j) = (g(cur_xi + x_delta) - g(cur_xi))/delta_(j);
            % 求解对应处的导数
        end
    end
end
```
