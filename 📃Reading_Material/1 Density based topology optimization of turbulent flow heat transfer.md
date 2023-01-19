
[[Density based topology optimization.pdf]]
### 1. Introduction
1. General influence factor of the convectional heat exchanger

	forced convection is widely applied in the cooling electronic devices, and in this case the performance depends on 

	-  amount of conductive material 
	-  distribution of the material
2. we can also optimize the performance by
	- change the number and the position of the fins
	- optimizing their shape 
	- changing completely in a non-uniform way the distribution of the conductive material
3. The trial of the topology optimization
	- frozen turbulence assumption(2008).
	- Spalart - Allmaras (SA) turbulent model(1 eq).
	- 1-eq k-omega  model . 
	- Reynolds-averaged Navier-Stokes(RANS) eqs.
	- 2-eq k-omega model .

### 2. Governing Equations

1. steady-state incompressible RANS eqs

$$\begin{matrix}
\nabla \cdot \boldsymbol{u} = 0 \tag{1} \\
\nabla \cdot (\boldsymbol{u}\otimes \boldsymbol{u}) = \nabla\cdot(2\nu S) - \frac{1}{\rho}\nabla p + \nabla\cdot T_t - \lambda \chi (\gamma) \boldsymbol{u}
\end{matrix}
$$
where u is the mean velocity vector. $p$ is pressure, $\nu$ is the kinematic  viscosity of the fluid,$\rho$ is the fluid density

the mean  strain rate tensor is defined as $\large S = \frac{1}{2}(\nabla u + \nabla u^T)$

the corresponding Reynold stress tensor: 
$$ T_t =-\overline{u' \otimes u' } $$
$\nu_t$ is the turbulent eddy viscosity
$u'$ indicate the turbulent fluctuating velocities.
the turbulent kinetic energy per unit mass: 
$$ k = \frac{1}{2}\overline{u' \cdot u'}$$
the material distribution is modeled using the Brinkman penalization parameter $\LARGE \lambda$

(unit: $t^{-1}$)
The penalization  term for value $\chi(\gamma) = 1$ and large enough $\lambda$,
approximates a no-slip  boundary condition at the fluid-solid interface.

$\gamma$ is a spatially varying function representing the materials distribution

for the $k-\omega$ model  of Wilcox, It is utilized to describe the effects of turbulence
in this turbulent model, the model calculate the eddy velocity $\nu_t$ as 
$$
\large
\begin{matrix}
\nu_t = \frac{k}{\tilde{\omega}} \\
\tilde{\omega} = \max \left[ \omega, C_{lim}\sqrt{\frac{2S:S}{\beta^*}} \right] \\
C_{lim} = \frac{7}{8}
\end{matrix}
\tag{*}
$$
where $S:S$ means the scalar product of two tensors
[[tensor calculation]]
![[Pasted image 20221118105048.png|300]]
![[Pasted image 20221118104923.png|300]]
The turbulent kinetic energy (per unit mass)  $k$
and the specific dissipation rate $\omega$
are obtained by solving additional transport eqs

$$\begin{matrix}
\nabla \cdot (\boldsymbol{u} \text{ } k) = T_t:\nabla u - \beta^*\omega k + \nabla \cdot \left[ (\nu + \sigma^* \frac{k}{\omega}) \nabla k\right] - \lambda \chi(\gamma)k \\
\nabla\cdot (u \text{ } \omega) = \frac{\alpha \omega}{k}T_t:\nabla u - \beta \omega^2 + \frac{\sigma_d}{\omega}\nabla k \cdot \nabla\omega + \nabla \cdot \left[ (\nu + \sigma\frac{k}{\omega})\right] + \lambda \chi(\gamma)(\omega_b - \omega)
\end{matrix}$$
the mean vorticity

