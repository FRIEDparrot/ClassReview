## 3.8 Uniform flow 
Uniform flow is the most basic elementary incompressible flow. We consider **a uniform flow with velocity $V_{\infty}$ oriented in the positive $x$ direction**. 
![[Excalidraw/1-8 Introduction and Concepts 2023-08-20 08.46.08]]
we have :
$$\frac{\partial \phi}{\partial x} = u = V_{\infty} \qquad \frac{\partial \phi}{\partial y} = v = 0$$
then we integral the equation with respect to $x$, we have :
$$\phi = V_{\infty} x + f(y)\qquad \phi = \text{const} + g(x)$$
and we set constant to zero and we can derive : 
$$\Large\boxed{\phi = V_{\infty} x}\tag{3.53}$$
also note that the equation (3.53) <mark style="background: transparent; color: red">implies to both incompressible and compressible flow</mark>
if we use the stream function $\psi$ for calculation, we have $\frac{\partial \psi}{\partial y} = u = V_{\infty}$, and $- \frac{\partial \psi}{\partial x} = v =  0$ so that 
$$\Large \boxed{\psi = V_{\infty} y}\tag{3.55}$$
with this equation, we can also gives the velocity potential and  the stream function in polar coordinates :
$$\Large\begin{cases}
\phi = V_{\infty} r \cos \theta \\
\psi = V_{\infty} r \sin \theta
\end{cases}\tag{3.56}$$
also we refer to the [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 1 Essential Concepts/Chapter2 Fundamental Principles and Equations/9-16 The Application Concepts#2.13 Circulation|Definition of Circulation]] and for an arbitrary closed shape in the flow field (also refer to the [[📘ClassNotes/📐Mathmatics/📈Advanced Mathematics/♎重要公式/格林公式, 高斯公式和斯托克斯公式以及几个重要概念|stokes' equation]]), we have : 
$$ \Gamma = -\oint _C V d\vec{s} = - \iint \left( \nabla \times  V\right) dS = 0\rightarrow\Large \boxed{\Gamma =  0}$$
this can also be derive from that the line integral of $d\vec{s}$ along a closed curve is zero. So we affirm that <u>the circulation around any closed curve in a uniform flow is zero</u>

## 3.9 Source Flow
we consider a flow where all the streamlines are straight lines emanating from a central point O. a source flow is a physically possible incompressible flow, hence $\nabla\cdot V = 0$. 
also, a sink flow is a negative source flow. where by definition the streamlines are directed toward the origin. *The radical flow surrounding the origin is simply being ==induced== **by the presence of discrete source or sink of the origin***.(reference [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 1 Essential Concepts/Chapter3 Fundamentals  of Invisid and Incompressible Flow/Implements/Derivation for the divergence and curl.pdf|Derivation for the divergence and curl.pdf]])
So we recall that **for a source flow**, $\nabla \cdot V = 0$ <b><mark style="background: transparent; color: blue">everywhere except the origin</mark></b>, <b><mark style="background: transparent; color: red">where is infinite (it represents the time rate of change of volume)</mark></b>. `->` this point is called a ***singular point***. also we can <u>interpret this singular point as a discrete source of sink of a given strength</u>. 
![[Excalidraw/8-16 Elementary Flow Patterns 2023-08-21 08.50.33|650]]
When we look more closely at the velocity field induced by a source or sink, if we set the flow from the origin per unit time is $Q$, then we can calculate the velocity in the radius $r$:
$$v_r = \frac{Q}{2 \pi r} = \frac{c}{r}\qquad  v_{\theta} = 0$$
if we consider the line source, we have : 
$$\boxed{\dot{m} = \int_{0}^{2\pi} \rho v_r  r l  d\theta = 2 \pi r l  \rho v_r}$$
we also denote the rate of the volume by $\dot{v}$, then 
$$\boxed{\dot{v} = \frac{\dot{m}}{\rho} = 2 \pi r l  v_r}$$
we denote the **flow rate per unit length per unit depth** by $\Lambda$, then 
$$\Lambda = \frac{\dot{v}}{l} = 2 \pi r v_r \qquad \rightarrow \qquad \boxed{v_r = \frac{\Lambda}{2 \pi r}}$$
then $\Lambda$ is defined as <b><mark style="background: transparent; color: blue">source strength</mark></b>, which is the same definition as $Q$ above. then we can obtain the **velocity potential** : 
$$\Large\boxed{\frac{\partial \phi}{\partial r} = \frac{1}{r}\frac{\partial \psi}{\partial \theta} = V_r  = \frac{\Lambda}{2 \pi r}}\qquad \Large\boxed{\frac{1}{r}\frac{\partial \phi}{\partial \theta} = -\frac{\partial \psi}{\partial r} = V_\theta = 0}\tag{3.63}$$
then we integrate (3.63) and obtain the **velocity potential** and the **stream function** 
$$\phi = \frac{\Lambda}{2 \pi} \ln r +C \qquad  \psi = \frac{\Lambda \theta}{2\pi} + C$$
the constant can be dropped without loss of rigor. Then : 
$$\Large\boxed{\phi = \frac{\Lambda}{2 \pi} \ln r\qquad  \psi = \frac{\Lambda }{2\pi}\theta}$$
so we know at a circle that $r = \text{constant}$, the velocity potential is constant. and on a radical ray from the origin point, the stream function is a constant. 
To evaluate circulation for the 2-dimension source flow, we note that $\nabla \times  V = 0$ everywhere. 
$$\Large\boxed{\Gamma = \iint_{S} (\nabla \times  V) dS = 0}$$
==for any closed curve $C$ chosen in the flow  field==. 

