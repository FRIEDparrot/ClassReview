for any coordinate system we define
![[Note - Transformation Jacobian 2023-01-30 12.29.42.excalidraw|300]]
in the integral process, we consider the method of the variable substitution. 
$$\int^{x=g(b)}_{x= g(a)} f(x)dx = \int^{u=b}_{u=a}f(g(u)) g'(u) du$$
and in the multivariable calculus, For $x= g(u,v), y =h(u,v)$
$$\iint f(x,y)dy dx = \iint_G f(g(u,v), h(u,v)) J(u,v) dudv$$
where the $J$ is called Jacobian matrix 
and the determinant$(det)$ of the matrix is called Jacobian of $u, v$ for x and y
$$J = \left|
\begin{matrix}
\frac{\partial u}{\partial x} & \frac{\partial u}{\partial y} \\
\frac{\partial v}{\partial x} & \frac{\partial v}{\partial y}
\end{matrix}\right|$$
and can be written as: 
$$\frac{\partial (u,v)}{\partial (x,y)}= J(\frac{u,v}{x,y})$$
the area before and after the transformation can be calculated as follows:
![[Pasted image 20230130125336.png|450]]
Note that cross product of the 2-D vector can be considered as the z-direction is 0
