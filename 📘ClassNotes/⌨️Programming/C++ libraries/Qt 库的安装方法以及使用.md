Qt库是一个开源的C语言(C++)的GUI图形界面库，对于Ansys等软件，其底层的建模
[Qt库的安装方法](https://www.bilibili.com/video/BV1TS4y1J7TV)
本电脑的安装路径为`E:/Qt`

在安装Qt库之后， 我们可以将Qt库配置到visual Studio

只需在visual studio 的Extensions中，选择安装Qt Tools即可
安装完毕之后在Qt的options中的versions文件夹下，需要选取
`qt/版本号/msvc2017.64/bin/remark`
找到`qmake.exe`文件并添加即可

接下来就可以使用qt widgets application 创建最简单的应用程序了
