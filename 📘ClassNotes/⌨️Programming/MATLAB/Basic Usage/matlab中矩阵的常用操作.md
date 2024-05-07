常用UI如下, 比较方便查找和变量查看， 同可以进行文档阅读和调试;
![[attachments/Pasted image 20240308180734.png|1500]]

# 行列操作
```erlang title:MATLAB给矩阵某一列+1
% 实际上是加上一个向量[0,1,0,0]
```

# 布尔矩阵的创建
```erlang
calculated_vec = false(1,Mesh.node_number);  % 创建布尔矩阵，存储某个节点是否已经被计算过
```


```erlang
% surf(X,Y,)
% 对于绘制三角形，可以使用trimesh函数
% trimesh(T, x,y,z "FaceColor","")
```

# 保留小数
Retain two decimal places
```
% 方法1 roundn = @(x,n) 10.^n .* round(x/10.^n); roundn(x, -5)
round(x, 5);
```