## 3.10 The Combination of a  uniform flow and a source flow
If we only consider a polar system with a source of strength $\Lambda$ located at origin. From the above we can add the stream function together. then the <mark style="background: transparent; color: red">total stream function is</mark>:
$$\Large\boxed{\psi = V_{\infty} r \sin \theta + \frac{\Lambda}{2\pi } \theta}$$
![[Excalidraw/8-16 Elementary Flow Patterns 2023-08-21 20.21.26|600]]
$$\Large\boxed{V_r = \frac{1}{r} \frac{\partial \psi}{\partial \theta} =V_{\infty} \cos \theta + \frac{\Lambda}{2 \pi r}}$$
then we also have that 
$$\Large\boxed{V_{\theta} = - \frac{\partial \psi}{\partial r} = -V_{\infty} \sin \theta}$$
on the location of stagnation point, we have $V = 0$, then $\theta = \pi$: 
$$V_{\infty}  = \frac{\Lambda }{2\pi  r} \rightarrow r = \frac{\Lambda}{2 \pi V_{\infty}}$$
at the stagnation point, we have $\psi  = V_{\infty} \frac{\Lambda}{2 \pi V_{\infty}} \sin \theta + \frac{\Lambda}{2 \pi } \theta = \frac{\Lambda}{2 \pi} (\sin \theta + \theta) \overset{\theta = \pi}{\rightarrow } \frac{\Lambda}{2}$, that is at the stagnation point : 
$$\Large\boxed{\psi  = \frac{\Lambda}{2} = \text{const}}$$
also we give the conclusion that `->` the stagnation line can ==be replaced with a solid body which has the solid part on the right side of the line==. This streamline of the combined flow extends to infinite in the downstream direction. so if we want to form a closed body, we can use a source flow and a sink flow to do this. 

In the picture , the flow from the freestream inside the <u>stagnation streamline</u>, which is called <b><mark style="background: transparent; color: blue">dividing streamline</mark></b>, is emanated from the source, and all the flow outside the streamline is from the freestream. 

also, we note the **flow pattern near a source-sink pair** can be calculated as follows: 
![[Excalidraw/8-16 Elementary Flow Patterns 2023-08-21 20.59.23|450]]
since the distance from the origin point to the source and sink are all $b$, we can derivate the following conclusions from the [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 1 Essential Concepts/Chapter3 Fundamentals  of Invisid and Incompressible Flow/Implements/Derivation for the flow pattern of source-sink pair.pdf|Derivation for the flow pattern of source-sink pair]] : 
1. the total Stream function: 
$$\Large\boxed{\psi = V_{\infty}r \sin \theta - \frac{\Lambda}{2\pi} \left(\theta_1 - \theta_2 \right)}$$
note that *at every streamline, the stream function is constant*. 
2. the distance between the stagnation point and origin point : 
$$\Large\boxed{OA = OB = \sqrt{b^2 - \frac{\Lambda b}{\pi V_{\infty}}}}$$
3. the function of the stagnation streamline : 
$$V_{\infty}r \sin \theta - \frac{\Lambda}{2\pi} \left(\theta_1 - \theta_2 \right) = 0 \qquad \text{or}\qquad \boxed{\Large \psi = 0}$$
note that this function is a function of oval, which is called <b><mark style="background: transparent; color: blue">Rankine Oval</mark></b> 

