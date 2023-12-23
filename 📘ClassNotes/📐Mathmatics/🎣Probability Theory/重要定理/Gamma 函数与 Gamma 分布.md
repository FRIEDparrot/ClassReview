## (1) $\Gamma$ 函数 
$\Gamma$函数定义为(单参数时, $\theta =1$)
$$\Large \Gamma (x)= \int_{0}^{+\infty} t^{x-1} e^{-t} dt$$
复数域中的$\Gamma$函数定义: 
$$\Gamma(z) = \int_{0}^{+\infty} t^{z-1} e^{-t} dt$$
`````ad-note
title: $\Gamma$函数的性质:
collapse: open
1. 对于正整数n, 有:
$$\Gamma(n) = (n-1)!$$
2. 对任意数$x$, 满足: 
$$\Gamma(x + 1) = x\Gamma(x)$$
3. 可以凑出正态积分公式(代换t为$y^2$显然可得)
$$\int_{0}^{+\infty} e^{-x^{2}} dx = \frac{1}{2} \Gamma\left(\frac{1}{2}\right)= \frac{\sqrt{\pi}}{2}$$
`````
## (2) $\Gamma$ 分布及其性质
$\Gamma$分布和$\Gamma$函数的区别是$\Gamma$<mark style="background: transparent; color: red">分布是双参数</mark>的, 因此要避免函数和分布混淆。参考[[📘ClassNotes/📐Mathmatics/🎣Probability Theory/重要定理/常用的概率分布及数学期望和方差|常用的概率分布]], 有: 
**概率密度函数**为:
$$\Large \Gamma(\alpha , \theta)\qquad  f(x) = \begin{cases}
\frac{1}{\theta^{\alpha} \Gamma (\alpha)}x^ {\alpha -1 } e^{- \frac{x}{\theta}} \qquad  x > 0  \\
0 \qquad  \qquad  \qquad  \qquad \space \space \text{others}
\end{cases}$$
并且**满足$\Gamma$分布的可加性**: 当$X\sim \Gamma(\alpha, \theta), Y \sim \Gamma(\beta, \theta)$, 则
$$Z = X + Y \sim \Gamma(\alpha + \beta, \theta)$$
证明参考第二章例题2

beta 函数: 
$$B(\alpha, \beta) = \int_{0}^{1} t^{\alpha-1} (1 -t)^{\beta-1} dt$$
有关系:
$$B(\alpha, \beta) = \frac{\Gamma (\alpha)  \Gamma (\beta)}{\Gamma(\alpha + \beta)}$$
