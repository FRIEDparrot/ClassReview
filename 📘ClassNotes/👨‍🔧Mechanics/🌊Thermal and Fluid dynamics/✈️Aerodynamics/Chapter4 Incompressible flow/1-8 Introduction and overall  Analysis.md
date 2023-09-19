## 4.1 Introduction 
For an airfoil of a  particular shape at a certain angle of attack to a given low-speed flow, the calculation of the lift of airfoil `->` first and simplest method is make a model and test it in a wind tunnel.
This chapter will have discussion on <u>how the airfoil properties is obtained. 
</u> and the <mark style="background: transparent; color: red">circulation theory of lift -- the crowing jewel of inviscid</mark>. 

Also we note in the year 1945, NASA developed four essential airfoil shapes, as shown below.
![[Excalidraw/1-8 Introduction and overall  Analysis 2023-08-29 23.56.00|600]]
for the typical supercritical airfoil, it can <mark style="background: transparent; color: red">reach a good efficiency near Mach one</mark>. 

Since the force and the moment is mainly due to the pressure distribution, also this can be analyzed in the condition of inviscid flow, so we present two roaches to low-speed theory `->` one is classical thin airfoil theory. and the other is  the modern numerical approach for arbitrary airfoils (using the vortex panels method).

## 4.2 Airfoil NomenClature
The nomenclature developed by NASA is a Common standard for naming the airfoil. 
We consider the airfoil in the figure . The <mark style="background: transparent; color: red">mean camber line is the locus of points halfway between the upper and lower surface</mark>, and <b><mark style="background: transparent; color: blue">the most forward and rearward points of the mean camber line are the leading and trailing edges</mark></b>.
Then we note the straight line between the leading edge and the trailing edges is called the <b><mark style="background: transparent; color: blue">chord line</mark></b>. 
can ==the length of he chord== is represented by $c$ in the calculation.
Also, the camber is the <u>maximum distance between the mean chamber line  and the chord line</u>. and thickness is the distance between the upper and lower surface. as shown in the figure below. 
![[Excalidraw/1-8 Introduction and overall  Analysis 2023-08-30 00.35.04|750]]
The nomenclature of the airfoil include 4-digits naming method and 5-digits naming method. 

We give some examples of the naming of the Airfoil. The first family of airfoils is **"four-digit" series**. in which <mark style="background: transparent; color: orange">the first digits give the maximum camber in hundredths of chord, and the second digit is location of maximum camber along the chord from the leading edge in tenths of chord, and the last two digits give the maximum thickess in hundreds of chord</mark>. For NASA 2412 foil, the maximum camber is $0.02c$ located at $0.4c$ from the leading edge. and the maximum thickness is $0.12c$. 

Another family of the airfoils is **5-digit series**, For example, NACA 23012 airfoil, ==the first digit $\times  \frac{3}{2}$ gives the lift coefficient, the next two digits when divided by 2 gives the location of the maximum camber along the chord from the leading edge, and the final two digits gives the maximum thickness in hundredths of chord==,   the design lift coefficient is 0.3, and the location of it is $0.15c$, and the airfoil has 12 percent maximum thickness.

One of most-widely used family of NACA airfoils is **6-series** laminar flow airfoils. For NACA 65-218 airfoil , the 6 is the series designation, and the minimum pressure occurs at $0.5c$ for basic symmetric thickness distribution at zero lift. And the **design lift coefficient** is 0.2 and the airfoil is 18 percent **thick**. 

## 4.3 Airfoil Characteristics 
We  know that the typical variation of lift coefficient with angle of attack for an airfoil is sketched in [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Chapter1 Introductory Thoughts/Design Utilities/Design Box 1.8.2|Design Box 1.8.2]]

we give the following figure to give detailed designation for the lifting coefficient. 
![[Excalidraw/1-8 Introduction and overall  Analysis 2023-08-31 13.36.38|400]]
We note that $c_l$ increases linearly with $\alpha$ at the left side of the curve, and become Nonlinear at the top. The maximum of $c_l$ occurs just prior to the stall, We note the lift at $\alpha$ = 0 is finite. **the lift goes to zero only when the airfoil is pitched to some negative angle of attack**, this angle $\alpha$ (when lift equals zero) is ***called zero-lift angle of attack*** and denoted by $\alpha_{L = 0} =0$, and <mark style="background: transparent; color: red">for a symmetric airfoil</mark>, $\alpha_{L = 0} = 0$. 

