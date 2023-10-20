可以通过Delaunay三角剖分的数据结构来获取三角形的边的信息。

具体来说,您可以:

1. 获取三角形的迭代器:

```cpp
Delaunay_triangulation_2<Kernel>::Finite_faces_iterator fit = dt.finite_faces_begin();
```

2. 遍历所有三角形:

```cpp
for ( ; fit != dt.finite_faces_end(); ++fit) {
   // Get info for each triangle 
}
```

3. 对于每个三角形,获取其三条边:

```cpp 
Delaunay_triangulation_2<Kernel>::Edge e1 = fit->edge(0);
Delaunay_triangulation_2<Kernel>::Edge e2 = fit->edge(1); 
Delaunay_triangulation_2<Kernel>::Edge e3 = fit->edge(2);
```

4. 对于每个边,获取其两个端点:

```cpp
Point p1 = e1.first->vertex((e1.second + 1) % 3)->point(); 
Point p2 = e1.first->vertex((e1.second + 2) % 3)->point();
```

5. 您可以存储这些信息(端点和边),或者直接使用它们。

以上代码会为每个三角形获取其三条边,并获取这三条边的两个端点。

完整的示例代码如下:

```cpp
for (Delaunay_triangulation_2<Kernel>::Finite_faces_iterator fit = dt.finite_faces_begin(); 
     fit != dt.finite_faces_end(); 
     ++fit) {
   Delaunay_triangulation_2<Kernel>::Edge e1 = fit->edge(0);
   Point p1 = e1.first->vertex((e1.second + 1) % 3)->point(); 
   Point p2 = e1.first->vertex((e1.second + 2) % 3)->point();
   // Similarly get info for e2 and e3
}
```

希望这些信息能够帮助您获取三角形的边的信息。如果您有任何疑问,请随时联系我。