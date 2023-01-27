Central to this chapter  is the derivation and  the discussion of the **three most important and fundamental equations in aerodynamics: the continuity, momentum, and energy equations.**
## 2.1 Introduction and the Road map
The most important thing is the build of the Aerodynamic tools used in the analysis.
![[Screenshot_20230125_194205_com.adobe.reader.png|400]]

## 2.2 Vector Relations 
the aerodynamics is full of the vectors such as force, velocity, etc. These are ***Vector Quantities***
Note that the content of this chapter can be found from the advanced mathematics
#### 2.2.1. Vector algebra
unit vector $\boldsymbol{n}$ is defined by the equation
$$\vec{n} = \frac{\vec{A}}{|A|}$$
the scalar product (dot product) is 
$$\vec{A} \cdot \vec{B} = |A|\cdot |B| cos \theta$$
the vector product (cross product) is 
$$\vec{A} \times \vec{B} = (|A||B| sin \theta)\vec{e}=\vec{G}$$
![[Screenshot_20230125_195414_com.adobe.reader.png|430]]

#### 2.2.2 Typical Orthogonal coordinate systems
We are interested in three common orthogonal coordinate systems:  *cartesian, cylindrical and spherical*. For a Orthogonal coordinate system, three coordinate directions are mutually perpendicular. 

For a cartesian *coordinate system* in the Fig, $i,j,k$ are the unit vectors in $x,y,z$ directions. 
in cartesian coordinate system, the point can  be located by three coordinates $(x,y,z)$ or the position vector $r = xi +yj+zk$

![[1-16 Fundamental Principles 2023-01-26 16.20.21.excalidraw|300]]
For a cylindrical coordinate system, the point $P$ in the space is given by three coordinates $(r,\theta, z)$ and
$$\begin{cases}
x = r\cos \theta \\
y = r \sin \theta \\
z= z
\end{cases}$$
or 
$$\begin{cases}
r = \sqrt{x^2 + y^2} \\
\theta = \arctan \frac{y}{x} \\
z= z
\end{cases}$$
In a spherical coordinate System, the $r$ is the distance of the point from the origin. 
$$\vec{A} = A_r \vec{e}_r + A_\theta \vec{e_\theta} + A_\varphi \vec{e}_\varphi$$
the relationship is: 
$$\begin{cases}
x = r \sin \theta cos\varphi \\
y = r \sin \theta sin\varphi \\
z = r \cos \theta
\end{cases}$$
$$\begin{cases}
r = \sqrt{x^2 + y^2 + z^2} \\
\theta = \arccos \frac{z}{r} \\
\varphi = \arccos \frac{x}{\sqrt{x^2 + y^2}}
\end{cases}$$
#### 2.2.3 Scalar and vector fields
the scalar quantity given as a function of coordinate space and the time $t$ is called a scalar field, for example, 
$$p = p(x,y,z,t)$$
also, a vector  quantity given as a function of coordinate space and time is called the vector field, and are written as :
$$V = V_x i + V_y j + V_z k$$
where
$$\begin{cases}
V_x = V_x(x,y,z,t) \\
V_y = V_y(x,y,z,t) \\
V_z = V_z(x,y,z,t) \\
\end{cases}$$
#### 2.2.4 Vector products

in cartesian coordinate:
$$A\cdot B  = A_x B_x + A_y B_y + A_z B_z$$
$$A\times B =
\left|
\begin{matrix}
\vec{i} & \vec{j} & \vec{k} \\
A_x & A_y & A_z \\
B_x & B_y & B_z
\end{matrix}
\right|$$
In cylindrical coordinates, 
$$\begin{matrix}
A \cdot B = A_r B_r + A_\theta B_\theta + A_z B_z \\
A \times B =
\left|
\begin{matrix}
\vec{\vec{e}_r} & \vec{\vec{e}_\theta} & \vec{\vec{e}_z} \\
A_x & A_y & A_z \\
B_x & B_y & B_z
\end{matrix}
\right|
\end{matrix}$$
for the Spherical coordinates, also Definition is totally similar: 
![[Pasted image 20230126173901.png|400]]

