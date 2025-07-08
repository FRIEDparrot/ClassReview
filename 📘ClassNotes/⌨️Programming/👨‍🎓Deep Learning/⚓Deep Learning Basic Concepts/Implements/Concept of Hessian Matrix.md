### **What is the Hessian Matrix?**
The **Hessian matrix** is a square matrix of second-order partial derivatives of a scalar-valued function. It describes the local curvature of a function of multiple variables and helps determine whether a critical point (where the gradient is zero) is a local minimum, local maximum, or saddle point.

For a function $f(x_1, x_2, \dots, x_n)$, the Hessian matrix $H$ is defined as:

$$
H(f) = \begin{bmatrix}
\frac{\partial^2 f}{\partial x_1^2} & \frac{\partial^2 f}{\partial x_1 \partial x_2} & \cdots & \frac{\partial^2 f}{\partial x_1 \partial x_n} \\
\frac{\partial^2 f}{\partial x_2 \partial x_1} & \frac{\partial^2 f}{\partial x_2^2} & \cdots & \frac{\partial^2 f}{\partial x_2 \partial x_n} \\
\vdots & \vdots & \ddots & \vdots \\
\frac{\partial^2 f}{\partial x_n \partial x_1} & \frac{\partial^2 f}{\partial x_n \partial x_2} & \cdots & \frac{\partial^2 f}{\partial x_n^2}
\end{bmatrix}
$$

### **How the Hessian Matrix Determines Critical Points**
- **All eigenvalues positive** → **Local minimum** (the function curves upward in all directions).
- **All eigenvalues negative** → **Local maximum** (the function curves downward in all directions).
- **Mixed positive and negative eigenvalues** → **Saddle point** (the function curves up in some directions and down in others).
- **Some eigenvalues zero** → **Test is inconclusive** (higher-order derivatives may be needed).

---

### **Simple Example**
Let’s consider the function:
$$
f(x, y) = x^2 + 2y^2 + xy
$$

#### **Step 1: Compute the Gradient (First Derivatives)**
$$
\frac{\partial f}{\partial x} = 2x + y, \quad \frac{\partial f}{\partial y} = x + 4y
$$

#### **Step 2: Find Critical Points (Where Gradient = 0)**
$$
2x + y = 0 \\
x + 4y = 0
$$
Solving gives $x = 0, y = 0$. So, $(0,0)$ is a critical point.

#### **Step 3: Compute the Hessian Matrix (Second Derivatives)**
$$
\frac{\partial^2 f}{\partial x^2} = 2, \quad \frac{\partial^2 f}{\partial y^2} = 4, \quad \frac{\partial^2 f}{\partial x \partial y} = 1
$$
So, the Hessian at $(0,0)$ is:
$$
H = \begin{bmatrix}
2 & 1 \\
1 & 4
\end{bmatrix}
$$

#### **Step 4: Compute Eigenvalues of the Hessian**
The eigenvalues $\lambda$ are found by solving $\det(H - \lambda I) = 0$:
$$
\det \begin{bmatrix}
2 - \lambda & 1 \\
1 & 4 - \lambda
\end{bmatrix} = (2 - \lambda)(4 - \lambda) - 1 = 0
$$
$$
\lambda^2 - 6\lambda + 7 = 0 \implies \lambda = 3 \pm \sqrt{2}
$$
Both eigenvalues ($\approx 4.41$ and $\approx 1.59$) are **positive**, so $(0,0)$ is a **local minimum**.

### **Conclusion**
The Hessian matrix helps determine the nature of critical points by examining second-order curvature. In this example, since both eigenvalues are positive, the function has a local minimum at $(0,0)$. 