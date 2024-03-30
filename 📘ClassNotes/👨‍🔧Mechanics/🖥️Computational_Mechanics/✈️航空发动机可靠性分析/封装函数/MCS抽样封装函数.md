```matlab title:MCS方法失效概率求解
function  [Pf, Pf_mu, Pf_sigma] = MCS_solu(mu_, sigma_, g, num_MCS)
% 其中 Pf_mu, Pf_sigma 仅对相互独立的正态变量求解有效
sigma_d = sqrt(diag(sigma_))';
if (nargin == 3)
    num_MCS = 1e7;
end
n = size(mu_, 2);
xp = lhsnorm(mu_, sigma_, num_MCS,"on");
fail_points = find(g(xp) < 0);
fail_xp = xp(fail_points,:);

Pf = size(fail_points, 1)/num_MCS;
% Pf_Sigma = sqrt((Pf - Pf^2)/(num_MCS -1));  % 注意
% Pf_Cov = Pf_Sigma/Pf;
z_i = (fail_xp - mu_)./sigma_d;                  % 正态化变量
Pf_mu = sum(z_i ./ sigma_d) /num_MCS;
Pf_sigma = sum((z_i.^2 - 1) ./ sigma_d)/num_MCS;
end
```


```

```