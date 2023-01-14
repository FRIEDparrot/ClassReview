Default model input parameters: 
![[Pasted image 20230114143429.png|500]]
Relevant parameters: 
![[Pasted image 20230114143059.png|500]]
where $k_a$ is the reaction rate and set to 0.25
The object of the topology optimization is to minimize: 
![[Pasted image 20230114143230.png|500]]
where r is the $\phi$ in  the calculation example, and the 
where $\varepsilon$ denotes the volume fraction of solid catalyst
![[Pasted image 20230114143521.png|500]]
dtopo1.theta is
![[Pasted image 20230114144208.png|500]]
the Reaction Rates at the end of the reaction is set to $-\phi$
![[Pasted image 20230114144834.png|200]]
The optimization problem is: 
![[Pasted image 20230114145419.png|400]]
![[Pasted image 20230114145438.png|400]]
(first equation is NS function)
![[Pasted image 20230114145706.png|500]]
the variable $obj$ is  the parameter for minimization: 
![[Pasted image 20230114150524.png|500]]
we set  the optimization target at the optimization module on the optimization step under the study1:
![[Pasted image 20230114150404.png|250]]
we can also use the MMA method 
