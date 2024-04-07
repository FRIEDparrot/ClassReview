
行间距除了可以用setspace宏包以外， 还可以使用如下方法:

```tex
%% 全局选项调整 
\linespread{1.5} 

% 段落选项调整
\setlength{\baselineskip}{1.3\baselineskip}   % 调整段落行距, 注意每一部分都要加\\
\vspace{20pt} % 两段之间插入20pt行间距

% selectfont 用于刷新间距信息
\linespread{2.0} \selectfont   
```
