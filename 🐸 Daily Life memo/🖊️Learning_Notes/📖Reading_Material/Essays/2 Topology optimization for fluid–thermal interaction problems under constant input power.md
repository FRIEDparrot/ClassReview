[[1-Topology optimization for fluid–thermal interaction problems under constant input power.pdf|article contents]]

- The density-based topology optimization method considering fluid and thermal interactions 

- It is important to maximize the performance of a heat exchanger under a constant input power.

- in the fluid analysis, the inlet pressure is determined by solving the extra integral equation together with the Navier– Stokes equation.

### 1. related topology optimization methods 

-  For the last two decades several optimization methods for topology optimization have been developed:
	1. the homogenization method
	2. Solid Isotropic Material with Penalization (SIMP) method

*thermal field optimization problems, the heat conduction problems governed by the Poisson equation have been well studied because of their relatively simple constitutive equations*

Li et al. (2004) applied the evolutionary structural optimization method to the heat conduction problems.

Bruns proposed the SINH method (Bruns 2005) and applied it to convection dominated heat transfer problems

As for numerical shape optimization for fluid dynamics problems, a comprehensive summary can be found in Mohammadi and Pironneau.

For topology optimization, Borrvall and Petersson (2003) presented a density-based approach to minimizing the power dissipated in the Stokes flow

Pingen and Meyer (2009) applied the lattice Boltzmann method to a topology optimization method for heat transfer applications.

### 2. methods to design a heat exchanger better

**Generally, heat exchangers are designed to maximize performance given a certain amount of input power.**

It is assumed that the fluid flow in the idealized porous medium is parameterized with a fluid fraction which is utilized as the design variable. The distribution of the fluid fraction represents the topology of the solid and fluid domains

The optimization problem is then formulated for the maximization of heat transfer with respect to two types of heat sources: temperature-dependent and temperature-independent heat sources.

the constant input power condition does not appear as an explicit equality constraint; The constant input power condition is, instead, ensured by solving the additional integral equation together with the Navier–Stokes equation and the transport equation

#### 3. the representation of the fluid domain and solid domain

for a fluid fraction, γ ,which varies continuously from zero to one, γ = 0 stands for solid and γ = 1 for fluid
![[Pasted image 20221126220950.png|350]]


### 4. Governing equations

governing equations of the incompressible steady fluid flow: 
(Navier-Stokes equation)
![[Pasted image 20221126221237.png|350]]

where u is the given velocity and the F is a body force caused by the solid objects in the flow(namely, the porous media).Assuming F w is proportional to the fluid velocity, u

$$F = -\alpha u$$
In the following numerical examples we will solve (1) and (2) under a constant input power, P 

In order to ensure constant input power 
![[Pasted image 20221126221917.png|350]]
$p_{in}$ is inlet pressure

Equations (1) and (2) can be transformed into a dimensionless form using dimensionless variables
![[Pasted image 20221126222147.png|300]]
where L and U are the characteristic length and velocity.
and the eq (1) and (2) are rewritten as follows: 
![[Pasted image 20221126222304.png|350]]
Re ≡ ρUL/μ is Reynolds number, α∗(x∗) is the dimensionless inverse permeability which is given by
![[Pasted image 20221126222509.png|150]]
where q is a positive parameter used for tuning the function shape of α'(γ). 
It is desirable that α∗ max is always large enough to keep the significance of the body force term, α∗u∗, higher than the others regardless of the Reynolds number, so $\alpha^*$ is defined as:
![[Pasted image 20221126222931.png|150]]
q and Da are set to 10−2 and 10−4, respectively
the Re based on the inlet velocity depends on the design of the flow channels.

to obtain the optimized results under the constant input power, P, we determine the characteristic velocity U based on the power P: 
$$P = \rho U^3L^2 \tag{11}$$
![[Pasted image 20221126223407.png|280]]

### 5. Thermal field modeling

There are different governing equations for the thermal field in the fluid and solid domains. When the heat source is only placed in the solid domain

When the heat source is only placed in the solid domain, with the specific heat of the fluid, c, and the thermal conductivities of the fluid and solid parts, $k_f$ and $k_s$, the governing equations can be expressed for the temperature, T , as

![[Pasted image 20221126225938.png|350]]

where Q is the heat generation per unit time and volume.

Therefore, one cannot know a priori which equation should be adopted at each point. To solve this issue using the fluid fraction γ , the two equations.

To solve this issue using the fluid fraction γ , the two equations, (14) and (15), are unified into the following single equation which is applicable to both the fluid and solid domains: 
![[Pasted image 20221126230817.png|350]]
where $a \equiv k_f/\rho c$ is the thermal diffusivity of the fluid and $K \equiv k_s/k_f$ is the ratio of the solid and fluid thermal conductivities.
where Q is the heat source

In this work, the equation is further simplified by setting $K = 1$

![[Pasted image 20221126231100.png|200]]
where Tin is the inlet temperature, and TQ is another reference temperature.

#### 5.1 temperature dependent heat-source and temperature independent heat source



