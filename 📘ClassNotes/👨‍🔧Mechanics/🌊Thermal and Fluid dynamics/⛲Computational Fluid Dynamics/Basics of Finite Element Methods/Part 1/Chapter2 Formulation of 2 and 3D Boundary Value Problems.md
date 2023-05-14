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
[[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/⛲Computational Fluid Dynamics/Basics of Finite Element Methods/Part 1/Implements/Derivation of the weak form of 2D Ellipse Boundary Problem.pdf|derivation of the weak form of 2D ellipse Boundary Problem.pdf]]
the weak form of the problem is : 
$$\iint_{\Omega} \left(\frac{\partial u}{\partial x} \frac{\partial (\delta u)}{\partial x} + \frac{\partial u}{\partial y}\frac{\partial (\delta u)}{\partial y}\right)d\Omega + \iint_{\Omega} p \delta u d\Omega = \int_{\Gamma_2}g \delta u d\Gamma$$

`````ad-caution
title: the test function $\delta u$
collapse: open

Note that here we choose $\delta u$ as the test solution, in this case, if a boundary condition is given as essential boundary condition(not a natural boundary condition), we set $\delta u$ as zero. 

`````

`````ad-attention 
title: The Gauss's law for 1-D problem 
collapse: open
note that we derive from the pdf above that the **Gauss's equation for the 2-D problem** is: 
$$\iint_{\Omega} \nabla\cdot \vec{F} d\Omega = \oint_{\Gamma} \vec{F}\cdot \vec{n}_e ds$$
`````

after obtaining the weak form of the equation, we need to discrete the domain into element to solve :
![[Excalidraw/Chapter2 Formulation of 2 and 3D Boundary Value Problems 2023-03-26 00.19.46|200]]
then we can list the coordinates of the nodes and the boundary condition, including essential boundary condition and the natural boundary condition.

