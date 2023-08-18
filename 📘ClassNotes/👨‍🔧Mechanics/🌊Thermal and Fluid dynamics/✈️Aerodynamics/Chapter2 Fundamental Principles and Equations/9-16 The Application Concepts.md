## 2.9 Substantial derivative

Compare to the [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/🌊fluid dynamics/第三章 流体运动概述/一、流体运动简介#3. 物质导数|substantial derivative in fluid dynamics]]
for $\rho = rho(x,y,z,t)$
$$
\frac{D\rho}{Dt} = \frac{\partial \rho}{\partial t} + \frac{\partial \rho}{\partial x} \frac{\partial x}{\partial t} + \frac{\partial \rho}{\partial y} \frac{\partial y}{\partial t} + \frac{\partial \rho}{\partial z} \frac{\partial z}{\partial t} = \frac{\partial \rho}{\partial t} + u\frac{\partial \rho}{\partial x} + v\frac{\partial \rho}{\partial y} +w \frac{\partial \rho}{\partial z}
$$
and using the operator $\nabla = i \frac{\partial}{\partial x} + j\frac{\partial}{\partial y} +k\frac{\partial}{\partial z}$
$$\boxed{\frac{D}{Dt} =\frac{\partial}{\partial t} + (V\cdot \nabla)}$$
this is the definition of the substantial derivative, which is the combination of the local derivative and the convective derivative.

the substantial derivate **states the change of properties of the fluid elements**.

also we note that the divergence of velocity equals the time rate of change of a moving fluid with fixed mass per unit volume, Which can be inferred from the following two notes : [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Chapter2 Fundamental Principles and Equations/Implements/Rate of Volume Change & Jacobian transformation|Rate of Volume Change & Jacobian transformation]] 
[[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Chapter2 Fundamental Principles and Equations/Implements/Note - Transformation Jacobian|Note - Transformation Jacobian]]

