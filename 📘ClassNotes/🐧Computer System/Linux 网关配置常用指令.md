查看网关:
```sh
ifconfig
```

**自动配置网关地址**:
```sh
sudo dhclient ens33
ping 8.8.8.8   #  ping 成功则正常联网
```

```sh
sudo nmtui
sudo systemctl restart NetworkManager
```

重启网络服务
```sh
sudo systemctl restart networking
sudo systemctl restart NetworkManager
```



ubuntu 的网络下载镜像源设置问题: 
```sh
sudo cp /etc/apt/sources.list /etc/apt/sources.list.bak 
sudo nano /etc/apt/sources.list
```

```sh
sudo nano /etc/resolv.conf
```

```sh
将 `security.ubuntu.com` 替换为 `mirrors.aliyun.com`：
deb http://mirrors.aliyun.com/ubuntu/ bionic main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ bionic-updates main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ bionic-security main restricted universe multiverse
```

## 配置 DHCP 自动获取 IP 地址
如果你使用 `NetworkManager` 将 `ens33` 配置为 DHCP 自动获取 IP 地址，通常只需要通过 `nmcli` 或 `nmtui` 进行配置，无需手动修改其他配置文件。不过，有几件事需要检查和注意，以确保配置正确，并且不会与其他配置文件冲突：

### 1. **使用 `nmcli` 配置时的注意事项**
   - 使用以下命令将 `ens33` 配置为 DHCP 自动获取 IP 地址：
 ```bash
 sudo nmcli connection modify ens33 ipv4.method auto
 sudo nmcli connection up ens33
 ```
   - 这个命令会自动更新 `NetworkManager` 的内部配置文件，不需要手动编辑其他文件。

### 2. **检查 `/etc/network/interfaces` 文件（对于 Debian/Ubuntu 系统）**
   - 如果你的系统中存在 `/etc/network/interfaces` 文件，建议确保 `ens33` 在该文件中没有重复配置。
   - 例如，如果 `/etc/network/interfaces` 文件中存在静态 IP 配置，可能会与 `NetworkManager` 的配置冲突。
   - 如果 `NetworkManager` 已管理 `ens33`，你可以注释掉或删除有关 `ens33` 的配置：
     ```bash
     sudo nano /etc/network/interfaces
     ```
     将任何关于 `ens33` 的配置注释掉：
     ```
     #auto ens33
     #iface ens33 inet static
     #  address 192.168.1.100
     #  netmask 255.255.255.0
     #  gateway 192.168.1.1
     ```

### 3. **确认 `/etc/NetworkManager/NetworkManager.conf` 设置**
   - 确保 `NetworkManager` 正在管理你的网络接口。可以检查或编辑 `NetworkManager` 的配置文件：
     ```bash
     sudo nano /etc/NetworkManager/NetworkManager.conf
     ```
   - 确保 `managed=true` 设置在 `[main]` 部分下，以便 `NetworkManager` 管理网络接口：
     ```
     [main]
     plugins=ifcfg-rh
     managed=true
     ```
   - 如果你对该文件进行了修改，重启 `NetworkManager` 以使更改生效：
     ```bash
     sudo systemctl restart NetworkManager
     ```

### 4. **确保 `NetworkManager` 已启用并正在运行**
   - 确保 `NetworkManager` 服务已启用并正在运行，否则它将不会管理网络接口：
     ```bash
     sudo systemctl enable NetworkManager
     sudo systemctl start NetworkManager
     ```

### 5. **重启后验证**
   - 配置完成后，重启系统或网络服务，并验证 `ens33` 是否成功通过 DHCP 获取 IP 地址：
     ```bash
     ip addr show ens33
     ```
   - 确保 `inet` 后面有一个动态分配的 IP 地址。

### 总结
如果你使用 `NetworkManager` 来配置 `ens33`，只需要确保它的配置不会与 `/etc/network/interfaces` 或其他网络配置文件中的设置冲突即可。一般情况下，`nmcli` 或 `nmtui` 会自动更新配置文件，因此不需要手动编辑多个文件。


## 添加 NetworkManager 的 ens33 管理配置

出现 `Error: unknown connection 'ens33'` 的问题，通常是因为 `NetworkManager` 中没有记录名为 `ens33` 的连接配置。这意味着 `NetworkManager` 可能没有正确管理这个网络接口。以下是解决这个问题的步骤：

### 1. **查看现有的连接配置**
   - 首先查看 `NetworkManager` 中已有的连接列表，以确定是否存在相关的配置：
     ```bash
     nmcli connection show
     ```
   - 如果没有看到 `ens33`，可能需要手动添加一个新的连接配置。

### 2. **添加新的 `ens33` 连接配置**
   - 如果 `ens33` 未出现在连接列表中，可以手动添加一个新的 DHCP 配置：
     ```bash
     sudo nmcli connection add type ethernet ifname ens33 con-name ens33 ipv4.method auto
     ```
   - 这条命令会添加一个名为 `ens33` 的连接，并设置 `ipv4.method` 为 `auto`，即使用 DHCP 自动获取 IP 地址。

### 3. **激活新配置**
   - 添加完新连接后，激活该连接：
     ```bash
     sudo nmcli connection up ens33
     ```
   - 这会让 `ens33` 接口开始使用新的配置进行连接。

### 4. **验证网络状态**
   - 检查 `ens33` 是否成功获取了 IP 地址：
     ```bash
     ip addr show ens33
     ```
   - 确认 `inet` 后有一个动态分配的 IP 地址。

