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
$$\boxed{N' = \int^{TE}_{LE} (-p_u ds_u cos\theta - \tau_u ds_u sin\theta)ds_u + \int^{TE}_{LE} (p_l ds_l cos\theta - \tau_l ds_l sin\theta) ds_u} $$
$$\boxed{A' = \int^{TE}_{LE} (-p_u ds_u sin\theta + \tau_u ds_u cos\theta)ds_u  + \int^{TE}_{LE} (p_l ds_l sin\theta + \tau_l ds_l cos\theta) ds_u}$$
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
also note that the C in the uppercase denotes the 3-D body force/moment 

it's conventional to denote the force/moment per unit span by lowercase c: 
$$c_l \equiv \frac{L'}{q_\infty c} \quad c_d \equiv \frac{D'}{q_\infty c} \quad c_m \equiv \frac{M}{q_\infty c^2}$$
where $S = c(1) =c$
also, two additional dimensionless quantities of immediate use are:

Pressure coefficient: 
$$C_p \equiv \frac{p-p_\infty}{q_\infty}$$
Skin friction coefficient:
$$c_f \equiv \frac{\tau}{q_\infty}$$

So In the application, we often use the dimensionless form of the coefficients introduced above. 

We have the following geometry relationships: 
$$\begin{matrix}
dx = ds \cos\theta \\
dy = -(ds \sin\theta) \\
S=  c(1)
\end{matrix}$$
![[Some Introductory Thoughts 2023-01-19 00.50.37.excalidraw]]
