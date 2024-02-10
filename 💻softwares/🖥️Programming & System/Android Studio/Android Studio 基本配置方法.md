入门教程参考: https://blog.csdn.net/leah126/article/details/131268770
# 一、 添加虚拟设备与仿真
### (1) 虚拟设备添加方法
首先在Device Manager中添加设备, 
![[attachments/Pasted image 20240208104618.png|500]]
选择Physcial > Pair Using WiFi 可以配对。
另外也可以选择添加虚拟设备; 使用Create Device 来添加虚拟设备; (其中R的安卓版本为11.0)
![[attachments/Pasted image 20240208105455.png|800]]
注意: 本机的虚拟仿真使用Pixel 6 API 30的R系统, 界面如图: 
![[attachments/Pasted image 20240208150745.png]]

### (2) 仿真方法
选择右下角的Emulator开始虚拟仿真。
![[attachments/Pasted image 20240208145602.png]]

仿真调试图像如下 : 
![[attachments/Pasted image 20240208151146.png]]

### (3) 设置JDK版本方法 
选用 File > Project Structure > SDK Location > 选择Gradle Settings  

![[attachments/Pasted image 20240208105709.png|700]]

一此处选择为Embedded JDK 即可

# 二、SDK Manager 的配置和使用
### (1) SDK Manager 的配置
Android Studio 自带有SDK Manager 
![[attachments/Pasted image 20240208124433.png]]
首先， 当工程初始创建时， 会Failed To Find Build Tools revision 30.0.3
![[attachments/Pasted image 20240208125129.png]]

首先在SDK Platforms 下载Android 11.0 并接受条款; 
![[attachments/Pasted image 20240208125312.png|1400]]
在添加11.0之后, 再次联网之后即可进行自动下载;  
下载过程如图所示: 
![[attachments/Pasted image 20240208125723.png]]
最终点击Build 可以Build Successful， 如下图: 
![[attachments/Pasted image 20240208125815.png|500]] 


### (2) ADB 环境配置 
由于采用的是Android Studio 自带的Android SDK, 因此只要设置为Android Studio 的SDK路径即可, 这个可以从SDK Manager 查到
![[attachments/Pasted image 20240208133233.png]]
此时首先添加Android_Home环境变量
![[attachments/Pasted image 20240208132902.png]]
然后将下面的三条添加到环境变量: 
![[attachments/Pasted image 20240208133137.png]]

输入`ADB version` 查看是否配置成功
![[attachments/Pasted image 20240208133350.png]]

此时在下的Terminal 中即可使用ADB 进行Wifi 连接。
![[attachments/Pasted image 20240208133544.png]]

### (3) 连接USB 在手机上进行实际调试
首先在电脑端安装华为手机助手, 并打开HiSuite, 然后进行连接 (手机设置搜索HDB 选择连接) 
![[attachments/Pasted image 20240208140531.png]]
手机上打开USB调试， 
![[attachments/Screenshot_20240208_131544_com.android.settings.jpg|300]]
然后将收集使用USB连接到电脑 ， 并选取USB调试操作为开启, 选用文件传输模式。
![[attachments/Pasted image 20240208140957.png|700]]
然后即可在手机端得到调试部分， 调试成功。 
![[attachments/Screenshot_20240208_141154_com.example.myapplicat(1).jpg|400]]