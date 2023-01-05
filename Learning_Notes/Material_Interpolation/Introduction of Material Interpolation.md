
### 1. Introduction of the SIMP and RAMP method
Solid Isotropic microstructures with penalization (SIMP) 
Rational approximation of material properties (RAMP).
![[Pasted image 20230103113547.png|350]]
and when an optimization analysis performed, it can be written as 
$$x_i^{k+1} = \left( \frac{f(x_i)}{\lambda H_i R(x_i)}\right)^{(k)} \qquad i = 1,2,...N  \tag{11}$$
or  considering the upper and lower limits: 
$$x_i^{(k+1)} = \begin{cases}
\alpha\left( \frac{f(x_i)}{\lambda H_i R(x_i)}\right)^{(k)} + (1-\alpha)x_j^{(k)}  \\
x_{min} \qquad  x_j \leq x_{min}  \\
x_{max} \qquad  x_j \geq x_{max}  
\end{cases}\qquad i = 1,2,...N  \tag{12} $$
##### (1) SIMP method
$$k_i = x_i^p k_{i0}$$
thus:
![[Pasted image 20230103113640.png|350]]
and finally we have: 
$$x_i^{k+1} = \left( \frac{2pe_i}{\lambda \nu _i}\right)^{k} \qquad  i = 1,2,... N $$
lambda can be obtained by a bisectional algorithm
also,considering the upper and the lower limits of the design variable $x_i$and the optimization convergence,
$$x_i^{(k+1)} = \begin{cases}
\alpha (\frac{2pe_i}{\lambda \nu_i})^{(k)} + (1-\alpha) x_j^{(k)} ,  \qquad  x_{min} < x_i < x_{max} \\
x_{min} ,  \qquad  x_j \leq x_{min} \\
x_{max}, \qquad    x_j \geq x_{max}
\end{cases}$$
##### (2) RAMP method 
1. implement : RAMP function(slope function)
![[Introduction of Material Interpolation 2023-01-03 09.12.18.excalidraw]]

2. RAMP interpolation model
According to the variable density model, the RAMP interpolation model is defined as :
$$k_i = \frac{x_i}{1 + q(1-x_i)} k_{i0}$$
where $q$ is the weight coefficient off the RAMP  interpolation model 
hence the weight coefficient of the RAMP interpolation model is defined as :

$$f(x_i) = c_i = \frac{x_i}{1+ q(1-x_i)} u_i^T k_{i0} u_i$$
$$\frac{\partial C}{\partial x_i} = -U^T\frac{\partial K}{\partial x_i}U = \frac{x_i}{[1+ q(1-x_i)]^2} u_i^T k_{i0} u_i $$
and if we substitute it into the equation (11), and the iteration formula is: 
$$x_i^{(k+1)} = \left(\frac{2(1+q)e_i}{\lambda \nu_i(1 + q(1-x_i))} \right)^{(k)} $$
*iterative criterion of improved guide-weight method*


$$R(x) = -\frac{q}{1 + q} x_i  + 1$$
for the form of the RAMP interpolation of the thermal conductive Factors
$$k(\gamma) = \frac{\gamma(C_k(1 + q_k) -1 )}{C_k (1+ q_k \gamma)}$$
where 
$$C_k = \frac{k_f}{k_s}$$
