#### 积分变换的一般形式: 
$$g[f(t)] =  F(w) = \int^{b}_{a} G(t,w) f(t) dt$$
其中$G(t,w)$称为变换的核

#### （一）复习： Fourier级数展开: 

对于周期为$2l$的函数$f(x)$,满足一定条件时，可以展开为Fourier级数
$$\large\begin{matrix}
f(x) = \frac{a_0}{2} + \sum^{\infty}_{n = 1} (a_n \cos\frac{n\pi}{l}x + b_n sin\frac{n\pi}{l} x) \\ 
a_n = \frac{1}{l}\int^{l}_{-l} f(t)cos\frac{n\pi}{l}t  dt  \\
b_n = \frac{1}{l}\int^{l}_{-l} f(t)sin\frac{n\pi}{l}t  dt 
\end{matrix} $$
将上式代入,得到
$$f(x) = \frac{1}{2l} \int^{l}_{-l} f(t)dt +\frac{1}{l} \sum^{\infty}_{n = 1}\int^{l}_{-l} f(t) \cos\frac{n\pi}{l}(x -t) dt \qquad(\star)$$
令$\omega_n = \frac{n\pi}{l} , \qquad \Delta\omega_n = \frac{\pi}{l}$
右边第二项理解为
$$\frac{1}{\pi} \sum^{\infty}_{n = 1}\left(\int^{l}_{-l} f(t) \cos\omega_n(x -t) dt \right) \Delta \omega = \frac{1}{\pi} \sum^{\infty}_{n=1} \varphi_l(\omega_n) \Delta \omega$$
将求和理解为从$n = 0 \to n = \infty$的积分(省去n)
上式转变为
$$f(x) = \frac{1}{2l} \int^{l}_{-l} f(t)dt +\frac{1}{\pi} \int^{\infty}_{0} d\omega \int^{l}_{-l}  f(t) \cos \omega(x -t) dt \tag{2}$$

#### (二)Fourier变换:
由上式$l \to \infty$,则第一项趋于0,消去
得到: 
$$f(x) = \frac{1}{\pi} \int^{\infty}_{0} d\omega \int^{\infty}_{-\infty}  f(t) \cos \omega(x -t) dt$$
将上式写为
$$f(x) = \frac{1}{2\pi} \int^{\infty}_{-\infty} d\omega \int^{\infty}_{-\infty}  f(t) [\cos \omega(x -t) + i \sin\omega(x-t)]dt \overset{euler formula}{ = } \frac{1}{2\pi} \int^{\infty}_{-\infty} e^{i\omega x} d\omega\left[ \int^{\infty}_{-\infty} f(t) e^{-i\omega t}dt \right ]$$
其中通过欧拉公式，将$[\cos \omega(x -t) + i \sin\omega(x-t)]dt$转换为$e^{-i\omega(x-t)}$

因此引入==Fourier变换==: 
$$F(\omega) = \mathcal{F}(F(t)) = \int^{+\infty}_{-\infty}   f(t) e^{-i\omega t} dt   \tag{6-9}$$
==以及Fourier反变换==: 
$$
f(t) = \mathcal{F}^{-1} [F(\omega)] = \frac{1}{2 \pi} \int^{+\infty}_{-\infty}  f(t) e^{-i\omega t} dt
$$
 变换的核$G(\omega,t)$为$e^{-i\omega t}$