the closure coefficients and other parameter can be defined as follows:
$$\begin{matrix}
\alpha = 0.52 & \beta_0 = 0.0708 & \beta^* = 0.09 \\
\beta = \beta_0 f_\beta & f_\beta = \frac{1+ 85\chi_\omega}{1+100\chi_\omega}  & \quad \chi_\omega = \left| \frac{\Omega_{ij}\Omega_{jk}S_{ki}}{(\beta^* \omega)^3} \right| \\
\sigma = 0.5 & \sigma^* = 0.6  &\sigma_{d0} = 0.125
\end{matrix}$$
$$\sigma_d= \begin{cases}
\sigma_{d0} \quad  \text { when } \nabla k \cdot \nabla\omega > 0 \\
0  \qquad \text{when }\nabla k \cdot \nabla \omega < 0
\end{cases}$$
where $\Omega$ is the mean vorticity tensor, is defined as: 
	$$\Omega  = \frac{1}{2}(\nabla \boldsymbol{u} - \nabla\boldsymbol{ u^T})$$
the $\chi_\omega$ is the closure function.
note that $\chi_\omega = 0$ for 2-D flows

2. boundary conditions and expressions

$$\begin{matrix}
k_b = 0 \\ \text{the boundary condition of turbulent kinetic energy}
\end{matrix}$$
also, the specific dissipation rate $\omega$ has a singular behavior near a wall

For smooth walls , the non-homogenous Dirichlet type boundary condition is given as:
$$\omega_b = \frac{60\nu}{\beta_1y_1^2} , \quad \beta_1 = 0.075$$
these boundary conditions are utilized in the computations
where the value of $y_1$ represents ==the distance from the wall to the cell center nearest the wall==  
the wall boundary condition $\omega_b$ is related to the length scale
$$l = \frac{\sqrt{k}}{\omega}$$
of turbulent eddies(apparently $l\to 0$ when $y\to 0$)

in the penalization process, both k and omega are penalized to their boundary condition


### 3. heat transfer
1. conjugate heat transfer equation

the  temperature variations in the fluid and solid regions are captured using the conjugate heat transfer eq: 

$$\chi_t(\gamma)\nabla \cdot (\boldsymbol{u}T) = \nabla[\alpha(\gamma)\Delta T] + Q(\gamma) \tag{11}$$
$T$ is temperature, $Q(\gamma)$ is the volume heat source
the $\chi_t(\gamma)$ is the non-dimensional function to turn on and off the convection term to turn on the convection term in fluid region or solid region
if $\chi_t(\gamma)$ = 1 , it ==turns on the convection term in fluid region==
also if $\chi_t(\gamma) = 0$ it ==turns on the convection term in the solid region==

the $\alpha(\gamma)$ interpolates between thermal diffusivity of the fluid and solid, that is: 
$$\begin{matrix}
\alpha_f = (\frac{\nu}{Pr} + \frac{\nu_t}{Pr_t}) \\
\alpha_s = \frac{k_s}{c_s\rho_s}
\end{matrix}$$
where $Pr$ and $Pr_{t}$ are laminar and turbulent Prandtl numbers
$K_s$ is thermal conductivity , $c_s$ is specific heat capacity


2. discretization

The system of PDEs is discretized using the finite volume method.
The discrete set of the algebraic equations is solved using a segregated approach for the pressure-velocity coupling. 
The SIMPLE algorithm is used in the solution

the solutions for the fluid velocity and the pressure are obtained from the discrete RANS set of eqs.

3. topology optimization

The topology optimization target:
$$\begin{matrix}
\underset{\gamma}{min}\quad\mathcal{C}(\gamma,U(\gamma),T(\gamma))\\ \\
s.t \begin{cases}
\quad \boldsymbol{R}_U(\gamma,U(\gamma)) = 0 \\ \\
\quad R_T(\gamma,T(\gamma),U(\gamma)) = 0  \\ \\
\quad g_i(\gamma) \leq 0, \quad \forall i = 1,2....N \\ \\
\quad 0 \leq \gamma \leq 1
\end{cases}
\end{matrix}
$$
where $\mathcal{C}(\cdot)$ is the objective function 

