原视频链接 https://www.youtube.com/watch?v=SPteuq0-WLk 

# 许可证注意事项
在进行求解时,  需要保证 lmtools 中的 ANSYS License Manager CVD 和 Simens PLM License Server 都处于开启状态(SSQ 是给Abaqus 使用的)
![[attachments/Pasted image 20240331102106.png]]
不能开可以到task manager 或者Services 中强制开启。 
![[attachments/Pasted image 20240331102653.png]]

# UG 参数化配置
首先， 在 Tools `->` Expression 中定义某个参数,  需要注意, 只有以 ANS_或者 DS_开头，才能被WorkBench识别; (这个需要在WorkBench 中设置)

![[attachments/Pasted image 20240321092339.png|1200]]
在几何模型中， 把对应的部分设置为相关的参数 
![[attachments/Pasted image 20240321094640.png|600]]
启动WorkBench时， 需要保证 lmtools 中的 License Server 是打开状态; 
然后从关联(用CAD configuration Manager 关联)的 workBench 进行打开(注意不能有小黑牛), 具体办法如下: 
![[attachments/Pasted image 20240321093608.png]]
# DesignModeler 参数化方法 
选取 Edit Geometry in DesignModeler, 
![[attachments/Pasted image 20240321095115.png]]
设置识别的 Parameter Key 
![[attachments/Pasted image 20240321095228.png]]
拉到最下面把两个参数的 Parameters 进行勾选
![[attachments/Pasted image 20240321095312.png]]
可以在右侧 Table of  Design Points中添加一组值对应的属性; 而对应的属性在参数集中也已经给出: 
![[attachments/Pasted image 20240321095534.png|1300]]

> [!caution] 说明
> 使用Ansys workBench 中的Retain属性， 可以指定计算的设计点， 而 Current Design Point 是当前的设计点， 且只能指定一个。
> 另外， 在计算之后，可以将某个计算的部分设置为具有 Parameter 属性， 并且可以通过Update From The 

![[attachments/Pasted image 20240321104017.png]]

在新的一组属性值上， 选用 Set As Current 。(其中注意Retain需要勾选)
![[attachments/Pasted image 20240321095657.png|700]]
然后重新选择Generate部分 
![[attachments/Pasted image 20240321100023.png]]
例如, 更改参数为拉伸长度10, 初始拉伸为5时， 显然已经实现了几何模型的更新;
![[attachments/Pasted image 20240321100236.png|1000]]
注意: 一定要使用非结构化的网格部分, 才能进行计算; 

另外， DesignModeler 中本身也提供了Attach to Active CAD Geometry 的方法, 可以直接连接到参数化几何模型。 
![[attachments/Pasted image 20240321214101.png]]

# 在多设计点下的响应计算方法

如图， 如果我们将最大主应力设置到参数集中， 然后每一次都进行计算， 则我们可以得到不同设计点下的主应力;
![[attachments/Pasted image 20240321105215.png]]


![[attachments/Pasted image 20240321110846.png|700]]

在绘制图表时， 先选中 Parameters Chart  
![[attachments/Pasted image 20240321112729.png]]
然后在 retain 中选中图表绘制需要的数据点, 然后即可绘制出对应的图

![[attachments/Pasted image 20240321113440.png]]

对于每一个Properity, 都可以指定对应的表达式, 例如 20 * P1 
![[attachments/Pasted image 20240321113620.png]]

# 使用Python语言的 Ansys日志文件以及运行方法
使用 日志文件 (Journal files) 连接Matlab 和Ansys Workbench 是非常有效的Ansys 的MATLAB命令流方案。

首先在 Parameter 中选中并添加一个参数, 形成如图的类似的结构

![[attachments/Pasted image 20240321115530.png|600]]

在Workbench 中， 使用  File > Scripting > Record Journal  
![[attachments/Pasted image 20240321120303.png]]

选择保存一个Journal文件(.wbjn)

然后为了修改，  修改或者添加输入参数的值， 则日志会自动记录到 Journal 中。

为了在日志中能够有 生成.csv文件的代码， 需要在参数中选中得到的结果 Output 参数, 然后使用Export Table Data as CSV 
![[attachments/Pasted image 20240321115816.png]]
得到如图所示的表格: 
![[attachments/Pasted image 20240321120230.png]]

