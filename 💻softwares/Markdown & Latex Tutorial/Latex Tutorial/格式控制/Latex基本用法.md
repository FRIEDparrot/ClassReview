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


## 三、Latex文献引用方法


```LaTeX
\begin{thebibliography}{99}
	\bibitem{ref1} 			
\end{thebibliography}
```

对于Zotero的引用，直接将对应的文献拖入Latex中即可， 需要去掉$[J]$后面的OL，以及DOI格式

可以在Zeotero-> 受选项 -> 导出->格式中更改引用的格式




# 四、对齐, 公式与表格控制
```Latex
\documentclass[UTF8]{ctexart}    % it can also use beamer

\usepackage{amsmath, amssymb}  % fundamental math packages
% American Math Society math and symbol 
\usepackage{tikz}    % Use the package of the MikTex(Then the package of Miktex will be used)
\usepackage{geometry}   % use the packages ---> the most fundamental package is the geometry package  (don't care about the not found since it actually works)  --> if new package are needed, we can use Miktex consol to install it.
\usepackage{graphicx}   % embed *.jpg pictures and *.png pictures
\usepackage{ulem}   % this is for using the uuline and uwave
\usepackage{array}	
\usepackage{verbatim}

\begin{comment}
******	this multi-line comment need using the verbatim package ******
	this can also be recognized as comment
	and we also use this method to give a multi-line comment
\end{comment}

\begin{comment}
	the latex document has 2 basic parts: 
	first is preamble and the second part is the document
	first part of the Latex contains basic parameters for the document
\end{comment}

\begin{document}
	\begin{center}
		{\Huge Big Center Text}
	\end{center}

	\begin{center}
		\includegraphics[width=0.3\linewidth]{parrot}
	\end{center}
\textbf{\large{Part 1. Basic Text settings and the style settings}}

	\large  I. Text basics
	\tabularnewline	This is the text of {\LARGE the content} you are currently reading
	if you need to large a part of the text, you can surround the text you need to large in curly brackets
	
	the method to change the \textit{italic Text} and the \textbf{BOLD TEXT} or \underline{underline texts} is add the corresponding command before the brackets  
	
	\uuline{This is another example} \uwave{using the ulem package}
	
	The three default font in the Latex are \textrm{Roman}, \textsf{Sans serif} and the \texttt{typewriter}
	\\
	
	\large II. Text Justification/Alignment
	
	 the default adjust method is the fully justified Text.
	\begin{center}
		center text
	\end{center}
	\begin{flushleft}
		left align
	\end{flushleft}
	\begin{flushright}
		RIGHT
	\end{flushright}

	we can also use a double backslash for a new line\\
	this will be compiled as a new line\\[2\baselineskip]  % we can use this to skip different baselines 
	
	Latex indents new paragraphs automatically
	
	\noindent if we want to not indent this paragraph before the code, use the \verb*|\noindent| command % use the \verb to write code in your article
	
	if you want to change indent of the entire file, you can use the \verb*|\setlength{parindent} {1cm}| or \verb*|\setlength{partindent} {15pt}| in preamble or at the start of document.
	
	
	
	\large III. Tables
	
	\begin{table}[h!]
		\begin{center}    % we must put the table under the title of the table
			\caption{First Test Table}
			\begin{tabular} {l|c|c|r} % use this line to make the alignment of the tablet elements --which 1st col is left 2,3 cols are center and 4 col is right 
				\textbf{$x_1$} & \textbf{$x_2$} & \textbf{$x_3$} & \textbf{y} \\
				$\alpha$ & $\beta$ & $\gamma$ & $\zeta$ \\
				\hline
				1 & 0.5 & 0.8 & 2 \\
				2 & 0.7 & 0.5 & 3 \\
			\end{tabular}
		\end{center}
	\end{table}


\textbf{\LARGE{Part 2. Basic mathematical Manipulations }}

There are two math mode in the Latex -- That is, Display style math and inline or text style math

The display math create a new zone and put the math formula in the display and Inline math mode create the math formula using a inline mode

There are two ways to create the display math and the first is
\[f(x) = x^2 + 3x + b\]
another is
\begin{align*}   % the asterisk is to eliminate the automatic numbered
	& -m\omega^2x^2 + 2\zeta \omega x+kx^2 = f_0cos(\omega t) \\
	& kx = c\dot{x}
	% in this line, the ampersand sign "&" tells the document what the alignment to be.
\end{align*}

\begin{align} \textstyle   % force it to display as a inline style
	f_1(x) = k*x + b\\   % This will be automatically numbered
	 f2(x) = b \nonumber
\end{align}


\begin{align*}
	\cdot \dot{a} \times \frac{num}{den}\\
	\left( \right.
\end{align*}

This the using method of the \(math formula1\) and $f2(x) - the second$ and if you want to make it a variable, use the previous format
	
also we can use \verb*|  \dfrac{num}{den}  and \tfrac{num}{den}| to force the Latex use inline or displaced styles

if we only want a side bracket we can use \verb*| \left( \right. | the right must be added but if we don't want it use dot to replace the bracket

we can also use \verb*|\text{this is a string}| command to write the formula


\Large Part 3. Tables and Arrays
There are two environment to write the Table $\rightarrow$ that is, the table environment and the tabular environment.

\begin{table}
	\caption{This is a Table}
	\begin{center}
	\begin{tabular} {l||r|l||r}
		\hline
		s1 & s2 & s3 & s4 \\
		\hline
		a1 & a2 & a3 & a4 \\
		\hline \hline
		
		% use the ampersand sign to jump to the next cell and use the double backslash to start a new row
		
		% use the verticle bar into the align definition
		% to create the horizontal line to the table, use the \hline command
	\end{tabular}
	\end{center}
\end{table}

\begin{tabular}{c}   % we can use {array} package to create arrays
\end{tabular}

use this method to make a correct array: \\
\begin{center}
	$\left[\begin{array}{c|c|c}
		a_{11} & a_{12} & a_{13} \\
		a_{21} & a_{22} & a_{23}
	\end{array}\right]$
\end{center}
also, the environment \verb*|pmatrix and bmatrix| are also available

we can use \verb*|\multicolumn{4} {Center Merged} or {Left Align Merged}  |

\begin{center}
你好，世界!
\end{center}

\begin{comment}
	\begin{aligned}
		$$\nu_t = \frac{k}{\tilde{\omega}}$$
	\end{aligned}
\end{comment}


\end{document}
```