
```matlab title:线抽样法封装函数
function Pf = LS_solu(mu_, sigma_, g, num_LS)
%LS_SOLU 获取功能函数在输入变量均为独立正态分布下的线抽样法的失效概率(改进线抽样方法)
    % *** 先将变量调用StdVar进行归一化 *** , 获取新的mu_, sigma_, g;
    [mu_new, sigma_new, g_new] = StdVar(mu_, sigma_, g);  % xpp = lhsnorm(mu_, sigma_, num_LS,"on");
    xp = lhsnorm(mu_new, sigma_new, num_LS);              % [mu_new, sigma_new, g_new, xp] = StdVar(mu_, sigma_, g, xpp);
    
    % 归一化完毕之后, 再求解设计点
    [x_ii, ~, ~] = AFOSM_solu(mu_new, sigma_new, g_new); % obtain design point by AFOSM method
    sigma_d_new = sqrt(diag(sigma_new))'; x_i = (x_ii - mu_new)./sigma_d_new;
    clear xii;
    
    % use standarlized variable to derive the  important sampling direction
    alpha_ = x_i./sqrt(sum(x_i.^2)); % use standarlized vector as direction

    clear g xpp
    % @ caution 点积和直接相乘区别, 点积需要使用相乘再相加求解, 直接相乘只是相加之后是模, 方向并不相同
    % 例如: xp_tau = xp - sum(xp .* alpha_, 2) .* alpha; 错误代码 xp_tau = xp - xp.* alpha_ 
    xp_tau = xp - sum(xp .* alpha_, 2) .* alpha_;      % perpendicular weight of xp
    xp_r = xp - xp_tau;                                % tangential weight of xp
    coef_alpha = sqrt(sum(xp_r.^2,2)).* sign(sum(xp_r .* alpha_, 2));  % derive the length coef of xp
    coef_interp = [0.3, 0.7, 1];                               % interpolation coefficient
    m = size(coef_interp, 2);

    cj_res = zeros(num_LS, 1);
    for i = 1:num_LS
        cj_interp = coef_alpha(i) * coef_interp; % c_j interpolation
		yp_interp = zeros(1, m);
        for j = 1:m
		    xp_temp = xp_tau(i,:) + cj_interp(j) .* alpha_;  % x_p calculated (alpha->important vector)
            yp_interp(j) = g_new(xp_temp);                      
        end
        clear xp_temp
        p = polyfit(yp_interp, cj_interp, 2);  % use 2 order-function for interpolation
        % @note: p returns the 3 coefficients returned;
        cj_res(i) = p(3);  % for yp = 0, derive the correspond cj using interpolation
        clear p  cj_interp
    end
    Pf = mean(normcdf(-cj_res));          % obtain the failure probablity
end
```


