[Ipopt](https://coin-or.github.io/Ipopt)(**I**nterior **P**oint **O**ptimizer) is an open source software package for large-scale nonlinear optimization. (**非线性规划（NLP，NonLinear Programming）求解器**)

This package is used for  <b><mark style="background: transparent; color: orange">solving general nonlinear programming problems of the form</mark></b>  
$$\Large \min_{x \in  R^{n}} f(x)$$
$$\text{s.t.}\space  g^{L}\leq  g(x) \leq  g^{U}\qquad  x^{L} \leq  x \leq  x^{U}$$

> [!NOTE] Cite when use Ipopt 
> Also, if you are using Ipopt to obtain results for a publication, we politely ask you to point out in your paper that you used Ipopt, and to cite the publication [12](https://coin-or.github.io/Ipopt/citelist.html#CITEREF_WaecBieg06:mp)

最方便的安装方法是使用 anaconda 环境的 python 进行安装, Ipopt 依赖线性代数库（如 BLAS、LAPACK）和其他工具（如 ASL、HSL) 等, conda 能够自动解决依赖关系问题。尤其是 `--use-pep517` 等需要时,  如果仍然无法解决,  可以尝试 conda 自动解决。

Ipopt 本身是一个 C++ 求解器，Python 需要通过特定包调用它。常用选项: 
- **CyIpopt**：直接调用 Ipopt 的 Python 接口（推荐）。
- **Pyomo**：建模工具，可调用 Ipopt 作为求解器。 

```sh
conda install -c  conda-forge ipopt
conda install -c conda-forge cyipopt 
pip install pyomo 
```

- **支持问题类型**:
    - 目标函数和约束条件可以是非线性函数。
    - 支持连续变量和约束（不直接支持离散优化，需结合其他工具如 Bonmin）。

```python
# 激活你的 Conda 环境（如果未激活）
conda activate <你的环境名>

# 安装 Ipopt 核心库（如果尚未安装）
conda install -c conda-forge ipopt

# 安装 CyIpopt（直接 Python 接口）
pip install cyipopt

# 或者安装 Pyomo（建模工具 + Ipopt 支持）
conda install -c conda-forge pyomo
```

简单的 demo 测试用例 :  
```python
from cyipopt import minimize_ipopt  
import numpy as np  
  
# minimize the objective function :  
# (x-1)^2 + (y-2)^2  
result = minimize_ipopt(  
    fun=lambda x: (x[0]-1)**2 + (x[1]-2)**2,  
    x0=np.array([0, 0]),  
    options={'disp': 5}  
)  
print("result:", result.x)  # [1. 2.]
```