We review [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Chapter1 Introductory Thoughts/1-11 Some Introductory Thoughts#1.5 Aerodynamic Forces and moments|section 1.5]], We also note the **quarter- chord point is commonly used**.

The drag of airfoil is <u>due to both body friction drag and pressure drag due to flow separation</u>. The sum of two effects yields the profile drag coefficient $c_d$ for the airfoil. We also note $c_{d}$ is sensitive to $\text{Re}$ 
![[Excalidraw/1-8 Introduction and overall  Analysis 2023-08-31 14.43.23|1000]]
On the airfoil there is **one point which the moment is independent of angle of attack**, this point is defined as the ***aerodynamic center***. 

## 4.4 The Vortex Sheet  
The vortex sheet method is the theoretical solutions for low-speed flow over airfoils. 

We image a straight line **in $y$ direction** at the figure below, and this called it **a straight vortex filament**. As in the figure below, the filament itself <u>is identical induced to that induced by a point vortex</u> of strength $\Gamma$; That is, the flows in the planes perpendicular to the <mark style="background: transparent; color: red">vortex filament</mark> at $O$ and $O'$. a section of straight vortex filament is shown below.
![[Excalidraw/1-8 Introduction and overall  Analysis 2023-09-01 10.51.56|750]]
We introduce the concept of **source sheet**. A ***source sheet*** is <b><mark style="background: transparent; color: blue">an infinite number of line sources side by side. The strength of each line source is infinitesimally small.</mark></b> For the vortex flow , we can consider a analogous situation `->` when we place infinite straight filaments together, <u>the vortex filaments forms a vortex sheet</u>, this is shown in the right side of the figure above.

We define **the strength of the sheet** as $\gamma = \gamma(s)$ as the strength of the sheet. Then the strength of an infinitesimal portion is $\gamma ds$, then we consider a point $P$ on the figure above, and the small section induces a velocity $dV$ at point $P$, which is given by [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Chapter3 Fundamentals  of Invisid and Incompressible Flow/8-16 Elementary Flow Patterns#3.13 Vortex Flow|the velocity equation of the Vortex Flow]] as : 
$$\Large\boxed{dV = -\frac{\gamma ds}{2\pi r}}\tag{4.1}$$
The velocity potential at Point $P$ can be derived by 
$$dV_{\theta} = \frac{1}{r}\frac{\partial \phi}{\partial \theta} \rightarrow \phi = - \frac{\gamma ds}{2 \pi} \theta$$
then we integral the entire potential $P$ due to the entire vortex sheet from $a$ to $b$, then get the velocity potential at point $P$ due to the source sheet (this equation is important for vortex panel sheet): 
$$\Large\boxed{\phi(x, z) = - \frac{1}{2 \pi} \int_{a}^{b} \theta \gamma ds}\tag{4.3}$$
We also give that the **circulation around the vortex sheet** is the sum of the elemental vortices:
$$\Large\boxed{\Gamma = \int_{a}^{b} \gamma ds}\tag{4.4}$$
the source sheet <mark style="background: transparent; color: red">has a discontinuous change in the direction of the normal comoinent of velocity across the sheet</mark>. We note that the change in the direction of the normal component of velocity changes direction by 180$\degree$ crossing the sheet, whereas the tangential component of velocity are the same above and below the sheet. 
![[Excalidraw/1-8 Introduction and overall  Analysis 2023-09-01 14.40.00|600]]
we consider the a section of the sheet of length $ds$, and <u>the velocity components </u> tangential to the top/bottom of the sheet is given by $u_1$ and $u_2$, then the <b><mark style="background: transparent; color: blue">circulation</mark></b> around the dashed path is :
$$\Gamma = - (v_{2} d n  - u_{1}ds - v_{1}dn + u_{2} ds )\tag{4.5}$$
that is the **integral of the velocity in the clockwise direction**. or we can write it as $\Gamma ds = (u_{1} - u_{2} )ds + (v_{1} - v_{2}) dn$. In that case, we let the surface at top and bottom approach the vortex sheet, then $dn \rightarrow 0$, we have : 
$$\Large\boxed{\gamma = u_{1} - u_{2}}\tag{4.8}$$
this equation states that **the local jump in tangential velocity across the vortex sheet is equal to  the local sheet strength**. 
![[Excalidraw/1-8 Introduction and overall  Analysis 2023-09-01 15.07.12|400]]
when added to the uniform velocity of magnitude $V_{\infty}$ will make the vortex sheet a streamline of the flow. Apparently, **for a vortex sheet of the shape of airfoil**, the circulation can be calculated as : 
$$\Large\boxed{\Gamma = \int \gamma  ds}$$
the resulting **lift is given by Kutta-Joukowski theorem** in [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Chapter3 Fundamentals  of Invisid and Incompressible Flow/8-16 Elementary Flow Patterns#3.15 Kutta-Joukowski Theorem|section 3.15]] as :
$$\Large L' = \rho_{\infty} V_{\infty} \Gamma$$
`````ad-note
title: real-life application
collapse: close
in the real life, there is a *distribution of vorticity* along the airfoil surface due to the viscous effects.We can replace the airfoil surface with a vortex sheet. And this can be construed as a way of **modeling the effect in an inviscid flow**. 

secondly, if the airfoil is made very thin, we can replace it with **a single vortex sheet distributed over the camber line**. This approach has the advantage of yielding a closed-form analytical solution. 
`````

## 4.5 The Kutta Condition
For a given airfoil at a given attack of angle $\alpha$ , there are **infinite number of valid theoretical solutions**, which correspond to the **infinite choice of** $\Gamma$, so we need <mark style="background: transparent; color: red">an additional condition</mark> that fix $\Gamma$ for a given airfoil at a given $\alpha$. we use the two solutions below as example : 
![[Excalidraw/1-8 Introduction and overall  Analysis 2023-09-03 14.41.30|700]]
We give the condition that <mark style="background: transparent; color: red">in establishing the steady flow over a given angle of attack, the nature</mark> <b><mark style="background: transparent; color: blue">adopts the particulat value of  circulation which results in the flow leaving smoothly at the trailing edge.</mark></b> This condition is called **Kutta Condition**. 

We consider the trailing edge with a finite angle , and denote the velocities along the top surface and the bottom surface by $V_1$ and $V_2$. For the trailing edge, since the direction at the point A in the two directions is not physically possible, which states that $A$ is a stagnation point.
![[Excalidraw/1-8 Introduction and overall  Analysis 2023-09-03 14.49.40|550]]
for the cusp trailing edge, the Bernoulli equation yields that the velocities at top and bottom has the same magnitude and direction. 
$$p_a + \frac{1}{2} \rho V_{1}^{2} = p_{a} + \frac{1}{2} \rho V_{2}^{2}\rightarrow  V_{1} = V_{2}$$

**The Kutta Condition Yields** : 
1. For a given airfoil at a gien angle of attack, the value of $\Gamma$ around the airfoil is such that  the flow leaves the trailing edge smoothly. 
2. If the trailing-edge angle is finite, the trailing edge is a stagnation point. 
3. If the trailing-edge is cusped, the velocity leave the top surface and bottom surface are finite and equal in magnitude and direction. 

We consider the vortex sheets, The strength of the vortex sheets is variable along the sheet and denoted by $\gamma_{s}$, then we obtain the strength at the trailing edge from [[#4.4 The Vortex Sheet|equation(4.8)]]: 
$$\gamma_{TE} = \gamma_{a} = V_{1}- V_{2}$$
hence the Kutta condition gives the result : 
$$\Large \boxed{\gamma(TE) = 0}$$

## 4.6 Kelvin's Circulation Theorem and Starting Vortex 
We consider how does the nature generate the circulation for the need of Kutta condition. In the figure below, the left figure gives **the shape of fluid element along a curve $C_1$ at a given instant time** $t_1$. then when the stream flows downstream, and at some later time $t_2$ the same fluid elements forms another curve $C_2$, then consider the circulation around two different circles : 
![[Excalidraw/1-8 Introduction and overall  Analysis 2023-09-04 09.33.49|450]]
by definition, the circulation around $C_1$ is $\Gamma_{1} = - \int_{C_{1}} V ds$, and the same fluid elements forms the circulation $\Gamma_{2} = - \int_{C_{2}} V ds$. **Since we are following a set of specified fluid elements, we can state that circulation around a closed curve formed by a set of continuous fluid elements remains constant as the fluid element elements move throughout the flow**. So we have : 
$$\Large\boxed{\Gamma_{1} =  \Gamma_{2}}$$
`````ad-bug
collapse: close
why for a specified fluid element the circulation remains constant ? 
We assume that body force $f$ are zero, since the momentum is conserved, we have
$$\int_{c} V ds  = \int_{c} V_{x} dx + V_{y} dy$$
we set the flow is inviscid, then the tangential velocity is not changed since there's no tangential friction. 
`````

then the statement of the above discussion is 
$$\Large \boxed{\frac{D \Gamma}{ Dt} =  0}\tag{4.11}$$
this equation is called ***Kelvin's circulation theorem***. 
An interesting consequence of Kelvin's theorem is proof that a stream surface which is **a vortex sheet at some instant in time remains a vortex sheet for all times**. which is shown in the figure below : 
![[Excalidraw/1-8 Introduction and overall  Analysis 2023-09-04 10.16.33|450]]
At first time, since $V = 0$ everywhere, the circulation around $C_1$ is zero. so $\Gamma_{c_{1}}= \Gamma_{c_{2}}= 0$,  Then **start the flow in motion over the airfoil.** The flow will **tend to curl around the trailing edge**.
`````ad-note
title: Starting Vortex 
collapse: open
We note that using the Kutta Condition, the velocity at the trailing edge becomes infinite, but in real life, it tend towards a very large finite number. Then a thin region of very large velocity is formed at the trailing edge.  This region is **high-vorticity region** and it's fixed for some fluid elements, and the vortex is called **starting vortex**, as shown in figure above. 
`````

We subdivide $C_2$ as the figure above by $bd$, then form $C_3$(right side) and $C_4$(left side), Also note the circulation at $C_4$ is clockwise while the circulation over $C_3$ is counterclockwise. We can easily gives that $\Gamma_3+ \Gamma_4 = \Gamma_{2} = 0$. then 
$$\Large\boxed{\Gamma_{3}= - \Gamma_{4}}$$
<u>That is, the circulation around the airfoil is equal and opposite to the circulation aound the starting vortex</u>. With the help of [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Chapter3 Fundamentals  of Invisid and Incompressible Flow/8-16 Elementary Flow Patterns#3.15 Kutta-Joukowski Theorem|Kutta-Joukowski Theorem]] (equation 3.140), we can easily **calculate the strength of the starting vortex only with the Lift per unit span, density and the velocity at infinite distance**. But we note that <u>no practical aerodynamic calculation requires the strength of a starting vortex</u>, this is a theoretical construct that is consistent with the generation of circulation around a lifting two-dimensional body. 

`````ad-quote
title: The Starting Process
collapse: open
We consider how the Kutta Condition is satisfied again at last, As the **starting process** continues, vorticity from the trailing edge constantly **fed into the starting vortex**, then making it stronger with a consequent larger counterclockwise circulation, then the clockwise <u>circulation around the airfoil becomes stronger  making the flow at the trailing edge more closely approach the Kutta condition</u>. **Thus weakening the vorticity shed from the trailing edge**. Then **Finally the starting vortex builds up to just the right strength** such  that <b><mark style="background: transparent; color: blue">the equal-and-opposite clockwise circulation around the airfoil leads to smooth flow from the trialing edge</mark></b>, then the Kutta condition is exactly satisfied. 
`````

## 4.7 Thin Airfoil Theory : The Symmetric airfoil
We consider the thin airfoils as  follows: the thin airfoil plane can be **simulated by a vortex sheet placed along the camber line**. Since the $\Gamma$ is given in the section [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Chapter3 Fundamentals  of Invisid and Incompressible Flow/8-16 Elementary Flow Patterns#3.15 Kutta-Joukowski Theorem|3.15]], then we only need to calculate $\gamma(s)$ that the <mark style="background: transparent; color: red">camber line becomes a streamline of the  flow and the Kutta condition satisfied</mark>. Then we can form the calculation of the lifting force $L$. 

for an airfoil  at an angle of attack $\alpha$, the distance <mark style="background: transparent; color: red">along the camber line</mark> is $s$, and shape is $z =z(x)$, Chord length is $c$. We use the figure at the right side to simulate the airfoil at left : we set $w' = w'(s)$ is the component of velocity <mark style="background: transparent; color: red">normal to the camber line</mark> induced by the vortex sheet. 
![[Excalidraw/1-8 Introduction and overall  Analysis 2023-09-04 20.00.17|900]]
For a thin airfoil, Since <b><mark style="background: transparent; color: blue">the camber line is close to the chord line,</mark></b> the distribution of a vortex sheet placed on the camber line can be viewed approximately falls on the chord line. hence we can write a new function for $\gamma$ as $\gamma = \gamma(x)$ (with the Kutta condition, we have : $\gamma(c) = 0$): 
For the camber line to be a streamline, the component of velocity normal to the surface of airfoil is zero, then the normal component of the velocity. then 
$$\Large \boxed{V_{\infty,n} + w'(s) = 0}\tag{4.12}$$
so an expression for $V_{\infty, n}$ is obtained by analyze the airfoil shape. 
![[Excalidraw/1-8 Introduction and overall  Analysis 2023-09-04 20.18.58|900]]
we use the velocity at infinite distance $V_{\infty}$ and the angle of attack $\alpha$, then the normal component along the airfoil is:  $V_{\infty, n} = V_{\infty} \sin (\alpha - \theta)$ 
for small $\theta$ we have $\sin \theta \approx \tan\theta \approx \theta$, We assume that both the angle of attack and $\theta$ are small angle, then use the shape function $z = z(x)$ as the description of $\theta$ as 
$$\Large\boxed{V_{\infty, n} = V_{\infty} (\alpha - \frac{dz}{dx})}\tag{4.14}$$
also in the  figure 1, if the camber line is close to the chord line, we can use the approximation: 
$$w'(s) \approx w(s)$$
The **expression for $w(s)$ in terms of the strength of vortex sheet $\gamma(s)$** is easily obtained from [[#4.4 The Vortex Sheet|the equation (4.1)]], then the vortex at the chord line `->` we consider **a elemental vortex of strength $\gamma d\xi$ located at the distance $\xi$ from the origin along the chord line**, then $\gamma = \gamma(\xi)$, then 
the velocity (note that is the total velocity) $dw$ at point $x$ induced by the elemental vortex at the point $\xi$ is given as : 
$$\boxed{dw = - \frac{\gamma(\xi) d\xi}{2 \pi (x - \xi)}}\tag{4.16}$$
where $r = x - \xi$ 
or in the integral form :
$$\Large \boxed{w(x) = - \int_{0}^{c} \frac{\gamma (\xi) d \xi}{2\pi (x - \xi)}}\tag{4.17}$$
Since the [[#4.7 Thin Airfoil Theory The Symmetric airfoil|the normal component of velocity is 0 (4.12)]] is the condition for the camber line to be a stream line, then we also have equation (4.14) above which gives the expression of the $V_{\infty, n}$, then we have : 
$$V_{\infty, n} = - w'(s) = V_{\infty} \left(\alpha - \frac{dz}{dx}\right)= \int_{0}^{c} \frac{\gamma(\xi) d\xi}{2 \pi (x - \xi)}$$
then we get 
$$\Large\boxed{\frac{1}{2 \pi } \int_{0}^{c} \frac{\gamma (\xi) d \xi}{x - \xi} = V_{\infty} \left(\alpha - \frac{dz}{dx}\right)}\tag{4.18}$$
This equation is the <b><mark style="background: transparent; color: blue">fundamental equation of thin airfoil theory</mark></b>, it is a statement that the camber line is a streamline of the flow. We also give important standard integral that frequently used :
`````ad-caution 
title: Essential Standard integral that frequently used in airfoil theory
collapse: open
$$\Large \int_{0}^{\pi} \frac{\cos n \theta}{\cos \theta - \cos \theta_{0}} d\theta = \pi \frac{\sin n \theta_{0}}{ \sin \theta_{0}}$$
$$\Large \int_{0}^{\pi} \frac{\sin n\theta \sin \theta }{\cos \theta - \cos \theta_{0}} = - \pi \cos n \theta_{0}$$
`````

For a given point $x$ on the chord line, If we only consider a given airfoil at a given angle of attack, both $\alpha$ and $\frac{dz}{dx}$ are know values. Then <mark style="background: transparent; color: red">the central is the solution of the equation (4.18) under the Kutta Condition</mark> $\gamma(c) = 0$. 
we <b><mark style="background: transparent; color: orange">treat the case of a symmetric airfoil</mark></b>, then according to [[#4.2 Airfoil NomenClature]], the chord is zero. Hence $\frac{dz}{dx}$ is zero, then 
$$\frac{1}{2\pi} \int_{0}^{c} \frac{\gamma(\xi) d \xi}{x - \xi} = V_{\infty}\alpha$$
through [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Chapter4 Incompressible flow/implements & Examples/Derivation of Lift Coefficient for symmetric airfoil.pdf|Derivation of Lift Coefficient for symmetric airfoil.pdf]], we can derive the following results : For symmetric airfoil: (note that we set $\xi = \frac{1}{2} (1 - \cos \theta) ,\space  d\xi = \frac{c}{2} \sin \theta$,then from the leading edge to trailing edge $\theta$ vary from 0 to $\pi$)
**Circulation**:
$$\Large \boxed{\gamma = \frac{2 V_{\infty}  \alpha}{\sin \theta} (\cos \theta + 1)}$$
**Total Circulation**:
$$\Gamma = \pi \alpha c V_{\infty}$$
**Lift Coefficient**: 
$$c_{l} = 2 \pi \alpha\tag{4.33}$$
**Lift Slope**:
$$\frac{dc_{l}}{d\alpha} = 2 \pi$$
the expression of lift slope can be referenced from [[#4.3 Airfoil Characteristics]] 

We consider the **theoretical result the lift is linearly proportion to angle of attack**, then we consider the elemental vortex of strength $\gamma(\xi)$ located at distance $\xi$ from the leading edge. We can calculate the moment about the leading edge as follows : 
Since the circulation that produced by the elemental vortex is $d \Gamma = \gamma (\xi ) d\xi$, then the increment of the lift $dL$ contributed by the elemental vortex is $dL = \rho V_{\infty} d\Gamma$, which **create a moment $-\xi dL$ about the leading edge**. 
then we can obtain the moments of the airfoil is : 
$$\boxed{M'_{LE} = - \int_{0}^{c} \xi dL  = - \rho V_{\infty} \int_{0}^{c} \xi \gamma(\xi) d\xi}\tag{4.35}$$
we easily derive the moment for the symmetric airfoil as :
$$M'_{LE} = - \frac{\pi \alpha c^{2}}{4} \rho V_{\infty}^{2} = - \frac{\pi \alpha}{2} q_{\infty} c^{2}$$
then we can calculate the moment coefficient as 
$$\Large c_{m,le} = \frac{M'_{LE}}{q_{\infty} S c} = - \frac{\pi \alpha }{2}$$
then we have the **relationship between the Lift Coefficient and the moment coefficient** as : $c_{m,le} = - \frac{c_{l}}{4}$
we recall the [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Chapter1 Introductory Thoughts/1-11 Some Introductory Thoughts#1.6 Center of pressure|equation (1.22)]] which gives $M'_{LE} = M'_{\frac{c}{4}} - L' \cdot \frac{c}{4}$,then we get 
$$c_{m, le} = c_{m, \frac{c}{4}} - \frac{L'}{4q_{\infty}S} = c_{m,\frac{c}{4}} -\frac{c_{l}}{4} \qquad \rightarrow  \qquad \boxed{\Large  c_{m, \frac{c}{4}} = 0}$$
That is,**the quarter chord is both the center of pressure and the aerodynamic center**. the experimental value of $c_{m, \frac{c}{4}}$ is constant over a wide range of $\alpha$, as shown below 
![[attachments/Screenshot_20230905_214902_com.microsoft.skydrive.png|400]]
## 4.8 The Cambered Airfoil
The airfoil theory for a cambered airfoil is analogous to that of axisymmetric airfoil. we just use [[#4.7 Thin Airfoil Theory The Symmetric airfoil]] and modify the equation (4.18) where $\frac{dz}{dx}$ is not zero : 
$$\frac{1}{2 \pi } \int_{0}^{c} \frac{\gamma (\xi) d \xi}{x - \xi} = V_{\infty} \left(\alpha - \frac{dz}{dx}\right)\tag{4.18}$$
then we firstly substitute $\xi = \frac{c}{2} (1 - \cos \theta)$ and obtain : 
$$\boxed{\frac{1}{2\pi} \int_{0}^{c} \frac{\gamma(\theta) \sin \theta d\theta}{\cos \theta - \cos \theta_{0}} = V_{\infty}\left(\alpha - \frac{dz}{dx}\right)}\tag{4.42}$$
also we use the Kutta Condition $\gamma(\pi) = 0$, then (the process is complex so we don't derive it)
$$\gamma(\theta) = 2 V_{0} \left(A_{0} \frac{1+ \cos \theta}{\sin \theta} + \sum^{n}_{i=1}A_{n} \sin n \theta\right)\tag{4.43}$$
**The values of $A_n$ depends on the shape of the airfoil (that is, depend on $\frac{dz}{dx}$)**, this is in order that the camber line is a streamline of the flow. We can substitute the expression of $\gamma(\theta)$ into equation (4.42), the substitution and the integral implies: 
$$A_{0} - \sum^{\infty}_{i=1} A_{n} \cos n \theta_{0} = \alpha - \frac{dz}{dx}$$
then we can get
$$\frac{dz}{dx} = (\alpha - A_{0}) + \sum^{\infty}_{i=1} A_{n}\cos n \theta_{0}\tag{4.46}$$
this equation is <b><mark style="background: transparent; color: blue">the transformed version of the fundamental equation of thin airfoil theory</mark></b>. we also note the original fundamental equation is evaluated at the given point $x$, then we have $\theta_{0}$ correspond to the same point $x$ on the  chord line. 
We also refer to the [[📘ClassNotes/📐Mathmatics/📈Advanced Mathematics/第十二章 无穷级数#五、傅里叶级数|Fourier series expansion]], we just **use a ==cosine series== to represent the function $f(\theta)$ over an interval $0 \leq \theta \leq \pi$**(since we only simulate the function in the interval 0 to $\pi$), then the equation (4.42), the function $\frac{dz}{dx}$ is analogous to $f(\theta)$ and can be expand into cosine Fourier expansion on the left, then we **write the coefficient according to the order of $\cos(n\theta)$ then compare every coefficient, we have**: 
$$A_{0} =\alpha -  \frac{1}{\pi} \int_{0}^{\pi} \frac{dz}{dx} d\theta_{0}\tag{4.50}$$
which gives the calculation of $A_0$. and the coefficient $A_n$ as :
$$\Large \boxed{A_{n} = \frac{2}{\pi} \int_{0}^{\pi} \frac{dz}{dx} \cos n \theta_{0} d \theta_{0}}\tag{4.51}$$
In this case, we obtain the liftin coefficient for a cambered airfoil. Then the circulation is calculated as: 
$$\Gamma = \int_{0}^{c} \gamma(\xi) d\xi  =\frac{c}{2} \int_{0}^{\pi} r(\theta) \sin \theta d\theta$$
then we obtain the circulation around a cambered airfoil as : 
$$\Gamma = cV_{\infty} \left[ A_{0} \int_{0}^{\pi} (1+ \cos \theta) d\theta +\sum^{\infty}_{n=1} A_{n} \int_{0}^{\pi} \sin n \theta \sin \theta d\theta \right]$$
by [[Excalidraw/1-8 Introduction and overall  Analysis 2023-09-06 21.42.44|derivation process]] we can derive the **expression of total circulation :**
$$\Gamma = c V_{\infty}\pi \left(  A_{0} + \frac{A_{1}}{2} \right) \tag{4.54}$$
then the lift per unit span is calculated as : 
$$\Large L' =  \rho_{\infty} V_{\infty} \Gamma  = \rho V_{\infty}^{2} c\left(\pi A_{0} + \frac{\pi}{2} A_{1}\right)\tag{4.55}$$
we know $c_l$ is equal to the $2 \pi A_{0} + \pi A_{1}$, then we <mark style="background: transparent; color: red">substitute the equation (4.51) and the equation (4.50) </mark>into this equation, then we have: 
$$\Large \boxed {c_{l} = 2 \pi  \left[\alpha + \frac{1}{\pi } \int_{0}^{\pi} \frac{dz}{dx} (\cos \theta_{0} - 1 ) d\theta_{0}\right]}\tag{4.57}$$
and the lift slope is calculated as 
$$\boxed{\text{Lift Slope} = \frac{d c_{l}}{d \alpha} = 2 \pi}\tag{4.58}$$
we note this result is a general result from thin airfoil theory that $\frac{dc_l}{d\alpha} = 2\pi$, but the expression for $c_{l}$ itself  <mark style="background: transparent; color: red">differs between a symmetric airfoil to a cambered airfoil</mark>, we note that this difference being the integral term in the equation (4.57). **The angle of zero lift is denoted by $\alpha_{L = 0}$.  Which is a negative value**.

We know $c_{l}$ can be calculated as :
$$c_{l} = 0 + \frac{d c_{l}}{d\alpha }(\alpha - \alpha_{L = 0}) =2 \pi (\alpha - \alpha_{L = 0})$$
the figure of $c_l$ can be referenced from the [[#4.3 Airfoil Characteristics]]. We also have 
$$\boxed{\alpha_{L = 0} = - \frac{1}{\pi} \int_{0}^{\pi} \frac{dz}{dx} (\cos\theta_{0} -1) d\theta_{0}\tag{4.61}}$$
we also calculate **the moment about the leading edge**, which is shown in [[#4.7 Thin Airfoil Theory The Symmetric airfoil|figure (2)]] can be **obtained by substituting $\gamma(\theta)$ from equation (4.43) into the equation (3.35)** then we just give the integration of it as : 
$$c_{m,le} = - \frac{\pi}{2} \left(A_{0} + A_{1} - \frac{A_{2}}{2}\right)$$
`````ad-todo 
title: The derivation process is at problem 4.9  
collapse: open
`````
considering the equation lifting coefficient is given above as $2 \pi A_{0} + \pi A_{1}$, we have : 
$$\Large \boxed{c_{m,le} = -\left[ \frac{c_{l}}{4}  +  \frac{\pi}{4} (A_{1} - A_{2})\right]}\tag{4.63}$$
then we use [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Chapter1 Introductory Thoughts/1-11 Some Introductory Thoughts#1.6 Center of pressure|equation (1.22)]] then get the equation, which give the <b><mark style="background: transparent; color: blue">quarter chord point is the theoretical location of the aerodynamic center for a camber airfoil.</mark></b> (this is not equal to 0 as the symmetric airfoil). 
$$\Large\boxed{c_{m, \frac{c}{4}} = \frac{\pi}{4} (A_{2} - A_{1})}$$

we also gives the location of center of pressure as :
$$x_{cp} =  \frac{M'_{LE}}{L'} = - \frac{c_{m} q_{\infty} c^{2}}{ c_{l} q_{\infty} c} = - \frac{c_{m} c}{c_{l}}$$
then from equation (4.63) we can easily derive the 
$$\boxed{x_{cp} = \frac{c}{4} \left[ 1 + \frac{\pi }{c_{l}} (A_{1}  - A_{2})\right]}\tag{4.66}$$
This equation gives the center of pressure. We note <u>when the lift approaches zero, the distance between the center of pressure and leading edge become infinite.</u> hence <mark style="background: transparent; color: red">we use the aerodynamic center rather than pressure center to analyze an aerodynamic problem</mark>. 
