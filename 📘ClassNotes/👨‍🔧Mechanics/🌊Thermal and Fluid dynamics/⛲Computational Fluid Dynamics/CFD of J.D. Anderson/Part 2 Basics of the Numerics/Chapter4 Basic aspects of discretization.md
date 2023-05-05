## 1. Introduction for the Finite difference method 
The discretization techniques are divided into 3 types, i.e. [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/⛲Computational Fluid Dynamics/CFD of J.D. Anderson/Part 1 Basic Equations/Chapter1 Philosophy of Computational Fluid Dynamics#(2) Introduction FDM, FVM and FEM method|FDM, FEM, FVM]]

From the Chapter 1 we can get the [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/⛲Computational Fluid Dynamics/CFD of J.D. Anderson/Part 1 Basic Equations/Chapter1 Philosophy of Computational Fluid Dynamics#1. The Finite Difference Method|FDM equations and the derivation]]

the basic thought of the Finite Discretization Method is ==using the discrete numbers to replace the partial derivates in the governing equations==. 

In the calculation of the CFD process, the numerical calculations are **carried out in a transformed computational space** which corresponds to nonuniform spacing in the physical plane. 

the recent studies of the CFD focused on the unstructured grid. (in contrast to the structured ones which reflects the consistent geometrical regularity)

## 2. Basic Derivations for finite difference method 
using the Taylor series, we denote the $x$ component of the velocity by $u$, and then 
$$u_{i+1, j} = u_{i,j} +  \left. \left(\frac{\partial u}{\partial x}\right)\right|_{i,j} \Delta  x + \frac{1}{2!} \left. \frac{\partial^2 u}{\partial x^2}\right|_{i,j} + ......$$
![[Excalidraw/Chapter4 Basic aspects of discretization 2023-04-07 11.09.46|300]]

with  the derivation process in chapter 1 or use [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/⛲Computational Fluid Dynamics/CFD of J.D. Anderson/Part 2 Basics of the Numerics/Implements/Derivation for the Difference form of derivate.pdf|Derivation for the Difference form of derivate.pdf]], we have : 
***first order forward difference***: 
$$\boxed{\left( \frac{\partial u}{\partial x}\right)_{i,j} =\frac{u_{i+1,j} - u_j}{\Delta x} + O(\Delta x)}$$
***First order Rearward difference***:
$$\boxed{\left( \frac{\partial u}{\partial x}\right)_{i,j} = \frac{u_{i,j} - u_{i-1,j}}{\Delta  x} + O(\Delta x)}$$
where the $O(\Delta x)$ is the ***Truncation error***
But more often, we use a second-order finite-difference accuracy equation, that is a second-order ***Central Difference***
$$\boxed{\left( \frac{\partial u}{\partial x}\right)_{i,j} = \frac{u_{i+1,j} - u_{i-1,j}}{2\Delta x}+ O(\Delta x^2)}$$
Also the second-order difference is 
$$\boxed{\left( \frac{\partial^2 u}{\partial x^2}\right)_{i,j} = \frac{u_{i+1,j} - 2u_{i,j}+u_{i-1,j}}{(\Delta x^2)}+ O(\Delta x^2)}$$
where the equation of 2nd order derivate of y is same and the mixed derivate : 
$$\boxed{\left(\frac{\partial^2 u}{\partial x\partial y}\right)_{i,j} = \frac{u_{i+1, j+1} - u_{i-1, j+1}- u_{i+1,j-1} + u_{i-1,j-1}}{4\Delta x \Delta  y} + O[(\Delta x)^2,(\Delta y)^2]}$$
## 3. Difference Equations 
using the finite-difference quotient to replace the partial differential equation. and the essence of the finite-difference solution in CFD is to use the difference equations for the dependent variables at each grid point. 

We consider an equation of the one-dimensional heat conduction equation with constant thermal diffusivity, which is : 
$$\frac{\partial T}{\partial t} = \alpha\frac{\partial^2 T}{\partial x^2}$$
then the time difference of the equation by : 
$$\left(\frac{\partial T}{\partial t}\right)^n_i = \frac{T_i^{n+1} - T_i^n}{\Delta  t} - \left(\frac{\partial^2 T}{\partial t^2}\right)\frac{\Delta  t}{2} + .....$$
and also : 
$$\frac{\partial^2 T}{\partial x^2} = \frac{T_{i+1}^n + T_{i-1}^n - 2T_{i}^n}{\Delta  x}$$
Note that : <mark style="background: transparent; color: yellow">The i in the equation above is the location in the space (or the number of the node), and n is the time</mark>
The form of the equation above is the difference form which ==**use the forward step of time**== and **==the central step of the space==**

also note the Truncation error for the equation above is $O(\Delta t, \Delta x^2)$

If when he truncation error approaches 0, the difference equation does indeed approach the original differential equation, and the solution is more precise. In that case, we say that the equation is ***consistent***, and the solution is stable. 
## 4. The Explicit and Implicit Approach 
### (1) Explicit Approach 
for the equation : 
$$\frac{\partial T}{\partial t} = \alpha\frac{\partial^2 T}{\partial x^2}$$
we have: 
$$\frac{T_i^{n+1} - T_i^n}{\Delta t} = \frac{T_{i+1}^n + T_{i-1}^n - 2T_{i}^n}{(\Delta x)^2}$$
we can write it into the form : 
$$T_i^{n+1} =T_i^n  +  \alpha\frac{\Delta t }{(\Delta x)^2} (T_{i+1}^n + T_{i-1}^n - 2T_{i}^n)$$
The equation above allows the direct calculation of the $T_{i}^{n+1}$, thus we can call it as ***Explicit Approach***, in the explicit approach, the equation only contains one unknown at time level $n+1$

### (2) Implicit Approach 
#### 1. Head-Conduct equation 
For the Parabolic differential equation, it presupposes the stipulation of the boundary conditions. 
for example, we consider the figure below : 
![[Excalidraw/Chapter4 Basic aspects of discretization 2023-04-12 22.14.17|300]]
As a counterexample to the explicit approach., the spatial difference on the right-hand side *in terms of the average properties between n and n+1*, that is : 
$$\frac{T_i^{n+1} - T_i^n}{\Delta t} = \alpha \frac{\frac{1}{2}\left( T_{i+1}^{n+1} - T_{i+1}^n \right) + \frac{1}{2}\left(-2T_i^{n+1} - 2T_i^n\right) + \frac{1}{2}\left(T_{i-1}^{n+1} - T_{i-1}^{n}\right)}{(\Delta x^2)}$$
This special type of differential equations is called ***the Crank-Nicolson form***, which is commonly used to solve the problems governed by parabolic equations. 

An implicit approach ***is one where the unknowns $T_i^{n+1}$ can be solved simultaneously and the unknowns must be obtained by means of a simultaneous solution of the difference equations***. 

Using the [[Excalidraw/Chapter4 Basic aspects of discretization 2023-04-13 19.45.56|Derivation Process]], we get : 
$$AT_1 - BT_2 + AT_3  = K_2$$
where: 
$$A = \frac{\alpha \Delta t}{2(\Delta x)^2} \qquad B = 1 + \frac{\alpha \Delta  t}{(\Delta x)^2} \qquad K_i  = -T_i^n - \frac{\alpha \Delta t}{2(\Delta  x^2)}\left(T^{n}_{i-1} -2 T_{i}^{n} + T_{i+1}^{n}\right)$$
thus, in the picture below : 
![[Excalidraw/Chapter4 Basic aspects of discretization 2023-04-12 22.14.17|300]]
the equation at point 2 : 
$$AT_1- BT_2 + AT_3 = K_2$$
we can also denote the $K_2 - AT_3$ by $K_2'$, then 
$$AT_3 - BT_2 = K'_2$$
for each gird point, we can write the following equation : 
$$\begin{cases}
AT_2 + BT_3 + AT_4 = K_3 \\
AT_3 + BT_4 + AT_5 = K_4 \\
AT_4 + BT_5 + AT_6 = K_5 \\
...
\end{cases}$$
$$\left[\begin{matrix}
-B & A & 0 & 0 & 0 \\
A & -B & A & 0 & 0 \\
0 & A & -B & A & 0 \\
0 & 0 & A & -B & A \\
0 & 0 & 0 & A & -B \\
\end{matrix}\right]\left[\begin{matrix}
T_2 \\ T_3 \\ T_4 \\ T_5 \\ T_6
\end{matrix}\right] = \left[\begin{matrix}
K'_2  \\ K_3 \\ K_4 \\ K_5 \\K_6'
\end{matrix}\right]$$
On the matrix above, the coefficient matrix is a *tridiagonal matrix*

On the basis of the above example, <mark style="background: transparent; color: yellow">an implicit approach is more involved than an explicit approach</mark>

#### 2. The Thermal diffusivity equation 
The thermal diffusivity equation is presented as : 
$$\frac{\partial T}{\partial t} = \alpha(T) \frac{\partial^2 T}{\partial x^2}$$
![[Excalidraw/Chapter4 Basic aspects of discretization 2023-04-13 21.05.05|400]]

`````ad-caution 
The Most important difference between the explicit and implicit approach is the stability problem. 

Note that the increment of $\Delta x$ and $\Delta t$ appear in all the above difference equations. And for the explicit approach, once $\Delta x$ is chosen, then $\Delta t$ is not a independent or arbitary choice $\rightarrow$ and also, $\Delta t$ is restricted to be equal to (or less than) a certain value prescribed by a ***Stability criterion*** 

If $\Delta t$ is taken larger than the limit imposed, <mark style="background: transparent; color: violet">the time-marching procedure will quickly becomes unstable</mark>

However, for the implicit methods, some implicit methods are ***unconditionally stable***.

Also note that an implicit method using large values of $\Delta t$ <mark style="background: transparent; color: yellow">may not accurately define the timewise variation of the flow field</mark>. In this situation, the advantage og an implicit approach may be totally negated  
`````

## 5. Stability and Error Analysis
### (1) Stability Analysis 
The three general method for discretization is : 
$$\begin{cases}
\xi_i^{n+1} = \xi_{i}^{n} - \alpha \frac{\Delta t}{2\Delta  x}\left( \xi_{i+1}^n - \xi_{i-1}^n\right)\quad  \text{FTCS}\\
\xi_{n+1} = x_i^n - \alpha \frac{\Delta t}{\Delta x}(\xi_{i+1}^n - \xi_{i}^n)\qquad  \text{FTFS}\\
\xi_{n+1} = x_i^n - \alpha \frac{\Delta t}{\Delta x}(\xi_i^n - \xi_{i-1}^n) \qquad \text{FTBS}
\end{cases}$$

The stability of the equation is also called ***the Convergence of Equation***
the Convergence of difference equation is defined as : **When the step approaches to zero, the solution of the difference can approach the solution of the differential equation.**

there are generally two types of Convergence i.e. Absolute Convergence and Conditional Convergence (which means <mark style="background: transparent; color: yellow">the equation only converge when some conditions are satisfied</mark>). 

We say a solution method is <mark style="background: transparent; color: lime">unstable if the increment in the marching direction exceeded some Prescribed value.</mark>

we still use the 1-D heat conduction equation as example 
$$\frac{\partial T}{\partial t} = \alpha\frac{\partial^2 T}{\partial t^2}$$
<mark style="background: transparent; color: orange">The numerical solution of the equation is influenced by two types of errors, which is : </mark>
1. ***Discretization Error*** : the difference between the exact solution and analytical solution of the partial differential equation. (<mark style="background: transparent; color: orange">The error is the truncation error for the difference equation plus the errors introduced by the numerical treatment of the boundary conditions</mark>)
2. ***Round-Off Error*** : numerical error introduced because of the accuracy of the double/float numerical type and the computer is rounding after a repetitive number of calculations. 
$$\begin{cases}
A  =  \text{analytical solution of the differential equation} \\
D  = \text{the exact solution of the} \textbf{ Difference } \text{equation} \\
N  = \text{numerical solution of the real computer}
\end{cases}$$
$$\begin{cases}
A-D = \text{Discretization error} \\ 
\epsilon = N- D = \text{Round-off Error}
\end{cases}$$
so numerical solution of the computer can be written as the sum of the 
$$N = D + \epsilon$$
then we call the part $\epsilon$ , i.e. <mark style="background: transparent; color: yellow">The round-off error, is called the error of the equation</mark>, we simply write it as the $e$ later. 
***Solution is stable if :*** 
$$\left| \frac{e^{n+1}_i}{e^{n}_i}\right|\leq  1$$
or we can expand the error $e_{i}^n$ as a continuous function $Z(x,t)$, if 
$$\left| \left|Z\left( x,t\right) \right|\right|\leq  K ||Z(x,0)||$$
where $K = constant$ then the difference form is stable. 
The solution and the analysis can be found in the [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/⛲Computational Fluid Dynamics/CFD of J.D. Anderson/Part 2 Basics of the Numerics/Problems&Examples/3 examples for the convergence analysis.pdf|3 examples for the convergence analysis.pdf]]

