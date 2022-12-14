## 计算流体力学建模一
### 一、理论知识
#### (1) 主要内容: 
1. 流体方程与边界条件
2. 蠕动流，层流，非牛顿流体建模
3. 湍流建模
4. 旋转机械内的流体流动
5. 薄膜流和管道流
6. 多孔介质和地下流
7. 多相流仿真
8. 流体中的粒子追踪
9. 非等温流建模
10. 高马赫数流动
11. 多物理场模型：反应流， 流固耦合模型
#### (2) 流体计算的基本原理
通用原理：连续介质在剪切应力下发生的变形
流体力学研究流体及力-应变相应
一般原理：质量，动量和能量守恒
流体的流动分类主要是多孔流动，自由流动

主要的简单流动包括
1. 压力流(Poiseuille)
2. 拖曳流(Couette)
3. stokes边界层
4. Taylor-Green 涡流

流体力学包括
1. 流体静力学: 压力变化, 流动状态相对较弱
2. 流体动力学: 对于轮胎的充气过程等等， 运动不可忽略，同时计算速度场和压力场
3. 流体运动学: 流体质点的运动， 遵从连续性介质假设，研究速度，位移和加速度的问题

一般而言研究的都是流体运动学(流体运动受力作用)
对于数值的求解方法，可以在空间和时间上进行离散
1. 有限差分法(FDM)
2. 有限体积法(FVM)
3. 有限元法(FEM)
#### (3) 流体力学的平衡方程
###### 1. 质量守恒方程(连续性方程)
$$\frac{\partial \rho}{\partial t} + \frac{\partial(\rho u)}{\partial x} + \frac{\partial (\rho v) }{\partial y} + \frac{\partial (\rho w)}{\partial z} = 0$$
也即：
$$\frac{Du}{Dt} =  \frac{1}{\rho} \nabla \cdot\sigma + g$$
###### 2. 基于牛顿第二定律的动量守恒平衡方程
动量守恒方程(Navier-Stokes方程)
$$\rho \frac{\partial u}{\partial t} + u \frac{\partial u}{\partial t} + v\frac{\partial  u}{\partial y} = - (\frac{\partial \sigma_x}{\partial x} + \frac{\partial \tau_{xy}}{\partial y}) + X $$
也即：
$$\rho \frac{Du}{Dt} = \rho\left(\frac{\partial u}{\partial t} + \left(u \cdot \nabla\right) u \right) = -\nabla p  + \nabla \cdot \left\{ \mu \left[\nabla u + (\nabla u)^T - \frac{2}{3}(\nabla \cdot u) I \right] + \zeta (\nabla \cdot u)I \right\} +  \rho g$$
###### 3. 能量守恒方程
![[Pasted image 20230107190225.png|350]]

#### 4. 方程综述
共5个方程(3个动量方程), 有11个变量
![[Screenshot_20230107_190714_tv.danmaku.bilibilihd.jpg]]

#### 5. 本构关系
例如牛顿流体：
$$\tau = 2\mu S - \frac{2}{3}\eta (\nabla\cdot u) I$$
通过添加额外的本构方程， 去掉$\tau$部分的力

#### 6. 需要考虑的特征参数
![[Screenshot_20230107_202037_tv.danmaku.bilibilihd 1.jpg]]
主要区分不同类型的流场的方式: 
1. 通过介质区分准数
2. 通过相关准数进行区分

考察单向流的流场
1. 雷诺数: 
$$Re_L \equiv \frac{\rho U L }{\mu} = \frac{\rho L}{\nu} = \frac{\text{inertia force}}{\text{viscous force}}$$
$$\begin{matrix}
Re << 1 \qquad \text{creeping flow} \\
Re = 1 \to 2000 \qquad \text{laminar flow} \\
Re > 4000 \qquad \text{turblent flow}
\end{matrix} \qquad $$
注意一定要确定好相应的雷诺数 ，可能需要进行手动计算
![[Pasted image 20230108111142.png|450]]

2. 马赫数
$$Ma \equiv \frac{|u|}{a} = \frac{\text{flowing speed}}{\text{acoustic velocity}}$$
$$\begin{matrix}
Ma = 0 \qquad \text{Incompressible fluid}\\
0 < Ma < 0.3 \qquad \text{Weakly compressible fluid}\\
0.1 < Ma < 1 \qquad \text{Moderately compressible fluid} \\
Ma \geq 1 \qquad \text{Compressible fluid} 
\end{matrix}$$
单相流Ma < 0.3 
the equation shape of the different equations are different
附注： 注意可压缩流体， 不可压缩流体和可压缩流动和不可压缩流动的区别
$$\begin{matrix}
c_{air} = 343 m/s \\
c_{water} = 1484 m/s 
\end{matrix}$$
3. Knudsen number
$$K_n \equiv \frac{\lambda}{L} = \frac{\text{average free path}}{\text{physical length dimension}}$$
$$
\begin{matrix}
K_n < 0.01  \qquad \text{continous flow} \\
0.1 < K_n <10 \qquad \text{transition flow} \\
K_n > 10 \qquad \text{free molecular flow}
\end{matrix}
$$
一般情况下，在过渡流和分子流无法界定时，使用过渡流解决问题

