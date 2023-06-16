the optimization of the microchannel is one of the examples that make topology optimization of the Navier-Stokes equation.

the Geometry is as the below,  and the pressure drop of the inlet and the outlet drives the the flow. 

![[Pasted image 20230118164152.png|400]]
the Navier-Stokes equations are presented as follows: 
![[Pasted image 20230118164845.png]]
or in the COMSOL model: s
![[Pasted image 20230118183050.png|300]]
[[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/🌊fluid dynamics/第五章 粘性流体运动基础/五、粘性流体的运行模型建立#3. N-S方程|Compare to the N-S equation of the steady flow]]

compare to the N-S equation, it add the part $\alpha(\gamma) u$, which is a force term where the coefficient 
$$\alpha(\gamma) \equiv \alpha_{max}\frac{q(1-\gamma)}{q+ \gamma}$$

characterizes the flow in a porous medium.
where the $\alpha$ is interpreted as a continuous mapping determined by the function
$$\gamma :\Omega \to[0,1]$$
which in the limit of *decreasing Darcy number and decreasing mesh size* should be a discrete-valued function

the force $\alpha(\gamma) u$ is applied in the model as follows: 
![[Pasted image 20230118182854.png|600]]

for $\lim q\to \infty$, $\frac{\alpha}{\alpha_{max}} = 1 -\gamma$ , and in this case $\gamma$ can be interpreted as the local porosity. 
==and $\gamma = 0$ is  filled and $\gamma =1$ is open channel==

when $\gamma =1$, $\alpha$ is zero -> free flow. 
conversely $\gamma =0$, $\alpha = \alpha_{max}$
$\alpha_{max}$ is defined by the Darcy Number: 
where the Darcy Number is: 
$$Da = \frac{\eta}{\alpha_{max} L^2}$$
The convergence of the optimization process depends on three important factors -> 
1. Darcy number
2. Mesh size 
3. coefficient $q$

the parameter list are list as follow:
![[Pasted image 20230118174506.png|400]]

the dynamic viscosity of the fluid is defined as the spf.mu 

and the $\alpha, \alpha_{max}$ are defined in the comp1 definition
![[Pasted image 20230118174641.png|450]]
also the dtopo1.theta_p is equal to 
$$\frac{q(1-\gamma)}{q+ \gamma}$$
[[microreactor_optimization|microreactor example]]
> note that in the above example,  the $dtopo1. theta_p$ can be found as above in other essays

then the pressure of the inlet is set to $2Pa$ to generate the pressure drop

the optimization object is set as minimize the flow velocity 
![[Pasted image 20230118183303.png]]
where the $u_0$ is the flow velocity scale defined as 0.01m/s
and the u is the x Component of the velocity field
![[Pasted image 20230118183629.png|350]]

and the topology optimization can be set as: 
firstly,  we  use the point sum Object from the optimization node,  and then  the optimization object is set to $\frac{u}{u_0}$ 
![[Pasted image 20230118185037.png|350]]
![[Pasted image 20230118183813.png|300]]
the result is as below:
![[Pasted image 20230118183853.png]]

First, you calculate the solution for the flow in an empty channel (that is, with no porous material)

If the specified convergence criterion is fulfilled, the solution process terminates; otherwise the new design-variable values are used in the next calculation of the flow problem.

However, for design problems such as this one, there is a tradeoff between computation time and convergence. The solution may be sufficiently improved long before convergence is reached in the mathematical sense.

it is useful to limit the number of steps taken by the optimization algorithm after which the solution can be evaluate.

attention that we can decrease the element size of the middle manually in the topology meshing.
![[Pasted image 20230118184742.png|500]]