### 5. **确保 `NetworkManager` 正在管理 `ens33`**
   - 如果 `ens33` 是通过其他方式（如 `/etc/network/interfaces`）管理的，`NetworkManager` 可能不会管理它。
   - 可以尝试移除其他配置或在 `/etc/network/interfaces` 中注释掉 `ens33` 的手动配置，让 `NetworkManager` 接管：
     ```bash
     sudo nano /etc/network/interfaces
     ```
     注释掉有关 `ens33` 的部分（如果存在）：
     ```
     #auto ens33
     #iface ens33 inet dhcp
     ```
   - 然后，重启 `NetworkManager` 服务：
     ```bash
     sudo systemctl restart NetworkManager
     ```

### 6. **手动添加 DHCP 客户端（`dhclient`）配置为启动时自动运行**
   - 如果你希望 `dhclient` 在启动时自动运行，可以通过在 `/etc/rc.local` 文件中添加命令来实现（如果你的系统支持该文件）：
     - 编辑或创建 `/etc/rc.local` 文件：
       ```bash
       sudo nano /etc/rc.local
       ```
     - 在 `exit 0` 之前添加：
       ```bash
       dhclient ens33
       ```
     - 确保文件可执行：
       ```bash
       sudo chmod +x /etc/rc.local
       ```
   - 这将确保在系统启动时自动运行 `dhclient`，使 `ens33` 获取 IP 地址。

### 总结
- 首先尝试通过 `NetworkManager` 添加并管理 `ens33` 接口。
- 如果 `NetworkManager` 无法正确管理，考虑在系统启动时自动运行 `dhclient` 来解决 DHCP 问题。
- 确保不同的网络配置工具之间（如 `NetworkManager` 和 `/etc/network/interfaces`）不会冲突。

这样配置后，`ens33` 应该能够在启动时自动获取 IP 地址，而不再需要手动运行 `sudo dhclient ens33`。

由于仍然不行, 因此采用强制管理 ens33 的方式: (下面第二条):
然后由于是使用 NetworkManager ， 所以采用下面代码让每一次启动时, NetWorkManager 为 ens33 自动寻找地址: 
```sh
sudo nmcli connection modify ens33 ipv4.method auto
sudo nmcli connection up ens33
```
- This will set `ens33` to automatically obtain an IP address using DHCP.


### 1. **Check for `NetworkManager` Device Ignore Rules**
   - Sometimes, `NetworkManager` may be instructed to ignore certain interfaces through files in `/etc/NetworkManager/conf.d/` or other configuration locations. Look for any files that might be instructing `NetworkManager` to ignore `ens33`:
     ```bash
     grep -i unmanaged /etc/NetworkManager/conf.d/* /usr/lib/NetworkManager/conf.d/*
     ```
   - If you find any configuration file mentioning `unmanaged-devices`, such as:
     ```
     [keyfile]
     unmanaged-devices=interface-name:ens33
     ```
   - Edit the file to remove or comment out this line:
     ```bash
     sudo nano /etc/NetworkManager/conf.d/your_file.conf
     ```
   - After editing, restart `NetworkManager`:
     ```bash
     sudo systemctl restart NetworkManager
     ```

### 2. **Force `NetworkManager` to Manage `ens33`**
   - If no specific configuration file is found, you can try adding a custom configuration to ensure `ens33` is managed:
     - Create a new configuration file:
       ```bash
       sudo nano /etc/NetworkManager/conf.d/managed-ens33.conf
       ```
     - Add the following content:
       ```
       [keyfile]
       unmanaged-devices=none
       ```
     - Save the file and restart `NetworkManager`:
       ```bash
       sudo systemctl restart NetworkManager
       ```
### 3. **Recreate the Connection Profile Manually**
   - If `ens33` is still unmanaged, you can try deleting any old or corrupted connection profiles for `ens33` and then recreate it:
     ```bash
     sudo nmcli connection delete ens33
     ```
   - Create a new connection:
     ```bash
     sudo nmcli connection add type ethernet ifname ens33 con-name ens33 ipv4.method auto
     ```
   - Then bring up the connection:
     ```bash
     sudo nmcli connection up ens33
     ```

### 4. **Check `/etc/network/interfaces.d/` Directory**
   - In some distributions, interfaces might be configured in files located under `/etc/network/interfaces.d/`. Check if there's a specific file for `ens33`:
     ```bash
     ls /etc/network/interfaces.d/
     ```
   - If you find a configuration file for `ens33`, edit it and ensure that `ens33` is not manually configured there:
     ```bash
     sudo nano /etc/network/interfaces.d/ens33
     ```
   - Comment out any static or manual configuration.

### 5. **Reboot the System**
   - After making the changes, reboot your system to ensure all configurations are properly applied:
     ```bash
     sudo reboot
     ```

### 6. **Verify with `NetworkManager` Logs**
   - If the issue persists, checking the logs can provide more information about why `ens33` remains unmanaged:
     ```bash
     sudo journalctl -u NetworkManager
     ```
   - Look for any messages related to `ens33` or `unmanaged` devices. This may provide clues about what is causing `ens33` to be treated as unmanaged.

### Summary
These steps ensure that there are no settings explicitly telling `NetworkManager` to ignore `ens33`. By creating a new configuration profile and ensuring no conflicting settings exist, `ens33` should become managed and be able to automatically obtain an IP address. If the issue persists, examining the logs should provide further insight into the root cause.