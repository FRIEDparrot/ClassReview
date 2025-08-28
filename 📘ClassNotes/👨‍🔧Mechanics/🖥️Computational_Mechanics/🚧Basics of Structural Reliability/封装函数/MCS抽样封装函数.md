```matlab title:MCS方法失效概率求解
function  [Pf, Pf_mu, Pf_sigma, msc] = MCS_solu(mu_, sigma_, g, num_MCS)
% MCS 失效概率求解函数;
% 其中 Pf_mu, Pf_sigma 仅对相互独立的正态变量求解有效
sigma_d = sqrt(diag(sigma_))';
if (nargin == 3)
    num_MCS = 1e7;
end
n = size(mu_, 2);
xp = lhsnorm(mu_, sigma_, num_MCS,"on");
If = (g(xp) < 0);
[Pf, Pf_mu, Pf_sigma] = NLfunc(xp, If, mu_, sigma_d, num_MCS);
msc.Pf_Sigma = sqrt((Pf - Pf^2)/(num_MCS -1));  % 注意
msc.Pf_Cov = msc.Pf_Sigma/Pf;
end

% Normal loss Function: 对于正态分布变量,求解失效概率和均值方差灵敏度
function [Pf, Pf_mu, Pf_sigma] = NLfunc(xp, If, mu_, sigma_d, nums)
	Pf       = nnz(If)/nums;             % 初始时必定为p0(一开始<b的必定是p0)
	Pf_mu    = sum(If .* (xp - mu_)./ sigma_d.^2, 1)/nums;  
	Pf_sigma = sum(If .*(((xp - mu_)./sigma_d).^2 -1), 1)./sigma_d ./nums;
end
```
