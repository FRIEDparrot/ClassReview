#### 一、形状改变比能解释

![[Pasted image 20221118193837.png]]
![[Pasted image 20221118193915.png]]

由于应变能为
$$\begin{matrix} W = \frac{1}{2}(\sigma_1 \varepsilon_1 + \sigma_2\varepsilon_2 + \sigma_3\varepsilon_3) \hfill \\ 
= \frac{1}{2E}\left[ \sigma_1^2 + \sigma_2^2 + \sigma_3^2 - 2\nu (\sigma_1\sigma_2+\sigma_2\sigma_3 + \sigma_1\sigma_3) \right] \end{matrix}$$
![[Pasted image 20221118194607.png|500]]
而单位体积的体积变化能为：
（取得方法：将$\sigma_1 = \sigma_2 = \sigma_3 = \sigma_m$代入上式，并将$\varepsilon_1 =... = \varepsilon_m$均代入，得到
$$W_v = \frac{1}{2}\times  3 (\sigma_m\varepsilon_m)$$
$$\begin{matrix}
W_v = \frac{1}{2}(\frac{\sigma_1+\sigma_2 + \sigma_3}{3})(\varepsilon_1 +\varepsilon_2+\varepsilon_3) \\ 
= \frac{1 - 2\nu}{6E}\left[ \sigma_1 + \sigma_2 + \sigma_3\right]^2
\end{matrix}$$
形状改变比能(弹性改变比能/歪斜能)定义为
$$W_d = W - W_v$$
$$\begin{matrix}
W = \frac{1+\nu}{6E}\left[ (\sigma_1-\sigma_2)^2 +(\sigma_2-\sigma_3)^2 + (\sigma_1 -\sigma_3)^2 \right] \\
= \frac{1+\nu}{E}J_2 = \frac{(1+\nu)}{3E}k^2
\end{matrix}$$
故在Mises屈服条件下，形状改变比能不变

#### 二、 八面体切应力不变解释

[[弹性力学]]
$$\tau_8 = \sqrt{\frac{2J_2}{3}} = \frac{\sqrt2}{3} k$$
为不变量

#### 三、统计平均剪应力解释

围绕一点小圆球上的统计切应力
$$ \bar{\tau}= \sqrt{ \sum^{n}_{i=1}\tau_i^2} = \frac{1}{\sqrt{15}}\sqrt{(\sigma_1-\sigma_2)^2 +(\sigma_2-\sigma_3)^2 + (\sigma_1 -\sigma_3)^2 }$$
