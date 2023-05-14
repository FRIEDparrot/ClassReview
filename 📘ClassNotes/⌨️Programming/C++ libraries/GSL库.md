路径: `C:\gsl-2.7.1`
提供了微积分等等一系列数学库函数
安装方法是使用Project - Manage NuGet Packages 
然后直接搜索gsl 64

![[attachments/Pasted image 20230502121235.png|500]]

![[attachments/Pasted image 20230502121342.png|500]]

之后packages.config 中会出现对应的部分
![[attachments/Pasted image 20230502121431.png|600]]



如果您下载的是GSL源代码的压缩包，那么您需要先进行编译和安装，然后才能在您的项目中使用GSL库。

以下是在Windows系统上编译和安装GSL库的步骤：

1. 下载并解压GSL源代码压缩包。

2. 打开命令提示符，并进入GSL源代码所在的目录。

3. 运行以下命令来生成Makefile：(使用git bash进行生成)

   ```
   ./configure
   ```

   这个命令会检查您的系统环境，并生成一个Makefile，用于编译和安装GSL库。

4. 运行以下命令来编译GSL库：

   ```
   make
   ```

   这个命令会使用Makefile来编译GSL库。编译完成后，您可以运行以下命令来测试GSL库：

   ```
   make check
   ```

5. 运行以下命令来安装GSL库：

   ```
   make install
   ```

   这个命令会将GSL库安装到默认的安装路径中。如果您想将GSL库安装到其他路径，可以使用`--prefix`选项来指定安装路径。例如，以下命令将GSL库安装到`C:\gsl`目录中：

   ```
   ./configure --prefix=C:/gsl
   make
   make check
   make install
   ```

完成上述步骤后，您就可以在Visual Studio中使用GSL库了。在添加GSL库的头文件和库文件时，您需要将它们添加到正确的位置。具体来说，您需要将GSL库的头文件添加到Visual Studio的包含目录中，将GSL库的库文件添加到Visual Studio的库目录中，并将GSL库的名称添加到链接器的附加依赖项中。