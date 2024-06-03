对于MP3是一种需要进行解码的音频文件, 而wav一般是直接播放不需要解码的文件

对于3.3V的电路, 一般无法带动喇叭, 因此需要加上功放芯片, 常用的有TC8002d, 输入电压 2.0V～5.5V 

使用软件解码算法（例如MP3解码库）将其解码为PCM音频数据。最后，将PCM数据传递给DAC（数字模拟转换器）以播放音频。

可以考虑使用AC1802

### Helix 解码库实现MP3软件解码
也可以选择直接软件解码, 其中往往使用Helix解码库来解码MP3(相对labmad占用RAM更少), 参考[Helix 实现 MP3 解码](https://blog.csdn.net/weixin_42258222/article/details/122640413)


[基于stm32的wav解码方案](https://blog.csdn.net/u012507643/article/details/50432635)
