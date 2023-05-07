## 1. Strong or Classical form of the 1-D boundary problem 

<mark style="background: transparent; color: yellow">Main constituents of a finite element method</mark> for the solution of a boundary-value problems are
1. The variational or weak statement of the problem 
2. <mark style="background: transparent; color: yellow">approximate solution of the variational equations</mark> through the use of "Finite Element Method". 

Firstly we suppose we want to solute the following differential equations as : 
$$u_{,xx} + l = 0\tag{1.1.1}$$
This equation is the governing equation of the ==transverse displacement== of a string in tension. And this equation also can be considered as governing equation for  ==longitudinal displacement== of an elastic rod. 

<mark style="background: transparent; color: yellow">Note that the comma stands for differential here</mark> ($u_{xx} = \frac{d^2 u}{d x^2}$)， and $l$ is a given smooth, scalar-valued function defined on the unit interval. 
$$l : [0,1] \rightarrow  R$$
where $[0,1]$ is unit interval. For each $x\in [0,1], l(x)\in R$. Also, we have that $[0,1]$ is said to be the ***domain*** of $l$ and $R$ is the ***range***. 
we also use
$$\begin{matrix}
\Omega = ]0,1[ \qquad \text{(open)} \\
\bar{\Omega} = [0,1] \qquad \text{(close)}
\end{matrix}$$
The **boundary conditions** on the function u are: 
$$\begin{matrix}
u(1) = q  \\
-u_{,x}(0) = h 
\end{matrix}$$
In this condition, this type of boundary conditions lead to the so-called ***Two Point Boundary Value Problems*** 

Hence the <mark style="background: transparent; color: yellow">strong form</mark> of the boundary value can be written as : 
$$(S)\quad \begin{cases}
\text{Given $l$ : $\bar{\Omega} \rightarrow R$ and constants $q$ and $h$, and then find $u : \bar{\Omega}\rightarrow  R$, such that :  }\\
u_{, xx}(x) + l(x) = 0 \qquad \text{on } \Omega \\
u(1) = q \\
-u_{,x}(0) = h
\end{cases}$$
When we write the $u_{,xx}+ l =0$ on $\Omega$, we mean $u_{,xx}(x) + l(x) = 0$ ==for all== $x\in \Omega$(whole zone are satisfy this differential equation), then we can easily obtain the exact solution of the $u$ as : 
$$u = q + (1- x)h + \int_{x}^{1} \left\{\int_{0}^{y}l(z) dz\right\}dy\tag{1.2.3}$$
where the $y$ and $z$ are used to <mark style="background: transparent; color: yellow">denote dummy variables</mark>.

However , We are interested in developing schemes for obtaining approximate solutions to $(S)$ which can be applicable to much more complex situations in which exact solutions are not possible.

## 2. Weak or Variational Form of the Problem 
To define the weak, variational, counterpart of $(S)$, we need to characterize **two classes of functions**. 
1. The trail solutions (candidate solutions): 
In the solution of this problem, we require the solution to <u>satisfy the boundary condition</u> $u(1) = q$ , while <u>the other boundary condition will not be required in the definition</u>. Furthermore, for the solution make sense, we should require that the <u>derivates of the trail solutions be squared-integrable</u>, which is, for a trial solution $u$, we have :
$$\boxed{\int_{0}^{1} (u_{,x})^2 dx < \infty \tag{1.3.1}}$$
`````ad-note
title: H1 function
collapse: open

The functions that satisfy the equation $(1.3.1)$ is called $H^1-\text{functions}$, also we denote that $u$ is a solution of $H^1$ as $u \in H^1([0,1])$ or $u \in H^1$. 
`````

Thus <mark style="background: transparent; color: yellow">the collection of trail solutions</mark>, denoted by $\delta$, ==consists of all functions which have square-integrable derivates and  take on the value== at $x =1$, can be written as :
$$\delta = \left\{u | u \in H^1, u(1) = q \right\}$$

2. weighting functions 
The ***Weighting functions*** is also called ***Variations*** 
This collection is similar to the trail solutions except <mark style="background: transparent; color: yellow">we require the homogenous counterpart (using the same order) of the q-boundary condition</mark>. which denotes that the boundary condition satisfy $w(1) =0$, we require the **weighting functions**. to satisfy $w(1) = 0$. This collection is denoted by $\mathcal{V}$ as : 
$$\mathcal{V} = \left\{ w|w\in H^1, w(1) = 0 \right\}$$
also we simplify the question by assuming that $l: \Omega \rightarrow R$ as being smooth. 
Given $l,q$ and $h$, as before. Find $u\in \delta$, such that for all $w \in\mathcal{V}$, then we have the suitable weak form of the boundary-value problem. 

we derive the weak form by integral the $w_{,x} u_{,x}$ , <mark style="background: transparent; color: yellow">use partial integral and substitute the boundary condition</mark> $w(1) = 0$ into it. 
$$\int_{0}^{1} w_{,x} u_{,x} dx = \int_{0}^{1}u_{,x} dw =\left. u_{x} w\right|_{0}^1 -  \int_{0}^{1} u_{,xx} w(x) dx = \int_{0}^{1} wl dx + w(0) h$$
$$(W)\quad \boxed{\begin{cases}
\text{Given $l$,q,and $h$, Find $u \in \delta\space $ s.t. for all $w\in \mathcal{V}$  } \\
\int_{0}^{1} w_{,x} u_{,x} dx = \int_{0}^{1}wl dx + w(0)h
\end{cases}}$$
Formulations of this type are often called ***Virtual Work***. or ***Virtual Displacement***
and in the function $w$ is ***Virtual Displacements***

<mark style="background: transparent; color: yellow">Hence the Variational equation is also called The equation of virtual work</mark>, It's also the ***weak*** or ***generalized solution***. 

Now we assume $u$ to be a weak solution, Thus $u \in \delta$; consequently $u(1) =  q$ and 
$$\int_{0}^{1} w_{,x} u_{,x} dx = \int_{0}^{1} wl dx + w(0)h$$
for all $w\in \mathcal{V}$, we integrate it by parts and also make use of $w(1) = 0$, then
$$0 = \int_{0}^{1} w(u_{,xx} + l ) dx + w(0)\left[ u_{,x}(0) +  h\right]$$
The proof of the proposition of this solution can be found in [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/⛲Computational Fluid Dynamics/Basics of Finite Element Methods/Part 1/Implements/Proof of the proposition of 1-D FEM problem.pdf|Proof of the proposition of 1-D FEM problem.pdf]]

