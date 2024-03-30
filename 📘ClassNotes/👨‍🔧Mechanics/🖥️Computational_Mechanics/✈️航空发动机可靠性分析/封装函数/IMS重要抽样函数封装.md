
```MATLAB
function [Pf, Pf_mu, Pf_sigma,msc] = IMS_solu(mu_, sigma_, g_arr, num_MXS)
    %IMS_SOLU 重要抽样方法求解多模式下的混合概率密度(兼容单失效模式);
    if (nargin == 3)
        num_MXS = 2500;
    end
    %% %%%%%%%%% 使用AFOSM方法求解设计点以及多个失效模式的可靠度 %%%%%%%
    n = length(mu_);
    m = length(g_arr);                % 失效模式个数
    sigma_d = sqrt(diag(sigma_))';    % 
    
    x_arr = zeros(m, n); beta_ = zeros(1,m);
    for i = 1:m
        [x_arr(i,:), beta_(i), ~] = AFOSM_solu(mu_, sigma_, g_arr{i});
    end
    % 定义联合概率密度 -> 即每一个带方差部分的概率密度乘积;
    %% %%%%%%%%%%%%%%%%% 多失效模式下的混合抽样 %%%%%%%%%%%%%%%%%%%%%%
    I_f_sig = false(num_MXS,m);       % 失效标志位
    xpt = zeros(num_MXS, n, m);       % xp_tot, 简称为xpt
    alpha_ = normcdf(-beta_)/sum(normcdf(-beta_));
    for i = 1:m
        g = g_arr{i};  % 获取对应的失效函数 -> ****不要直接使用g***
        xpt(:,:,i) = lhsnorm(x_arr(i,:), sigma_,num_MXS);  % xp, 抽样点     
        I_f_sig(g(xpt(:,:,i))< 0,i) = 1;                   % 取出其中失效的点
    end
    clear xp fail_xp
    %% %%%%%%%%%%%%%%%% 结果求解部分 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    f_x = @(x) joint_pdf(x,mu_,sigma_d);
    h_x = @(x) Kernel_func(x, x_arr, sigma_d, beta_);
    fX_mu = zeros(num_MXS,n,m);  fX_sigma = zeros(num_MXS, n,m);
    for i = 1:m
        xp = xpt(:,:,i);
        fX_mu(:,:,i) = ((xp- mu_)./ sigma_d.^2) .* f_x(xp);
        fX_sigma(:,:,i) = (((xp - mu_)./ sigma_d).^2 -1) .* f_x(xp)./sigma_d;
    end
    [Pf, Pf_mu, Pf_sigma, msc] = IMS_result(xpt, I_f_sig, f_x, h_x, fX_mu, fX_sigma, alpha_);
end

%% %%%%%%%%%%%%%%%%%%%%% 函数部分 %%%%%%%%%%%%%%%%%%%%%%%%%%% 
% @brief: 多变量的混合重要抽样密度函数(直接使用正态函数代替)
function h_x = Kernel_func(x, x_arr, sigma_d, beta_)
    m = length(beta_);                    % 失效模式个数
    alpha_ = normcdf(-beta_)/sum(normcdf(-beta_));  % alpha, 每个失效模式权重
    temp = 0;
    for i = 1:m
        x_i = x_arr(i,:);
        H = prod(1./sqrt(2 * pi .* sigma_d.^2).* exp(-0.5 .* ((x - x_i)./(sigma_d)).^2),2);
        temp = temp + alpha_(i) * H;
    end
    h_x = temp;
end

% @brief: 求解多变量的联合概率密度函数
function f_x = joint_pdf(x, mu_,sigma_d)
    f_x = prod(1./sqrt(2 * pi * sigma_d.^2).* exp(-0.5 .* ((x - mu_)./(sigma_d)).^2), 2);
end
```

```MATLAB
function [Pf, Pf_mu, Pf_sigma, msc]= IMS_result(xpt, I_f, f_x, h_x, fX_mu, fX_sigma, alpha_)
%% %%%%%% 输出函数 [xpt, I_f_sig,f_x, h_x, fX_mu, fX_sigma, alpha_] %%%%%%%%
    m = length(alpha_);
    n = size(xpt, 2);
    nums = size(xpt, 1);
    
    Pf_t = zeros(1, m);  Pf2_t = zeros(1, m); % Pf2_t备用
    Pf_mu_t = zeros(m,n); Pf2_mu_t = zeros(m,n);
    Pf_sigma_t = zeros(m,n); Pf2_sigma_t = zeros(m,n);
    
    for i = 1:m
        xp = xpt(:,:,i);               % 获取对应点xp
        fail_points = I_f(:,i);    % 失效点逻辑向量
        fail_xp = xp(fail_points,:);   % 失效点的x向量
    
        Pf_t(i) = sum(f_x(fail_xp)./h_x(fail_xp))/nums;
        Pf2_t(i) = sum(f_x(fail_xp).^2./ h_x(fail_xp).^2); % 2 表示平方, 用于计算后面项; 
    
        Pf_mu_t(i,:)    = 1/nums * sum(fX_mu(fail_points,:,i)./h_x(fail_xp), 1);  % 分量均值灵敏度
        Pf2_mu_t(i,:)   = sum(1./h_x(fail_xp).^2 .* fX_mu(fail_points,:,i).^2, 1);
        
        Pf_sigma_t(i,:) = 1/nums * sum(fX_sigma(fail_points,:,i)./h_x(fail_xp), 1); % 分量方差灵敏度
        Pf2_sigma_t(i,:)= sum(1./h_x(fail_xp).^2 .* fX_sigma(fail_points,:,i).^2, 1);
    end
    clear fail_points fail_xp
    
    Pf = sum(alpha_ .* Pf_t,2);
    msc.Pf_var = sum(1/(nums * (nums -1)) * (sum(alpha_.^2' .* Pf2_t) - nums * Pf_t.^2), 2);
    msc.Pf_cov = sqrt(msc.Pf_var)./Pf;
    % 上面两条公式正确性已经检验, 可放心使用
    
    Pf_mu     = sum(alpha_' .* Pf_mu_t ,1);
    % 公式4.4.3.3 @caution: 正确性可能有误
    msc.Pf_mu_var = 1/(nums * (nums-1)) * sum(alpha_.^2' .* Pf2_mu_t - nums * (alpha_' .* Pf_mu_t).^2 , 1);  % 注意第二项不要丢掉nums
    msc.Pf_mu_cov = sqrt(msc.Pf_mu_var)./abs(Pf_mu);
    
    Pf_sigma  = sum(alpha_' .* Pf_sigma_t, 1);
    % 公式4.4.3.3 @caution: 正确性可能有误
    msc.Pf_sigma_var = 1/(nums * (nums-1)) * sum(alpha_.^2' .* Pf2_sigma_t - nums * (alpha_' .* Pf_sigma_t).^2,1);
    msc.Pf_sigma_cov = sqrt(msc.Pf_sigma_var)./abs(Pf_sigma);
end

```