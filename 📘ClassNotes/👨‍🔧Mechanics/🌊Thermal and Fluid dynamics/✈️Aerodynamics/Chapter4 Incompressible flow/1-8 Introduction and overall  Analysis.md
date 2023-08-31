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

we review [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Chapter1 Introductory Thoughts/1-11 Some Introductory Thoughts#1.5 Aerodynamic Forces and moments|section 1.5]], We also note the **quarter- chord point is commonly used**.

The drag of airfoil is <u>due to both body friction drag and pressure drag due to flow separation</u>. The sum of two effects yields the profile drag coefficient $c_d$ for the airfoil. We also note $c_{d}$ is sensitive to $\text{Re}$ 
![[Excalidraw/1-8 Introduction and overall  Analysis 2023-08-31 14.43.23|1000]]
On the airfoil there is **one point which the moment is independent of angle of attack**, this point is defined as the ***aerodynamic center***. 