`````ad-note
title: 2 types of boundary conditions
collapse: close

In the problem above, the boudnary condition is not mentioned explicitly, this type boundary conditions are referred as ***Natural Boundary Conditions***. The trail solutions are explicitly required to satisfy $u(1) = q$, This type of boundary conditions are called ***essential boundary conditions***. 

$\space$

The prove of the propsition goes under the name of ***fundamental lemma*** in the literature of the calculus of variations.  

$\space$

the methodology enables us to deduce the differential equations and boundary conditions implied by the weak problems. 
`````

for obtaining an <mark style="background: transparent; color: yellow">approximate solution to the original boundary-value problem</mark>, we have alternative starting points(i.e. The strong statement and the weak statement of the problem). FEM is based on the weak statement of the solution. 

Then we Let
$$\boxed{\begin{matrix}
a(w,u) = \int_{0}^{1} w_{,x} u_{,x} dx \\
(w,l) = \int_{0}^{1} wl dx 
\end{matrix}}$$
so we have the equation : 
$$\LARGE\boxed{a(w,u) = (w,l) + w(0)h}$$
`````ad-note
title: Symmetric & Bininear Forms
collapse: open

In the equation above, we note that $a(\cdot ,\cdot)$ and $(\cdot ,\cdot )$ are examples of symmertic and bilinear forms. 

The property ***Symmetry*** is : 
$$\begin{matrix}
a(u,v) = a(v,u) \\
(u,v) = (v,u)
\end{matrix}$$

The property ***Bilinearity*** is: 
$$\begin{matrix}
a(c_1 u  +c_2 v , w) = c_1 a(u,w) + c_2 a(v,w) \\
(c_1 u + c_2 v, w) = c_1(u,w) + c_2 (v,w)
\end{matrix}$$

`````

## 3. The Galerkin's Approximation method
#### 1) Preferences 
we now describe a method of obtaining an approximate solutions to boundary-value problems based on the weak formulations. The first step in developing the method is <mark style="background: transparent; color: yellow">to construct a finite-dimensional approximation</mark> of $\delta$ and $\mathcal{V}$. which we can describe the collections as $\delta^h$ and $\mathcal{V}^h$ with a ***mesh***, or discretization, of the domain $\Omega$, which is parameterized by a characteristic length scale $h$. We wish to think of $\delta^h$ and $\mathcal{V}^h$ <mark style="background: transparent; color: yellow">as being subset of</mark> $\delta$ and $\mathcal{V}$. 
$$\begin{matrix}
\delta^h \subset \delta \\
\mathcal{V}^h \subset \mathcal{V}
\end{matrix}$$
where $w^h$ is the *virtual displacement*, i.e.
$$\begin{matrix}
\text{if} \space u^h \in \delta^h , \text{then} \space u^h\in \delta \\
\text{if} \space w^h \in  \mathcal{V}^h, \text{then} \space w^h \in \mathcal{V}
\end{matrix}$$
The collections, $\delta, \mathcal{V},\delta^h, \mathcal{V}^h$ are often referred to as ***function spaces***