#### 7. 边界条件
包括流动种类以及以下的边界条件
1. 流动压力
参考压力：一般的压力为相对于参考压力的部分， NS方程只计算压力梯度，不计算绝对值。
有的流场需要使用==绝对压力==的材料属性进行计算，需要和能量方程进行关联， 包含管道流和高马赫数流
使用相对压力进行求解时，由于使用了参考压力，接近于系统的压力，参考压力主要决定了流场求解的稳定性和收敛性

2. 流体属性
3. 初始值
对于稳态问题，边界条件不需要与初始值一致
对于瞬态的初始值，初始值是物理上的初始状态，对之后得到的解有较大的影响。
![[Pasted image 20230108144112.png|400]]

在边界条件选项中，还存在充分发展的流动等设置
壁边界条件的滑移速度和滑移效应。内部壁可以避免对薄的结构划分网格，可以设定为滑移壁，无滑移壁等等

出口位置的设置: 如果出口附近存在特定的流动结构如回流部分(难以计算质量守恒)，需要将出口延长至完全包含该流动结构然后进行求解
![[Pasted image 20230108144447.png]]
对于一个开始下降很快但后期不稳定的部分，可能是没有稳态解的问题，需要使用瞬态求解器解出一个瞬态解
流量控制装置也可以作为边界条件

#### 8. 轴对称和周期性
虽然可以极大减少计算时间，但是必须保证严格的对称
#### 9. 压力点约束
对于无法决定流场入口的速度， 且无法对压力做限制时， 作为压力点约束的位置
一般来说，选取重力场中的上面的点作为相应的压力约束点

考虑重力时，出口的压力不可以恒定
![[Pasted image 20230108145606.png|300]]
#### 10. 稳定性
在模型开发器中设置勾选稳定性的部分， 然后在层流中找到自洽稳定性和非自洽稳定性并进行勾选

对于非自洽稳定性
由于方程稳定，收敛简单地多，但是不符合质量守恒和动量守恒的自洽性
增加$\delta$ 取值虽然会增加稳定性但是会增大误差

对于自洽稳定性(默认勾选)
符合质量和动量守恒的自洽
可以使用侧风扩散后可以消除梯度位置不稳定，
保证梯度部分的精度,得到更加精确的结果

稳定性技术的影响主要有网格和容差

#### 11. 适用性(?????)
1. 流线扩散
对流作用占主导的部分， 必须使用流线扩散
2. 侧风扩散
对于不可压缩流动且不包含激波的部分， 可以用于在尖端的边界以及剪切层引入额外的扩散
### 二、 相关案例讲解
#### 1. 非牛顿流体的演示
研究与剪切速率相关的粘度系数对线性聚苯乙烯溶液流动的影响
使用非牛顿流的Carreau模型， 通过轴对称将三维模型转换为二维轴对称模型
参数化扫描研究进口压力的影响

其中有$\mu_0$设置为$166(Pa\cdot s)$,属于比较粘稠的粘性

注意：如果参考压力为1atm，而出口压力为0，则出口绝对压力为1个大气压

对于动力粘度$\mu$，可以使用
如果非牛顿流和预置的参数都不同，则可能需要用测量的参数
通过插值函数实现剪切线速率和粘性的关系的实验测定 
通过int1(spf.sr)来实现相应的计算

往往在流体求解时，需要使用细化或者超细化网格


#### 2. 其它案例
(1) 后台阶流(incompressible-flow-over-a-backstep)
(2) 星型的微流道芯片(star-shaped-microchannel)
(3) 薄板混合器(lamella-mixer)
(4) 层流静态混合器(laminar-static-mixer)
(5) PID控制反应器(process-control-using-a-pid-controller)
## 计算流体力学建模二
### 一、理论知识
#### (1) 湍流建模和RANS方法
COMSOL中的所有的湍流模型，都是基于RANS方程的

流动可以分为层流和湍流
层流到湍流会经过一个无序的漩涡的转裂
通过层流边界层的过程，进行相应的转裂

从低雷诺数的层流过程到高雷诺数的过程为过渡

往往湍流模型使用一阶单元来进行描述，可能出现数值不稳定的现象

在湍流部分，往往是湍流的混沌现象， 流动的细节强烈依赖于初始条件，强混合，湍动能转换为热能(小尺度)
具有高度的耗散性（动量和热的相互转换）

