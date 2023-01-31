### Mass Flux 
$$\boxed{\text{Mass Flux} = \frac{\dot{m}}{A} = \rho  V_n}$$
### Continuum Equations
##### 1. Integral Form
$$\Large\boxed{\frac{\partial }{\partial t}∰_V\rho dV + ∯_S \rho \vec{V} \cdot d\vec{S} = 0}\tag{2.48}$$
##### 2. partial differential equation form
$$\boxed{\Large \frac{\partial \rho}{\partial t} + \nabla \cdot (\rho V) = 0\tag{2.52}}$$
##### 3. For steady flow
$$\Large\boxed{{∯_s} \rho V \cdot dS =  0} \tag{2.53}$$
$$\Large\boxed{\nabla \cdot (\rho V) =0}$$
### Momentum Equation
##### 1. Integral Form 
$$\boxed{∰_V \rho f d\mathcal{V} -∯_S p dS + F_{viscous} =\frac{\partial}{\partial t}∰_V (\rho V)  d\mathcal{V} + ∯_S (\rho V \cdot dS) \space V}$$

##### 2. partial differential equation form 
$$ \boxed{\frac{\partial(\rho u)}{\partial t}   + \nabla \cdot (\rho u V)= - \frac{\partial p}{\partial x} + \rho f_x + (\mathcal{F_x})_{viscous}}$$
$$\boxed{\frac{\partial(\rho v)}{\partial t}   + \nabla \cdot (\rho v V)= - \frac{\partial p}{\partial y} + \rho f_y + (\mathcal{F_y})_{viscous}}$$
$$\boxed{\frac{\partial(\rho w)}{\partial t}   + \nabla \cdot (\rho w V)= - \frac{\partial p}{\partial z} + \rho f_z + (\mathcal{F_z})_{viscous}}$$

##### 3. For steady and inviscid flow 
$$\Large ∯_S(\rho V\cdot dS)V= -∯_S p dS$$
$$\Large\boxed{\begin{matrix}
\nabla \cdot (\rho u V) = -\frac{\partial p}{\partial x} \\
\nabla \cdot (\rho v V) = -\frac{\partial p}{\partial y} \\
\nabla \cdot (\rho w V) = -\frac{\partial p}{\partial z} 
\end{matrix}}$$

### Energy equation
##### 1. Integral Form 
$$
\boxed{
\begin{equation}
\begin{split}
∰_V \dot{q} \rho dV + \dot{Q}_{viscous} -∯_S pV\cdot dS + ∰_V \rho (f\cdot V) d\mathcal{V} + \dot{W}_{viscous}\\
= \frac{\partial }{\partial t}∰ \rho (e + \frac{V^2}{2}) d\mathcal{V} + ∯_S (\rho V\cdot dS ) (e + \frac{V^2}{2})
\end{split}
\end{equation}}
$$
##### 2. partial differential equation form 
$$\boxed{\frac{\partial}{\partial t}\left[\rho (e + \frac{V^2}{2})\right] + \nabla\cdot \left[\rho (e+ \frac{V^2}{2})V\right] = \rho\dot{q} - \nabla \cdot(pV) + \rho (f\cdot V) + \dot{Q}'_{viscous} +\dot{W}'_{viscous}}$$
##### 3. for steady, inviscid, adiabatic flow 
$$
\boxed{∯_S pV \cdot dS + ∯_S \left[ p\space \left(e + \frac{V^2}{2}\right) V \cdot dS\right] =0}\tag{2.97}
$$
$$\boxed{\nabla \cdot \left[ \rho(e+ \frac{V^2}{2}) V\right] = -\nabla\cdot(pV) } \tag{2.98}$$

### Drag Force equation for the Control Volume
Note the control volume should be enclosed by the streamline 
![[1-16 Fundamental Principles 2023-01-29 16.21.44.excalidraw|300]]
$$\Large\boxed{D' = \int^{b}_{h} \rho_2 u_2(u_1 -u_2) dy \overset{\rho_2 \text{ is constant }}{\rightarrow }    \rho_2 \int^{b}_{h} u_2(u_1 -u_2) dy}$$
### Basic Equations in the substantial derivative form 
##### 1. Continuum equation
$$\Large \boxed{\frac{DV}{Dt} + \rho \nabla \cdot V = 0}\tag{2.108}$$

##### 2. Momentum equation

$$\boxed{\rho \frac{Du}{Dt} = -\frac{\partial p}{\partial x} + \rho f_x + \mathcal{F}_{x, viscous}}$$
$$\boxed{\rho \frac{Dv}{Dt} = -\frac{\partial p}{\partial y} + \rho f_y + \mathcal{F}_{y, viscous}}$$
$$\boxed{\rho \frac{Dw}{Dt} = -\frac{\partial p}{\partial z} + \rho f_z + \mathcal{F}_{z, viscous}}$$

##### 3. Energy equation 
$$\Large\boxed{\rho\frac{D(e+ \frac{V^2}{2})}{Dt} =\rho \dot{q} -\nabla\cdot (p V)+ \rho (f\cdot V) + \dot{Q}'_{viscous} +\dot{W}'_{viscous} } \tag{2.108}$$

### Angular Velocity, Vorticity and 
$$\boxed{\omega = \frac{1}{2}\left[(\frac{\partial w}{\partial y} - \frac{\partial v}{\partial z}) i + (\frac{\partial u}{\partial z } - \frac{\partial w}{\partial x}) j +(\frac{\partial v}{\partial x } - \frac{\partial u}{\partial y})k\right]\tag{2.217}}$$
$$\boxed{\xi = (\frac{\partial w}{\partial y} - \frac{\partial v}{\partial z}) i + (\frac{\partial u}{\partial z } - \frac{\partial w}{\partial x}) j +(\frac{\partial v}{\partial x } - \frac{\partial u}{\partial y})k\tag{2.218}}$$
$$\boxed{\xi = \nabla \times V = 2\omega}$$
angular strain :
$$\boxed {\varepsilon_{xy} = \frac{\partial v}{\partial x} + \frac{\partial u}{\partial y}\qquad \varepsilon_{xz} =\frac{\partial u}{\partial z} +\frac{\partial w}{\partial x} \qquad \varepsilon_{yz} = \frac{\partial v}{\partial z}+\frac{\partial w}{\partial y}}$$
