apt 不正常可能导致工作站时不时寄的问题. 
sudo nano /etc/apt/sources.list

适用于 arm 架构的 source.list 配置如下:
```sh
# Ubuntu Mirror 
deb [arch=arm64] http://ports.ubuntu.com/ jammy main restricted universe multiverse
# deb-src http://ports.ubuntu.com/ jammy main restricted universe multiverse

deb [arch=arm64] http://ports.ubuntu.com/ jammy-security main restricted universe multiverse
# deb-src http://ports.ubuntu.com/ jammy-security main restricted universe multiverse

deb [arch=arm64] http://ports.ubuntu.com/ jammy-updates main restricted universe multiverse
# deb-src http://ports.ubuntu.com/ jammy-updates main restricted universe multiverse

deb [arch=arm64] http://ports.ubuntu.com/ jammy-backports main restricted universe multiverse
# deb-src http://ports.ubuntu.com/ jammy-backports main restricted universe multiverse
```


- `sudo apt upgrade`：升级所有可升级的包，但不会删除包或安装新依赖。
- `sudo apt full-upgrade`：升级包、删除过时的包，并安装新依赖，进行全面升级。

```sh
sudo apt remove firefox
sudo apt autoremove
sudo apt full-upgrade
```

mysqld will log errors to /var/log/mysql/error.log
mysqld is running as pid 13239
