## 一、Delaunay 三角形网格自动剖分算法
### (1) 方法概述
如图为三角形自动划分网格的方法演示
![[Excalidraw/Delaunay triangulation 三角形网格自动划分方法 2023-04-26 23.40.01|300]]
在三角形的三角网格划分中，Delaunay 算法可以让三角形的外接圆均满足空圆性质， 可以使划分网格得到的三角形的最小的角度尽可能最大化

Delaunay 算法的良好性质是，<mark style="background: transparent; color: yellow">对于任意一个三角形，其外接圆不包含任何其他点。</mark>这样的剖分具有良好的性质，如最大化最小角度，最小化三角形边长差异等，使其在许多应用中都有广泛的应用。
Delaunay 算法的具体实现往往是使用逐点插入的方法进行的，具体方法包括<mark style="background: transparent; color: yellow"> Lawson 算法和 Bowyer-Watson 算法</mark>等。

Delaunay三角形划分算法的过程如下：
1.  初始化：将点集中的前三个点构成一个初始三角形（不一定是最终的三角形）。
2.  逐点插入：可以使用<mark style="background: transparent; color: yellow">Lawson 算法或者Bowyer-Watson 算法</mark>依次将点插入到当前三角形剖分中。插入点时，首先找到包含该点的三角形。
3.  调整：以该三角形为基础，将该三角形和它相邻的所有三角形删除，然后重新连接它们，形成一组新的三角形。
4.  终止：当所有点都插入完成后，得到的三角形剖分即为Delaunay三角形剖分。

### (2) Bowyer-Watson 算法划分网格
Bowyer-Waston算法的核心是， 首先生成大三角形，包含所有划分的节点，[Bowyer-Waston算法简介](https://www.youtube.com/watch?v=GctAunEuHt4)

首先我们找到所有节点的最大x,y值和最小x,y值，计算出其外接圆，则显然以该圆为内接圆的三角形即为包含所有节点的超级三角形
如下图， 任意六边形的超级三角形可以如图获取
![[Excalidraw/Delaunay triangulation 三角形网格自动划分算法 2023-04-27 00.58.42|200]]

简要的matlab代码如下: 
```erlang
% 假设原始网格的节点坐标存储在一个n x 2的矩阵P中

% 找到所有节点的最小外接矩形
xmin = min(P(:, 1));
xmax = max(P(:, 1));
ymin = min(P(:, 2));
ymax = max(P(:, 2));

% 计算最小外接矩形的中心点和半径
center = [(xmin + xmax)/2, (ymin + ymax)/2];
radius = max(xmax - xmin, ymax - ymin);

% 选择超级三角形的顶点
p = [center(1), center(2) + 2*radius];

% 创建超级三角形
T = [p(1) - 3*radius, p(2) - radius;
     p(1), p(2) + 2*radius;
     p(1) + 3*radius, p(2) - radius];
```

在调整步骤中，需要保证新生成的三角形满足Delaunay条件，即新生成的三角形的外接圆不包含任何其他点。为了保证这个条件，需要对新生成的三角形进行检查和调整。具体的检查和调整方法可以使用Bowyer-Watson算法等。


## 二、Delaunay 四边形单元剖分算法
对于四边形单元的剖分，常用的算法包括：
1. 四边形单元的Delaunay剖分
2. 四边形单元的最小矩形剖分

在边界外围添加一个大三角形，使得所有的节点都位于这个大三角形内部。这个大三角形的顶点可以选择在边界外围的任意位置，但需要保证大三角形的三条边不与原始网格中的任何边相交。这样可以确保生成的超级三角形不会影响到原始网格的Delaunay性质。

好的，下面是详细的步骤：

1.  找到原始网格中所有节点的最小外接矩形，记为R。
2.  在R的外接圆上选择一个点作为超级三角形的顶点，记为p。
3.  将p与R上相邻的两个点连接，得到三角形T。
4.  将T添加到原始网格中，成为超级三角形。

需要注意的是，在选择超级三角形顶点�p时，需要保证其不在原始网格内部，同时也要避免与原始网格中的任何边相交。一种常见的做法是将�p选择在�R的外侧，并且距离原始网格边界足够远，使得超级三角形不会对原始网格的Delaunay性质产生影响。
