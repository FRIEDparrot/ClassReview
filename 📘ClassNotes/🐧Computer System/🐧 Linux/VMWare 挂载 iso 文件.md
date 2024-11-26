首先, 对于 vmtools, 首先需要下载对应的 tar.gz 或者 deb 包,  

然后采用 iso creator 将其转化为 iso 镜像文件, 然后直接挂载到 Linux 机上 

对于 deb 文件: 采用: 
```sh
sudo dpkg -i  template.deb 
```

对于 tar.gz 文件, 直接采用 tar 进行解压: 

```sh
tar -xzvf template.tar.gz
```

使用 apt-cache depends 查看某个软件包的依赖项，

```sh
sudo apt-get install apt-offline
```
例如, 执行
```sh
sudo apt =-get download make gcc g++
sudo dpkg -i * .deb
```

对于 C 编译器不工作, 编辑 `~/.bashrc`, 添加路径:
```sh
export PATH=/usr/local/bin:$PATH
source  ~/.base
```

![[attachments/Pasted image 20241005195059.png]]


```sh
mkdir testdir 
cd testdir 
apt-get download $(apt-cache depends --recurse [package-name] | grep -v i386 | grep "w")
```

### 安装 python 3 缺少zlib 解决方案

这个错误通常是由于缺少 `zlib` 库引起的。你可以按照以下步骤来解决这个问题：
1. **安装 `zlib` 依赖**：
    - 在有互联网连接的计算机上，下载 `zlib` 及其开发包： 
```bash
sudo apt-get install zlib1g zlib1g-dev
```      
2. **重新编译 Python**：
    - 确保在编译 Python 时包含 `zlib` 支持。进入 Python 源码目录并运行以下命令：
        ```bash
        ./configure --with-zlib=/usr/include
        make
        sudo make install
        ```