##### 2. The Displacement Function of the Element`
Firstly, we assume that the displacement of each element as a linear form as : 
$$\Phi_i = a_i +b_i x + c_i y\qquad (i = 1,2,3)$$
[[📘ClassNotes/👨‍🔧Mechanics/🖥️Computational_Mechanics/🕸️FEM Analysis/Structural FEM Analysis/第二章 平面问题#(1) 形状函数|displacement function in structural FEM analysis]]
we set the coordinate of three points of the triangle element is $(x_i ,y_i)$ where $i = 1,2,3$. Also Note that the interpolation function gives that :
$$\Phi_i(x_j^{(e)}, y_j^{(e)}) = \delta_{ij}\quad (i,j = 1,2,3)$$
which is : 
$$\begin{cases}
a_i + b_i x  + c_i y = 1 \\
a_j + b_j x + c_j  y = 0 \\
a_k + b_k x + c_k y = 0 
\end{cases}$$
then we can solve the $a_i, b_i, c_i$ the process is just the same as [[📘ClassNotes/👨‍🔧Mechanics/🖥️Computational_Mechanics/🕸️FEM Analysis/Structural FEM Analysis/第二章 平面问题#(2) 位移函数的求解方法|the derivation of the coefficient in structural FEM analysis]]
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

by substituting the $u_i^{(e)}$ into [[#1. the derivation of the weak form|the weak form equation]], then using the process of derivation [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/⛲Computational Fluid Dynamics/Basics of Finite Element Methods/Part 1/Implements/Derivation of 2D characteristic functions.pdf|Derivation of 2D characteristic functions.pdf]]

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
[[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/⛲Computational Fluid Dynamics/Basics of Finite Element Methods/Part 1/Chapter 1 One-Dimensional Boundary Value Problem#12. Boundary Condition|The method for deal with essential boundary condition ]]
then we just need to solve the FEM equation later. 

### (2) Nonlinear steady state problem
the equation to solve is the form of fundamental equations in the fluid dynamics. 
$$\begin{cases}
\alpha\frac{\partial^2 u}{\partial x^2} - u\frac{\partial u}{\partial x} = -p \\
u|_{\Gamma_1} = u_1 \\
\alpha \frac{\partial u}{\partial x}|_{\Gamma_2} = g
\end{cases}$$
since the coefficient of the second part is not a constant, which <mark style="background: transparent; color: yellow">denotes the convection term</mark>, the equation is not a linear equation. 

using the derivation process : 
[[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/⛲Computational Fluid Dynamics/Basics of Finite Element Methods/Part 1/Implements/Derivation of Nonlinear steady problem.pdf|Derivation of Nonlinear steady problem.pdf]]

we can derivate the weak form of the problem as : 
$$\int_{\Omega} \left[\alpha \frac{\partial u}{\partial x}\frac{\partial (\delta u)}{\partial x} - \frac{u^2}{2} \frac{\partial (\delta u)}{\partial x} - p \delta u \right]d\Omega = \int_{\Gamma_2}\left[\alpha \frac{\partial u}{\partial x} - \frac{u^2}{2}\right]\delta u d\Gamma$$
then the characteristic equation is that : 
$$\Large A_{ik}^{(e)} u_i^{(e)} + B_{ijk}^{(e)} u_i^{(e)}u_j^{(e)} = f_k^{(e)}$$

where : 
$$A_{ik}^{(e)} = \int_{\Omega^{(e)}} \alpha\frac{\partial \Phi_i}{\partial x} \frac{\partial \Phi_k}{\partial x}d\Omega$$
$$B_{ijk}^{(e)} = -\frac{1}{2}\int_{\Omega^{(e)}} \Phi_i^{(e)} \Phi_j^{(e)} \frac{\partial \Phi_k^{(e)}}{\partial x} d\Omega + \frac{1}{2}\int_{\Gamma_2^{(e)}} \Phi_i^{(e)} \Phi_j^{(e)} \Phi_k^{(e)} d\Gamma$$
$$f_k^{(e)} = \int_{\Omega^{(e)}} p \Phi_k^{(e)} d\Omega + \int_{\Gamma_2^{(e)}} g\Phi_k^{(e)}d\Gamma$$
the assembly of the equation should be : 
$$\Large A_{nm} u_m + B_{nmk} u_m u_k = f_n \quad (n = 1,2 ...N)$$
where $m,n$ is correspond to the index $i$ and $j$

### (3) Unsteady problems 
we set an example by using the diffusion equation of the gas : 
$$\begin{cases}
\frac{\partial u}{\partial t} - \alpha \frac{\partial^2 u}{\partial x^2} = 0 \\ 
u|_{t= 0} = u_0 \\
u|_{\Gamma_1} = u_1 \\
\alpha \frac{\partial u}{\partial x}|_{\Gamma_2} = g
\end{cases}$$
then we can write the weak form using :[[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/⛲Computational Fluid Dynamics/Basics of Finite Element Methods/Part 1/Implements/Weak form of unsteady problems.pdf|Weak form of unsteady problems.pdf]]
$$\int_\Omega \left(\frac{\partial u}{\partial t}\delta u  + \alpha \frac{\partial u}{\partial x}\frac{\partial (\delta u)}{\partial \Gamma x}\right)d\Omega = \int_{\Gamma_2} \alpha \frac{\partial u}{\partial x}\delta u d\Gamma$$
There are two forms to set the $\Phi$ in the displacement function : 
$$u^{(e)} =u_i^{(e)}\Phi_i(x,t) \quad \text{and}\quad u^{(e)} = u_i^{(e)}(t)\Phi_i(x)$$

>  In the first form above, the basic function is the variable of the time and space $(t, x)$ and the the <mark style="background: transparent; color: yellow">function value on the node</mark> is constant. 

>  In the second form above, the basic function $\Phi_i(x)$ is the function that vary with space , and the function value $\Phi$ is a constant **which not vary with time**. and then the <mark style="background: transparent; color: yellow">function value on the node is the function of time</mark> 

$$\Large A_{ij}^{(e)} \dot{u}_j^{(e)} + B_{ij}^{(e)}u_j^{(e)} = f_j^{(e)}$$
where 
$$\Large\begin{cases}
A_{ij}^{(e)}  = \int_{\Omega^{(e)}} \Phi_i \Phi_j d\Omega \\
B_{ij}^{(e)}  = \int_{\Omega^{(e)}} \alpha \frac{d\Phi_i}{dx} \frac{d\Phi_j}{dx} d\Omega \\
f_j^{(e)} = \int_{\Gamma_2} g\Phi_j^{(e)} d\Gamma
\end{cases}$$
> note $f_j = 0$ if the element is not at the boundary $\Gamma_2$

finally, we assembly each equation together, and then we form the ***Global Finite Element Equation*** as 
$$\Large\boxed{A_{mn} \dot{u}_n + B_{mn}^* u_n = f_m^*}$$
where the $B_{mn}^*$ and the $f_m^*$ is the matrix and vector refined by [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/⛲Computational Fluid Dynamics/Basics of Finite Element Methods/Part 1/Chapter 1 One-Dimensional Boundary Value Problem#12. Boundary Condition|The method for deal with essential boundary condition ]]

## 2. The Interpolation Function of FEM method
The thought of the finite element method is to <mark style="background: transparent; color: yellow">discrete the field into small piece</mark> and then <mark style="background: transparent; color: yellow">use approximate method</mark> for each piece. which use the linear combination of the ***Basis Function*** to approximate the problems. 

### (1) The Shape Function of the Element 
The Shape function of the element is  determined by : 
1. The geometry shape of the Element. 
2. The Node number in the Element.

The often-used elements in the 1D and 2D problems are shown as follows: 
![[Excalidraw/Chapter2 Formulation of 2 and 3D Boundary Value Problems 2023-03-30 19.31.26|450]]

### (2) Two types of Interpolation Basis Functions 
(1) <mark style="background: transparent; color: yellow">Lagrange interpolation basis function</mark> : 
$$\Phi_i(x_j) =\delta_{ij}$$
where the $\Phi_i$ is the interpolation basis function of the Node $i$. which is : for the line section above, we have : 
$$\begin{matrix}
\Phi_1(x_1) = 1 , & \Phi_1(x_2) = 0 \\
\Phi_2(x_1) = 0, & \Phi_2(x_2) = 1
\end{matrix}$$
then the ***approximation function of Displacement*** is 
$$\Large\boxed{u = u_i \Phi_i}$$
> Note that the Lagrange interpolation function may not have the continuity of the First derivate 

(2) Hermit interpolation basic function : 
$$\frac{d^k}{dx^k} H_{li} (x_j) = \delta_{ij}\delta_{lk}\equiv \begin{matrix}
1 \quad i = j \text{ and } l = k \hfill \\ 
0 \hfill \end{matrix}$$
**In the equation above**, the $H_{li}$ is the $l^{th}$ <mark style="background: #FF5582A6;">order</mark> interpolation <mark style="background: #FFF3A3A6;">polynomial</mark> of $i^{th}$ Node 
where $l = 0,1,2, .... K$. ($K$ is the maximum order of the derivate) 

![[Excalidraw/Chapter2 Formulation of 2 and 3D Boundary Value Problems 2023-03-30 20.02.49|300]]
For example, for a line section with three nodes, the interpolation basis function is : 
$$H_{01}\space  H_{11}....H_{K1},\quad  H_{02}\space H_{12} .....H_{K2}, \quad H_{03} \space H_{13}...H_{K3}$$
Then we use a two-node element as example, if the node has its $1^{st} \text{ and }2^{rd}$ derivates given, then the ***approximation function of Displacement*** is : 
![[Excalidraw/Chapter2 Formulation of 2 and 3D Boundary Value Problems 2023-03-30 20.19.58|150]]
$$u = u_1 H_{01} + u_2 H_{02} + u_1' H_{11}+ u_2'H_{12} + u_1''H_{21} + u_2'' H_{22}$$

> Different from the Lagrange interpolation basis function, the Hermit interpolation function has the continuity of the $K^{th}$ order derivative 

`````ad-note
A reasonable polynomial interpolation basis function must have the properties below : 

$\space$

if the **highest order of the derivation in the integral function** is $r+1 ^{\text{th}}$ order,The interpolation function should satisfy the **continuity condition** as follows:

1. the Hermit interpolation function<mark style="background: transparent; color: yellow"> has the continuity of</mark> $r^{\text{th}}$ derivative. (also this is called **the continuity of $C^r$**)
2. the interpolation function is at least $(r+1)^{\text{th}}$ polynomial inside the element, i.e. it has a continuity on the $r^{\text{th}}$ derivative. 
`````

also note the ==freedom of the element is equal to the number of the nodes of the element==. Then we construct the interpolation function according to the freedom of the element. **The number of the term of the interpolation function is equal to the freedom of element**(which is equal to the <mark style="background: transparent; color: yellow">number of the unknown parameters</mark>)

for example, for a triangle element with 6 nodes, the expression is a $2^{th}$ order polynomial, which should be written as : 
$$\Phi_i  = a_{i1} +a_{i2} x +  a_{i3} y + a_{i4}x^2 + a_{i5}xy + a_{i6} y^2 \qquad (i = 1,2,... 6)$$
where $i$ is the node number. 

note that the parameter should be chosen considering the **Symmetry method**. which means if there's 8 nodes on a element, the interpolation function term should be : 
$$1, x , y, x^2 , xy , y^2 , x^2 y, xy^2 \quad  \text{or} \quad 1, x , y, x^2 , xy , y^2 , x^3 ,y^3$$

### (3) The 1-D interpolation function of Element 
#### 1. Local Coordinates 
in the condition that there's 2 nodes in a element, i.e. : 
![[Excalidraw/Chapter2 Formulation of 2 and 3D Boundary Value Problems 2023-03-28 16.36.59|200]]
we set there's $m$ nodes<mark style="background: transparent; color: yellow">distributed uniformly on the element</mark>, so we can calculate the coordinate of elements by :
$$x_i = x_1 + \frac{i - 1}{m -1}(x_m - x_i)\quad (i =1,2 ...m)$$
we define the ***local coordinate*** as follows : 
firstly we set the coordinate of the <mark style="background: transparent; color: yellow">first node as 0 and the last node as 1</mark>, then: 
$$x = x_1 + \frac{x_i - x_1}{x_m - x_1} \xi$$
hence 
$$\xi_i = \frac{x_i - x_1}{x_m - x_1}$$
with the condition that the node ==distributed uniformly==, we can write the local coordinate of each node as : 
$$\xi_i = \frac{i-1}{m-1} \qquad (i = 1,2, ... m)$$
`````ad-note
title: another definition method for local coordinate 
collapse: close

if the local coordinate of the first node is -1 and the last is 1, then
$$\xi = \frac{2x - (x_m + x_1)}{x_m - x_1}$$
$$x = \frac{1}{2}[(x_m + x_1) + (x_m - x_1)\xi]$$

and the local coordinates of each node are :
$$\xi_i = -1 + 2\frac{i-1}{m-1}\qquad (i = 1,2 ...m)$$
`````

#### 2. Lagrange interpolation function 
since the Lagrange interpolation function $\Phi_i(\xi_i) = \delta_{ij}$, and the order of the interpolation polynomial is $m-1 = 1$
For 2 node line segment :
$$\Large\boxed{\Phi_i(\xi) = a_i + b_i \xi}$$
since the condition satisfies $\Phi_i{\xi_j} = \delta_{ij}$, we have : 
$$\Phi_1 = 1 - \xi\quad  \Phi_2 = \xi $$
in the line segment of 3 nodes [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/⛲Computational Fluid Dynamics/Basics of Finite Element Methods/Part 1/Implements/Derivation of high-ordered interpolation basis function.pdf|Derivation of high-ordered interpolation basis function.pdf]]

we have 
$$\begin{cases}
\Phi_1 = (1- \xi) (1- 2\xi) \\
\Phi_2 = 4\xi( 1 - \xi) \\
\Phi_3 = \xi(2\xi -1)
\end{cases}$$

![[Excalidraw/Chapter2 Formulation of 2 and 3D Boundary Value Problems 2023-03-28 16.41.29|300]]

```ad-seealso
collapse:close
if we set $\xi(x_1) = -1$ and $\xi(x_3) = 1$, then :
$$\begin{cases}
\Phi_1 = \frac{1}{2} \xi(\xi -1) \\
\Phi_2 = (1- \xi) (1 + \xi) \\
\Phi_3 = \frac{1}{2} \xi(1 + \xi)
\end{cases}$$
```

### (4) Different Coordinates of the problems  
##### (1) The coordinates of the length in 1-D problem
for the 1-D problem, we set two coordinates $\xi_1$ and $\xi_2$ : 
$$\xi_1 = \frac{L_1}{L} \quad  \xi_2 = \frac{L_2}{L}$$
note that  $\xi_1$ and $\xi_2$ are not independent, and it's apparently that $\xi_1 + \xi_2 =1$

##### (2) Coordinates of Area in 2-D problem
$$\xi_i = \frac{A_i}{A}$$
**Local Coordinates transform the triangle to a standard right-angled triangle**: 
![[Excalidraw/Chapter2 Formulation of 2 and 3D Boundary Value Problems 2023-03-28 17.04.41|450]]
<mark style="background: transparent; color: yellow">Note that later, the interpolation function becomes</mark>
$$\Phi_1 = \xi_1 , \Phi_2 = \xi_2, \Phi_3 = \xi_3$$
also $\xi_1 + \xi_2 + \xi_3 = 1$
the transformation from the relationship of Area coordinate (also, local coordinate) and the Cartesian coordinate can be derived by [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/⛲Computational Fluid Dynamics/Basics of Finite Element Methods/Part 1/Implements/Derivation of transformation between the local Coordinate & Cartesian Coordinate.pdf|Derivation of transformation between the local Coordinate & Cartesian Coordinate.pdf]] as :
$$\xi_i = a_i + b_i x + c_i y, \qquad i = 1,2,3$$
where $a_i,b_i, c_i$ can be calculated by using 
$$[\Lambda] = \left| \begin{matrix}
1 & x_1 & y_1 \\
1 & x_2 & y_2 \\
1 & x_3 & y_3
\end{matrix} \right|$$
and just using the algebraic cofactor of each element multiplied by a coefficient as :
$$a_1 = \frac{1}{2\Delta}\left| \begin{matrix}
x_2 & y_2 \\
x_3 & y_3
\end{matrix} \right| \qquad ...$$
where $\Delta  = \frac{1}{2}|\Lambda| = S_{triangle}$
then we can also solve the position of the point by local coordinates :
$$x_i = x_1 \xi_1 + x_2\xi_2 + x_3 \xi_3 \quad  y_i = y_1\xi_1+y_2\xi_2 + y_3\xi_3$$

`````ad-note
title: a Faster method for constructing the interpolation function 
collapse: close

we just need to find the boudary that contain all other points and then use the function value on the edges : 

the method is explained as follows: 

![[Excalidraw/Chapter2 Formulation of 2 and 3D Boundary Value Problems 2023-03-28 17.25.15|650]]

`````

### (5) Interpolation basis function for Rectangular Element
we use two coordinates $\xi$ and $\eta$ 
$$\begin{matrix}
\xi = \frac{1}{a} (x - x_c) \\
\eta = \frac{1}{b} (y - y_c)
\end{matrix}$$
we construct the expression of the interpolation basis function using :
$$\Phi(\xi, \eta) = L_i(\xi), N_i(\eta) \quad \text{which satisfy } \quad \Phi_i(\xi, \eta) = L_i (\xi) N_i(\eta)$$
where $L_i(\xi)$ and $N_i(\xi)$ is the 1-D interpolation function in the $\xi$ and $\eta$ direction.
$$\begin{cases}
L_1 = L_4 = \frac{1}{2}(1-\xi)\\
N_1 = N_2 =\frac{1}{2}( 1-\eta) \\
\end{cases}$$
the ***Interpolation function*** of each Node are shown in the following picture.
![[Excalidraw/Chapter2 Formulation of 2 and 3D Boundary Value Problems 2023-03-28 17.31.43|350]]

then, for the 6 node element in the following picture, we have : 
![[Excalidraw/Chapter2 Formulation of 2 and 3D Boundary Value Problems 2023-03-31 01.07.10|300]]

## 3. The 
$$u_i =u_{i\alpha} \phi_\alpha$$
commonly we set $\phi_i$ one more order than the velocity function. 
$$\begin{cases}
\int_{\Omega^e} \left( \rho v_i u_j \frac{\partial u_i}{\partial x_j} + \mu \frac{\partial v_i}{\partial x_j}\frac{\partial u_i}{\partial x_j} \right)d\Omega= p \frac{\partial v_i}{\partial x_i} \\
\int_{\Omega^e} q \frac{\partial u_j}{\partial x_j} d\Omega = 0
\end{cases}$$
we substitute that $u_i = u_{i\alpha}\phi_\alpha$

$$\Large \boxed{A_{}  + B_{k\alpha}^e u_{i\alpha}}$$

where : 
$$B = $$
$$C_{\beta ki}^e p_\beta$$
$$\begin{cases}
A_{k\alpha}^e u_{i\alpha} u_{jl} + B_{k\alpha} u_{i\alpha}+ C_{\beta k i}^e p_{\beta} - D_{ki}^e = 0 \\ E_{k j \alpha }^e u_{j\alpha} = 0
\end{cases}$$
where $i = 1,2$
$\alpha, k ,l = 1,2.....I_v$ is the function of the velocity. 


assembly:
$$\begin{cases}
A_{k\alpha} u_{i\alpha} u_{jl} + B_{k\alpha} u_{i\alpha}+ C_{\beta k i}^e p_{\beta} - D_{ki}^e = 0 \\ E_{k j \alpha }^e u_{j\alpha} = 0
\end{cases}$$
$A$ -> convective matrix 
$B$ -> diffusion matrix
$C$ -> pressure matrix  
$D$ -> matrix of external force 
$E$ -> continuum matrix 


$$\begin{cases}
A_{mnjs} u_{in} u_{js} + B_{mn} u_{in}+ C_{mir}^e p_{r} - D_{mi} = 0 \\ E_{rjn} u_{j n} = 0
\end{cases}$$
