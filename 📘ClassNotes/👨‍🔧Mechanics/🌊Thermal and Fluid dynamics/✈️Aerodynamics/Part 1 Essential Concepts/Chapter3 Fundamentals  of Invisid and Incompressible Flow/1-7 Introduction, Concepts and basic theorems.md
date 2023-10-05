## 3.1 Incompressible flow 
The incompressible flow has constant density, and the analysis applies to ==the flow of liquids==, and also can be applied to the low-speed gases, that is, <mark style="background: transparent; color: red">slower than 0.3 or less Mach number (in this range the density of air just vary a few percent)</mark>. The Inviscid Flow and the Incompressible flow are ideal model for analysis in engineering application. 

Some **Basic Theorems** are Listed as follows:
1. **Bernoulli's equation** 
2. **Laplace's equation** 
3. Elementary Flow Patterns `->` Kutta - Joukowski theorem 
4. *panel technique* `->` numerical method for the solutions of flow over bodies of general shape. 

## 3.2 Bernoulli's Equation 
For inviscid, incompressible flow, the Bernoulli Equation is:
$$\Large\boxed{p + \frac{1}{2}\rho V^2 = \text{const}}$$

It can be derived by referencing the [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 1 Essential Concepts/Chapter2 Fundamental Principles and Equations/9-16 The Application Concepts|Chapter2]]
we considet the $x$ component of the Bernoulli's equation. Referencing the [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 1 Essential Concepts/Chapter2 Fundamental Principles and Equations/1-8 Fundamental Principles#(2) partial differential equation of the momentum formula|Momentum equation of fluid]], and we ignore the body force. 
$$\rho \frac{Du}{Dt} = - \frac{\partial p}{\partial x}$$
$$\boxed{\rho \frac{\partial u}{\partial t} + \rho u \frac{\partial u}{\partial x} + \rho v \frac{\partial u}{\partial y} +  \rho w \frac{\partial u}{\partial z} = -\frac{\partial p}{\partial x}}$$
also we give the [[1-16 Fundamental Principles 2023-01-28 20.31.38.excalidraw|Implements and the clarification]], and **we have the approxiamtion that the rate of change of momentum that fluid in *x direction* leads to is equal to the change of momentum in x-direction** 

