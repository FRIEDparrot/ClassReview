利用StdVar函数可以将变量化为标准正态变量并且返回归一化后的新的g(z)函数

```matlab
%% %%%%%%%%%%%% 将非变量进行解耦, 并进行变量归一化, 生成标准正态化变量 %%%%%%%%%%%%%%%%%%
function [mu_new, sigma_new, g_new, xp_new] = StdVar(mu_, sigma_, g, xp)
    n = size(mu_, 2);
    % use uncoupled:  y = A(x - mu_x), -> b = -A * mu_x
    [A,sigma_y] = eig(sigma_);  % 求解协方差阵的特征值和特征向量
    A = A';   % 使用的是 y = Ax + b -> sigma_x = A^T sigma_y A, which is A' 
    % mu_y = zeros(1, n);
    sigma_yd = sqrt(diag(sigma_y)');
    
    % --------  g(x) = g(x(u)) --------- 
    mu_new = zeros(1,n);
    sigma_new = diag(ones(1,n));
    % 1. y = A(x - mu_x),  2. z = y/sigma_yd;  -> g(x) = g(A^T * z * sigma_y + mu_x)
    g_new = @(z) g((A' * (sigma_yd'.* z') + mu_')');  % equation 6.2.3.4 
    
    % 说明: 这个其实一般用不着, 因为直接从lhsnorm(0,1)抽取即可
    if nargin == 4
        % zp, which is transformed from xp using: z = A(x-mu_x)/sigma_y
        xp_new = zeros(size(xp));
        for i = 1:size(xp, 1)
            xp_new(i,:) = (A * (xp(i,:) - mu_)')' ./ sigma_yd;
        end
    end
end

```