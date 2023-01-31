## Circulation 
#### (1) Definition
$$\boxed{\Gamma \equiv -\oint_C V \cdot  ds}\tag{2.136}$$
$$\boxed{\Gamma \equiv -\oint_C V\cdot ds = -\iint (\nabla \times V)\cdot dS}$$
#### (2) Derivative relationship
$$\boxed{(\nabla \times  \vec{V}) \cdot \vec{n} = -\frac{d\Gamma}{dS}}$$
## Streamline Function 
#### (1) Streamline equation
$$ds \times V = 0$$
or
$$\boxed{\frac{dx}{u(x,y,z,t)} = \frac{dy}{v(x,y,z,t)} = \frac{dz}{w(x,y,z,t)}}$$
or 
![[Pasted image 20230130180356.png|250]]

#### (2) Quality of Streamline function
for 
$$\bar{\psi} (x,y) =   C$$
we have relationship
1. Cartesian coordinates 
$$\boxed{\Large\begin{matrix}
\frac{\partial \bar{\psi}}{\partial x} = - \rho v  \\
\frac{\partial \bar{\psi}}{\partial y} = \rho u \hfill
\end{matrix}}$$
2. Cylindrical coordinates
$$\Large\boxed{
\begin{matrix}
\frac{\partial \bar{\psi}}{\partial r} = -\rho V_\theta \\ 
\frac{1}{r}\frac{\partial \bar{\psi }}{\partial \theta } = \rho  V_r
\end{matrix}}$$

#### (3) Quality For incompressible flow 
we have $\psi = \frac{\bar{\psi}}{\rho}$
$$\Large\boxed{\begin{matrix}
u = \frac{\partial \psi}{\partial y} \\
v = -\frac{\partial \psi}{\partial x}
\end{matrix}} \qquad \text{and} \qquad 
\boxed{\begin{matrix}
V_r = \frac{1}{r}\frac{\partial \psi}{\partial \theta} \\ 
V_\theta = -\frac{\partial \psi}{\partial r}
\end{matrix}}$$
## Velocity Potential 
#### (1) Definition 
$$\boxed{\Large V = \nabla \phi}$$

#### (2) Cartesian coordinates
$$\boxed{u = \frac{\partial \phi}{\partial x}, v= \frac{\partial \phi}{\partial y}, w= \frac{\partial \phi}{\partial z}}$$

#### (3) Cylindrical coordinates 
$$\boxed{V_r = \frac{\partial \phi}{\partial r} , V_\theta =\frac{1}{r}\frac{\partial \phi}{\partial \theta}, V_z=\frac{\partial \phi}{\partial z}}$$

#### (4) Spherical coordinates
$$\boxed{V_r = \frac{\partial \phi}{\partial r} , V_\theta =\frac{1}{r}\frac{\partial \phi}{\partial \theta}, V_\varphi =\frac{1}{r\sin\theta }\frac{\partial \phi}{\partial \varphi}}$$


