## (1) Quasi-Linear partial differential equations 
The governing equations of the flow is either written in the integral forms or partial differential equations. we firstly examine some mathematical properties of the partial differential form of the partial differential equations themselves. 

We will introduce three types of partial differential equations below : 

for quasi-linear equations : 
$$\begin{cases}
a_1 \frac{\partial u}{\partial x} + b_1 \frac{\partial u}{\partial y} + c_1 \frac{\partial v}{\partial x} + d_1\frac{\partial v}{\partial y} = f_1 \\ 
a_2 \frac{\partial u}{\partial x} + b_2\frac{\partial u}{\partial y} + c_2\frac{\partial v}{\partial x} + d_2 \frac{\partial v}{\partial y} = f_2
\end{cases}\tag{3.1}$$
We consider any point in the $xOy$ plane. at any given points, there is a unique value of $u$ and $v$, then we can make a strange statement by *considering any given point in this plane*. For the point $P$ on the plane, we can <mark style="background: transparent; color: yellow">seek the lines through this point</mark> along with **derivates** of $u$ and $v$ are ***indeterminate*** and across which may be ***discontinuous***. these lines are called <mark style="background: #ADCCFFA6;">Characteristic lines</mark>. 
To Find the ***Characteristic lines***, we can write the total differential of the continuous function : 
$$\begin{cases}
du = \frac{\partial u}{\partial x} dx + \frac{\partial u}{\partial y}dy \\
dv = \frac{\partial v}{\partial x} dx + \frac{\partial v}{\partial y}dy
\end{cases}\tag{3.2}$$
![[Excalidraw/Chapter3 Mathematical behavior of partial differential equations 2023-04-07 08.55.00|300]]
then we can write the equation $(4.1)$ and $(4.2)$ together in matrix form : 
$$\left[\begin{matrix}
a_1 & b_1 & c_1 & d_1 \\
a_2 & b_2 & c_2 & d_2 \\
dx & dy & 0 & 0 \\
0 & 0 & dx & dy
\end{matrix}\right]
\left[\begin{matrix}
\frac{\partial u}{\partial x} \\
\frac{\partial u}{\partial y} \\
\frac{\partial v}{\partial x} \\
\frac{\partial v}{\partial y}
\end{matrix}\right] = 
\left[\begin{matrix}
f_1 \\ f_2 \\ du \\ dv
\end{matrix}\right]$$
we can let $[A]$ denote the matrix above. 

Using the <mark style="background: transparent; color: yellow">Cramer's rule</mark>, we can solve it by replacing each column with the right hand side $\left[\begin{matrix} f_1 & f_2 & du & dv\end{matrix}\right]^T$, and  for example, we can use a matrix replace its first column by the right column and solve $\frac{\partial u}{\partial x}$ as below : 
$$B = \left[\begin{matrix}
f_1 & b_1 & c_1 & d_1 \\
f_2 & b_2 & c_2 & d_2 \\
du & dy & 0 & 0 \\
dv& 0 & dx & dy
\end{matrix}\right] \qquad \frac{\partial u}{\partial x} = \frac{|B|}{|A|}$$
However, there's *one major exception* for this problem if $|A| =0$ 

`````ad-todo
title: calculation of |A|
collapse: open
`````

If we expand the determinant and divide each side by $(dx)^2$ then we have 
$$(a_1 c_2 - a_2 c_1)(\frac{dy}{dx})^2 - (a_1 d_2 - a_2 d_1 + b_1 c_2 - b_2 c_1) \frac{dy}{dx} + (b_1d_2 - b_2 d_1)  = 0 $$
we will easily solve the $\frac{dy}{dx}$ as :
$$\frac{-b + \sqrt{b^2 - 4ac}}{2a}$$
where 
$$\begin{matrix}
a = a_1 c_2 - a_2 c_1 \\ 
b = a_1 d_2 - a_2 d_1 + b_1 c_2 - b_2 c_1 \\
c = b_1 d_2 - b_2 d_1
\end{matrix}$$
and we can use $\Delta = b^2 - 4ac$ as the discriminant to ==classify the three different types of the partial differential equations== : 
$$\boxed{\begin{cases}
\Delta  > 0 \qquad  \text{hyperbolic}\quad \text{two real and distinct characteristic lines through the point}\\
\Delta  = 0 \qquad  \text{parabolic}\\
\Delta < 0 \qquad  \text{elliptic}\quad \text{characteristic lines are imaginary}\\
\end{cases}}$$
We note that if the only $|A|$ were  zero, since $\frac{\partial u}{\partial x}$ is not infinite, we have $|B|$ is zero. that is : 
$$|B| = \left|\begin{matrix}
f_1 & b_1 & c_1 & d_1 \\
f_2 & b_2 & c_2 & d_2 \\
du & dy & 0 & 0 \\
dv& 0 & dx & dy
\end{matrix} \right| = 0$$
The expansion of the equation above yields an *ordinary differential equation* in terms of  $du$ and $dv$, which is called ==*the compatibility equation*==. a method for solute this problem is  construct characteristic lines in the $xOy$ space and solve the simpler equations along the characteristics. This technique is called method of  ==characteristics==. 
***method of the characteristics*** is useful for the solution of inviscid flow. however, the method of characteristics is only useful for **solution of hyperbolic partial differential equations**. 

## (2) Eigenvalue Method 

We assume the [[#(1) Quasi-Linear partial differential equations|quasi-linear equation]] are 0 on the right for simplicity : 
$$\begin{cases}
a_1 \frac{\partial u}{\partial x} + b_1 \frac{\partial u}{\partial y} + c_1 \frac{\partial v}{\partial x} + d_1\frac{\partial v}{\partial y} = 0 \\ 
a_2 \frac{\partial u}{\partial x} + b_2\frac{\partial u}{\partial y} + c_2\frac{\partial v}{\partial x} + d_2 \frac{\partial v}{\partial y} = 0 \end{cases}$$
and we define $W$ as the column vector : 
$$W = \left\{\begin{matrix}
u \\ v
\end{matrix}\right\}$$
then we can write the equation above as : 
$$\left[\begin{matrix}
a_1 & c_1 \\ a_2 & c_2 
\end{matrix}\right]\frac{\partial W}{\partial x} + \left[\begin{matrix}
b_1 & d_1 \\ b_2 & d_2 
\end{matrix}\right]\frac{\partial W}{\partial y} = 0$$
we denote the matrix above by $[K]$ and $[M]$ separately and then 
$$\frac{\partial W}{\partial x} + [K]^{-1}[M] \frac{\partial W}{\partial y} = 0$$
then we let $[K]^{-1}[M] = [N]$ , we would have : 
$$\frac{\partial W}{\partial x} + [N] \frac{\partial W}{\partial y} = 0$$
`````ad-todo
title: The Three different types of equations
collapse: open
`````