#### 2) Galerkin method
since the collection $\mathcal{V}^h$ is given, to each member $v^h\in \mathcal{V}^h$, we will construct a function $u^h \in \delta^h$ by 
$$u^h = v^h + q^h\tag{1.5.5}$$
where $q^h$ is a given function <mark style="background: transparent; color: yellow">satisfying the essential boundary condition</mark>. i.e.
$$u^h(1) = v^h(1) + q^h(1)$$
so we can now write a variational equation in terms of $w^h \in \mathcal{V}^h$ and $u^h \in \delta^h$, which is the ***discrete weak Form*** of the equation :
$$(W_h): \quad  \boxed{a(w^h, u^h) = (w^h , l) + w^h(0)h}\tag{1.5.8}$$
substitute $(1.5.5)$ into $(1.5.8)$, and we reach the ***Galerkin Weak Form*** $G$ : 
$$\Large (G): \quad \begin{cases}
\text{Given $l,q,h$, find $u^h = v^h + q^h$, where $v^h\in \mathcal{V}^h$} \\
\text{s.t. for all $w^h \in \mathcal{V}^h$}\\
a(w^h, v^h) = (w^h, l) + w^h(0) h - a(w^h, q^h) 
\end{cases}\tag{1.5.9}$$
This equation is sometimes referred to as ***Galerkin equation***

Approximation methods of the type considered are examples of so-called ***weighted residual methods***. 

## 4. Matrix Equations, Stiffness Matrix $K$
#### 1) Linear Combinations Definitions 
we let the $\mathcal{V}^h$ <mark style="background: transparent; color: yellow">consists of all linear combinations of given functions denoted by</mark> $N_A: \bar{\Omega} \rightarrow R$, where $A = 1,2,....n$, as : 
$$w^h = \sum^{n}_{A=1} c_A N_A$$
where $N_A$'s are referred to as ***Shape, Basis or interpolation function***. Since $w^h(1) = 0$, we have $N_A(1) = 0$, hence obviously that the $\mathcal{V}^h$ have dimensions $n$. 

For defining $\delta^h$, we need to specify $q^h$  and also we introduce  another ***Shape Function*** $N_{n+1}$as : 
$$N_{n+1}: \bar{\Omega} \rightarrow R , \qquad  N_{n+1} (1) = 1$$
thus the $q^h$ is given by 
$$q^h = qN_{n+1}\qquad  q^h(1) = q$$
Using this definitions, we can write $u^h$ in the linear combination form : 
$$\boxed{u^h = \sum^{n}_{A=1} N_A d_A + q N_{n+1}}\tag{1.6.6}$$
where the $d_A$'s are constants. 

`````ad-caution 
collapse: close
since $u^h \in \delta^h$ , we assume that $u^h = \sum^{n}_{A=1} d_A N_A$, and also, $w^h$ is the weighting functions, $w^h \in \mathcal{V}^h$, which is also the linear combination of the functions $N_A$

. 

also we assume that $v_h \in \mathcal{V}^h$, then we also have the combination form as : 
$$u^h = v^h + q^h$$
where 
$$w^h = \sum^{n}_{A=1} c_A N_A$$
$$v^h = \sum^{n}_{A=1} d_A N_A$$
$$u^h = \sum^{n}_{A=1} d_A N_A + q^hN_{n+1}$$
we wrote $d_B N_B$ in the derivation for the distinction of different functions 
`````

using the derivation process [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/⛲Computational Fluid Dynamics/Basics of Finite Element Methods/Part 1/Implements/Derivations of the Stiffness Matrix in FEM.pdf|Derivations of the Stiffness Matrix in FEM.pdf]]

we may state the matrix equivalent of the Galerkin problem as : 
$$\Large (M) \begin{cases}
\text{Given the coefficient matrix $K$ and the vector $F$, find $d$ s.t.} \\
Kd = F
\end{cases}$$
if we assume the inverse of $K$ exists and we have the solution :
$$d = K^{-1} F$$
where 
$$K = [K_{AB}] = a(N_A,N_B)$$
$$F_A = (N_A,l) + N_A(0) h - a(N_A,N_{n+1})q$$
then the solution of the Galerkin Equation can be obtained at any point $x\in \Omega$ by equation $(1.6.6)$ as above.

`````ad-note
collapse: open
1. The matrix $K$ is Symmetric. This followsfromthe symmeric of $a(\cdot ,\cdot )$, that is : 
$$K_{AB} = a(N_A, N_B) = a(N_B,N_A) = K_{BA}$$

2. the finite element method for the given problem can be found

$$(S) \leftrightarrow (W) \approx (G) \leftrightarrow (M)$$

3. Sometimes It's also convenient to write 
$$u^h(x) = \sum^{n+1}_{A=1} N_A(x) d_A$$
where $d_{n+1} = q$
`````

