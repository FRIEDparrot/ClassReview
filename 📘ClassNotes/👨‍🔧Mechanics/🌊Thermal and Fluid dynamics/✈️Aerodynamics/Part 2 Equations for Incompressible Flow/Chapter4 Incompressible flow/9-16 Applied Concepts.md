## 4.9 Aerodynamic Center 
We have given the definition of aerodynamic center in section [[#4.3 Airfoil Characteristics]] which is defined as the point where **the moment is independent of angle of attack**. 

With the assumptions embodied in [[#4.7 Thin Airfoil Theory The Symmetric airfoil|the theory of thin airfoil]], the aerodynamic center for a thin airfoil is located at the quarter chord point. We also set $x_{ac}$ as the the location of aerodynamic center (or $c\bar{x}_{ac}$). Then we have 
$$M'_{ac} = L' \left(c\bar{x}_{ac} - \frac{c}{4}\right)+ \frac{M'_{\frac{c}{4}}}{q_{\infty} S c}$$
![[Excalidraw/1-8 Introduction and overall  Analysis 2023-09-07 18.54.16|300]]
so we have : 
$$c_{m, ac} = c_{l} (\bar{x}_{ac} - 0.25) + c_{m, \frac{c}{4}}\tag{4.68}$$
we difference this equation with respect to angle $\alpha$. Then **since the point is aerodynamic center**,we have $\frac{d c_{m, ac}}{d \alpha} = 0$, Then the equation becomes: 
$$0 = \frac{dc_{l}}{d\alpha}\left(\bar{x}_{ac} - 0.25\right) + \frac{dc_{m, \frac{c}{4}}}{d\alpha}$$
and we know for the airfoils below the stall angle of attack, the **slopes of the lift coefficient** 
is a constant, then Designating these slopes by : 
$$\boxed {\frac{dc_{l}}{d \alpha} \equiv a_{0} \qquad  \frac{d c_{m}}{d\alpha}\equiv m_{0}}$$
so we get : **For a body with linear lift and moment curves, the aerodynamic center exists as a fixed point on the airfoil**. 
$$\Large \boxed{\bar{x}_{ac} = -\frac{m_{0}}{a_{0}} +0.25}$$
## 4.10 Airfoil Drag for Viscous flow 
We know for an airfoil the lift is primarily due to the pressure distribution exerted on the surface of it. When integrated <u>in the lift direction, the shear stress is usually negligible</u>. The lift can be accurately calculated assuming inviscid flow in  conjunction with the Kutta condition. But this same approach yields zero drag, which is called **d'Alembert's paradox**. 

The paradox is removed when the friction is included in the flow, we note that **the viscosity in the flow is responsible for the aerodynamic drag on the airfoil** through : 
1. **Skin- friction drag** 
2. **Pressure Drag** due to <u>flow separation</u>, which called **form drag**.
![[Excalidraw/1-8 Introduction and overall  Analysis 2023-09-09 15.10.32|400]]
### (1) Skin Friction Drag for laminar flow
For the viscid low-speed incompressible flow, the boundary layer thickness for incompressible laminar flow over the airfoil is shown in figure below.
![[Excalidraw/1-8 Introduction and overall  Analysis 2023-09-09 15.14.48|800]]
the boundary layer thickness is given as : 
$$\boxed{\delta = \frac{5.0 x}{\sqrt{\text{Re}_{x}}}}$$
where the Reynolds Number is : 
$$\boxed{\text{Re}_{x} = \frac{\rho_{\infty} V_{\infty} x}{\mu_{\infty}}}$$
hence $\delta \propto \sqrt{x}$ `->` boundary layer shape. 

We choose the top surface for integral of the shear stress, and then yields the net friction as $D_f$ on the plate. for a plate we have $D_{f, bottom} = D_{f, top}$, then $D_{f} = 2D_{f, top} =  2D_{f, bottom}$. 
then the **skin-friction for the flow over one surface is defined as :** 
$$C_{f} \equiv  \frac{D_{f, top}}{q_{\infty} S} = \frac{D_{f,bottom}}{q_{\infty} S}$$
the skin friction drag is a function of Reynolds number, which is given as : 
$$C_{f} = \frac{1.328}{\sqrt{\text{Re}_{c}}}\qquad  \text{Re}_{c} = \frac{\rho_{\infty} V_{\infty} c}{\mu_{\infty}}$$
### (2) Skin Friction Drag for turbulent flow
The analysis of the turbulent flow are **approximate**. the analysis of the turbulent boundary layer over a flat plate is no exception. so we give the experimental formula as : 
$$\delta = \frac{0.37 x}{Re_{x}^{\frac{1}{5}}}$$
and the friction coefficients as : 
$$C_{f} = \frac{0.0074}{\text{Re}_{c}^{\frac{1}{5}}}$$
### (3) Transition 
In reality, the flow always starts out from the leading edge as laminar. Then **at some point downstream of leading edge**, the laminar boundary layer becomes turbulent flow. 
![[Excalidraw/1-8 Introduction and overall  Analysis 2023-09-09 15.14.48|800]]
The value of $x$ <u>where transition is said to take place</u> is the **critical value** $x_{cr}$, which allows the definition for the **critical Reynolds number**. (often it is given experimentally)
$$\text{Re}_{x_{cr}} = \frac{\rho_{\infty} V_{\infty} x_{cr}}{\mu_{\infty}}\rightarrow\quad  x_{cr} = \frac{\mu_{\infty} \text{Re}_{x_{cr}}}{\rho_{\infty} V_{\infty}}$$

## 4.11 Flow Separation 
For a <mark style="background: transparent; color: red">completely attached flow</mark> **over** an airfoil, <u>the pressure acting on the rear surface gives rise to a force in the forward direction</u>  while <u>the pressure acting on the front surface produce force in rearward direction</u>. Which completely counteracts and **result in zero drag**. (if the trailing edge is the same height as leading edge, the area projection are the same). 

When the flow separate over the rear surface, the pressure on the rear surface will be smaller (since the flow rapidly expands over the top surface). 

note that [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/♎Important Conclusions and Lists/Dimensionless force and moment coefficients|the pressure coefficient]] : 
$$C_p \equiv \frac{p-p_\infty}{q_\infty}$$
![[attachments/Screenshot_20230909_163757_com.microsoft.skydrive.png|500]]
We also have [[attachments/Screenshot_20230909_164121_com.microsoft.skydrive.png|pressure variance caused by flow separation]]. (due to a sharp decrease of the pressure coefficient when the angle of attack is large). Then we have [[attachments/Screenshot_20230909_164645_com.microsoft.skydrive.png|pressure distribution figure]] shown.

We note that the lift for the separated flow<mark style="background: transparent; color: red"> is smaller than the lift that would exist if the flow were attached</mark>. 
`````ad-note
title: the reason of the loss of lift 
collapse: open
When the flow is sparated, causing a higher pressure near the leading edge of the airfoil surface. Also the direction in which the pressure is acting is closely aligned to the vertical. Hence <u>the full effect of the increased pressure is felt by lift</u>. But the <mark style="background: transparent; color: red">combined effect of the increased pressure on the top surface near the leading edge</mark> (which is approximately horizontal) leads to the loss of lift when the flow separates.
`````

And due to the flow separation, the pressure on the back is lower than it would be when flow is attached, which <u>leads to higher drag acting toward the right</u>. Then the major consequence of the flow separating over an airfoil are :
1. loss of lift (stalling)
2. major increase of drag 

## 4.12 The Real Case for Flow over an airfoil 
We show the streamline of the flow around the airfoil for the real case of the airfoil by the figure below :
![[attachments/Screenshot_20230918_133027_com.microsoft.skydrive.png|700]]
We note at a low angle of attack, the stremlines are undistributed from the freestream shapes and $c_l$ is small. $c_l$ increases as $\alpha$ increased. and in the region the inrease is linear. We note when the $\alpha\approx 15\degree$, the flow is still attached over the bottom surface of the airfoil, but as $\alpha$ increased, the flow separation occures, and the airfoil begins to stall(<mark style="background: transparent; color: red">This type of phenomenon is called leading edge stall</mark>, which gives a sharp, precipitous decrease in lift coefficient). 

We give the concept of **Trailing Edge Stall as follows**: from the figure (NACA4421) , we see a progressive and <u>gradual movement of separation from the trailing edge toward the leading edge</u>  as $\alpha$ increased. 
![[attachments/Screenshot_20230918_135548_com.microsoft.skydrive.png|800]]
the Lift Coefficients of the airfoils with LE stall and TE stall are shown in the figure below. 
![[Excalidraw/9-16 Applied Concepts 2023-09-18 14.03.41|500]]
then we give the thin airfoil stall as follows : 
![[attachments/Screenshot_20230918_141252_com.microsoft.skydrive.png|900]]
we note that the thickness of the airfoil have a major impact on obtaining a higher $c_{l,\max}$, but it also determinates the stall form of the airfoil. Also, For a complete flight vehicle,  <mark style="background: transparent; color: red">the maximum lift coefficient determines the stalling speed of aircraft</mark> , the equation is given in the [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 1 Essential Concepts/Chapter1 Introductory Thoughts/Design Utilities/Design Box 1.8.2|Chapter 1]] as : 
$$V_{stall} = \sqrt{\frac{2W}{\rho_{\infty} S C_{L, \max}}}$$
Also, the **lift-drag ratio** $L/D$ and **the maximum lift coefficient** $c_{l,\max}$ are the primarily used to judge the quality of a given airfoil. 

`````ad-note
title: other properties 
collapse: close
we may also use the properties of **leading edge droop**, which can increase the maximum lifting coefficient. 

The properties of **LE droop** and **Kruger flap** are given in p403 of the textbook. 

We give the use of combination of leading-edge slats(flaps) and trailing-edge flaps in mordern wings as follows : 
![[attachments/Screenshot_20230918_145202_com.microsoft.skydrive.png|400]]
note : slat would lead to attached flow on the airfoil surface. 
`````

