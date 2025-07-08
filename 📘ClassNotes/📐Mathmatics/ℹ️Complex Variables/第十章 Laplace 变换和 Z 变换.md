This chapter includes the Fourier Transform, Fourier Series, Laplace transform and Z-transform  

The virtues of employing an input of the form $Ae^{i\omega t}$  are : 
1. Compactness of notation  --> $\alpha cos(\omega t + \phi) + \beta sin(\omega t +  \psi)$ can be represented  by $Re(Ae^{i\omega t})$
2. The fact that the differentiation of the above (sinusoids) function amounts to multiplication by $i\omega$  -> replacing calculus by algebra 
3. The fact the steady-state response of the system to the input will have the form of $e^{i\omega t}$ 
## 1. Fourier Series (Finite Fourier Transform)
#### Fourier Series 
For a decomposition of the $F(t)$, 
$$\omega_n = \frac{2\pi n}{L}$$
[[8. The transforms of Applied Mathmatics 2023-01-09 19.04.40.excalidraw|proof of the Fourier Series]], then we have the Fourier Series: 
$$
F(t) = \sum^{\infty}_{-\infty} c_n e^{i n t} \tag{13}
$$
where
$$c_m = \frac{1}{2\pi} \int^{\pi}_{-\pi}F(t) e^{imt} dt \tag{14}$$
valid whenever the series above is uniformly convergent

**Definition : if F has period $2\pi$ and is integrable over $[-\pi, \pi]$, the  series with coefficients above is  called the Fourier Series for $F$ , and the numbers $c_n$ are called the Fourier  coefficients of $F$**

more **generally**, if $F$ has a period of $L$, then we can <b><mark style="background: transparent; color: orange">define the Fourier series as follows</mark></b>: 
$$F(t) = \sum^{\infty}_{-\infty} c_n e^{-i n\frac{2\pi t}{L}}$$
where
$$c_n = \frac{1}{L} \int^{L/2}_{-L/2} F(t) e^{-i n \frac{2\pi t}{L}} dt$$
then  we  introduce: 
$$F(t) =: f(e^{it})$$
and we can derive the equation of the Fourier transform from the following steps:
[[8. The transforms of Applied Mathmatics 2023-01-09 20.22.54.excalidraw|the derivation of the relationship with Laurent Series]]

#### the Poisson Formula(????)
we want to  solve the Dirichlet problem for the unit disk and $U(\theta)$ has a Fourier expansion: 
$$U(\theta) = \sum^{\infty}_{n = -\infty}\left[ \frac{1}{2 \pi} \int^{\pi}_{-\pi} U(\phi) e^{-in\phi} d\phi\right] e^{in\phi}$$
where we insert 
$$c_m = \frac{1}{2\pi}\int^{\pi}_{-\pi} F(t) e^{-imt} dt$$
thus
$$U(\theta) = \frac{1}{2\pi} \int^{\infty}_{-\infty} U(\phi) d\phi + 2\sum^{\infty}_{n = 1} \frac{1}{2\pi} \int^{\infty}_{-\infty} U(\phi)\cos n(\theta - \phi) d\phi$$
Now we we a device known as Abel-Poisson summation to sum the series and we have: 
$$g(r,\theta) := \frac{1}{2\pi} \int^{\infty}_{-\infty} U(\phi) d\phi + \frac{1}{\pi}\sum^{\infty}_{n = 1} \int^{\infty}_{-\infty} U(\phi) r^n \cos n(\theta - \phi) d\phi$$
with the equality
$$1 + 2\sum^{\infty}_{n=1} r^n\cos n(\theta -\phi) = \frac{ 1 - r^2}{1 - 2r \cos(\theta -\phi) + r^2}$$
we can finally arrive the Poisson formula: 
the Poisson formula ( the detailed derivation can be referenced from the textbook p451)
$$u\left(re^{i\theta}\right) = \frac{1 - r^2}{2 \pi} \int^{\pi}_{-\pi} \frac{U(\phi)}{1 - 2r\cos(\theta - \phi) + r^2} d\phi$$