#### 2) Example : problem of 1 and 2 degree of freedom
##### 1) 1-D problem
for $n = 1$ <mark style="background: transparent; color: yellow">in the question above</mark>, the $u^h$ only consist of 1 unknown function, then $u^h = v^h + q^h = d_1N_1 + qN_2$, Then the only unknown is $d_1$. The shape functions must satisfy 
$$K = [K_{11}] = K_{11}\quad F = \left\{ F_1\right\}\quad  d= \left\{ d_1\right\} = d_1$$
also we let $N_1(x) = 1-x$ and $N_2(x) = x$, then : 
$$K_{11} = a(N_1,N_1) = \int_{0}^{1} N_{1,x},N_{1,x} dx = 1$$
so 
$$\begin{matrix}
F_1=(N_1, l ) + N_1(0) h- a(N_1, N_2) q \\
\qquad \qquad \space \space = \int_{0}^{1}(1-x) l(x) dx + h - \int_{0}^{1} N_{1,x}N_{2,x}dx q \\
= \int_{0}^{1} (1-x) l(x) dx + h + q\quad \space 
\end{matrix}$$
then $d^1 = K_{11}^{-1}F_1 = F_1$, so we get the solution of 1-D of freedom problem: 
$$u^{h}(x) = \left[\int_{0}^{1}(1-y)l(y)dy + h + q\right](1-x) + qx$$
at that time, we assume the solution under the different $l$ : 
at $l(y) = 0$, then 
$$u^h(x) = \left[\int_{0}^{1}(1-y)l(y) dy + h +q\right](1-x) + qx $$
we write it into the form:
$$\boxed{u(x) = q(1-x)h + \int_{x}^{1}\left\{ \int_{0}^{y} l(z) dz\right\}dy}$$
![[Chapter 1 One-Dimensional Boundary Value Problem 2023-03-16 17.01.31|200]]
for $l = 0$, 
$$\begin{matrix}
u(x) = h(1-x) +q  \quad \text{exact solution} \\
u^{(h)}(x) = h(1-x) + q \quad \text{numerical soltuion}
\end{matrix}$$
also, for $l = \rho = constant$ 
$$u(x) = u^{h}(x) = \frac{\rho(1-x)^2}{2} + h(1-x) + q$$
where the particular solution is: 
$$\frac{\rho (1- x)^2}{2}$$
note that on the node $u(0)$ and $u(1)$, the solution are exactly the same : 
$$u(0) = u^h(0) = \frac{\rho}{2} + h + q$$
also it can be derived when $l(x) = px$,the exact solution is the same as the numerical solution (derivation is put in the next part). 

##### 2) 2-D problem 
for case $n=2$, then $w^h = c_1 N_1 + c_2N_2$, where $N_1(1) = N_2(1) =0$, and $u^h = d_1 N_1 + d_2N_2 + qN_3$, where $N_3 = 1$. 
we can define $N_1, N_2, \text{ and } N_3$ as : 
$$N_1(x) = \begin{cases}
1-2x\qquad 0\leq x\leq \frac{1}{2} \\
0 \qquad \frac{1}{2}\leq x\leq  1
\end{cases}
\quad N_2(x) = \begin{cases}
2x\qquad 0\leq x\leq \frac{1}{2} \\ 
2(1-x) \quad \frac{1}{2}\leq x\leq  1
\end{cases}, \quad N_3(x) = \begin{cases}
0 \quad ...\\ 2x -1 \quad ....
\end{cases}$$

![[Chapter 1 One-Dimensional Boundary Value Problem 2023-03-14 21.46.50]]
using $K_{AB} = \int_{0}^{1/2}N_A, N_B dx + \int_{1/2}^{1} N_{A,x}, N_{B,x} dx$, then finally : 
$K_{11} = \int_{0}^{1/2} N_1 N_1dx = 4 \times 1/2 = 2$
$$K = 2\left[\begin{matrix}
1 & -1 \\ 
-1 & 2
\end{matrix}\right]$$
with the derivation process in the [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/⛲Computational Fluid Dynamics/Basics of Finite Element Methods/Part 1/Implements/Compare of 2D  exact solution and numerical solution.pdf|Compare of 2D  exact solution and numerical solution.pdf]] we know that 

in the case of $l(x) = 0,l(x) = p,l(x) =px$, the solutions on the nodes can all derived to be the same. 

Also note that the $W^h$ is a continuous function and the derivation of it is Piecewise continuous. (not continuous at the boundary of each element).

## 5. Piecewise Linear Finite Element Space 
To define <mark style="background: transparent; color: yellow">the general case</mark> in which $\mathcal{V}^h$ is $n$-dimensional, then we partition the domain into $n$ nonoverlapping subintervals. Which the typical subinterval us denoted by $[x_A, x_{A+1}]$, where $x_A < x_{A+1}$. $A = 1,2,... n$. So we require $x_1$ to be zero and $x_{n-1} = 1$, then the $x_A,x_{A+1},...$ are called ***nodal points***.
![[Chapter 1 One-Dimensional Boundary Value Problem 2023-03-16 22.23.04|350]]
then <mark style="background: transparent; color: yellow">the shape functions</mark> are defined as :
$$N_A(x) = \begin{cases}
\frac{(x - x_{A-1})}{h_{A-1}} \quad x_{A-1} \leq x \leq x_A \\
\frac{(x_{A+1} - x)}{h_A} \quad  x_{A} \leq x_{A-1} \\
0 \qquad \text{elsewhere}
\end{cases}$$

where $\delta_{AB}$ is the Kronecker delta. 

A ==typical member $w^h \in \mathcal{V}^h$ has the form $\sum^{n}_{i=1} c_A N_A$ and appears as in Fig below.==
![[Chapter 1 One-Dimensional Boundary Value Problem 2023-03-16 22.33.07]]
also note the typical members of the $\delta^h$ are obtained by adding $q^h = qN_{n+1}$ to the typical members of $\mathcal{V}^h$, which ensures that $u^h(1) = q$

