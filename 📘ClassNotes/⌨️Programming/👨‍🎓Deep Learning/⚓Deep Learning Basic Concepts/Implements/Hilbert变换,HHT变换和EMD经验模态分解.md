#### Hilbert变换的概念:  
Hilbert变换是从时域到时域的变换，只针对于实信号 
#### 1. 公式
$$\mathcal{H}[f(t)] = \hat{f}(t)*\frac{1}{\pi t}  = \frac{1}{\pi} \int^{+\infty}_{-\infty} \frac{f(\tau)}{t - \tau} d\tau = -\frac{1}{\pi} \int^{\infty}_{-\infty} \frac{f(t+\tau)}{\tau} d\tau$$
实质上可以看做是信号$f(t)$与$\frac{1}{\pi t}$卷积


故==Hilbert变换的公式==:
$$\hat{f}(\tau) = \mathcal{H}[f(t)] = \frac{1}{\pi} \int^{+\infty}_{-\infty} \frac{f(\tau)}{t - \tau} d\tau$$
并有==Hilbert反变换公式==:
$$f(t) = \mathcal{H}^{-1}[\hat{f}(t)] = \frac{1}{\pi}\int^{\infty}_{-\infty} \frac{\hat{f}(\tau)}{t- \tau} d\tau$$
#### 2. 解释

对于任意一个实信号，对其进行傅里叶变换，得到相应的频谱，则其频谱满足：
（1）幅度谱偶对称
$$\begin{matrix}
X(\omega) = X(-\omega^*) \\
|X(\omega)| = |X^*(-\omega)|
\end{matrix}$$
（2）相位谱奇对称
$$\arg X(\omega) = -\arg X(-\omega)$$
利用正频率上的幅度谱和相位谱可以表示出负频率上的频谱结构
即：时域形式可以写成：

$$\begin{matrix} x(t)= \frac{1}{2\pi}\int^{\infty}_{-\infty} X(\omega)e^{j\omega t}d\omega = \frac{1}{2\pi}\int^{0}_{-\infty} X(\omega)e^{j\omega t}d\omega + \frac{1}{2\pi}\int^{+\infty}_{0} X(\omega)e^{j\omega t}d\omega \\
\qquad= \frac{1}{2\pi} \int^{\infty}_{0}X(-\omega) e^{j\omega t} d\omega 
 + \frac{1}{2\pi}\int^{+\infty}_{0} X(\omega)e^{j\omega t}d\omega  \hfill \\
 \qquad = Re[\frac{1}{2\pi}\int^{\infty}_{0} 2X(\omega) e^{jwt}d\omega] \hfill
\end{matrix} $$
(3) 由于Hilbert变换公式
$$\hat{f}(\tau) = \mathcal{H}[f(t)] = \frac{1}{\pi} \int^{+\infty}_{-\infty} \frac{f(\tau)}{t - \tau} d\tau$$
可以看做是$f(t)$和$\frac{1}{\pi t}$的卷积（另一个函数的冲击响应为$\frac{1}{\pi t}$），因此
1.对于$\frac{1}{\pi t}$的傅里叶变换，定义为:
$$\frac{1}{\pi t} \Leftrightarrow{}- j \text{ sgn}(\omega) = 
\begin{cases}
-j, \qquad \space \omega \geq 0 \\
j,  \qquad \quad  \omega < 0
\end{cases} $$
Hilbert变换实质上是将信号中的负频率部分相移$90\degree$,正频率部分相移$-90\degree$

利用Hilbert变换，可以构造解析信号（复信号），其中$X(t)$为原信号，$\hat{X}(t)$为变换的信号
$$Z(t) = X(t) + j\hat{X}(t)$$
解析信号的频谱满足：
$$S_z(\omega) = \begin{cases}
4S_x(\omega)  \qquad  \omega \geq 0 \\
0 \qquad \qquad \space \space \omega < 0
\end{cases}$$
`
## 一、经验模态分解法(EMD方法)

基于瞬时频率，==本征模态函数==（Intrinsic Mode Function, MTF）,将信号分解为若干个IMF分量

Norden E. Huang 等人于1998年提出本征函数的概念(Intrinsic Mode Function, IMF)

经验模态分解方法(Empirical mode decomposition, EMD)为经验模式分解方法

## 二、经验模态分解的由来
1. Hilbert 变换公式
参考[[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Implements/Hilbert变换,HHT变换和EMD经验模态分解|Hilbert变换,HHT变换和EMD经验模态分解]], 对于任意信号，我们可以得到它的Hilbert变换$y(t)$为：
$$y(t) = \frac{1}{\pi} \int^{\infty}_{-\infty} \frac{x(\tau)}{t-\tau} d\tau$$
因此利用Hilbert变换的应用, 构造解析函数$z(t)$

$$z(t) = x(t) + jy(t) = a(t)e^{j\varphi(t)}$$
$$a(t) = \sqrt{x(t)^2 + y(t^2)}$$
其中: $\varphi(t)$是相位角
$$\begin{matrix}
\omega(t) = \frac{d\varphi(t)}{dt}\\
f(t) = \frac{1}{2\pi} \frac{d\varphi(t)}{dt}
\end{matrix} 
$$
为瞬时角速度与瞬时频率

- 对于Hilbert变换本身,往往会导致函数失真问题
	- 瞬时频率的失真
	- 瞬时幅值的失真问题

![[Pasted image 20221122231701.png|400]]
![[Pasted image 20221122232446.png|400]]

每次通过变换之后，可能出现瞬时的幅值失真问题

![[Pasted image 20221122234139.png|400]]
![[Pasted image 20221122234301.png|400]]

通过经验分解（EMD）方法，将原始信号分解为多个本征信号（IMF）之和，通过分解为本征模式函数，保证信号瞬时频率具有物理意义

- 本征模式分量（IMF）需要满足的两个条件
	1. 在整个整数序列中，极值点$N_e$的数量与零点数量$N$必须相等，或者最多相差不超过一个
	2. 在任一时间点$t_i$上，信号的极大值包络线$f_{\max}(t)$和局部极小值的下包络线$f_{\min}(t)$的均值为0

EMD的分解流程(每次多次减去包络线的均值)
![[Pasted image 20221122235001.png]]
