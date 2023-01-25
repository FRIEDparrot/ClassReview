The resultant aerodynamic force is frequently resolved into two components defined as **lift and Drag**, but aerodynamics deal  instead with lift and drag coefficients 

## 1.1 Importance of aerodynamics & Examples

Newton assumed that upon striking a surface inclined at an angle $\theta$ to the stream, the particles would transfer their normal momentum to the surface and the tangential  momentum would be preserved, which leads the expression for the hydrodynamic force on the surface as $sin^2\theta$  ->  sine-squared law of Newton
![[Some Introductory Thoughts 2023-01-16 22.26.05.excalidraw]]

Euler pointed out the physical Inconsistency of the Newton's model and noted the fluid moving to a body "before reaching the latter, bends its direction and its velocity so when it reaches the body it flows past it along the surface"


for the theory of the long-range intercontinental ballistic missiles: 
in 1951 H. Julian Allen at the NACA Ames Aeronautical Laboratory,  introduced the concept of the *blunt* reentry body. 
![[Some Introductory Thoughts 2023-01-16 23.20.52.excalidraw]]
in that case, the way  to increase the  heating of the airflow is to  create a strong shock wave at the nose -> so  we can **use the reentry bodies rather than slender bodies to minimize the aerodynamic heating**
![[Some Introductory Thoughts 2023-01-16 23.36.28.excalidraw]]

## 1.2 Classification and Practical objectives
When a force is applied tangentially to the surface of a solid, the solid will experience a finite deformation,  and the tangential force per unit area(shear stress) would usually be proportional to the amount of deformation. 

fluid dynamics is subdivided into three areas as: 
1. hydrodynamics -- flow of liquids
2. Gas dynamics -- flow of gases
3. Aerodynamics -- flow of air

It's important for we to know about the source of the aerodynamic forces to obtain them by integrating the surface properties. 

important concepts in chapter 1: 
1. lift, drag, and the moment coefficients
2. center of pressure
3. dimensional Analysis and the  flow similarity
4. fluid statics
5. concept of the boundary layer

## 1.4 Some fundamental aerodynamic variables
the pressure, density, temperature, and flow velocity are all fundamental concepts 
the pressure at point B in a volume of fluid is defined as: 
$$p = \lim \frac{dF}{dA} \qquad dA \to 0$$
where the pressure $p$ is defined as the force per unit area. 

the density $\rho$ is defined as: 
$$\rho=\lim\frac{dm}{dv} $$
the temperature $T$ of the gas is directly proportional to the average kinetic energy of the molecular  kinetic energy of the molecules of the fluid.  
if the temperature is given by $E_k = \frac{3}{2}kT$ $\Rightarrow \text{k is the Boltzmann constant}$
where the $E_k$  is the average kinetic energy of the single molecular.
$$\text{Boltzmann constant:} \quad  k=1.380649 \times 10^{-23} \space J/K $$
Hence we can qualitatively visualize a high-temperature gas as one in which the molecular and atoms are randomly rattling. 

##### Streamlines
the concept of the velocity is more subtle than that of a solid body. For the fluid is a "Squishy" substance, and for a fluid in motion, one part of the fluid may be travel at different speed from another part. 

we consider a  fluid element in the following picture:
![[Some Introductory Thoughts 2023-01-17 23.57.35.excalidraw]]
as long as the flow is steady the path which is the traces of a moving fluid element. 

A moving fluid element traced out a fixed path in space as long as the flow is steady, this path is called ***streamline***

the friction can play a role internally in a flow. we consider two adjacent streamlines a and b of infinitesimal distance $dy$
![[Some Introductory Thoughts 2023-01-18 19.28.14.excalidraw|300]]
##### Shear Force
in the two streamlines, we assume that the velocity at the point 2 is slightly higher than the point 1.  and the streamline a is rubbing against the streamline b. on streamline b. and due to the friction, exerts a force of magnitude $dF_f$ on streamline $b$.
the tangent force acting on the unit area $dA$ is defined as  the shear force of the fluid:
$$\tau = \lim_{dA\to 0}\frac{dF}{dA}$$

##### Viscosity coefficient 
for the type of gases and liquids of interest in aerodynamics, the value of the shear force is us proportional to the spatial rate of change of velocity normal to the streamline at that point. 
and the constant of proportionality is defined as the ***viscosity coefficient*** $\mu$
and the shear force can be calculated as: 
$$\tau = \mu \frac{dV}{dy}$$
and $\frac{dV}{dy}$ is the velocity gradient.  (note that $\mu$ is not really a constant)
we deduce that in the regions where the velocity in the flow is large, the shear force $\tau$ is large