In this case the <mark style="background: transparent; color: yellow">continuity condition is identically satisfied</mark> and the summation of integrals over the element interiors may be replaced by <mark style="background: transparent; color: yellow">an integral over the entire domain</mark>. 

## 6. The Construction of Stiffness matrix $K$ 
The shape functions $N_A$, $A =1,2,....n +1$, are <mark style="background: transparent; color: yellow">zero outside a neighborhood of node</mark> $A$. Also as a result, many of the entries of $K$ are zero. 
$$\boxed{K_{AB} = \int_{0}^{1}N_{A,x}N_{B,x}dx = 0\qquad(B>A+1)}$$
note the Symmetry of the matrix $K$ implies that <mark style="background: transparent; color: yellow">the equation holds for</mark> $A>B+1$. 
The $K$ is ***banded***, that is :
$$K = \left[\begin{matrix}
K_{11} & K_{12} &  0 & ... \\
K_{21} & K_{22} & K_{23} & 0 & ....\\
0 & K_{32} & K_{33} & K_{34} & 0 & ...\\
... & ... & ... & ... & ... & ... \\
0 & ... & &0 & K_{n, n-1} & K_n
\end{matrix}\right]$$

**Theorem** : The $n\times  n$ matrix $K$ is <mark style="background: transparent; color: yellow">positive definite</mark>.
[[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/⛲Computational Fluid Dynamics/Basics of Finite Element Methods/Part 1/Implements/Proof of the positive definite of the Stiffness Matrix.pdf|Proof of the positive definite of the Stiffness Matrix.pdf]]

