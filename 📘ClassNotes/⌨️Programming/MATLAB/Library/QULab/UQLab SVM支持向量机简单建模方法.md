# UQLab
UQLab Offers Three sampling based algorithms, including: 
1. Monte Carlo simulation 
2. Importance sampling 
3. Subset Sampling (MCMC simulation)

The Qulab contains the following Surrogate Model, Learning function, Reliability method and **Stopping criterion**:
![[attachments/Pasted image 20240403200341.png|600]]
Note that not only LF-based method and stability of beta is used, also stability of Pf is used. 


> [!NOTE] 附注
> 一般地， 在UQLab中， 都可以在上方右键选择 Open .m as Live Script File, 获取更加方便的示例代码;

# SVM 模型建立最简代码
The most simple code for build a SVM model and predict is as following:
initialization: 
```matlab
%% %%%%%%%%%%%%% Support Vector Machine %%%%%%%%%%%%% 
% set the random number generator for reproducible results,
% and initialize the UQLab framework:
clearvars, clc;
rng(1,'twister')
uqlab
```

对于SVC模型, 需要指定 Type,MetaType, ExpDesign.X, ExpDeisgn.Y 四个参数; 最终调用 uq_createModel 建立模型;
```matlab
% %%%%%%%%%%%%%%% create the SVC metamodel %%%%%%%%%%%%%%%
load("fisher_iris_reduced.mat", "X", "Y");

MetaOpts.Type = 'Metamodel';
MetaOpts.MetaType = 'SVC';
MetaOpts.ExpDesign.X = X;
MetaOpts.ExpDesign.Y = Y;

% ----- following are optional --------------------
% Select a linear penalization:
% MetaOpts.Penalization = 'linear';

% Use the span leave-one-out (LOO) error estimate to calibrate the kernel hyperparameters:
% MetaOpts.EstimMethod = 'SpanLOO';

% Use the cross-entropy method for the optimization:
%  MetaOpts.Optim.Method = 'CE';

% Using the same options above, several SVC metamodels are created using
% different kernel families.

% Select the Gaussian kernel family:
% MetaOpts.Kernel.Family = 'Gaussian';

% Create the SVC metamodel:
model = uq_createModel(MetaOpts);
```

如果直接使用文件读取存储X,Y的话, 可以调用如下代码:
```matlab
MetaOpts.ExpDesign.DataFile = 'mydata.mat' ;
```

另外, 为了将函数进行可视化, 我们常常调用下面两个:
```matlab
uq_print(mySVC_mat);       % 显示建立的代理模型信息
uq_display(mySVC_mat);   % 绘制代理模型图像
```

## 三、常用定义和函数
### (1) 自定义输入范围和输出量
```matlab
IOpts.Marginals(1).Type = 'Uniform';
IOpts.Marginals(1).Parameters = [0 10];
IOpts.Marginals(2).Type = 'Uniform';
IOpts.Marginals(2).Parameters = [0 10];
myInput = uq_createInput(IOpts);

% 使用字符串创建函数
MOpts.mString = 'sign(X_2 - X_1.*sin(X_1) - 1)' ;
myModel = uq_createModel(MOpts);

MetaOpts.Input = myInput;
MetaOpts.FullModel = myModel;
```

这样就能够指定相应的输入范围了
![[attachments/Pasted image 20240427165320.png]]

