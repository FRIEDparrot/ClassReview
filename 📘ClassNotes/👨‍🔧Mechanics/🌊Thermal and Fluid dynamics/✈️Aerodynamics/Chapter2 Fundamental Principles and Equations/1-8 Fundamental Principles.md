Central to this chapter  is the derivation and  the discussion of the **three most important and fundamental equations in aerodynamics: the continuity, momentum, and energy equations.**
## 2.1 Introduction and the Road map
The most important thing is the build of the Aerodynamic tools used in the analysis.
![[Screenshot_20230125_194205_com.adobe.reader.png|400]]

## 2.2 Vector Relations 
the aerodynamics is full of the vectors such as force, velocity, etc. These are ***Vector Quantities***
Note that the content of this chapter can be found from the advanced mathematics
#### 2.2.1. Vector algebra
unit vector $\boldsymbol{n}$ is defined by the equation
$$\vec{n} = \frac{\vec{A}}{|A|}$$
the scalar product (dot product) is 
$$\vec{A} \cdot \vec{B} = |A|\cdot |B| cos \theta$$
the vector product (cross product) is 
$$\vec{A} \times \vec{B} = (|A||B| sin \theta)\vec{e}=\vec{G}$$
![[Screenshot_20230125_195414_com.adobe.reader.png|430]]

#### 2.2.2 Typical Orthogonal coordinate systems
We are interested in three common orthogonal coordinate systems:  *cartesian, cylindrical and spherical*. For a Orthogonal coordinate system, three coordinate directions are mutually perpendicular. 

For a cartesian *coordinate system* in the Fig, $i,j,k$ are the unit vectors in $x,y,z$ directions. 
in cartesian coordinate system, the point can  be located by three coordinates $(x,y,z)$ or the position vector $r = xi +yj+zk$

![[1-16 Fundamental Principles 2023-01-26 16.20.21.excalidraw|300]]
For a cylindrical coordinate system, the point $P$ in the space is given by three coordinates $(r,\theta, z)$ and
$$\begin{cases}
x = r\cos \theta \\
y = r \sin \theta \\
z= z
\end{cases}$$
or 
$$\begin{cases}
r = \sqrt{x^2 + y^2} \\
\theta = \arctan \frac{y}{x} \\
z= z
\end{cases}$$
In a spherical coordinate System, the $r$ is the distance of the point from the origin. 
$$\vec{A} = A_r \vec{e}_r + A_\theta \vec{e_\theta} + A_\varphi \vec{e}_\varphi$$
the relationship is: 
$$\begin{cases}
x = r \sin \theta cos\varphi \\
y = r \sin \theta sin\varphi \\
z = r \cos \theta
\end{cases}$$
$$\begin{cases}
r = \sqrt{x^2 + y^2 + z^2} \\
\theta = \arccos \frac{z}{r} \\
\varphi = \arccos \frac{x}{\sqrt{x^2 + y^2}}
\end{cases}$$
#### 2.2.3 Scalar and vector fields
the scalar quantity given as a function of coordinate space and the time $t$ is called a scalar field, for example, 
$$p = p(x,y,z,t)$$
also, a vector  quantity given as a function of coordinate space and time is called the vector field, and are written as :
$$V = V_x i + V_y j + V_z k$$
where
$$\begin{cases}
V_x = V_x(x,y,z,t) \\
V_y = V_y(x,y,z,t) \\
V_z = V_z(x,y,z,t) \\
\end{cases}$$
#### 2.2.4 Vector products

in cartesian coordinate:
$$A\cdot B  = A_x B_x + A_y B_y + A_z B_z$$
$$A\times B =
\left|
\begin{matrix}
\vec{i} & \vec{j} & \vec{k} \\
A_x & A_y & A_z \\
B_x & B_y & B_z
\end{matrix}
\right|$$
In cylindrical coordinates, 
$$\begin{matrix}
A \cdot B = A_r B_r + A_\theta B_\theta + A_z B_z \\
A \times B =
\left|
\begin{matrix}
\vec{\vec{e}_r} & \vec{\vec{e}_\theta} & \vec{\vec{e}_z} \\
A_x & A_y & A_z \\
B_x & B_y & B_z
\end{matrix}
\right|
\end{matrix}$$
for the Spherical coordinates, also Definition is totally similar: 
![[Pasted image 20230126173901.png|400]]

#### 2.2.5 Gradient of a Scalar Field 
the ***gradient*** of a scalar field $p$ is $\nabla p$, *it's a vector*, and it is defined as: 
1. Its magnitude is the maximum rate of change of $p$ per unit length of the coordinate space at the given point.
2. Its direction is that of the maximum rate of the change of $p$ at the given point. 
and some definitions are shown in the figure below.
![[Pasted image 20230126174546.png|300]]
we choose the arbitrary direction $s$ at the point $(x,y)$ and then Let the $\vec{n}$ be a unit vector in the $s$ direction.  
the rate of the change of $p$ per unit length in the s direction can be calculated as: 
$$\frac{dp}{ds} = \nabla p\cdot \vec{n} \tag{2.15}$$
and $\frac{dp}{ds}$ is called the ***directional derivative***. 

