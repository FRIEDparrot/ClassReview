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

## 2. The Interpolation Function of FEM method
### (1) Two Basic types of Interpolation Functions 
Lagrange interpolation basic function : 
$$\Phi_i(x_j) =\delta_{ij}$$
Hermit interpolation basic function : 
$$\frac{d^k}{dx^k} H_{li} (x_j) = \delta_{ij}\delta_{lk}\equiv \begin{matrix}
1 \quad i = j \text{ and } l = k \hfill \\ 
0 \hfill
\end{matrix}$$
if the highest order of the derivation in the integral function is $r+1 ^{\text{th}}$ order : 
The interpolation function should *satisfies the **continuity condition***: 
1. the function has the continuity of $r^{\text{th}}$ derivative. 
2. the interpolation function is a $(r+1)^{\text{th}}$ polynomial inside the element, i.e. it has a continuity on the $r^{\text{th}}$ derivative. 

also note the freedom of the element is equal to the number of the nodes of the element. Then we construct the interpolation function according to the freedom of the element. **The number of the term of the interpolation function is equal to the freedom of element**(which is equal to the <mark style="background: transparent; color: yellow">number of the unknown parameters</mark>)

note that the parameter should be chosen considering the **Symmetry method**. which means if there's 8 nodes on a element, the interpolation function term should be : 
$$1, x , y, x^2 , xy , y^2 , x^2 y, xy^2 \quad  \text{or} \quad 1, x , y, x^2 , xy , y^2 , x^3 ,y^3$$

### (2) The 1-D interpolation function of Element 
in the condition that there's 2 nodes in a element, i.e. : 
![[Excalidraw/Chapter2 Formulation of 2 and 3D Boundary Value Problems 2023-03-28 16.36.59|200]]
we set there's $m$ nodes in a element, then the local coordinate of each element be denoted by $\xi$ , then the interpolation function is : 
$$\Phi_i(\xi) = a_i + b_i \xi$$
since the condition satisfies $\Phi_i{\xi_j} = \delta_{ij}$, we have : 
$$\Phi_1 = 1 - \xi\quad  \Phi_2 = \xi $$
in the element of 3 nodes : 

$$\xi = \frac{x - x_i}{x_j - x_i}$$
$$\begin{matrix}
\Phi_1 = \frac{1}{2}\xi(\xi - 1) \\
\Phi_2 = (1- \xi)(1+ \xi) \\
\Phi_3 = \frac{1}{2}\xi(1+\xi)
\end{matrix}$$
![[Excalidraw/Chapter2 Formulation of 2 and 3D Boundary Value Problems 2023-03-28 16.41.29|300]]

### (3) Different Coordinates of the problems  
##### (1) The coordinates of the length in 1-D problem
for the 1-D problem, the equation becomes : 
$$\xi_1 = \frac{L_1}{L} \quad  \xi_2 = \frac{L_2}{L}$$
it's apparently that $\xi_1 + \xi_2 =1$

##### (2) Coordinates of Area in 2-D problem
$$\xi_i = \frac{A_i}{A}$$
then we can just transform the triangle to a standard right-angled triangle : 
![[Excalidraw/Chapter2 Formulation of 2 and 3D Boundary Value Problems 2023-03-28 17.04.41|450]]
<mark style="background: transparent; color: yellow">Note that later, the interpolation function becomes</mark>
$$\Phi_1 = \xi_1 , \Phi_2 = \xi_2, \Phi_3 = \xi_3$$

`````ad-note
title: a Faster method for constructing the interpolation function 
collapse: open

firstly, the interpolation function of element satisfies : 



`````

![[Excalidraw/Chapter2 Formulation of 2 and 3D Boundary Value Problems 2023-03-28 17.25.15|250]]

the construction using the 
$$\Phi(\xi, \eta) = L_i(\xi), N_i(\eta)$$
where $L_i(\xi)$ and $N_i(\xi)$ is the 1-D interpolation function in the $\xi$ and $\eta$ direction.
$$\begin{cases}
L_1 = L_4 = 1-\xi\\
L_2 = L_3 = 1-\eta \\
\end{cases}$$

![[Excalidraw/Chapter2 Formulation of 2 and 3D Boundary Value Problems 2023-03-28 17.31.43|250]]