湍流往往采用RANS平均模型来进行，基于空间和时间的平均来求解相应的流场 
RANS(Navier-Stokes)模型可以预测完全湍流区的流动在时间上的平均的解
RANS可以在台式机上进行求解

#### (2) 层流到湍流的转裂判定
临界雷诺数：
当雷诺数近似大于相应的临界雷诺数时， 转换为湍流状态模型

临界雷诺数依赖于不同的流动问题, 与平板边界和障碍物等等有关

#### (3) DNS方法(不用RANS)的计算成本
DNS(直接数值模拟)
计算成本正比于$Re^3$

$LES$大涡模拟
大涡模拟将湍流分为大小尺度分别使用N-S和湍流模型进行求解
精确但是计算成本高
三个接口都是基于变分的多尺度方法(VMS)的
由于RANS方法会导致涡项被过滤掉，使用大涡模拟来进行一些湍流的模拟过程

#### (4) 常见的几种湍流RANS模型
根据引入的偏微分方程数目不同可以分为
(1) 零方程模型
yplus模型
(2) 一方程模型
Spalart - Allmaras粘度模型
(3) 两方程模型
$k-\varepsilon$模型
$k-\omega$模型
(4) 四方程模型
$v^2 -f$模型
![[Pasted image 20230108195155.png|450]]

对于不同的RANS湍流模型的计算成本，从零到四方程计算成本逐渐升高
计算精度也是逐渐增加的，论文中往往使用$k-\varepsilon$模型来进行求解

##### 1. Spalart -Allmaras 模型
Spalart - Allmaras 湍流模型适合于高雷诺数，大自由度的问题
主要用于空气动力学求解问题
具有稳定和容易求解，内存需求小， 不使用壁函数等等优点

对于升力，阻力和传热等等计算比较精确
缺点是不能计算： 
1. 剪切流
2. 分离流
3. 衰变湍流

##### 2. K-epsilon模型
主要用于工业模型计算
稳定，容易求解
可以在壁面附近使用粗糙的网格
往往对于外流问题以及复杂几何，有良好的表现

缺点：
1. 要求流场的雷诺数必须足够大
2. 不能精确计算存在逆压梯度（出口局部的高压问题）， 强曲率流动， 射流等现象存在的流场
3. 在边界附近求解的结果不一定精确
比如计算相应的升力等等
比如对于拉法尔喷管的问题，会形成漩涡（分离涡）,和边界层分离有关不适用
![[Pasted image 20230108202452.png|450]]
后台阶流也是突然扩张，也是会导致逆压梯度的产生

##### 3. $k-\varepsilon$模型的修正
$$\mu_T = \rho C_\mu \frac{k^2}{\varepsilon}$$
其中$C_\mu$应当修正为何应变率变化有关

在标准的$K-\varepsilon$模型中，需要满足一些可真实化约束
![[Pasted image 20230108203207.png|350]]

##### 4. $k-\omega$ 模型的注意事项
注意 $k-\varepsilon$和$k-\omega$的混合长度限制
- 直接通过湍动能和耗散率计算湍流粘性时，可能出现除0错误
- 混合长度可以用来计算湍流粘度

$k-\omega$模型是对$k-\varepsilon$模型的改进
优点：
1. 与$k - \varepsilon$相同的内存需求
2. 对于内部的流动问题表现良好
3. 存在下列现象时，可以更加精确地计算流场：
		(1) 具有强曲率的流动
		(2) 分离流
		(3) 逆压梯度
		(4) 喷射

缺点： 
(1)  通过壁函数进行近似，所以壁附近的解可能不够精确
(2) 对于外流问题，对于自由来流（进口）的$\omega$值敏感

常常使用$k-\varepsilon$模型提供一个初始的条件


##### 5. 低雷诺数的$k-\varepsilon$模型
优点： 
可以用于随后获取更加精确的解
不使用壁函数，计算力和壁面通量的结果更加精确
可以推广到边界层的底层

缺点：
需要更加精细的边界层网格， 以及良好的入口或者初始值条件来提高收敛性
需要的内存高于$k-\varepsilon$以及$k -\omega$模型

##### 6. SST 模型
剪切应力传递模型
结合了$k-\varepsilon$和$k-\omega$模型
对边界条件不敏感   --->  但是对初始条件敏感
满足可真实化约束   
不使用壁函数， 可以求解升阻力问题和传热等问题 --> 但是内存需求高于$k-\varepsilon$模型

##### 7.$v^2- f$模型
通过求解流线法向的湍流速度脉动来考虑湍流的各向异性

可以求解旋钮等中的各向异性的湍流模型
求解的是和$v^2$和$f$有关的两个变量的输运方程

得到法向和切向的各项异性的问题适合使用不同的模型
![[Pasted image 20230108215539.png|500]]
www.comsol.com/blogs/which-turbulence-model-should-choose-cfd-application/



