We know that the aerodynamic coefficients for an **airfoil (whole aircraft)** and for the wing has grate difference, but it must have relationship between the aerodynamic properties of an airfoil and finite wings. Then the single and most important purpose of the chapter is focused on the sigle finite wings. 

## 5.1 Downwash and Induced Drag 
We consider the wing below, the flow near the wing tips <u>tends to curl around the tips</u>, which is forced by the high-pressure region underneath and the low-pressure region on the top, which cause the streamline over the bottom surface <b><mark style="background: transparent; color: blue">bend from root to tip, and the streamline over the top surface bend from tip to root</mark></b>. 
![[Excalidraw/1-8 Introduction and fundamental Concepts 2023-09-18 19.20.54|1000]]
The tendency for the flow to **leak around the wing tips** has another important effect on the aerodynamics of the wing, which is a circulatory motion that <u>trails downstream the of the wing</u>; also the tip vortices are drawn in the right figure, which induce a small velocity component in the downward direction of the wing, the downward component is called <mark style="background: transparent; color: red">downdash</mark>. 

We gives the concept of three types of angle of attack in the figure below. The angle <mark style="background: transparent; color: orange">between the chord line and the direction</mark> of $V_{\infty}$ is angle of  $\alpha$, which is called <mark style="background: transparent; color: red">geometric angle of attack</mark>, and also, the local relative wind is inclined below the direction of $V_{\infty}$ by angle $\alpha$, called <mark style="background: transparent; color: red">induced angle of attack</mark>. Hence the presence of the downwash, whose <u>effect on inclining the local relative wind in the downward direction</u>. The angle of attack actually seen by the local airfoil section is the angle between the chord line and local relative wind, which is called <mark style="background: transparent; color: red">effective angle of attack</mark>. 
![[Excalidraw/1-8 Introduction and fundamental Concepts 2023-09-18 20.15.19|650]]
Appearantly we have the relationship of effect angle of attack :
$$\alpha_{eff} = \alpha - \alpha_{i}$$
Secondly, the local lift vector is aligned perpendicular to the local relative wind. We see that the local lift vector in the direction of $V_{\infty}$, and there is a <mark style="background: transparent; color: red">drag created by the presence of downwash</mark>, which is defined as <b><mark style="background: transparent; color: blue">induced drag</mark></b>, which is shown in figure as $D_i$. 

The tilting backward of the lift vector shown in the figure above is one way of visualizing the **physical generation of induced drag**. 

We note that **the wing-tip vortices alters the pressure distribution on the finite wing** in such a fashion that a net pressure imbalance exists in the direction of $V_{\infty}$, then the induced drag is a type of <mark style="background: transparent; color: red">"pressure drag"</mark>. Also, the wing-tip vortices contains large amount of translational and rotational kinetic energy. so that the <u>extra power provided by the engine that goes into the vortices is the extra power required from the engine to overcome the induced drag</u>. 

