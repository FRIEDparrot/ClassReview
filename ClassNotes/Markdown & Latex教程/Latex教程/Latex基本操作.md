### 一、文章开头常用的包和文章类型
开头
```Latex
\documentclass{article}    % it can also use beamer
\documentclass[options]{class-name}
\documentclass[UTF8]{ctexart}  % 用于编译中文
```

article [[Pasted image 20221115213445.png]]
options[[Pasted image 20221115213605.png]]

常用的包类型

```Latex
\usepackage{amsmath, amssymb}  % fundamental math packages
% American Math Society math and symbol 
\usepackage{tikz}    % Use the package of the MikTex(Then the package of Miktex will be used)
\usepackage{geometry}   % use the packages ---> the most fundamental package is the geometry package  (don't care about the not found since it actually works)  --> if new package are needed, we can use Miktex consol to install it.
\usepackage{graphicx}   % embed *.jpg pictures and *.png pictures
\usepackage{ulem}   % this is for using the uuline and uwave
\usepackage{array}	
\usepackage{verbatim}
```

之后
```Latex
\begin{document}
content....
\end{document}
```
即可正常运行

### 二、注释

```Latex
\usepackage{verbatim}

\begin{comment}
******	this multi-line comment need using the verbatim package ******
	this can also be recognized as comment
	and we also use this method to give a multi-line comment
\end{comment}
```


