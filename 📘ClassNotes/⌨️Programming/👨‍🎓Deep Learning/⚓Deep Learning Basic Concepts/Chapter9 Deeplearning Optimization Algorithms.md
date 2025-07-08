## 1. Problems in optimization 
### (1) Introduction 
The  optimization algorithm directly  affect the training efficiency.  

We know that almost  **all optimization problems  arising  in  deep learning are nonconvex**. Nonetheless, the design and analysis of algorithms  are in context of convex are instructive.  

The objective function of the optimization algorithm is usually a loss function based on the training dataset, <mark style="background: transparent; color: red">the goal of optimization is to reduce the training error. However, the goal of deep learning (or more broadly, statistical inference) is to reduce the generalization error. </mark>

we simply define a risk function and empirical  risk function as follows : 
```python fold title:RiskFunction-and-empirical-function 
import matplotlib.pyplot as plt  
import numpy as np  
import torch  
  
  
def f(x):  
    return x * torch.cos(np.pi * x)  

def g(x):  
    return f(x) + 0.2 * torch.cos(5 * np.pi * x)

# plot f(x) and g(x)  
x = torch.arange(0.5, 1.5,0.01)  
plt.figure()  
plt.plot(x, f(x), label="risk function")  
plt.plot(x, g(x), label="empirical risk function", linestyle='--')  
plt.legend()  
plt.show() 
```

**minimum of the empirical risk on a training dataset may be at a different location from the minimum of the risk**  
![[attachments/Pasted image 20250428145744.png|350]]
Some of the most vexing challenges are <mark style="background: transparent; color: red">local minima, saddle points, and vanishing gradients.</mark> 

### (2) Local Minima, Saddle points and Vanishing Gradients
firstly, we implement a minimum search function by  ` scipy.optimize` : 

```python fold title:
import numpy as np  
from scipy.optimize import differential_evolution  
  
# note differential_evolution  is more efficient than SA algorithm for global optimization  
bounds = [(-1, 2)]  
  
x = np.linspace(bounds[0][0], bounds[0][1], 100)  
  
  
def func(x):  
    return x * np.cos(np.pi * x)

# find the global minimum using  differential_evolution  
result = differential_evolution(func, bounds, maxiter=1000, popsize=100, tol=1e-4)  
  
best_x = result.x[0]  # convert the result to tensor  
best_y = func(best_x) 
plt.figure()  
plt.plot(x, func(x), label="goal function")  
plt.grid()  
  
# annotate the 'local minimum' at (-0.3, -0.25), (-0.77, -1.0)  
plt.annotate('Local Minimum', xy=(-0.3, func(-0.3)), xytext=(-0.8, -0.8),  
             arrowprops=dict(facecolor='black', shrink=0.05))  
plt.annotate('Global Minimum', xy=(best_x, best_y), xytext=(0.5, 0.8),  
             arrowprops=dict(facecolor='black', shrink=0.05),  
             fontsize=10)
```

![[attachments/Pasted image 20250428152559.png|300]]
we also give the saddle point as : 
![[attachments/Pasted image 20250428153151.png|200]]
where gradient is 0 

- When **the eigenvalues of [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Implements/Concept of Hessian Matrix|the function’s Hessian matrix]] at the zero-gradient position are negative and positive, we have a saddle point for the function**.  