## 3.11 doublet flow 
A special,  degenerate case of a source-sink pair that leads to a singularity called a **doublet**. Which give the stream function as : 
$$\psi = \frac{\Lambda}{2 \pi} (\theta_1 - \theta_2) = - \frac{\Lambda}{2\pi} \Delta  \theta$$
we note that ==in a doublet, the distance $l$ between two point is infinitesimal compare to the distance== $OP$ (that is, $l \rightarrow  0$).
The strength of a doublet is denoted by $\kappa$, which gives $\kappa = l \Lambda = \text{const}$. so the stream function is defined as :
$$\psi = \lim_{l\rightarrow 0,\kappa = l \Lambda} \left( - \frac{\Lambda}{2 \pi}\right)d\theta\tag{3.85}$$
![[Excalidraw/8-16 Elementary Flow Patterns 2023-08-21 23.32.37|650]]
using the approximation method in the figure above, we have :
$$V_r = \frac{1}{r}\frac{\partial \psi}{\partial \theta} = - \frac{\Lambda}{2\pi}\left( \frac{1}{r} - \frac{1}{b}\right) = \frac{\Lambda}{2 \pi} \frac{l \cos \theta}{r^2} = \frac{\partial \phi}{\partial r}$$
$$\boxed{d\theta = \frac{a}{b} \approx \frac{l \sin \theta}{r}}$$
we use this approximation and we can easily derive **the stream function**: 
$$\Large\boxed{\psi = -\frac{\kappa}{2\pi r} \sin \theta }\tag{3.87}$$
and the **velocity potential** is : 
$$\Large\boxed{\phi = \frac{\kappa}{2 \pi r}\cos \theta}\tag{3.88}$$
`````ad-note
title: streamline function
collapse: close
At any streamline we just need to set $\psi$ as constant. Which gives : 
$$\Large\boxed{\psi =- \frac{\kappa}{2 \pi r} \sin \theta  = c}$$
so we have:
$$\Large\boxed{r = - \frac{\kappa}{2\pi c}\sin \theta}$$
we set the cofficient of it as $d$, and we have : 
$$r = d \sin \theta \qquad  d = - \frac{\kappa}{2 \pi c}$$
we can easily know that this function is a circle with the center located at $(0, \frac{d}{2})$. and so that different streamline with different $c$ forms a family of circles. as shown below : 
![[Excalidraw/8-16 Elementary Flow Patterns 2023-08-22 00.16.51|350]]

secondly, we define the direction of the doublet drawn <b><mark style="background: transparent; color: blue">from sink to source</mark></b>. which is consistent with equation (3.87) and (3.88), and also denote the start direction of the flow. 
`````

we also note in a doublet flow, the source and the sink fall on the top of each other but not extinguish each other because ==the absolute magnitude of the strengths becomes infinitely large in the limit==. so this is an <mark style="background: transparent; color: red">indeterminate form</mark> that  can have a finite value. also in the limit, we have a singularity of strength. 

