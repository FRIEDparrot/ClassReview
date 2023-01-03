
### 1. Introduction of the SIMP and RAMP method
Solid Isotropic microstructures with penalization (SIMP) 
Rational approximation of material properties (RAMP).
![[Pasted image 20230103113547.png|350]]
##### (1) SIMP method
$$k_i = x_i^p k_{i0}$$
thus:
![[Pasted image 20230103113640.png|350]]
and finally we have: 
$$x_i^{k+1} = \left( \frac{2pe_i}{\lambda \nu _i}\right)^{k} \qquad  i = 1,2,... N $$
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



for the form of the RAMP interpolation of the thermal conductive Factors
$$k(\gamma) = \frac{\gamma(C_k(1 + q_k) -1 )}{C_k (1+ q_k \gamma)}$$
where 
$$C_k = \frac{k_f}{k_s}$$
