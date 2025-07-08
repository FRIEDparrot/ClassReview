是的，有些 Linux 主机会根据电源管理策略在一段时间内无操作时进入休眠或待机状态。你可以通过以下方法强制让 Linux 不休眠：

---

### **1. 使用 `systemd` 调整电源管理配置**
如果你的系统使用 `systemd`，可以通过调整其电源管理设置防止休眠：

#### **查看当前电源管理配置**
运行以下命令查看休眠相关配置：
```bash
systemctl status sleep.target
```

#### **禁用自动休眠**
可以用以下命令禁用自动休眠：
```bash
sudo systemctl mask sleep.target suspend.target hibernate.target hybrid-sleep.target
```

#### **恢复默认设置**
如果需要恢复默认休眠行为，可以取消屏蔽：
```bash
sudo systemctl unmask sleep.target suspend.target hibernate.target hybrid-sleep.target
```

---

### **2. 修改 `/etc/systemd/logind.conf` 文件**
编辑 `logind.conf` 文件，修改或添加以下配置：
```bash
HandleSuspendKey=ignore
HandleLidSwitch=ignore
IdleAction=ignore
```

保存后，重启 `systemd-logind` 服务以应用更改：
```bash
sudo systemctl restart systemd-logind
```

---

### **3. 使用 `Caffeine` 工具**
如果需要临时保持系统不休眠，可以使用 `Caffeine` 等工具：

#### **安装 Caffeine**
在基于 Debian 的系统中（如 Ubuntu），可以通过以下命令安装：
```bash
sudo apt install caffeine
```

#### **运行 Caffeine**
启动后，Caffeine 会阻止系统进入休眠。可以在命令行运行：
```bash
caffeine &
```

---

### **4. 防止屏幕熄灭**
如果只是希望屏幕保持亮着，可以通过 `xset` 工具调整屏幕节能设置：
```bash
xset s off         # 禁用屏幕保护程序
xset -dpms         # 禁用显示电源管理信号
```

如果需要保持永久生效，可以将这些命令添加到启动脚本中，例如 `~/.xprofile` 或 `/etc/rc.local`。

---

### **5. 用 `watch` 或后台进程模拟操作**
运行一个轻量级后台任务以模拟系统操作，从而避免休眠。例如：
```bash
while true; do xdotool mousemove 1 1 mousemove 0 0; sleep 300; done
```
这段脚本会每 5 分钟模拟一次鼠标移动，避免系统识别为“无操作”。

---

### 总结
上述方法可以按需调整，若仅临时不休眠，可以使用 `Caffeine` 或后台模拟操作。如果需要永久设置，可以通过 `systemd` 或电源管理配置文件来实现。





通过 `systemd` 可以永久设置 Linux 系统不进入休眠或待机状态，具体步骤如下：

---

### **1. 禁用相关的休眠服务**

使用 `systemctl mask` 禁用与休眠相关的服务：

```bash
sudo systemctl mask sleep.target suspend.target hibernate.target hybrid-sleep.target
```

#### **命令说明**
- `sleep.target`：与系统进入睡眠模式相关的目标。
- `suspend.target`：与系统挂起（suspend）相关的目标。
- `hibernate.target`：与系统休眠（hibernate）相关的目标。
- `hybrid-sleep.target`：与混合休眠（hybrid-sleep）相关的目标。

**屏蔽这些目标后，系统将无法进入挂起或休眠状态。**

---

### **2. 修改 `logind.conf` 文件**

编辑 `logind.conf` 配置文件以调整电源管理行为：

```bash
sudo nano /etc/systemd/logind.conf
```

添加或修改以下选项：
```plaintext
HandleLidSwitch=ignore       # 关闭笔记本盖子时不进入休眠
HandleLidSwitchDocked=ignore # 笔记本接入扩展坞时不休眠
IdleAction=ignore            # 空闲时不执行任何动作
IdleActionSec=0              # 设置空闲时间为无限
```

保存后，重启 `systemd-logind` 服务以应用更改：

```bash
sudo systemctl restart systemd-logind
```



### **3. 禁用自动挂起或休眠（适用于 GNOME 桌面环境）**

在使用 GNOME 桌面环境的系统上，可能需要修改 `gsettings` 配置：

#### **查看当前设置**
运行以下命令查看系统是否会自动挂起：
```bash
gsettings get org.gnome.settings-daemon.plugins.power sleep-inactive-ac-type
gsettings get org.gnome.settings-daemon.plugins.power sleep-inactive-battery-type
```

#### **修改设置**
将挂起行为设置为“无”（`'nothing'`）：
```bash
gsettings set org.gnome.settings-daemon.plugins.power sleep-inactive-ac-type 'nothing'
gsettings set org.gnome.settings-daemon.plugins.power sleep-inactive-battery-type 'nothing'
```

---

### **4. 防止显示屏关闭**

如果需要防止显示器进入节能模式，可以调整 `xset` 配置：

#### **禁用屏幕保护和节能**
运行以下命令：
```bash
xset s off         # 禁用屏幕保护
xset -dpms         # 禁用显示电源管理信号
```

#### **永久生效**
将这些命令添加到启动脚本（例如 `~/.xprofile`）：
```bash
echo "xset s off" >> ~/.xprofile
echo "xset -dpms" >> ~/.xprofile
```

---

### **5. 验证设置**

重启系统后，运行以下命令验证休眠相关的目标状态：
```bash
systemctl status sleep.target suspend.target hibernate.target hybrid-sleep.target
```

如果显示 `masked`，说明相关目标已禁用，系统不会进入休眠或待机状态。

---

### 总结

- 禁用 `systemd` 休眠目标 (`mask`) 是最根本的方法。
- 修改 `logind.conf` 可定制休眠和空闲行为。
- 对于桌面环境（如 GNOME），需要同步调整电源管理设置。
- 配合 `xset` 禁用屏幕节能，防止显示器关闭。

这样设置完成后，系统将永久不进入休眠或待机状态。

```sh
systemctl list-timers
```