[NX二次开发教程](https://blog.csdn.net/weixin_54698498/article/details/134772876)
西门子NX/UG Python api开发文档：nxopen_python_ref/namespaces.html。
西门子NX/UG C API开发文档：ugopen_doc/index.html。
西门子NX/UG C++ API开发文档：open_c++_ref/namespaces.html

[NXOpen文档](https://docs.plm.automation.siemens.com/data_services/resources/nx/10/nx_api/en_US/custom/nxopen_python_ref/NXOpen.html)
https://docs.plm.automation.siemens.com/data_services/resources/nx/12/nx_api/custom/en_US/nxopen_python_ref/index.html
[How to execute NXOpen Python code from any command prompt? (siemens.com)](https://support.sw.siemens.com/en-US/okba/PL8750551/How-to-execute-NXOpen-Python-code-from-any-command-prompt/index.html)
https://docs.sw.siemens.com/en-US/doc/209349590/PL20221117716122093.nxopen_prog_guide/xid1124926

UG中的python基本调用方法: https://www.youtube.com/watch?v=r81Pc13oNjo 
基本API 的使用方法是`pip install nxopen --upgrade`, 但是不需要

# 一、Eclipse pydev 环境配置方法
首先， UG 中**本来就提供了直接运行python脚本的方案， 即使用alt + F8直接打开某个python脚本并且运行**;
```py title:示例程序
import NXOpen
session = NXOpen.Session.GetSession();
lw = session.ListingWindow;
lw.Open()
lw.WriteLine('Hello, World')
```
但是， 实际上的python实体是`E:\NX12\NXBIN\run_journal.exe.` 可以使用UG的command prompt 打开。 UG自带的python解释器不具备模块扩展能力， 因此需要配置环境换成当前电脑的python配置。

先将`E:\NX12\NXBIN\python` 添加到系统环境变量的Path中。`NX_HOME=E:\NX12\NXBIN` 

在pycharm中配置变量， 使用File > Settings > Project > Project Interpreter > Show All 
![[attachments/Pasted image 20240315180159.png]]
由于python3.9和这个配置不兼容, 因此将python环境改为3.6, 如下图:
![[attachments/Pasted image 20240315223750.png|900]]
选择Show Path for the selected Interpreter , 然后**添加`E:\NX12\NXBIN\python`环境变量**(其他两个不加也可);
![[attachments/Pasted image 20240315200413.png]]
然后在Project Structure 中添加环境， 如下图: NXbin\python 
![[attachments/Pasted image 20240315213842.png|900]]

<b><mark style="background: transparent; color: blue">需要说明的是， Pycharm 中不能给出提示, 但是可以使用Eclipse编辑器</mark></b>

参考[[💻softwares/🗽2D & 3D design/UG二次开发/NXOpen.NET/NXOpen Python/Debug_NXOpen_Python_with_NX12_and_Eclipse_PyDev.pdf|Debug_NXOpen_Python_with_NX12_and_Eclipse_PyDev.pdf]]配置eclipse即可, 注意一定要选用3.6为配置环境。
成功效果图如下: 
![[attachments/Pasted image 20240315224041.png]]


# UG 中修改路径使得Alt + F8可以执行对应编译器
参考[文档](https://docs.sw.siemens.com/en-US/doc/209349590/PL20221117716122093.nxopen_prog_guide/xid1124926)修改下述环境变量， 我在`E:\NX12\UGII`中的 ugii_env.dat 进行修改来覆盖原来的环境变量:

注意下面的全改成3.6的路径: 

| 环境变量                    | 变量值                                                                                                                      |
| ----------------------- | ------------------------------------------------------------------------------------------------------------------------ |
| UGII_BASE_DIR           | E:\NX12    (一般已经设置好了， 不用继续设置)                                                                                            |
| UGII_ROOT_DIR           | ...... \UGII                                                                                                             |
| UGII_PYTHON_HOME        | C:\Users\Parrot\python                                                                                                   |
| UGII_PYTHON_DLL         | python39.dll                                                                                                             |
| UGII_PYTHON_LIBRARY_DIR | C:\Users\Parrot              (注意是python39.dll路径, 可能在System32下)                                                           |
| UGII_PYTHONPATH         | C:\Users\Parrot;C:\Users\Parrot\DLLs;C:\Users\Parrot\Lib;C:\Users\Parrot\Lib\site-packages;%UGII_BASE_DIR%\NXBIN\python; |

```py title:截取自默认的ugii_env_ug.dat
UGII_PYTHON_HOME=${UGII_BASE_DIR}\nxbin\python
UGII_PYTHON_DLL=python36.dll
UGII_PYTHON_LIBRARY_DIR=${UGII_PYTHON_HOME}
UGII_PYTHONPATH=${UGII_PYTHON_HOME};${UGII_PYTHON_HOME}\Python36.zip
```

```py
#
#     Copyright 1991 - 2013 Siemens Product Lifecycle Management Software Inc.
#                                   All Rights Reserved.
#
###############################################################################
# 
#
# This file should be used to override any standard NX environment variables.
# The values defined here will take precedence over the NX values. This file 
# will not be overwritten by an MR installation. 
#
# Override any variables here.

UGII_PYTHON_HOME=E:\Python36
UGII_PYTHON_LIBRARY_DIR=E:\Python36
UGII_PYTHON_DLL=python36.dll
UGII_PYTHONPATH=${UGII_PYTHON_HOME};E:\Python36\DLLs;E:\Python36\Lib;E:\Python36\Lib\site-packages;${UGII_BASE_DIR}\nxbin\managed;${UGII_BASE_DIR}\nxbin\python
#
```

注意: 上述设置会导致NX卡住无法进行，因此我们仅使用外部的方法, 
# 安装第三方库
方法是直接在eclipse 中进行安装, 我们以numpy为例: 
注意不要更新pip 
![[attachments/Pasted image 20240315234721.png]]

简单示例代码如下:
```python
import NXOpen as nx
import numpy
import os


def main():
    s = nx.Session.GetSession();
    lw = s.ListingWindow;
    lw.Open()
    lw.WriteLine('Hello, World')

if __name__ == "__main__":
    main()
```

# 强制动态分析(代码补全过滤)

![[attachments/Pasted image 20240316163149.png]]
当Import 对应类时， 我们可以在 Force BuiltIns 中 添加对应的类, 这样就能够让编辑器强制解析， 并给出对应的提示,  给出库的动态信息来提高代码补全的准确率。

![[attachments/Pasted image 20240316163128.png]]

```python
NXOpen
NXOpen.Annotations
NXOpen.Assemblies
NXOpen.CAM
NXOpen.CAE
NXOpen.Drafting
NXOpen.Drawings
NXOpen.Features 
NXOpen.BlockStyler
NXOpen.UF
NXOpen.Display
NXOpen.Fields
NXOpen.Gateway
NXOpen.GeometricUtilities
NXOpen.Issue 
NXOpen.Report
```

具体的模块可以在[链接](https://docs.plm.automation.siemens.com/data_services/resources/nx/12/nx_api/custom/en_US/nxopen_python_ref/index.html)找到， 里面列出了所有的packages, 并且均可以设置为解析