[通过这些步骤，你应该能够解决 `zipimport.ZipImportError: can't decompress data; zlib not available` 错误。如果问题仍然存在，请告诉我更多详细信息，我会尽力帮助你解决](https://www.cnblogs.com/Jimc/p/10218062.html)[1](https://www.cnblogs.com/Jimc/p/10218062.html)[2](https://blog.csdn.net/weixin_30619101/article/details/95437485)[3](https://blog.csdn.net/younger_china/article/details/131958223)。




```sh
ip addr # 查看当前 ip 地址部分 
sudo ip addr add 192.168.31.100/24 dev ens33
sudo ip route add default via 192.168.31.1 dev ens33
```

**自动配置网关地址**:
```sh
sudo dhclient ens33
ping 8.8.8.8
```

更新 ubuntu 版本(更新所有软件包):
```sh
sudo do-release-upgrade
```



## Ubuntu 版本更新
要更新虚拟机中的 Ubuntu 系统并保留现有的数据，你可以按照以下步骤进行操作。这个过程会更新系统到最新的软件包和安全补丁，同时尽量避免数据丢失：

### 1. 备份重要数据
虽然正常的系统更新不会影响到用户的数据，但为了以防万一，建议你先备份重要的文件和配置：

- 将重要的数据文件、配置文件复制到外部存储（如 USB 驱动器）或上传到云存储。
- 你也可以使用 `rsync` 或 `tar` 命令创建备份：

  ```bash
  sudo rsync -avh /path/to/important/data /path/to/backup/
  ```

### 2. 更新当前系统的软件包
执行以下命令来更新系统中已经安装的所有软件包：

```bash
sudo apt update
sudo apt upgrade
```

- `sudo apt update` 会更新软件包索引，确保你获取到最新的软件包列表。
- `sudo apt upgrade` 会升级所有已安装的软件包，但不会删除或安装新的软件包。

### 3. 进行全面升级（可选）
如果你想让系统进行更彻底的更新，包括可能需要移除或安装额外的软件包，可以使用 `dist-upgrade`：

```bash
sudo apt dist-upgrade
```

- `dist-upgrade` 不仅升级软件包，还会处理软件包之间的依赖关系，确保所有软件包在最新的版本下能够正常工作。

### 4. 清理不再需要的软件包
清理升级后不再需要的旧包和缓存，可以腾出空间并保持系统整洁：

```bash
sudo apt autoremove
sudo apt clean
```

- `sudo apt autoremove` 会移除不再使用的依赖包。
- `sudo apt clean` 会清理下载的软件包缓存，释放存储空间。

### 5. 升级到新版本的 Ubuntu（可选）
如果你想将系统升级到一个新的 Ubuntu 发行版本（例如从 Ubuntu 18.04 升级到 Ubuntu 20.04），可以使用以下命令：

```bash
sudo do-release-upgrade
```

- 这个命令会自动检测并下载最新的 LTS 版本，升级过程会下载新版本的所有软件包并替换旧的软件包。

### 6. 重启系统
更新完成后，最好重启系统以确保所有更新都已生效：

```bash
sudo reboot
```

### 7. 验证更新
重启后，你可以检查 Ubuntu 的版本号，确认更新已经成功：

```bash
lsb_release -a
```

这将显示当前安装的 Ubuntu 版本信息。

### 注意事项
- 系统更新和升级通常不会影响用户的文件和数据，但一些应用的配置文件可能会被覆盖，因此备份很重要。
- 确保在升级前有足够的磁盘空间。
- 如果你使用的是虚拟机的快照功能（如 VMware 或 VirtualBox），可以在升级前创建一个快照，这样如果出现问题，可以恢复到升级前的状态。

通过这些步骤，你可以安全地更新 Ubuntu 系统，同时保留现有的数据。


要在 Linux 虚拟机上安装 `marktext.AppImage` 并设置为默认打开 Markdown 文件，可以按照以下步骤操作：


## AppImage 安装
### 1. 下载 `marktext.AppImage`
首先，在 Linux 虚拟机中下载 `marktext.AppImage` 文件。你可以从 MarkText 的[官方 GitHub 页面](https://github.com/marktext/marktext/releases)下载最新的 `.AppImage` 文件。

### 2. 赋予执行权限
下载完成后，打开终端，导航到存放 `marktext.AppImage` 文件的目录，然后运行以下命令来赋予执行权限：
```bash
chmod +x marktext.AppImage
```

### 3. 运行 `marktext.AppImage`
运行以下命令来启动 MarkText：
```bash
./marktext.AppImage
```

这会启动 MarkText。如果需要，可以将其移动到系统的 `~/Applications` 目录中，便于管理：
```bash
mkdir -p ~/Applications
mv marktext.AppImage ~/Applications/
```

然后，你可以通过以下命令启动 MarkText：
```bash
~/Applications/marktext.AppImage
```

### 4. 设置 MarkText 为默认 Markdown 文件编辑器
要将 MarkText 设置为 Markdown 文件（例如 `.md` 文件）的默认打开程序，可以执行以下步骤：

1. **右键设置（图形界面）**：
   - 如果你的 Linux 系统使用图形界面，右键点击一个 `.md` 文件，选择 "属性"。
   - 在 "打开方式" 选项中选择 MarkText。如果 MarkText 不在列表中，手动浏览并选择 `~/Applications/marktext.AppImage`。

2. **使用命令行设置**：
   打开终端，运行以下命令：
   ```bash
   xdg-mime default marktext.desktop text/markdown
   ```
   这里 `marktext.desktop` 是你为 MarkText 创建的桌面启动文件（见下面步骤）。

3. **创建 `marktext.desktop` 文件**：
   如果使用第二种方式，你需要创建一个 `.desktop` 文件来表示 MarkText。可以使用以下命令：
   ```bash
   nano ~/.local/share/applications/marktext.desktop
   ```
   在文件中添加以下内容，然后保存退出：
   ```ini
   [Desktop Entry]
   Name=MarkText
   Exec=/home/your-username/Applications/marktext.AppImage %U
   Terminal=false
   Type=Application
   Icon=marktext
   MimeType=text/markdown;
   请将 `/home/your-username/Applications/marktext.AppImage` 替换为你的实际文件路径。
```

### 5. 更新桌面数据库
更新桌面文件数据库，以便系统识别新的 `.desktop` 文件：
```bash
update-desktop-database ~/.local/share/applications
```

### 6. 测试
现在双击一个 `.md` 文件，系统应该会自动使用 MarkText 打开它。

通过这些步骤，你可以在 Linux 虚拟机上安装 `marktext.AppImage` 并设置为默认的 Markdown 文件编辑器

File Printer 

![[attachments/Pasted image 20241016144737.png]]

![[attachments/Pasted image 20241016145251.png]]