### 1.4.1 Units 

![[Pasted image 20230118204646.png|450]]

For example, units of pressure and shear stress are $lb/ft^2$ or $N/m^2$
density are $slug/ft^3$ or $kg/m^3$

## 1.5 Aerodynamic Forces and moments
##### Basic concepts
the **aerodynamic forces and moment** on the body are due to only two sources:
1. Pressure distribution over the body surface
2. shear stress distribution over the body surface
the net effect of the p and $\tau$ distributions integrated over the complete body surface is a resultant aerodynamic force $R$ and moment $M$ on the body 
![[Some Introductory Thoughts 2023-01-18 20.59.42.excalidraw|600]]
the flow far away from the freestream is called freestream.
we define the $V_\infty$ as the ***relative wind***, also called ***freestream velocity*** 

##### Relationship between normal | axial force and Lift | Drag
$$L \equiv lift \equiv \text{component of } R \text{ prependicular to }V_\infty$$
$$D \equiv drag \equiv \text{component of } R \text{ parallel  to }V_\infty$$
$$N \equiv \text{normal force}  $$
$$A = \text{axial force }$$
where the $\alpha$ is the angle of attack 
we also have the relationship: 
$$\boxed{\begin{matrix}
L =  N \cos\alpha - A \sin\alpha \\
D = N \sin \alpha + A \cos \alpha 
\end{matrix} } \tag{1.1}$$
for the detailed integration of the pressure and shear stress distributions to obtain the aerodynamics forces and the moments
we draw the chord line horizontally, and thus the relative wind inclined by the angle of $\alpha$ 

the distance from the leading edge measured along the body surface to an arbitrary point $A$ on the upper surface is $s_u$  and the the distance to an arbitrary point B on the lower surface is $s_l$. The ==pressure and the shear stress on the upper surface== is $p_u$ and $\tau_u$, and correspondingly we define $p_l$ and $\tau_l$ 
![[Some Introductory Thoughts 2023-01-18 21.36.06.excalidraw|600]]

##### Aerodynamic Force Equations

if we consider the shape of the wing that has extrude as a long cylinder, the arc becomes a piece of area $dS$, and the relation of  the force and the pressure can be described as follows: 
$$\boxed{\begin{matrix}
dN_u' = -p_u ds_u cos\theta - \tau_u ds_u sin\theta \\
dA_u' = -p_u ds_u sin\theta + \tau_u ds_u cos\theta 
\end{matrix}}$$
$$\boxed{\begin{matrix}
dN_l' = p_l ds_l cos\theta - \tau_l ds_l sin\theta \\
dA_l' = p_l ds_l sin\theta + \tau_l ds_l cos\theta
\end{matrix}}$$
note that the positive direction of all the parameters: 
the positive clockwise convention for $\theta$ must be followed
if the $\tau$ is upper inclined,  the $\theta$ would be counterclockwise and hence negative. 

