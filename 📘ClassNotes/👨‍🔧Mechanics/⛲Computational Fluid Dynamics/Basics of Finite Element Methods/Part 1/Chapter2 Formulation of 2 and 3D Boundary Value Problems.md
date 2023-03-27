## 1. The Analysis of 2D FEM problem 
### (1) Ellipse Problem 
##### 1. the derivation of the weak form
we set equation of the 2D FEM problem is a second-order partial differential equation and then it satisfies the **Essential Boundary Conditions(on $\Gamma_1$)** and the **Natural Boundary Conditions (on $\Gamma_2$)** 
$$\begin{cases}
\text{Equation :}\space \space \frac{\partial^2 u}{\partial x^2} + \frac{\partial^2 u}{\partial y^2} =p ,\quad (x,y) \in \Omega \\
\text{boundary condition :} \begin{matrix}
\left. u\right|_{\Gamma_1} = \bar{u} \\
\left. \frac{\partial u}{\partial n}\right|_{\Gamma_2} = g
\end{matrix}
\end{cases}$$
so we can then write the weak form of the problem by the derivation process : 
[[📘ClassNotes/👨‍🔧Mechanics/⛲Computational Fluid Dynamics/Basics of Finite Element Methods/Part 1/Implements/Derivation of the weak form of 2D Ellipse Boundary Problem.pdf|derivation of the weak form of 2D ellipse Boundary Problem.pdf]]
the weak form of the problem is : 
$$\iint_{\Omega} \left(\frac{\partial u}{\partial x} \frac{\partial (\delta u)}{\partial x} + \frac{\partial u}{\partial y}\frac{\partial (\delta u)}{\partial y}\right)d\Omega + \iint_{\Omega} p \delta u d\Omega = \int_{\Gamma_2}g \delta u d\Gamma$$
after obtaining the weak form of the equation, we need to discrete the domain into element to solve :
![[Excalidraw/Chapter2 Formulation of 2 and 3D Boundary Value Problems 2023-03-26 00.19.46|200]]
then we can list the coordinates of the nodes and the boundary condition, including essential boundary condition and the natural boundary condition.

