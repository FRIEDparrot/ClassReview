[比较重要的控件教程可以在这里找到](https://doc.qt.io/qtdesignstudio/index.html)
### 1. Button 
对于一个按钮，可以根据Style 来改变对应的背景颜色， 但是使用背景时， 仍然使用图来进行
![[attachments/Pasted image 20230603142854.png|600]]

下面的States部分用于添加窗口状态
![[attachments/Pasted image 20230603143112.png|600]]

在类似于JavaSript语法的UI编辑器中，进行调用函数

### 2. Mouse Area 

mouse area 的界面如下图:
![[attachments/Pasted image 20230603143731.png]]

在Qt的按键绑定中，可以自定义使用按键，可以使用如下的代码并使用 `|` 来分割按键(表示"或")，并可以在CursorShape中定义预制的不同的鼠标形状
![[attachments/Pasted image 20230603144438.png|700]]

使用 prevent stealing 选项可以防止MouseArea中的鼠标被限制在MouseArea中，但是仅在下一个click事件发生前起作用
![[attachments/Pasted image 20230603145325.png|400]]
对于下方的drag选项，如果是渐进拖动的，勾选smooth即可实现

在Advanced选项卡中, 如果需要设置焦点(Focus), 只需将其中的Focus选项选上即可。但是如果是可导出的 assets 组件时， 则需要使用到 Focus Scope 组件
在Focus Scope组件中，如果有多个设置了Focus组件的部分，则仅最后设置Focus的组件有Focus 

### 3. Filckable 组件
Flickable组件可以允许窗口的大小进行动态滑动调整, 只需要将properties中的Interactive 设置为true即可

![[attachments/Pasted image 20230603154725.png]]


### 4. Positioner 
1. Column Positioner
2. Row and Flow Positioner 
3. Grid Positioner  `->` 指定放置的行，列等等， 可以使用gridPositioner 来进行
![[attachments/Pasted image 20230603205726.png|750]]

