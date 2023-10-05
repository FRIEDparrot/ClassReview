## 1.12 Applied Aerodynamics : The Aerodynamic coefficients
##### Basic Concepts 
The applied aerodynamics implies the application of the aerodynamics.
In this section we address the aerodynamic coefficients defined in [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 1 Essential Concepts/Chapter1 Introductory Thoughts/1-11 Some Introductory Thoughts#1.5 Aerodynamic Forces and moments|Section1-5]]

for an incompressible flow, the Mach number is theoretically zero, not because the velocity goes to zero, but rather because the speed of sound is infinitely large. Thus, for a low-speed flow, **the aerodynamic coefficients for a fixed shape at a fixed orientation to the flow are functions of ==just the Reynolds number==**.
$$Re = \frac{\rho_\infty V_\infty d}{\mu_\infty} $$
the flow fields over the configurations shows a weak downstream of the body. Which is caused by the flow separating from the body surface, with a low-energy, recirculating flow inside the wake.
![[Pasted image 20230124202702.png|400]]
Note in particular that the physical effect of the streamlining in Figure 1.54c results in a very small wake, hence a small value for the drag coefficient.

For the large streamlined body in Figure 1.54c,
$$D' = q_∞SC_D = 0.12q_∞d$$
For the small circular cylinder in Figure 1.54d,
$$D' = q_∞SC_D = q_∞(0.1d)(1.2) = 0.12q_∞d$$
and they are exactly the same.

Also note that the $C_D$ decrease precipitously at the higher Reynolds number. the answer of this would be shown in section 3.18

Note the values of $C_D$ for the *aerodynamic shapes* in $Figure 1.54$. $C_D$ is based on the ***frontal projected area (S = d(1) per unit span)***, and the values of $C_D$ range from a maximum of 2 to numbers as low as 0.12 is typical values of $C_D$ in aerodynamic bodies.

Note that the **drag of the vertical flat plate and the circular cylinders is dominated by pressure drag**, whereas, in contrast, **most of the drag of the streamlined body is due to skin friction**. Indeed, this type of comparison leads to the definition of two *generic body shapes* in aerodynamics, as: 
$$\text{Blunt Body} = \text{a body where most of the drag is pressure drag}$$
$$\text{Stramlined Body} = \text{a body where most of the drag is skin friction drag}$$
for example, in the figure above, the plate and the cylinder are blunt body.
and in the figure below, the skin friction drag and the pressure drag are indicated in the bar charts. 
![[Pasted image 20230125142429.png|400]]
note that the large pressure drag of the blunt bodies is due to the *massive regions of the flow separation* 

We now examine the drag on a **plate at zero angle of attack**, as sketched in the figure below, the *drag is complete due to the shear stress(no pressure in this direction)*. 

##### Impact of the Reynolds number on the drag coefficient
The ***Skin Friction Drag Coefficient***is defined as:
$$C_f = \frac {D'}{q_{\infty} S}=\frac{D'}{q_{\infty}c(1)}$$
and will be further discussed in Chapter 16, here we show the relationship between the $C_f$ and the Reynolds Number
![[Pasted image 20230125142542.png|450]]
![[Pasted image 20230125145931.png|250]]
1. $C_f$ is a  strong function of $Re$ which is based on the chord length, and in that case $C_f$ decreases as the $Re$ increase. 
2. also the value of the $C_f$ depends on the flow over the plate(laminar flow or the turbulent flow). And for the turbulent the $C_f$ is higher than the 
3. The magnitudes of the $C_f$ ranges typically from 0.001 to 0.01 over a large range of $Re$ 

also note that the typical airfoil drag coefficient is ranged from 0.004 and 0.006, which is 
$$c_d = \frac{D'}{q_\infty c}$$
where $D'$ is the drag per unit span.
typical airfoil drag-coefficient is *on the order of 0.004 to 0.006*
$c_d$ increase with increasing $\alpha$ in the Fig above because the flow separation 

**For a full airplane**,  also we have 
$$C_D = \frac{D}{q_\infty S}$$
where $D$ is the drag of the airplane.  For a full airplane in full operational condition is typically 0. 0275

in that case,  the $C_D$ is a function of the Reynolds Number. which is shown in the expression of it. 
[[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/♎Important Conclusions and Lists/Important Variables and Numbers]]

##### Impact of the Mach number on the Drag coefficient

Since the $C_D$ increase sharply with the generation of the turbulent,  we have that the $C_D$ increase sharply at the Mach number 0. 86 in the following Fig. 
![[Pasted image 20230125151122.png|300]]
also,  the variation of the lift coefficient versus the attack angle is shown in the fig below. we can see the $c_l$ increase linearly with the $\alpha$ but it dropped after $\alpha$ is greater than $14\degree$
![[Pasted image 20230125151209.png|270]]
for an airfoil l,  the $c_l$ is about a factor 100 times larger than $c_d$
Also,  for the flight performance, the ***ratio of the lift and Drag(or so-called $L/D$ ratio)*** is a particularly important figure. so in design process we should consider the $L/D$ coefficient for a whole plane.

The variation of the lift coefficient $C_L$ with the attack angle $\alpha$ is shown in the Fig below:
![[Pasted image 20230125151425.png|280]]
![[Pasted image 20230125152026.png|300]]
the moment coefficient for the airfoil is a *negative* quality. 
all conventional airfoils produce negative, or "pitch down" moments.
