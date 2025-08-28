
```matlab title:MIAK_modelbuild.m
function [dmodel, pref, msc] = MIAK_modelbuild(mu_, sigma_, g, initial_point, fx, params)
% MIAK_modelbuild : build the Meta-IS-AK submodel for the given performance function
% 
% Inputs :
%    mu_: 均值
%    sigma_          :       co-variance matrix 
%    g               :       功能函数 (performance function)
%    initial_point   :       MCMC 迭代初始点, 一般只需按照经验选取失效域中的点
%    fx    (optional):       分布函数, 不填默认使用正态分布代替, 即 fx = @(x) joint_pdf(x, mu_,sigma_d);
%    params(optional):       参数调整设置, 可以参考 MIAK_paraminit 函数, 不填使用该函数进行初始化
%
% Output : 
%    dmodel: 用于预测的功能函数
%    pref:   struct with performance information derived from dacefit
%    msc:    使用IS方法计算的失效概率和灵敏度数据
% note: 仅适用于独立正态变量的失效概率和灵敏度求解
    n = size(mu_, 2);
    sigma_d = sqrt(diag(sigma_))';
    
    if nargin < 5
        fx = @(x) joint_pdf(x, mu_, sigma_d);
    end
    if nargin < 6
        params = MIAK_parainit(n);
    end
	
	x_train = lhsnorm(mu_, sigma_, params.num_MKIS_init);    % 生成随机样本点
	y_train = g(x_train);                   

	% 抽样一次, 用于估计扩展失效概率
	x_temp = slicesample(mu_, 2e4,"burnin",500, "pdf",fx, 'thin', 5);   % lhsnorm(mu_, sigma_, 2e4)
	
	for epoch = 1:params.max_epoch1
		[dmodel, ~] = dacefit(x_train, y_train,params.regr,params.corr,params.theta0,params.lob,params.upb);
		% 可以首先通过MCS抽样确定扩展失效概率 Pf_epsilon 的大小
		Pf_epsilon = mean(fx_pred(x_temp, dmodel));         % 求解扩展失效概率 Pf_epsilon 的值;
		
		% 在求解过程中,  pi(x) -> 直接通过 fx_pred 求解得到
		fx_pdf = @(x) fx(x);                                % 原始变量的概率密度函数 
		px_pdf = @(x) fx_pred(x, dmodel);       		    % 修正项 pi(x)
		hx_pdf = @(x) px_pdf(x) .* fx_pdf(x) ./ Pf_epsilon;     % 新的重要抽样函数概率密度 h(x)
		
		% 以hx为概率密度 -> 根据经验取失效域中某一点为起始点进行 Marklov 抽样 (初始点影响不大)
		[xp_mcmc] = slicesample(initial_point,params.num_MCMC,"burnin",1000, "pdf",hx_pdf, "thin", 3);
		% 此处是以h(x)为密度函数抽样的, 同时把 predictor 代入到了h(x)中进行抽样
		
		% 用交叉验证方法(cross validation method)求解 \hat{a_corrLoo}判定是否收敛, 否则增加样本点进行迭代计算 
		a_LOSS = crossloss(x_train, y_train, dmodel);
		
		if (a_LOSS > params.a_LOSS_min && a_LOSS < params.a_LOSS_max)
			break;
		else
			fprintf("refine epoch:%d, alpha_Loss : %f\n", epoch,a_LOSS)
			% 通过kmeans算法,计算形心, 
			[~, C] = kmeans(xp_mcmc, params.nKMeans);
			x_train = [x_train; C];
			y_train = [y_train; g(C)];
		end
	end
	clear fx_pdf px_pdf C xp_mcmc x_temp;

	if epoch < params.max_epoch1
		fprintf("\nSampling function meta-model build successfully\n")
		fprintf("total epoch for meta-model: %d\n", epoch)
		fprintf("alpha Loss is %.4f\n", a_LOSS)
		fprintf("Pf_epsilon = %f\n\n", Pf_epsilon)
	else
		warning("reach maximum epoch %d, model may be not correct", params.max_epoch1)
		fprintf("alpha Loss is %.4f\n", a_LOSS)
	end
	clear epoch a_LOSS Pf_epsilon

	%% %%%%%%%%%%% 使用 AK-IS 方法构建更加精细的 Kriging 模型 %%%%%%%%%%%%%%%%
	x_test = slicesample(initial_point,params.num_ISAK,"burnin",1000, "pdf", hx_pdf, "thin", 5);
	% 注意: 其中是以 hx_pdf 为重要抽样函数的

	for epoch = 1:params.max_epoch2
		[dmodel, pref] = dacefit(x_train, y_train, params.regr, params.corr, params.theta0, params.lob, params.upb);
		[mu_g, sigma_g] = predictor(x_test, dmodel);  % -> 此时的模型已经具有一定的精度
		U_x = U_LearningFunc(mu_g, sigma_g);
		[U_min, index_min] = min(U_x);
		
		fprintf("total sample number: %d, min U_x: %f\n", size(x_train, 1), U_min)
		if (U_min < 2)
			% 加点, 取 xp 中 U_x 最小的点加入
			x_train = [x_train; x_test(index_min,:)];
			y_train = [y_train; g(x_test(index_min,:))];
			x_test(index_min,:) = [];
		else
			break;
		end
	end

	if epoch < params.max_epoch2
		fprintf("\nsubmodel build successfully\n")
		fprintf("total epoch number for refine the submodel: %d\n", epoch)
	else
		warning("reach maximum epoch %d, the submodel may be not precise\n", params.max_epoch2)
	end
	clear epoch
	%%%%%%%%%%%% 直接利用 IS 方法求解重要抽样计算模型失效概率以及局部灵敏度 %%%%%%%%%%%%%%%%%
	x_pred = slicesample(initial_point,params.num_ISAK,"burnin",1000, "pdf", hx_pdf, "thin", 5);
	y_pred = dacepredict(x_pred, dmodel);
	If = (y_pred < 0);

	msc.Pf = sum(If .* fx(x_pred) ./ hx_pdf(x_pred)) ./ params.num_ISAK;
	msc.Pf_var = 1./(params.num_ISAK -1) .* (1./params.num_ISAK .* sum(If.* (fx(x_pred).^2 ./ hx_pdf(x_pred).^2), 1) - msc.Pf.^2);
	msc.Pf_cov = sqrt(msc.Pf_var) ./ msc.Pf;

	fX_mu = ((x_pred - mu_)./ sigma_d.^2) .* fx(x_pred);
	fX_sigma = (((x_pred - mu_)./ sigma_d).^2 -1) .* fx(x_pred)./sigma_d;
	msc.Pf_mu = 1/params.num_ISAK .* sum(If .* fX_mu./hx_pdf(x_pred), 1);
	msc.Pf_sigma = 1/params.num_ISAK .* sum(If .* fX_sigma./hx_pdf(x_pred),1);
    msc.hx_pdf = hx_pdf;
end 

%% %%%%%%%%%%%%%%%%%%%%%%% functions %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% @brief: U 学习函数
function U_x = U_LearningFunc(mu_g, sigma_g)
    U_x  = abs(mu_g)./sqrt(sigma_g);
end

% @brief: pi(x) 值的计算, 即返回各个项失效的概率
function px = fx_pred(xp, dmodel)
    [mu_g,sigma_g] = dacepredict(xp, dmodel); % 单个输入变量, 获取方差(用于提供hx_pdf作为抽样函数)
    px = normcdf(- mu_g ./sqrt(sigma_g));     % 直接通过分布函数取均值获取 pi(x) 的每一项
end

% @brief: 计算修正因子alpha_corr的留一法估计值
function a_corrLoo = crossloss(x_train, y_train, dmodel)
	m = size(x_train, 1);
	If = (y_train < 0);	
	a_i = zeros(m, 1);
	for i = 1:m
		If_temp = If; If_temp(i,:) = [];
		Pi_temp = fx_pred(x_train, dmodel); Pi_temp(i,:) = [];
		a_i(i) = sum(If_temp .* Pi_temp) ./ m;
	end
	a_corrLoo = mean(a_i);
end
```