for steady flow, we have $\frac{\partial u}{\partial t} = 0$, then the equation above is written as : 
$$u \frac{\partial u}{\partial x} + v \frac{\partial u}{\partial y} + w\frac{\partial u}{\partial z} = -\frac{1}{\rho}\frac{\partial p}{\partial x}$$
we consider **a long streamline** in 3-D space. The **equation of a streamline** is given by [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 1 Essential Concepts/Chapter2 Fundamental Principles and Equations/9-16 The Application Concepts#2.11 Path line, streamline and Streak line of a flow#(2) Stream Line|streamline equation]] (2.117), then we can multiply the equation above by $dx$,  and obtain:
$$u \frac{\partial u}{\partial x} dx+ v \frac{\partial u}{\partial y} dx+ w\frac{\partial u}{\partial z} dx= -\frac{1}{\rho}\frac{\partial p}{\partial x}dx$$
then we get **the momentum function along the stream line**: 
$$\boxed{u \left(\frac{\partial u}{\partial x} dx+ \frac{\partial u}{\partial y} dy + \frac{\partial u}{\partial z} dz \right) = - \frac{1}{\rho} dx}$$
since $du = \left(\frac{\partial u}{\partial x} dx+ \frac{\partial u}{\partial y} dy + \frac{\partial u}{\partial z} dz \right)$, we have :
$$\boxed{udu = \frac{1}{2} d(u^2) = -\frac{1}{\rho}\frac{\partial p}{\partial x}dx}$$
then similarity we have:
$$\frac{1}{2} d(u^2 + v^2 + w^2) = -\frac{1}{\rho} \left(\frac{\partial p}{\partial x}dx + \frac{\partial p}{\partial y}dy + \frac{\partial p}{\partial z}dz\right) \qquad \rightarrow \frac{1}{2} d(V^2) = - \frac{d p}{\rho}$$
then we get <b><mark style="background: transparent; color: blue">Euler's equation</mark></b>: 
$$\Large\boxed{dp  =  - \rho V dV}\tag{3.12}$$
`````ad-caution
title: condition for Euler's equation
collapse: open
Euler's equation applies to **inviscid flow with no body forces**, and relates the change in velocity along a streamline $dV$ to the change in pressure $dp$ **along the same streamline**

also, Bernoulli's equations holds for incompressible flow only. 
`````

also along the streamline , we integral the Euler's equation and get <b><mark style="background: transparent; color: blue">Bernoulli's Equation</mark></b>
$$\Large\boxed{p_2 + \frac{1}{2} \rho V_2^2 =  p_1 + \frac{1}{2} \rho V^1_2}\tag{3.13}$$
or
$$p + \frac{1}{2} \rho V^2 = \text{const}\qquad  \text{along same streamline}$$
the equations above clarified that *when the velocity increases , the pressure decreases.*

## 3.3 Incompressible flow in a duct 
we consider a flow through a duct, also we assume that the **flow-field properties is uniform throughout the cross section**.
the cross section area varies as a function of $x$ ($A = A(x)$), and all flow-field properties are also assume to be function of $x$ only, which means we have 
$$A = A(x)\qquad V =V(x) \qquad p = p(x)$$
which is called *quasi-one-dimensional flow* 
![[Excalidraw/1-8 Introduction and Concepts 2023-08-17 09.08.30|700]]
we consider the continuity equation : 
$$\frac{\partial }{\partial t}∰_{\mathcal{V}} \rho d\mathcal{V} + ∯_{S} \rho V \cdot dS = 0\tag{2.48}$$
and for steady flow, we have:
$$\Large \boxed{∯_{S} \rho V \cdot dS = 0}\tag{3.17}$$
also we consider different part in figure above in the equation : since we have $V \cdot  dS= 0$ on wall, we have : 
$$\boxed{\iint_{A_1} \rho V \cdot  dS + \iint_{A_2} \rho V \cdot dS = 0}\tag{3.18}$$
then if the velocity of flow on the section is a constant, then$-\rho_1 A_1 V_1+\rho_2 A_2 V_2 = 0$, that is: 
$$\Large \boxed{\rho_1 A_1 V_1 = \rho_2 A_2 V_2}\tag{3.21} \overset{\rho = \text{const}}{\rightarrow } A_1 V_1 = A_2V_2$$
this is <b><mark style="background: transparent; color: blue">quasi-one-dimensional continuity equation</mark></b>
with the continuity equation and the [[#3.2 Bernoulli's Equation|Bernoulli's equation]] (3.13), then we can easily derive : 
$$\boxed{V_1 = \sqrt{\frac{2(p_1 - p_2)}{\rho_2 \left(\frac{A_1}{A_2}\right)^2 - \rho_1}}}\tag{3.26}$$
this equation gives the relationship of  inlet air velocity $V_1$ the in terms of pressure difference $p_1 - p_2$ nd the density $\rho$. 
a venturi can be used to measure airspeeds, but the most common method for measuring the airspeed is using <b><mark style="background: transparent; color: blue">Pitot tube</mark></b> [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/♨️Engineering thermodynamics/第十一章 喷管内的流动方程|Pitot tube method for measuring the airspeed]]
![[Excalidraw/1-8 Introduction and Concepts 2023-08-17 20.20.54|450]]
we consider an aerodynamic model. the air converges to a smaller area when it pass to a nozzle, after passed through the aerodynamic model, it <mark style="background: transparent; color: red"> passes into a diverging duct</mark> called a **diffuser**. 
also we have the following relationship: 
$$V_2 = \frac{A_1 V_1}{A_2}\qquad  V_3 = \frac{A_2 V_2}{A_3}$$
then the pressure and the various locations 
$$p_1 + \frac{1}{2} \rho V_1^2 = p_2 + \frac{1}{2}\rho V_2^2  = p_3 + \frac{1}{2} \rho V_3^2$$
then we solve the $V_2$ by the parameter in the section 1: 
$$V_2 = \sqrt{\frac{2 (p_1 - p_2)}{\rho\left[ 1  - \left(\frac{A_2}{A_1}\right)^2\right]}}$$
also in low-speed wind tunnels. the ==product of density and the acceleration of gravity== $g$ <mark style="background: transparent; color: red">of the liquid in the manometer</mark>. and we denote this weight by $w$, then we can write the relationship between difference of pressure and the difference of height of liquid in the manometer $\Delta h$. and we set $\rho g  = w$ is the weight per unit volume.
$$p_1 - p_2 = \rho_{\text{liquid}} g \Delta h = w g = \frac{1}{2} \rho_{gas} V_1^2  - \frac{1}{2}\rho_{gas} V_2^2$$
we have 
$$\Delta h = \frac{\rho_{\text{gas}} V_1^2}{2 \rho_{\text{liquid}} g} - \frac{\rho_{\text{gas}}V_2^2}{2\rho_\text{liquid}g}$$
also we can get : 
$$V_2  = \sqrt{\frac{2 \rho_{\text{gas}} g \Delta  h}{\rho_{\text{liquid}} \left[ 1 - \left(\frac{A_2}{A_1}\right)^2\right]}}$$
`````ad-caution
collapse: open
be careful that in the equation $p + \frac{1}{2} \rho V^2 = \text{const}$, the density is the density of air $(\rho_{\text{gas}})$
However, in the equation $p_1 - p_2 = \rho g h$, the density is the density of the liquid in the manometer.
`````

## 3.4 Pitot Tube 
The Pitot Tube is worked as the figure below. The **Pitot tube is the most common device for measuring the flight velocities of airplanes**.
![[Excalidraw/1-8 Introduction and Concepts 2023-08-18 09.44.01|400]]
we consider a flow with pressure $p_1$ moving with velocity $V_1$. this pressure is called <mark style="background: transparent; color: red">Static Pressure</mark>, which is <u>totally caused by the random motion of the molecules in the gas.</u> 
However, the fluid of the flow in  streamline DB slow down as the as they get closer to the Pitot tube. and **the gas velocity go to zero right at the point B**.

Any point in a flow *where the velocity becomes zero* is called a **stagnation point**. Since the pressure increases as the velocity decrease, we have $p_B > p_1$. The pressure stagnation point is called **stagnation pressure**, which is denoted by $p_0$. so we have $p_B = p_0$. stagnation pressure is also called **total pressure**. (this pressure is exerted by the flow at the tube inlet)

the pressure gage at point C ==reads $p_0$and this also allows the calculation of $V_1$ in Bernoulli's equation==, so we can write the following solution: 
`````ad-caution 
collapse: open
the pressure of $C$ is directly reads $p_0$(since pitot tube is vacuum), but when calculation we should subtract the $p_1$ (static pressure) `->` for standard sea level pressure, it's $2116 lb/ft^2$
`````
$$\boxed{p_1 + \frac{1}{2} \rho V_1^2 = p_0}$$
then we have : 
$$\Large \boxed{V_1 = \sqrt{\frac{2(p_0 - p_1)}{\rho}}}\tag{3.34}$$
also it's possible to combine the pressure measurement of the total and static pressure. we set the term  $\frac{1}{2}\rho V_1^2$ as the **dynamic pressure** and also denote it by $q_1$, then we have:
$$p_1 + q_1 = p_0\qquad \text{or}\qquad \boxed{q_1 = p_0 - p_1}\tag{3.35}$$
## 3.5 Pressure Coefficient 
The pressure is a dimensional quantity. also in [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/♎Important Conclusions and Lists/Dimensionless force and moment coefficients|Chapter 1]] we have introduced many essential dimensionless quantities. we can get the definition of *pressure coefficient* as : 
$$C_p = \frac{p - p_{\infty}}{q} = \frac{\frac{1}{2} \rho (V_{\infty}^2 - V^2)}{\frac{1}{2} \rho V_{\infty}^2} \tag{3.36}$$
then we get : 
$$\Large\boxed{C_p = 1 - \left(\frac{V}{V_{\infty}}\right)^2}\tag{3.38}$$
this equation is a useful equation for the pressure coefficient. <mark style="background: transparent; color: red">but it just holds for incompressible flow only</mark>

`````ad-caution
collapse: open
keep in mind that for the location where $V > V_{\infty}$ or $p < p_{\infty}$, $C_p$ is a negative value.
`````

another property of the pressure coefficient can be seen by using the equation 3.36 as:
$$p = p_{\infty}  + C_p \cdot  q_{\infty}$$
this situation $C_p$ tells us **how much differs from $p_{\infty}$  in multiples of the dynamic pressure q**. then if $C_p = 1$, we have $p = p_{\infty} + q_{\infty}$, that is *"one times the dynamic pressure above the static pressure"*.

we also note that the $C_p$ is *given* in most of the problems because $C_p$ is almost unchanged when the velocity increase. in [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 1 Essential Concepts/Chapter1 Introductory Thoughts/1-11 Some Introductory Thoughts#1.8 Flow Similarity|Flow Similarity]] we know that $C_p$ is a similarity parameter and **depend only on Mach, Reynolds number, shape and the orientation of the body**. 

`````ad-note
title: Mach number Condition 
collapse: open
The condition is that freestream Mach number is less than 0.3 is essential. as the sound of speed at standard sea level is $1117ft/s$, if the freestream velocity is greater than $335ft/s$, the air should be considered compressible and the formula is no longer fit.
`````

## 3.6 Condition on velocity for incompressible flow 
We firstly introduce a basic condition on the incompressible flow. 
Firstly, we consider the physical definition of the incompressible flow. Which is, $\rho = \text{constant}$ `->` **the rate of change of the volume of a ==moving fluid element== per unit volume is zero**. we have that $\nabla \cdot  V$ is physically the time rate of change of the volume of a moving fluid element per unit volume. 
For a moving fluid element of fixed mass, the rate of change of the volume can be expressed as (referencing [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 1 Essential Concepts/Chapter2 Fundamental Principles and Equations/1-8 Fundamental Principles#2.2.6 Divergence of a Vector Field|Divergence of a Vector Field]]) : 
$$\boxed{\frac{D(\delta \mathcal{V})}{Dt}  \equiv \nabla \cdot V = 0}\qquad \text{for incompressible flow}\tag{3.39}$$
then the fact that the divergence of the velocity is zero for an incompressible flow can also be shown directly from the [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 1 Essential Concepts/Chapter2 Fundamental Principles and Equations/1-8 Fundamental Principles#2.4 Continuity equation|Continuity equation]], which gives: 
$$\frac{\partial \rho}{\partial t} + \nabla \cdot  \rho V  = 0$$
for incompressible flow, $\frac{\partial \rho}{\partial t} = 0$, so we can also derive the equation above.

## 3.7 Governing equation : Laplace's equation 
we know that from [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 1 Essential Concepts/Chapter2 Fundamental Principles and Equations/9-16 The Application Concepts#(2) Vorticity|Concept of Vorticity]] and [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 1 Essential Concepts/Chapter2 Fundamental Principles and Equations/9-16 The Application Concepts#2.15 Velocity Potential|The Velocity Potential]] that for a flow that a velocity potential $\phi$ can be defined by equation (2.154) as : 
$$\Large \boxed{V = \nabla \phi}$$
we can combine this equation with (3.39) and get : 
$$\Large\boxed{\nabla \cdot \left( \nabla \phi\right) = \nabla^2 \phi = 0}$$
this equation is the **Laplace's equation**. this is one of the most famous and extensively studied equations in mathematical physics. 

Solutions of Laplace's equation are called <b><mark style="background: transparent; color: blue">harmonic functions</mark></b>. Then it's most fortuitous that incompressible, irrotational flow is described by Laplace's equation. then the form of Laplace's equation in different coordinates are listed as follows : 

1. Cartesian Coordinates: 
$$ \Large\boxed{\nabla^2 \phi = \frac{\partial^2 \phi}{\partial x^2} + \frac{\partial^2 \phi}{\partial y^2} + \frac{\partial^2 \phi}{\partial z^2} =  0} $$
2. Cylindrical Coordinates
$$\Large\boxed{\nabla^2 \phi  = \frac{1}{r} \frac{\partial }{\partial r} \left(r \frac{\partial \phi}{\partial r} \right) + \frac{1}{r^2} \frac{\partial^2 \phi }{\partial \theta^2} + \frac{\partial^2 \phi}{\partial z^2} = 0}$$

3. Spherical Coordinates 
$$\Large\boxed{\nabla^2 \phi = \frac{1}{r^2 \sin \theta} \left[\frac{\partial }{\partial r}\left( r^2\sin \theta\frac{\partial \phi}{\partial r}\right)  + \frac{\partial }{\partial \theta} \left(\sin \theta \frac{\partial \phi}{\partial \theta} \right) + \frac{\partial }{\partial \varphi} \left(\frac{1}{\sin \theta} \frac{\partial \phi}{\partial \varphi} \right)\right] = 0}$$
also we have the the [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 1 Essential Concepts/Chapter2 Fundamental Principles and Equations/9-16 The Application Concepts#2.14 Stream Function|stream function]] which fits : 
$$\Large\boxed{u = \frac{\partial \psi}{\partial y} \qquad  v = - \frac{\partial \psi}{\partial x}}\tag{2.150}$$
then we have $\nabla \cdot V = 0$ automatically satisfied. 
Indeed, the very definition and use of $\psi$ is a statement of the conservation of mass, and therefore equation (2.150) can also be used as the continuity equation.

Also, <mark style="background: transparent; color: red">If the incompressible flow is irrotational</mark>. which can be seen through the [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 1 Essential Concepts/Chapter2 Fundamental Principles and Equations/9-16 The Application Concepts#(2) Vorticity|irrotational condition]] : 
$$\frac{\partial v}{\partial x } - \frac{\partial u}{\partial y} =0\tag{2.131}$$
which gives that : 
$$\Large\boxed{\left( \frac{\partial^2 \psi}{\partial x^2} + \frac{\partial^2 \psi}{\partial y^2}\right) =  \nabla^2 \psi  = 0} \tag{3.46}$$
this is also <b><mark style="background: transparent; color: blue">Laplace's equation</mark></b>. hence in the situation of incompressible, irrotational flow, <mark style="background: transparent; color: red">the stream function also satisfies the Laplace's condition.</mark>

`````ad-summary
title:Conclusion
collapse: open
1. Any irrotational, incompressible flow has a **velocity potential and stream function** (for the 2-dimensional flow) that <b><mark style="background: transparent; color: blue">both satisfy Laplace's equation</mark></b>.
2. Conversely, <b><mark style="background: transparent; color: blue">any soution of Laplace's equation </mark></b> <mark style="background: transparent; color: red">represents the the velocity potential or stream function</mark> for an irrotational and incompressible flow.
3. the Laplcae's equation is a **second-order linear partial differential equation**. Also the linear equation has the property that <mark style="background: transparent; color: red">the sum of any particular solution of linear differential equation is also the solution of the equation</mark>. Which means the sum 
$$\phi = \phi_1 + \phi_2 + ,,, + \phi_n$$
is also the solution of the equation `->` we can use elementry fow pattern together to get a complicated flow  pattern. 

the streamlines and pressure distribution for different bodies can be  founde in *boundary conditions*. the  Boundary conditions are therefore vital concern in aerodynamic analysis. 
`````

we note that for an airfoil model, the flow is bounded by (1) the freestream flow that occurs an infinite distance, away from the body (2) the surface of the body itself. hence we will have discussion on the boundary condition in the next section. 
![[Excalidraw/1-8 Introduction and Concepts 2023-08-20 06.04.38|300]]
## 3.7.1 Infinity Boundary Conditions 
Far away from the body, the flow approaches the uniform freestream conditions. Then we let $V_{\infty}$ be aligned with the $x$ direction, which gives : 
$$\Large\boxed{u = \frac{\partial \phi}{\partial x} = \frac{\partial \psi}{\partial y} = V_{\infty}}$$
$$\Large\boxed{v = \frac{\partial \phi}{\partial y} = -\frac{\partial \psi}{\partial x} = 0}$$

## 3.7.2 Wall Boundary Condition 
For the solid  surface, it's impossible for flow to penetrate the surface. For inviscid flow, the velocity vector must be tangent to the surface. We give this equation that component of velocity normal to the surface must be zero. 
$$\Large\boxed{V \cdot  \vec{n} = \nabla \cdot  \phi = 0}\tag{3.48}$$
or 
$$\Large\boxed{\frac{\partial \phi}{\partial n} = 0}\tag{3.48}$$
this equation gives the boundary condition at the wall. Also if we are dealing with $\psi$ rather than $\phi$. then the wall condition gives that $\psi$ is constant along a streamline: 
$$\Large \boxed{\frac{\partial \psi}{\partial s} = 0\quad  \rightarrow  \quad \psi_{\text{surface}} = \psi_{y=y_b} = \text{const}}\tag{3.48} $$
if we deal with the boundary conditions by the component of velocity $u$ and $v$, we have : 
$$\Large\boxed{\frac{dy_b}{dx} = \left( \frac{u}{v}\right)_{\text{surface}}} \tag{3.49}$$
here the $\frac{dy}{dx}$ is the slope of the surface. 
`````ad-caution
title: applicable scope for (3.49)
collapse: open
The condition given in equation $(3.49)$ is the flow tangency condition at the body surface  is used for <b><mark style="background: transparent; color: blue">all inviscid flows (not incompressible flow)</mark></b>, incompresible to hypersonic, and doesn't depend on the formulation of the problem in terms of $\phi$ and $\psi$
`````

`````ad-tip
title: boundary condition for inviscid flow 
collapse: open
for viscid flow, the influence of friction between the fluid and the solid  surface create a zero velocity as surface (boundary layer condition)
`````

For solute the whole flow field, we can firstly solve the Laplace's equation for the velocity potential $\phi$, then calculate the velocity, and finally obtain the pressure from the Bernoulli's equation. 
