Optislang 是用于集成化设计以及优化的重要工具, 在 ANSYS Optislang 是用于可靠性分析和优化的重要工具。 也是最常见的 PIDO (Process Integration & Design Optimization Tool) 
  
在 Ansys Optislang 中， 依靠 MOP (MetaModel of Optimal Prognosis) 技术,  用于在**不确定性分析**和**可靠性优化**中高效构建高精度代理模型。 

  

<b><mark style="background: transparent; color: orange">主要特点</mark></b> : 
- **自动选择最佳代理模型**： 对比多种代理模型（如多项式、Kriging、径向基函数RBF、神经网络等），选择预测能力最强的模型。 
- **量化模型可靠性**： 通过**预后系数（Coefficient of Prognosis, CoP）**评估代理模型的预测精度（`CoP=1`表示完美预测，`CoP=0`表示无预测能力）。
- **降维与灵敏度分析**： 结合**PCA（主成分分析）**或**Sobol指数**，识别关键输入变量，减少计算复杂度。 

#### **注意事项** 
- **样本量要求**：初始DOE样本需覆盖设计空间（通常至少10×变量数）。
- **高非线性问题**：可能需要增加Kriging或ANN的样本量。

![[attachments/Pasted image 20250526160827.png]] 


其他部分具有 : 
1.  reliability analysis & robust design 
2. sensitivity analysis & Optimization  

具体如下所示 : 
![[attachments/Pasted image 20250526205733.png|700]]
注意对于 premium 版和 Enterprise 版, 并行的参数化进程不同。 

在 Workbench 中, 左侧部分具有对应的 Optislang  模块， 用于集成 Optislang 分析部分
![[attachments/Pasted image 20250526211022.png|550]]


## 一、 基于 Optislang 的敏感性计算和基础优化法
对于敏感性分析部分， 
1. 可以定义为连续区间，  
2. 定义为离散值， 例如 1; 3; 8; 10; 

![[attachments/Pasted image 20250526212048.png]]

另外, Parameter Type 可以设置为 Dependent, 用于将某个变量用其他另外一个变量进行计算。

转到 Sensitivity Wizard 模块部分， 在右键中， 
![[attachments/Pasted image 20250526212537.png|500]]  

将某些部分设定为对应约束, 同时设定对应变量， 约束以及相应的极限状态函数， 例如， 此处以  `Life_Minimum >= 60` 为相应的极限状态函数， 
![[attachments/Pasted image 20250526213258.png]]

采样方法设置界面 : 
![[attachments/Pasted image 20250526213503.png|400]]
其中第一个是自适应 MOP ， 而默认的 MOP 测试的代理模型有 Polynomial (多项式) 和移动最小二乘 (Moving Least Squares) 模型以及各向同性 Kriging 代理模型均为默认的选择。 
![[attachments/Pasted image 20250526214311.png|450]]
另外， 可以在 Advance 选项中， 使用支持向量机代理模型部分，
![[attachments/Pasted image 20250526214711.png|600]]

对于优化问题部分， 只需加入 Optimization 模块，可通过如下设置,  在寿命大于 60 的情况下， 最小化质量， 进行变量的优化 :
![[attachments/Pasted image 20250526214901.png|600]]
对于Optimization Methods 选择部分， 可以在 Settings 中看出， 已经有内置的优化方法包括自适应响应面方法， 进化算法， 粒子群方法 (PSO), 随机设计提升法， 协方差矩阵自适应方法 (CMA) 等;  
![[attachments/Pasted image 20250526215446.png|700]]
在工程上方， 如果有 Optislang 组件， 可以通过该图标 Go To Optislang , 从而在 optislang 中显示对应工程 : 
![[attachments/Pasted image 20250526221755.png]]

在敏感性分析中， 需要定义 : 
1. 各个变量的上下限控制 
2. 需要多少计算点 (决定计算时间) 

在优化过程中， 最多用的是 MOP (<b><mark style="background: transparent; color: orange">MetaModel of Optimal Prognosis</mark></b>) 的概念， 对于 0 维情况下为 Scalar MOP， 1 维 为 Signal MOP, 另外有 2D Field MOP 或者 3D Field MOP， 实际上是对应0D 是某个值的优化问题， 而1维一般是针对某个一维目标响应量趋向于另外一个或者是优化到目标相应的过程 ;  
![[attachments/Pasted image 20250526231727.png]]

当构建好了代理模型 (pre-built MOP) 之后，可以进行 Pranto 前阵面搜索优化等部分; 

对于如图在 Workbench 中的工作流部分， 如下图部分: 
![[attachments/Pasted image 20250527091058.png|400]]
在 Optislang 中的工作流设定图像如下 : 





得到类似如图的分析结果 (其中， 可以看出 DS_d6 对质量 Mass 影响最大， 而由于重新定位文件导致每次寿命求解结果都是相同的, 实际结果应当正常计算) 
![[attachments/Pasted image 20250527091400.png|600]]

重要的是 COP Matrix (coefficient of Prediction), 是 optislang 用于衡量代理模型（如多项式、Kriging、MLS等）预测精度的关键指标矩阵。

一般地， 可以在优化结果中， 选中某个点并点击 "Use Current design Points", 这样会作为当前设计点， 因此可用于后期如优化等内容。 选用设计点之后， 会自动调整响应面使得对应的设计点放在响应面上。 
![[attachments/Pasted image 20250527093229.png]]

可以调整 Palette Data 以绘制不同的内容 : 
![[attachments/Pasted image 20250527093508.png|650]]

另外上面一排图标部分， 分别绘制的不同图像， 均可以使用和观察 : 
![[attachments/Pasted image 20250527094114.png]]


## 二、 工作流设计流程方法 

上述过程中， 均可以采用 One-Click Optimization 作为 Module 
![[attachments/Pasted image 20250527095014.png|300]]
可以从 Process Chain Elements  中选取对应子流程部分， 从而形成整体流程设计, 另外， 块之间连接只需 右键 Connect 即可 :  
![[attachments/Pasted image 20250527095329.png]]





1.  Parametric Optimization 
2. Topology Optimization 
3. Shape Optimization  