## 3.12 Nonlifting Flow over a circular cylinder 
We demonstrate that the combination of a uniform flow and a doublet produces  the flow over a circular cylinder, which is one of the most basic geometric shapes available. So we consider the addition of a uniform flow with velocity $V_\infty$ and a doublet of strength $\kappa$. 
![[Excalidraw/8-16 Elementary Flow Patterns 2023-08-22 08.09.06|330]]
we know that the velocity function for a uniform flow and a doublet flow are separately: 
$$\psi_1 = V_{\infty}  r \sin \theta \qquad \psi_2 = -\frac{\kappa}{2 \pi r}\sin \theta$$
and the **stream function** of the combination is : 
$$\psi = V_{\infty}r \sin \theta - \frac{\kappa}{2\pi r}\sin\theta = V_{\infty} r \sin\theta (1- \frac{\kappa}{2 \pi r^2 V_{\infty}})$$
so if we set $R$ here. we know that the stream function is zero only when $r^2 = \frac{\kappa}{2 \pi V_{\infty}}$, so we define : 
$$\Large\boxed{R^2 = \frac{\kappa}{2 \pi V_{\infty}}\quad  \rightarrow \quad  R = \sqrt{\frac{\kappa}{2 \pi V_{\infty}}}}$$
we note that $R$ **is the radius of the stagnation streamline**, which gives :
$$\Large\boxed{\psi = V_{\infty} r \sin \theta \left(1 - \frac{R^2}{r^2} \right)}$$
this is a equation of circle, we have $\psi = 0$ on streamline. <u>For all the points inside the diving streamline</u> we have $\psi < 0$ and <u>outside the streamline</u> $\psi > 0$
then we can also calculate the velocity field as : 
$$\Large\boxed{V_r = \frac{1}{r} \frac{\partial \psi}{\partial \theta} = \left(1 - \frac{R^2}{r^2} \right) V_{\infty} \cos \theta }\tag{3.93}$$
$$\Large\boxed{V_{\theta} = -\frac{\partial \psi}{\partial r} = - V_{\infty} \sin \theta \left(1 +  \frac{R^2}{r^2} \right) }\tag{3.94}$$
the stagnation points can be solved be setting equation $(3.93)$ and $(3.94)$ to zero. 

`````ad-note
title: Analysis for the lifting of the cylinder 
collapse: close
If we replace the dividing streamline by a cylinder, the pressure distibution on the cylinder is Symmetrical about both axis. so this distuibution **causes no lift**. also the pressure disteibution over the front of the cylinder is exactly balanced by the pressure distribution over the back of the cylinder. therefore it **causes no drag**. 

we can also calculate the velocity distribution oevr the cylinder as : 
$$V_r = 0\qquad  V_{\theta} =-2V_{\infty} \sin \theta$$
by the formula of [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Chapter3 Fundamentals  of Invisid and Incompressible Flow/1-7 Introduction, Concepts and basic theorems#3.5 Pressure Coefficient|Pressure Coefficient]] we can calculate the Pressure Coefficient as : 
$$\Large \boxed{C_p = 1 - 4 \sin^2 \theta}\tag{3.101}$$
we also give the distribution of it as :
![[attachments/Screenshot_20230822_093355_com.microsoft.skydrive.png|300]]
referencing the [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/♎Important Conclusions and Lists/integral forms of the force and the moments coefficients|integral forms of the force and the moments coefficients]], we assert $c_{f} = 0$ here, and we get the equation : 
$$c_n = \frac{1}{c} \int_{0}^{c} (C_{p,l} - C_{p,u}) dx\qquad  c_a = \int_{TE}^{LE} (C_{p,u} - C_{p,l}) dy$$
![[Some Introductory Thoughts 2023-01-18 21.36.06.excalidraw|800]]

on a cylinder which is surrounded by nonlifting flow, the distribution of $C_p$ can be found in example 3.13(p261). 
`````