note that the <mark style="background: #FFF3A3A6;">Transfer Equation of the error</mark> is also noted in this equation as  
$$e_{i}^{n+1} = e_i^n + \alpha \frac{\Delta t}{(\Delta x)^2}(e_{i+1}^n + e_{i}^n + e^{n}_{i-1})$$

### (2) Von-Neumann method for Stability analyzation 
Von-Neumann method for Stability analyzation is also called the method of Fourier expansion. 

`````ad-todo 
title: Fourier Expansion of the error -> p159 and PPT p90-p96
collapse: open
`````

### (3) Relationship of Compatibility, Convergence and the Stability of the equations 

The compatibility is the necessary condition of the convergence. 

also we have the **Lax law** : 
for a ==Well-posed linear question which satisfy the compatibility condition of the solution==, if the form of the equation is stable, then it converges, and if it's unstable then it not converges. 


## 6. General difference forms of the differential function
### (1) The Convection Function 
$$\begin{cases}
\frac{\partial \xi}{\partial t} + \alpha \frac{\partial \xi}{\partial x} = 0 \\
\xi_{x,0} = \bar{\xi}(x)
\end{cases}$$
##### 1. Upwind Difference Form
a general form of the difference for the convection equation is : 
$$\xi_i^{n+1} = \xi_i^{n} - \frac{\lambda}{\alpha}\left[ \frac{1}{2}(\alpha + |\alpha|)(\xi_i^n + \xi_{i-1}^n)+ \frac{1}{2}\left(\alpha - |\alpha|\right) (\xi_{i+1}^n - \xi_i^n)\right]$$
where 
$$\lambda  = \alpha \frac{\Delta t}{\Delta x}$$