#### 2.2.5 Gradient of a Scalar Field 
the ***gradient*** of a scalar field $p$ is $\nabla p$, *it's a vector*, and it is defined as: 
1. Its magnitude is the maximum rate of change of $p$ per unit length of the coordinate space at the given point.
2. Its direction is that of the maximum rate of the change of $p$ at the given point. 
and some definitions are shown in the figure below.
![[Pasted image 20230126174546.png|300]]
we choose the arbitrary direction $s$ at the point $(x,y)$ and then Let the $\vec{n}$ be a unit vector in the $s$ direction.  
the rate of the change of $p$ per unit length in the s direction can be calculated as: 
$$\frac{dp}{ds} = \nabla p\cdot \vec{n} \tag{2.15}$$
and $\frac{dp}{ds}$ is called the ***directional derivative***. 

![[Pasted image 20230126174659.png|200]]
the expression for the $\nabla p$ in the different coordinate system are given as: 
1. cartesian:
$$p = p(x,y,z)$$
$$\boxed{\nabla p = \frac{\partial p}{\partial x}\vec{i} +\frac{\partial p}{\partial y} \vec{j} + \frac{\partial p}{\partial z}\vec{k}}$$
2. Cylindrical: 
$$p = p(r,\theta ,z)$$
$$\boxed{\nabla p = \frac{\partial p}{\partial r} e_r + \frac{1}{r}\frac{\partial p}{\partial \theta} + \frac{\partial p}{\partial z} e_z}$$
3. Spherical
$$p = p(r, \theta, \varphi)$$
$$\nabla  p = \frac{\partial p}{\partial r}\vec{e_r} + \frac{1}{r}\frac{\partial p}{\partial\theta}\vec{e_\theta} + \frac{1}{r \sin \theta}\frac{\partial p}{\partial \varphi} e_\varphi$$

#### 2.2.6 Divergence of a Vector Field 
Consider a vector field 
$$\vec{V} = \vec{V}(x,y,z) = \vec{V}(r,\theta, z) = \vec{V}(r,\theta,\Phi)$$
and the $\vec{V}$ is a vector that can represent vector quantity. however, for the practical purposes and to aid in physical interpretation. Consider $V$ to be the flow velocity.

In the section 2.3, we will prove the time rate of the change of the volume of a moving liquid element is equal to the ***divergence*** of the $\vec{V}$, which is expressed as
***divergence*** of $\vec{V}$: 
$$\nabla \cdot V$$
note that the divergence of a vector is a scalar quantity. then the divergence in different vectors are shown as: 
1. Cartesian system
$$\boxed{\nabla \cdot V = \frac{\partial V_x}{\partial x} + \frac{\partial V_y}{\partial y} + \frac{\partial V_z}{\partial z}}$$
2. Cylindrical: 
$$\boxed{\nabla \cdot V = \frac{1}{r} \frac{\partial }{\partial r} (rV_r) +\frac{1}{r} \frac{\partial V_\theta}{\partial \theta} + \frac{\partial V_z}{\partial z}}$$
3. Spherical: 
$$\boxed{\nabla \cdot V = \frac{1}{r^2} \frac{\partial}{\partial r}(r^2 V_r) + \frac{1}{r \sin \theta} \frac{\partial}{\partial \theta}( V_\theta sin\theta ) +  \frac{1}{r \sin\theta}\frac{\partial V_\varphi}{\partial \varphi}}$$
#### 2.2.7 Curl of a Vector Field
We consider a vector field:
$$\vec{V} = \vec{V}(x,y,z) = \vec{V}(r,\theta, z) = \vec{V}(r,\theta,\Phi)$$
for $V$ is the velocity of the fluid element, it rotate with an *angular velocity* $\omega$  as it translates along the streamline. And we will prove the $\omega$ is one-half of the curl of $V$ 

Cartesian: 
$$\nabla \times V =
\left|
\begin{matrix}
\vec{i} & \vec{j} & \vec{k} \\
\frac{\partial }{\partial x} & \frac{\partial }{\partial y} & \frac{\partial }{\partial z} \\
V_x & V_y & V_z
\end{matrix}
\right|$$
and for the Cylindrical and Spherical system:
Cylindrical:
![[Pasted image 20230126192350.png|200]]
Spherical:
![[Pasted image 20230126192432.png|250]]

#### 2.2.8 Line Integrals
![[1-16 Fundamental Principles 2023-01-26 19.26.24.excalidraw]]
we define the vector $ds = n ds$, and the ***line Integral along the curve C from point A to B is***
$$\int^{b}_{a} A \cdot ds$$
and if C is closed
$$\oint_{C} A \cdot ds$$
#### 2.2.9 Surface Integrals


