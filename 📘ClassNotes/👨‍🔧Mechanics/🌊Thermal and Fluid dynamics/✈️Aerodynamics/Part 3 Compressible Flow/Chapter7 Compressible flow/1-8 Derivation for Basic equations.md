## 7.1 Introduction and Basic Thermodynamic Equations 
### (1) Introduction 
In contrast to the low-speed, incompressible flow, the **pivotal aspect of high-speed flow is that the density is a variable**, hence it is called *compressible flows*. another pivotal aspect of high-speed compressible flow is **energy**. 

when the velocity of flow decrease, **the internal energy of the gas increases**. Hence the temperature of the gas increases. Which leads the energy and temperature change are important considerations. 

We know from [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/♨️Engineering thermodynamics/第三章 理想气体的性质与过程|Engineering thermodynamics]] that the important equations are : 
$$pV = m RT\rightarrow p =  \rho RT\tag{7.1}$$
where R is the <u>specific gas constant</u>. also, if we use the specific volume : 
$$pv = RT\tag{7.2}$$
also, we know the enthalpy and entropy are also the important concepts from [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/♨️Engineering thermodynamics/第二章 热力学第一定律#(3) 焓的概念|the concepts of enthalpy]], this is <u>the sum of the energies of all the molecules in the volume</u>. denoted by $h$ as:
$$h =  e + pv\tag{7.3}$$
For a perfect gas, [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/♨️Engineering thermodynamics/第三章 理想气体的性质与过程#(1) 理想气体内能的计算|the calculation of internal energy]] and [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/♨️Engineering thermodynamics/第三章 理想气体的性质与过程#(2) 理想气体的焓|the calculation of enthalpy]] are:
$$de = c_{v} dT\qquad dh = c_{p} dT$$
so we can define : 
$$\Large \boxed{ \begin{matrix}
e = c_{v} T \\ h = c_{p} T \end{matrix} }\tag{7.6}$$
where $c_p$ and $c_{v}$ are [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/♨️Engineering thermodynamics/第三章 理想气体的性质与过程#(2) 比定容热容和比定压热容|specific heat capacity for gas at constant pressure and constant volume]], the relationships are:
$$c_{p}- c_{V} = R$$
$$\boxed{c_{p}  = \frac{\gamma R}{\gamma - 1}} \quad \boxed{c_{p}= \frac{R}{\gamma - 1}}\tag{7.9}$$
note $\gamma =\frac{c_{p}}{c_{v,m}} = \frac{7}{5}, c_{p,m} = \frac{7}{2} R_{m}, c_{v} = \frac{5}{2}R_{m}$ (referencing [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/♨️Engineering thermodynamics/第三章 理想气体的性质与过程#(3) 理想气体比热容与温度的关系|calculation of specific heat capacity]]) for air $R = 287.1 J/(kg\cdot K)$, $m$ is for unit mass (If calculate total change of internal energy we should multiply mass). 

### (2) First Law of Thermodynamics
The concept of *surroundings*, *system* are introduced in the [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/♨️Engineering thermodynamics/第一章 基本概念#一、热力系统|thermodynamic system]]. and also we can review [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/♨️Engineering thermodynamics/第二章 热力学第一定律#一、热力学第一定律|the first law of thermodynamics]] as : 
$$\delta q = \delta e  + \delta w$$
where $\delta w$ is the work done by the system to the surroundings. But we use the *work done by the surroundings to the system* here, hence $\delta W = -p d v$ (different from the enginnering thermodynamics), we got: 
$$\Large\boxed {\delta e = \delta w + \delta q =  \delta q - p d v}\tag{7.11}$$
### (3) Second Law of Thermodynamics
The definition of entropy can be referenced from [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/♨️Engineering thermodynamics/第四章 热力学第二定律与熵|engineering thermodynamics]] as :
$$ds = \frac{\delta q_{rev}}{T}$$
$\delta q_{rev}$ is a <u>incremental amount of heat added reversibly to the system</u>. we also note entropy $s$ is a state parameter. An alternative and probably more lucid relation: 
$$\Large\boxed{ds = \frac{\delta q}{T} + ds_{irrev}}\tag{7.14}$$
for adiabatic system, $ds \geq 0$ since $\delta q =0$, then we can substitute the $\delta q = T ds$, then got :
$$\Large \boxed{\delta q = T ds = \delta e + p dv}\tag{7.18}$$
since $h =  e + pv$ , we also have :
$$\Large\boxed{\delta q = T ds = \delta h - v d p}\tag{7.20}$$
also we can just reference the [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/♨️Engineering thermodynamics/第四章 热力学第二定律与熵#1. 理想气体的熵变计算|calculation of the enthalpy of ideal gas]] and [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/♨️Engineering thermodynamics/第三章 理想气体的性质与过程#四、理想气体的熵|definition of enthalpy]]
$$\Large\boxed{s_{2} - s_{1} =  \begin{cases}
c_{p} \ln \frac{T_{2}}{T_{1}} - R \ln \frac{p_{2}}{p_{1}}  \\
 c_{v} \ln \frac{T_{2}}{T_{1}} + R \ln \frac{v_{2}}{v_{1}}  \\
 c_{v} \ln \frac{p_{2}}{p_{1}} + c_{p}\ln \frac{v_{2}}{v_{1}}
\end{cases}}$$
### (4) Isentropic Relations 
we also have  [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/♨️Engineering thermodynamics/第四章 热力学第二定律与熵#1. 理想气体的熵变计算|calculation of the enthalpy of ideal gas]]: $s_{2} - s_{1} = 0 \rightarrow  \frac{c_{p}}{R}  = \frac{\gamma}{\gamma - 1}$, then  
**for Isentropic Process we can easily derive**: 
$$\boxed{\frac{p_{2}}{p_{1}} = \left(\frac{\rho_{2}}{\rho_{1}}\right)^{\gamma}= \left(\frac{T_{2}}{T_{1}}\right)^{\frac{\gamma}{\gamma -1}}}$$
$$\boxed{\frac{v_{2}}{v_{1}} = \frac{\rho_{1}}{\rho_{2}}  = \left(\frac{T_{2}}{T_{1}}\right)^{- \frac{1}{\gamma- 1}}}$$
we note that for a large number of flows can be assumed isentropic. Such as airfoil surface, rocket nozzle walls, the dissipative effects of viscosity can be neglected and assumed as an adiabatic process. 

## 7.2 Definition of Compressibility 
The amount by which a substance can be compressed is given by compressibility. Which is defined as the reverse number of the decrease of volume with the increase of pressure : 
$$\Large\boxed{\tau = - \frac{1}{v} \frac{dv}{dp}}\tag{7.33}$$
In another words, the compressibility is <mark style="background: transparent; color: red">the fractional change in volume of the fluid element per unit change in pressure</mark>. If the surroundings held a constant temperature, this is called <mark style="background: transparent; color: red">isothermal compressibility</mark> $\tau_{T}$. Similarly we define <mark style="background: transparent; color: red">isentropic compressibility</mark> $\tau_{s}$.
$$\boxed{\tau_{T} = - \frac{1}{v}\left(\frac{dv}{dp}\right)_{T} \qquad \tau_{s} = - \frac{1}{v} \left(\frac{dv}{dp}\right)_{s}}\tag{7.35}$$
we know $v = \frac{1}{\rho}, dv = -\frac{1}{\rho^{2}}$, then we can get 
$$\tau = \frac{1}{\rho} \frac{d\rho}{dp}\rightarrow d\rho = \rho \tau dp\tag{7.37}$$
Hence for the low-speed flow, we assume $dp$ is small, then the flow would be incompressible. Also the subsonic flow and supersonic flow is distinguished by Mach number, for $M > 0.3$ the gas is assumed to be compressible. 
$$M \equiv \frac{V}{a}$$
## 7.3 Governing Equations for inviscid, Compressible flow 
For incompressible flow, it obeys purely mechanical laws and doesn't require thermodynamic considerations. We reference from [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 1 Essential Concepts/Chapter2 Fundamental Principles and Equations/1-8 Fundamental Principles|Chapter 2]] and 
1. Continuity Equation (both integral  and partial differential form): 
$$\Large\boxed{\frac{\partial }{\partial t}∰_V\rho dV + ∯_S \rho \vec{V} \cdot d\vec{S} = 0}$$
$$\boxed{\Large \frac{\partial \rho}{\partial t} + \nabla \cdot (\rho V) = 0}$$
2. Moment Equation (N-S Equation)
[[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 1 Essential Concepts/Chapter2 Fundamental Principles and Equations/1-8 Fundamental Principles#(1) The momentum equation in the integral form|the integral form of momentum equation]] : 
$$\boxed{\frac{\partial}{\partial t}\left(\iiint_V (\rho V)  d\mathcal{V}\right) + ∯_S (\rho V \cdot dS) \space V =  -∯_{S} p dS + ∰_V \rho f d\mathcal{V}}$$
or in [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 1 Essential Concepts/Chapter2 Fundamental Principles and Equations/1-8 Fundamental Principles#(2) partial differential equation of the momentum formula|the partial differential form]]: 
$$\boxed{\frac{D(\rho V)}{Dt} =  \frac{\partial (\rho V)}{\partial t} + (\nabla \cdot  V)\rho V = - \nabla \cdot p + \rho f}$$
3. [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 1 Essential Concepts/Chapter2 Fundamental Principles and Equations/1-8 Fundamental Principles#(3) Total Energy Change in the control volume|Energy Equation]]: 
$$\boxed{\frac{\partial }{\partial t}∰ \rho (e + \frac{V^{2}}{2}) d\mathcal{V} + ∯_{S} (\rho V ) \left(e + \frac{V^{2}}{2}\right) \cdot dS = ∰_V \dot{q} \rho d\mathcal{V}  -∯_S pV\cdot dS + ∰_V \rho (f\cdot V) d\mathcal{V}} \tag{7.43}$$
and in the partial differential form : 
$$\boxed{\frac{\partial}{\partial t}\left[\rho (e + \frac{V^2}{2})\right] + \nabla\cdot \left[\rho (e+ \frac{V^2}{2})V\right] = \rho\dot{q} - \nabla \cdot(pV) + \rho (f\cdot V)  }$$
we note equation of state $p = \rho R T$ and internal energy as $e = c_{v} T$ 
## 7.4 Definition of Stagnation Conditions
The concept of the **static pressure** $p$ was mentioned in some detail in [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 1 Essential Concepts/Chapter3 Fundamentals  of Invisid and Incompressible Flow/1-7 Introduction, Concepts and basic theorems#3.4 Pitot Tube|Pitot Tube]], and the stagnation pressure exists at a point where $V=0$. We define <mark style="background: transparent; color: red">the value of temperature of the fluid element after it has been brought to rest adiabatically</mark> as the **total temperature** $T_0$. Then the corresponding value of enthalpy is defined as **total enthalpy** $h_{0}$. We note $h_{0} = c_{p} T_{0}$. 
Firstly, we assume that the flow is adiabatic ($q = 0$), and the body force can be neglected, hence : 
$$\rho \frac{D(e + \frac{V^{2}}{2})}{Dt} = -  \nabla \cdot pV\tag{7.45}$$
we have the expansion of the right part (note p is a scalar and V is a vector), then: 
$$\boxed{\nabla \cdot pV =p \nabla \cdot V + V\cdot \nabla   p}$$
we note that the substantial derivate can also be expanded by <mark style="background: transparent; color: red">normal laws of differentiation</mark>: 
$$\small \rho \frac{D\left(p/\rho\right)}{Dt} =\rho \frac{\rho \frac{Dp}{Dt}- p\frac{D\rho}{Dt}}{\rho^{2}} = \frac{Dp}{Dt} - \frac{p}{\rho} \frac{D\rho}{Dt}$$
We have the <u>continuity equation</u> (with reference to [[📘ClassNotes/👨‍🔧Mechanics/🌊Thermal and Fluid dynamics/✈️Aerodynamics/Part 1 Essential Concepts/Chapter2 Fundamental Principles and Equations/9-16 The Application Concepts#2.9 Substantial derivative|Substantial derivative]]) 
$$\frac{D \rho}{Dt} = \frac{\partial \rho}{\partial t} + V \cdot \nabla \rho= - \rho \nabla \cdot V\rightarrow \frac{D \rho}{Dt} +\rho \nabla \cdot V =0$$
then we can use [[Excalidraw/1-8 Derivation for Basic equations 2023-10-03 18.19.16|simple derivation]], we finally obtain: 
$$\rho \frac{D (h + \frac{V^{2}}{2})}{Dt} = \frac{\partial p}{\partial t}$$
for the steady flow, we have $\frac{\partial p}{\partial t} = 0$, then for an adiabatic flow, we have relationship: 
$$\Large\boxed{h + \frac{V^{2}}{2} = \text{const} = h_{0}}$$
For velocity $V = 0$, we have $h = h_{0}$(defined as <mark style="background: transparent; color: red">total enthalpy</mark>)
`````ad-caution
this relationship($h_{0} = \text{const}$ <mark style="background: transparent; color: red">throughout the entire flow</mark>) only works when the flow is steady and adiabatic.
Also, if all the streamlines of the flow **originate from a common uniform freestream**, then $h_{0}$ is the same for each streamline. 
`````

For a calorically perfect gas, $h_{0} = c_{p} T_{0}$, then the above results also states the **total temperature** is a constant:
$$\boxed{\Large T_{0} = \text{const}}\tag{7.56}$$
<b><mark style="background: transparent; color: blue">This equation can be used as a form of the governing energy equation</mark></b>. 
We image us to slow down the velocity of the flow <mark style="background: transparent; color: red">both adiabatically and reversibly</mark>. And also define the resulting pressure and density as **total density** $\rho_{0}$ and **total pressure** $p_0$. Since the flow is Isentropic, we have(for isentropic flow): 
$$\Large\boxed{p_{0} = \text{const} \qquad \rho_{0}= \text{const} }$$
We also define another temperature, for a subsonic flow where static temperature $T_{0}$ and speed up to sonic adiabatically, <mark style="background: transparent; color: red">the temperature it would have at sonic speed</mark> is denoted as $T^{*}$, We note $a^{*} = \sqrt{\gamma RT^{*}}$.
