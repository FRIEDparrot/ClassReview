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
[[Some Introductory Thoughts#Aerodynamic Force Equations|The Force Equations]]
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
consider an airplane in the steady, level flight, and as shown in Fig. For this case, we have the equilibrium relationship as: 
![[Some Introductory Thoughts 2023-01-22 21.41.13.excalidraw|300]]
when the plane is sustain in level flight
$$L = W$$
and for steady(unaccelerated) flight:
$$D = T$$
typically, for conventional cruising flight, $L/D\approx$ 15 to 20

For an airplane of given shape, at given march and Reynolds number, the actual values of $C_L$ and $C_D$ for a given body shape just depend on the Orientation of the body --> which determines the attack angle. The generic variations for $C_L$ and $C_D$ versus $\alpha$ is shown in fig
![[Some Introductory Thoughts 2023-01-22 22.04.32.excalidraw|250]]

the $C_L$ increase linearly with the attack angle $\alpha$ until the wing ***stalls***

the lowest possible velocity at which the airplane can maintain steady level flight is *stalling velocity*, $V_{stall}$, which is dictated by the value of $C_{max}$ by the [[lowest velocity|link]]

from the definition of the lift coefficient, considering $L = W,$ we have: 
$$C_L = \frac{L}{q_\infty S} =  \frac{2W}{\rho_\infty V^2_\infty S}$$
thus we have
$$\boxed{V_\infty = \sqrt{\frac{2W}{\rho_\infty C_L S}}}$$
since the $\rho_\infty, W$ and $S$ are fixed values for a given airplane at a given attitude, the $V_\infty$ corresponds to the specific value of $C_L$, thus when the $C_L$ reaches it's maximum, $V_\infty$ reaches the minimum. Then we have
$$\boxed{V_{stall} = \sqrt{\frac{2W}{\rho_\infty SC_{L,max}}}}$$
and note that $C_L$ is purely defined by nature at an given attitude. Through the physical laws for the aerodynamic flow-field over the plane, the airplane designer has some devices available that can artificially increase the $C_{L,max}$ ---> these are called high-lift devices(flaps, slat and slots, etc.). 
They are used for decreasing the $V_{stall}$ and thus deploy for landing and take-off.

---
also note that the maximum velocity for a given airplane with given maximum trust can be found by $T=D$, then we have 
$$C_D =\frac{D}{q_\infty S} = \frac{T}{q_\infty S} = \frac{2T}{\rho_\infty V_\infty^2 S}$$
also we have
$$\boxed{V_\infty = \sqrt{\frac{2T}{\rho_\infty C_D S}}}$$
thus when $C_D$ reaches its minimum, the $V_\infty$ reaches
$$\boxed{V_{\infty,max} = \sqrt{\frac{2T_{max}}{\rho_\infty C_{D,min} S}}}$$
![[Some Introductory Thoughts 2023-01-22 23.20.52.excalidraw|400]]

