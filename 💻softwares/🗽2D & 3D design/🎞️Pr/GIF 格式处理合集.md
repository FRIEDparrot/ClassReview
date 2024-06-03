# Pr批量去gif白边
50 个带有白边的gif图片, 如果使用Pr去除白边效率极低, 

在assembly中找到 VideoEffects > Keying > Color Key 并拖拽到要修改的gif上
![[attachments/Pasted image 20240526230933.png]]
此时右边可以找到对应的Video Effect Controls的面板

修改Key Color为背景色黑色, 并修改Edge Thin 和Edge Feather的值
![[attachments/Pasted image 20240526232145.png]]
依次修改即可, 最终导出视频

另外, 如果背景具有一定渐变， 也可以修改 Tolerance 的值, 实现gif批量去白边的效果

## 循环播放gif的预览方法
从视频右下角的Settings找到Loop，勾选即可循环播放
![[attachments/Pasted image 20240527004418.png]]



## 切割视频之后批量导出文件

首先将视频切分为多段, 然后在每一段中点击 Nest, 来新建嵌套序列
![[attachments/Pasted image 20240527002312.png|500]]

在左侧项目窗口中, 把所有的要导出的部分选上, 然后点击File>Export
![[attachments/Pasted image 20240527011634.png]]
但是这样只有在有特定APP时才能用

导出注意选择Animated GIF , 不用GIF格式导出

如果帧过大,更改帧大小在Sequence> Sequence Settings 中即有。
![[attachments/Pasted image 20240527013134.png|500]]