$U$ and $T$ are vectors with discrete state variables representing flow 
$R_U$ and $R_T$ are residual vector functions obtained from the discretization of the governing eqs of velocity, pressure, turbulence quantities and temperature fields.

and the set  $g_i(\gamma) \quad i = 1,2...N$ represents the additional inequality constraints and the vector $\gamma$ represents the material distribution of the design domain.

The interpolation between solid and fluid can be realized by
$$\chi(\gamma) = q \frac{1 - \bar{\gamma}}{q +\bar{\gamma}}$$
==where q controls the curvature of the function==. 

the physical design field $\bar{\gamma}$ is  obtained by regularized Heaviside projection.

The choice of Brinkman penalization parameter $\lambda$ is based on the dimensionless Darcy Number:[[1 Density based topology optimization of turbulent flow heat transfer#2. Governing Equations]]
$$Da = \frac{\nu U /L}{\lambda L U} =  \frac{\nu}{\lambda L^2}$$
where $U$ and $L$ are specific velocity and length

The diffusion and the source term coefficients of the heat equation are interpolated.

- The convection, diffusion and the source term coefficients 

$$\large\begin{matrix}
\chi_t(\gamma) = \bar{\gamma} (\gamma)^n \\
\alpha(\gamma) = \alpha_s + (\alpha_f-\alpha_s)\bar{\gamma}(\gamma)^n\\
Q(\gamma) = (1 - \gamma (\gamma)^n) \frac{\dot{q}}{c_s\rho_s}
\end{matrix}$$
$q$ is the volumetric heat source which is only active in intermediate and the solid region.
==n is the parameter to controls the curvature of the interpolation function==

The updating of $\boldsymbol{\gamma}$ is determined by using the method MMA


### 4. Discrete adjoint

the state of the solution is obtained by the SIMPLE algorithm in a matrix free fashion.
The Jacobian of such a complex set of fully coupled equations is obtained with the help of Automatic Differentiation (AD).

The objective is augmented with Lagrange multipliers as:
$$
\mathcal{L} = \mathcal{C}(\gamma,U(\gamma),T(\gamma))+\lambda^T_U R_U(\gamma,U(\gamma),T(\gamma)) + \lambda^T_U R_T(\gamma,U(\gamma),T(\gamma))
$$
and $$\frac{d\mathcal{L}}{d\gamma} = 0$$
thus we finally find $$\frac{d\mathcal{C}}{d\gamma} = \frac{\partial \mathcal{C}}{\partial \gamma} + (\frac{\partial R_U}{\partial \gamma})^T \lambda_U + (\frac{\partial R_T}{\partial\gamma})^T \lambda_T $$
since we want to find the minimum of the $\mathcal{C}$, we should let $\frac{d\mathcal{C}}{d\gamma} = 0$ and the equation will be solved.

#### 3.2 Objective and constraint functions

the objective function is to ==minimize the average temperature== in solid domains. 
==In 2D the objective function== is defined as: 
$$\mathcal{C} = \frac{\int_V (1-\gamma)T dV}{(1-f)\int_V dV}$$
where  the $f$ denotes the volume fraction of fluid in the design domain. and ==In 3D==  the domain in which the average temperature is minimized is passive. And the objective function is: 
$$\mathcal{C} = \frac{\int_V T dV}{\int_V dV}$$
The fluid volume is bounded from above, and the constraint is given as: 
$$g_1(\gamma) = \frac{\Delta V_i \gamma_i}{f V} -1 \leq 0$$
An expression for the ==power dissipation  stems from the energy eq== and can be derived as a scalar product of the momentum eqs and the velocity vector.
![[Density based topology optimization of turbulent flow heat transfer 2022-11-26 17.06.44.excalidraw]]
