#### Hilbert变换的概念:  
Hilbert变换是从时域到时域的变换，只针对于实信号

#### 1. 公式: 

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
