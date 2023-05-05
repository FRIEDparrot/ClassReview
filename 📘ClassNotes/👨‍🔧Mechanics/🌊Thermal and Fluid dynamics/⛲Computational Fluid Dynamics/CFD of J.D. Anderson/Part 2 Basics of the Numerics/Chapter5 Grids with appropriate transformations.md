## 1. Difference Equation for CFD solution 
### (1) Vorticity equation for the Uncompressible Inviscid Flow 
We note the [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Chapter2 Fundamental Principles and Equations/9-16 The Application Concepts#2.14 Stream Function|Definition of the Stream Function]] at here. also gives that : 
$$\begin{cases}
u = \frac{\partial \psi}{\partial y} \\
v = -\frac{\partial \psi}{\partial x}
\end{cases} \qquad  d\psi = -v dx + udy$$
For a 2-D Uncompressible viscid flow, the vorticity is defined as twice of the angular velocity.  
[[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Chapter2 Fundamental Principles and Equations/9-16 The Application Concepts#(2) Vorticity|Definition of the Vorticity]]
$$\xi = \nabla \times V = \frac{\partial v}{\partial x} - \frac{\partial u}{\partial y} = - \frac{\partial^2 \psi}{\partial y^2} - \frac{\partial^2 \psi}{\partial x^2}$$
so we have the relationship of vorticity and the stream function: 
$$\boxed{\Large \Delta \psi = -\xi}$$
where 
$$\Delta  = \frac{\partial^2 }{\partial x^2} + \frac{\partial^2 }{\partial y^2}$$
We note a new stream function for the incompressible flow is :
[[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/🌊fluid dynamics/第五章 粘性流体运动基础/粘性流体的运行模型建立#3. N-S方程|Reference : N-S equation]] : For example : 
$$\frac{\partial u}{\partial t} + u\frac{\partial u}{\partial x} + v\frac{\partial u}{\partial y} + w \frac{\partial u}{\partial z} = g_x + \nu \nabla^2 u$$
we can write it into the form of vector, <mark style="background: transparent; color: yellow">then neglect the volume force</mark> and it becomes: 
$$\Large\boxed{\frac{\partial \xi}{\partial t} + \frac{\partial \psi}{\partial y}\frac{\partial \xi}{\partial x} - \frac{\partial \psi}{\partial x}\frac{\partial \xi}{\partial y} = 0}$$
and also for an irrotational flow, we have: 
$$\nabla \times  V = \xi = 0, \quad \text{or} \quad \Delta \psi =  0$$
### (2) Uniform coordinates 
the grid we choose for a equation can make or break the solution, so ***The Grid Generation*** is important for the analysis and the solution of the problem. 

For standard finite-difference method, we need a uniform grid. Hence for a general problem, we can use the grid on the right picture rather than the first one. 

Note that there's two reasons for the choice of the second grid : 
1. For the grid points that are inside the airfoil, i.e. completely out of flow, It's hard for we to ascribe suitable flow characters to these points. 
2. In the left grid, there are many points that are out of the surface of the airfoil, which makes the boundary condition hard to describe. 
![[Excalidraw/Chapter5 Grids with appropriate transformations 2023-04-14 22.19.18|500]]
then we can transform the grid into a uniform one: 
![[Excalidraw/Chapter5 Grids with appropriate transformations 2023-04-14 22.29.07|200]]
The points on the physical plane are correspond to the points on the computational plane, and the governing equations are solved by a finite difference method which is carried out in the computational space. The governing equations are solved in the computational space, which says that the governing equations should be transformed from $(x,y)$ to $(\xi,\eta)$ as the new independent variables. 
### (3)General Transformation of the Equations 
##### 1) 1-D Transformations for the Coordinates 
For one-dimensional steady flow, we set : 
$$x = x(\xi)$$
and then 
$$dx = x_\xi d\xi\qquad  d\xi = \frac{dx}{x_\xi}$$
where $x_\xi$ is the 1st order derivate of the coordinate $x$ about $\xi$. 

also we can set 
$$\xi = \xi(x)\qquad  d\xi = \xi_x dx$$
hence we reach the relationship of the 1st derivate of $\xi$ and $x$, i.e. 
$$\boxed{\xi_x = \frac{1}{x_\xi}}$$

##### 2) 2-D Transformation of the Coordinates  
we consider a two-dimensional unsteady flow, with independent variables $x,y$ and $t$
$$\begin{cases}
\xi = \xi(x,y,t) \\
\eta = \eta(x,y,t) \\
\tau =  \tau(t)
\end{cases}$$
For the time being, the transformation is written in generic form, and for an actual application , this transformation must be given as specific analytical transformation. 

By the derivation process we can reach that: 
$$\boxed{\frac{\partial }{\partial x} = \left( \frac{\partial }{\partial \xi}\right)\left(\frac{\partial \xi}{\partial x}\right) + \left(\frac{\partial }{\partial \eta} \right)\left(\frac{\partial \eta}{\partial x}\right)}\tag{5.2}$$
$$\boxed{\frac{\partial }{\partial y} = \left(\frac{\partial }{\partial \xi} \right)\left(\frac{\partial \xi}{\partial y} \right) + \left( \frac{\partial }{\partial \eta}\right)\left( \frac{\partial \eta}{\partial y}\right)}$$
$$\boxed{\frac{\partial }{\partial t} = \left( \frac{\partial }{\partial \xi}\right)\left( \frac{\partial \xi}{\partial t}\right) + \left( \frac{\partial }{\partial \eta}\right)\left(\frac{\partial \eta}{\partial t} \right) + \left(\frac{\partial }{\partial \tau} \right)\left(\frac{\partial \tau}{\partial t} \right)}$$
other essential equations are derivate in the [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/⛲Computational Fluid Dynamics/CFD of J.D. Anderson/Part 2 Basics of the Numerics/Implements/Derivation for transformation of 2-D computational plane.pdf|Derivation for transformation of 2-D computational plane.pdf]]

### (4) Matrix and Jacobians 
since in the derivation of the 2D transformation for computational plane, we have : 
$$\frac{\partial u}{\partial x} = \left( \frac{\partial u}{\partial \xi}\right)\left(\frac{\partial \xi}{\partial x}\right) + \left(\frac{\partial u}{\partial \eta} \right)\left(\frac{\partial \eta}{\partial x}\right)$$
using [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/⛲Computational Fluid Dynamics/CFD of J.D. Anderson/Part 2 Basics of the Numerics/Implements/Derivation for 2D Jacobian.pdf|Derivation for 2D Jacobian.pdf]]
we reach 
$$\boxed{\frac{\partial }{\partial x} = \frac{1}{J} \left[\left(\frac{\partial }{\partial \xi}\right)\left( \frac{\partial y}{\partial \eta}\right) -\left( \frac{\partial }{\partial \eta}\right)\left(\frac{\partial y}{\partial \xi} \right)\right]}$$
$$\boxed{\frac{\partial }{\partial y} = \frac{1}{J}\left[\left(\frac{\partial }{\partial \eta} \right)\left( \frac{\partial x}{\partial \xi}\right)- \left( \frac{\partial }{\partial \xi}\right)\left( \frac{\partial x}{\partial \eta}\right) \right]}$$
where : 
$$J = \left| \begin{matrix}
\frac{\partial x}{\partial \xi} & \frac{\partial y}{\partial \xi} \\
\frac{\partial x}{\partial \eta} & \frac{\partial y}{\partial \eta}
\end{matrix} \right|$$
`````ad-todo
title: PPTp172
collapse: open

`````
