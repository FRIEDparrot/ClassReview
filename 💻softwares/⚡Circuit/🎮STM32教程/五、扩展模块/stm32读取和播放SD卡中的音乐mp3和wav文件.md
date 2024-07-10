对于MP3是一种需要进行解码的音频文件, 而wav一般是直接播放不需要解码的文件

对于3.3V的电路, 一般无法带动喇叭, 因此需要加上功放芯片, 常用的有TC8002d, 输入电压 2.0V～5.5V 

使用软件解码算法（例如MP3解码库）将其解码为PCM音频数据。最后，将PCM数据传递给DAC（数字模拟转换器）以播放音频。

可以考虑使用AC1802

### Helix 解码库实现MP3软件解码
也可以选择直接软件解码, 其中往往使用Helix解码库来解码MP3(相对labmad占用RAM更少), 参考[Helix 实现 MP3 解码](https://blog.csdn.net/weixin_42258222/article/details/122640413)

[基于stm32的wav解码方案](https://blog.csdn.net/u012507643/article/details/50432635)

Helix 库下载的地址为: https://github.com/derkling/adplayer/tree/master/HELIX  
对于stm32，仅有PA4和PA5可以用于输出模拟波形;

对于Helix 库的使用，首先需要定义环境: 
```cpp
#if defined(_WIN32) && !defined(_WIN32_WCE)
#
#elif defined(_WIN32) && defined(_WIN32_WCE) && defined(ARM)
#
#elif defined(_WIN32) && defined(WINCE_EMULATOR)
#
#elif defined(ARM_ADS)
#
#elif defined(_SYMBIAN) && defined(__WINS__)	/* Symbian emulator for Ix86 */
#
#elif defined(__GNUC__) && defined(ARM)
#
#elif defined(__GNUC__) && defined(__i386__)
#
#elif defined(_OPENWAVE_SIMULATOR) || defined(_OPENWAVE_ARMULATOR)
#
#elif defined(__GNUC__) && defined(__AVR32_UC__)
#
#else
#error No platform defined. See valid options in mp3dec.h
#endif
```

第一个 \_\_GNUC\_\_ 是编译环境, 第二个 ARM 是目标架构。 
我们这里采用 ARM_ADS 作为架构





WAV 音频的示例代码: 
链接： https://pan.baidu.com/s/1Z1Qhxhivc6DZzF8h5L5AwQ   
提取码：la0x  


最后的问题: 当 PA4 作为SPI 使用时, 则上面的DAC会收到干扰, 不能进行正确的初始化 !!!! 

在WAV中, 文件的存储是以小端字节为起始进行存储的, 例如44AC, 实际上是AC44, 即表示44100为采样率

