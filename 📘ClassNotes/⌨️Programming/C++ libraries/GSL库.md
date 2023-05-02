路径: `C:\gsl-2.7.1`
提供了微积分等等一系列数学库函数
安装方法是使用Project - Manage NuGet Packages 
然后直接搜索gsl 64

![[attachments/Pasted image 20230502121235.png|500]]

![[attachments/Pasted image 20230502121342.png|500]]

之后packages.config 中会出现对应的部分
![[attachments/Pasted image 20230502121431.png|600]]

测试代码
```cpp 
#include <gsl/gsl_math.h>
#include <gsl/gsl_sf_bessel.h>

double result = gsl_sf_bessel_J0(5.0);
```
