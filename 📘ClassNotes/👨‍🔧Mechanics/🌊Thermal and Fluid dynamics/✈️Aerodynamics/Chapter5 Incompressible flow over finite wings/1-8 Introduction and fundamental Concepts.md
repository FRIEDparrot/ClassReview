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
We review the [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Chapter4 Incompressible flow/1-8 Introduction and overall  Analysis#4.4 The Vortex Sheet|Concept of Vortex Sheet]], and expand the concept of a vortex filament. we discuss the straight vortex filament extending to $\pm \infty$. This concept is extended below.
![[Excalidraw/1-8 Introduction and fundamental Concepts 2023-09-18 21.48.07|650]]
In general, a vortex filament <u>can be curved</u>, note only a portion of the filament is illustrated in the figure. This filament induces a flow field in the surrounding space. And <u>If the circulation is taken about any path enclosing the filament, we can reach a constant value</u>. We consider a directed segment of the filament $dl$ as shown in Figure. Note that the raius from $dl$ to an arbitary point $P$ is $r$, then this segment induces a velocity $dV$ as: 
$$\Large\boxed{dV = \frac{\Gamma}{4 \pi} \frac{\boldsymbol{dl \times r}}{|r|^{3}}}$$
this equation is called <mark style="background: transparent; color: red">Bitot-Savart law</mark> which is the most fundamental relation in the theory of inviscid and incompressible flow. 

`````ad-quote
title: Bitot-Savart law in Electromagnetics
collapse: open
The Bitot-Savart law is also extensively used in Electromagnetics, which gives the theory of the electronic field, which ==gives the magnetic field strength caused by a wire carring an electronical current== $I$ as : 
$$dB = \frac{\mu l}{4 \pi} \frac{dl \times  r}{|r|^{3}}$$

Indeed, the Bitot-Savart law is <mark style="background: transparent; color: red">a general result of potential theory</mark>. 
`````

We give that <u>when a number of fliaments are used in conjunction with a uniform freestream</u>, Then it's possible to synthesize a flow which has a practical application. Then we apply the Bitot-Savart law to a straight vortex filament of infinite length, and 