for  vanishing gradient is as follows, We note <b><mark style="background: transparent; color: orange">often a reparameterization of the problem helps. </mark></b>
![[attachments/Pasted image 20250428155332.png|250]]
### (3) Convex and Convex functions 
firstly we introduce the [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/👨‍🎓机器学习算法(sklearn)/4. 最优化方法, 梯度寻优法及Logistic 回归#(2) 凸集和分离定理|Convex Sets]], which is : 
$$\alpha x_{1} + (1- \alpha)  x_{2}  \in   X , \quad  \forall x_{1} , x_{2} \in  X \quad \forall  \alpha \in [ 0.1 ]$$
we note that the <mark style="background: transparent; color: red">intersection of two  convex is also a convex</mark>.  
![[attachments/Pasted image 20250428160514.png|200]]
also, if for two convex that  $X \cap Y = \emptyset$, for every $x_{1} \in  X$ and $x_{2} \in  Y$,  we have : 
$$x_{1} + (1 - \alpha) x_{2} \notin X \cup  Y  $$
which implies that **unions of convex  sets  needn't to be convex**. 
![[attachments/Pasted image 20250521161759.png|250]]

For <b><mark style="background: transparent; color: orange">Convex functions</mark></b>, the following equation is satisfied  :  
$$\Large \boxed{\lambda f(x) +  (1 - \lambda) f(x') \geq   f(\lambda x  + (1 - \lambda) x')}$$
![[Excalidraw/Chapter9 Deeplearning Optimization Algorithms 2025-05-21 16.21.52|250]]

#### 3.1 Jensen's Inequality  
For any convex function $f$, we have one of the most useful tool <b><mark style="background: transparent; color: orange">Jense's inequality</mark></b> : 
$$\sum_{i} \alpha_{i} f(x_{i}) \geq  f \left( \sum_{i} \alpha_{i} x_{i} \right) \quad  \text{and} \quad  E_{X} [f(X)] \geq  f(E_{X}[X])$$
where we have  
$$\sum_{i} \alpha_{i} = 1$$
and  $X$ is a random variable. <b><mark style="background: transparent; color: orange">the above equations elucidate that expectation of a convex function is no less than the convex function of an expectation, where the latter is usually a  simpler expression</mark></b>

We give one of the common application of it as **log-likelihood of partially observed random variables**. 
since we have : 
$$\int P(Y) P(X|Y) dY =  P(X)$$
Here Y is typically the **unobserved random variable**, P (Y ) is the **best guess of how it might be distributed**, and **$P (X)$ is the distribution with Y integrated out**. in clustering,  $Y$ might be the cluster labels, and  $P(X|Y)$  is the generative model when applying  a cluster labels. then we have :  
$$E_{Y \sim  P(Y)} \left[ -  \log_{}P(X|Y) \right] \geq   - \log_{} P(X)$$

#### 3.2 Properties 
For convex functions, it have many properties that useful. 

1.  The  Local minima of  convex functions are **also the global minima**. 
proof : assume the local minimum at $x^{*}$ is not global minimum, and true global minimum is $x'$, then $\forall \varepsilon > 0$, we have : 
$$f (\lambda x^{*} + (1 - \lambda) x') \leq  \lambda f(x)^{*} + (1 - \lambda) f(x') <  f(x^{*})$$
since  $x^{*}$ is a local minimum,  for $|x - x^{*}| < \varepsilon$,  $f(x) > f(x^{*})$, we let 
$$|\lambda x^{*} + (1 - \lambda) x'  - x^{*}| < \varepsilon \rightarrow  |(1- \lambda)( x' - x^{*} )|< \varepsilon \rightarrow   \lambda  < 1 -  \frac{\varepsilon}{|x' - x^{*}|}$$
then in that field we have : 
$$ f(x^{*}) < f(\lambda x^{*}  + (1 -  \lambda) x')$$
which draws contradiction. 
2.  **Blow Sets of convex are convex** 

3.  Convexity and Second Derivates 
a **twice- differentiable 1-D function f : $R \rightarrow R$ is convex** is and only if  second derivate 
$$f'' > 0$$
firstly, to see that convexity of f implies $f'' > 0$, we can simply proof it by the definition. 

### (4) Constraints  
The  optimization problem is : 
$$\underset{x}{\text{minimize}}\space  f(x)$$
$$ \text{subject to}\quad   c_{i}(x)\leq 0  \space ,  \forall   i  \in  \left\{ 1, 2, \dots  n \right\}$$
we can **add the constraints together** use Lagrangian Optimization problem of the above function, which gives : 
$$\Large\boxed{L(x, \alpha_{1}, \dots  \alpha_{n}) = f(x) + \sum_{i = 1}^{n} \alpha_{i} c_{i}(x)\qquad  \alpha_{i} \geq  0}$$
$\alpha_i$ is Lagrange multiplier. 

in that case, satisfying the constrained optimization problem **at least approximately** is to adapt the $L$ to **slack the constraints condition**.  note we may also use $||w||^2/2$ in SVM to prevent getting large. 

Also,  an alternative  strategy for satisfying the constrains  is <b><mark style="background: transparent; color: orange">projection.</mark></b>  For example, we used [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Chapter7 RNN#(3) Gradient Clipping|Gradient Clipping]] for 
$$g \leftarrow  \min  \left(1, \frac{\theta}{||g||}\right)\boldsymbol{g}$$
in that case, we cast $g$  to the circle with radius $\theta$.  

More generally, the projection on a convex $\mathcal{X}$ can be defined as : 
$$\text{Proj}_{\mathcal{X}} (x) = \text{argmin}_{x' \in  \mathcal{X}} || x - x'||$$
![[Excalidraw/Chapter7 RNN 2025-05-21 18.14.44|400]]
## 2. Gradient Descend and Adaptive  Methods 
### (1) General SGD method 
for gradient descend, we can refer to [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Chapter3 Linear Neural  Networks for regression(back Propagation)#2. Minibatch Stochastic gradient descent(SGD)|SGD Method]], and use Gradient Descend method like : 
$$x \leftarrow  x - \eta  \nabla f(x)$$
where  
$$\nabla f(x) =  \left[\frac{\partial f}{\partial x_{1}},  \dots  \frac{\partial f}{\partial x_{n}} \right]$$

<b><mark style="background: transparent; color: orange">Newton's Method</mark></b>
consider the Tylor Expansion, we have : 
$$f(x + \varepsilon) = f(x) + \varepsilon \nabla f(x) + \frac{1}{2} \varepsilon^{T} [\nabla^{2} f(x)] \varepsilon +  O (\varepsilon^{3})$$
we define : 
$$H  \equiv   \nabla^{2} f(x)$$
We note here that  for deep neural networks,  $H$ may to prohibitively large, we note that for minimum of $f$, it satisfies  $\nabla f = 0$ 
$$\nabla f(x) + H \varepsilon = 0 \overset{}{\longrightarrow}  \varepsilon = - H^{-1}\nabla  f(x)$$
note that the Hessian method not always converge to the global minima. One way would be to “fix” the Hessian by **taking its absolute value instead.** 

### (2) Adaptive Methods 
#### 1) Precoinditioning 
Since the Hessian matrix need expensive cost for store, we want to find an alternative  that <b><mark style="background: transparent; color: orange">avoid computing Hessian</mark></b>, if we can only  compute the diagonal of the  entries, it result in the following update method : 
$$x \leftarrow   x - \eta  \text{diag} (H)^{-1} \nabla f(x)$$

For the stochastic  gradient descent method, it use  
$$x \leftarrow  x - \eta  \nabla f_{i} (x)$$
where we uniformly sample an index $i \in  \{ 1, \dots  n\}$ at random. we note :
$$E_{i} \nabla f_{i}(x) = \frac{1}{n} \sum_{i = 1}^{n}  \nabla f_{i} (x)  = \nabla f(x)$$
Which states that SGD is good estimte of gradient method on average. 

Also considering that when we arrive the  minimum, we will still suffer from the uncertainty. So we may use the dynamic  learning rate $\eta$  
#### 2) Dynamic  Learning rate  
Main strategies are including : 

1. exponentail decay : 
$$\eta(t) = \eta_{0} e^{- \lambda t}$$
2. polynomial decay : 
$$\eta(t) = \eta_{0} (\beta  t + 1)^{- \alpha}$$
Note a popular choice is polynomial decay with $\alpha = 0.5$

We set $x^*$ as the minimzed point exist within the domain. Also consider   for any convex function $f$ it holds that : 
$$f(y) \geq  f(x) + <f'(x), y - x>$$
then we have : 
$$f(\xi_{t}, x^{*}) \geq  f(\xi_{t}, x_{t}) + <x^{*} - x_{t}, \partial_{x} 
f(\xi_{t}, x_{t})>$$
where  $f(\xi_t, \boldsymbol{x})$ is the **objective funcction with respect to the training example $\xi_t$**,  and  $\boldsymbol{x}$ is  model parameter. The **expected risk** is the **average error**  for all $\xi$ : 
$$R(x) = E_{\xi} [f(\xi, \boldsymbol{x})]$$
we can derivate that :
$$||x_{t}  - x^{*}||^{2} - ||x_{t+1} - x^{*}||^{2} \geq   2\eta_{t} (f(\xi_{t}, x_{t}) - f(\xi_{t}, x^{*})) - \eta_{t}^{2} L^{2}$$
since we always need the result converge (left side $\geq  0$) we got that if the  difference between  current loss and  optimal loss outweights $\eta_{t} \frac{L^{2}}{2}$, the difference is bound to converge to 0. 

We  can got  
$$||x_{1} - x^{*}||^{2}  \geq   2 \left(\sum_{t = 1}^{T}  \eta_{t}\right) [E[R(\boldsymbol{x}_{t})] - R^{*}] - L\sum_{t = 1}^{T} \eta_{t}^{2}$$
we define: 
$$\overline{x} \xlongequal{def}\frac{\sum_{t = 1}^{T} \eta_{t}  \boldsymbol{x}_{t}}{\sum_{t=1}  ^{T}  \eta_{t}}$$
we can derive  that : 
$$[E[\overline{x}]] - R^{*}  \leq  \frac{r^{2} + L^{2} \sum_{t = 1}^{T} \eta_{t}^{2}}{2 \sum_{t = 1}^{T}  \eta_{t}}$$
where  $r^{2} = ||x_{1} - x^{*}||^{2}$  is a bound  on the distance  between  the initial  choice of parameters. 

Note that we want  the above part < $r$. so if  $r, L$ and  $T$ are all known, we can specify the learning rate as $\eta = r/(L \sqrt{T})$ which is  converged  with rate  $O(1/\sqrt{T})$


For stochastic gradient descent, It is <b><mark style="background: transparent; color: orange">not particularly computationally efficient since CPUs and GPUs cannot exploit the full power of vectorization. </mark></b>  
### (3) Momentum method  
#### 1. Conception  
Since the  SGD equation  is : 
![[attachments/Pasted image 20250603155618.png|350]]
and the  gradient computation is : 
$$x_{t} =   x_{t-1} -  \eta_{t} g_{t, t-1}$$
we can replace the above gradient computation  by a  `leaky average`,  and this formed  the  accelerated gradient methods, such as <b><mark style="background: transparent; color: orange">gradients with momentum</mark></b>.  
$$\boxed{v_{t} =  \beta v_{t-1}  + g_{t, t-1 }}$$
here $v$ is velocity (It accumulates past gradients similar to how a heavy ball rolling down the objective function landscape integrates over past forces) 

where  $g = \nabla f (w^k)$  is gradient. and  <b><mark style="background: transparent; color: orange">then substitute the gradient descend method by the following equation</mark></b> : 
$$\boxed{x_{t} = x_{t-1} - \eta_{t}  v^{t}}$$
Through this equation,we can recursively expand the $v_t$ into : 
$$v_{t} = \beta^{2} v_{t-2}  + \beta g_{t - 1,  t-2} + g_{t, t-1} = \sum_{m = 1}^{t-1}  \beta^{m}  g_{t-m, t-m - 1} $$
here Large $\beta$  amounts to a long-range average. We define $\beta$ as momentum, see  https://distill.pub/2017/momentum/  for details. 

> [!NOTE] 
> For small beta, it may take great concern to the $\eta$ of last several steps, and $\beta$ would converge to 0

#### 2. Application 
For [SGD in pytorch](https://docs.pytorch.org/docs/stable/generated/torch.optim.SGD.html), we can directly use the `momentum` and `dampening`
```python 
optimizer = torch.optim.SGD(params, lr=0.01, momentum=0.9, dampening=0)
```

where `dampening` is dampening for momentum, `nesterov` is weather nesterov moment is used. 

> [!NOTE] 
> In default pytorch not use momentum in SGD, so we can specify the momentum. 
> 
> Also, for Adam oprimizer, it use `EMA` implement a mechanism similar to the momentum. 

For a general problem : 
$$h(x) = \frac{1}{2} x^{T} Q x  + x^{T}c + b$$
then 
$$h(x) = \frac{1}{2} x^{T} Q x  + x^{T}c + b$$
generally,  we have $x^{*} =  -(Q+ Q^{T})^{-1} c$, but here <mark style="background: transparent; color: red">we consider Q as a symmetric matrix</mark>. 

Thus, we have $x = Q^{-1}c$,  the minimum is : $b -  \frac{1}{2} c^{T }Q^{-1} C$,  since  we can decompose  $Q = O^{T} \Lambda O$, then  we can write it to the following  form by completing the squre : 
$$h(x) = \frac{1}{2} (x - Q^{-1 }c)^{T} Q (x -Q^{-1} C ) + (b -  \frac{1}{2} c^{T}  Q^{-1} c)$$
also, applying the decomposition,  and set 
$$z = O (x - Q^{-1} c)$$
then we have the simple standarized expression : 
$$h(z) = \frac{1}{2} z^{T}\Lambda z + b'$$
and $b' = b - \frac{1}{2} c^T Q^{-1}c$.  note O is orthogonal  matrix that doesn't pertube the gradients.  so the gradient is calculated as : 
$$h '(z) =  \Lambda  z$$
then GD becomes :  
$$z_{t} = z_{t-1} - \Lambda  z_{t-1} =  (I - \Lambda ) z_{t-1}$$
this <b><mark style="background: transparent; color: orange"> not mix beteween different eigen-values </mark></b>, and 
The following equations are displayed: 
$$ v_t = \beta v_{t-1} + A z_{t-1} $$
$$ z_t = z_{t-1} - \eta (\beta v_{t-1} + A z_{t-1}) = (I - \eta \Lambda) z_{t-1} - \eta \beta v_{t-1} $$
> [!TIP] Theorem 
> gradient descent with and without momentum for a convex quadratic function decomposes into **coordinate-wise optimization in the direction of the eigenvectors of the quadratic matrix** 

we can also write it into : 
$$
\begin{bmatrix}
v_{t+1} \\
x_{t+1}
\end{bmatrix}
=
\begin{bmatrix}
\beta & \lambda \\
-\eta \beta & (1 - \eta \lambda)
\end{bmatrix}
\begin{bmatrix}
v_t \\
x_t
\end{bmatrix}
=
R(\beta, \eta, \lambda)
\begin{bmatrix}
v_t \\
x_t
\end{bmatrix}.
$$
so it's **up to  the eigenvalues of $R$ to determine the speed of convergence**. 
It also suggests that ==in general large values of β are desirable==. 

## 3. Other Optimization methods 
### (1) Adagrad 
Firstly, we can consider adjusting learning rates by a clock,  however, Given a decreasing learning rate we might end up in a situation where the parameters for common features converge rather quickly to their optimal values.  

Rather than choosing a learning rate of the form 
$$\eta = \frac{\eta_{0}}{\sqrt{t + c}}$$
we often use : 
$$\eta_{i}=  \frac{\eta_{0}}{\sqrt{s(i,t) + c}}$$
For Adagrad, it use : 
$$s(i, t + 1) = s(i, t) + (\partial_{i} f(x))^{2} $$
as a mean to adjust the learning rate; 

> This has two benefits: first, we no longer need to decide just when a gradient is large enough. Second, it scales automatically with the magnitude of the gradients  

- however, the learning rate wouldn't recover when the gradient become large once. 

### (2) RMSProp 
For nonconvex  problem, **the learning decrease monotonically may not be ideal for nonconvex  problem**. 

since $s_{t} = s_{t-1} + g_{t}^{2}$, which <b><mark style="background: transparent; color: orange">keeps on growing  without bound due to lack of normalization.</mark></b> 

For RMSprop  algorithm, it use : 
$$s_{t}  = \gamma s_{t-1}  + (1 - \gamma) g_{t}^{2}$$
and then use : 
$$x_{t} = x_{t-1}  - \frac{\eta}{\sqrt{s_{t} + \varepsilon} } \cdot  g_{t}$$
we note that  expanding the above equation $s_{t}$ yields : 
$$s_{t} = (1- \gamma) [g_{t}^{2}  +  \gamma  g_{t-1}^{2} + \gamma^{2} g_{t-2}^{2}  + \dots  ]$$
since : 1 + $\gamma$  + $\gamma^2$ + ... = $\frac{1}{1- \gamma}$,   it equals to 1 for any $\gamma$ chosen. 

### (3) Adadelta  
It also use the <b><mark style="background: transparent; color: orange">normalized learning-rate decreasing factor</mark></b> :
1.  
$$s_{t}  = \gamma s_{t-1}  + (1 - \gamma) g_{t}^{2}$$
like RMSProp algorithm, however, it use the **rescaled** $g_t$, and perform the updates with rescaled gradient $g'$ (<mark style="background: transparent; color: red">rather than using the rate</mark> $\eta$ ):   
$$x_{t} = x_{t-1}  - g'_{t}$$
We  initialize $\Delta x_{0}$ as 0,  and update it as : 
$$\Delta x_{t} = \rho \Delta x_{t-1}  + (1-  \rho) g_{t}^{'2}$$
where
$$g_{t}' = \frac{\sqrt{\Delta x_{t-1} + \varepsilon}}{\sqrt{s_{t} + \varepsilon }}  \odot   g_{t}$$
we note $s_t, g_{t}'$ are two state variables. 
> [!NOTE] 
> Adadelta has no learning rate parameter `->` just calculate the gradient and calculate $\Delta x$ according to the gradient and state variables. 

```python fold title:key_code-example-of-Adadelta
for p, (s, delta) in  (params, stats):
	with torch.no_grad(): 
		gr = p.grad 
		s[:] = rho * s + (1 - rho) * torch.square(gr)
		g = torch.sqrt(delta + eps)/torch.sqrt(s +eps) * gr
		# gradient descent 
		 p[:] -=g 
		 delta[:] = rho  * delta + (1 - rho) *g ** 2
	p.grad.data.zero_()
```

## 4. Adam Optimizer 
### (1) Adam Optimizer Algorithm 
The Initial promoted Adam Optimizer algorithm is as follows :  
![[attachments/Pasted image 20250611113803.png|700]]

We note this algorithm can diverge due to poor variance control, and Zaheer  proposed a hotfix to Adam. 

Firstly, it use leaky Averaging  to obtain both momentum and  the variance:
$$v_{t} \leftarrow  \beta_{1}  v_{t-1}  +(1- \beta_{1}) g_{t}$$
$$m_{t} \leftarrow  \beta_{2} m_{t-1}  + (1- \beta_{2}) g_{t}^{2} =   m_{t-1}  - (1- \beta_{1} ) ( m_{t-1} -  g_{t}^{2})  $$
where $m_{t}$ is momentum. 
> [!NOTE] weight choice 
> common choice for the above is $\beta_{1} = 0.9$ and  $\beta_{2} = 0.999$ (<b><mark style="background: transparent; color: orange">variance moves much slowly  than the momentum term</mark></b>)
> Also, if  we  initialize $v_{0} = s_{0} = 0$,  we would have very slow amount increase  of $v$ and $s$ and also a significant amount of bias, which can be  solved by <mark style="background: transparent; color: red">re-normalize terms</mark>: 
> $$\sum_{i = 1}^{t}  \beta^{i}  = \frac{1- \beta^{t}}{1-\beta}$$
> hence the normalized  state variables  is : 
> $$\hat{v}_{t} = \frac{v_{t}}{1 - \beta_{1}^{t}}\qquad  \hat{s}_{t} = \frac{s_{t}}{1 - \beta_{2}^{t}}$$

Firstly, we use : 
$$g'_{t}  =  \frac{ \eta  \cdot  \hat{v}_{t}}{\sqrt{\hat{m }_{t}} + \epsilon }$$
note the $\sqrt{\hat{m}_{t}} + \epsilon$ works slightly better  than $\sqrt{\hat{m}_{t}  + \epsilon}$ in practice. So it has both momentum, 
we can update the parameter as : 
$$x_{t} \leftarrow   x_{t-1}  - g_{t}'$$

> [!HINT] Weight decay parameter 
> The weight decay is just the [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Chapter3 Linear Neural  Networks for regression(back Propagation)#(5) Norms and Weight decay|**L2 norm**]] after calculation. However, in AdamW the gradient and  weight decay are decouped. 

### (2) Yogi  Algorithm  
Note for the **YOGI algorithm** (hotfix version by Zaheer) the modifications are as follows : 
![[attachments/Pasted image 20250611132801.png|700]]
in above equation, it simply modifies : 
$$ (v_{t-1} - g_{t}^{2})  \rightarrow \text{sgn}  (v_{t-1}  - g_{t}^{2}) g_{t}^{2}$$
in that case, $v_t$ can increase if $g_{t}$ is greater. so it also speed up the training process. 

> [!question] why?
> Since $g_{t}^{2}$ has high variance or **updates are sparse**,  $s_{t}$ might  forget  past values quickly. which is because  $\beta^{t}\rightarrow 0$, then if $g_{t}^{2} \rightarrow  0$ (sparse)
> $$v_{t } \approx \beta_{2} v_{t-1}$$
> and this way, $s_t$ only increases or decreases in direction of the deviation but not by its magnitude — avoiding large or premature updates and slowing down forgetting.
> Also, In practice we may use : 
> $$w_{t}  = \max (w_{t}, \delta), \delta > 0$$
> to guarantee $s_t$ is safe.   

we can add yogi optimizer by `pip install pytorch-optimizer` 
```python
from pytorch_optimizer import Yogi
optimizer = Yogi(model.parameters(), lr=1e-3, betas=(0.9, 0.999), eps=1e-3)
```

## 5. Learning Rate Schedulers  
### (1) Learning rate visualization  
See [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/🔥Pytorch Basics/Pytorch Schedulers|Pytorch Schedulers]], we can wrap many LRScheduler in `SequentialLR` 

Common choices are polynomial decay and piecewise constant schedules. Beyond that, **cosine learning rate schedules have been found to work well empirically on some problems**. 

Also, <b><mark style="background: transparent; color: orange">It's benificial to warm up the optimizer using larger learning rates</mark></b>

For a most simply constant , we can use  [`lr_scheduler.ConstantLR`](https://docs.pytorch.org/docs/stable/generated/torch.optim.lr_scheduler.ConstantLR.html#torch.optim.lr_scheduler.ConstantLR "torch.optim.lr_scheduler.ConstantLR")  
$$lr_{t + 1} = k \times   lr_{t}$$
To see the learning rate  variation in training process, we can use the following : 
```python fold title:learning-rate-visualization 
import torch  
from torch import nn  
import matplotlib.pyplot as plt

class Net1(nn.Module):  
    def __init__(self):  
        super().__init__()  
        self.net = nn.Linear(10, 10)  
  
    def forward(self, x):  
        return self.net(x)  
        
# create a simple network instance  
net = Net1()  
optimizer = torch.optim.SGD(net.parameters(), lr=0.1)  
scheduler = torch.optim.lr_scheduler.CosineAnnealingLR(optimizer, T_max=20)  
  
# scheduler = torch.optim.lr_scheduler.StepLR(optimizer, step_size=5, gamma=0.1)  
  
learning_rates = []  
for epoch in range(100):  
    # Dummy training step  
    optimizer.step()  
    scheduler.step()  # Update the learning rate  
    current_lr = optimizer.param_groups[0]['lr']  
    learning_rates.append(current_lr)  
    print(f"Epoch {epoch + 1}, Learning Rate: {current_lr}")  
  
plt.plot(learning_rates)  
plt.xlabel("Epochs")  
plt.ylabel("Learning Rate")  
plt.show()
```

which gives the following image : 
![[attachments/Pasted image 20250611171826.png|200]]

### (2) Warmup Implementation 
For Example, If we want to use LinearLR at first and CosineAnnealingLR after 100 epochs, we can use the following strategy : 
```python fold title:
from torch.optim.lr_scheduler import SequentialLR, LinearLR, CosineAnnealingLR

optimizer = AdamW(model.parameters(), lr=0.1)
warmup_epochs = 5
total_epochs = 100 

# Warmup  
warmup = LinearLR(optimizer, start_factor=0.01, end_factor=1.0, total_iters=warmup_epochs)
# CosineAnneling 
cosine = CosineAnnealingLR(optimizer, T_max=total_epochs - warmup_epochs, eta_min=1e-6) 

scheduler = SequentialLR(optimizer, schedulers=[warmup, cosine], milestones=[warmup_epochs])
```
using the above function we can obtain the following learing rate : 
![[attachments/Pasted image 20250611173540.png|200]] 

• A warmup period before optimization can prevent divergence 
### (3) Schedulers 
For Cosine Scheduler, we have : 
$$\eta_{t} = \eta_{T}  + \frac{\eta_{0} - \eta_{T}}{2} \left(1 +\cos \left( \frac{ t}{T} \pi\right)\right)$$
where $\eta_0 = \eta_\max, \eta_T = \eta_\min$

Cosine schedulers are popular for some computer vision problems (see gluon-cv.mxnet.io)

> [!NOTE] Cosine Anneling 
> We may set $T_{max}$ to max epoch in cosine anneling function  
