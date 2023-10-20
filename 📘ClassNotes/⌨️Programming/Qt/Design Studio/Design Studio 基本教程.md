### 1. 工程标题的设置

在App.qml中设置整个窗口的标题
![[attachments/Pasted image 20230603001719.png|700]]

常见的Qt组件有QML types, Resources, Timeline (用于设置动画) 等等， 需要注意Navigator 图标，用于创建对应的组件系统， 


![[attachments/Pasted image 20230603073957.png|700]]

添加组件联系的方法: 
![[attachments/Pasted image 20230603074232.png|700]]

2. 多种不同的类型(在main.cpp中进行声明)

主要的逻辑部分是在main.cpp中进行编写

```cpp 
#include <QGuiApplication>     // Gui命令程序
#include <QCoreApplication>  // 控制台命令程序

```