```matlab
function Pf = DS_solu(mu_, sigma_, g, num_DS, mode)
%DS_SOLU Directional sampling solution (方向抽样法)
%@note: 当维数n = 2, 3时, 计算生成均匀样本, 而 n = 其他时, 采用均匀抽样
%@note: 对于非标准的正态相关变量, DS_solu函数会进行自动解耦计算
%@note: 适用于多失效模式的计算, 只需将g传入为 {g1, g2} 即可
%@param: mode : 0 为串联失效模式, 其他为并联失效模式, 不填默认为串联, 仅多失效模式有用
    if nargin == 4
        mode = 0;
    end
    n = size(mu_, 2); m = size(g, 2);  g_new = cell(1, m);
    for i = 1:m
        [~, ~, g_new{i}] = StdVar(mu_, sigma_, g{i});
    end
    
    if (n == 2)
        theta_ = (0: 2 * pi/num_DS: 2 * pi * (1- 1/num_DS))';
        xp = [cos(theta_), sin(theta_)];
	    clear theta_
    else
        if n == 3
            %%%%%%%%%%% 计算产生的样本点部分(由于生成均匀分布较难，所以比较慢) %%%%%%%%%%%%%%%%%%%
            nums_theta = round(sqrt(num_DS /4 * pi));      % 纬线方向上
		    theta_arr = (- pi/2 : pi/nums_theta : pi/2)';  % 生成对应的 theta_ 取值范围 (num_theta + 1个点)
		    xp = [];
            for i = 1:length(theta_arr)
			    nums_phi = round(2 * nums_theta *  cos(theta_arr(i)));
                if (nums_phi == 0)
				    nums_phi = 1;
				    phi_ = 0;    % 两个端点的部分, 仅生成一个点;
			    else
				    phi_ = (0: 2 * pi/(nums_phi): 2 * pi * (1 - 1/nums_phi))';  % 最后一个是避免重复产生点, 长度为 nums_phi
                end
			    theta_ = theta_arr(i) * ones(nums_phi, 1);   % 生成同纬度的均布圆
			    xpp = [cos(theta_) .* cos(phi_), cos(theta_) .* sin(phi_), sin(theta_)];
			    xp = [xp; xpp];   % 拼接出 xp 数组
            end
	        % 注意: theta_ 必须同一放在后面一个, 否则 reshape 后会导致theta不对应
	        clear theta_ phi_ xpp theta_arr nums_phi nums_theta
        else
            xp_pre = randn(num_DS,n);  % 随机抽取生成相应的方向向量
            %%%%%% 注意, 抽取完成后需要进行规范化, 生成对应的单位向量
            xp = xp_pre ./ sqrt(sum(xp_pre.^2,2));
            clear xp_pre
        end
    end
    % plot3(xp(:,1), xp(:,2), xp(:,3), '.');
    num_DS_real = size(xp, 1); r_arr = zeros(num_DS_real, m);
    sprintf("given sampling number:%d, real sampling number: %d", num_DS, num_DS_real)
    
    opts = optimoptions('fsolve','Display','off');
    for j = 1:m
        for i = 1:num_DS_real
            fun = @(r) g_new{j}(r * xp(i,:));                   % 求解非线性方程零点;
            [r_res, fval, exitflag] =  fsolve(fun,0, opts);  % 以 r = 0开始求解方程
            % output.funcCount 用于获取迭代次数
            if exitflag <= 0 || abs(fval) > 0.1
                % 如果在这个方向上没有求解到, 认为无解即半径很大, 由于chi2(15)已经很小了, 对概率贡献为0
                r_arr(i,j) = 15;
            else
                r_arr(i,j) = r_res;  % 否则认为求解得到结果, 则加入对应半径
            end
        end
    end
    if mode == 0
        r_arr = min(abs(r_arr), [], 2);
    else
        r_arr = max(abs(r_arr), [], 2);
    end
    Pf_arr = (1 - chi2cdf(r_arr.^2 , n))./2;
    Pf = mean(Pf_arr);
end

```