#### Relevant Theorems
**Theorem 1. If the integral  $\int^{\pi}_{-\pi}|F(t)|^2$ exists, then the Fourier series defined by the above equations exists and it  converges to F in the mean square sense**
$$\lim_{N\to \infty} \int^{\pi}_{-\pi} \left| F(t) - \sum^{N}_{n = -N} c_n e^{int}  \right|^2 dt  = 0$$
**Theorem 2. Suppose that $F$ is Periodic and Piecewise smooth, then the Fourier Series For the converges to $\frac{1}{2}[F(\tau_{j+} + F(\tau_{j-})]$ at the point of discontinuity**

**Theorem 3. Suppose the Fourier expansion is valid and the derived series converges on $[-\pi, \pi]$ Then**
$$\sum^{\infty}_{n = -\infty} inc_n e^{int} = \frac{d}{dt}\sum^{\infty}_{n = -\infty} c_n e^{int}$$
#### The Fourier transform
Suppose that F has a convergent Fourier series expansion:
[[8. The transforms of Applied Mathmatics 2023-01-10 12.38.47.excalidraw|brief solution of the sin\theta]]

We move on to the decomposing arbitrary functions into sinusoids, 
we firstly assume that we are given a nonperiodic function $F(t)$, and $-\infty< t < \infty$, and continuously differentiable. Then if we pick an interval of the form $(-\frac{L}{2}, \frac{L}{2})$,we can represent $F(t)$ by 
$$F(t) =  \sum^{\infty}_{n = -\infty} c_n e^{in \frac{2\pi t}{L}}, \qquad  -\frac{L}{2} < t < \frac{L}{2}$$
where 
$$c_n = \frac{1}{L} \int^{\frac{L}{2}}_{-\frac{L}{2}} F(t) e^{in\frac{-2\pi t}{L}} dt$$
so when $L \to \infty$
we define 
$$g_n = \frac{c_nL}{2\pi} = \frac{1}{2\pi}\int^{\frac{L}{2}}_{-\frac{L}{2}} F(t) e^{-in\frac{2\pi t}{L}} dt$$since we use the $n^{th}$ L in the function,  we introduce the factor $[(n+1)-n]\equiv 1$ into the equation, and
$$F_L(t) = \sum^{\infty}_{-\infty} g_n e^{in \frac{2\pi t}{L}} \frac{2\pi  [(n+1) - n]}{L}$$
so we rewrite the $\omega_n = \frac{n 2\pi}{L}$, producing: 
$$F_L(t) = \sum^{\infty}_{n = -\infty} G_L(\omega_n) e^{i\omega_n t}(\omega_{n+1} - \omega_n) \tag{5}$$
and
$$G_L(\omega) := \frac{1}{2\pi} \int^{\frac{L}{2}}_{-\frac{L}{2}} F(t) e^{-i\omega t} dt$$
so we define
$$\boxed{G_L(\omega) := \frac{1}{2\pi} \int^{\frac{L}{2}}_{-\frac{L}{2}} F(t) e^{-i\omega t} dt} $$
as  the *Fourier Transform* of $F$, and the $F(t)$ become a Riemann sum: 
$$ \boxed{ F(t) = \int^{\infty}_{-\infty}G(\omega) e^{i\omega t}d\omega}$$
called the *Fourier inverse Formula* of $G(\omega)$
the two equations above are the essence of Fourier transform theory.

**Theorem 4. Suppose that $F(t)$ is piecewise on every bounded interval and $\int^{\infty}_{-\infty}|F(t)|dt$ exists, Then the Fourier transform $G(\omega)$, of $F$ exists and**
$$\text{p.v.} \int^{\infty}_{-\infty} G(\omega) e^{i\omega t} dt = \begin{cases}
F(t) \qquad \qquad \quad \text{where F is continuous} \\
\frac{F(t+) + F(t-)}{2} \qquad \text{otherwise}
\end{cases}$$
where $p.v.$ means the present value

the example can be found in the page 481

## 2. Laplace transform 
### (1) Definition of Laplace transform 
In the previous section is used for solve  a linear system by means of frequency analysis, The strategy we were employing can be stated -> if the linear system is forced by a sinusoid input function, $e^{i\omega t}$, and then we expect  that there ought to be a solution of the form $Ae^{i\omega t}$, which is a solution of the same frequency.

but Now this may not be the only solution, for example, consider the problem of finding a function $f(t)$ that satisfies the equation: 
$$\frac{d^2 f(t)}{dt^2} + 2\frac{df(t)}{dt} + f(t) = e^{i 2 t}$$
It has the solution of $Ae^{i2t}$ with $A = \frac{1}{4i-3}$ , but considering the solution of the associated homogenous equation $f(t)$
$$\frac{d^2 f(t)}{dt^2} + 2\frac{df(t)}{dt} + f(t) = 0$$
the function f(t) can be added to the solution of the eq above

like -> the function
$$\frac{1}{4i - 3}e^{i2t} + 7e^{-t}$$
is also one of the solution of  the equation, and $7e^{-t}$ is a "homogenous solution"

Note that for most physical systems, these homogenous solutions are ==transient== in nature. (that is, the homogenous part die out as time increases like $e^{-t}$) -> the reason is that for the most physical systems. If not forced, is eventually come to rest due to dissipative phenomena. -> called *asymptotically stable*

we should consider the system may  start in some "initial configuration" at $t=0$ that probably doesn't coincide with steady-state solution, and also, the inputs at "t=0" doesn't drive the system for all $t\geq 0$
the Laplace transform handles both of the problem above 

for the input function, $F(t)$ is defined for all $t\geq 0$, so we set $F(t) =0$ for $t< 0$, and then consider the Fourier transform of $F$ 
$$G(\omega) = \frac{1}{2\pi} \int^{\infty}_{-\infty} F(t) e^{-i\omega t} dt$$
since $F(t) = 0(t\leq 0)$, we have:
$$G(\omega) = \frac{1}{2\pi} \int^{\infty}_{0} F(t) e^{-i\omega t} dt \tag{3}$$
We have that $F$ is <b><mark style="background: transparent; color: orange">sufficiently well behaved near infinity</mark></b>(we shouldn't be precise)-> the equation (3) above defines a function of $\omega$ that is ==analytic in the lower-half-plane== $Im(\omega)< 0$,  and the derivation of the frequency omega is given by: 
$$\frac{dG(\omega)}{d\omega} = \frac{-i}{2\pi} \int^{\infty}_{0}tF(t) e^{-i\omega t} dt$$
and the lower half-plane is appropriate because (since $|e^{i\theta t}| =1$) 
$$\left|e^{-i\omega t}\right| = e^{(\text{Im}\omega )t}$$
is bounded there.  

If we let $\omega$ be pure imaginary say $\Large\omega = -is$ with nonnegative, we create 
$$g(s) := 2\pi G(-is)$$
and we reach a function 
$$\Large \boxed{g(s) = \int^{\infty}_{0}F(t) e^{-st} dt \tag{5}}$$
this is called the *Laplace Transform* of $F(t)$. and It's often useful to indicate the relation between $g(s)$  and $F(t)$ by employing this notation: 
$$g(s) = \mathcal{L}\{F\}(s)$$
for example: [[8. The transforms of Applied Mathmatics 2023-01-10 18.04.12.excalidraw|simple example]]

> [!NOTE] General definition of $s$ above 
> Generally,  In  Laplace Transform, we often define 
> $$s = \sigma + i \omega$$ 
> So we can say that Laplace Transform is similar to  Fourier Transform when  $\sigma = 0$. Also we can use $s = i\omega$ to find the Fourier Transform of it. 

> [!caution] Bilateral Laplace transform   
> We  often treat the response is zero before the input signal. Which is : 
> $$f(t) = 0,  \forall  t < 0$$
> however, We also use the bilateral Laplace Transform, defined as : 
> $$F(s) = \int_{-\infty}^{\infty} f(t) e^{-st} dt$$
> We note the double-side Laplace Transform should satisfy the decay condition in both postive and negative  directions.  

Remark that the $F(t)$ the integral in the Eq above may converge even if  $F$ doesn't approach zero as $t \to \infty$ , provided $s$ is sufficiently large, for example the function $F(t) = e^{7t}$->  $\frac{e^{(7-s)b} - 1}{7-s}$, then  the s should be large when the s>7, the function converges to  $(s -7)^{-1}$ as $b \to \infty$

Accordingly, we should say  that the **equation (5) defines the Laplace transform ==for any complex value of $s$ for which the integral converges==. In essence the Laplace transform is able to encompass more function than the Fourier transform.** 

For a simple extension of the preceding computation, By interpreting the statement with various choice of the constant $a$，we are able to derive the [[📘ClassNotes/📐Mathmatics/ℹ️Complex Variables/补充内容/常用拉普拉斯变换表|Laplace transform table]].   

### (2) Important Relations & Properties 
There are some important relationships for Laplace Transform :

1. Translation property : 
$$\mathcal{L}\{F(t)e^{-at}\}(s) =  \mathcal{L}\{F\}(s+a)$$

2. Derivation properity :  
Now if $F'$ is sufficiently **well behaved so that integration by parts is permitted**, this becomes: 
$$\mathcal{L}\{F'\}(s) = \int^{\infty}_{0}e^{-st}F'(t)dt =  \left. e^{-st} F(t)\right|^{\infty}_0 + \int^{\infty}_{0} se^{-st}F(t)dt$$
thus 
$$\boxed{\mathcal{L}\{F'\}(s) = s\mathcal{L}\{F\}(s)-F(0)}$$
also, the Iteration of the equation results in 
$$L\{F''\}(s) = s\mathcal{L}\{F'\}(s) - F'(0) = s^2\mathcal{L}\{F\}(s) - sF(0) - F'(0)$$
and in general 
$$\mathcal{L}\{F^{(k)}\}(s) = s^k \mathcal{L}\{F\}(s) - s^{k-1} F(0) - s^{k-2} F'(0) - ... - F^{(k-1)}(0) \tag{8}$$

> [!NOTE] Transforms  for  derivatives of $F$ 
> **Theorem 5. Suppose that the function $F(t)$ and it's first $n-1$ derivatives are continuous for $t\geq 0$ ,  and that $F^{(n)}(t)$ is piecewise smooth on every finite interval $[0,b]$, Also suppose that there are positive constants $M$, $\alpha$ such that for $k=0,1,2,....n-1$** 
> $$\left|F^{(k)}(t)\right|\leq Me^{\alpha t} \qquad (t\geq 0)$$
> **Then  the Laplace transforms of $F, F', ...$. exists for Re(s)>$\alpha$ and Eq$(8)$ is valid for $k= 1,2,...n$**  

### (3) Inverse Laplace Transform 
**Theorem 6. Suppose  that $F(t)$ is piecewise smooth on every finite interval $[0,b]$ and $|F(t)|$ is bounded by $Me^{at}$ for  $t\geq 0$ , Then the $\mathcal{L}\{F\}(s)$ exists for $Re(s)>\alpha$. and for all $t>0$ and $a>\alpha$**, we have : 
$$\frac{F(t+) + F(t-)}{2} = \frac{1}{2\pi i} p.v.\int^{a + i\infty}_{a - i\infty}\mathcal{L}\{F\}(s) e^{st} ds$$
This is a theorem for recovering the  Original Function value,  where $p.v.$ is **principle value**, i.e. 
$$p.v. \int_{a - i \infty}^{a +i \infty} = \lim_{R \rightarrow  \infty}\int_{a - i R}^{a +i R}$$

We define the  Inverse Laplace Transform as : 
for $s = \sigma + i \omega$, we have :
$$\mathcal{L}^{-1}( g(s)) (t) = \frac{1}{2\pi i} \int_{\sigma - i \infty}^{\sigma + i \infty} g(s) e^{st}  ds$$
where  $\sigma$ is a real number located in the region of convergence of $g(s)$. 

This Equation is applicable for both unilateral and bilateral Laplace Transform  

## 3. Z-transform
### (1) Definition 
> [!HINT] 
> the Z-transform can be viewed as a specific instance of a [Laurent series](https://en.wikipedia.org/wiki/Laurent_series "Laurent series") . See [[📘ClassNotes/📐Mathmatics/ℹ️Complex Variables/第四章 复变函数和级数#四、 洛朗级数 (Laurent Series)|Laurent Series]] in textbook. 

In Practice we usually encounter the function that are <b><mark style="background: transparent; color: orange">discrete data structures</mark></b>. 

See [Wiki](https://en.wikipedia.org/wiki/Z-transform).  Z-Transform <b><mark style="background: transparent; color: orange">converts a discrete time signal into a complex-valued frequency domain</mark></b>, which is called z-domain or z-plane. The Z-transform **provided a systematic and effective method for solving linear difference equations with constant coefficients**, which are ubiquitous in the analysis of discrete-time signals and systems. 

So we denote a discrete sequence of numbers by $x_{n}$, we assume $n$ to **take integer values from $-\infty$ to $\infty$ we also allow the $x_{n}$ can be complex**, As examples consider :  
1. for sample $2^{-|x|}$ at integers  $x=n$ : 
$$\{x\} \to ...\frac{1}{16}, \frac{1}{8}, \frac{1}{4}, \frac{1}{2}, 1,\frac{1}{2},\frac{1}{4},.....$$
2. for sample $cos(\pi x)$ :  
$$\{x\} = -1,1,-1,1,.....$$

The **Z-transform** can be defined as either a _one-sided_ or _two-sided_ transform. **Z-transform of the sequence of the sequence $x(n)$ is defined as the ==sum of the power series==** : 
1. **Bilateral Z-transform**  
$$\boxed{X (z) =  \mathcal{Z}\{ x [n] \} := \sum^{\infty}_{n = -\infty} x_{n} z^{-n}}$$
where  $z$ is a complex number :
$$z = A e^{j \phi} = A \cdot  (\cos \phi  + j \sin \phi)$$
and currently **variable becomes $z$**  

2. Unilateral Z-transform  
$$\boxed{X(z) = \mathcal{Z}\{ x[n]\} = \sum_{n = 0}^{+\infty} x_{n} z^{-n}}$$
> [!NOTE] The choose of $z$ 
> $z \in \mathbb{C}$ should be inside the **region of convergence (ROC)**

### (2) Inverse Z-Transform
Theorem 7. **Let A(z) be the z-transform of the sequence  $[x(n): -\infty <n < \infty$ in the annulus $a < |z| < b$, Then** 
$$x_n = \frac{1}{2\pi i} \oint_\Gamma X(z) z^{n-1}dz \qquad (n =0, \pm 1, \pm 2,....)$$
**where $\Gamma$ is any positively oriented simple closed contour lying in the annulus and encircling the origin**

> [!HINT] Special Case  
> For unit circle, we have $z=e^{j\omega}$ and $dz = j e^{j \omega} d\omega$. so that : 
> $$x_{n} = \frac{1}{2\pi} \int_{-\pi}^{+\pi} X(e^{i\omega})  e^{i n\omega} d\omega$$

Theorem 8. **Let A(z) be the z-transform of the sequence  $[a(n): -\infty <n < \infty$ in the annulus $a < |z| < b$, Then the corresponding z-transform of the shifted sequence $\{b(n) = a(n+1): -\infty < n < \infty\}$ is given by $zA(z)$, and also , the $c(n) = a(n+N)$ is given by $z^NA(z)$ for any positive $N$** 


