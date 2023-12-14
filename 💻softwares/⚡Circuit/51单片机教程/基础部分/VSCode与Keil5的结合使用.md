首先在keil中配置工程生成hex文件。
使用EIDE导入uvproject工程
在左侧使用EIDE进行程序的烧录, 并将C51/INC以及C51/INC/Atmel 添加到包含目录中，才能进行正常编译
![[attachments/Pasted image 20231208160456.png]]

编译无错误之后，即可正常编译并使用STC-ISP烧录。
![[attachments/Pasted image 20231208160824.png|600]]

需要注意的是, VsCode 构建成功之后，需要改用Keil再次构建, 才能保证生成的.hex文件正确, 然后烧录即可。