in the equation above, ==the parameter $\alpha$ is amount to the velocity $u$ in the momentum equation== 
1) FTBS -> when $\alpha > 0$ the equation becomes $\xi_{i}^{n+1} = \xi_i^n - \lambda(\xi_i^n - \xi_{i-1}^n)$
then that form of the difference is called ***Upwind Difference***
the stability condition is : $|\lambda | \leq 1$, and the truncate error is $R(\Delta t,\Delta x^2)$
in this form, the equation is in the form 

2) another difference form is FTFS -> when $\alpha<0$ the equation becomes $\xi_i^{n+1} = \xi_i^n -\lambda(\xi_{i+1}^n - \xi_i^n)$

##### 2. Lax-Wendroff Difference form
Using the Taylor's series of $\xi$ consider the time, we can reach 
$$\xi_i^{n+1} = \xi_i^n + \Delta t(\xi_t)_i^n + \frac{(\Delta t)^2}{2}(\xi_{tt}) + O((\Delta t)^3)$$
and Using the Differential equation 
$$\begin{cases}
\xi_t = -\alpha \xi_x \\
\xi_{tt} = \alpha^2 \xi_{xx}
\end{cases}$$
we have
$$\xi_i^{n+1} = \xi_i^n - \alpha\Delta t(\xi_x)_i^n + \frac{\alpha^2(\Delta t)^2}{2}(\xi_{xx}) + O((\Delta t)^3)$$
Then the Lax-Wendroff Form of the difference equation can be written by substituting the difference form into the equation : 
$$\xi_i^{n+1} = \xi_i^n -\frac{\lambda}{2}(\xi_{i+1}^n - \xi_{i-1}^n) + \frac{\lambda^2}{2}(\xi_{i+1}^n  - 2 \xi_{i}^n +\xi_{i-1}^n) + O((\Delta t)^3)$$
where $\lambda = \frac{\alpha\Delta t}{\Delta x}$
##### (3) Whole Implicit Form
For the implicit form of the equation, there may be many terms of the time $n+1$ 
the implicit form for the convection function is reached by writing $\left(\frac{\partial \xi}{\partial x} \right)_i^n = \frac{\xi_{i+1}^{n+1} - \xi_{i-1}^{n+1}}{2\Delta x}$
then : 
$$\xi_i^{n+1} - \frac{\lambda}{2}(\xi_{i+1}^{n+1} - \xi_{i-1}^{n+1}) = - \xi_i^n$$
this form is ***unconditionally stable*** in solution. 

