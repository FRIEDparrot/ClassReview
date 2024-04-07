What we study the Numerical methods for ?
1. *for solution of large-systems of equations nonlinearities and complicated geometries  that are not uncommon in Engineering and Science*
2. *for developing package with numerical methods, or canned programs*
3. *for better understanding of mathematics...*

### 1.1 Definition of a mathematical model
a  *mathematical model can be broadly defined as a formulation or equation that express the essential features of a system or proceed in mathematical terms.*
$$\text{Dependent variable} = f \left( \text{independent variables} \cdot \text{parameter }\cdot \text{forcing functions} \right)$$
`````ad-example
title: Euler's Method
collapse: open

for a jumper from the high place to the lower place,

we have the equation 
$$\frac{dv}{dt} = g - \frac{c_d}{m}v^2$$

the analytic solution of the problem is
$$v(t) = \sqrt{\frac{gm}{c_d}} \tanh\left( \sqrt{\frac{gc_d}{m}} t\right)$$

for a numerical analysis, if we have $\frac{dv}{dt}$
then a approximation can be calculated by the ***Euler's method*** as:

$$\frac{v(t_{i+1}) - v(t_i)}{t_{i+1} - t_i} = g -\frac{c_d}{m}v(t_i)^2$$
or
$$v_{i+1} = v_i +\frac{dv}{dt}\Delta t$$

note that in this method, each halving of the step size to attain more accuracy leads to doubling of the number of the computations
`````

### 1.2 Summary of the  Numerical Methods in the entire book 

![[Screenshot_20230228_040851_com.adobe.reader.png|550]]

#### (a) Roots and optimization 
Roots: $f(x) =0$
Optimization: 
Solve for $x$ so that $f'(x) = 0$
![[Chapter1 Mathmatic Modeling, Numerical Methods and Problem Solving 2023-02-28 04.15.49]]

#### (b) Linear algebraic equations 
$$\begin{cases}
a_{11} x_1 +a_{12} x_2 = b_1 \\
a_{21} x_1 +a_{22} x_2 = b_2
\end{cases}$$
#### (c) Curve fitting 
this part include Regression and Interpolation 

#### (d) Integration and differentiation 

![[Chapter1 Mathematic Modeling, Numerical Methods and Problem Solving 2023-02-28 04.26.41|200]]

#### (e) Differential equations
$$\frac{dy}{dx} \approx \frac{\Delta y}{\Delta t} = f(t,y)$$
solve for $y$ as function of $t$