hence the normal and the axial force ***per unit span*** can be written as the integration of the sum of the above two equations
$$\boxed{N' = \int^{TE}_{LE} (-p_u ds_u cos\theta - \tau_u ds_u sin\theta)ds_u + \int^{TE}_{LE} (p_l ds_l cos\theta - \tau_l ds_l sin\theta) ds_u} \tag{1.7}$$
$$\boxed{A' = \int^{TE}_{LE} (-p_u ds_u sin\theta + \tau_u ds_u cos\theta)ds_u  + \int^{TE}_{LE} (p_l ds_l sin\theta + \tau_l ds_l cos\theta) ds_u} \tag{1.8}$$
where the $LE$ is leading edge and the $TE$ is trailing edge

note that if we substitute the equation above to the equation $(1.1)$, the lift and the drag would be solved

##### Aerodynamic Moment Equations
The Aerodynamic moment depends on the point about which moments are taken. 

We consider the moments about the leading edge. By convection, the moments that tend to increase $\alpha$ are positive and decrease $\alpha$ is negative
![[Some Introductory Thoughts 2023-01-18 22.49.41.excalidraw]]
the moment per unit span can be calculated as: 
$$dM'_u = - dN_u x + dA_u y = (p_u ds_u cos\theta + \tau_u ds_u sin\theta)x  + (-p_u ds_u sin\theta + \tau_u ds_u cos\theta)y$$
$$dM_l' = -dN_l x + dA_ly= (- p_l ds_l cos\theta +  \tau_l ds_l sin\theta) x + (p_l ds_l sin\theta + \tau_l ds_l cos\theta)y$$
thus the integration expression  of the moment about  the leading edge is: 
$$\begin{matrix}
M_{LE} = \int^{TE}_{LE} \left[(p_u cos \theta + \tau_u \sin \theta) x +(-p_u sin\theta + \tau_u cos\theta) y\right]ds_u \hfill \\
\qquad \space + \int^{TE}_{LE}\left[(-p_l cos \theta + \tau_l sin\theta)x +(p_l sin\theta + \tau_l cos\theta)y \right]ds_l \hfill
\end{matrix} $$
a  major goal of theoretical aerodynamics is  to calculate the $p(s)$ and  $\tau(s)$ for a given body shape and freestream conditions. thus yielding the aerodynamics forces and moments. 

##### Dimensionless Force and Moment coefficients
as the discussion of aerodynamics progress, there are quantities  of an even more fundamental nature than aerodynamic forces and moments themselves. -> The ***Dimensionless Force*** and the ***Moment Coefficients***

![[Dimensionless force and moment coefficients]]

So In the application, we often use the dimensionless form of the coefficients introduced above. 

We have the following geometry relationships: 
$$\begin{matrix}
dx = ds \cos\theta \\
dy = -(ds \sin\theta) \\
S=  c(1)
\end{matrix}$$
![[Some Introductory Thoughts 2023-01-19 00.50.37.excalidraw|300]]

also,
![[integral forms of the force and the moments coefficients]]

also note from the equation above that the Aerodynamic force and moment coefficients can  be obtained by integrating the *pressure and the skin friction coefficients* over the body. This is a common procedure in both theoretical and experimental aerodynamics. 

## 1.6 Center of pressure
[[1-11 Some Introductory Thoughts#Aerodynamic Force Equations|The Force Equations]]
from the above equations$(1.7)$ and $(1.8)$, we can get that the normal and the axial force on the body are due to the *distributed loads* imposed by the pressure and the shear stress distributions.
Moreover, the *distributed loads* generate a moment about the leading edge. as given by Equation $(1.11)$

We can consider the situation that the aerodynamic force on a body is specified as a resultant single force $R$ or the component $N$ and $A$, then where the resultant force should be placed on -> firstly the moment and the force should be equivalent to the original pressure (i.e. produce the same effects). 

Therefore, 

If $A'$ is placed on the chord line, $N$ must be located at a distance $x_{cp}$ downstream of the leading edge. 
$$M_{LE} = -(x_{cp}) N'$$
so we get the distance of the $x_{cp}$ as:
$$\boxed{x_{cp}= -\frac{M_{LE}'}{N'}}\tag{1.20}$$
since the moment of the force is always positive(clockwise) for the leading edge, if we want to add a moment to reach the equilibrium of the moment, the moment of the resultant force at the center of pressure should be negative(counterclockwise). 

The $x_{cp}$ is defined as the *center of pressure*, It's the location where the resultant moment of a 
distributed loads loads would be zero. 

In case where the angle of attack of the body is small, $sin \alpha \approx 0$ and $\cos \alpha\approx 1$, also $L' \approx N'$
thus the equation becomes: 
$$x_{cp} = -\frac{M_{LE}'}{L'}$$
![[Some Introductory Thoughts 2023-01-20 21.56.12.excalidraw|650]]
in the picture above, the different resultant forces were given at different point of the airfoil
By inspection of the above three cases, we can get the equivalent relation ship of the different moments in Fig as: 
$$M_{LE}' = M_{\frac{c}{4}}' - L' \cdot \frac{c}{4} = - L' x_{cp}$$
## 1.7 The Buckingham PI theorem (Dimensional Analysis)
we consider what physical quantities determine the variation of these force and moments
-> the answer can be found by powerful method of ***dimensional analysis***

Consider a body of given shape at a given angle of attack. and the resultant aerodynamic force is $R$, and on a physical, intuitive basis, we *expect* $R$ to depend on:
(1) the freestream velocity $V_\infty$ 
(2) the freestream density $\rho_\infty$
(3) the viscosity of the fluid. -> depends the shear stress $\tau$, which is given by the Newton's fluid friction equation $\tau = \mu \frac{du}{dy}$
(4) the size of the body -> represented by the some of the chosen chord length $c$ (or other chosen reference length)
(5) the compressibility of the fluid. The compressibility is related to the variation of the density throughout the fluid field. 
Note that compressibility is related to the speed of sound $a$ in the fluid(shown in Chapter 8). Therefore, we represent the compressibility by the freestream speed of sound $a_\infty$

so In the light of the above, we can write that
$$R = f(\rho_\infty,V_\infty,c, \mu_\infty, a_\infty)\tag{1.23}$$
that is a general equation relation of the Aerodynamic Force and is not very practical for calculating the $R$, so In principle we could mount the given body in a wind tunnel and do experiment at the given angle of attack and then measure the variation of $R$ due to the variations of the $\rho_\infty, C_\infty$, etc.

if we won't want to cost large amount of time on the wind-tunnel experiment, we can employ the method of dimensional analysis -> which is based on a set of the dimensionless parameter we calculated before.

Dimensionless analysis is based on the obvious fact that in a physical equation, ==each term of the equation must have the same dimensions==.

for example, if
$$\psi + \eta + \zeta = \phi $$
then $\psi, \eta, \zeta, \phi$ have the same dimension, and can be made dimensionless by the action:
$$\frac{\psi}{\phi} + \frac{\eta}{\phi} +\frac{\zeta}{\phi} =1$$
> This idea is formally embodied in the Buckingham pi theorem
---
![[Buckingham pi Theorem]]
---

and then return to the consideration of the aerodynamic force on a given body at a given angle of attack.
Equations $(1.23)$ can be  written in the form 
$$f_1(\rho_\infty,V_\infty,c, \mu_\infty, a_\infty)\tag{1.27} =0$$
Following the Buckingham pi theorem -> we can find the fundamental dimensions are: 
1. $m$ =  dimension of mass
2. $l$ = dimension of length 
3. $t$ = dimension of time 
then $K =3$, thus the physical variables and their dimensions are
$$\begin{matrix}
[R] = mlt^{-2} \hfill \\
[\rho_\infty] = ml^{-3} \hfill \\
[V_\infty] = lt^{-1} \hfill \\
[c] = l \hfill \\
[\mu_\infty] = ml^{-1} t^{-1} \hfill \\
[\alpha_\infty] = lt^{-1} \hfill 
\end{matrix}$$
hence we have $N =6$, and In the above the dimensions of the force $R$ are obtained by from the Newton's second law $(kg\cdot m/ s^2)$, and the $\mu = \tau /\frac{\partial u}{\partial y}$ and the unit of the $\frac{\partial u}{\partial y}$ is $(1/s)$. 

and also the equation can also be reexpressed in terms of $N-K =3$ dimensionless $\Pi$ products in the term of the Equation $(1.25)$: 
$$f_2(\Pi_1,\Pi_2,\Pi_3) = 0$$
and from the equation $(1.26)$ above, we firstly choose the $\rho_\infty, V_\infty, c$ as the origin 
we can get the dimensionless products as: 
$$\begin{matrix}
\Pi_1 =f_3(\rho_\infty , V_\infty, c, R) \\
\Pi_2 =f_4(\rho_\infty , V_\infty, c,\mu_\infty) \\
\Pi_3 =f_5(\rho_\infty , V_\infty, c,\alpha_\infty) 
\end{matrix}$$
we can resolve the $\Pi_1$ as follows: 
assume that the $\Pi_1 = \rho_\infty^{a_1} V_\infty^{a_2} c^{a_3} R^{a_4}$
and since
$$(ml^{-3})^{a_1}(lt^{-1})^{a_2} l^{a_3} {mlt}^{-2} = 1$$
[[Some Introductory Thoughts 2023-01-21 20.46.44.excalidraw|solution of the other Pi products]]
we can solve that $a_1=-1, a_2=-2, a_3=-2$
then we have 
$$\Pi_1 = R\rho_\infty^{-1} V_\infty^{-2}c^{-2} = \frac{R}{\rho_\infty V_\infty ^2 c^{2}}$$
then we have all the other solution: 
$$\Pi_2 = \mu_\infty\rho_\infty^{-1} V_\infty^{-1}c^{-1} \quad \overset{\text{or better}}{\rightarrow} \quad \Pi_2 = \frac{\rho_{\infty}V_\infty c}{\mu_\infty}$$
$$\Pi_3 = V_\infty^{-1} \alpha_\infty \overset{\text{also}}{\rightarrow} \frac{V_\infty}{\alpha_\infty}$$
==Note that since the==
[[Dimensionless force and moment coefficients|Reference]]
$$q_\infty =\frac{1}{2}\rho_\infty V_\infty^2\qquad$$
then we can replace the term $\rho_\infty V_\infty^2$  as $q_\infty$ 
and also, the $c^2$ has the same dimensions of an Area, so we can replace the $c^2$ as $S$, thus the first term becomes: 
$$\Pi_1 = \frac{R}{q_\infty S}$$
and we have the equation 
$$f_2(\Pi_1,\Pi_2,\Pi_3) = 0$$
and substitute the dimensionless parameters in it, we get
$$f_2(\frac{R}{q_\infty S}, \frac{\rho_\infty V_\infty c}{\mu_\infty}, \frac{V_\infty}{a_\infty}) = 0$$
we write it as the form:
$$f_2(C_R, Re, M_\infty) = 0$$
so we have
$$\Large\boxed{C_R = f_6(Re, M_\infty)}\tag{1.38}$$

$C_R = \frac{R}{qS}$ is the dimensionless aerodynamic force coefficient
$Re = \frac{\rho_\infty V_\infty c}{\mu_\infty}$ is the Reynolds Number.
$M_\infty = \frac{V_\infty}{a_\infty}$ is the Mach Number.
where we define the dimensionless Reynolds number and Mach number, they are called similarity parameters, and are introduced as aerodynamic discussion process.

so **The $C_R$ is ==a function of== only $Re$ and $M_\infty$ -> so The Mach Number and Reynolds number are called the dominant similarity parameters**
Therefore, by using the Buckingham pi theorem, we have reduced the number of independent variables from 5 to 2. 

Since the lift and drag component of the resultant force, we have also the equations. and also, the aerodynamic moments has the similar relationship, so
$$\begin{matrix}
C_L = f_7(Re, M_\infty) \\
C_D = f_8(Re, M_\infty) \\
C_M = f_9(Re, M_\infty)
\end{matrix}$$
We should pay attention that the analysis above is about a specified(given) shape of the body and a given attack angle of $\alpha$. Hence the equation above can be generalized to
$$\Large\boxed{
\begin{matrix}
C_L = f_7(Re, M_\infty,\alpha) \\
C_D = f_8(Re, M_\infty,\alpha) \\
C_M = f_9(Re, M_\infty,\alpha )
\end{matrix}}$$
Note the equation above assume a given body shape.
so most theoretical and experimental aerodynamics is focused on obtaining explicit expression for the above equations

Note that for the problems that also involve thermal dynamics and  heat transfer, ==the temperature,  specific heat, and thermal conductivity of the fluid as well as the temperature of the body surface(wall temperature)== must be added into this list before.  -> and so that the dimension of the temperature should be add into the fundamental dimensions. 

For the case involve the thermal dynamics | heat transfer, the dimensional analysis yield ==additional similarity products== such as ==heat transfer coefficients==. 

additional similarity parameters like
the ratio of the specific heat at constant pressure to that at constant value  $\Large\frac{c_p}{c_v}$ 
[[The specific heat and Thermal conductivity]]
the ratio of the wall temperature to freestream temperature $\Large\frac{T_w}{T_\infty}$
the Prandtl number $\Large Pr=\frac{\mu_\infty c_p}{k_\infty}$

note that the thermal dynamics are in the Chapter 7 to 14, and the heat transfer (part of the study of the viscous flow) is in the Chapter 15 to 20.


## 1.8 Flow Similarity
#### 1. Conditions of similarity
We consider two different flows over two different bodies, and **by definition, the different flows are dynamically similar if:**
1. The streamline patterns are geometrically similar. 
2. he distributions of the $V/V_\infty, p/p_\infty,T/T_\infty$ etc., throughout the flow field are the same when plotted against the common nondimensional coordinates. 
3. The force coefficients are the same 

Note that if the nondimensional pressure and the shear stress distributions over different bodies are the same, then the nondimensional force coefficients will be the same. ->  so in reality the condition 3 is a consequence of the condition 2.  

The criteria to ensure the two flows are dynamically similar is: 
1. The  bodies and any other solid boundaries are geometrically similar for both flows
2. The ==similarity parameters== are the same for both flows

for many aerodynamic applications, the Reynolds number and the Mach number are the dominant similarity parameters. -> so in a limited sense, we can say that ==flows over geometrically similar bodies at the same **Mach** and **Reynold** number are dynamically similar== and hence the ***lift, drag, and moment coefficients will be identical*** for the bodies for the free-flight case in the test-section flow. also note that this statement is not always precise because there are other similarity parameters that influence the flow.

Differences  in the freestream turbulence between the wind tunnel and free flight can have an important impact on $C_D$ and the maximum of the $C_L$. 
[[Example1.5]]
[[Example1.6]]
through the example above, we can reach that the wind tunnel should be pressurized to reach correct similarity parameters. one of the schematic is shown below: 
![[Pasted image 20230122213217.png|500]]
#### 2. Design utilities
![[Design Box 1.8.2]]

## 1.9 Fluid Statics: Buoyancy Force
In this section, we consider the case of no fluid motion(fluid statics). A body immersed in a fluid will still experience a force even if no relative motion between the body and the fluid.
since the pressure of the above surface and the below surface are note equal: 
![[Some Introductory Thoughts 2023-01-23 17.19.12.excalidraw|300]]
from the picture above, we have:
$$\text{Net pressure force } = p(dxdz) - (p +\frac{\partial p}{\partial y} dy)(dxdz) = -\frac{\partial p}{\partial y}(dxdydz)$$
Hence we have:  
$$\text{Gravity force} = -\rho g(dxdydz)$$
for the fluid element, we have 
$$-\rho g(dxdydz) =\frac{\partial p}{\partial y}(dxdydz)$$
that is 
$$\boxed{dp = -g \rho dy}\tag{1.52}$$
the function above can be applied in the analysis of the "standard atmosphere"
if we assume $\rho$ is constant, and the $h$ below and above is separately $h_1$ and $h_2$, we can integral the equation as:
$$\int^{h_2}_{h_1} dp  = -\rho g \int^{h_2}_{h_1}dy$$
or 
$$p_2 -p_1 = -\rho g (h_2 -h_1)$$
$$\boxed{p + \rho g h = constant}$$
also we can see the force distribution over the vertical faces of the Rectangular body lead to equal and opposite force which cancel each other.
$$F = (p_2 -p_1) l(1)$$
![[Some Introductory Thoughts 2023-01-23 18.21.55.excalidraw|250]]
thus we can obtain the expression of the buoyancy force: 
$$\boxed{F = l \int^{h_1}_{h_2} \rho g dy}$$
note the limit of the integration is $h_2$ to $h_1$ $(h_2 < h_1)$
since the *weight* of total volume of the fluid is $\rho g l\int^{h_2}_{h_1}dy$
we reached the ***Archimedes principle***: 
$$\boxed{\text{Buoyancy force on body}=\text{weight of the fluid displaced by body}}$$
## 1.10 Types of Flow 
Different types of aerodynamic flows are categorized and visualized. And in the process of analyzing we attempt to model the flows with less detail and neglect some of the less significant phenomena. 

#### 1.10.1 Continuum flow Versus Free Molecule flow 

***The mean-free path $\lambda$:*** the mean distance that a molecule travels between collisions with neighboring molecules.

If $\lambda$ is orders of magnitude *smaller than the scale of the body* measured by $d$, then the flow appears to the body as a continuous substance. 
In that case, the molecules impact the body surface so frequently that the body cannot distinguish the individual molecular collisions. so the surface feels the fluid as a *continuous medium*. Such flow is called ***Continuum Flow*** 

In the case of $\lambda$ is *on the same order as the body scale*. then the gas molecules are spaced so far apart  that collisions with the body occurs infrequently. Thus the body can feel distinctively each molecular impact. Such flow is called ***Free Molecular Flow***

for the manned flight, the space shuttle encounter **free molecular flow** at  the *extreme outer edge of the atmosphere*.  there are *intermediate cases where flows can exhibit some characteristics of both continuum and free molecular flow*, which is labeled as ***Low-density Flows*** in contrast to continuum flow. 
note that  the low-density flows and free-molecule flow are just a small part of the total spectrum of aerodynamics -> **So we will always treat the fluid as the continuum flow in problems**

#### 1.10.2 Inviscid Flow Versus Viscous flow
when the Molecule move, they obviously transport their mass.  momentum, and energy from one location to another in the fluid. 
![[Some Introductory Thoughts 2023-01-24 11.43.50.excalidraw|250]]
The transport on a molecular scale gives rise to the phenomena of the **mass diffusion, viscosity (friction) and thermal conduction**. This transport phenomena will be discussed in Chapter 15 in detail.

the flows exhibit the effects of the transport area are called ***Viscous Flows***

however, in many practical aerodynamic flows where the influence of transport phenomena is small and we can model it as ***Inviscid flow***. 

[[Important Variables and Numbers]]
Theoretically, the inviscid flow is approached in the limit **as the Reynolds Number goes to infinity.** for practical problems, many problems in high Reynolds number can be assumed to be inviscid.
For the high Reynolds number flow, the influence of the **friction, thermal conduction and diffusion** is limited to the ***Boundary Layer***, and the remainder of the flow outside is essentially Inviscid. so the inviscid theory adequately predicts the pressure distribution and lift to the body. 

---
The flow that are dominated by viscous effects are given examples below:
![[Some Introductory Thoughts 2023-01-24 12.42.38.excalidraw]]
The first fig is a "stalled" airfoil. 
In the case above, the separated flow expands around the front face of the surface and thus form a wake downstream. no inviscid theory can independently predict the aerodynamics of this kind of flow.  --> This is included in the Part 4. 

#### 1.10.3 Incompressible flows Versus Compressible flows
a flow in which the density $\rho$ is constant is called ***incompressible  flow***
and a flow where the density is variable is called ***compressible flow***
-> the detailed discussion will be presented in Chapter 7.

we note there the flow of gas at low Mach number is essentially incompressible. **For $M < 0.3,$ it's always safe to assume $\rho = constant$.** and high-speed flow(near $Mach = 1$ and above) must be treated as compressible. (the incompressible gas are discussed in Chapter 3 to 6 and the compressible flow is discussed in Chapter 7 to 14)

#### 1.10.4 Mach Number Regimes
the Mach number is the most prevalent distinction of different aerodynamic flows. 

we define flow as (not precise definition):
***subsonic*** : $M < 1$
***sonic*** : $M = 1$
***supersonic*** : $M > 1$

##### subsonic flow(M<1 everywhere)
subsonic flows are characterized by *smooth streamlines(no discontinuity in its slope)*. 
note that *the Mach number $M_\infty$ less than 1 doesn't guarantee a totally subsonic flow over the body*.
if $M_\infty$ is close enough to 1, the local Mach  number may become supersonic in certain region  of the flow. 

##### Transonic flow (mixed area of $M<1$ and $M>1$)
In the transonic flow, we can separate the flow of this condition into two different cases:
1. **When the $M_\infty$ is subsonic but is nearly sonic**, the flow can become locally supersonic  -> which would form the pocket of supersonic flow at different places 
2. **When the $M_\infty$ is supersonic**, a bow-shock wave is formed in front of the body, and the weak shock are generated at the trailing edge. 
3. As a **rule of the thumb(not precisely)**, for slender bodies, the transonic flows occur for freestream Mach number in the **range 0.8 to 1.2**. 
![[Some Introductory Thoughts 2023-01-24 14.41.17.excalidraw|650]]

##### supersonic flow(M > 1 everywhere)
the flow properties and the streamlines change discontinuously. Listing of $M_\infty > 1.2$ is strictly  a rule of thumb

##### hypersonic flow 
the shock layer temperature become high enough. and the chemical reactions in the shock layer. The $O_2$ and $N_2$ molecules are torn apart (the gas  molecules dissociate)
an arbitrary but frequently used rule of thumb for it is $M_\infty<5$

#### 1.10.5 The road map of the entire book
for discussing different kinds of flows the aerodynamics of this book are shown in the Fig below:
![[The road map of the book]]

## 1.11 Introduction to Boundary Layers(Viscous flow)
##### (1) Concepts of boundary layers
For the thin region of the flow adjacent to the surface, the velocity gradients are large and the friction plays a Defining role. 
The natural division of the flow into two regions was firstly recognized by the German fluid dynamist *Ludwig Prandtl*. The thin region adjacent to the body surface is called ***boundary layer***

in the boundary layer, the friction and the dissipation is included. **The boundary layer is  the source of the friction drag on an aerodynamic body.** when the flow separates from the surface, resulting a large increase in drag, which is called ***Pressure Drag***

for the purely inviscid flow over the airfoil shape, there is no friction. So the streamline that is right on the surface of the body slips over the surface. due to *friction the infinitesimally thin layer of air molecules adjacent to the body surface against the surface* so it has zero velocity relative to the surface, and it's also the cause of the large velocity gradients -> ***No slip condition*** 
![[Pasted image 20230124162859.png|300]]
the conventional boundary layer analysis assume the *flow conditions at the outer edge of the boundary layer are the same as the flow conditions at the surface flow conditions from an inviscid flow analysis*. 

##### (2) Pressure, Temperature and Sharing stress distribution
It shown practically and theoretically that ==the pressure through the boundary layer==. That is, for the point a and b be the pressure at points a and b, and in the Fig above, we have $p_a = p_b$.  

and we can model the boundary layer as follows: 
the velocity profile and the temperature profile within the boundary layer are as follows
![[Some Introductory Thoughts 2023-01-24 17.01.25.excalidraw|600]]

For a explanation of ***frictional dissipation***, As a fluid element moves along a streamline inside the boundary layer. hence increasing the gas temperature inside the boundary layer, it slows down because of **the original kinetic energy it had before it entered the boundary layer is converted to internal energy due to the friction inside the layer**. which increase the gas temperature inside the boundary layer.

The slope of the velocity profile at the wall is of particular importance **since  it governs the wall shear stress.**  Let $\frac{dv}{dy}$ indicates the gradient of the velocity, Then the shear stress is:
$$\tau_w = \mu \left(\frac{du}{dy}\right)_{y=0}$$
where $\mu$ is the ***absolute viscosity coefficient***. 
However, compare to the [[1. 流体力学相关定义及假设#(1) 动力粘度与运动粘度|problems in the fluid dynamics]], we have to consider the variation of the $\mu$ with the temperature $T$ increase. 
![[Pasted image 20230124175137.png|350]]

##### (3) Aerodynamic Heating
For air at standard sea-level temperature, the absolute viscosity is: 
$$\mu = 1.7894 \times  10^{-5} kg/(m\cdot s) = 3.7373 \times 10^{-7} slug / (ft \cdot s)$$
we let $(\frac{dT}{dy})$ as the *temperature gradient* at the wall

***Aerodynamic heating rate (energy per second per area)*** are defined as 
$$\boxed{\dot{q}_w = -k\left( \frac{dT}{dy}\right)_{y=0}}$$
where $k$ is the ==***Thermal conductivity***== of the gas
and the minus sign connotes that ==heat is conducted from a warn region to a cooler region==, which is the ==opposite to the direction of increasing temperature==. Also, if the heat transfer from the gas into the wall, $\frac{dT}{dy}$ would be positive. and if the heat transfer away from the gas from the wall, $\frac{dT}{dy}$ would be negative. 
The heating and the cooling of a wall by a flow over the wall is called ***==convective heat transfer==***.

***Aerodynamic Heating*** is important in high-speed flows, and it even becomes absolutely dominant in hypersonic flows, and Finally, we note that $k$ in equation above. It's a physical property of the fluid. **And $k$ is also a function of temperature**

For air at sea-level temperature, we have
$$k = 2.53 \times 10^{-2}J/(m\cdot s \cdot K) =3.16 \times 10^{-3} lb/ (s \cdot R\degree)$$
note that thermal conductivity $k$ is essentially proportional to the viscosity coefficient, that is,
$$k = constant \times \mu$$
##### (4) thickness of the boundary layer
![[Some Introductory Thoughts 2023-01-24 18.32.58.excalidraw]]

We introduce the ***Reynolds number*** here. Consider the development of the boundary layer on a surface, and Let $x$ be measured from the leading edge, we have: 
$$Re_x = \frac{\rho_\infty V_\infty x}{\mu}$$
also the local value of the $\tau_w$ and the $\delta$  is function of $Re_x$
##### (5) Viscous flow in boundary Layers
there are two basic types of viscous flow as: 
1. laminar flow
2. Turbulent flow : in this case the streamlines break up and fluid elements moves in a irregular and tortuous fashion.

The profile of the flow with the velocity profiles through the boundary layer is plotted in the Fig below.
![[Some Introductory Thoughts 2023-01-24 19.06.16.excalidraw]]
Consider the velocity gradient at the wall, 
$${(dV/dy)_{y=0} \text{ for laminar flow} < (dV/dy)_{y=0}\text{ for turbulent flow}}$$
Recalling equation $(1.59)$ for $\tau_w$ leads to the fundamental and highly important fact that *laminar shear stress is less than turbulent shear stress*
$$\tau_{w\text{ laminar}} < \tau_{w\text{ turbulent}}$$
