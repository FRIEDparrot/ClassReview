## 3.1 Incompressible flow 
The incompressible flow has constant density, and the analysis applies to ==the flow of liquids==, and also can be applied to the low-speed gases, that is, <mark style="background: transparent; color: red">slower than 0.3 or less Mach number (in this range the density of air just vary a few percent)</mark>. The Invisicd Flow and the Incompressible flow are ideal model for analysis in engineering application. 

Some **Basic Theorems** are Listed as follows:
1. **Bernoulli's equation** 
2. **Laplace's equation** 
3. Elementary Flow Patterns `->` Kutta - Joukowski theorem 
4. *panel technique* `->` numerical method for the solutions of flow over bodies of general shape. 

## 3.2 Bernoulli's Equation 
For inviscid, incompressible flow, the Bernoulli Equation is:
$$\Large\boxed{p + \frac{1}{2}\rho V^2 = \text{const}}$$

It can be derived by referencing the [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Chapter2 Fundamental Principles and Equations/9-16 The Application Concepts|Chapter2]]
we considet the $x$ component of the Bernoulli's equation. Referencing the [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Chapter2 Fundamental Principles and Equations/1-8 Fundamental Principles#(2) partial differential equation of the momentum formula|Momentum equation of fluid]], and we ignore the body force. 
$$\rho \frac{Du}{Dt} = - \frac{\partial p}{\partial x}$$
$$\boxed{\rho \frac{\partial u}{\partial t} + \rho u \frac{\partial u}{\partial x} + \rho v \frac{\partial u}{\partial y} +  \rho w \frac{\partial u}{\partial z} = -\frac{\partial p}{\partial x}}$$
also we give the [[1-16 Fundamental Principles 2023-01-28 20.31.38.excalidraw|Implements and the clarification]], and **we have the approxiamtion that the rate of change of momentum that fluid in *x direction* leads to is equal to the change of momentum in x-direction** 

for steady flow, we have $\frac{\partial u}{\partial t} = 0$, then the equation above is written as : 
$$u \frac{\partial u}{\partial x} + v \frac{\partial u}{\partial y} + w\frac{\partial u}{\partial z} = -\frac{1}{\rho}\frac{\partial p}{\partial x}$$
we consider **a long streamline** in 3-D space. The **equation of a streamline** is given by [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Chapter2 Fundamental Principles and Equations/9-16 The Application Concepts#2.11 Path line, streamline and Streak line of a flow#(2) Stream Line|streamline equation]] (2.117), then we can multiply the equation above by $dx$,  and obtain:
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
a venturi can be used to measure airspeeds, but the most common method for measuring the airspeed is using <b><mark style="background: transparent; color: blue">Pitot tube</mark></b> [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/♨️Engineering thermodynamics/第十章 喷管内的流动方程|Pitot tube method for measuring the airspeed]]
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
we consider a flow with pressure $p_1$ moving with velocity $V_1$. this pressure is called **Static Pressure**, which is <u>totally caused by the random motion of the molecules in the gas.</u> 
However, the fluid of the flow in  streamline DB slow down as the as they get closer to the Pitot tube. and **the gas velocity go to zero right at the point B**.

Any point in a flow *where the velocity becomes zero* is called a **stagnation point**. 

