The Tesla Microvalve is that: 
fluid can flow freely in one direction but not in the reverse direction

also Typically the Reynolds Number of the flow in the microfluidics is between 1 and 100

The model solves two instances of the Navier-Stokes equations, one for the forward flow and one for the reverse

## Solution
---
#### Basic Parameters
the properties of the materials are listed in the empty material as follows:
![[Pasted image 20230119182653.png|400]]

the parameters are set as follows:
![[Pasted image 20230119171310.png|450]]
note that the $D,L$ and $H$ is the geometric parameter, so the parameters that matters are: 
$Re = 100$
$\mu_0 = 10^-3(Pa/s)$
$\rho_0 = 1000(kg/m^3)$
$U_{in} = Re \frac{\mu_0}{\rho_0 D}$
![[Pasted image 20230119171400.png|250]]
where $L$ is the  length between the inlet and the outlet -> so 
$$p_{forward} = \frac{\int_{inlet}pdS-\int_{outlet}pdS}{L}$$
For the reverse flow the same expression is used, except the inlet and outlet correspond to different boundaries in the model

the ratio of the pressure drop between the reverse and forward flow is defined as 
$$D_i = \frac{\Delta p_{\text{backward}}}{\Delta p_\text{forward}}$$
but since the expression not form a well posed objective function, so an alternative expression is required for the optimization problem.

the ==energy dissipation is a well-posed objective function== for topological optimization
$$obj = \int_\Omega (\tau : S + \alpha (u\cdot u))dV$$
$$S = \frac{1}{2} (\nabla u  + (\nabla u)^T)$$
#### N-S equation
and the N-S equation is still written as: 
![[Pasted image 20230119172416.png|300]]
[[Minimizing the flow velocity of the microchannel]]
where $\theta = \gamma$ is the material volume factor, 
and also the $\alpha(\theta)$ is given by:
$$\alpha(\theta_p) = \alpha_0 \theta_p, \qquad \theta_p = \alpha_0 \frac{q + \theta}{q + \theta }$$
![[Pasted image 20230119172957.png|300]]
where $\alpha_0$ is also the $\alpha_{max}$ in the essay [[Minimizing the flow velocity of the microchannel]]
where $\theta_c$ and $\theta_f$, $\theta$ and $\theta_p$ are the control- and ==filtered material volume factor==.
the volume force is defined as following: 
![[Pasted image 20230119201153.png|500]]
also in the definition of the component1
note that from the essay above, we can get the calculation method of the $\alpha$
$$\alpha = \frac{\mu}{Da \cdot  L^2} \cdot dtotpo.p$$
where $Da = \frac{1}{16}$ in this example.

#### Manual Parameter Definitions
In the flow 1, the parameter are as follows:
![[Pasted image 20230119193440.png|500]]
and In the flow2, we just define the Di is $\frac{dp_{forward}}{dp_{backward}}$ (that is used  in the global calculation in the post processing)
![[Pasted image 20230119193302.png|500]]
where the $spf.Qvd$  means the viscous dissipation
and the **optimization target  is to maximize the energy dissipation $obj$**

We add the Optimization module(Density Module) in the Density module 1
**the viscous and ==friction dissipation which can be integrated over the domain to obtain a suitable
objective function==.**

where 
$$Obj = \frac{\int_{\Omega} \phi_{forward}}{\int_{\Omega} \phi_{backward}}$$
and 
$$\phi_{forward} = spf.Qvd + \alpha \cdot (u^2 + v^2), \qquad \phi_{backward} = spf2.Qvd + \alpha \cdot (u^2 + v^2)$$
if we want to get the $dp_{forward}$ and $dp_{backward}$,  The solution should be update in the study1.

The filter radius should not be smaller than the mesh element size, so the default will work, but a fixed value has to be chosen to make the result of the optimization mesh independent.

note the p and p2 can be found their definitions in the node laminar flow as : 
![[Pasted image 20230119202740.png|300]]
![[Pasted image 20230119202835.png|200]]

#### Optimization Settings
we add a density topology feature $\theta$, which can be used to distinguish between free flow and
solid regions.

the Interpolation model in the optimization node is: 
![[Pasted image 20230119203403.png|300]]
$q_{darcy} \text{ and } \theta_0$ is  all set to 1 (free flow as  the initial condition)

also at the optimization node under the study1, we have the following settings: 

for the density-model optimization, In the topology-optimization node, we use the MMA method(SNOPT method is also acceptable) for the topology optimization process.  and the objective function is set to $comp1.obj$ --> maximize this function 
![[Pasted image 20230119204213.png|300]]

***note that we should add the fully coupled node in the Solver configuration node***
Optimization>Solver Configurations>Solution 1 (sol1)>Optimization Solver 1>Stationary 1 node, then click Fully Coupled 1
then the topology parameter $\theta$ would be coupled back to the fluid flow node automatically
![[Pasted image 20230119205007.png|300]]
then the fully coupled node can be set as: 
![[Pasted image 20230119205310.png|400]]

#### Implements
---
**Note that To avoid the effect of grayscale the filtered field is projected to construct the material volume factor**, θ, which is related to the damping term using a convex function

The triangular shaped obstacle has a flat edge normal to the incident fluid, which means the velocity is redirected upward and downward toward the exterior walls.

the boundary condition in this example are all fully developed flow 
note In the Results toolbar, click ==More Datasets and choose Mirror 2D==.
can significantly reduce the calculation.
![[Pasted image 20230119205921.png|300]]
for the two data, the forward flow and the backward flow are the $spf.U$ and the $spf2.U$ sepreately.

also it just optimize the result in the condition of the forward flow