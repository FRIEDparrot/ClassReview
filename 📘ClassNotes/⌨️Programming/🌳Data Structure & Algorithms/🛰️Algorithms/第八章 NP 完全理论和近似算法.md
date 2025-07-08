### (1) 非确定性图灵机
首先，对于多项式复杂度算法， 输入规模为 $n$ 时对应的 计算时间为 $O(n^k)$. 

对于图灵机计算模型, 计算复杂度实际上是未知的,  而提出非确定性图灵机模型 (nondeterministic Turing machine, NDTM). 而在该模型下部分问题能够在多项式时间内解出。

非确定性图灵机 M 是一个七元组 $(Q,T, I, \delta , b , q_0, q_f)$  其中允许参数 $\delta$ 具有不确定性， 并称为<b><mark style="background: transparent; color: orange">移动函数</mark></b>， 对于 $Q\times T^{k}$ 中的每个值  $(q; x_1, \dots x_k)$ 属于 $\delta$ 的定义域时,  则可以在 $\delta(q; x_1 , \dots  x_k)$ 中选择一个作为其函数值。 

如果对于每个输入长度为 n 的可接受的输入串,   接受该输入串的非确定性图灵机 $M$ 的计算路径长度至少为  $n$,  则称其时间复杂度是 $T(n)$ 

### (2) P 类和NP类问题 
两个重要的语言类 P, NP 定义为:
$$P  = \left\{  L|L 为能在多项式时间内被一台 DTM 接受的语言 \right\}$$
$$NP  = \left\{  L|L 为能在多项式时间内被一台 NDTM 接受的语言 \right\}$$
参考 [[📘ClassNotes/⌨️Programming/🌳Data Structure & Algorithms/🛰️Algorithms/理论证明/RAM 计算模型和 RASP 计算模型|RAM 计算模型和 RASP 计算模型]]  

一般地, 对于 NP 完全问题, 常常采用如下策略: 
1. **仅针对问题特殊实例求解**  
2. 动态规划或分支限界法  
3. 概率算法 
4. 求近似解  
5. 启发式方法求解等等 
