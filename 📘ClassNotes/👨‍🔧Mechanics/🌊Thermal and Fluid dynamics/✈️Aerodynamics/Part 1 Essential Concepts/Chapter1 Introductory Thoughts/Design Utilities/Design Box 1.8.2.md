#### Design method in Drag and Lifting coefficients

consider an airplane in the steady, level flight, and as shown in Fig. For this case, we have the equilibrium relationship as: 
![[Some Introductory Thoughts 2023-01-22 21.41.13.excalidraw|300]]
when the plane is sustain in level flight
$$L = W$$
and for steady(unaccelerated) flight:
$$D = T$$
typically, for conventional cruising flight, $L/D\approx$ 15 to 20

For an airplane of given shape, at given march and Reynolds number, the actual values of $C_L$ and $C_D$ for a given body shape just depend on the Orientation of the body --> which determines the attack angle. The generic variations for $C_L$ and $C_D$ versus $\alpha$ is shown in fig
![[Some Introductory Thoughts 2023-01-22 22.04.32.excalidraw|250]]

the $C_L$ increase linearly with the attack angle $\alpha$ until the wing ***stalls***

the lowest possible velocity at which the airplane can maintain steady level flight is *stalling velocity*, $V_{stall}$, which is dictated by the value of $C_{max}$ by the [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 1 Essential Concepts/Chapter1 Introductory Thoughts/Implements/lowest velocity|link]]

from the definition of the lift coefficient, considering $L = W,$ we have: 
$$C_L = \frac{L}{q_\infty S} =  \frac{2W}{\rho_\infty V^2_\infty S} \tag{1.45}$$
thus we have
$$\boxed{V_\infty = \sqrt{\frac{2W}{\rho_\infty C_L S}}}$$
since the $\rho_\infty, W$ and $S$ are fixed values for a given airplane at a given attitude, the $V_\infty$ corresponds to the specific value of $C_L$, thus when the $C_L$ reaches it's maximum, $V_\infty$ reaches the minimum. Then we have
$$\boxed{V_{stall} = \sqrt{\frac{2W}{\rho_\infty SC_{L,max}}}}$$
and note that $C_L$ is purely defined by nature at an given attitude. Through the physical laws for the aerodynamic flow-field over the plane, the airplane designer has some devices available that can artificially increase the $C_{L,max}$ ---> these are called high-lift devices(flaps, slat and slots, etc.). 
They are used for decreasing the $V_{stall}$ and thus deploy for landing and take-off.

---
also note that the maximum velocity for a given airplane with given maximum trust can be found by $T=D$, then we have 
$$C_D =\frac{D}{q_\infty S} = \frac{T}{q_\infty S} = \frac{2T}{\rho_\infty V_\infty^2 S} \tag{1.48}$$
also we have
$$\boxed{V_\infty = \sqrt{\frac{2T}{\rho_\infty C_D S}}}$$
thus when $C_D$ reaches its minimum, the $V_\infty$ reaches
$$\boxed{V_{\infty,max} = \sqrt{\frac{2T_{max}}{\rho_\infty C_{D,min} S}}}$$
![[Some Introductory Thoughts 2023-01-22 23.20.52.excalidraw|380]]
so from the discussion we clearly know that the aerodynamic coefficients are important -> the design of performance of airplanes is determined on it. the $V_{stall}$ is determined by the $C_{L,max}$ and the $V_{max}$ is determined by the $C_{D,min}$, in the design of the plane the $V_{stall}$ must be reached with the designed wing area and the weight. 

then the discussion into the whole range of the flight velocity for a given plane -> so if the plane wants to fly faster, the angle of attack should be smaller. so at low speed the plane is at low $\alpha$

For designing, to obtain raw lift on a body is relatively easy, so what we focused on is to obtain a necessary lift with as low a drag as possible. -> The value of $C_L$ required *over the entire flight range for an airplane* (can even be obtained for the least effective lifting shape with given angle of attack). 

However the $V_\infty$ can also have an affect on the $C_D$ and $C_L \text{ (not token into consideration now)}$ so we can plot the variation of the $V_\infty$ with $C_D$.  
![[Some Introductory Thoughts 2023-01-23 14.59.40.excalidraw|300]]
as denoted by the curve above, an aerodynamically efficient shape(lower $C_D$), will produce much lower drag and thus reach  higher $V_{max}$

note that ==the true measure of the aerodynamic efficiency is== the ***Lift -Drag Ratio***, which is
$$\frac{L}{D} = \frac{q_\infty S C_L}{q_\infty S C_D} = \frac{C_L}{C_D}$$
in the natural flight, **the value of $C_L$ necessary for flight is determined by the airplane's weight and it's wing area**.
Actually it's is calculated by the ratio of $\Large \frac{W}{S}$ through the relationship given by the equation $(1.45)$ [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 1 Essential Concepts/Chapter1 Introductory Thoughts/Design Utilities/Design Box 1.8.2|Loading coefficients equation]] called the **wing loading**.

The value of $L/D$ at the corresponding velocity is controlled by $C_D$

at any given velocity, we want $L/D$ to be as high as possible. -> the higher is $L/D$, the more aerodynamically efficient is the body. 

The $L/D$ is a function of the $V_\infty$, with the increase of the $V_\infty$, the $L/D$ increase from a low value first and reaches maximum at some intermediate values, then decrease. 

also, since $C_D$ and $C_L$ is given as a function $\alpha$ above, so we can also plot the coefficient of the lift and drag force below:
![[Design Box 1.8.2 2023-01-23 16.00.22.excalidraw|550]]



