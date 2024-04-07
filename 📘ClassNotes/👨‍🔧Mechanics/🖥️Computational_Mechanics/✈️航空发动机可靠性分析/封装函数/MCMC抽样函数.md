文档参考[[📘ClassNotes/👨‍🔧Mechanics/🖥️Computational_Mechanics/✈️航空发动机可靠性分析/补充知识/1. 马尔科夫MCS抽样(MCMC)|马尔科夫MCS抽样(MCMC)]]和[[📘ClassNotes/👨‍🔧Mechanics/🖥️Computational_Mechanics/✈️航空发动机可靠性分析/第四章 重要抽样法原理与应用#三、基于核密度估计的自适应重要抽样法|基于核密度估计的自适应重要抽样法]] 部分

```matlab
% @brief: 马尔科夫链抽样, 失效集为x_i, l_i 为步长, f_X 为概率密度, g 失效函数, num_MCMC
% empirical value: l_i = 6 .* sigma_d * num_MCMC .^(-1/(n+4));
% transfer probability: f_X(x_i(i,:))/f_X(x_i(i-1,:))  set (q(i,j)  == 1)
function x_i = Makov_Sample(x0, l_i, f_X,g,num_MCMC)
    n = length(x0);
    x_i = zeros(num_MCMC, n);  x_i(1,:) = x0;
    for i = 2:num_MCMC
        res = zeros(1, n);  % 存储抽样得到的点
        for j = 1:n
            range = [x_i(i-1,j) - l_i(j)/2, x_i(i-1, j) + l_i(j)/2];  % 马尔科夫链抽样范围
            res(j) = unifrnd(range(1),range(2),1, 1);  % 平均抽样方法获取下一个备选转移状态
        end
        if g(res) > 0  % 抽取到未失效点
            x_i(i,:) = x_i(i-1,:);   % 直接保持原状态;
            continue
        end
        % 对于其中的每一个变量, 求解转移概率
        r = f_X(res)/f_X(x_i(i-1,:)); % 决定接收或者拒绝 
        if (rand(1) < min(r))         % 用平均分布产生数据, random('Uniform', 1)
            x_i(i,:) = res;
        else
            x_i(i,:) = x_i(i-1,:);   % 直接保持原状态;
        end
    end
end
```
