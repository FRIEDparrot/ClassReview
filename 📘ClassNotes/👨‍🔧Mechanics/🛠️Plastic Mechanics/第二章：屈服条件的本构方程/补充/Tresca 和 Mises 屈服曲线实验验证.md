#### 一、Lode拉压实验

![[3.弹塑性屈服条件的建立#五、屈服条件的差别]]

双剪应力的$\frac{\sigma_1 -\sigma_3}{\sigma_s}$公式以及证明：
[[Tresca 和 Mises 屈服曲线实验验证 2022-11-22 15.20.18.excalidraw]]

![[Tresca 和 Mises 屈服曲线实验验证 2022-11-22 17.17.01.excalidraw]]

#### 二、Taylor 和 Quinney 拉扭实验

对于薄壁圆管的拉扭问题,有：
$$\sigma_z=  \frac{T}{2\pi r t} ,\qquad \sigma_r = 0, \qquad \tau_{r\theta} = \frac{M}{2\pi r^2 t}$$
此时的主应力为:
$$\begin{cases}
\sigma_1 =  \frac{\sigma_z}{2} + \sqrt{\frac{\sigma_z^2}{4}+ \tau_{\theta z}^2} \\
\sigma_2 = 0 \\
\sigma_3 = \frac{\sigma_z}{2} - \sqrt{\frac{\sigma_z^2}{4}+ \tau_{\theta z}^2}
\end{cases}$$
对应
$$\begin{cases}
S_1 = \frac{1}{6}(\sigma_z +3\sqrt{\sigma_z^2 + 4 \tau_{xz}^2}) \\
S_2 = -\frac{\sigma_z}{3} \\
S_3 = \frac{1}{6}(\sigma_z -3\sqrt{\sigma_z^2 + 4 \tau_{xz}^2})
\end{cases}$$

然后分别写出
1)Tresca屈服条件
$$\tau_{\max} = \frac{1}{2}(\sigma_1 -\sigma_3) = \frac{1}{2}\sqrt{\sigma_z^2 + 4 \tau_{\theta z}^2} = \frac{\sigma_s}{2}$$
即为：
$$(\frac{\sigma_z}{\sigma_s})^2 + 4(\frac{\tau_{\theta z}}{\sigma_s})^2 =1$$
2)Mises屈服条件
$$\tau_{\max} = \frac{\sigma_1 - \sigma_3}{2} = \frac{\sigma_s}{\sqrt{3+ \mu_\sigma^2}}$$
或者
$$J_2 = \frac{1}{6}(2\sigma_z^2 +6\tau_{\theta z}^2) = \frac{1}{3}\sigma_s^2$$
即$$(\frac{\sigma_z^2}{\sigma_s^2}) + 3(\frac{\tau_{\theta z}^2}{\sigma_s^2}) = 1$$
3)双剪屈服条件
$$\frac{1}{4}(\frac{\sigma_z}{\sigma_s}) + \frac{3}{4}\sqrt{(\frac{\sigma_z}{\sigma_s})^2 + 4(\frac{\tau_{\theta z}}{\sigma_s})^2} = 1$$
上述三个条件分别在正xy平面内确定三个椭圆形包络线,做出包络线如图
![[Tresca 和 Mises 屈服曲线实验验证 2022-11-22 18.45.34.excalidraw|300]]


