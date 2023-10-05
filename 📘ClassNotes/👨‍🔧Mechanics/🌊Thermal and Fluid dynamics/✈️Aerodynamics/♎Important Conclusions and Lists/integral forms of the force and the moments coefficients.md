the definition of $p_u, p_l, ....$  and relevant parameters can be found in [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 1 Essential Concepts/Chapter1 Introductory Thoughts/1-11 Some Introductory Thoughts#1.5 Aerodynamic Forces and moments|1-11 Some Introductory Thoughts]] 
and we can find the definition of dimensionless force and moment coefficients [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/♎Important Conclusions and Lists/Dimensionless force and moment coefficients|here]] 
we can obtain the integral forms of the dimensionless parameters of the force and the moments coefficients:
$$\boxed{c_n = \frac{1}{c}\left[ \int^{c}_{0} (C_{p,l} -C_{p,u})dx  + \int^{c}_{0}(c_{f,u} \frac{dy_u}{du}  + c_{f,l}\frac{dy_l}{dx})dx \right]} \tag{1.15}$$
$$\boxed {c_a = \frac{1}{c}\left[\int^{c}_{0}(C_{p,u}\frac{dy_u}{dx} - C_{p,l}\frac{dy_l}{dx})dx + \int^{c}_{0}(c_{f,u} + c_{f,l}) dx \right]}\tag{1.16}$$
$$\boxed{c_{m_{LE}} =\frac{1}{c^2}\left[\int^{c}_{0}(C_{p,u}-C_{p,l})x\space dx - \int^{c}_{0}(c_{f,u}\frac{dy_u}{dx} + c_{f,l}\frac{dy_l}{dx}) x \space dx +\int^{c}_{0}(C_{p,u}\frac{dy_u}{dx} + c_{f,u})y_u dx + \int^{c}_{0}(-C_{p,l}\frac{dy_l}{dx} + c_{f,l})y_l dx  \right]} \tag{1.17}$$
`````ad-caution
collapse: open
1. We note here **that $u$ means the uppper surface and $l$ means the  lower surface**.
2. Also the first term in $c_n$ is $C_{p,l}$ while the first term in $c_a$ is $C_{p,u}$, this should be take care of during the integration. 
`````
and the lift and the drag coefficients are obtained as :
$$
\boxed{
\begin{matrix}
c_l = c_n \cos \alpha - c_a \sin \alpha  \\
c_d  = c_n \sin \alpha + c_a \cos \alpha
\end{matrix}
}\tag{1.18}
$$
note that the integral forms of for $c_l$ and $c_d$ can be obtained from substitute the $c_n, c_a$ into the above equation. 
