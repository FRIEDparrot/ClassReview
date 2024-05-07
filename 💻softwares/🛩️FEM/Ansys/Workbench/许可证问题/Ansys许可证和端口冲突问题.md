可以上淘宝搜索Ansys重装
找文件夹下的AnsysLM.zip    

其他版本可以找 http://119.29.215.66:5244/d/%E5%A4%A9%E7%BF%BC%E4%BA%91%E7%9B%98/LM/ANSYS/24R1/AnsysLM.zip

然后将Services中下列改为Disabled 
![[attachments/Pasted image 20240426112810.png]]

安装许可证之后, cmd执行下面的部分确保已安装
```shell
netstat -ano|findstr 1055
```

正常结果如下:
```shell
TCP    0.0.0.0:1055           0.0.0.0:0              LISTENING       17892
TCP    127.0.0.1:1055         127.0.0.1:4871         ESTABLISHED     17892
TCP    127.0.0.1:4871         127.0.0.1:1055         ESTABLISHED     4084
TCP    [::]:1055              [::]:0                 LISTENING       17892
```

如果出现7272端口, 则是其他服务占用的, 例如sql服务, 
![[attachments/Pasted image 20240426112640.png]]
需要在 Task > Manager > Details 中进行关闭;