```matlab title:MIAK_parainit(初始化默认参数)
function params = MIAK_parainit(n)
%% intitialize a default params struct for MIAK_modelbuild function 
%% n: number of the variables 
	
	% params for first model to build and optimize 
	params.num_MKIS_init = 30;   	     % 初始样本点 m0
	params.max_epoch1 = 10;              % 第一次代理模型优化的最大数量
	params.nKMeans = 10;			     % 优化过程每次通过KMeans添加的点数
	params.num_MCMC = 1e4;               % 更新初始代理模型时每次抽取的点
	params.a_LOSS_min = 0.1;             % 初始模型接受阈值a_LOSS下界
	params.a_LOSS_max = 10;			     % 初始模型接收阈值a_LOSS上界

	% params for submodel build (dace.dacefit)
	params.regr = @regpoly0;             % regr function 
	params.corr = @corrgauss; 		     % corr function 
	params.theta0 = ones(1,n);		     % theta definition 
	params.lob  =  1e-5 .* ones(1,n);
	params.upb  = 100 .* ones(1,n);
	
	% params for optimization of submodel in iteration process 
	params.num_ISAK   = 2e3;         % ********* 注意: 这个会影响收敛性, 所以不要调特别高*****
	params.max_epoch2 = 1000;        % ****** 从ISAK样本中最大添加样本数量(< num_ISAK) ****** 
end

```


