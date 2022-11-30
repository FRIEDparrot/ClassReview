### 一、弹性力学基本问题分类

1. Boussinesq问题
2. Hertz接触问题
3. 热应力问题
4. 弹性波问题


Boussinesq问题

[[Boussinesq问题 2022-11-30 23.49.51.excalidraw|平衡方程的推导过程]]
1. 平衡方程: 

$$\large\begin{cases}
\frac{\partial \sigma_\rho}{\partial \rho} + \frac{\partial \tau_{\rho z}}{\partial z} + \frac{\sigma_\rho - \sigma_\varphi}{\rho} + f_x = 0 \\
\frac{\partial \tau_{\rho z}}{\partial x} + \frac{\partial \sigma_z}{\partial z} + + \frac{\tau_{\rho z}}{\rho} + f_z = 0
\end{cases}$$
2. 应变位移关系式:

如图(在圆柱的剖面上)建立坐标系
![[Boussinesq问题 2022-12-01 02.04.56.excalidraw|300]]
容易得到
$$\varepsilon_\rho = \frac{\partial u}{\partial \rho} \qquad \varepsilon_z = \frac{\partial w}{\partial z} \qquad \varepsilon_\varphi = \frac{u}{\rho} \qquad \gamma_{\rho z} = \frac{\partial u}{\partial z} + \frac{ \partial w}{\partial \rho}$$

---
中间内容之后补充

---

得到：
$$\begin{cases}
\frac{1}{1 -2 \nu}\frac{\partial \theta}{\partial\rho} + \nabla^2 u- \frac{u^2}{\rho} = 0 \\
\frac{1}{1 -2 \nu}\frac{\partial \theta}{\partial z} + \nabla^2 w = 0 
\end{cases} \qquad \qquad \text{不计体力}$$
其中
$$\nabla^2 = \frac{\partial}{\partial \rho^2} + \frac{1}{\rho} \frac{\partial}{\partial \rho} + \frac{\partial^2}{\partial z^2}$$
借助于勒夫(Love)函数来求解上述方程
