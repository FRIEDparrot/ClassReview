# 一、写在前面
 最近终于是有些时间可以更了，以后的话也会持续更新一些力学和程序相结合的内容。
 另外最近会把以前鸽的一篇计算流体力学有限元的文章补上(确实太鸽了)
前段时间一直没有怎么写代码，而且python也已经有六七个月没写了，导致感觉python几乎不会写了(所以拿这个来回回火)
也希望接下来一切顺顺利利 ，之后也会学习更多广的知识

这一篇文章比较长，讲的也很详细，如果需要，请耐心看完。
# 二、问题简介  
 这一期其实也是是我们疲劳与断裂的课程的一期作业，要求是使用疲劳拉伸实验机的实验数据，绘制实验曲线并画出相关的图像
 疲劳的实验数据可以从下面链接获取: 
 链接：https://pan.baidu.com/s/1asfeN2usBPBAn-F53AfKGw?pwd=kzek 
提取码：kzek

另外此处补充预置实验参数: 
1. **加卸载应力的周期为200s**
2. 从表格中可以看出，共进行了20000s实验，即100次循环
![在这里插入图片描述](https://img-blog.csdnimg.cn/94e80c0118b24a9aa65f4cae3eb9dadb.png)
这一期的话我们主要讲滞回环的绘制和应变-寿命曲线的参数拟合和绘制过程， 而应力-循环次数曲线比较容易， 因此仅略讲
# 三、滞回环曲线的绘制过程
## (1) 平均区段选取方案
首先我们给出课本上应变控制下低碳钢拉压疲劳的滞回环(参考书目[1]p70) 
![在这里插入图片描述](https://img-blog.csdnimg.cn/2f54d0b97e804523a816217449720c1b.png)
首先我们很容易从表格中提取出应力数据和应变数据(其中第三列的应边百分比不用管)，而绘制出大体的实验数据，我们只需要下列几行代码
```cpp
import pandas as pd
import matplotlib.pyplot as plt

data = pd.DataFrame(pd.read_csv("fatigue_analysis_data.CSV"))
print(data)
x = data["strain "]
y = data["stress(MPA)"]
plt.figure()
plt.plot(x,y)
plt.show()
```
绘制得到的曲线图如下: 
![在这里插入图片描述](https://img-blog.csdnimg.cn/bcf1ac44dfd44b70ba1a1bef9158f552.png)
但是由于实验获取的曲线过于密集，我们希望得到分布的曲线来看出随着加载路径加载和卸载情况下，应力和应变的整体变化关系，从而看出这个材料的应力随着加卸载的循环是逐渐减小的还是逐渐增加的，不想要如此密集的曲线。我们下面思考如何获取我们希望得到的图像。
首先为了区分不同的曲线， 我们利用周期200s, 将实验数据分成不同的周期, 多次使用plot保证每个周期不相同，只需稍加改动代码: 
```python
data = pd.DataFrame(pd.read_csv("fatigue_analysis_data.CSV"))
print(data)
t = data["time(s)"]
x = data["strain "]
y = data["stress(MPA)"]
plt.figure()
for k in range (100):
    x_i = (np.array(t >= k * 200) * np.array(t < (k+1) * 200))
    # 中间的乘是逻辑的与
    plt.plot(x.loc[x_i], y.loc[x_i])
plt.show()
```
容易得到下图: 
![在这里插入图片描述](https://img-blog.csdnimg.cn/5d22e124b1c049dabe144252971af430.png)
但是最主要的问题是， 这个曲线的变化太缓慢，分布太密集了，我们希望得到一个更加稀疏和反映整体应力随加载路线变化的曲线, 最终同时反映出滞回环的变化, 那么如何得到这样的曲线呢? 

为了得到这样的曲线， 我们可以有思路是将上述曲线的"几圈"合并为"一圈"并使用一个"平均值"来代替上述曲线的值。我们假设整个加载过程分为$m$个小圈，而这$m$个小圈相互衔接，得到我们的最终曲线。其核心问题，**一是怎么分配平均曲线(用那几条曲线进行平均化处理), 二是怎么平均化处理(如何通过要平均的曲线得到最终的曲线)**。

1. 首先我们思考使用$m$个小圈首尾相连得到最终曲线的办法，即**将曲线分为几个连续的不同周期， 然后每个周期赋值不同的权重将这条曲线"平均化"出来并保留初始和最终应力**。但是这样毕竟是比较难以实现的。而且难以保证衔接处的导数的较好的连续性。
2. 我们仍然从思考使用附近的曲线来平均得到这条曲线的办法，我们设总共有$n$条曲线(即n个周期,题目中n =100)，这些曲线相当与一个个样本点，我们只需要取样本点附近的部分对其进行平均化处理即可。另外， **在平均化处理过程中， 还有两个基本要求，一是不能丢失初始的加载过程，二是要反映最终的滞回环的趋于稳态的特性**。为了保证初始加载过程符合要求， 我们**对第一圈不做平均化处理**。同时为了真实反映低碳钢加卸载随着实验进程变化的关系，我们**必须采用等时间的循环步长**(其实最好是等周期)，即不能开始时平均3条曲线而之后平均30条曲线，这也会导致曲线失真。

在平均化时，我们假设除了1之外，我们共有$m$圈，通过$m$圈计算出每次平均$k$圈, 首先一侧的平均化方法难以保证连续性，我们考虑两侧的平均化方法， 以下以$n = 20, k = 5$, 每次以相同的奇数间隔或者偶数间隔取区间分布，最终平均方案举例如下。
![在这里插入图片描述](https://img-blog.csdnimg.cn/490dcfa56e834c5187e6b4240ba24d32.png)
下面是偶数区间分布(即取$k/2$为每次跃进的步长), 要求最后$k$要舍入为偶数。
![在这里插入图片描述](https://img-blog.csdnimg.cn/7cdac27f0cd948cc9fde425ec0c6d3f3.png)
上述方法中， 我们看出在奇数和偶数区间时，$k$往往不是整数，可能需要进行舍入获取实际的$k$, 而**这种选取方法导致$k$不确定， 从而不能满足我们总绘制圈数确定为$m$的要求**， 同时这也导师$m$有一定变化时，选取的$k$变化较大(浮动范围可能达到2)，我们希望有更好的方案。

我们比较上面两种方案，都是通过一定步长跃进加上一定区间平均，**我们得出第三种方法，即步进法** 
步进法的基本思路是使用
$$l = \text{floor}(\frac{n-1}{m})$$
(其中floor为不大于的整数)为每次步进的步长， 显然这个步长$l$经过$m$圈不会超出剩余个数上限，而为了让所有的m个都能参与到平均中，只需取这个步长到最后刚好平均到最后一个即可，而步长左侧个数$l$, 右侧个数$k - l$，即1经过$m-1$次步进之后恰好有$k-l$个, 得到方程
$$1 + k \cdot (m - 1)+ (k-l) = n$$
![在这里插入图片描述](https://img-blog.csdnimg.cn/bba56e687e9d425e99ddae928bc42103.png)
这个得到的每次平均的圈数为$k$个，而共有$m$组， 可以组装成$m \times k$的矩阵, 显然是一种更好的方法。而且保证步长在$n$变动时$k$舍入变化不会超过1, 从而有一定的稳定性。

需要说明的是，我们应当适当调整$m$值来在上述计算中获取到较小的$k$值，使得平均的的圈数更有局部性。在最后程序中，我们输出了$k$的取值($n=100$且调整绘制圈数$m = 19$时, 可以取到$k = 14$较小, 绘图中的每一圈会以$l = floor199/19 =10$为步长并每次以14圈为平均圈数)

## (2) 曲线平均的拟合方法
接下来就是如何拟合曲线的问题了，在数据中，每200s为一个周期, 如果你把前200s(第一周期)的数据绘制出来，得到的是一个这样的图。
![在这里插入图片描述](https://img-blog.csdnimg.cn/338616679bcc46a394232d068d68255b.png)
所以每一次我们按照1中讲的方案，**每一次将相应序号的几圈曲线平均起来，作为下一个环**， 但是在平均过程中，可以发现，由于间隔大多数是2s, 所以一般一个周期(200s)大多是100组数据点, 但是*由于时间取得并不是均匀的，仍然有许多不是100组的数据*。且时间分布也不相同。

例如在数据中，有一段这样的数据 : 
![在这里插入图片描述](https://img-blog.csdnimg.cn/cab1b64ad8af4b0b8c3ae37922840bba.png)
其中第一行是时间， 可以看出并不完全是2s间隔，从而在确定周期内的布点时，我遇到过有一个周期105个点的情况，且在一个周期内时间分布情况和以前的周期不同。相当于在下图中，给出黄，蓝两条曲线，寻找"平均曲线"红线
![在这里插入图片描述](https://img-blog.csdnimg.cn/22ceae080489462e82c86b12f9f86d9e.png)
首先由于每组曲线都是一个周期， 我们将需要平均的曲线投影到一个周期内，必定会得到一组x,y对应的数据(称为"原数据点")，相当于给出一组数据进行插值曲线绘制。
![在这里插入图片描述](https://img-blog.csdnimg.cn/e06578f5079d47119ec5b27a6d6b0d84.png)
根据上述的方法， 我们只需先建立一个插值坐标 $[0,2, 4, ....200]$, 然后定义一个插值半径$r$(Interp_radius), 而对每一个插值坐标点，其附近(x绝对值相差)$r$以内的原数据点认为对这个点的插值坐标值有影响，我们即可用原数据点y值的加权平均计算插值点的y值，而本程序为反映性质和方便，直接使用附近点的平均值作为插值点的$y$坐标。(本程序边界处不进行线性插值)

然后我们只需先计算出插值要的圈数序列(`Step Array`)，如图所示: 
![在这里插入图片描述](https://img-blog.csdnimg.cn/5d848b47c3f64df89cfb850b5a542f38.png)
其中0是第一圈，其余的每一行是一圈，用(2)中讲的办法平均得到这一圈的曲线， 从而得到如下的图像: 
![在这里插入图片描述](https://img-blog.csdnimg.cn/a269da46bd9147fab37d3f37ff8008f4.png)
这个曲线上方的间断是由x= 0处不连续绘制造成的，我们只需定义一个`self.last_stress`和`self.last_strain`来记录上一个点的位置即可。

整个程序的代码如下: 
```python
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# 对几组数据采用取平均值方法处理, 绘制出较为分散的线条来反映物体的加载路径, 从而看出是硬化还是软化材料
# 我们先将实验数据进行整体分组, 将每200s设置为一组
# 为了保证边值清楚, 第一圈只使用第一组数据, 后面的数据由平均前后多圈的数据得到, 而过程中使用平滑进行衔接
# 为了保证滞回环的出现, 平均的圈数应当是相等的。
class Fatigue_Analysis():
    def __init__(self):
        # ======== 绘图控制自选参数 =======
        self.drawCircNum = 19  # 绘制滞回环的圈数m (需要注意的是, 采用不同的m会有不同的平均圈数产生，以11, 33为佳)
        '''其中33时步长为3而平均个数为6,有很好的效果'''
        self.T = 200                       # 定义周期
        self.Interp_segments = 100         # 插值分段部分 -> 分为100 段进行插值
        self.Interp_radius = 2             # 插值半径取2(由于最后加和是使用对应的点)
        self.Interp_dx = self.T/self.Interp_segments  # dx 由周期内的分段决定(不改动)
        # ======== 计算参数 =======
        self.data = pd.DataFrame(pd.read_csv("fatigue_analysis_data.CSV"))
        # plt.rcParams['font.sans-serif'] = ['SimHei']  # 这一行用来正常显示中文标签
        self.fig = plt.figure()            # 绘制图像
        self.length = len(self.data)       # 总数据长度
        self.time_span = self.data["time(s)"]  # 列数据
        self.tot_time = self.data["time(s)"].iloc[-1]  # 获取总的模拟时间
        self.cycNum = self.tot_time/self.T             # 周期数n, 用于计算对应平均的次数

        # ----- 使用步进平均思路进行绘图->设步进长度为step, 平均需要的个数为k ------
        self.step = np.floor((self.cycNum - 1) / self.drawCircNum)       # 计算步进平均长度 l = step
        self.k = (self.cycNum - 1) - self.step * (self.drawCircNum - 2)  # 计算每组元素的个数 k
        print("Average Circle number when plotted: ", self.k)
        print("Note You can adjust the 'DrawCircleNum' to make it lower to reach a higher calculation speed")
        self.Step_Array = np.zeros((int(self.drawCircNum-1), int(self.k))) # 生成步进法对应的绘制数据平均区间(直到99)
        for i in np.arange(self.drawCircNum-1):
            start = 1 + i * self.step
            stop  = start + self.k
            self.Step_Array[i,:] = np.arange(start, stop)


        # ===== 定义投影周期内的插值点范围 ====
        self.Interp_points = np.arange(0, self.T, self.Interp_dx)
        # ===== 调用绘图函数 =========
        self.draw_Fatigue_Cycle() # 绘制滞回环图
        self.draw_max_stress()    # 绘制最大应力幅值-循环次数图
        plt.show()

    # ===== 将数据分为许多圈, 并分别采用不同的颜色进行绘制 =======
    def draw_Fatigue_Cycle(self):  # 疲劳循环图(滞回环)的绘制
        # 处理数据，先单独取出第一周期的内容并单独画出
        ax1 = self.fig.add_subplot(121)
        fstCol = np.array(self.time_span < self.T)  # 注意不能使用self.timespan[self.timespan < self.T]
        fstStrain = self.data["strain "].iloc[fstCol]
        fstStress = self.data["stress(MPA)"].iloc[fstCol]
        ax1.plot(fstStrain,fstStress)
        # 使用平均方法, 平均多个周期内的数据并绘制对应的循环应力图
        # 需要注意的是，此处默认得到的不同周期内的数组长度是相同的，如果不同还需要进行处理
        self.last_strain = fstStrain[len(fstStrain) - 1]
        self.last_stress = fstStress[len(fstStress) - 1]
        for dc in np.arange(0,self.drawCircNum-1,1):
            # 将每个部分取平均并求解出
            Tot_Strain = np.zeros(self.Interp_segments)
            Strain_adds = np.zeros(self.Interp_segments)   # 记录对应的插值点个数
            Tot_Stress = np.zeros(self.Interp_segments)
            Stress_adds = np.zeros(self.Interp_segments)
            for k_0 in self.Step_Array[dc]: # k_0 是相应的周期, 其中长度默认为100, 对数据进行平均处理
                # 获取插值所用的行
                cols_t = np.array(self.time_span >= k_0 * self.T) * np.array(self.time_span < (k_0 + 1) * self.T)
                cols = np.arange(len(cols_t))[cols_t]
                time_eq = self.data["time(s)"].iloc[cols] - k_0 * self.T      # 将多个周期的时间投射到一个周期(0-200s)内
                # 对于每一个time_eq中相应的点，附近的点如果在插值半径以内, 则给它们加上平均值, 此处我不优化代码
                # 对于数据进行加和, 在每一组加完之后，进行平均
                for c in cols :   # c 必须是对应的下标
                    t = time_eq[c]       # 取相应的时间, 时间附近r以内的, 进行平均处理
                    avg_points = np.abs(self.Interp_points - t) < self.Interp_radius   # avg_points 是需要平均的点
                    Tot_Stress[avg_points] += self.data["stress(MPA)"].iloc[c]
                    Stress_adds[avg_points] += 1
                    Tot_Strain[avg_points] += self.data["strain "].iloc[c]
                    Strain_adds[avg_points] += 1
            # 通过添加last_sigma对断点进行修复
            Ave_Stress = np.append(self.last_stress, Tot_Stress / Stress_adds)
            Ave_Strain = np.append(self.last_strain, Tot_Strain / Strain_adds)
            # 另外，为了避免每个周期都是间断的, 我们每一次都记录上一次的末值与这个进行拼接，得到第二次的绘制
            # 画一圈滞回环
            ax1.plot(Ave_Strain, Ave_Stress)
            self.last_stress = Ave_Stress[len(Ave_Stress)-1]
            self.last_strain = Ave_Strain[len(Ave_Strain)-1]
        ax1.set_xlabel('Strain($\\epsilon$)')
        ax1.set_ylabel('Stress($\\sigma$)(MPA) ')
        ax1.set_title("Hystersis (Stress-Strain)")

    # 绘制最大应力-循环次数曲线
    def draw_max_stress(self):
        ax2 = self.fig.add_subplot(122)
        strain_span = self.data["strain "]
        stress_span = self.data["stress(MPA)"]
        x_axis = np.arange(0,2 * self.cycNum,1)

        self.max_stress = np.empty(2 * (int)(self.cycNum))
        # 由于是每半个周期出现一次最大应变, 所以每隔半周期取一次, 一次最值在0-T/2内，另一次在T/2-T内
        for c in x_axis:
            Half_Cycle = np.array(self.time_span >= c * self.T/2) * np.array(self.time_span < (c + 1) * self.T / 2)
            self.max_stress[int(c)] = np.max(np.abs(stress_span[Half_Cycle]))
        ax2.plot(x_axis, self.max_stress)
        plt.xlim([0,210])
        plt.ylim([200, 400])
        ax2.set_xlabel('2 x Cycle Number(2 $\\times$ N)')
        ax2.set_ylabel('Max_Extension_Stress($\\sigma_{\\max}$)(MPA)')
        ax2.set_title("Stress-CycleNumber Curve")

if __name__ == "__main__":
    Fatigue_Analysis()
```
绘制效果如图: 
![请添加图片描述](https://img-blog.csdnimg.cn/6ad943d3dfc14e8cb9088ad0960caa07.png)
左侧是得到的疲劳滞回环曲线，右侧的应力-循环次数曲线只需要将曲线分解为多个周期并且每次取绝对最大值即可。

左侧的曲线很容易反映出在疲劳循环过程中的软化现象(应力降低)， 同时由于在内侧曲线处更为密集，可以看出滞回环是有收敛现象的。

# 四、拓展部分
除了应力-循环次数曲线，我们在常见的疲劳实验中还有应变-寿命曲线，这个曲线是通过控制不同的应变得到的寿命曲线。
![在这里插入图片描述](https://img-blog.csdnimg.cn/aeda0ab93b434fa396e92f6f3c497ad5.png)
在物体拉伸过程的应变分析中， 我们使用胡克定律得到弹性应变的公式为 $\varepsilon_{e} = \frac{\sigma}{E}$, 而Holomon关系给出了塑性应变与变形的关系$\varepsilon_{p} = K \varepsilon_{p}^{n}$ 从而我们假设物体的总应变为:  
$$\varepsilon = \varepsilon_{e} + \varepsilon_{p} = \frac{\sigma}{E} +\left(  \frac{\sigma}{K'}\right)^{\frac{1}{n'}}$$
我们假设疲劳实验过程中的应变和单向拉伸的应变为: 
$$\varepsilon_{a} = \varepsilon_{ea} + \varepsilon_{pa } = \frac{\sigma_{a}}{E} +\left(  \frac{\sigma_{a}}{K'}\right)^{\frac{1}{n'}}$$
其中$a$表示疲劳实验中的量。

我们在整体的疲劳实验中， 有结论$\lg \varepsilon_{ea} - \lg 2N$和$\lg \varepsilon_{pa} - \lg (2N)$都是对数线性关系, 即: 
$$\varepsilon_{ea} = \frac{\sigma_f'}{E} (2N)^b \qquad\varepsilon_{pa} = \varepsilon_f' (2N)^c$$
因而有： 
$$\varepsilon_{a} = \frac{\sigma_f'}{E} (2N)^b + \varepsilon_f' (2N)^c$$
上式反映了弹性应变幅$\varepsilon_{ea}$与寿命$N$之间的关系, $\sigma_f'$称为<mark style="background: transparent; color: red">疲劳强度系数</mark>。其中$b$为<mark style="background: transparent; color: red">疲劳强度指数</mark>, 一般为$-0.06\sim -0.14$, 而$\varepsilon_{f'}$称为<mark style="background: transparent; color: red">疲劳延性系数</mark>，而$c$为<mark style="background: transparent; color: red">疲劳延性指数</mark>一般为$-0.5\sim -0.7$, 可以使用$-0.6$估计。

我们按照上述要求，如果知道实验曲线， 可以利用$b = -0.1, c= -0.6$估计曲线中的参数$\sigma_f', \varepsilon_f'$, 在实际估计时，可以采取双参数的方法， 通过不断调整参数和计算误差来拟合曲线，也可以将曲线分段， 在左侧部分塑性形变较大，可忽略弹性形变， 右侧部分塑性形变小， 而弹性为主要形变，可以对相应的参数进行估计。

# 五、参考书目
[1] 《疲劳与断裂》第二版 杨新华， 陈传尧 华中科技大学出版社