### (2) 自定义分类函数
分类函数的定义形式为:
The computational model is an analytical function defined as:
$$Y(\mathbf{x}) = \left\{ \begin{array}{ll}
  -1 & \quad Y_{val}(\mathbf{x}) < 0  \\
  +1 & \quad Y_{val}(\mathbf{x}) > 0 \\
\end{array}\right.$$
下面的代码定义了分类函数: 
$$Y_{val}(\mathbf{x}) = x_2 - x_1 \sin(x_1) - 1$$
```matlab
function Y = uq_cmsin( X )
%UQ_MSIN Implementation of msin Function for classification y = x sin x - 1
%   Simple function that is used for testing 1D metamodelling
YX = X(:,2)  - X(:,1).*sin(X(:,1)) - 1;
Y = zeros(size(YX));
Y(YX >=0) = 1;
Y(YX < 0) = -1;
end
```

然后使用ModelOpts.mFile = 'FunctionName'
```matlab
% Create a MODEL object from the function file:
ModelOpts.mFile = 'uq_cmsin';
myModel = uq_createModel(ModelOpts);
```

另外也可以使用字符串定义分类函数:
```
MOpts.mString = 'sign(X_2 - X_1.*sin(X_1) - 1)';
```

### (3) 自定义验证集和误差计算
验证集可以在 MetaOpts.ValidationSet 中进行指定, 在建立模型后会自动计算
![[attachments/Pasted image 20240427162505.png]]

下面这个用于设置求解器, 一般设置为SMO或者IDSA, 且对于大型数据集建议使用 SMO 方法;
![[attachments/Pasted image 20240427164357.png]]

### (4) 自定义核函数

核函数需要超参数 $\theta$ 进行相应的定义, 方法是使用 Kernel.Handle 或者 Kernel.Family 
```matlab fold
%% 3.4 User-defined kernel
%
% A user-defined (custom) kernel is built up as a mixture of a Gaussian
% kernal and a polynomial kernel:
%
% $k(x,x') = \theta_3 \cdot \exp[||x-x'||^2/ ( 2\, \theta_1)^2 ] + (1-\theta_3) \cdot (x \cdot x' + \theta_2 )^5$
% It is defined  through a handle to a function written in an m-file
% (shipped with UQLab):
MetaOpts.Kernel.Handle = @uq_MixedSVRKernel;

%% 
% Initialize the hyperparameters of the kernel:
MetaOpts.Hyperparameters.theta = [0.5 0.5 0];

%%
% Set the bounds on the search space for the hyperparameters calibration: 
MetaOpts.Optim.Bounds.C = [10; 1000];
MetaOpts.Optim.Bounds.theta = [0.1 1 1e-3; 5 5 1];

%%
% Create the SVC metamodel with the custom kernel:
mySVC_cusK = uq_createModel(MetaOpts);

%% 
% Print a report on the main features of the resulting SVC metamodel
% (custom kernel):
uq_print(mySVC_cusK)

%%
% Create a plot of the SVC predictor (custom kernel):
uq_display(mySVC_cusK)
```

核函数定义如下: 
```matlab fold title:核函数自定义示例
function K = uq_MixedSVRKernel(X1,X2,theta,Options)

N1 = size(X1,1) ;
N2 = size(X2,1) ;

if size(X1,2) ~= size(X2,2)
    error('Error: Xi s  in K(X1,X2) must have the same number of marginals!')
end
M = size(X1,2);
if size(theta,1) == 1 && ~isscalar(theta)
    theta = transpose(theta) ;
end

isGramm = (N1 == N2) && isequal(X1,X2);   % Similar to Kriging consider whther K is the Gramm matrix

K1 = ones(N1*N2,1);

[idx2, idx1] = meshgrid(1:N2,1:N1);
if isGramm % if it is a Gramm matrix, we don't need to calculate anything from the diagonal up
    zidx = idx1 > idx2 ;
    idx1 = idx1(zidx) ;
    idx2 = idx2(zidx) ;
else
    zidx = idx1 > 0 ;
end

K1(zidx(:)) = prod(exp(-0.5* (bsxfun(@rdivide,X1(idx1(:),:) - X2(idx2(:),:), theta(1))).^2 ), 2) ;
    %% Reshape K to the original size
    K1 = reshape(K1, N1, N2);
    K1(~zidx) = 0;
    % if it is a covariance, check if we need to add the nugget, as well as add
    % back the upper triangular elements, as well as the main diagonal
    if isGramm
        
        K1 = K1 + transpose(K1) + eye(size(K1));
        
    end
K2 = (X1*X2' + theta(2)).^5 ;
K = theta(3)* K1 + (1-theta(3)) * K2 ;

end
```

另外给出一个示例:
![[attachments/Pasted image 20240427161819.png]]

### (5) 分类预测方法
![[attachments/Pasted image 20240427163509.png]]

基本的函数是 uq_evalModel(model, x) 函数, 可以对新的样本进行分类预测。
```

```