### (2) The Diffusion Function 
$$\begin{cases}
\frac{\partial \xi}{\partial t} = \beta\frac{\partial^2 \xi}{\partial t^2} \\
\xi(x, 0) = \bar{\xi}(x)
\end{cases}$$
##### 1. Classical form
$$\xi_{i}^{n+1} = \xi_{i}^n + \mu (\xi_{i+1}^n - 2\xi_i^n + \xi_{i-1}^n)$$
where 
$$\mu = \frac{\beta \Delta t}{(\Delta x)^2}$$
the stable condition is $\mu \leq  \frac{1}{2}$

##### 2. Implicit form of equation
the difference method is : 
$$\frac{\partial^2 \xi}{\partial x^2} = \frac{\xi_{i+1}^{n+1} - 2\xi_{i}^{n+1} + \xi_{i-1}^{n+1}}{(\Delta x)^2}$$
$$\frac{\partial \xi}{\partial t} = \frac{1}{2}\left[\left(\frac{\xi_i^n - \xi_i^{n-1}}{\Delta t }\right) + \left( \frac{\xi_i^{n+1} - \xi_i^n}{\Delta t}\right) \right]$$

`````ad-note
title: The difference form of the convection-diffusion function. 
collapse: open

for the convection-diffusion function, i.e. 

$$\begin{cases}
\frac{\partial \xi}{\partial t} + \alpha \frac{\partial \xi}{\partial x} = \beta \frac{\partial^2 \xi}{\partial x^2} \\
\xi(x,0) = \bar{\xi}(x)
\end{cases}$$

it also has the Upwind difference form, the implicit difference difference form and the central explicit difference form. 
`````