## 2.10 Fundamental Equations in terms of substantial derivative
In this process, we express the continuity, momentum and the energy equation in forms of the substantial derivate, and also we would make use of the equation 
$$\nabla \cdot(\rho V) = \rho \nabla \cdot V + V\cdot\nabla \rho$$
the original equation  
1. we write the [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/♎Important Conclusions and Lists/Basic Aerodynamic Equations#Continuum Equations#2. partial differential equation form|Continuum Equation]] in the following form :
$$\frac{\partial \rho}{\partial t} + V \cdot \nabla \rho + \rho \nabla \cdot V = 0$$
since $\frac{DV}{Dt} =\frac{\partial \rho}{\partial t} +V\cdot\nabla \rho$,we can derive that: 
$$\Large \boxed{\frac{DV}{Dt} + \rho \nabla \cdot V = 0}\tag{2.108}$$
2. the [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/♎Important Conclusions and Lists/Basic Aerodynamic Equations#Momentum Equation#2. partial differential equation form|Moment Equation]] is 
x - direction: 
$$\frac{\partial (\rho u)}{\partial t} +\nabla(\rho u V) = \frac{\partial p}{\partial x} +\rho f_x + (\mathcal{F_x})_{viscous}$$
since 
$$\nabla(\rho u V) \equiv \nabla\left[u \cdot (\rho V)\right]= u \nabla\cdot (\rho V) + \rho V \cdot \nabla u$$
thus we have the equation
$$\rho \frac{\partial u}{\partial t} + \boxed{u\left[\frac{\partial  \rho}{\partial t} +\nabla\cdot(\rho V)\right]} + (\rho V)\cdot \nabla u = -\frac{\partial p}{\partial x} + \rho f_x + (\mathcal{F_x})_{viscous}$$
and the part in the box above is exactly the continuum equation and hence its value is zero, then we get
$$\boxed{\rho \frac{Du}{Dt} = -\frac{\partial p}{\partial x} + \rho f_x + \mathcal{F}_{x, viscous}}$$
$$\boxed{\rho \frac{Dv}{Dt} = -\frac{\partial p}{\partial y} + \rho f_y + \mathcal{F}_{y, viscous}}$$
$$\boxed{\rho \frac{Dw}{Dt} = -\frac{\partial p}{\partial z} + \rho f_z + \mathcal{F}_{z, viscous}}$$
3. [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/♎Important Conclusions and Lists/Basic Aerodynamic Equations#Energy equation#2. partial differential equation form|The energy equation]] can be written as : 
$$\Large\boxed{\rho\frac{D(e+ \frac{V^2}{2})}{Dt} =\rho \dot{q} -\nabla\cdot (p V)+ \rho (f\cdot V) + \dot{Q}'_{viscous} +\dot{W}'_{viscous} }\tag{2.108}$$
we can derive it from the following process: 
[[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Chapter2 Fundamental Principles and Equations/📝Examples/Energy equation in substantial derivative|The Derivation process]]

And the detailed derivation can also be referenced from the following pdf: 
[[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Chapter2 Fundamental Principles and Equations/Implements/fluidsreview-1.pdf|fluidsreview-1.pdf]]

we called the form that we derived previously *the conservation form of the fundamental equations* 
then the form of Equation we have just derived is called *nonconservation form* 
Note that for the numerical solution of some aerodynamic problems, the conservation form sometimes leads to more accurate results.

## 2.11 Path line, streamline and Streak line of a flow
### (1) Path Line
the path line <b><mark style="background: transparent; color: blue">describe the path of the fluid elements</mark></b>
and the definition is just as it in [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/🌊fluid dynamics/第三章 流体运动概述/一、流体运动简介#2. 流线，迹线和脉线|fluid dynamics]]
the **equation of the path line** is : 
$$\frac{dx}{dt} = u(x,y,z,t) \quad \frac{dy}{dt} = v(x,y,z,t)$$

### (2) Stream Line
<b><mark style="background: transparent; color: red">the concept of the stream line is a curve whose tangent at any point is in the direction of the velocity vector</mark> </b>
we let $ds$ be a **direct element along the streamline**, then we have : 
$$\boxed{ds \times V =  0}$$
and if the ds is described as $dx \vec{i} + dy \vec{j}+ dz\vec{k}$
then **the equation of the streamline** is:
$$\boxed{\frac{dx}{u(x,y,z,t)} = \frac{dy}{v(x,y,z,t)} = \frac{dz}{w(x,y,z,t)}}$$
or
$$ds\times V = \left|
\begin{matrix}
i & j & k \\
dx & dy & dz \\
u & v & w 
\end{matrix}
\right| = 0$$
then 
$$\begin{matrix} wdy - v dz = 0 \\
u dz - w dx = 0\\
v dx - u dy = 0
\end{matrix}\tag{2.117}$$
note the mass flow  across all cross sections of a stream tube is constant.

**Note that For a steady flow, path line, stream line and the streak lines are all the same curves, and only in unsteady flow they are different**

## 2.12 Angular Velocity, Vorticity and Strain 
#### (1) Angular Velocity
In this section, we pay particular attention to the orientation of the element and its change in shape **as it moves along a streamline** as shown in the figure, the shape of it can be distorted 
![[9-18 The Application Concepts 2023-01-30 18.40.47.excalidraw|300]]

we consider an infinitesimal body moving in a flow field, and it will also rotate and distorted
The amount of the distortion depends on the velocity field 
also note that the *counterclockwise rotation by convection are considered positive*
![[9-18 The Application Concepts 2023-01-30 18.45.41.excalidraw]]
in the picture above, we just consider the rotation of $AB$ and $AC$, 
we have 
$$\theta_2 \approx \tan\theta_2 = \frac{\partial v}{\partial x} \Delta t$$
$$\theta_1 \approx \tan \theta_1 = -\frac{\partial u}{\partial y} \Delta t$$
and the equations becomes: 
$$\frac{d\theta_1}{dt} = -\frac{\partial u}{\partial y} \quad \frac{d\theta_2}{dt} =  \frac{\partial v}{\partial x}$$
***By definition, the angular velocity of fluid element is the average of the angular velocities of lines AB and AC***, so 
$$\omega_z = \frac{1}{2} \left(\frac{d\theta_1}{dt} + \frac{d\theta_2}{dt}\right) = \frac{1}{2}\left(\frac{\partial v}{\partial x } - \frac{\partial u}{\partial y}\right)$$
since the angular velocity is a vector of three dimensions, the result becomes:
$$\omega = \omega_x i+ \omega_y j + \omega_z k$$
$$\boxed{\omega = \frac{1}{2}\left[(\frac{\partial w}{\partial y} - \frac{\partial v}{\partial z}) i + (\frac{\partial u}{\partial z } - \frac{\partial w}{\partial x}) j +(\frac{\partial v}{\partial x } - \frac{\partial u}{\partial y})k\right]\tag{2.217}}$$

#### (2) Vorticity 
the ***Vorticity*** is defined as the twice of the angular velocity, and denote by the vector $\xi$ 
$$\xi = 2\omega$$
$$\boxed{\xi = (\frac{\partial w}{\partial y} - \frac{\partial v}{\partial z}) i + (\frac{\partial u}{\partial z } - \frac{\partial w}{\partial x}) j +(\frac{\partial v}{\partial x } - \frac{\partial u}{\partial y})k\tag{2.218}}$$
$$\boxed{\xi = \nabla \times V}$$
==**In a velocity field, the curl of the velocity is equal to the vorticity**==
also, if $\nabla \times V \neq 0$, the flow is called *rotational* and if $\nabla \times V = 0$, it is called *irrotational*

for a two-dimensional flow $\xi =\xi k =(\frac{\partial v}{\partial x } - \frac{\partial u}{\partial y})k$ 
then for a irrotational flow, $\xi = 0$, then 
$$\frac{\partial v}{\partial x } - \frac{\partial u}{\partial y} =0$$
is **the condition of irrotationality for two-dimension flow**

we make a distinction between rotational and irrotational flows because of the answer becomes the irrotational flows are much easier to analyze than rotational flows. 
we let the angle between AB and AC in [[9-18 The Application Concepts 2023-01-30 18.45.41.excalidraw|figure]] denoted by $\kappa$, which is initially $90\degree$
and at time $t +\Delta t$ , $\kappa$ has changed by $\Delta \kappa$
$$\kappa = -\Delta \theta_2 - (-\Delta\theta_1)$$
$$\varepsilon_{xy} \equiv -\frac{d\kappa}{dt}  = \frac{d\theta_2}{dt}-\frac{d \theta_1}{dt}$$
substitute $\theta_1$ and $\theta_2$ into the equation, then 
$$\boxed {\varepsilon_{xy} = \frac{\partial v}{\partial x} + \frac{\partial u}{\partial y}\qquad \varepsilon_{xz} =\frac{\partial u}{\partial z} +\frac{\partial w}{\partial x} \qquad \varepsilon_{yz} = \frac{\partial v}{\partial z}+\frac{\partial w}{\partial y}}$$
then the rate of strain depend solely on the velocity derivatives of the flow field. 
$$\left[
\begin{matrix}
\frac{\partial u}{\partial x} &\frac{\partial u}{\partial y} &\frac{\partial u}{\partial x} \\
\frac{\partial v}{\partial x} & \frac{\partial v}{\partial y} & \frac{\partial v}{\partial z} \\
\frac{\partial w}{\partial x} & \frac{\partial w}{\partial x} & \frac{\partial w}{\partial x}
\end{matrix}
\right]$$
in this matrix
1. the sum of the diagonal term is simply equal to $\nabla \cdot V$ (means the time rate of change of volume of a fluid element). 
2. the diagonal terms represent the *dilatation* of a fluid element
3. the off-diagonal terms are cross derivates which appear in the angular velocity and the angular strain.

#### implement: Some Basic Results
1. *the viscous flow are rotational* because of the rotational moment exerted by the shear stress
2. the inviscid flow where the *freestream is a uniform flow* with velocity $V_\infty$ , and in this flow $\frac{\partial u}{\partial y} =0, \frac{\partial v}{\partial x} = 0$ is irrotational.
3. a flow field that is original irrotational, without any internal mechanisms such as friction shear stress to generate vorticity, will remain irrotational throughout.

## 2.13 Circulation 
the circulation is a fundamental tool for the calculation of aerodynamic lift

We consider a closed curve $C$ in a flow field, and at a point on C, the circulation is defined as:
$$\boxed{\Gamma \equiv -\oint_C V \cdot  ds}\tag{2.136}$$
In aerodynamics, **it's more convenient to consider a positive circulation as being clockwise, Hence we add the minus sign before the equation above to account for the positive counterclockwise sense for the counterclockwise sense of the integral**
![[9-18 The Application Concepts 2023-01-31 00.24.08.excalidraw|250]]

if the flow is irrotational everywhere within the counter of integration $(\Gamma \times V =0)$, then $\Gamma=0$
from the [[第十一章 曲线积分和曲面积分#七、Stokes公式, 环量与旋度|Stokes' theorem]], we have the relation 
$$\boxed{\Gamma \equiv -\oint_C V\cdot ds = -\iint (\nabla \times V)\cdot dS}$$
a related result is to let the curve $C$ to shrink into infinitesimal size and the limit becomes:
$$d\Gamma = -(\nabla \times V)\cdot dS = -(\nabla \times V) \cdot \vec{n} dS$$
$$\boxed{(\nabla \times  \vec{V}) \cdot \vec{n} = -\frac{d\Gamma}{dS}}$$
where $dS$ is the infinitesimal area closed by curve $C$ 

## 2.14 Stream Function 
#### (1) Stream Function 
we consider a two-dimension flow, for a streamline, its function is given as : 
$$\frac{dy}{dx} =  \frac{v}{u} \tag{2.118}$$
and it can be integral as the *algebraic equation of the streamline* : $f(x,y) = C$
in the equation above, we denote the function of x and y by the symbol $\bar{\psi}$, then the equation becomes 
$$\bar{\psi} (x,y) =   C$$
and the function $\bar{\psi}$ is called streamline function. and then **the equation for a streamline is given by setting the streamline function equal to a constant**.
![[Screenshot_20230131_101712_com.adobe.reader(1).png|200]]
let us define the numerical value of $\bar{\psi}$ such that the difference of two streamlines are defined as : 
$$\boxed{\Delta \bar{\psi} = c_2 -c_1}$$
this difference is equal to the ==*mass* flow== between two streamlines. the mass flow between two streamlines is defined *per unit depth perpendicular to the figure plane*.
The equivalence between $\bar{\psi} = constant$ designating a streamline 
$\Delta\bar{\psi}$ itself is constant for a given stream tube along the tube 

for the streamlines, we assume the streamlines are close together and the flow velocity $V$ is a constant value  across the $\vec{n}$ The mass flow through the stream tube per unit depth is
$$\Delta \bar{\psi} \equiv \rho V \Delta n(1) \quad \text{or} \quad \frac{\Delta \bar{\psi}}{\Delta n} = \rho V$$
and for $\Delta n \to 0$
$$\rho V = \lim_{\Delta n\to 0} \frac{\Delta\bar{\psi}}{\Delta n} = \frac{\partial \bar{\psi}}{\partial n}$$
![[Pasted image 20230131105613.png|200]]
then if we represent $\Delta n$ as the $\Delta y$ and $\Delta x$: 
$$\text{Mass flow} = \Delta \bar{\psi} = \rho V \Delta n = \rho (u\Delta y + v (-\Delta x)) \tag{2.144}$$
or in the limit
$$\boxed{ d\bar{\psi} = \rho (u dy -vdx)}$$
also since 
$$d\bar{\psi}= \frac{\partial \bar{\psi}}{\partial x} dx  + \frac{\partial \bar{\psi}}{\partial y}dy$$
then we have the relationship: 
$$\boxed{\Large\begin{matrix}
\frac{\partial \bar{\psi}}{\partial x} = - \rho v  \\
\frac{\partial \bar{\psi}}{\partial y} = \rho u \hfill
\end{matrix}}$$
If written in a polar coordinate, we can redraw the equations as : 
$$\Large\boxed{
\begin{matrix}
\frac{\partial \bar{\psi}}{\partial r} = -\rho V_\theta \\ 
\frac{1}{r}\frac{\partial \bar{\psi }}{\partial \theta } = \rho  V_r
\end{matrix}}
$$

#### (2) For incompressible flow
for the incompressible flow, the $\rho = constant$ then, we can derive that $V = \frac{1}{\rho} \frac{\partial \bar{\psi}}{\partial \vec{n}}$
**We Define a new stream function $\psi$ for incompressible flow where**
$$\psi = \frac{\bar{\psi}}{\rho}$$
then we equation becomes :
$$\Large\boxed{\begin{matrix}
u = \frac{\partial \psi}{\partial y} \\
v = -\frac{\partial \psi}{\partial x}
\end{matrix}} \qquad \text{and} \qquad 
\boxed{\begin{matrix}
V_r = \frac{1}{r}\frac{\partial \psi}{\partial \theta} \\ 
V_\theta = -\frac{\partial \psi}{\partial r}
\end{matrix}}$$
In addition, since $\Delta \bar{\psi}$ denotes the mass flow between two streamlines, then 
**$\Delta \psi =\frac{\Delta \bar{\psi}}{\rho}$ denotes the volume flow between two streamlines** , and it has unit $m^2/s$

## 2.15 Velocity Potential 
for a irrotational flow, we have $\nabla \times V$ is 0s
we can refer the [definition in wiki](https://en.wikipedia.org/wiki/Velocity_potential)
then if $\nabla \times V=0$ for a flow which is satisfies [[2. 解析函数|the Cauchy-Riemann Equation]] (i.e. is an analytic function):
$$\frac{\partial u}{\partial y} = \frac{\partial v}{\partial x}$$
then we can find a function $\phi$ with $\frac{\partial \phi}{\partial x} =u,\frac{\partial \phi}{\partial y} = v$, s.t 
$$\boxed{\Large V = \nabla \phi} \qquad \text{or} \qquad\boxed{u = \frac{\partial \phi}{\partial x}, v= \frac{\partial \phi}{\partial y}, w= \frac{\partial \phi}{\partial z}}$$
then if $\phi$ is a scalar function, we have: 
$$\nabla \times (\nabla \phi) = 0$$
in the cylindrical coordinates, we have:
$$\boxed{V_r = \frac{\partial \phi}{\partial r} , V_\theta =\frac{1}{r}\frac{\partial \phi}{\partial \theta}, V_z=\frac{\partial \phi}{\partial z}}$$
$$\boxed{V_r = \frac{\partial \phi}{\partial r} , V_\theta =\frac{1}{r}\frac{\partial \phi}{\partial \theta}, V_\varphi =\frac{1}{r\sin\theta }\frac{\partial \phi}{\partial \varphi}}$$
#### The Compare of the Velocity Potential and the streamline function 
1. with $\phi$ we can derive the velocity  by differentiating it *in the same direction of flow* while we can reach the velocity by differentiating $\psi$ in the *direction normal to the velocity*
2. the **velocity Potential is defined for irrotational flow only** and streamline function can applied in both irrotational and rotational flow.
3. **the stream function is defined for only two dimensional flows only**, while the velocity Potential can applied in the 3-D flows 

also irrotational flows is called ***Potential flows*** since it can be described by the velocity potential $\phi$

## 2.16 Relationship between stream function & velocity function
for the irrotational flow $V = \nabla\phi$, a line of constant $\phi$ is an isoline of Velocity Potential (a ***equipotential line***)
and a **streamline** is a constant line of $\bar{\psi}$ for a two-dimension body $\psi(x,y) = constant$

from above equation, we have 
**1. streamline**
$$d\psi = -v dx + u dy = 0$$
then we have:
$$\left.\frac{dy}{dx}\right|_{\psi = \text{const}} = \frac{v}{u}$$
**2. equipotential line**
$$d\phi = u dx + v dy = 0$$
$$\left.\frac{dy }{dx}\right|_{\phi  = \text{const}} = -\frac{u}{v}$$
then we have
$$(\frac{dy}{dx})_{d\psi = 0} = -\frac{1}{(\frac{dy}{dx})_{d\phi =0}}$$
hence **The streamline and the equipotential line are mutually perpendicular**
