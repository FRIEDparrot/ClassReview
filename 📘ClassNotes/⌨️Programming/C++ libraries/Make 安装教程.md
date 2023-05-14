
首先下载和安装cygwin64.exe [下载地址](https://www.cygwin.com/install.html)

在Cygwin64中，可以打开win在安装过程中选取make包进行安装， 
将显示包改成Full, 搜索make然后右键把skip改成安装4.4版本


![[attachments/Pasted image 20230513215716.png]]

除了make软件包，还需要安装binutils软件包，

安装make完成之后，使用cygwin64-terminal，cd进入gsl源码对应的文件夹， 执行以下代码进行编译 : 

```Bash
./configure
```

这个命令会检查系统环境，并生成一个Makefile，用于编译和安装GSL库。
运行以下命令来编译GSL库

```bash
make
make check # 测试GSL库
make install # 安装GSL库
```

1.  这个命令会将GSL库安装到默认的安装路径中。如果您想将GSL库安装到其他路径，可以使用`--prefix`选项来指定安装路径。例如，以下命令将GSL库安装到`C:\gsl`目录中：

```bash
./configure --prefix=C:/gsl make make check make install
``` 

完成之后即可添加默认的路径，使用gsl库了