##### 2. The Displacement Function of the Element`
Firstly, we assume that the displacement of each element as a linear form as : 
$$\Phi_i = a_i +b_i x + c_i y\qquad (i = 1,2,3)$$
[[📘ClassNotes/👨‍🔧Mechanics/🕸️FEM Analysis/Structural FEM Analysis/第二章 平面问题#(1) 形状函数|displacement function in structural FEM analysis]]
we set the coordinate of three points of the triangle element is $(x_i ,y_i)$ where $i = 1,2,3$. Also Note that the interpolation function gives that :
$$\Phi_i(x_j^{(e)}, y_j^{(e)}) = \delta_{ij}\quad (i,j = 1,2,3)$$
which is : 
$$\begin{cases}
a_i + b_i x  + c_i y = 1 \\
a_j + b_j x + c_j  y = 0 \\
a_k + b_k x + c_k y = 0 
\end{cases}$$
then we can solve the $a_i, b_i, c_i$ the process is just the same as [[📘ClassNotes/👨‍🔧Mechanics/🕸️FEM Analysis/Structural FEM Analysis/第二章 平面问题#(2) 位移函数的有限元求解方法|the derivation of the coefficient in structural FEM analysis]]
we set : 
$$[\Lambda] = \left[\begin{matrix}
1  & x_i & y_i \\ 
1 & x_j & y_j \\ 
1 & x_k & y_k
\end{matrix}\right]$$
Examples are as follows : 
$$a_i = \frac{1}{D}\left| \begin{matrix}
x_j^{(e)} & y_j^{(e)} \\ 
x_k^{(e)} & y_k^{(e)} \\
\end{matrix} \right| \qquad b_i = \frac{1}{D}\left| \begin{matrix}
1 & y_j^{(e)} \\
1 & y_k^{(e)}
\end{matrix} \right| \qquad c_i = \frac{1}{D}\left| \begin{matrix}
1 & x_j^{(e)} \\
1 & x_k^{(e)}
\end{matrix} \right|$$
where 
$$D = 2\left|\Lambda \right| = 2 A^{(e)}$$
Note in the equation above $A^{(e)}$ is the Area of the triangle
then we can calculate the entire function as : 
$$\boxed{u^{(e)} = \left[\begin{matrix}
1, x , y \end{matrix}\right]\left[\begin{matrix}
a_1 & a_2 & a_3 \\
b_1 & b_2 & b_3 \\
c_1 & c_2 & c_3
\end{matrix}\right] \left[\begin{matrix}
u_1^{(e)} \\ u_2^{(e)} \\ u_3^{(e)}
\end{matrix}\right] = \Phi_1^{(e)} u_1^{(e)} + \Phi_2^{(e)} u_2^{(e)} + \Phi_3^{(e)} u_3^{(e)}= \sum^{3}_{i=1} \Phi_i^{(e)}u_i^{(e)}}\tag{2-27}$$
or write into the tensor form: 
$$\Large\boxed{u^{(e)} = u_i^{(e)} \Phi_i^{(e)}}$$

##### 3. Characteristic Functions of the Element 

by substituting the $u_i^{(e)}$ into [[#1. the derivation of the weak form|the weak form equation]], then using the process of derivation [[📘ClassNotes/👨‍🔧Mechanics/⛲Computational Fluid Dynamics/Basics of Finite Element Methods/Part 1/Implements/Derivation of 2D characteristic functions.pdf|Derivation of 2D characteristic functions.pdf]]

we have 
$$A_{ij}^{(e)} u_j^{(e)} = f_i^{(e)}$$
where 
$$\small A_{ij} = \iint_{\Omega^{(e)}} \left(\frac{\partial \Phi_i^{(e)}}{\partial x} \frac{\partial \Phi_j^{(e)}}{\partial x} + \frac{\partial \Phi_i^{(e)}}{\partial y}\frac{\partial \Phi_j^{(e)}}{\partial y}\right)d\Omega
\qquad \small f_i^{(e)} = -\iint_{\Omega^{(e)}} p \Phi_i^{(e)}d\Omega + \int_{\Gamma_2^{(e)}}g\Phi_i^{(e)}d\Gamma$$
so we can substitute $\Phi_i$ into the equation (derivation process is also on above) 
$$\boxed{A_{ij} = A^{(e)}(b_i b_j + c_i c_j)}$$
$$\boxed{f_i = -\int_{\Omega^{(e)}}p(a_i + b_i x+ c_i y) + I_i}$$
where if we set a boundary with the node 1,2 on $\Gamma_2$
$$\begin{cases}
I_1 = \frac{L}{6}\left[ 2g_2^{(e)} + g_3^{(e)}\right] \\
I_2 = \frac{L}{6} \left[ g_2^{(e)} + 2g_3^{(e)}\right]
\end{cases}$$

##### 4. The Assembly of 2D characteristic equation
$$\boxed{A_{nm} = \sum^{E}_{e=1}\left[ \Delta_{ni}^{(e)}A_{ij}^{(e)} \Delta_{im}^{(e)}\right]}$$
$$\boxed{f_n = \sum^{E}_{e=1} \Delta_{ni}^{(e)} f_i^{(e)}}$$
where the $n,m$ is correspond to the local coordinates $A_{ij}^{(e)}$.
For the example below : 
![[Excalidraw/Chapter2 Formulation of 2 and 3D Boundary Value Problems 2023-03-26 09.53.20|400]]
$A^{(1)}, A^{(2)}$ are all 3x3 matrix, then 
$$A_{mn} = \left[\begin{matrix}
A_{33}^{(1)} & A_{31}^{(1)} &A_{32}^{(1)} & 0\\
A_{13}^{(1)} & A_{11}^{(1)} &A_{12}^{(1)} & 0\\
A_{21}^{(1)} & A_{21}^{(1)} & A_{31}^{(1)}& 0\\
0 &  0 & 0 & 0
\end{matrix}\right] + \left[\begin{matrix}
0 & 0 & 0 & 0 \\
0 & A_{22}^{(2)} & A_{23}^{(2)} & A_{21}^{(2)}\\
0 & A_{32}^{(2)} & A_{33}^{(2)} & A_{31}^{(2)}\\
0 & A_{12}^{(2)} & A_{13}^{(2)} & A_{11}^{(2)}
\end{matrix}\right]$$
where the superscript is the element 

then 
$$f = \left\{ \begin{matrix}
f_3^{(1)} \\ f_1^{(1)} \\ f_2^{(1)} \\ 0
\end{matrix}\right\} + \left\{ \begin{matrix}
0 \\ f_2^{(2)} \\ f_3^{(2)} \\ f_1^{(2)}
\end{matrix}\right\}$$

also the boundary condition of FEM function can be refined through these 2 ways: 
[[📘ClassNotes/👨‍🔧Mechanics/⛲Computational Fluid Dynamics/Basics of Finite Element Methods/Part 1/Chapter 1 One-Dimensional Boundary Value Problem#12. Boundary Condition|The method for deal with essential boundary condition ]]
then we just need to solve the FEM equation later. 

### (2) Nonlinear steady state problem
the equation to solve is the form of fundamental equations in the fluid dynamics. 
$$\alpha\frac{\partial^2 u}{\partial x^2} - u\frac{\partial u}{\partial x} = -p$$
since the coefficient of the second part is not a constant, which <mark style="background: transparent; color: yellow">denotes the convection term</mark>, the equation is not a linear equation. 

using the derivation process : 
[[📘ClassNotes/👨‍🔧Mechanics/⛲Computational Fluid Dynamics/Basics of Finite Element Methods/Part 1/Implements/Derivation of Nonlinear steady problem.pdf|Derivation of Nonlinear steady problem.pdf]]