```matlab title:joint_pdf.m(联合概率密度函数)
function f_x = joint_pdf(x, mu_,sigma_d)
% brief: 求解多变量的联合概率密度函数(normpdf)
    f_x = prod(1./sqrt(2 .* pi .* sigma_d.^2).* exp(-0.5 .* ((x - mu_)./(sigma_d)).^2), 2);
end
```

使用代码示例如下:
```matlab
clear, clc;
%% --------------- example 1 ----------------------------------------- 
mu_ = [0, 0];  sigma_d = [1, 1]; sigma_ = diag(sigma_d.^2);
c = 2;
g1 = @(x) c - 1 - x(:,2) + exp(- x(:, 1).^2 ./ 10) + (x(:,1)./5).^4;
g2 = @(x) c.^2 ./ 2 - x(:,1) .* x(:,2);
g = @(x) min([g1(x), g2(x)],[],2);
initial_point  = [-4, 4];             % 提供初始样本点(失效域内即可)
[dmodel, pref,msc]= MIAK_modelbuild(mu_, sigma_,g, initial_point);
fprintf("Pf = %f", msc.Pf)

%% --------------- example 2 -----------------------------------------
mu_ = [2,2,2,1]; 
sigma_d = [0.2, 0.2, 0.2, 0.25]; 
sigma_ = diag(sigma_d.^2);

g1 = @(x) 2 .* x(:,1) + 2 .* x(:,3) - 4.5 .* x(:,4);
g2 = @(x) 2 .* x(:,1) + x(:,2) + x(:,3) - 4.5 .* x(:,4);
g3 = @(x) x(:,1) + x(:,2) + 2 .* x(:,3) - 4.5 .* x(:,4);
g4 = @(x) x(:,1) + 2.* x(:,2) + x(:,3) - 4.5 .* x(:,4);
g = @(x) min([g1(x), g2(x), g3(x), g4(x)],[],2);
fx = @(x) joint_pdf(x, mu_, sigma_d);
params = MIAK_parainit(4);
params.max_epoch1 = 20;
params.nKMeans    = 15;

initial_point  = [1, 1, 1, 1];           % 提供初始样本点
[dmodel, pref, msc]= MIAK_modelbuild(mu_, sigma_,g, initial_point, fx, params);
fprintf("Pf = %f", msc.Pf
```
