白噪声具有平衡的功率谱密度(实际上是一条直线), 且能量为无穷大的白噪声激励;
高斯白噪声也可以通过randn来实现, 其功率谱密度固定为 N_0/2 * dB, 也可以通过

限带白噪声是通过过滤高频噪声产生的, 即通过一个高频和低频滤波,

```erlang title:过滤选择器设置代码
designfilt
% 也可以使用如下的代码:
filterDesigner
```
![[attachments/Pasted image 20240308113952.png]]
注意: FIR 是 Finite Impulse Response, 即  finite number of coefficients and a finite length impulse response, 而对应的IIR 是Infinite 意思

对于带通滤波器， 有两个参数: 分别为通频带和阻频带; 
```erlang
Fs = 1000; % Sampling frequency
fpass = [200, 300]; % Passband frequencies
fstop = [150, 350]; % Stopband frequencies
dev = [0.01, 0.1]; % Allowable deviations in passband and stopband
```
实际上会类似定义一个如图类似的通频带部分(其中大小顺序为fstop1, fpass1,fpass2, fstop2); 
且往往是定义在峰值3dB以下的部分;
![[attachments/Pasted image 20240308123338.png]]
可以通过 Kaiser window FIR filter design estimation parameters 定义相应的凯瑟窗;
```MATLAB
[n,Wn,beta,ftype] = kaiserord(fstop, fpass, dev, Fs);
```

另外两个重要参数是 Passband Ripple 和 Stopband Attenuation 
其中Passband Ripple 是用于控制范围内幅值的变化程度 `->` 控制通频内的最大起伏程度; 
往往是设置一个从0到通频带增益峰值的范围值; 

Stopband Attenuation 是控制阻频带的衰减部分,  往往设定为频率的函数, 滤波器的阻带衰减（Stopband Attenuation）决定了滤波器对于高于截止频率信号的抑制能力。若阻带衰减设置为60dB，这意味着高于1024Hz的信号将被减小至少60dB。

# 声压的模拟
RMS 值的计算:
$$SPL  =  20 \cdot \log_{10}\left(\frac{p}{p_{ref}}\right)$$
其中:
- ( SPL ) 是声压级，以分贝（dB）表示。
- ( p ) 是测量的声压（RMS）。
- ( $p_{ref}$ ) 是参考声压，通常取为 ( 20$\mu Pa$ ), 这是国际公认参考值。
因此我们可以得到: 
$$P = p_{ref} * 10^{\frac{SPL}{20}}$$
为声压的计算方法;

```erlang title:1024Hz白噪声随机声压的生成代码
clear, clc;
time_tot = 2;
Freq  = 2500;  % 0 - 1024Hz limited-band white noise
power = 112;   % 112 dB
fpass = 1024;  % filter pass
fstop = 1024 +30;     % filter stop
apass = 1;             % Passband Ripple (dB)
astop = 80;      % Stopband Attenuation (dB) -> 阻带内部,完全衰减掉, 不保留任何成分

%% ************** 用于生成某个频率范围内的限带白噪声数据 **************************
delta_t = 1/Freq;    % sampling period
t = 0 : delta_t : time_tot - delta_t;
n = Freq * time_tot;    % sampling number

pref = 20e-6; % 参考声压为 20微帕斯卡
p_rms = pref * 10^(power/20); % 计算声压级为 112dB 的 RMS 值

% 调整噪声的RMS值: % noise = wgn(1, n,power,'dBW', 'real');
noise = randn(n,1);                 % 生成标准正态分布的白噪声
noise = noise * p_rms/std(noise);   % 调整噪声的RMS值 -> 保证新的方差为rms值

% 检查结果的声压级
rms_level = 20*log10(std(noise)/pref);
disp(['声压级设置 RMS: ', num2str(rms_level), ' dB']);

filter_ = designfilt('lowpassfir', 'PassbandFrequency',fpass, ...
    'StopbandFrequency',fstop,'PassbandRipple',1, ...
    'StopbandAttenuation',astop,'SampleRate',Freq, ...
    'DesignMethod','kaiserwin');
noise_filtered = filtfilt(filter_ , noise); % 对噪声进行过滤, 通过低通滤掉

%% *************************** 原始噪声图像绘制 ******************************** 
figure("Name", "origin Noise");
subplot(3,1,1);
plot(t, noise);  % 绘制经过滤波的白噪声图像
subplot(3,1,2);
Y = fft(noise, n);
P = abs(Y/n);                % 对应点的功率P 
P1 = P(1:(n/2)+1); P1(2:end-1) = 2*P1(2:end-1);
f_vec = Freq * (0 :(n/2))/n;  % 获取对应的频率向量
plot(f_vec, P1);
subplot(3,1,3);
[pxx,w] = pwelch(noise,[], [], Freq);
plot(10*log10(pxx));
xlabel('rad/sample')
ylabel('dB / (rad/sample)')

clear y P P1 f_vec pxx w noise
%% 滤波噪声图像绘制
figure("Name", "filtered Noise");
subplot(3,1,1);
plot(t, noise_filtered);  % 绘制经过滤波的白噪声图像
subplot(3,1,2);
Y = fft(noise_filtered, n);  % If n is not specified,is the same size as X.
P = abs(Y/n);                % 对应点的功率P 
P1 = P(1:(n/2)+1); P1(2:end-1) = 2*P1(2:end-1);  % 对应的功率 x2; 
f_vec = Freq * (0 :(n/2))/n;  % 获取对应的频率向量
plot(f_vec, P1);
title('Single-Sided Amplitude Spectrum of X(t)')
xlabel('f (Hz)')
ylabel('|P1(f)|')
% 计算功率谱密度(使用矩形窗);
subplot(3,1,3);
[pxx,w] = pwelch(noise_filtered,[], [], Freq);
plot(10*log10(pxx));
xlabel('rad/sample')
ylabel('dB / (rad/sample)')
% pxx: power spectral density,  w: normalized frequency vector
```

注意: 经检验, 上述程序能够生成正确的白噪声， 而可能在功率谱密度PSD绘制中出现一定问题

噪声图像如图所示: 其中在1250以后的基本被滤波掉了;
![[attachments/Pasted image 20240308180248.png]]