## 7. Mathematical Analysis 
We let the $\delta(x) = \delta (x-y)$ denote the ***Dirac Delta function***, which is not a function in the classical sense but rather an<mark style="background: transparent; color: yellow"> operator defined by its action on the continuous functions</mark>. We let $w$ be continuous on $[0,1]$, then we have : 
$$\boxed{(w, \delta_y)= \int_{0}^{1}w(x) \delta(x-y) dx = w(y)}$$
The **Green's function problem** may be state as : 
Find a function $g$ such that : 
$$\begin{matrix}
g_{,xx} + \delta_y = 0 \quad \text{on }\Omega \\
g(1) = 0;\\
g_{,x}(0) = 0;
\end{matrix}$$
(we can compare it with the previous problem [[#1. Strong or Classical form of the 1-D boundary problem|Equation of 1-D FEM problem]])

The problem may be solved by way of formal calculation with **distribution of generalized functions** such as $\delta_y$ we note that the formal integral of the $\delta_y$ is ***Heaviside function***, which is shown in the fig below : 

![[Chapter 1 One-Dimensional Boundary Value Problem 2023-03-17 09.18.21|550]]
Then to solve the Green function problem, we integrate the $g_{,xx} + \delta_y = 0$ twice, then 
$$\begin{matrix}
g_{,x} + H_y = c_1 \\
g(x) + <x-y> = c_1 x + c_2
\end{matrix}$$
substitute the boundary condition into this function, we can easily derive that : 
$$g(x) = (1 - y) - <x-y>$$
Observe that the $g$ is piecewise linear (as the fig above), then if $y = x_A \text{ i.e.}\space  y\space   \text{is a node}$ and $g\in \mathcal{V}^h$, we can substitute the $u$ <mark style="background: transparent; color: yellow">in the example problem</mark> [[#2) Galerkin method|(Galerkin Method)]] by the $g(x)$ and the $l$ by $\delta_y$ and $q \text{ and } h$ by 0. then we have : 
$$\Large\boxed{a(w,g) = (w, \delta_y) = w(y)\tag{1.10.10}}$$
this function holds for all the continuous $w\in \mathcal{V}$ 

`````ad-note
title: Theorem  
collapse: open

$$u^h(x_A) = u(x_A) , A = 1,2... n+1$$
i.e. $u^h$ is exact  at the nodes 

````ad-todo 
title: The proof of the Theorem 
collapse: open
````

`````

```ad-todo 
the section of the elimination theorem
```

## 8. The Element Point of View 
### 1) Local Coordinates and the Global Coordinates
with the different nodes and the different domains, shape functions , the globally defined function can be written as follows, and also, for each element we can view it by ***the element (local) point of view***. Then the set of the local qualities are also written below (<mark style="background: transparent; color: yellow">note in the local description, the nodal number begins with 1</mark>):
$$\text{global:}\begin{cases}
\text{Domain: } [x_A, x_{A+1}] \\
\text{Nodes: } {x_A, x_{A+1}} \\
...
\end{cases} \qquad \text{local:}\begin{cases}
\text{Domain: } [x_1, x_2] \\ 
\text{Nodes: } {x_1, x_2} \\
...
\end{cases}$$
in the local description, the $u^h(\xi)$ can be calculated as : 
$$u^h(\xi) = N_1 (\xi)d_1 + N_2(\xi)d_2$$
we relate the domains of the global and local descriptions by an "affine" transformation $\xi :[X_A,X_{A+1}] \rightarrow [\xi_1,\xi_2]$ , then ==it's practical to take $\xi_1 = -1$ and $\xi_2 =1$==, then : 
$$\xi(x) = c_1 + c_2 x, \quad \xi(x_A) = -1, \quad \xi(x_B) = 1$$
we can easily solve the system and then : 
$$\xi = \frac{x_A + x_B - 2x}{x_A - x_B} \quad \overset{B = A+1}{\rightarrow }\quad \xi = \frac{2x - x_A - x_{A+1}}{x_{A+1} - x_A}$$
or if we set $x_{A+1} - x_A = h$, then we have : 
$$\boxed{\xi(x) = \frac{2x - x_A - x_{A+1}}{h_A} \quad \text{and} \quad x(\xi)= \frac{h_A \xi + x_A +x_{A+1}}{2}} \tag{1.12.4}$$
`````ad-quote
title: stipulate
collapse: open
after that, we set the Uppercase characters $A,B,C....$ as the global function while using the Lowercase $a,b,c ...$ as the local function. 
`````

Then in terms of $\xi$, the shape functions in the local description take on a standard form, i.e. $(1.12.4)$ can be written as the following equations : 
$$\boxed{N_a(\xi) = \frac{1}{2}(1 + \xi_a \xi) \qquad a = 1, 2}$$
where $\xi_1 = -1, \xi_2 = 1$ is the local coordinates 
$$\boxed{x^e (\xi) = \sum^{2}_{a=1} N_a (\xi) x_a^e}$$
where the superscript $e$ denote a quantity in the local description associated with element number $e$. <mark style="background: transparent; color: yellow">The equation above defines the interpolation between the point</mark> $\xi_1$ and $\xi_2$

also note that $\xi_1 = -1\space  \text{and}\space \xi_2 = 1$, then $N_1 = \frac{1 - \xi}{2}, N_2 = \frac{1+ \xi}{2}$, for $\xi = -1, N_2 = 0$ and also $\xi = 1, N_1 = 0$ 
![[Chapter 1 One-Dimensional Boundary Value Problem 2023-03-17 11.16.05|450]]

`````ad-summary
title: The steps for the FEM problems 
collapse: close
1. firstly write the weak form of the equation 
2. discrete the solving field 
3. write the basic functions of element (select the interpolation functions with suitable boundary conditions)
4. analyze each element  
5. assembly each matrix of element into a whole matrix
6. deal with the boundary conditions (modify the natural boundary condition to the essential boundary condition)
7. solve the equation and finally calculate the parameters
`````

### 2) The Element Basic Equations
for the element of a 1-D problem, we assume our model consists of $n_{el}$ elements, which is numbered a shown in the Figure 
![[Chapter 1 One-Dimensional Boundary Value Problem 2023-03-17 12.37.49|400]]
then we recall the definition of the global stiffness matrix and the force vector : 
$$K = [K_{AB}]\quad (n\times n), \qquad F = \{F_A\} \quad (n\times 1)$$
where 
$$K_{AB} = a(N_A, N_B) =\int_{0}^{1} N_{A,x} N_{B,x} dx$$
$$F_A = (N_A, l) + \delta_{A1}h - a(N_A, N_{n+1})q = \int_{0}^{1} N_{A} l dx + \delta_{A1} h -\int_{0}^{1} N_{A, x} N_{n+1,x} dx \space q$$
note that in the equation above, we have assumed $N_A(x_1) = \delta_{A1}$, <mark style="background: transparent; color: yellow">as for the piecewise linear finite element space</mark>. The integrals over the element domains may be written as <mark style="background: transparent; color: red">sums of integrals over the element domains. </mark> as follows : 
$$K = \sum^{n_{el}}_{e=1} K^e\qquad K^e = [K_{AB}^e]$$
$$F = \sum^{n_{el}}_{e=1} F^e\qquad  F^e = \left\{F_A^e\right\}$$
where 
$$K_{AB}^e = a(N_A, N_B)^e = \int_{\Omega^e} N_{A,x} N_{B,x} dx\tag{1.13.6}$$
$$F_A^e = (N_A, l)^e + \delta_{e1}\delta_{A1}h - a(N_A, N_{n+1})^e q = \int_{\Omega^e} N_A ldx + \delta_{e1}\delta_{A1}h - \int_{\Omega^e} N_{A,x} N_{n+1,x} dx\space q$$
and $\Omega^e = [x^e_1, x^e_2]$ is the domain of $e^{th}$ element

Also, the <mark style="background: transparent; color: yellow">important observation to make</mark> is that $K$ and $F$ can be constructed by *summing the contributions of element matrices and vectors respectively*. In the literature, the process is called ***the direct stiffness method***, also note that the stiffness matrix and the force vector for a element can be calculated as :  
$$k^e = [k_{ab}^e]\quad(\text{for element}, 2\times 2)  \qquad f^e = \{f_a^e\}\quad (2\times 1)$$
$$k_{ab}^e = a(N_a, N_b)^e = \int_{\Omega^e}N_{a,x} N_{b,x} dx$$
$$f_a^e =\int_{\Omega^e} N_a l dx + \begin{cases}
\delta_{a1} h \qquad e = 1 \\
0 \qquad \quad\space  e = 2, 3...n_{el}-1 \\
-k_{a2}^e  q \quad\space  e= n_{el}
\end{cases}$$

## 9. Assembly of Global Stiffness Matrix

in a finite element computer program, it's a task of a **"finite element subroutine"** to produce $k^e$ and $f^e$ . where $e = 1,2... \text{to}\space  n_{el}$. from the given data and provide an "assembly subroutine" so that the terms in the $k^e$ and $f^e$ <mark style="background: transparent; color: yellow">can be added to the appropriate locations in</mark> $K$ and $F$, respectively. 

The assembly information is stored in the *location matrix*.  we use $LM$ as it.
the location matrix is showed as follows 

![[Chapter 1 One-Dimensional Boundary Value Problem 2023-03-21 18.48.37]]

| Local Element Number \ Element numbers | 1   | 2   | 3   | ... | e   | ... |
| -------------------------------------- | --- | --- | --- | --- | --- | --- |
| 1 (local)                              | 1   | 2   | 3   | ... | e   | ... |
| 2 (local)                              | 2   | 3   | 4   | ... | e+1 | ... |

note<mark style="background: transparent; color: yellow"> the element in the table is the global node number</mark> for the whole structure.

from the $LM$ array, we can deduce the following assembly procedure : 

The nonzero element of each line is indexed in the *location matrix*, for the example above, the first column of the table is 
| ElemNumber\Local ElemNumber | 1(local) | 2(local) |
| --------------------------- | -------- | -------- |
| 1 (element)                 | 1        | 2        |

then the transform from the global displacement vector to the local displacement vector becomes :
$$\left[\begin{matrix}
u_1^{(1)} \\ u_2^{(1)} 
\end{matrix}\right] = \left[\begin{matrix}
1 & 0 & 0 & 0 & 0 \\
0 & 1 & 0 & 0 & 0
\end{matrix}\right]\left[\begin{matrix}
u_1 \\ u_2 \\ u_3 \\ u_4 \\  u_5
\end{matrix}\right]$$
where in the $e$ in the $u^{(e)}$ means the <mark style="background: transparent; color: yellow">element number. </mark>

In the equation above, we let ***Boolean Matrix*** as 
$$\Delta_{in}^{(e)} = \begin{cases}
1 \qquad  \text{ the element i in the element } e \space \text{is the same node as  } n \text{in global coordinates}\\
0 \qquad  \text{otherwise}  
\end{cases}$$
For example : 
$$\Delta_{in}^{(2)} = \left[\begin{matrix}
0 & 1 & 0 & 0 & 0 \\
0 & 0 & 1 & 0 & 0
\end{matrix}\right]$$

also, <mark style="background: transparent; color: yellow">we set the</mark> **Basic function of the element** as $\Phi_i \qquad (i = 1, 2,.... I)$ 
in the element, the ***Approximation Function*** is $u^{(e)}$, which can be expressed as the linear combination of the basic functions : 
$$\Large\boxed{u^{(e)} = u_i^{(e)} \Phi_i}$$
`````ad-note
title: Properties of The Basic Functions  
collapse: open
(1) The number of the basic functions is <mark style="background: transparent; color: yellow">the same as the number of the nodes in the elements</mark>, the node with number $i$ has the basic function $\Phi_i \quad (i = 1, 2... I)$

(2) $\Phi_i$ satisfies the condition (interpolation conditions):
$$\Phi_i(p_j) = \delta_{ij}$$
and
$$\delta_{ij} = \begin{cases}
1 \qquad i = j \\
0 \qquad i\neq j
\end{cases}$$

```ad-hint
if we use Polynomial Function as the Basic function of the element, we have that : 

$$\Large\boxed{\Phi_i = \frac{x_j^{(e)} - x}{x_j^{(e)} - x_i^{(e)}}}$$

also, we can use the ***Dimensionless local coordinates*** to express the basic function as : 

$$\boxed{\begin{matrix}
\Phi_1 = 1 - \xi \\ \Phi_2 = \xi
\end{matrix}}$$

where
$$\xi = \frac{x - x_1^{(e)}}{\Delta x}$$
```

`````

We can derive that the <mark style="background: transparent; color: yellow">expression of the basic function</mark> of the element $n$ is : 
$$\Large\Phi_n(x) = \begin{cases}
\frac{x- x_{n-1}}{x_n - x_{n-1}} \quad x \in [x_{n-1}, x) \\
1  \qquad \quad \space \space \space x  = x_n\\
\frac{x_{n+1} - x}{x_n - x_{n-1}} \quad x \in [x_{n}, x_{n+1})\\
0 \qquad \qquad \quad \text{otherwise}
\end{cases}$$
also the approximate functions is Assembled by each element function

## 10. Element FEM Characteristic Equation
In the problem we formula to solve is : 
$$\begin{cases}
\frac{d^2 u}{dx^2} = c \quad  0 < x < h\\
u(0) = 0 \\
u(h) = 0
\end{cases}\tag{2-1}$$
and the weak form of the first function can be written as : 
$$\int_{0}^{h} \left( \frac{du}{dx}\frac{dw}{dx} - cw \right)dx = 0 \tag{2-3}$$
for the problem above, the integral of the weak form (i.e. the equation 2.3) can be set as the summation of integral of each element : 
$$\int_{0}^{h} \frac{du}{dx} \frac{d(\delta u )}{dx}  + c \delta u dx = \sum^{E}_{e=1} \int_{x_1^{(e)}}^{x_2^{(e)}}\frac{du}{dx} \frac{d(\delta u)}{dx} + c\delta u dx$$
Note that for single element, the ***Element FEM Equation*** is : 
$$\int_{x_1^{(e)}}^{x_2^{(e)}}\frac{du}{dx} \frac{d(\delta u)}{dx} + c\delta u dx = 0$$
In the equation above, we use the [[#9. Assembly of Global Stiffness Matrix|approximation expression of basic function]] $\Phi_i$ . 
Then we derive the function in the process written in the [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/⛲Computational Fluid Dynamics/Basics of Finite Element Methods/Part 1/Implements/Derivation of the Element FEM characteristic equation.pdf|Derivation of the Element FEM characteristic equation.pdf]]. Then we have : 

**1. The general form of the element FEM equation**
$$\Large \boxed{A_{ij} u_j^{(e)} = f_i^{(e)}}$$
where 
$$A_{ij} = \int_{x_1^{(e)}}^{x_2^{(e)}}\frac{d\Phi_i^{(e)}}{dx}\frac{d\Phi_j^{(e)}}{dx}$$
$$f_i = \int_{x_1^{(e)}}^{x_2^{(e)}}c\Phi_i dx$$
**2. Matrix Form of the element FEM equation**
$$A_{ij}^{(e)} = \frac{1}{\Delta h}\left[\begin{matrix}
1 & -1 \\ -1 & 1
\end{matrix}\right]$$
$$f_i^{(e)} = -\Delta h \cdot c \left[\begin{matrix}
\frac{1}{2} \\ \frac{1}{2}
\end{matrix}\right]$$

## 11. Assembly of the Total characteristic Equation 
For **single element** it has the characteristic equation in the local coordinates with form : 
$$\Large \boxed{A_{ij} u_j^{(e)} = f_i^{(e)}}$$
and the **Total FEM Equation** has the form of : 
$$\Large\boxed{ A_{nm} u_m = f_n}$$
where the $A_{nm}$ and $u_m$ is the summation of $A_{ij}^{(e)}$ and $f_i^{(e)}$ 
with the derivation process [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/⛲Computational Fluid Dynamics/Basics of Finite Element Methods/Part 1/Implements/Derivation of the Total FEM Equation.pdf|Derivation of the Total FEM Equation.pdf]] , we can get the expression of $A_{nm}$ and $u_m$ :
$$\boxed{A_{nm} = \sum^{E}_{e=1} \left[\Delta_{ni}^{(e)}A_{ij}^{(e)}\Delta_{jm}^{(e)}\right]}$$
$$\boxed{f_n = \sum^{E}_{e=1}\Delta_{ni}^{(e)} f_i^{(e)}}$$
## 12. Boundary Condition

`````ad-caution
title: The method for Refining the total FEM equations 
collapse: open


1. Eliminate the element of the row and the column
if  we replace the diagonal element $A_{rr}$ with 1 and then the matrix would becomes : 
$$A_{nm}^* = \left[\begin{matrix}
A_{11} & A_{12} & ... &  A_{1, r-1} &   0 &  A_{1, r + 1} & ....  \\
... \\\
A_{r-1,1} & A_{r-1.2} & ... & A_{r-1, r-1} & 0 & A_{r-1, r+1} & .... \\
0 & 0 & ... & 0 & 1 & 0 & .... \\
...
\end{matrix}\right]$$

then we can modify the $f_n$ vector as :
$$f_n^*  =\left[\begin{matrix}
f_1 - A_{1,r} \bar{u}_r \\
f_2 - A_{2,r} \bar{u}_r \\
... \\
f_{r-1} - A_{r-1,r} \bar{u}_r \\
u_r \\
f_{r+1} - A_{r+1, r}\bar{u}_r \\
f_{N} - A_{N,r}\bar{u}_r
\end{matrix}\right]$$
Note that this method force the boudnary condition on the essential boundary as $u_r = \bar{u}_r$, then drop the element in the line and the column of the $u_r$

2. Enlarge the diagonal elements 
we can modify the $A_{pp}$ as : $10^{20}A_{pp}$

$$\left[\begin{matrix}
A_{11} & ... &  A_{1, p}  &  ... &  A_{1,N} \\
... \\
A_{p,1} & ...  & 10^{20} A_{p, p} & ... & A_{N, p}  \\
... \\
...
\end{matrix}\right]$$

also in this method, the force vector can be modified as :
$$f_n^*  = \left[\begin{matrix}
f_1 \\
... \\
10^{20} f_p \\
... \\
f_N
\end{matrix}\right]$$

`````
