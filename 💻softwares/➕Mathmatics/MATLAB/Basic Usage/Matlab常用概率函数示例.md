常用分布函数 pdf 为概率密度函数, cdf 为分布函数: 
```erlang
unifpdf(x, a, b);               % 均匀分布 
normpdf(x, mu, sigma);  % 正态分布 
exppdf(x, mu);                 % 指数分布
```


```erlang title:示例代码
x = -3:0.01:3;
y = normcdf(x,0, 1);
y2 = normpdf(x, 0,1);
hold on
plot(x,y);
plot(x,y2);
```

![[attachments/Pasted image 20240226155419.png]]

对于多维数组， 想要获取某一维的长度， 只需在size中加上参数
```erlang 
size(Mat, 2);      %获取行数(不写默认列)
```


```erlang 
skewness  % 返回偏度
kurtosis     % 返回峰度
```

例如， 生成一个正态分布并求解峰度和偏度(峰度约为3, 偏度约为0)
```erlang
x = randn(100000,1);
s = skewness(x); 
```