![[Pasted image 20230126174659.png|200]]
the expression for the $\nabla p$ in the different coordinate system are given as: 
1. cartesian:
$$p = p(x,y,z)$$
$$\boxed{\nabla p = \frac{\partial p}{\partial x}\vec{i} +\frac{\partial p}{\partial y} \vec{j} + \frac{\partial p}{\partial z}\vec{k}}$$
2. Cylindrical: 
$$p = p(r,\theta ,z)$$
$$\boxed{\nabla p = \frac{\partial p}{\partial r} e_r + \frac{1}{r}\frac{\partial p}{\partial \theta} + \frac{\partial p}{\partial z} e_z}$$
3. Spherical
$$p = p(r, \theta, \varphi)$$
$$\nabla  p = \frac{\partial p}{\partial r}\vec{e_r} + \frac{1}{r}\frac{\partial p}{\partial\theta}\vec{e_\theta} + \frac{1}{r \sin \theta}\frac{\partial p}{\partial \varphi} e_\varphi$$

#### 2.2.6 Divergence of a Vector Field 
Consider a vector field 
$$\vec{V} = \vec{V}(x,y,z) = \vec{V}(r,\theta, z) = \vec{V}(r,\theta,\Phi)$$
and the $\vec{V}$ is a vector that can represent vector quantity. however, for the practical purposes and to aid in physical interpretation. Consider $V$ to be the flow velocity.

In the section 2.3, we will prove the time rate of the change of the volume of a moving liquid element is equal to the ***divergence*** of the $\vec{V}$, which is expressed as
***divergence*** of $\vec{V}$:
$$\nabla \cdot V$$
note that **==the divergence of a vector is a scalar quantity==**. then the divergence in different vectors are shown as: 
1. Cartesian system
$$\boxed{\nabla \cdot V = \frac{\partial V_x}{\partial x} + \frac{\partial V_y}{\partial y} + \frac{\partial V_z}{\partial z}}$$
2. Cylindrical: 
$$\boxed{\nabla \cdot V = \frac{1}{r} \frac{\partial }{\partial r} (rV_r) +\frac{1}{r} \frac{\partial V_\theta}{\partial \theta} + \frac{\partial V_z}{\partial z}}$$
3. Spherical: 
$$\boxed{\nabla \cdot V = \frac{1}{r^2} \frac{\partial}{\partial r}(r^2 V_r) + \frac{1}{r \sin \theta} \frac{\partial}{\partial \theta}( V_\theta sin\theta ) +  \frac{1}{r \sin\theta}\frac{\partial V_\varphi}{\partial \varphi}}$$
#### 2.2.7 Curl of a Vector Field
We consider a vector field:
$$\vec{V} = \vec{V}(x,y,z) = \vec{V}(r,\theta, z) = \vec{V}(r,\theta,\Phi)$$
for $V$ is the velocity of the fluid element, it rotate with an *angular velocity* $\omega$  as it translates along the streamline. And we will prove the $\omega$ is one-half of the curl of $V$ 

Cartesian: 
$$\nabla \times V =
\left|
\begin{matrix}
\vec{i} & \vec{j} & \vec{k} \\
\frac{\partial }{\partial x} & \frac{\partial }{\partial y} & \frac{\partial }{\partial z} \\
V_x & V_y & V_z
\end{matrix}
\right|$$
and for the Cylindrical and Spherical system, we should pay attention to the coefficient before the bracket :
Cylindrical:
![[Pasted image 20230126192350.png|200]]
Spherical:
![[Pasted image 20230126192432.png|250]]

#### 2.2.8 Line Integrals
![[1-16 Fundamental Principles 2023-01-26 19.26.24.excalidraw]]
we define the vector $d\vec{s}=\vec{n}ds$, and the ***line Integral along the curve C from point a to b is***
$$\int^{b}_{a} A \cdot ds$$
and if C is closed, then the line integral is defined as: 
$$\oint_{C} A \cdot ds$$
where the *counterclockwise* direction around $C$ is considered positive (the direction to move that area is always on the left.)