We note that the <b><mark style="background: transparent; color: blue">total drag coefficient</mark></b> on a subsonic finite wingin real life is <mark style="background: transparent; color: red">the sum of the induced drag</mark> $D_i$, <mark style="background: transparent; color: red">the skin friction drag</mark> $D_f$, <mark style="background: transparent; color: red">and the pressure drag due to flow separation</mark> $D_p$ (3 parts), hence we define the <u>profile drag coefficient</u> as $c_d$, and we also gives the <u>induced drag coefficient</u> $C_{D,i}$: 
$$c_d = \frac{D_f  + D_p}{q_{\infty}S}\qquad C_{D,i} = \frac{D_{i}}{q_{\infty} S}$$
then the <u>total drag coefficient</u> is : 
$$\Large \boxed {C_{D} = c_{d} + C_{D, i}}$$
## 5.2 Biot-Savart Law & Helmholtz's Theorem 
We review the [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 2 Equations for Incompressible Flow/Chapter4 Incompressible flow/1-8 Introduction and overall  Analysis#4.4 The Vortex Sheet|Concept of Vortex Sheet]], and expand the concept of a vortex filament. we discuss the straight vortex filament extending to $\pm \infty$. This concept is extended below.
![[Excalidraw/1-8 Introduction and fundamental Concepts 2023-09-18 21.48.07|650]]
In general, a vortex filament <u>can be curved</u>, note only a portion of the filament is illustrated in the figure. This filament induces a flow field in the surrounding space. And <u>If the circulation is taken about any path enclosing the filament, we can reach a constant value</u>. We consider a directed <mark style="background: transparent; color: red">segment of the filament</mark> $dl$ as shown in Figure. Note that the raius from $dl$ to an arbitary point $P$ is $r$, then this segment induces a velocity $dV$ as (We note we don't take consideration on sign here, which is given by [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 1 Essential Concepts/Chapter3 Fundamentals  of Invisid and Incompressible Flow/8-16 Elementary Flow Patterns#3.13 Vortex Flow|velocity of Vortex Flow]]): 
$$\Large\boxed{dV = \frac{\Gamma}{4 \pi} \frac{\boldsymbol{dl \times r}}{|r|^{3}}}\tag{5.5}$$
this equation is called <mark style="background: transparent; color: red">Bitot-Savart law</mark> which is the most fundamental relation in the theory of inviscid and incompressible flow. 
`````ad-quote
title: Bitot-Savart law in Electromagnetics
collapse: open
The Bitot-Savart law is also extensively used in Electromagnetics, which gives the theory of the electronic field, which ==gives the magnetic field strength caused by a wire carring an electronical current== $I$ as : 
$$dB = \frac{\mu I}{4 \pi} \frac{dl \times  r}{|r|^{3}}$$

where $\mu$ is the permeability of the surrounding medium of the wire. Indeed, the Bitot-Savart law is <mark style="background: transparent; color: red">a general result of potential theory</mark>. 
`````

We give that <u>when a number of fliaments are used in conjunction with a uniform freestream</u>, Then it's possible to synthesize a flow which has a practical application. Then we apply the Bitot-Savart law to a straight vortex filament of infinite length, we note that the strength of the filament is $\Gamma$, then the velocity induced at point P by segment of the vortex filament $dl$ is given by equation (5.5) above, then we can calculate the velocity indeced at point P as : 
![[Excalidraw/1-8 Introduction and fundamental Concepts 2023-09-20 06.18.31|450]]
$$\vec{V} = \int_{- \infty}^{\infty} \frac{\Gamma}{4 \pi} \frac{dl\times r}{|r|^{3}}$$
With [[Excalidraw/1-8 Introduction and fundamental Concepts 2023-09-20 06.23.23|a simple derivation process]], we get the <mark style="background: transparent; color: red">velocity induced by the entire vortex filament</mark> is: 
$$\Large\boxed{V = \frac{\Gamma}{2 \pi h}}\tag{5.10}$$
we compare it to [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 1 Essential Concepts/Chapter3 Fundamentals  of Invisid and Incompressible Flow/8-16 Elementary Flow Patterns#3.13 Vortex Flow|equation 3.105]] and know that this is precisely the same result as it for the point vortex. (the result here is just the magnitude). 
also, for a semi-infinite vortex filament (the perpendicular feet is boundary of flow), the velocity is given by:
$$\small V= \frac{\Gamma}{4\pi h}$$
That is <mark style="background: transparent; color: red">the Helmholtz's vortex theorems</mark>, explained as follows: 
1. the strength of a vortex filament is constant along its length.
2. a vortex filament cannot end in a fluid, it must extend to  the boundaries of the fluid or a closed path. 

We introduce the concept of **lift distribution** along the span of a finite wing, where the **local chord** is $c$ and local **geometric angle of attack** is $\alpha$, and <u>the airfoil section is a given shape</u>. Which gives the lift per unit span as $L'(y_1)$, For **another location** $y_2$ along the span (we note $c$, $\alpha$ and the airfoil shape can be various). Also, <u>many wings are geometrically twisted</u>(so that $\alpha$ can be different at different spanwise locations, called **geometric twisted**). 

If the tip is **at a lower $\alpha$ than the root**, the wing is said to have ***washout***, and if higher $\alpha$, the wing has ***washin***. We note the <u>distribution of lift per unit span</u> along the wing is sketched as follows : 
![[Excalidraw/1-8 Introduction and fundamental Concepts 2023-09-20 08.54.16|400]]
## 5.3 Prandtl's Classical Lifting-Line Theory 
A vortex filament of strength $\Gamma$ is bounded to a fixed location (so-called "bound vortex"). This filament will experience a force $L' = \rho_{\infty}V_{\infty} \Gamma$ ([[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 1 Essential Concepts/Chapter3 Fundamentals  of Invisid and Incompressible Flow/8-16 Elementary Flow Patterns#3.15 Kutta-Joukowski Theorem|Kutta-Joukowski Theorem]]), we can replace a wing with a boundary vortex.
![[Excalidraw/1-8 Introduction and fundamental Concepts 2023-09-21 09.29.04|800]]
We assume the vortex filament continues as two free vortices trailing down from wing tips to infinity(Considering [[#5.2 Biot-Savart Law & Helmholtz's Theorem|Helmholtz's Theorem]]). A single Horseshoe vortex **induces a downwash** shown above, we give that the two trailing vortex both contribute to the induced velocity along the bound vortex and both contributions are in the downward direction.

We note that when the velocity is downward, the velocity is negative. and the velocity at point $y$ <b><mark style="background: transparent; color: blue">induced by the semi -infinite vortices</mark></b> is : 
$$\boxed{w(y) = - \frac{\Gamma}{4\pi \left(\frac{b}{2} +  y\right)} - \frac{\Gamma}{4\pi\left(\frac{b}{2} -y\right)} =- \frac{\Gamma b}{4 \pi \left[ (\frac{b}{2})^{2} + y^{2}\right]}}$$
Also, in the equation, the first term is the contribution from the left trailing vortex($y = - \frac{b}{2}$), and the second term is contribution from the right trailing vortex($y = \frac{b}{2}$). (for $y = \pm \frac{b}{2}$, $w \rightarrow \infty$). We simulate the wing by a finite number of **horseshoe vortex** along the lifting line.
![[Excalidraw/1-8 Introduction and fundamental Concepts 2023-09-21 10.07.56|600]]
We take the structure above as the **combination of 3 horseshoe vortex**, hence the <mark style="background: transparent; color: red">circulation vortex</mark> distributed over the span can be summed according to the figure above. the strength of each vortex is equal to the change of circulation along the lifting line.

The total strength of the sheet integrated across the wing is zero(since every horseshoe vortex consists of trailing vortices of equal strength but in opposite directions). We give the function of trailing vortex strength, if we set the circulation at $y$ is $\Gamma(y)$, the change of circulation over the segment $dy$ is $\frac{d\Gamma}{dy}dy$
![[Excalidraw/1-8 Introduction and fundamental Concepts 2023-09-21 13.57.34]]
we note every segment of the trailing vortex $dx$ induces a velocity at $y_0$ with [[#5.2 Biot-Savart Law & Helmholtz's Theorem]], for a segment, induced at $dy$, the velocity is given as(semi-infinite vortex): 
$$dw = - \frac{\frac{d\Gamma}{dy}dy}{4\pi (y_{0} - y)}$$
then we can calculate the velocity at point $y_0$ as : 
$$\boxed{w(y_{0}) = - \frac{1}{4\pi} \int_{-\frac{b}{2}}^{\frac{b}{2}} \frac{\frac{d\Gamma}{dy} dy}{y_{0} - y}}\tag{5.15}$$
this equation gives **the downwash at $y_0$ due to the trailing vortices**. We assume the airfoil with the model of a  lifting line, and also calculate $\Gamma(y)$ for the given finite wing. 

We learn from [[#5.1 Downwash and Induced Drag]] that the relationship between the velocity at infinite distance and the downwash is : 
$$\alpha_i (y_0) = \tan^{-1} \left(- \frac{w(y_0)}{V_{\infty}} \right)$$
for a small $\alpha$, we have $\alpha = - \frac{w(y_0)}{V_\infty}$, then we use the equation (5.15) above and get <mark style="background: transparent; color: red">the expression for the induced angle of attack in terms of the circulation distribution</mark>: 
$$\boxed {\alpha_i = \frac{1}{4 \pi V_\infty} \int_{- \frac{h}{2}}^{\frac{h}{2}} \frac{\frac{d\Gamma }{dy}dy}{y_0 - y}}$$
with the equaiton (note that $a_0$ is the <u>local section lift slope</u>, which is $2\pi (rad^{-1})$ for thin airfoil)
$$c_l = a_0 \left[ \alpha_{eff}(y_0) - \alpha_{L = 0}\right]= 2\pi [\alpha_{eff} - \alpha_{L = 0}]\rightarrow \alpha_{eff} = \alpha_{ L= 0} + \frac{c_l }{2 \pi}$$
If there is <b><mark style="background: transparent; color: blue">no aerodynamic twist</mark></b>, the $\alpha_{L= 0}$ not vary with $y_0$ and remains constant across the span. Then we know:
$$c_l'  = \frac{L'}{q_{\infty}S}= \frac{\rho_{\infty} V_{\infty} \Gamma(y_0)}{ q_{\infty} S}  = \frac{2\Gamma (y_0)}{V_{\infty} c(y_0)}$$
then considering [[#5.1 Downwash and Induced Drag]], $\alpha = \alpha_i + \alpha_{eff}$, then : 
$$\boxed {\alpha_{eff} = \alpha_{L= 0} + \frac{\Gamma(y_0)}{\pi V_\infty c(y_0)}\qquad  \alpha  = \alpha_{L= 0} + \frac{\Gamma(y_0)}{\pi V_\infty c(y_0)} + \frac{1}{4\pi V_{\infty}} \int_{- \frac{h}{2}}^{\frac{h}{2}}\frac{\frac{d\Gamma}{dy} dy}{y_0  - y}}\tag{5.23}$$
This is <b><mark style="background: transparent; color: blue">The Fundamental Equation of Prandtl's lifting-line theory</mark></b>. which state the calculation of geometry angle. We can obtain the 3 aerodynamic characters of a finite wing by the following equation: 
Lift Distribution 
$$L'(y_{0}) = \rho_{\infty}V_{\infty} \Gamma(y_{0})\tag{5.24}$$
then the total lift is obtained by integrating the equation over the span : 
$$L = \int_{-\frac{b}{2}}^{\frac{b}{2}} L'(y)dy = \rho_{\infty}V_{\infty} \int_{\frac{-b}{2}}^{\frac{b}{2}} \Gamma(y)dy\tag{5.25}$$
and the lift coefficient follows immediately from the equation as :
$$C_{L} = \frac{2}{V_{\infty} S}  \int_{\frac{-b}{2}}^{\frac{b}{2}} \Gamma(y)dy\tag{5.26}$$
also we have : 
$$D'_{i} = L'_{i}\sin \alpha_{i} \approx L'_{i}\alpha_{i}\rightarrow  \quad D = \rho_{\infty}V_{\infty} \int_{\frac{-b}{2}}^{\frac{b}{2}} \Gamma(y)\alpha_{i}(y)dy$$
$$C_{D,i} = \frac{D_{i}}{q_{\infty} S} = \frac{2}{V_{\infty} S}  \int_{\frac{-b}{2}}^{\frac{b}{2}} L(y) \alpha_{i}(y)dy\tag{5.30}$$
### 5.3.1 Elliptic Lift Distribution 
We can consider distribution given as : 
$$\boxed {\Gamma(y) = \Gamma_{0} \sqrt{1 - \left(\frac{2y}{b}\right)^{2}}}$$
it gives a elliptic distribution with $y$ along the span. 
$$\frac{d\Gamma}{dy} = - \frac{4 \Gamma_{0}}{b^{2}} \frac{y}{\sqrt{1 - \frac{4 y^{2}}{b^{2}}}}$$
if we make the substitution $y = \frac{b}{2} \cos \theta$ (which has been a reasonal substitution when we calculate the curl), then: 
$$\small w (\theta_{0}) = -\frac{\Gamma_{0}}{2 \pi b}\int_{0}^{\pi} \frac{\cos \theta}{\cos \theta - \cos \theta_{0}}d\theta$$
which yields(important form of integration):  
$$\Large \boxed{w(\theta_{0}) = - \frac{\Gamma_{0}}{2b}}\tag{5.35}$$
$$\alpha_{i}= - \frac{w}{V_{\infty}} = \frac{\Gamma_{0}}{2b V_{\infty}} = \text{const}\tag{5.36}$$
Hence <mark style="background: transparent; color: red">the downwash is constant over the span for an elliptic lift distribution</mark>. We also reach a more useful expression for $\alpha_{i}$ by substituting $\Gamma_{0}$ into the expression of lift(note we also use $y = \frac{b}{2}\cos \theta$),  and get: 
$$\Large\boxed{L  = \rho_{\infty} V_{\infty}\Gamma_{0} \frac{\pi  b }{4}}\qquad \boxed{\Gamma_{0} = \frac{4L}{\rho_{\infty}  V_{\infty} b \pi }}\tag{5.38}$$
through the [[Excalidraw/1-8 Introduction and fundamental Concepts 2023-09-22 08.19.57|simple derivation]], we can reach the expression 
$$\boxed{\alpha_{i} = \frac{C_{L}}{\pi \text{AR}}\qquad\qquad  \text{where AR}  \equiv  \frac{b^{2}}{S} \quad \text{ (aspect ratio)}}\tag{5.42}$$
note aspect ratio AR is a important geometry property. Also, we have the coefficient of drag : 
$$\boxed{C_{D,i} = \frac{C_{L}^{2}}{\pi \text{AR}}}$$
that equation states that <u>the drag coefficient is directly proportional to the square of lift coefficient</u>, and inverse proportional to <mark style="background: transparent; color: red">aspect ratio</mark>. 

We also note that because $C_{D}\propto C_{L}^{2}$, the <mark style="background: transparent; color: red">induced drag coefficient increases rapidly with the lifting coefficient</mark>. (which gives that when we want to increase the lifting coefficient to take off, we have higher drag coefficient that the plane flies slower). Also we gives the effect of the aspect ratio below: 
![[Excalidraw/1-8 Introduction and fundamental Concepts 2023-09-28 17.22.00|500]]

We consider a wing with <mark style="background: transparent; color: red">no geometric twist and no aerodynamic twist</mark>. From the equation [[#5.1 Downwash and Induced Drag|effect angle of attack]] and [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 2 Equations for Incompressible Flow/Chapter4 Incompressible flow/9-16 Applied Concepts#4.9 Aerodynamic Center|the lifting coefficient curve]], we have $\frac{dc_{l}}{d\alpha} = a_{0}$, then: 
$$c_{l} = a_{0} (\alpha_{eff} - \alpha_{L = 0})$$
(note $a_{0}= 2\pi$ for the thin airfoil theory), $c_{l}$ persists a constant along the span of the airfoil. Hence the lift per unit span is given by (we note that in the equation $L'(y)$ varies elliptically along the span):
$$L'(y) = q_{\infty} S_{\text{per unit length}} c_{l} = q_{\infty} c c_{l}\quad  c(y) = \frac{L'(y)}{q_{\infty} c_{l}}$$
This solution gives the concolutsion: <mark style="background: transparent; color: red">for an elliptic lift distribution, the chord must vary elliptically along the span</mark>. <mark style="background: transparent; color: blue">or the wing platform must be elliptical to reach the elliptic lift distribution</mark>. Which is shown below. 
![[Excalidraw/1-8 Introduction and fundamental Concepts 2023-09-28 19.30.00|550]]

### 5.3.2 The General Lift Distribution 
we can use  the transformation $y = -\frac{b}{2} \cos \theta$ for the camber line. We also gives the spanwise direction by $\theta$ and use the elliptic lift distribution as : $\Gamma(\theta) = \Gamma_{0} \sin \theta$ 
$$ \text{General Lift } \Gamma(\theta) \overset{\text{Fourier Expansion}}{\rightarrow }\quad \Gamma(\theta) = 2 b V_{\infty} \sum^{N}_{1} A_{n} \sin n \theta\tag{5.48}$$
the expansion above must satisfy [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 2 Equations for Incompressible Flow/Chapter5 Incompressible flow over finite wings/1-8 Introduction and fundamental Concepts#5.3 Prandtl's Classical Lifting-Line Theory|Fundamental equation of Prandtl's lifting-line theory]], Substituting the $\Gamma(\theta)$ into it and use [[Excalidraw/1-8 Introduction and fundamental Concepts 2023-09-28 20.34.55|the derivation process]] and obtain : 
$$\alpha_{i}(\theta_{0}) = \frac{\sum_{1}^{N} n A_{n} \sin n \theta_{0}}{\sin \theta_{0}}$$
$$\boxed{\alpha(\theta_{0}) = \frac{2b}{\pi c(\theta_{0})} \sum_{1}^{N} A_{n}\sin n\theta_{0} + \alpha_{L = 0} (\theta_{0}) + \sum_{1}^{N} n A_{n} \frac{\sin n \theta_{0}}{\sin \theta_{0}}}\tag{5.51}$$
In the equation above, $\alpha_{L = 0}$ is know by the airfoil properties and the only unknowns are the $A_{n}$'s, then we can write the equation <u>at a given spanwise location</u>. The numerical values that ensure that <mark style="background: transparent; color: red">the general circulation distribution satisfies</mark> [[#5.3 Prandtl's Classical Lifting-Line Theory]]. We give the lifting coefficient of the airfoil by [[#5.3 Prandtl's Classical Lifting-Line Theory|the equation (5.26)]] as : 
$$C_{L} = \frac{2}{V_{\infty} S} \int_{- \frac{b}{2}}^{\frac{b}{2}}\Gamma(y) dy = \frac{2 b^{2}}{S} \sum_{1}^{N}A_{n}\int_{0}^{\pi}  \sin n \theta \sin \theta  d\theta$$
we reference the [[📘ClassNotes/📐Mathmatics/📈Advanced Mathematics/♎重要公式/基本积分表|integral form of the high-order triangle function]], then 
$$\small\sum_{1}^{N} A_{n} \int_{0}^{\pi} \sin n \theta \sin \theta = \frac{\pi}{2} A_{1} $$
then we have the relationship 
$$\Large\boxed{C_{L}= \frac{b^{2}\pi A_{1}}{S} = A_{1} \pi \text{AR}}$$
From the [[#5.3 Prandtl's Classical Lifting-Line Theory|equation (5.30)]], we know the expression of the $C_{D,i}$, then use the [[Excalidraw/1-8 Introduction and fundamental Concepts 2023-09-28 21.57.20|Derivation process]], We can reach the result : 
$$\boxed{C_{D,i} = \frac{C_{L}^{2}}{\pi\text{AR}} (1 + \delta) \qquad \text{where}\quad  \delta = \sum_{2}^{N} n\left(\frac{A_{n}}{A_{1}}\right)^{2}}$$
we define a<mark style="background: transparent; color: red"> span efficiency factor</mark> by $e = (1 + \delta)^{-1}$(note that $e\leq  1$, for elliptic distribution, $e = 1$), then the equation can be written as: 
$$\Large\boxed{C_{D,i} = \frac{C_{L}^{2}}{\pi e \text{AR}}}$$
`````ad-check
title: conclusion
collapse: open
From the derivation above, we know that **the lift distribution which yields the minimum drag** is the **elliptical lift distribution**. 
`````
![[Excalidraw/1-8 Introduction and fundamental Concepts 2023-09-29 00.26.53|900]]
### 5.3.3 Effect of the Aspect ratio 
we know from [[#5.1 Downwash and Induced Drag]] that the total drag of a fninite wing is the sum of the profile drag coefficient and the <mark style="background: transparent; color: red">induced drag coefficient</mark> : 
$$C_{D} = c_{d} + \frac{C_{L}^{2}}{\pi e \text{AR}}\qquad C_{D,i} = \frac{C_{L}^{2}}{\pi e \text{AR}}\space \text{induced  drag coefficient}$$
the variation of $C_{D}$ with $C_{L}$ is a parabolic variation, which is shown in the figure below. 
![[attachments/Screenshot_20230929_004837_com.microsoft.skydrive.png|700]]
We consider <mark style="background: transparent; color: red">two wings with different aspect ratio</mark>, which gives 
$$C_{D,1} - C_{D,2} = \frac{C_{L}^{2}}{\pi e \text{AR}_{1}} - \frac{C_{L}^{2}}{\pi e \text{AR}_{2}}$$
we consider that when $C_{D, 2}$and$\text{AR}_1$ is fixed, we note if we increase $\text{AR}_2$, $C_{D,1}$ will decrease. In the figure below, we observe the angle between chord line and the relative wind $\alpha$, but the real lift coefficient is a function of $\alpha_{eff}$, then <u>the bottom abscissa of finite elliptic wing is stretched</u>. Also, since there's no induced effects when zero lift, the $\alpha_{L = 0}$ of infinite and finite wings are the same. 
![[Excalidraw/1-8 Introduction and fundamental Concepts 2023-09-29 01.20.20|550]]
We know from above that <mark style="background: transparent; color: red">the function of the finite wing is to reduce the lift slope</mark>. 
$$\frac{dC_{L}}{d (\alpha - \alpha_{i})} = a_{0} \quad \rightarrow\quad  C_{L}= a_{0}(\alpha - \alpha_{i} ) + \text{const} $$
we know from [[#5.3 Prandtl's Classical Lifting-Line Theory#5.3.1 Elliptic Lift Distribution|Elliptic Lift Distribution]] that for a elliptic wing the $\alpha_i$ can be written as : 
$$\alpha_{i} = \frac{C_{L}}{\pi \text{AR}} \rightarrow C_{L} = a_{0}\left(\alpha -\frac{C_{L}}{\pi  \text{AR}}\right) + \text{const}$$
we got (when $\text{AR} \rightarrow \infty, a\rightarrow a_0$) 
$$\Large\frac{dC_{L}}{d\alpha} = a = \begin{cases}
\frac{a_{0}}{1 + \frac{a_{0}}{\pi\text{AR}}} \qquad \space \space \space \space \text{Elliptic form} \\ 
\frac{a_{0}}{1 + \frac{a_{0}}{\pi \text{AR}} (1 + \tau)} \quad \text{General form}\\
\end{cases}$$
We note in the equation above, $\tau$ is the **lift efficiency factor**, and in some examples we can assume $\tau = \delta$ (induced drag factor, can be referenced from [[Excalidraw/1-8 Introduction and fundamental Concepts 2023-09-29 00.26.53|Figure]]) also,  $a_{0}$ is a constant (that is the ideal slop for the Infinite wing).
`````ad-note
title: The expression of lift and drag coefficient
collapse: open
we often use $c_l$ and $c_D$ as the lift and drag coefficient of the airfoil, $C_{L}$ and $C_{D}$ are the lift and drag coefficient of the wing. 
Note that this is usually not equal to each other, and for airfoil 
$$C_{D}= c_{d} + \frac{C_{L}^{2}}{\pi e \text{AR}}$$
[[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/♎Important Conclusions and Lists/Dimensionless force and moment coefficients|Dimensionless force and moment coefficients]]
also we introduce an important concept calld **Span loading**: 
$$Span loading \equiv \frac{W}{b}$$
where $W = L$ is the weight of airfoil. we can derive that 
$$D_{i} = \frac{1}{\pi e q_{\infty}} \left(\frac{W}{b}\right)^{2}$$
hence $D_{i}$ depends explicitly on the ratio $\frac{W}{b}$ but not the aspect ratio. 
`````

`````ad-todo
title: Implements 
collapse: open 
The numerical method and the flow over a 3-Dimension body. 
`````

