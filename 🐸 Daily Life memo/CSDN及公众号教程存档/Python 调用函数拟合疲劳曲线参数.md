


```python 
# 使用梯度下降误差的方法估计Remberg-Osgood参数的取值范围  
# 我们仅取用0-50s的情况  
def Calculate_Parameters(self):  
    # 取出第一个周期的实验滞回环数据  
    simudata = self.data.iloc[np.arange(0, self.T/4)]  
    sdata = simudata["strain "]  
    # 使用 argmax 求解最值的下标  
    maxid = np.argmax(sdata) # 最大应变时, 取到最大应力  
    self.firstdata = self.data.iloc[np.arange(0, maxid, 1)]  
  
    self.fststress = self.firstdata["stress(MPA)"]  
    self.fststrain = self.firstdata["strain "]  
    ax = self.fig2.add_subplot(131)  
  
    # 首先使用前10组数据最小二乘求解样式模量E, 此处为了方便直接使用index = 9处的斜率代替  
    Efit1 = self.fststress.iloc[np.arange(0,10)]  
    Efit2 = self.fststrain.iloc[np.arange(0,10)]  
    # 最小二乘法拟合直线参数是k和b  
    z = np.polyfit(self.fststrain.iloc[np.arange(0,10)], self.fststress.iloc[np.arange(0,10)], 1)  
    print("E = ",z[0]/1e3, "GPa, ", "b = ", z[1]) # 得到的是k,b  
  
    E = z[0] # 140 GPa , 注意E数值的单位是 MPa    ax.plot(self.fststrain, self.fststress)  
    # 绘制sigma - epsilon_e曲线 - 此时横坐标为epsilon_e  
    ax.plot(1/E * self.fststress , self.fststress)  
  
    # 拟合K, n'两个参数  
    plastic_strain = self.fststrain - self.fststress/E  # 这个是在sigma不变的情况下的epsilon_p, 单位均MPa  
    ax.plot(plastic_strain, self.fststress)  
    plt.legend(["Total Strain", "Elastic Strain", "Plastic Strain"])  
    ax.set_xlabel("strain"), ax.set_ylabel("stress")  
    ax.set_title("strain - total stress")  
  
    # ======== 使用scipy中的函数拟合另外两个参数 K, n', 注意塑性应变计算过程中单位均为国际单位==========  
    def func(x, K, n):  
        return 2 * (x / (2 * K)) ** (1 / n)  
    ax2 = self.fig2.add_subplot(132)  
    x_0 = self.fststress  
    y_0 = plastic_strain # 定义应力为横坐标，塑性应变为纵坐标  
    ax2.plot(x_0, y_0, color = "blue", linewidth = 1.5)  
    p0 = [1100 * 1e6, 0.25]  
    ax2.plot(x_0 , func(x_0 * 1e6 , p0[0], p0[1]), color = "orange", linewidth = 1.5) # 注意横坐标仍然MPa, 计算纵坐标需要转换为Pa  
    plt.legend(["Real Curve", "Original parameter", "fitted parameter"])  
    # 拟合时， 代入x_0需要乘10^6  
    para,conv = optimize.curve_fit(func, 1e6 * x_0, y_0, p0=p0)  # 其中p0是定义初始迭代参数  
    print("K =", para[0]/1e6," MPa, ", "n' = ", para[1])  
    ax2.plot(x_0, func(x_0 * 1e6 , *para), color = "red", linewidth = 2.0) # 绘制拟合完成塑性应变-应力曲线  
    ax2.set_ylabel("strain"), ax2.set_xlabel("stress")  
    ax2.set_title("plastic strain - total stress")  
  
    ax3 = self.fig2.add_subplot(133)  
    # 计算真应变  
    fit_strain = self.fststress/E + func (self.fststress * 1e6, *para)  
    ax3.plot( self.fststrain, self.fststress, color = "blue", linewidth = 1.5)  
    ax3.plot(fit_strain, self.fststress, color = "red", linewidth = 1.5)  
    plt.legend(["Real Curve", "fitted curve"])  
    ax3.annotate("K = {:.3f} MPa".format(para[0]/1e6), xy = (0.002,130))  
    ax3.annotate("n' = {:.2f} ".format(para[1]), xy = (0.002,80))  
    # 其中可以用round控制输出精度, 也可以使用上述输出格式控制  
    ax3.set_title("curve fit")
```


![[Excalidraw/Python 调用函数拟合疲劳曲线参数 2023-10-14 09.22.45]]