https://www.youtube.com/watch?v=qqB2HjgmDow
ADPL实际上是Workbench 的底层求解语言, 
在Workbench的Solution > WriteInputFile 可以写入输入文件
![[attachments/Pasted image 20240311094034.png]]
实际上生成的是.dat文件，具体位置一般存储如下:
![[attachments/Pasted image 20240311094422.png]]
这个.dat 文件可以通过APDL来 File > ReadInputfrom 可以直接获取此文件


# 在Workbench结构树中插入APDL命令
Automation 中的Scripting和Python Code界面 (渲染错误时， 往往是和小黑牛冲突， 可以关闭外网后重启程序解决)
![[attachments/Pasted image 20240311090820.png|700]]


基础的方法是
1. 在Solution前一个(例如Transient部分下面加入Commands(APDL)来进行覆盖). > 这个会在Solve之前执行
2. 在Solution下加入, 这个命令是用于后处理的部分;
```FORTRAN
!   Commands inserted into this file will be executed just prior to the ANSYS SOLVE command.  这个在Solve之前执行, 是在transient 下的
!   Commands inserted into this file will be executed immediately after the ANSYS /POST1 command. 是Solve之后执行的语句, 是在Solve下
```

![[attachments/Pasted image 20240311093855.png]]
例如, 可以通过
```Fortran
ET,1,PIPE288   ! Element Type 1 is defined as PIPE288 element
my_test_Se_BTU = 1000    !定义一个新的变量为1000
```
可以在下方的Results中找到自定义的变量, 另外也可以通过Get获取对应的变量; 
![[attachments/Pasted image 20240311104041.png]]

然后选取Execute Post Commands
![[attachments/Pasted image 20240311105829.png]]

```FORTRAN
/SHOW,PNG  ! 设置输出图片的格式为PNG
/VIEW,1 , 0.58, 0.27, 0.46 
/REPLOT  ! 重新绘制图形
/SHOW, CLOSE  ! 关闭图形窗口并保存图片
```

# 参数集的添加方法

参数集的添加方法如下: 在Command中的ARG1点击P使得参数输入被激活
![[attachments/Pasted image 20240311111812.png]]
此时， 项目部分会显示 Parameter 部分, 
![[attachments/Pasted image 20240311111917.png]]
则如图即得到对应的参数部分, 此时只要在下面的Expression中输入另一个命令集的参数， 即可实现在命令中修改参数集共享的参数。
![[attachments/Pasted image 20240311112311.png]]
# Ansys 命令流讲解
Ansys 命令包括Specification commands  和 operation Commands 两类, 而



![[attachments/Pasted image 20240311114845.png]]

对应代码的显示部分会在工程文件-> dp0 -> SYS? -> file0.err中存储， 例如写一个IF 不判断条件, 则file0.err 中会得到相应的报错信息; 
![[attachments/Pasted image 20240311115954.png]]

一般而言， 可以使用VsCode或者notepad++打开.err文件, 来获取其错误输出;
在notepad++中配置update silently 即可实现自动更新文件，不需要手动确认; 
![[attachments/Pasted image 20240311121112.png]]
此时只要切回屏就可以自动更新了; 