## 3.13 Vortex Flow 
We introduce the vortex flow. The vortex flow is the flow that **all the streamlines are concentric circles about a given point**. we let the velocity **along any given circular streamline be constant, but vary from one streamline to another**. which gives : 
$$V_{r} = 0, \qquad  V_\theta = \frac{\text{const}}{r} = \frac{C}{r}$$
![[Excalidraw/8-16 Elementary Flow Patterns 2023-08-24 08.51.04|250]]
we can easily derive the following conclusions : 
1. firstly vortex flow is a physically incompressible flow, $\nabla \cdot  V = 0$ **at every point**. [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 1 Essential Concepts/Chapter2 Fundamental Principles and Equations/1-8 Fundamental Principles#2.2.6 Divergence of a Vector Field|(referencing calculation of divergence)]] 
2. the vortex flow is an irrotational flow. Which gives $\nabla\times  V = 0$. at **every point except the origin, where the vorticity is infinite**.
[[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 1 Essential Concepts/Chapter2 Fundamental Principles and Equations/1-8 Fundamental Principles#2.2.7 Curl of a Vector Field|(referencing calculation of the curl in cylindrical coordinates)]] 

`````ad-note
title: how to show the curl is not zero at the origin point
collapse: open
for the divergence and the curl at the origin point, we can use a infinitesimal circle and use theorems. 
the calculation can reference the [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Chapter3 Fundamentals  of Invisid and Incompressible Flow/Implements & Examples/Derivation for the divergence and curl for source & vortex flow.pdf|Derivation for the divergence and curl for source & vortex flow.pdf]]
`````
to evaluate the constant $C$, we **take the circulation around a given circular streamline** of radius r : 
$$\Gamma = - \oint_C V\cdot  dS = - V_{\theta} (2 \pi r) \rightarrow C =- \frac{\Gamma}{2 \pi}$$
the $\Gamma$ is called **the strength** of the vortex flow. 
$$\Large\boxed{V_{\theta} = - \frac{\Gamma}{2 \pi r}}\tag{3.105}$$
also, we can easily derive the velocity potential : 
$$\Large\boxed{\phi = - \frac{\Gamma}{2 \pi} \theta}\tag{3.112}$$
and the stream function : 
$$\Large\boxed{\psi = \frac{\Gamma}{2 \pi} \ln r}\tag{3.114}$$
`````ad-note
collapse: close
the origin form of the streamline function all have a constant, that is : 
$$\psi = \frac{\Gamma}{2 \pi} \ln r$$
we set constant is 0 here.
`````
## 3.14 Lifting Flow over a cylinder
### (1) Synthesis of the Flow Pattern
We superimposed  **a uniform flow** and **a doublet** to synthesize the flow over a circular cylinder. In addition, we proved that <mark style="background: transparent; color: red">both lift and drag were zero</mark> for such a flow. 
referencing [[#3.12 Nonlifting Flow over a circular cylinder]], we write the stream function of it as : 
$$\psi = (V_{\infty} r \sin \theta) \left( 1 - \frac{R^2}{r^2}\right)$$
We consider a cylinder **in the uniform wind tunnel** (which is originally no lift and drag) but *spinning about its surface*. It can be formed by adding the nonlifting flow over a cylinder and a vortex of strength $\Gamma$, we recall the [[#3.13 Vortex Flow|stream function of Vortex Flow]], and get the overall stream function as : 
![[Excalidraw/8-16 Elementary Flow Patterns 2023-08-25 12.25.33|750]]
$$\psi_2 = \frac{\Gamma}{2\pi} \ln r + \text{const}$$
Since <b><mark style="background: transparent; color: blue">the constant is arbitrary, then we let it</mark></b> be : 
$$\text{const} = - \frac{\Gamma}{2 \pi}\ln R \rightarrow  \psi_2 = \frac{\Gamma}{2\pi} \ln\frac{r}{R}$$
then we get the stream function by adding the two stream function together :
$$\Large\boxed{\psi = (V_{\infty} r \sin \theta) \left(1 - \frac{R^2}{r^2} \right) + \frac{\Gamma}{2\pi} \ln\frac{r}{R}}\tag{3.118}$$
hence **when $r = R$, the stream function is zero** ($\psi = 0$)
the resulting streamline pattern given by equation (3.118) is shown in the figure above. 
Also note that because a vortex flow of strength $\Gamma$ added to the flow, the circulation about the cylinder is also equal to $\Gamma$.

we can easily derive the velocity of the lifting flow over a cylinder : 
$$\Large\boxed{V_r = V_{\infty} \cos \theta \left(1 - \frac{R^2}{r^2} \right)}$$
$$\Large\boxed{V_{\theta} = -  V_{\infty} \sin \theta \left(1 + \frac{R^2}{r^2} \right) - \frac{\Gamma}{2\pi r }}$$
to locate the stagnation point, we set $V_r = V_\theta = 0$, then 
$$\theta = \arcsin \left(- \frac{\Gamma}{4 \pi V_{\infty} R} \right)\tag{3.123}$$
we know that $\Gamma$ is a positive number, then $\theta$ must be **in the third and fourth quadrants**. Hence **there can be two stagnation points on the bottom half of the circular cylinder**. Note that this result is valid only when $\frac{\Gamma}{4 \pi V_{\infty} R} < 1$. When $\frac{\Gamma}{4 \pi  V_{\infty} R}$, the equation (3.123) has no meaning. 
![[Excalidraw/8-16 Elementary Flow Patterns 2023-08-25 16.15.48|800]]
Also note that when the equation not satisfied, there are two stagnation points, **one of them inside the cylinder and the other outside the cylinder.** 
`````ad-note
title: derivation for the third situation 
collapse: close
we substitute $\theta = \frac{\pi}{2}$ or $\theta = - \frac{\pi}{2}$ into the equation $V_{\theta} = 0$, and calculate the r, we can get 
$$r = \frac{\Gamma}{4 \pi V_{\infty}}  \pm \sqrt{\left( \frac{\Gamma}{4 \pi V_{\infty}}\right)^2 - R^2}$$
which gives the two stagnation points of the cylinder. 
`````

in that case, we know that **the normal force is not zero but the drag is zero** in this situation. 
referencing the [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 1 Essential Concepts/Chapter3 Fundamentals  of Invisid and Incompressible Flow/1-7 Introduction, Concepts and basic theorems#3.5 Pressure Coefficient|Pressure Coefficient]] we can derive the **pressure coefficient of the picture above on the surface** as (referencing [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 1 Essential Concepts/Chapter3 Fundamentals  of Invisid and Incompressible Flow/Implements/Pressure distribution of the cylinder under lift force.pdf|Pressure distribution of the cylinder under lift force.pdf]]): 
$$\Large\boxed{C_p = 1- \left[ 2 \sin \theta  + \frac{\Gamma}{2\pi R V_{\infty}}\right]^2\tag{3.126}}$$
### (2) Calculation of the drag force
the drag coefficient is given by [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/♎Important Conclusions and Lists/integral forms of the force and the moments coefficients|integral forms of the force and the moments coefficients]] as follows, note that for inviscid flow, we have $c_f = 0$, then 
$$c_d = c_a = \frac{1}{c} \int_{LE}^{TE} \left( C_{p,u} - C_{l,u}\right) dy= \frac{1}{c}\left[\int_{0}^{c} \left(C_{p,u} \frac{dy_u}{dx} - C_{p,l} \frac{dy_l}{dx} \right) \right]$$
for the cylinder above, we have $c = 2R$, also since $y = R \sin \theta$, $dy = R \cos \theta d\theta$, then $C_p = \text{constant}$ in both top surface and bottom.
$$c_d = \frac{1}{2} \left[ \int_{\pi}^{0} C_{p,u} R \cos \theta d\theta - \int_{\pi}^{2\pi} C_{p,l} R \cos \theta d\theta\right] = - \frac{1}{2} \int_{0}^{2\pi} C_p \cos \theta d\theta = 0$$
`````ad-caution 
collapse: open
the integration limit is from $0$ to $\pi$ and $\pi$ to $2\pi$, which is intergal from the leading edge to trailing edge on the top surface and bottom surface. 
`````

### (3) Calculation of the lifting force 
since the lifting force can be calculated referencing [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/♎Important Conclusions and Lists/integral forms of the force and the moments coefficients|equation 1.15]], also $c_f = 0, dx = -R\sin \theta d\theta$, we have 
$$c_n  = \frac{1}{c} \left[\int_{0}^{c} (C_{p,l}  - C_{p,u}) dx \right] = \frac{1}{2R} \left[ - \int_{\pi}^{2\pi}C_{p,l} R\sin \theta d\theta  + \int_{\pi}^{0} C_{p,u} R \sin \theta d\theta\right]$$
that is:
$$\Large\boxed{c_n = -\frac{1}{2}\int_{0}^{2\pi}  C_p \sin\theta d\theta}$$
then substitute equation (3.126) into it, we can obtain the lifting coefficient : 
$$\Large\boxed{c_l  = \frac{\Gamma}{ R V_{\infty} }}$$

from [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/♎Important Conclusions and Lists/Dimensionless force and moment coefficients|Dimensionless force and moment coefficients]] we know : 
$$L' = C_L q_{\infty} S = \frac{1}{2}C_L \rho V^2_{\infty} S$$
then since the platform area $S = 2 R \times 1$, then we obtain the <b><mark style="background: transparent; color: blue">expression of lifting force per unit span</mark></b> : 
$$\Large\boxed{L' = \rho \Gamma V_{\infty}}\tag{3.140}$$
We note after obtaining the lifting force that a rapid spinning cylinder can <mark style="background: transparent; color: red">produce a much higher lift than an airplane wing of the same platform area. At the same time the drag is also much higher than wing</mark>. We replace the sail on a boat with rotating circular, it produce propulsion for the boat. 

**The Magnus Effect** declare that the <u>generation of aerodynamic force perpendicular to the body's angular velocity  vector</u> is due to the nonsymmetric flows about the spinning bodies, which is simply explained in the figure below. The Magnus effect has an important influence on the performance of spinning missiles, and modern high-speed aerodynamic research has focused on the Magnus forces too. 
![[Excalidraw/8-16 Elementary Flow Patterns 2023-08-26 14.56.11|300]]

## 3.15 Kutta-Joukowski Theorem 
We acclaim that the result in equation [[#(3) Calculation of the lifting force|(3.140)]] is derived from a circular cylinder, the equation <b><mark style="background: transparent; color: blue">applies to any cylindrical bodies of arbitrary cross section. </mark></b>
we let the curve $A$ be any curve in the flow enclosing the airfoil. the circulation around the airfoil is given as : 
$$\Gamma \equiv \oint_{A} V \cdot dS$$
In turn, the lift per unit span $L'$ on the airfoil is given by **Kutta-Joukowski Theorem**. Which is 
$$\Large\boxed{L'  = \rho_{\infty} V_{\infty}\Gamma}$$
This theorem is derived by complex variables. It also shows that arbitrary functions of complex variables are general solutions of Laplace's equation. Which <u>governs incompressible potential flow.</u> 

The lifting flow in the figure below is synthesized by **superposing a uniform flow, a doublet and a vortex**. 
![[Excalidraw/8-16 Elementary Flow Patterns 2023-08-26 21.07.06]]
Firstly, the **circulation theory of lift** in aerodynamics is developed based on the analysis of aerodynamic force, which is <u>formed by the pressure and shear stress on a body</u>. So the imbalance of pressure and shear stress are also the source of the lift. While calculate the circulation around a body is much easier than integration of the surface pressure. 

## 3.16 The Numeric source panel method 
We specify the shape of an arbitrary body and consider a *numerical appropriate method for solution*. This technique is called the source panel method. 
we define $\lambda = \lambda(s)$ is the **source strength per unit length**. The <mark style="background: transparent; color: red">strength of an infinitesimal portion </mark> can be treated as a **distinct source  of strength** $\lambda ds$. 
![[Excalidraw/8-16 Elementary Flow Patterns 2023-08-26 21.50.44|700]]
We consider  the point  P in the flow, which located a distance $r$ from $ds$, the ==small section of the source sheet of strength $\lambda ds$ induces an infinitesimally small potential $d\varphi$ into at point P==. So we consider the integral of the potential from all the distinct source. at point $P$, is given as : 
$$\boxed{d \phi = \frac{\lambda ds}{2\pi} \ln r} \qquad \qquad \phi(x,y) = \int_{a}^{b} \frac{\lambda ds}{2\pi} \ln r \tag{3.141}$$
In general, the $\lambda(s)$ can change from positive to negative along the sheet. <mark style="background: transparent; color: red">The "source sheet" is a combination of line sources and line sinks.</mark> So the numerical problem becomes finding the appropriate $\lambda(s)$. The solution carried out as follows :

We let the source strength $\lambda$ per unit length <u>be constant over a given panel</u>, but allow it vary from one panel to the next. Hence the source strength is set to $\lambda_j \quad (j = 1,2,... n)$, The boundary condition is imposed by defining the normal component of the flow velocity is zero at each control point. 

then the velocity potential is calculated as : 
$$\Delta \phi_j = \frac{\lambda_j}{2 \pi} \int_j \ln r_{pj} ds_j\tag{3.143}$$
and
$$\Large\boxed{\phi(P) = \sum^{n}_{i=1} \Delta \phi_j = \sum^{n}_{j=1} \frac{\lambda_j}{2\pi} \int_{j} \ln r_{pj} ds_j}\tag{3.146}$$
note that in this equation, the distance $r_{pj}$ is the distance from Point $P$ to the $j^{th}$ panel's midpoint or we have the equation: 
$$r_{pj} = \sqrt{(x - x_j)^2 + (y - y_j)^2}$$
![[attachments/Screenshot_20230826_225434_com.microsoft.skydrive.png|750]]
the component $V_\infty$ normal to the $i^{th}$ panel is :
$$\Large\boxed{V_{\infty, n}  = V_{\infty}\cdot n_i = V_{\infty} \cos \beta_i}$$
the $\beta_i$ is the angle between the freestream direction $V_{\infty}$ and normal vector $n_j$. As shown above. We note that we only need to sum all the velocity generated by other discrete potential source. Which is given by 
$$\Large\boxed{V_n = \frac{\partial }{\partial n_i}\left[ \phi(x_i,y_i) \right]}$$
while when $j = i$, the contribution to the derivation is is simply $\frac{\lambda_i}{2}$, then the normal velocity can be obtained by : 
$$V_n^i = \frac{\lambda_i}{2} + \sum^{n}_{j=1(j\neq i)} \frac{\lambda_j}{2\pi}\int_j \frac{\partial }{\partial n_j}\left( \ln r_{ij}\right) ds_j$$
If the source panels forms a <mark style="background: transparent; color: red">solid body</mark> the <mark style="background: transparent; color: red">boundary condition</mark> gives : 
$$V_{\infty, n}^i + V_n^i = \frac{\lambda_i}{2} + \sum^{n}_{j=1(j\neq i)} \frac{\lambda_j}{2\pi}\int_j \frac{\partial }{\partial n_j}\left( \ln r_{ij}\right) ds_j + V_{\infty} \cos \beta_i = 0$$
we let $I_{i,j}$ be the value of integral when control point is on the $i^{th}$ panel and integral is over the $j^{th}$ panel, then the equation is : 
$$\Large\boxed{\frac{\lambda_i}{2} + \sum^{n}_{j=1} \frac{\lambda_j}{2\pi} I_{i,j} + V_{\infty} \cos \beta = 0}\tag{3.153}$$
this is *linear algebraic* equations can be solved using numerical methods. It <mark style="background: transparent; color: red">gives the boundary condition of airfoil flow </mark>

the *velocity tangent to the surface* at each control point can be calculated as : 
$$\Large\boxed{V_i = V_{\infty} \sin \beta_i}\tag{3.154}$$
then we can obtain the <u>tangential velocity</u> $V_s$ at the control point of the $i^{th}$ panel induced by all the panels as (note that the differentiating term corresponding to $j = i$ is equal to zero): 
$$V_s = \frac{\partial \phi}{\partial s} = \sum^{n}_{j=1} \frac{\lambda_j}{2\pi}\int_j \frac{\partial }{\partial s}(\ln r_{ij})ds_{j}$$
So we know the <b><mark style="background: transparent; color: blue">total surface velocity</mark></b> at $i^{th}$ control point $V_i$ 
$$\Large\boxed{V_i  =V_{\infty, s} + V_s = V_{\infty} \sin\beta_i  + \sum^{n}_{j=1} \frac{\lambda_j}{2\pi  }\int_j \frac{\partial }{\partial s}(\ln r_{ij})ds_j }$$
then we can <mark style="background: transparent; color: red">obtain the surface pressure by the surface velocity</mark>: 
$$C_{p,i} = 1 - \left( \frac{V_i}{V_{\infty}}\right)^2$$
then we can calculate the pressure distribution of a body of arbitrary shape. 
`````ad-note
title: Accuracy Test
collapse: close
The accuracy of the result can be tested `->` for a close body, the sum of all the source and sink strength must be zero. Or else the  body can absorb or add mass from the flow. That is : 
$$\Large\boxed{\sum^{n}_{j=1} \lambda_{j} S_j = 0}$$
`````
