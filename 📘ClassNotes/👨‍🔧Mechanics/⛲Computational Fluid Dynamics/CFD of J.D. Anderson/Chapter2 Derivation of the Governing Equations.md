## (1) Fundamental physical principles and Equations
Firstly we can review the governing equations of the fluid dynamics : the three basic physical principles gives the three governing equations of the fluid flow : 
$$\begin{cases}
\text{Continuity equation} \\
\text{Momentum equation} \\
\text{Energy equation}
\end{cases}$$
![[📘ClassNotes/👨‍🔧Mechanics/⛲Computational Fluid Dynamics/CFD of J.D. Anderson/Chapter1 Philosophy of Computational Fluid Dynamics#2) Governing equations]]

## (2) Equations for Uncompressible flow 

#### 1. Governing Equation
For the Uncompressible Flow , the governing equation becomes : 
Continuity Equation : 
$$\nabla \cdot \vec{V} = 0$$
Momentum Equation : 
$$\rho\frac{D\vec{V}}{Dt} = \rho \vec{f} - \nabla  p + \mathcal{F_{viscous}}$$
Energy Equation : 
$$\rho\frac{\partial T}{\partial t} = k\nabla^2T + p \dot{q} + \phi$$
`````ad-question 
collapse: open
momentum equation and energy equation
`````

#### 2. The Equation of Stream and Vorticity functions 
$$\frac{\partial \Omega}{\partial t} + ((\nabla \times \psi) \cdot \nabla) \vec{\Omega} - (\vec{\Omega}\cdot V)(\nabla \times \vec{\psi}) = \nabla \times \vec{F} + \frac{\mu}{\rho} \nabla^2 \Omega$$
where $\Omega = \nabla \times V = \nabla (\nabla \cdot \vec{\psi}) - \Delta \vec{\psi}$

for the 2-D flow : 
$$\frac{\partial \xi}{\partial t} + \frac{\partial \psi}{\partial y}\frac{\partial \xi}{\partial x} + \frac{\partial \psi}{\partial x}\frac{\partial \xi}{\partial y} = \frac{\partial F_y}{\partial x} - \frac{\partial F_x}{\partial y} + \frac{\mu}{\rho}\Delta \xi$$
where : 
$$\Delta \psi = - \xi$$
$$u = -\frac{\partial \psi}{\partial y} ,\quad v = -\frac{\partial \psi}{\partial x} \quad  \xi = \frac{\partial v}{\partial x} - \frac{\partial u}{\partial y}$$

## (3) Equations for Inviscid flow

![[attachments/Screenshot_20230406_232735_com.tencent.mobileqq.png|500]]

## (4) The popular-used equations for flow
convection equation : 
$$\frac{\partial \xi}{\partial t} + \alpha \frac{\partial \xi}{\partial t} = 0$$