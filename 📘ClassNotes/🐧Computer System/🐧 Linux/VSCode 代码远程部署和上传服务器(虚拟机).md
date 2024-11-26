vscode 中采用 ctrl + shift + v 进行预览html
ctrl + shift + i 采用 inspect 
采用  ctrl  + shift + p 打开命令行

### (1) 下载插件和添加服务器 
首先在 vscode 上需要下载 Remote 插件和 Docker 插件
![[attachments/Pasted image 20241024142324.png]]
采用 Remote-SSH -> Add New SSH Host,   
![[attachments/Pasted image 20241024142540.png]]

采用如下方式添加服务器 :
```sh
ssh user@your_server_ip
``` 
其中 your_server_ip 使用 ens33 的 inet 地址, 也可以采用 DNS 地址 (适用于带有域名的网站的情况)

### (2) 在服务端打开监听
1. 安装服务器和打开监听
首先, 在服务器端需要安装 openssh 服务器 :
```sh 
sudo apt update 
sudo apt install openssh-server
```
服务器端需要打开 ssh 监听: 
```sh
sudo systemctl status ssh
```
需要补充的是,  如果服务没有启动，可以用下面的命令启动：
```bash
sudo systemctl start ssh
```
让 `ssh` 开机自启动：
```bash
sudo systemctl enable ssh
```

