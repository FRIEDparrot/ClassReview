对于极坐标变换，有：
$$x = \rho \sin \theta \qquad  y = \rho \cos \theta $$
则变换为
$$\begin{cases}
 u = \rho  = \sqrt{x^2 + y^2} \\
v = \theta = \arctan{\frac{y}{x}} 
\end{cases}$$
[[极坐标变换的Jacobi矩阵 2023-02-05 12.20.06.excalidraw|求解过程]]
求解得到，对于使用$\rho, \theta$表达的式子
$$J =\frac{\partial (x,y)}{\partial (u,v)} = \rho$$
并代入[[第十章 重积分#(3) 二重积分的换元法|重积分的换元公式]]，有
$$\iint_D f(x,y)dxdy =\iint_D f[\rho \sin\theta , \rho \cos \theta] \rho d\rho d\theta$$