#### 2.2.9 Surface Integrals
Consider an open surface $S$ bounded by the closed curve $C$, as shown in the Fig above. And $dS$ be an elemental area of the surface. and the orientation of $\vec{n}$ is in the direction  according to the right-hand rule for movement along $C$. 
![[1-16 Fundamental Principles 2023-01-27 15.35.25.excalidraw|200]]
and the surface integral can be defined in three ways:
$$\underset{S}{\iint} p dS = \text{surface integral of a \textbf{scalar} } p \text{ over the open surface } S \tag{2.23}$$
note the result of $(2.23)$ is a vector
$$\underset{S}{\iint} A\cdot dS = \text{surface integral of a vector } A \text{ over the open surface }S \tag{2.24}$$
the result of $(2.24)$ is a scalar 
$$\underset{S}{\iint} A \times dS = \text{surface integral of a Vector } A \text{ over the open surface }S$$
and the result of $(2.25)$ is a vector 

And If the surface is closed, $n$ points out of the surface away from the enclosed volume, 

#### 2.2.10 Volume Integrals
We consider a volume $\mathcal{V}$ in space and let $\rho$ be a scalar field in the space. and the volume integral over the volume $\mathcal{V}$ of the quantity $\rho$ is written as: 
$$\underset{V}{∰} \rho dV = \text{volume  integral of a scalar } \rho \text{ over the volume } \mathcal{V}$$
the result is a scalar
$$\underset{V}{∰} A d\mathcal{V} = \text{volume integral over the volume } \mathcal{V} \text{ of the quality } A \text{ is written as}$$
the result is a vector
#### 2.2.11 Relations between the Line, surface and the Volume integral 
We consider an area $S$ bounded by enclosed curve $C$ and  Let $A$ be a vector field. 
![[1-16 Fundamental Principles 2023-01-27 15.35.25.excalidraw|150]]
Then the integral of $A$ over the curve $C$ is related to the surface integral of the vector field $A$ over $S$ by [[第十一章 曲线积分和曲面积分#七、Stokes公式, 环量与旋度|the Stokes' theorem]]
$$\boxed{\oint A \cdot ds =\underset{S}{\iint} (\nabla\times A) \cdot dS}\tag{2.25}$$
and also consider a volume $\mathcal{V}$ enclosed by the enclosed surface $S$. the surface volume integrals of the vector field $A$ are related through the [[第十一章 曲线积分和曲面积分#六、Gauss 公式, 通量与散度|divergence theorem(Gauss theorem)]]
$$\boxed{{\Large \underset{S}{∯}} A \cdot dS ={\Large \underset{\mathcal{V}}{∰}}(\nabla \cdot A)d\mathcal{V}}$$
and If p represents a scalar field, a vector relationship analogous to the equation above is given by the ***gradient theorem*** as: 
$$\boxed{\underset{S}{∯}p dS = \underset{V}{∰} \nabla p \space dV}$$
## 2.3 Model of the fluid

The equation are generally based on three fundamental physical principles which are deeply entrenched in the observation of the nature:
1. Mass is conserved 
2. Newton's second law 
3. Energy is conserved
in the fluid we use a control volume, the fluid element definition of it is just exactly the same as [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/🌊fluid dynamics/第一章 绪论/1. 流体力学相关定义及假设|the definition in the fluid dynamics]]

The ***infinitesimal fluid element approach*** and ***the molecular approach*** are all applied
In the molecular approach, the motion of the fluid is seen as a ramification of the mean motion of the atoms and molecules, and we should apply the suitable statistical averaging method to define the resulting fluid properties. And the approach is in the purview of *kinetic energy*. and in this book the element approach is used.

#### 2.3.2 Physical Meaning of the change of the Divergence of velocity
we know from [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/🌊fluid dynamics/第三章 流体运动概述/一、流体运动简介#4. 流体微团运动分析|the definition in fluid dynamics]] that the divergence of the velocity
$$\nabla \cdot V = \text{ the time rate of change of the volume of a moving fluid elememt}$$
for the compressible fluid, we consider a control volume $\mathcal{V}$ and control surface $S$, which are change with time as the control volume moves to different regions. and different $\rho$ exists in different region of the flow. 

**The Mass of the control volume is fixed**, and it constantly change its shape, so the volume of it varies.

since the change of the control volume is
$$D\mathcal{V} = ∯_S \Delta \mathcal{V} =∯_S [(V\Delta t) \cdot n]dS= ∯_S (V \Delta t) dS$$
and we divide this integral by $\Delta t$, then we have:
$$\frac{D \mathcal{V}}{Dt} = \frac{1}{\Delta t} ∯_S (V\Delta t)\cdot dS = ∯_S V\cdot dS $$
apply the [[1-16 Fundamental Principles#2.2.11 Relations between the Line, surface and the Volume integral|the divergence theorem]] we get the change of the volume per unit time :
$$\frac{D\mathcal{V}}{Dt} = ∰_V (\nabla \cdot V)  d\mathcal{V}$$
so we imagine the $\mathcal{V}$ shrunk to a small volume $\delta \mathcal{V}$
$$\frac{D\mathcal{\delta V}}{Dt} = ∰_{\delta V} (\nabla \cdot V)  d\mathcal{V}$$
if the $\delta V$ is small enough that the $\nabla \cdot V$ is essentially the same value throughout the $\delta \mathcal{V}$, we can get the following equation: 
$$\boxed{\nabla \cdot V = \frac{1}{\delta \mathcal{V}} \frac{D(\delta \mathcal{V})}{Dt}}\tag{2.32}$$
examine the above equation, $\nabla \cdot V$ is physically ***the time rate of the change of the volume of a moving fluid element per unit volume***.

#### 2.3.5 Specification of the Flow field
In the analysis of the aerodynamics, pressure, density, temperature and the  velocity are important variables, then they are given in the figure below as:
![[Screenshot_20230127_205851_com.adobe.reader.png|280]]
note that if we just considerate the steady flow, we should continuously observe the same constant values for $p, \rho,\vec{V}$ etc., for all time. So the steady flow field is specified by the relations:
$$\begin{matrix}
p = p(x,y,z) \\
\rho = \rho(x,y,z)
\end{matrix}$$

## 2.4 Continuity equation
we now apply the function to the models of a *fixed volume* at a *fixed space*. Note that **the mass of fluid contained in the control volume can change as a function of time but the control volume is fixed in space.**

hence we refer to the [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/🌊fluid dynamics/第三章 流体运动概述/一、流体运动简介#5. 连续方程|derivation process in the fluid dynamics]]
the volume of the fluid pass through the base area $A$ is 
$$\text{Volume} = V dt\cdot \vec{n} dA $$
thus the mass flow through A is $\text{mass} = \rho V dt \cdot \vec{n} A$
then we have the *time rate of the mass flow out of the control volume*:
$$\boxed{\dot{m} = \rho (V \cdot \vec{n}) A = \rho \vec{V}\cdot \vec{A}}$$
Then a concept related to it is ***Mass Flux***, which is defined as:
$$\boxed{\text{Mass Flux} = \frac{\dot{m}}{A} = \rho  V_n}$$
and typical unit of mass flux is $kg/(s\cdot m^2)$ and $slug/(s \cdot ft^2)$
for the use of the mass flux are extensive. the components of the velocity is $u,v,\text{ and } w$ , and we can find the product $\rho u, \rho v, \rho w$ as the mass flux in the $x,y,z$ directions.

Physical principle 1: Mass can be neither created nor destroyed
###### (1) Continuum equation in integral form 
$$\begin{multline}
\text{Net Mass flow out of control volume through surface } S =  \\
\text{time rate of decrease of mass inside the control volume }\mathcal{V}
\end{multline} \tag{2.45a}$$
or
$$B = C$$
and the mass flow across the area $dS$ is $\rho \vec{V} d\vec{S}$
where 
$$B= ∯_S \rho \vec{V}\cdot d\vec{S}$$
the increase of the mass inside the control volume can be written as: 
$$ dm =∰_V \rho dV$$
and the time rate of decrease of mass is $-\frac{dm}{dt} = -\frac{d}{dt} ∰_V \rho dV$
thus we have the equation: 
$$\Large\boxed{\frac{\partial }{\partial t}∰_V\rho dV + ∯_S \rho \vec{V} \cdot d\vec{S} = 0}\tag{2.48}$$
this equation express the result in the integral form.

This form can be useful when solve the problems without concerning about the state at specific point.
but when solving the problems at a given point, this equation may not be useful. So we reduce it to the form of the flow properties at specific points as below

###### (2) Continuum equation in partial differential equation form
we assume the control volume is at a specific point, *since the space of the volume is fixed, the limits of the integration is also fixed*, and the time derivate can be placed inside the volume integral (also we can reference the [Leibniz Integral rule](https://en.wikipedia.org/wiki/Leibniz_integral_rule))
so
$$\frac{\partial}{\partial t}\underset{V}{∰} \rho d\mathcal{V} = \underset{V}{∰}\frac{\partial \rho}{\partial t} d\mathcal{V}$$
Applying the [[1-16 Fundamental Principles#2.2.11 Relations between the Line, surface and the Volume integral|Divergence theorem]]
$$∯_S(\rho V)dS  =∰_V \nabla \cdot (\rho V) d\mathcal{V}$$
then we have: 
$$\underset{V}{∰} (\frac{\partial \rho}{\partial t} + \nabla \cdot (\rho V)) d\mathcal{V} = 0$$
that is 
$$\boxed{\Large \frac{\partial \rho}{\partial t} + \nabla \cdot (\rho V) = 0\tag{2.52}}$$
this is the **continuity equation in the form of a partial differential equation**. 
we can also compare it to [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/🌊fluid dynamics/第三章 流体运动概述/一、流体运动简介#5. 连续方程|the continuity equation in the fluid dynamics and its derivation process]]

note that in the derivation of the above equations, the only assumption is about the nature of the fluid is that it is a continuum.

###### (3) Continuum equation for the steady flow
**For the steady flow**, $\frac{\partial}{\partial t}$ is 0, and the [[1-16 Fundamental Principles#(1) Continuum equation in integral form|equation (2.48)]] becomes
$$\Large\boxed{{∯_s} \rho V \cdot dS =  0}$$
and equation (2.52) is 
$$\Large\boxed{\nabla \cdot (\rho V) =0}$$
## 2.5 Momentum equation
##### (1) The momentum equation in the integral form
The Newton's second law gives the relationship of the force and momentum as (for a general form): 
$$\boxed{F =  \frac{d}{dt}(m V)}\tag{2.56}$$
by expressing force as the summation of the ***body force*** and the ***surface force***
where body force is 
$$F_V = ∰_V \rho f d\mathcal{V}$$
and the surface force on a surface in a viscous fluid is the summation of the pressure applied on the surface and the shearing stress.
$$\boxed{F_s = -p dS + F_{viscous}}$$
and thus
$$\Large \boxed{F = ∰_V \rho f d\mathcal{V} - ∯_S p dS + F_{viscous}} \tag{2.59}$$
and for the control volume, the time rate of the change of the moment of the fluid should take **the Net flow of the moment out of control volume across** $S$ into consideration.
so we have 
$$\begin{matrix}
\text{Newton flow of momentum out of} \\
\text{control Volume throught the surface } S
\end{matrix} \equiv G$$
$$\begin{matrix}
\text{Time rate of change of moment due to} \\
\text{unsteady fluctuations of flow properties inside } \mathcal{V}
\end{matrix} \equiv H$$
where 
$$\text{the flow moment per second across }dS = \rho V n dA \cdot V$$
$$G = ∯_S (\rho V \cdot dS) \space V $$
and we have the **momentum per volume** is $\rho V$, then
$$H = \frac{\partial}{\partial t}∰_V (\rho V)  d\mathcal{V}$$
then the equation is: 
$$\boxed{\frac{d}{dt}(mV) =  ∯_S (\rho V \cdot dS) \space V  +  \frac{\partial}{\partial t}∰_V (\rho V)  d\mathcal{V}}$$
then we substitute the $F \text{ (equation 2.59)}$ and the above equation into the eq$(2.56)$, we get <mark style="background: transparent; color: red">the momentum formula in the integral form</mark> as : 
$$\boxed{∰_V \rho f d\mathcal{V} -∯_S p dS + F_{viscous} =\frac{\partial}{\partial t}\left(\iiint_V (\rho V)  d\mathcal{V}\right) + ∯_S (\rho V \cdot dS) \space V}$$

##### (2) partial differential equation of the momentum formula 
just as the method we used to derive the Continuum equation, we have: 
$$∰_V \rho f d\mathcal{V} -∯_S p dS + F_{viscous} = ∰\frac{\partial (\rho V)}{\partial t} d\mathcal{V} + ∯_S (\rho V \cdot dS) V$$
[[1-16 Fundamental Principles 2023-01-28 20.31.38.excalidraw|Implements and the clarification]]
and also use the [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Chapter2 Fundamental Principles and Equations/1-8 Fundamental Principles#2.2.11 Relations between the Line, surface and the Volume integral|Divergence theorem]], then we have
$$∯_S(\rho V \cdot dS )V=  ∰_V \nabla\cdot \rho V d\mathcal{V} \space V $$
(note that it has three components in three directions), and also
$$∯_S p dS = ∰_V \nabla p d\mathcal{V}$$
thus the equation becomes (note that the divergence of the vector is a scalar)
the equations in the x direction ,y direction, and z direction are as following: 

---
$$∰_V \frac{\partial(\rho u)}{\partial t} d\mathcal{V}  + ∰_V\nabla \cdot (\rho u V) d\mathcal{V}= ∰_V \left[\rho f_x - \frac{\partial p}{\partial x} \right] d\mathcal{V} + F_{viscous,x}$$
$$∰_V \frac{\partial(\rho v)}{\partial t} d\mathcal{V}+ ∰_V \nabla \cdot (\rho v V)d\mathcal{V}= ∰_V \left[\rho f_y - \frac{\partial p}{\partial y}\right] d\mathcal{V} + F_{viscous,y}$$
$$∰_V \frac{\partial(\rho w)}{\partial t}d\mathcal{V} + ∰_V \nabla \cdot (\rho w V)d\mathcal{V}= ∰_V \left[\rho f_z - \frac{\partial p}{\partial z}\right] d\mathcal{V} + F_{viscous,z}$$
---
thus  we have : 
$$ \boxed{\frac{\partial(\rho u)}{\partial t}   + \nabla \cdot (\rho u V)= - \frac{\partial p}{\partial x} + \rho f_x + (\mathcal{F_x})_{viscous}}$$
$$\boxed{\frac{\partial(\rho v)}{\partial t}   + \nabla \cdot (\rho v V)= - \frac{\partial p}{\partial y} + \rho f_y + (\mathcal{F_y})_{viscous}}$$
$$\boxed{\frac{\partial(\rho w)}{\partial t}   + \nabla \cdot (\rho w V)= - \frac{\partial p}{\partial z} + \rho f_z + (\mathcal{F_z})_{viscous}}$$
where $(\mathcal{F_x})_{viscous}$ is the proper form of the x component of the viscous shear stress when placed inside the volume integral
we know that in the fluid dynamics, the viscous force term in the equation is calculated as
$$\mu \left(\nabla^2 V + \nabla \cdot (\nabla \cdot V)\right)$$
so we can compare it with [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/🌊fluid dynamics/第五章 粘性流体运动基础/五、粘性流体的运行模型建立|the momentum equation in the fluid dynamics]]

##### (3) momentum equation for steady and inviscid flow
Specialized for a steady$(\frac{\partial}{\partial t} =0, F_{viscous} =0)$ and inviscid flow $(\nabla \cdot V =0)$, the equation becomes: 
$$\Large ∯_S(\rho V\cdot dS)V= -∯_S p dS$$
or
$$\Large\boxed{\begin{matrix}
\nabla \cdot (\rho u V) = -\frac{\partial p}{\partial x} \\
\nabla \cdot (\rho v V) = -\frac{\partial p}{\partial y} \\
\nabla \cdot (\rho w V) = -\frac{\partial p}{\partial z} 
\end{matrix}}$$
The moment equation for an inviscid flow is called ***Euler Equations*** [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/🌊fluid dynamics/第四章 理想流体运动基础/一、理想流体流动基础#4.1 欧拉方程|Euler Equations in the fluid dynamics]]
and the momentum equations for a viscous flow is called ***Navier - Stokes Equations***

## 2.6 Application of the momentum equations
Note that the important basic equations are arranged into the note [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/♎Important Conclusions and Lists/Basic Aerodynamic Equations|Important Conclusions and Lists/Basic Aerodynamic Equations]]

at a tunnel experiment of NACA, the lift and drag characteristics of airfoil shapes are carried out in a specially designed wind tunnel where the wing models spanned the entire test section. 

the lift was obtained from the pressure distribution on the ceiling and floor of the tunnel and the drag was obtained from measurements of the flow velocity *downstream of the wing* -> we do these measurement because they involve an application of the fundamental momentum equation in integral form. 

we consider a two-dimensional body in the fig below,
![[Screenshot_20230129_150059_com.adobe.reader.png|400]]
we consider the volume enclosed by the dashed lines as the control volume. 
where (b) is the rendering of the velocity profiles downstream of airfoil (which is obtained by adding a voltage through a straight wire perpendicular to the flow.)

and consider the width of the control volume is unity, also the pressure everywhere on the $abhi$ is the same on the freestream pressure, that is $p = p_\infty$ , also assume the inflow velocity $u_1$ is uniform across the entry $ai$. and the outflow velocity $(v_{bh} = u_2)$ is not uniform. we assume $u_1 = constant$ and $u_2 =f(y)$. 

The **surface force on the control volume** is stem from two contributions as:
1. the pressure distribution over the surface $abhi$
$$- \iint_{abhi} p dS$$
2. the surface force on $def$ created by the presents of the body. 
l
the moving fluid exerts pressure and shear stress distribution over the body surface which create a resultant aerodynamic force per unit span $R'$ on the body 
![[1-16 Fundamental Principles 2023-01-29 15.47.32.excalidraw|250]]
thus 
$$\text{surface force } = -\iint_{abhi} pdS - R' \tag{2.73}$$
also this is the *total force on the control volume*. -> (because the volumetric body force is neglectable)
so the [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/♎Important Conclusions and Lists/Basic Aerodynamic Equations#Momentum Equation|Momentum Equation]] is written as :
$$\frac{\partial}{\partial t}∰_V\rho V d\mathcal{V} + ∯_S(\rho V\cdot dS) V = -\iint_{abhi} p dS - R'$$
then we assume the flow is steady, the $R'$ can be calculated as :
$$\boxed{R' = -∯_S (\rho V \cdot dS)V -\iint_{abhi} pdS}$$
then the x component of the $R'$ is $D$, thus
$$D' = -∯_S (\rho V \cdot dS) u - \iint_{abhi} (p dS)_x$$
we recall that the $p$ is constant along the boundaries far enough from the body, then the pressure is constant. then we have the equation: 
$$p_\infty \iint_{abhi} dS_x = 0$$
$$\boxed{D' = -∯_S (\rho V \cdot dS) u}$$
Note that in the above part $D'$ 
1. The section $ab,hi$ and $def$ are streamlines of the flow.  and since the $V$ is parallel to the streamlines, we have $V \cdot dS = 0$ at these lines. thus there is no contributions on the face $ab, hi$ and $def$ 
2. The cuts $cd$ and $fg$ are adjacent to each other, and the mass flux(or, moment flux) out of $dc$ is identically the mass flux into the surface $fg$ , so we have $\iint_{dc,fg} \rho V \cdot dS=0$ , thus the drag force can be evaluated as: 
![[1-16 Fundamental Principles 2023-01-29 16.21.44.excalidraw]]
$$∯_S (\rho V \cdot dS) u = - \int_i^a \rho_{in} u_1^2 dy + \int^{b}_{h}\rho_2u_2^2dy\tag{2.79}$$
(where minus sign denotes that the vector of $V$ is opposite the normal vector of integral surface)
we apply the  [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/♎Important Conclusions and Lists/Basic Aerodynamic Equations#3. For steady flow|integral of form of the continuity function]] for steady flow. 
which gives 
$$-\int^{a}_{i} \rho_{in} u_1 dy + \int^{b}_{h}\rho_2 u_2 dy =0$$
or
$$\int^{a}_{i} \rho_{in} u_1 dy = \int^{b}_{h}\rho_2 u_2 dy$$
hence we multiply the both sides with $u_1$ and obtain
$$\int^{a}_{i}\rho_{in} u_1^2 dy = \int^{b}_{h} \rho_2 u_2 u_1 dy$$
we finally have the equation 
$$\Large\boxed{D' = \int^{b}_{h} \rho_2 u_2(u_1 -u_2) dy \overset{\rho_2 \text{ is constant }}{\rightarrow }    \rho_2 \int^{b}_{h} u_2(u_1 -u_2) dy}$$
where the $(u_1 -u_2)$ is the velocity decrement at a given $y$ location, and $\rho_2 u_2$ is the mass flux, then multiply $(u_1-u_2)$ is the momentum decrement 
and the velocity distribution across the wake of a body, which can be easily measured with a Pitot rake, can yield the drag.
![[Pasted image 20230129170612.png|200]]
note that the lift on a two-dimensional body can be obtained by measuring the pressure distribution on the  ceiling and the floor.
the derivation of it can be reached by using the momentum equations in the y direction.

The weakness is that we  need to assume some form for the variation of the flow properties over the control surface.

## 2.7 Energy Equation 

For an incompressible flow, $\rho$ is constant. and the primary-flow field variables are $p$ and $V$. 

But considering that $\rho$ is an additional variable which is determined by the temperature, we need an additional equation to complete the system. -> The relationship is the energy relationship.

The additional fundamental flow-field equations are introduced in the analysis, which is ***the internal energy*** $e$  and ***the Temperature*** $T$

**Physical principle: Energy can neither created nor destroyed; it can only change in form**

Consider a fixed amount of matter contained within a closed boundary. Because the molecules and atoms are consistent in motion, **the system contains a certain amount of energy**.

we denote the internal energy per unit mass by $e$, and also define the region outside the system defines the *surroundings* and we let a incremental amount of heat $\delta q$ be added to the system. and the work of amount $\delta w$ is the work done by the surroundings, and the change of the energy is $de$

from the physical principle that the energy is conserved we have for the system 
$$\delta q + \delta w = d e\tag{2.85}$$
this is a statement of **First law of thermodynamics**
$$\begin{matrix}
B_1 =\text{rate of heat added to fluid inside control volume} \hfill \\
B_2 =\text{rate  of the work done inside control volume} \hfill \\
B_3 =\text{rate of change of energy of fluid as it flows through control volume} \hfill
\end{matrix} $$
then 
$$B_1 + B_2 = B_3\tag{2.86}$$
each term of the above equation involves the *time rate of the energy change*, which is called ***Power***, and the equation above is called ***Power Equation***

#### (1) Rate of volumetric heating
since the conservation of the energy is a energy equation, We firstly consider the rate of heat transferred to or from the fluid. -> volumetric heating of the fluid inside the control volume. 
$$\text{Rate of volumetric heating} = ∰_V \dot{q} \rho dV\tag{2.87}$$
where $\dot{q}$ is the ***rate of heat addition per unit mass***. 
Typical units for $\dot{q}$ is          $J/s \cdot kg$    or      $ft\cdot lb/s\cdot slug$

If the flow is viscous, besides heat source inside the control volume, the heat can be transferred by means of:
1. Thermal conduction
2. mass diffusion across the control surface 

and viscous heat-addition terms will be discussed in detail in Chapter 15, then the rate of heat addition is given by:
$$B_1 = ∰_V \dot{q} \rho dV + \dot{Q}_{viscous}$$

#### (2) Rate of Doing Work
By definition, the work done on the object is $W = F \cdot dr$, hence the rate of it is $P = F \cdot \frac{dr}{dt}$
that is
$$\text{Rate of doing work on moving body}= F \cdot V$$
and the integral form of the work done on body is formed by the work done by the pressure on $S$ and by the volumetric force
since the pressure on the body is defined as $F = -p dS$ , thus **the rate of work done on the fluid passing through $dS$ with velocity $V$ is $-p dS \cdot V$, hence**
$$\begin{matrix}
\text{Rate of work done on fluid inside}\\ 
\nu\text{ due to the pressure force on S}\\
\end{matrix} = ∯_S -p dS \cdot V$$
and the work done by the volumetric force is 
$$\begin{matrix}
\text{Rate of work done on fluid inside} \\
\nu \text{ due to the body force}
\end{matrix} = ∰_V (\rho f  d\mathcal{V}) \cdot V$$
where $f$ is the body force per unit mass
![[1-16 Fundamental Principles 2023-01-29 21.02.59.excalidraw|300]]
If the flow is viscous, the **shear stress on the control volume** will also perform work on the fluid. hence: 
$$B_2 = -∯_S pV\cdot dS + ∰_V \rho (f\cdot V) d\mathcal{V} + \dot{W}_{viscous}$$

#### (3) Total Energy Change in the control volume
Since the energy inside the control volume can be expressed as the summation of the kinetic energy and the internal energy. The ==internal energy== $e$ is due to the random motion of the atoms and molecules inside the system. And the ==kinetic energy per unit mass== is $\frac{V^2}{2}$ 
hence 
$$\text{energy per unit mass} = e + \frac{V^2}{2}$$
The elemental mass flow across $dS$ is $\rho V \cdot dS$. so **the mass flows into the control volume bring it with its energy and the mass flows out of it takes its energy out**. and the energy change due to the flow in or out of the control volume is: s
$$\begin{matrix}
\text{Net rate of flow of total} \\ 
\text{energy across control surface} 
\end{matrix} = ∯_S (\rho V\cdot dS ) (e + \frac{V^2}{2}) \tag{2.92}$$
the total energy change inside the control volume due to the transient fluctuations of the flow field variables can be calculated as the derivative of the total energy with respect to time
$$ \begin{matrix}
\text{Time rate of the change of total Energy} \\
\text{inside } \nu \text{ due to transient variations of flow-field variables}
\end{matrix}
=\frac{\partial }{\partial t}∰ \rho (e + \frac{V^2}{2}) d\mathcal{V}$$
and the total energy change in the volume can be calculated as: 
$$B_3=∯_S (\rho V\cdot dS ) (e + \frac{V^2}{2}) + \frac{\partial }{\partial t}∰ \rho (e + \frac{V^2}{2}) d\mathcal{V}$$
then the energy equation can be written as follows: 
$$
\boxed{
\begin{equation}
\begin{split}
∰_V \dot{q} \rho dV + \dot{Q}_{viscous} -∯_S pV\cdot dS + ∰_V \rho (f\cdot V) d\mathcal{V} + \dot{W}_{viscous}\\
= \frac{\partial }{\partial t}∰ \rho (e + \frac{V^2}{2}) d\mathcal{V} + ∯_S (\rho V\cdot dS ) (e + \frac{V^2}{2})
\end{split}
\end{equation}} \tag{2.95}
$$
this is the energy equation in the integral form. Note that if a shaft penetrates the control volume surface, the work done by it ($\dot{W}_{shaft}$) should be added into the equation.

applying the divergence theorem, we can also reach the energy equation in the partial differential form:
$$\boxed{\frac{\partial}{\partial t}\left[\rho (e + \frac{V^2}{2})\right] + \nabla\cdot \left[\rho (e+ \frac{V^2}{2})V\right] = \rho\dot{q} - \nabla \cdot(pV) + \rho (f\cdot V) + \dot{Q}'_{viscous} +\dot{W}'_{viscous}}$$

#### (4) Energy Equation of the steady, inviscid, adiabatic flow
for steady and inviscid flow,  $\frac{\partial }{\partial t} =0,Q_{viscous} = W_{viscous} =0$, and adiabatic flow $(\dot{q} =0)$, the equation (2.95) becomes: 
$$\boxed{∯_S p V \cdot dS + ∯_S \left[ \rho  \left(e + \frac{V^2}{2}\right) V \cdot dS\right] =0}\tag{2.97}$$
and 
$$\boxed{\nabla \cdot \left[ \rho(e+ \frac{V^2}{2}) V\right] = -\nabla\cdot(pV) } \tag{2.98}$$
also , if the gas if perfect, 
$$e = c_v T$$
and also the **perfect gas equation is applicable**:
$$p = \rho RT$$
