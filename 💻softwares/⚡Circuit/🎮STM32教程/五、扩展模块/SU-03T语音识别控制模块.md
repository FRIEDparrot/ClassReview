### (1) SU-03T管脚定义
![[attachments/Pasted image 20240503214728.png]]
在使用时, SU-03T的部分, SPK+, SPK-接入喇叭, M-, M+ 接入咪头, 另外还需要接入CH340的USB转串口的模块;
![[attachments/Pasted image 20240502193458.png|600]]
具体接线图如下图所示(只需要CH340转串口部分):
![[attachments/IMG_20240502_195405.jpg|400]]
在烧录时一定要断电, 点击烧录之后上电, 下载完成如图所示:
![[attachments/Pasted image 20240502202040.png|700]]

### (2) SU-03T Linux 二次开发环境搭建
参考 SU03T二次开发环境搭建配套文档（外部版本)
首先， 参考[VMWare安装教程](https://blog.csdn.net/weixin_52799373/article/details/124324077) 在电脑上安装VMWare虚拟机 (Ubuntu参考 [Ubuntu配置](https://blog.csdn.net/Javachichi/article/details/131572766)) 注意: 我配置的是18版本, 20,22版本的相同;

然后参考 [跨虚拟机拖拽文件](https://blog.csdn.net/yakamoz423/article/details/128817915) 并使用nano编辑器删除WayLandEnable前面的注释, ctrl + O保存
```shell
# 先安装open-vm-tools
sudo apt install open-vm-tools
sudo nano /etc/gdm3/custom.conf   # 使用gedit 也可以
# 然后删除下面的WayLandEnable前面的注释, 保证是false
```

在 Settings 中,使用Shared Folders, 设置为 Always Enabled 
![[attachments/Pasted image 20240504095757.png|700]]
最后， 一定要安装open-vm-desktop这一选项,
```shell 
sudo apt-get install open-vm-tools-Desktop
# 然后重启
reboot
```
然后就可以跨虚拟机移动和复制粘贴文件了
![[attachments/Pasted image 20240504100057.png]]

我们把对应的 Andestech.tart.gz 放到 /opt 文件夹下, 然后解压:
```shell
sudo tar -xzvf Andestech.tart.gz
```

解压完毕后保留Andestech文件夹, 如下图:
![[attachments/Pasted image 20240504102256.png]]

### 系统目录和用户目录的访问
在文件夹中, Home默认是用户目录， 并使用 ~ 代替, 例如 ls -l ~/opt 是返回用户目录下的内容
![[attachments/Pasted image 20240504123036.png|700]]
但是我们需要使用系统的目录, 方法是打开Other locations > Computer, 在这里的才是根目录, 需要将Andestech交叉编译文件夹放置到这里并配置环境变量
![[attachments/Pasted image 20240504123227.png]]
我们可以使用sudo权限, 例如将用户下的opt/Andestech移动到系统的opt/Andestech:
```
sudo mv ~/opt/Andestech /opt/
```
然后在系统的opt文件夹里面就有Andestech了, 此时再输入 /opt/Andestech, 提示Is a Directory,配置成功!
![[attachments/Pasted image 20240504123720.png]]
### 3. 使用vim配置环境变量
```
sudo apt-get install vim # 安装vim编辑器 
sudo vi /etc/profile         # 使用vim打开
03、按住键盘Shift+i使文本可以编辑：按Enter进行换行，在文档最后添加如下语句：               

（（I 为插入的意、思），此时鼠标在第一行的第一个位置；shift+a 和 shift+i 的区别是：i 为光标前插入，a 为光标后插入）
```

![[attachments/Pasted image 20240504102635.png]]
加入这一句:
```
export PATH=$PATH:/opt/Andestech/BSPv422/toolchains/nds32le-elf-mculib-v3s/bin
```
然后更新etc/profile:
```
source /etc/profile  
是一个 shell 命令，用于重新加载系统环境变量文件 /etc/profile，以便使修改后的环境变量生效。
在 Linux 系统中，环境变量是指在整个系统中都可以使用的全局变量，用于存储一些常用的路径、命令别名、默认设置等。环境变量文件 /etc/profile 是系统级别的环境变量配置文件，它包含了系统全局的环境变量定义，通常被所有的登录用户所共享。
当我们需要修改系统级别的环境变量时，可以通过编辑 /etc/profile 文件来进行修改。但是，修改后的环境变量并不会立即生效，需要使用 source 命令重新加载 /etc/profile 文件，以便使修改后的环境变量立即生效。
```

### 3. 获取root权限并安装make
```cpp
su  root # 使用root权限 -> 报错 , sudo passwd root 输入两次密码
su root   # su friedparrot 切换回来
```
最终获得了root权限如下图:
![[attachments/Pasted image 20240504105816.png]]

安装make如下:
```
sudo apt-get install gcc automake autoconf libtool make  # 安装make 
```

### 4. 编译sdk
首先在桌面等随便位置新建一个su03t文件夹。 因为前面环境变量已经配置好了

所以直接在智能公元上下载SDK, 最终cd到unione_lite_app_hb_m文件夹
![[attachments/Pasted image 20240504104319.png|800]]
```shell
./build.sh update
```

编译成功:
![[attachments/Pasted image 20240504124519.png]]
最后在自己创建的文件夹中, 打开可以看到编译生成的output文件夹， 里面就有对应的bin文件了, 编译成功!
![[attachments/Pasted image 20240504124705.png]]