#### (5) 壁函数 | 入口条件
在粘性条件下，对数律时，满足非线性关系
假设计算域从$\delta_w$开始
注意需要合理进行边界层的网格划分
对于不需要壁函数的网格: 需要精细的网格来解析相应的边界区域流动
入口条件:    湍流长度，强度，等等

### 二、实例讲解
#### (1) 湍流实例
使用湍流 -> 生成新的湍流模型接口
靠近壁面， 湍流可以分为三个区域， 边界层的梯度大，尺寸小，二
粘性层

注意：湍流的宏观效应增加质量动量和能量的扩散通量
这对其他物理场存在影响，因此在传递方程中，必须校正方程的传递系数(热传导和扩散系数)

注意：需要使用几何-> 右键直接插入几何序列即可插入mph文件

一般情况下， 湍流条件选用指定湍流的长度和强度
一般的湍流强度约设置为0.05，  而对于湍流长度， 可以将上方的边界条件改为充分发展的湍流

对于边界层网格的设置，有可以设置边界层数，边界层拉伸因子和厚度调节因子（壁面方向的网格尺寸的大小/20是调节因子为1）

通常可以手动计算边界层的厚度
在其中的无量纲量
一般50~100内影响不大

#### (2) 旋转机械流动-二维混合器内的流体混合问题
预测旋转机械中的压力损耗和流动，(Ma<0.3)
内容：
旋转机械中的层流和湍流
基于RANS的湍流模型
搅拌器模块零件库
非等温流，反应流和多相流的多物理场接口

需要通过实际的文献判断是层流还是湍流
使用滑动网格（移动网格特例）方法

研究类型包括
1. 冻结转子
2. 瞬态

可以研究相应的自由液面

##### 注意事项： 
装配体一致对条件，
自动识别旋转壁
内部薄壳壁条件， 描述浸入结构，等等。 
搅拌器模块的零件库

对于非等温流，可以将热流问题进行耦合，从而有热流的耦合效果
旋转机械，多相流

注意在搅拌器中必须选择创建装配体，并选择创建对（在搅拌边界处创建一致对）

注意在定义Uin的斜坡函数时，需要使用
u_in * rm1(t[1/s])
来取消t的量纲 其中[1/s]表示乘积

对于多物理场耦合的求解器设置的参考：学习高效地求解多物理场问题

注意在之后必须在Laminar flow中设置Pairs, 并且使用flow Continuity来保证相应的流体的连续性

可以在Component模块或者Physics模块下添加Particle Tracing for Fluid flow
来进行粒子追踪


## 计算流体力学建模三
### 一、理论知识
主要内容：
- 薄膜流，管道流
- 多孔介质地下水流
- 两相流的仿真
#### (1) 薄膜流和雷诺方程
对于薄膜流，是在狭窄区域的润滑和流动，可以用于对三维的壳建模，并可以考虑气相的空化(???)作用

使用间隙内的雷诺方程来进行求解，忽略惯性力项， 使粘性力起主导作用
适用于间隙内的切向方向较大的流动
雷诺方程包括连续性方程，动量方程

薄膜流动 -> 修正的雷诺方程: 
使用理想气体状态方程修正气体的密度，并保持流场的温度不变

##### **流体属性的设置**
1. 参考平面的属性 -> 一般在油膜的中间假设一个平面为参考平面
需要定义上部的高度和下面的高度，
壁速度的变化也会导致影响相应的部分，注意有壁速度矢量时， 要注意法向的设置，这会影响解得的结果
![[Pasted image 20230109143336.png|500]]
2. 将流动区域分为空化区域和无空化区域
不考虑空化之后气体的可压缩性，
无空化区域的介质为液体，而空化区域压力恒定，且介质为部分或全部气体
可以在可压缩应中对可压缩性进行一些微小的调整(需要打开高级物理场选项)
![[Pasted image 20230109143641.png]]

#### (2) 管道流
将管道流简化为一维问题，通过摩擦因子来进行考虑，雷诺数，壁粗糙度和管道形状
主要的设置参数有:
1. 牛顿流体和非牛顿流体
2. 两相流
3. 管道属性(形状，摩擦模型， 表面粗糙度)

对于进出口边界，可以定义速度，也可以定义压力
可以计算在管道中的化学物质传递(如果需要更多的部分，需要购买化学物质等模块)

注意：在管道流和高马赫数流中压力取绝对压力

管道传热的耦合 -> 需要用到壁传热的边界条件，可以设置壁传热，壁厚，导热等等
可能会加上内部的膜阻和外部膜阻 ----> 外部膜阻时不能使用comsol自带的模型
注意管道的CFD模块的使用
可以通过水锤校准模型来模拟瞬间关闭阀门后换刀的内压随时间的变化

#### (3) 多孔介质和地下水流

### 二、实例讲解