2. **检查防火墙设置**
- 如果服务器启用了防火墙，请确保防火墙允许 SSH 连接（默认端口是 22）。可以使用以下命令检查并打开 SSH 端口：
- 使用 `ufw`（常用于 Ubuntu）：
```bash
sudo ufw allow ssh
sudo ufw status
```
- 使用 `firewall-cmd`（CentOS/RHEL）：
```bash
sudo firewall-cmd --zone=public --add-service=ssh --permanent`
sudo firewall-cmd --reload
```
3. **检查服务器 SSH 配置**
- 确保 SSH 配置文件（通常位于 `/etc/ssh/sshd_config`）中包含以下设置，确保 SSH 在默认的端口 22 上监听：
```bash
Port 22
ListenAddress 0.0.0.0
```
- 修改配置后，重新启动 `sshd` 服务:
```bash
sudo systemctl restart ssh
```

- 为了确认你的本地电脑能够访问服务器的 IP 地址，可以通过以下命令测试连通性：
```bash
ping 192.168.xxx.133
```

- 如果 `ping` 不通，可能是服务器或者网络配置的问题，需要检查路由或服务器的网络配置。

5. **检查密钥配置（如有使用）**
- 如果使用 SSH 密钥对登录，请确保公钥已添加到服务器的 `~/.ssh/authorized_keys` 文件中，并检查密钥权限：
- 在服务器端运行：
```bash
chmod 600 ~/.ssh/authorized_keys
```
- 本地的私钥也应设置为只读：
```bash
chmod 600 ~/.ssh/id_rsa
```

### (3) 连接到 Linux 主机
VsCode 命令界面 ctrl shift p 输入 Remote-SSH > Connect to Host ，即可连接到对应的 Linux 主机;
![[attachments/Pasted image 20241024145710.png]]


### (4) 配置文件同步, 将本地代码文件同步到 linux
可以采用  winscp 实现同步功能, 但是更推荐采用 rsync 同步功能进行, 此处采用 WinSCP 进行远程服务器同步文件更新。


要将 Windows 上某个目录的文件自动同步到 Linux 服务器，可以使用几种常见的方法。以下是一些有效的方案，从简单到复杂的不同选项，包括使用 `rsync`、`WinSCP`、`PowerShell` 脚本、和第三方同步工具：

#### 方法 1：使用 `rsync` 和 `SSH`（推荐）
   `rsync` 是一种快速、可靠的文件同步工具。可以结合 SSH 来同步文件：

1. **安装 WSL（Windows Subsystem for Linux）**:
- 在 Windows 10 或 11 上，安装 WSL，确保你有一个 Linux 子系统可以运行 `rsync`。
- 运行以下命令在 Windows 中启用 WSL 和安装一个 Linux 发行版（如 Ubuntu）：
```bash
wsl --install
```
- 或者从微软商店安装 Ubuntu。

2. **配置 `rsync`**: 
- 通过 WSL 访问你的 Windows 文件系统（如 `C:\` 映射到 `/mnt/c/`）。
- 编写一个简单的脚本，将 Windows 文件夹同步到 Linux 服务器，例如：
```bash
rsync -avz /mnt/c/path/to/local/folder/ user@remote_server:/path/to/remote/folder
```
- 将这个脚本保存为 `sync_files.sh`。

3. **设置定时任务（使用 `cron` 或 Windows 计划任务）**:
- 使用 `cron` 在 WSL 中定期运行脚本：
```bash
crontab -e
```
添加一行来每 5 分钟同步一次：
```bash
*/5 * * * * /path/to/sync_files.sh
```
- 你也可以在 Windows 中使用“计划任务”来每隔一段时间执行 WSL 命令：
- 任务操作中，设置运行命令为：
```bash
wsl rsync -avz /mnt/c/path/to/local/folder/ user@remote_server:/path/to/remote/folder
```

#### 方法 2：使用 WinSCP 的同步功能
   **WinSCP** 是一款适用于 Windows 的图形化 SFTP/FTP 客户端，它支持目录自动同步。
   1. **下载并安装 WinSCP**:
      - 从 [WinSCP 官方网站](https://winscp.net/) 下载并安装。
   2. **配置同步目录**:
      - 打开 WinSCP，设置一个连接到 Linux 服务器的 SFTP 连接。
      - 登录后，点击菜单中的“文件” > “保持远程目录最新”。
      - 配置本地和远程的同步路径。
      - 选择“自动模式”，WinSCP 会在检测到本地文件变化时自动上传。
   3. **设置后台运行**:
      - WinSCP 可以配置成后台运行，当文件发生更改时自动同步。适合需要简单图形化界面的用户。

#### **方法 3**：使用 PowerShell 脚本结合 `scp`（适合 Windows 用户）
   使用 Windows 原生的 PowerShell 可以自动化文件同步过程:
   1. **创建 PowerShell 脚本**:
      - 使用 `scp` 通过 SSH 传输文件，脚本示例如下：
        ```powershell
        $localPath = "C:\path\to\local\folder\*"
        $remoteUser = "user"
        $remoteHost = "192.168.xxx.xxx"
        $remotePath = "/path/to/remote/folder"
        $sshKeyPath = "C:\path\to\your\ssh\private\key"

        scp -i $sshKeyPath -r $localPath $remoteUser@$remoteHost:$remotePath
        ```

   2. **设置定时任务**:
      - 打开 Windows 计划任务（Task Scheduler），创建一个新的任务。
      - 设置触发器为定时运行（如每隔 5 分钟）。
      - 在“操作”中设置为运行 `powershell.exe`，并在参数中填写脚本路径。

#### **方法 4**：使用同步工具（如 `Syncthing` 或 `FreeFileSync`）
   如果你需要双向同步或更复杂的同步配置，第三方工具可能更合适:
1. **Syncthing**:
  - 下载并安装 [Syncthing](https://syncthing.net/) 在 Windows 和 Linux 服务器上。
  - 在 Windows 和 Linux 上分别运行 Syncthing，并将要同步的文件夹添加到界面中。
  - 通过 Syncthing 的 Web UI 进行设备配对并同步。

2. **FreeFileSync**:
  - 下载并安装 [FreeFileSync](https://freefilesync.org/)。
  - 配置一个同步任务，将本地目录与远程服务器目录进行同步。
  - FreeFileSync 提供 GUI 和命令行版本，可以创建批处理脚本并结合计划任务使用。

采用 WinSCP 同步两端的文件(打开文件夹即可):
![[attachments/Pasted image 20241024151523.png]]

如图进行远程主机的文件夹连接:
![[attachments/Pasted image 20241024151716.png]]

首先 node_modules 文件夹权限修改:  `sudo chmod -R 755 node_modules/`

安装 node.js 包的版本管理工具 nvm 
```sh
curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.39.4/install.sh | bash
```
修改 nvm 下载镜像源:
```sh
export NVM_NODEJS_ORG_MIRROR=https://registry.npmmirror.com/mirrors/node
```
重新加载 bash 文件:
```sh
source ~/.bashrc
```

安装 node :
```sh
nvm install  node 
node -v
```


开启服务器:
```
nginx 
```