日志文件示例如下: 
```python
# encoding: utf-8
# 2024 R1
SetScriptVersion(Version="24.1.144")
designPoint1 = Parameters.CreateDesignPoint()
parameter1 = Parameters.GetParameter(Name="P3")
designPoint1.SetParameterExpression(
    Parameter=parameter1,
    Expression="400000 [Pa]")
parameter2 = Parameters.GetParameter(Name="P4")
designPoint1.SetParameterExpression(
    Parameter=parameter2,
    Expression="350 [C]")
backgroundSession1 = UpdateAllDesignPoints(DesignPoints=[designPoint1])
designPoint2 = Parameters.CreateDesignPoint()
parameter3 = Parameters.GetParameter(Name="P1")
designPoint2.SetParameterExpression(
    Parameter=parameter3,
    Expression="6")
designPoint2.SetParameterExpression(
    Parameter=parameter1,
    Expression="200000 [Pa]")
designPoint3 = Parameters.GetDesignPoint(Name="6")
designPoint3.Retained = True
designPoint4 = Parameters.GetDesignPoint(Name="7")
designPoint4.Retained = True
designPoint1.Retained = True
designPoint2.Retained = True
backgroundSession2 = UpdateAllDesignPoints(DesignPoints=[designPoint2])
Parameters.ExportAllDesignPointsData(FilePath="C:/Users/Parrot/Desktop/Recent/毕设工程文件/TJP_Simulation/.TJP_parametric_test1_files.backup/DataTest1/testData.csv")

# encoding: utf-8
# 2024 R1
SetScriptVersion(Version="24.1.144")
Save(Overwrite=True)
Open(FilePath="C:/Users/Parrot/Desktop/Recent/毕设工程文件/TJP_Simulation/TJP_simulation.wbpj")
Open(FilePath="C:/Users/Parrot/Desktop/Recent/毕设工程文件/TJP_Simulation/TJP_simulation.wbpj")
Save(Overwrite=True)
Open(FilePath="C:/Users/Parrot/Desktop/Recent/毕设工程文件/TJP_Simulation/TJP_material_optimization.wbpj")

```
在matlab端， 需要修改 Ansys自带的 journal文件, 然后保存并且每一次使用ansys运行这个journal文件，就可以每一次获取到不同的分析结果了。
```matlab
tic clc; clear; 
%% Changing the input parameters 
Length = 25; Height = 10; 
% creating new journal file 
fid = fopen('base journal.wbjn','r'); 
f = fread(fid,'*char')'; fclose(fid);
f = strrep(f,'Length' , num2str(Length));   %% string replace, 其中把上面代码中要改变量的换成 Length命名即可
f = strrep(f,'Height' , num2str(Height));    %% string replace , 其中把上面代码中要改的换成 Height命名即可
fid = fopen('finaljournal.wbjn','w'); fprintf(fid,'%s',f);
fclose(fid); %% Calculations in ansys with input parameters 
system('C:\"Program Files"\"ANSYS Inc"\v212\Framework\bin\Win64\RunWB2.exe -B -R finaljournal.wbjn'); 
%% 也可以将其中 -B改成-X来使用窗口运行，-B 是batch mode, 更快
%% saving the output parameter 
Nusselt_no = xlsread('export data.csv','export data','D8'); 
toc
```




一个示例程序如下: 

```MATLAB
clear, clc, tic;
target_mini_life = 10000;   % 研究在 15000 次循环下不同变量的变化对应的最大寿命灵敏度

P3 = 3;   % DS_LA_RIB_WIDTH3
P4 = 3;   % DS_LA_RIB_WIDTH2
P5 = 3;   % DS_LA_RIB_WIDTH1
P6 = 3;   % DS_LO_RIB_WIDTH1
P7 = 3;   % DS_LO_RIB_WIDTH2
P8 = 3;   % DS_IC_RIB_WIDTH
P9 = 2;   % DS_LO_RIB_WIDTH_END2
P10 = 2;  % DS_LO_RIB_WIDTH_END1
          % Life Minium 

mu_ = [P3, P4, P5, P6, P7, P8, P9, P10];

g = @(x) fem_res(x) - target_mini_life;    % 当有限元结果小于对应的目标寿命, g < 0, 认为失效

%% %%%%%%%%%%%%%%% 修改journal文件并进行分析 %%%%%%%%%%%%%%%%%% 
f1 = fopen("journal_life.txt", "r");
cmd = string(fread(f1,'*char')');

para_names = ["DS_LA_RIB_WIDTH3", "DS_LA_RIB_WIDTH2", "DS_LA_RIB_WIDTH1",...
              "DS_LO_RIB_WIDTH1", "DS_LO_RIB_WIDTH2", "DS_IC_RIB_WIDTH",...
              "DS_LO_RIB_WIDTH_END2", "DS_LO_RIB_WIDTH_END1"];
para_array = [P3, P4, P5, P6, P7, P8, P9, P10];

for i = 1:length(para_names)
    cmd = strrep(cmd, para_names(i), string(num2str(para_array(i))));
end

f2 = fopen("C:\Users\Parrot\Desktop\Recent\Graduate_Projects\TJP_Simulation\TJP_sensitivity_analysis_files\journals\exec.wbjn","w");
fprintf(f2, cmd);
fclose(f2); fclose all;
%% %%%%%%%%%%%%%%% 执行命令行调用ansys进行分析  %%%%%%%%%%%%%%%%%%%
system('"E:\Ansys2024R1\ANSYS Inc\v241\Framework\bin\Win64\RunWB2.exe" -B -R "C:\Users\Parrot\Desktop\Recent\Graduate_Projects\TJP_Simulation\TJP_sensitivity_analysis_files\journals\exec.wbjn"');

Res = importdata("Results.csv").data;
% 可以使用importData中并使用data提取其中的数据 
toc
```



> [!caution] 实际编程说明
> 如果是在实际编程中新增设计点， 建议新增之后， 先设置最新的一个设计点Retain为true, 然后将这个设计点设置为current再进行求解，否则可能出现 Geometry Attached Failed 等等问题导致求解不了 。
> 在求解完毕之后，取消勾选最新设计点的retain选项，即可只保留所选参数的同时保证求解成功率更高
> **在求解过程中一定要保证两个license服务都是稳定运行的**

另外， 可以通过命令行中设置命令窗口的默认大小(Window Size), 这样后台运算时不容易影响到前台。 
![[attachments/Pasted image 20240402201523.png]]
 