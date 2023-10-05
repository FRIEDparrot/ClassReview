首先从网上下载源码的包

然后在Visual Studio 2019 中, 同时设置C/C++的general 属性 ，包含CGAL 的include 文件以及lib文件, 具体如下: 

- 将CGAL的include目录添加到INCLUDE环境变量中。 
    
- 将CGAL的lib目录添加到LIB环境变量中。

![[attachments/Pasted image 20230612203257.png|1200]]

然后使用如下的方法检测是否已经安装完毕
```cpp 
// CGAL 库的安装方法: 下载zip文件，然后添加其include文件到项目的包含路径下
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>  // 二维的三角形网格

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
```
