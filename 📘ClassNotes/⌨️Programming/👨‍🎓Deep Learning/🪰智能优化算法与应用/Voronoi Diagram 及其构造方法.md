参考 :  https://blog.csdn.net/qq_44339029/article/details/126296997

首先 ， 维诺图是由一组由连接两邻点直线的垂直平分线组成的连续多边形组成。具备以下特点 : 

（1）维诺图把平面划分成n个多边形域，每个多边形内 只有 一个生成元；
（2）每个多边形内的点到该生成元距离短于到其它生成元距离；
（3）多边形边界上的点到生成此边界的两个生成元的距离相等；
（4）邻接图形的Voronoi多边形界线以原邻接界线作为子集。
（5）Voronoi图至多有2 * n - 5个顶点和3 * n - 6条边。
（6）多边形内的生成元是形成三边的三点构成的三角形的外界圆圆心，而且所有的这些外界圆内部不含任何除这三点之外的顶点（空心圆特性）。

首先， Delaunay 三角形剖分可通过如下 Python 库生成 : 
```python fold title:
import numpy as np  
import matplotlib.pyplot as plt  
from scipy.spatial import Delaunay  
  
# 生成一批在 [0, 1]x[0, 1] 的正方形内的随机点  
np.random.seed(42)  
points = np.random.rand(15, 2)  # 15 个二维点  
  
# 构建 Delaunay 三角剖分  
tri = Delaunay(points)  
  
# 可视化  
plt.triplot(points[:, 0], points[:, 1], tri.simplices, color='blue')  
plt.plot(points[:, 0], points[:, 1], 'o', color='red')  
plt.gca().set_aspect('equal')  
plt.title("Delaunay Triangulation")  
plt.show()
```

常见的三角形剖分算法是[Bowyer-Watson算法](https://stackoverflow.com/questions/58116412/a-bowyer-watson-delaunay-triangulation-i-implemented-doesnt-remove-the-triangle) 
![[Excalidraw/Voronoi Diagram 及其构造方法 2025-06-09 16.07.22]]

