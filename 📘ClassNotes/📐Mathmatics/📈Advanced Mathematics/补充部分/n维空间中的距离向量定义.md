
`````ad-tip
title: $^*n$维向量空间中的距离定义
距离定义为
$$||x-y||_{R_2} =\sqrt{(x_1 -y_1)^2 + (x_2 -y_2)^2 +\ldots + (x_n - y_n)^2}$$
上式为欧几里得距离
````ad-abstract
title: 常见的几种距离
collapse: close
```ad-cite 
title: 曼哈顿距离
$$d = \sum^{n}_{k=1}|x_{1k} - x_{2k}|$$
```

```ad-cite
title: 欧几里得距离
$$d =\sqrt{(x_1 -y_1)^2 + (x_2 -y_2)^2 +\ldots + (x_n - y_n)^2}$$
```

```ad-cite
title: 切比雪夫距离
$$d = \max (|x_{1i}- x_{2i}|)$$
```
![[第九章 多元函数的微分法及应用 2023-02-03 12.47.10.excalidraw|400]]
```ad-cite
title: 闵可夫斯基距离
闵可夫斯基距离是对多个距离的概括表达
$$d_{12} = \sqrt[p]{\sum^{n}_{k=1}|x_{1k} -x_{2k}|^p}$$
其中: $p=1$为曼哈顿距离，$p=2$为欧几里得距离, $p\to\infty$时为切比雪夫距离
```
可以参考文章: 
[常见的距离以及定义](https://zhuanlan.zhihu.com/p/507711961)
````
`````
