### (1) 常用关键字
Visual Basic 中有许多常用关键字，以下是一些常见的关键字及其解释：

1. **Dim**：用于声明变量。例如，`Dim x As Integer` 声明一个整数变量 x。
2. **Private**：用于声明私有变量或方法，只能在声明它们的模块、类或结构中访问。
3. **Public**：用于声明公共变量或方法，可以在任何地方访问。
4. **Static**：用于声明静态变量，这些变量在程序运行期间保持其值。
5. **Sub**：用于定义一个子程序。
6. **End**：用于终止一个块，如 `End Sub` 终止一个子程序。
7. **If**：用于条件判断。
8. **Else**：与 `If` 一起使用，表示如果条件不成立时执行的代码块。
9. **For**：用于循环控制。
10. **Next**：与 `For` 一起使用，表示循环的下一次迭代。
11. **Function**：用于定义一个函数。
12. **Return**：用于从函数返回值。
13. **Select Case**：用于多条件判断。
14. **Try**：用于异常处理的开始。
15. **Catch**：用于捕获异常。
16. **Finally**：用于在异常处理结束后执行的代码块。
17. ByVal: 按值传递, 即传递的是值, 而非传递引用 
18. Const: 常量定义

vb 支持显式和隐式声明变量: 
```vb
Dim a as String 
a$ = "China 
```

Application.ProductName 获取当前的工程名称

结构体使用方法: 
```vb
Public Structure VarInfo
        Public Name As String
        Public Type As String
        Public Mean As Double
        Public Std As Double
        Public Loc As Double
        Public Scale As Double
        Public Sharp As Double
        Public Value As Double
    End Structure
```

### (2) 循环语句和条件判断
Do ...... Loop Until 循环:
```vb
Do
	str = FName.ReadLine
	ffx = CDbl(Microsoft.VisualBasic.Left(str, 8))
	ps = CDbl(Mid(str, 10))
Loop Until (ffx - Beta > 0.0)
```

For 循环的用法:
```vb
Dim FRead As New System.IO.StreamReader(FileToRead)
For i = 1 To IniLength - 1
	StrRead = FRead.ReadLine
	Continue For ' continue  
	Exit For  ' break
Next i    ' 加上或者不加 是等效的
```

Select....Case 条件判断语句:
```vb
Dim dayOfWeek As Integer
dayOfWeek = 3 ' 假设今天是星期三

Select Case dayOfWeek
    Case 1
        Console.WriteLine("今天是星期一")
    Case 2
        Console.WriteLine("今天是星期二")
    Case Else
        Console.WriteLine("无效的星期")
End Select
```

 在类中, Me 关键字和 this. 关键字是类似的, 可以在类以内访问相应的对象
 
### (3) 函数说明 
函数的相关说明: ByVal 和 ByRef 表示按照值传递和按照引用传递:
```vb
Sub Example(ByVal x As Integer)
    x = x + 1
End Sub

Public Sub ReadFEMPath(ByRef Path() As String, ByVal FileToRead As String) 
' a 的值仍然是 5
```

Sub 是一个循环调用的过程, 并且必须调用 Exit Sub 退出或者执行到 End Sub 结束, 否则
需要说明的是, sub 过程是没有返回值的, 相当于 void, 而对于具有返回值的部分, 应当采用 Function, 如下:
```vb
Function MyFunction() As Integer
    ' 执行一些操作
    MyFunction = 42 ' 返回一个整数值
End Function
```

类似于 python, 在 Visual Basic 中具有默认参数选项, 同时可以进行参数传递
```vb
' 函数的带有默认参数的定义方法
Public Function GetObject(Optional PathName As String = Nothing, Optional [Class] As String = Nothing) As Object

MsgBox("打开首选项" & num, Title:="首选项")   ' 支持将字符串和数字进行混用
```

采用Delegate 关键字的**函数指针**类型如下 :
```vb
Delegate Sub MyDelegate()
Dim functionArray As MyDelegate() = {AddressOf QianCL, AddressOf ZiBLSD, AddressOf FenXGC, AddressOf ZiBLYS, AddressOf XiangYLYS, AddressOf ShiXDY, AddressOf KeKKFX, AddressOf YouHFX, AddressOf FSX}
' 调用仅需要加入 invoke 方法 : 
functionArray(index).Invoke()
```

检测某个路径是否存在:
```vb
If My.Computer.FileSystem.DirectoryExists(CalcPath)
```


[官方文档和使用教程](https://learn.microsoft.com/en-us/visualstudio/get-started/visual-basic/?view=vs-2022) 
在新增界面时, 采用  View > Toolbox 
![[attachments/Pasted image 20241006100609.png]]

```vb
    Public Class GSAWindow_UI
        Public ui As Form_Main    '引用主窗体作为ui
        Public Property Analysis_Type As Integer
            Get
                Return ui.GSA_methodComboBox.SelectedIndex
            End Get
            Set(value As Integer)
                ui.GSA_methodComboBox.SelectedIndex = value
            End Set
        End Property

        Public Sub New(ByRef Form As Form_Main)
            Me.ui = Form
        End Sub
    End Class

    Public GSAwindow As New GSAWindow_UI(Form_Main)  '注意此处是将 property 进行传参
```

自带调用的 API 函数: 
1. Fun_Dis_Con : 生成某些指定的分布 
2. Get_Ps : 计算指定函数从-20到输入值的积分，在此处用来根据输入的可靠度指标值计算可靠度，即计算Phi值
3. Get_Ps1: 根据输入的可靠度指标值计算可靠度，即计算Phi值， Pf = 1 - Get_Ps(Beta) 
4. Lmd_miu: 计算均值灵敏度使用, 仅仅是存储值 
5. Lmd_sigma： 计算方差灵敏度使用, 用于存储值
6. Fun_Dx : 根据结构体中的相应参数以及定义的极限状态方程，利用微商近似求解函数在某点处的偏导数
7. Fun_Sx : 根据结构体中的相应参数以及Dx，计算一次二阶矩法过程中的灵敏度系数
8. Fun_Gx: 根据结构体中的相应参数以及所输入的方程，返回方程值

```vb
Private Sub Re_YCEJJ(ByVal StrToCalc As String, ByVal Dt As Double, ByVal MaxTime As Integer, ByRef Lmd_miu() As Double, ByRef Lmd_sigma() As Double, ByRef Design() As Double, ByRef Time As Integer, ByRef Beta As Double, ByRef Pf As Double)
	Try
		Dim NumVar As Integer
		NumVar = TableVar.Length - 1
		Dim Suma, Sumb As Double
		Dim BI, B As Double
		Dim Dx(NumVar), Sx(NumVar) As Double
		Dim Gx As Double
		Dim V(NumVar), C(NumVar) As Double
		B = 1
		BI = 0
		Time = 0
		For i = 1 To NumVar
			V(i) = TableVar(i).Mean
			C(i) = TableVar(i).Std
			Sx(i) = 0
		Next
		ListBox_Rel.Items.Add("----------------------------------------------------------------")
		ListBox_Rel.Items.Add("一次二阶矩法求解极限状态方程" & StrToCalc & "的可靠性")
		ListBox_Rel.Items.Add("求解的精度为：" & Dt)
		ListBox_Rel.Items.Add("最大迭代次数为：" & MaxTime)
		Do While (Math.Abs(BI - B) > Dt)
			If Time > MaxTime Then
				MsgBox("达到最大迭代次数，请修改相应参数后再试！", MsgBoxStyle.OkOnly + MsgBoxStyle.Critical, "警告")
				Exit Do
			End If
			Time = Time + 1
			B = BI
			For i = 1 To NumVar
				TableVar(i).Value = V(i) - B * Sx(i) * C(i)
			Next
			For i = 1 To NumVar
				If TableVar(i).Type = "正态分布" Then
					V(i) = TableVar(i).Mean
					C(i) = TableVar(i).Std
				ElseIf TableVar(i).Type = "对数分布" Then
					MsgBox("对数分布暂时不能使用该方法进行分析，请选择其他方法或者其他分布形式！", MsgBoxStyle.OkOnly + MsgBoxStyle.Critical, "错误")
					Exit Sub
				ElseIf TableVar(i).Type = "对数正态分布" Then
					Fun_Dis_Con_3(V(i), C(i), TableVar(i).Loc, TableVar(i).Scale, TableVar(i).Value)
				ElseIf TableVar(i).Type = "极值III型Weibull分布" Then
					Fun_Dis_Con_4(V(i), C(i), TableVar(i).Sharp, TableVar(i).Scale, TableVar(i).Value)
				ElseIf TableVar(i).Type = "极值I型Gumbel分布" Then
					Fun_Dis_Con_5(V(i), C(i), TableVar(i).Loc, TableVar(i).Scale, TableVar(i).Value)
				ElseIf TableVar(i).Type = "均匀分布" Then
					Fun_Dis_Con_1(V(i), C(i), (TableVar(i).Scale + TableVar(i).Loc), TableVar(i).Loc, TableVar(i).Value)
				ElseIf TableVar(i).Type = "指数分布" Then
					Fun_Dis_Con_2(V(i), C(i), TableVar(i).Loc, TableVar(i).Value)
				ElseIf TableVar(i).Type = "伽玛分布" Then
					MsgBox("伽玛分布暂时不能使用该方法进行分析，请选择其他方法或者其他分布形式！", MsgBoxStyle.OkOnly + MsgBoxStyle.Critical, "错误")
					Exit Sub
				End If
			Next
			Dx = Fun_Dx(StrToCalc, Dt)
			Sx = Fun_Sx(Dx)
			Gx = Fun_Gx(StrToCalc)
			Suma = 0
			Sumb = 0
			For i = 1 To NumVar
				Suma = Suma + (V(i) - TableVar(i).Value) * Dx(i)
				Sumb = Sumb + Sx(i) * C(i) * Dx(i)
			Next i
			If Sumb <> 0 Then
				BI = (Gx + Suma) / Sumb
			Else
				BI = 0
			End If
			ListBox_Rel.Items.Add("第" & Time & "次迭代")
			ListBox_Rel.Items.Add("可靠度指标为：Beta=" & BI)
			ListBox_Rel.SelectedItem = ListBox_Rel.Items.Item(ListBox_Rel.Items.Count - 1)
		Loop
		For i = 1 To NumVar
			Lmd_miu(i) = -(Dx(i) / (Math.Sqrt(2 * Math.PI) * Sumb)) * Math.Exp(-0.5 * (BI ^ 2))
			Lmd_sigma(i) = (Dx(i) ^ 2) * C(i) * (Gx + Suma) / (Math.Sqrt(2 * Math.PI) * Sumb ^ 3) * Math.Exp(-0.5 * (BI ^ 2))
			Design(i) = TableVar(i).Value
		Next
		Beta = B
		Pf = 1 - Get_Ps(Beta)
		ListBox_Rel.Items.Add("----------------------------------------------------------------")
		ListBox_Rel.Items.Add("一次二阶矩法对极限状态方程" & StrToCalc & "求解完毕")
		ListBox_Rel.Items.Add("本次求解迭代次数为：" & Time)
		ListBox_Rel.Items.Add("求解出的设计点为：")
		For j = 1 To NumVar
			ListBox_Rel.Items.Add(TableVar(j).Name & "：" & Design(j))
		Next
		ListBox_Rel.Items.Add("求解出的可靠度指标值为：")
		ListBox_Rel.Items.Add("Beta=" & Beta)
		ListBox_Rel.Items.Add("求解出的失效概率为：")
		ListBox_Rel.Items.Add("Pf=" & Pf)
		ListBox_Rel.Items.Add("相应的均值灵敏度分别为：")
		For j = 1 To NumVar
			ListBox_Rel.Items.Add(TableVar(j).Name & "：" & Lmd_miu(j))
		Next
		ListBox_Rel.Items.Add("相应的标准差灵敏度分别为：")
		For j = 1 To NumVar
			ListBox_Rel.Items.Add(TableVar(j).Name & "：" & Lmd_sigma(j))
		Next
		ListBox_Rel.Items.Add(Now)
		ListBox_Rel.SelectedItem = ListBox_Rel.Items.Item(ListBox_Rel.Items.Count - 1)
	Catch ex As Exception
		MsgBox(ex.Message & "ERROR IN Re_YCEJJ")
		Exit Sub
	End Try
End Sub
```

计算字符串表达式
```vb
Dim JiSu As New MSScriptControl.ScriptControl
JiSu.Language = "VbScript"
Fun_Gx = JiSu.Eval(StrToCalc)
```


Private Sub Fun_CFEM() 用于调用软件

My.Computer.FileSystem.WriteAllText(CalcPath & "\" & Path.GetFileName(TableStep(i).Input), RichTextBoxTemp.Text, False, Ascii)


在 Visual Basic (VB) 中，`Application.DoEvents` 是一个方法，用于处理当前应用程序的所有挂起的 Windows 消息和事件。这意味着，当你调用 `Application.DoEvents` 时，程序会暂停当前执行的代码片段，去处理任何等待处理的用户输入、重绘、定时器事件等，然后继续执行代码。

### 主要功能和用途：

- **UI 响应**：在长时间运行的循环或操作中，UI 可能会冻结，无法响应用户操作。调用 `Application.DoEvents` 可以暂时让系统处理用户的点击、窗口的重绘等，保持 UI 的响应性。
- **避免卡死**：在执行某些耗时任务时，比如大量计算、文件操作或网络请求，UI 会变得无响应。为了避免这种情况，可以在操作的某些位置调用 `Application.DoEvents`，让应用程序能够响应用户输入，而不会完全锁死。

```vb
For i As Integer = 1 To 10000
    ' 执行某些长时间的操作
    TextBox1.Text = i.ToString()

    ' 让程序在每次循环时处理挂起的事件
    Application.DoEvents()
Next
```

Debug模式下的依赖解决方法: 重新添加  DataVisualization.dll 文件, 实际的 dll 文件在 `\bin\x86\Release\#   System.Windows.Forms.DataVisualization.dll`, 添加依赖即可


## 重定向  Console.WriteLine 输出控制台

在 Visual Basic Windows 窗体应用程序中，如果你希望将控制台输出与窗体结合起来，并在窗体中展示类似 `Console.WriteLine` 的功能，可以通过以下两种方法来实现：

1. **在窗体中添加一个文本框控件**，模拟控制台输出。
2. **创建一个自定义的 `TextWriter`** 类，将 `WriteLine` 操作重定向到窗体的控件中（如 `TextBox` 或 `RichTextBox`）。

### 方法 1：在窗体中使用 `TextBox` 控件进行输出

你可以在窗体中放置一个 `TextBox` 或 `RichTextBox` 控件，用来显示 `WriteLine` 操作的内容。这里是如何实现的步骤：

#### 代码实现：

1. **在窗体中添加一个 `TextBox` 或 `RichTextBox`**：
   - 在 Visual Studio 的设计器中拖放一个 `TextBox` 或 `RichTextBox` 到你的窗体上，命名为 `txtConsole`（或其他名称）。
   - 设置 `TextBox` 的 `Multiline` 属性为 `True`，并将 `ScrollBars` 设置为 `Vertical`，以便支持多行文本和滚动条。

2. **创建一个方法来模拟 `WriteLine`**：
   使用 `TextBox.AppendText` 方法来追加内容。

#### 完整代码示例：

```vb
Public Class Form1
    ' 当窗体加载时，模拟 AllocConsole
    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        ' 模拟控制台输出
        WriteLine("窗体控制台初始化成功！")
        WriteLine("这是一个模拟的控制台输出。")
    End Sub

    ' 模拟 WriteLine 操作，将文本写入 TextBox
    Public Sub WriteLine(text As String)
        txtConsole.AppendText(text & Environment.NewLine)
    End Sub
End Class
```


## 自定义 `TextWriter` 重定向控制台输出到窗体控件
你可以创建一个自定义的 `TextWriter` 类，重定向 `Console.WriteLine` 的输出到窗体的 `TextBox` 或 `RichTextBox` 控件中。
#### 步骤：
1. **创建一个自定义的 `TextBoxWriter` 类**，继承自 `TextWriter`。
2. **在窗体的加载事件中**，将 `Console.SetOut` 设置为你的自定义 `TextWriter`。

#### 自定义 `TextWriter` 示例：

```vb
Imports System.IO
Imports System.Text

' 自定义的 TextWriter 类，将 Console 输出重定向到 TextBox
Public Class TextBoxWriter
    Inherits TextWriter

    Private txtBox As TextBox
    Public Sub New(ByVal output As TextBox)
        txtBox = output
    End Sub
		
    Public Overrides ReadOnly Property Encoding As Encoding
        Get
            Return Encoding.UTF8
        End Get
    End Property
	
    ' 重写 WriteLine 方法，将输出内容添加到 TextBox
    Public Overrides Sub WriteLine(value As String)
        txtBox.AppendText(value & Environment.NewLine)
    End Sub

    ' 重写 Write 方法，支持单行输出
    Public Overrides Sub Write(value As String)
        txtBox.AppendText(value)
    End Sub
End Class
```

#### 在窗体中使用：

```vb
Public Class Form1
    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        ' 将 Console 的输出重定向到 TextBox
        Dim writer As New TextBoxWriter(txtConsole)
        Console.SetOut(writer)

        ' 测试输出
        Console.WriteLine("这是通过 Console.WriteLine 输出的内容。")
        Console.WriteLine("重定向后的第二行。")
    End Sub
End Class
```

### 解释：
1. `TextBoxWriter` 类继承自 `TextWriter`，并重写 `Write` 和 `WriteLine` 方法，将输出内容添加到指定的 `TextBox` 中。
2. 在窗体加载时，使用 `Console.SetOut(new TextBoxWriter(txtConsole))` 将控制台输出重定向到 `TextBox`。
3. 现在，任何 `Console.WriteLine` 操作都会输出到窗体上的 `TextBox` 控件。

### 总结
- 如果你只需要简单的输出，可以直接使用 `TextBox.AppendText` 实现类似 `WriteLine` 的效果。
- 如果你希望将 `Console.WriteLine` 重定向到窗体，你可以通过自定义 `TextWriter` 类来实现。

如果你有其他问题或需要进一步的帮助，请随时告诉我！


```python
1-6分别为patran.exe、mdi.bat、mdnastranw.exe、abq691.bat、mentat.bat、ANSYS100.exe，在TreeView_Soft_AfterSelect和InitializeFEMPath中出现
```

```
'MessageBox.Show("请配置可靠性软件的计算路径", Application.ProductName, MessageBoxButtons.OK, MessageBoxIcon.Information)
MsgBox("CalcPath.ini文件不存在且计算路径设置失败， 程序即将退出!", vbCritical, Title:="致命错误")
```




`On Error Resume Next` 是 Visual Basic 中的一条语句，用于处理运行时错误。当这条语句被执行时，如果代码中发生错误，程序将继续执行下一条语句，而不会中断或显示错误消息。这在某些情况下可能会非常有用，但也需要谨慎使用，因为它可能会掩盖潜在的问题。



## 以对话形式创建窗口
在 Windows Forms 应用程序中，如果你希望在新建一个 `Form` 窗体的同时禁用主窗口（即让用户不能与主窗口交互，必须先处理新的窗体），可以使用 **模式对话框**（Modal Dialog）。在 Windows Forms 中，使用 `ShowDialog()` 方法显示一个窗体可以使其成为 **模式窗口**，直到该窗口被关闭之前，主窗口将无法与用户交互。

### 如何实现：
1. 当主窗口打开一个模式对话框（新窗体）时，主窗口会被禁用，用户只能与新窗体交互。
2. 在新窗体关闭后，主窗口将重新恢复可交互状态。

### 代码示例：

假设你有一个主窗口 `Form1`，点击某个按钮时打开新窗口 `Form2`，并且希望 `Form2` 关闭前主窗口不可操作。

#### `Form1`（主窗口）代码：

```vb
Public Class Form1
    Private Sub ButtonOpenForm2_Click(sender As Object, e As EventArgs) Handles ButtonOpenForm2.Click
        ' 创建一个新的 Form2 实例
        Dim form2 As New Form2()

        ' 以模式对话框方式显示 Form2，直到关闭它前，Form1 被禁用
        form2.ShowDialog()

        ' Form2 关闭后，Form1 会恢复正常交互
    End Sub
End Class
```

#### `Form2`（新窗口）代码：

```vb
Public Class Form2
    Private Sub ButtonOK_Click(sender As Object, e As EventArgs) Handles ButtonOK.Click
        ' 用户点击“确定”按钮时，关闭该窗口
        Me.Close()
    End Sub
End Class
```

### 关键点说明：
1. **`ShowDialog()`**：使用 `ShowDialog()` 方法显示 `Form2`，这是关键，它会将 `Form2` 作为模式对话框显示。模式对话框会阻止用户与其他窗口（如 `Form1`）的交互，直到 `Form2` 被关闭。
   
2. **主窗口禁用**：当 `Form2` 通过 `ShowDialog()` 显示时，`Form1` 被自动禁用。此时，点击主窗口不会有任何响应，用户必须先关闭 `Form2`。

3. **恢复交互**：当 `Form2` 关闭时（通过 `Me.Close()`），`Form1` 会恢复正常操作，用户可以重新与主窗口交互。

### 优点：
- 使用 `ShowDialog()` 自动处理了主窗口禁用和重新启用的逻辑，你不需要手动禁用或恢复主窗口。
- `ShowDialog()` 返回后可以获取用户在 `Form2` 中的输入或操作结果，如果需要，可以处理返回值。

### 调用示例：
用户点击主窗口的按钮时，`Form2` 以模式对话框显示。用户必须先完成 `Form2` 中的操作，然后点击确定按钮关闭 `Form2`，这时主窗口 `Form1` 才会恢复可用状态。


## TreeNode 说明

是的，`Tag` 属性可以存储任何类型的数据，包括整型。在 Visual Basic 中，`Tag` 属性的类型是 `Object`，因此你可以存储整型数据、字符串、对象等各种数据类型。存储整型数据时，稍后可以将其取出并直接转换为整型使用。

### 示例：
```vb
' 添加节点并将整型值存储到 Tag 中
Dim node As TreeNode
Dim i As Integer = 5
node = TreeView_Soft.Nodes.Add(key:="key" & i, text:=Form_SoftSet.Default_FEM_SoftWares(i))
node.Tag = i ' 将整型 i 存储到 Tag 中
```

### 取出并使用 `Tag` 作为整型：
当你从 `Tag` 中取出数据时，因为 `Tag` 是一个 `Object` 类型，可能需要进行类型转换（如果你明确知道 `Tag` 中存储的类型）。你可以使用 `CInt()` 来将 `Tag` 转换为整型。

### 示例代码（取出并使用整型 `Tag`）：

```vb
' 获取节点并从 Tag 中取出整数
Dim selectedNode As TreeNode = TreeView_Soft.SelectedNode
If selectedNode IsNot Nothing Then
    Dim value As Integer = CInt(selectedNode.Tag) ' 将 Tag 转换为 Integer
    MessageBox.Show("节点的 Tag 值为: " & value)
End If
```

### 说明：
- **`node.Tag = i`**：将整数 `i` 存储到 `Tag` 中。
- **`CInt(selectedNode.Tag)`**：从 `Tag` 中取出数据并将其转换为整数类型。
  
`Tag` 属性灵活地支持各种类型的数据存储，因此存储整型数据并在后续使用中取出和转换是完全可以的。



TempRead.Dispose()方法用于释放对象所占用的资源。在你的代码中，`TempRead` 是一个 `System.IO.StreamReader` 对象，当你调用 `TempRead.Dispose()` 时，它会释放 `StreamReader` 所占用的所有资源，例如文件句柄和内存。这是一个良好的编程习惯，可以防止资源泄漏。

以下是一个简化的解释：

- `TempRead.Close()` 关闭 `StreamReader`，使其不再读取文件。
- `TempRead.Dispose()` 释放 `StreamReader` 所占用的所有资源，确保它们可以被垃圾回收器回收。



```vb
' Create an instance of OpenFileDialog
Dim ofd As New OpenFileDialog()

' Set the title of the dialog
ofd.Title = "Select a file"

' Set the filter for file types
ofd.Filter = "Text Files (*.txt)|*.txt|" & _
             "Image Files (*.jpg; *.jpeg; *.png)|*.jpg; *.jpeg; *.png|" & _
             "Word Documents (*.doc; *.docx)|*.doc; *.docx|" & _
             "Excel Files (*.xls; *.xlsx)|*.xls; *.xlsx|" & _
             "All Files (*.*)|*.*"

' Set the default filter index (1-based, here 1 corresponds to Text Files)
ofd.FilterIndex = 1

' Optionally: Set the initial directory
ofd.InitialDirectory = "C:\"

' Show the dialog and get the result
If ofd.ShowDialog() = DialogResult.OK Then
    ' User selected a file
    MsgBox("You selected: " & ofd.FileName)
Else
    ' User canceled
    MsgBox("No file was selected.")
End If
```


Using 的使用类似于 Python 中的 With 效果
```vb
Imports System.IO

' 保存纯文本到文件
Dim filePath As String = "C:\YourFolder\output.txt"

Using writer As New StreamWriter(filePath)
    writer.Write(RichTextBox1.Text) ' 保存纯文本内容
End Using
```






在 VB.NET 中，使用 `Shell` 函数启动外部程序时，它默认是异步执行的，也就是说，程序会立即继续执行，而不会等待 `Shell` 启动的进程完成。如果你希望等待 `Shell` 命令执行完毕，可以通过以下方法实现。

### 使用 `Process` 类等待 Shell 命令执行完毕

你可以使用 `System.Diagnostics.Process` 类来启动外部进程，并等待它完成。示例如下：

```vb
Imports System.Diagnostics

Sub RunShellCommand()
    Dim proc As New Process()
    proc.StartInfo.FileName = "cmd.exe"        ' 要执行的命令
    proc.StartInfo.Arguments = "/C your_command" ' "/C" 表示执行完命令后关闭命令提示符
    proc.StartInfo.UseShellExecute = False     ' 使用操作系统 Shell 启动
    proc.StartInfo.RedirectStandardOutput = True ' 可选：如果想捕获输出
    proc.StartInfo.CreateNoWindow = True       ' 不显示窗口

    ' 启动进程
    proc.Start()

    ' 等待进程执行完毕
    proc.WaitForExit()

    ' 可选：获取命令输出
    Dim output As String = proc.StandardOutput.ReadToEnd()
    Console.WriteLine(output)

    ' 关闭进程
    proc.Close()
End Sub
```

### 解释：
- `proc.StartInfo.FileName` 是要执行的命令或程序（例如 `cmd.exe`）。
- `proc.StartInfo.Arguments` 是传递给命令的参数，例如你可以替换 `"your_command"` 为实际的命令。
- `proc.WaitForExit()` 用于等待该进程完成，它会阻塞当前线程，直到外部程序执行完毕。
- `proc.StartInfo.RedirectStandardOutput` 允许捕获外部命令的输出，可以选择性启用。

### 完整示例（等待 `ping` 命令执行完毕）：

```vb
Sub RunPingCommand()
    Dim proc As New Process()
    proc.StartInfo.FileName = "cmd.exe"
    proc.StartInfo.Arguments = "/C ping www.google.com"
    proc.StartInfo.UseShellExecute = False
    proc.StartInfo.RedirectStandardOutput = True
    proc.StartInfo.CreateNoWindow = True

    proc.Start()
    proc.WaitForExit()

    Dim output As String = proc.StandardOutput.ReadToEnd()
    Console.WriteLine(output)

    proc.Close()
End Sub
```

在这个示例中，`cmd.exe` 执行了 `ping` 命令，程序会等待 `ping` 完成后继续执行。

在 VB.NET 中，可以使用 `My.Computer.FileSystem.DeleteFile` 方法或 `System.IO.File.Delete` 方法来删除文件。以下是两种常用的方法。

### 1. 使用 `My.Computer.FileSystem.DeleteFile`
这个方法提供了一些选项，比如显示对话框、发送文件到回收站等。
```vb
My.Computer.FileSystem.DeleteFile("C:\path\to\your\file.txt")
```

#### 带选项的示例：
```vb
My.Computer.FileSystem.DeleteFile("C:\path\to\your\file.txt", _
    FileIO.UIOption.OnlyErrorDialogs, _
    FileIO.RecycleOption.DeletePermanently, _
    FileIO.UICancelOption.ThrowException)
```

- `FileIO.UIOption.OnlyErrorDialogs`：只显示错误对话框。
- `FileIO.RecycleOption.DeletePermanently`：永久删除文件，不发送到回收站。如果想把文件放到回收站，使用 `FileIO.RecycleOption.SendToRecycleBin`。
- `FileIO.UICancelOption.ThrowException`：如果用户取消操作，抛出异常。

### 2. 使用 `System.IO.File.Delete`

这是一个更简单的方法，直接删除文件，不提供用户界面选项，适合简单的删除操作。

```vb
System.IO.File.Delete("C:\path\to\your\file.txt")
```

### 错误处理

无论使用哪种方法，建议添加错误处理机制，以防文件不存在或其他异常情况：

```vb
Try
    System.IO.File.Delete("C:\path\to\your\file.txt")
    MessageBox.Show("文件已成功删除。")
Catch ex As Exception
    MessageBox.Show("删除文件时发生错误: " & ex.Message)
End Try
```

### 总结：
- 如果你需要更多选项（如确认对话框或回收站），使用 `My.Computer.FileSystem.DeleteFile`。
- 如果只需要简单删除文件，使用 `System.IO.File.Delete



## 运行 Shell, 同时将过程中的指令输出console
为了使用 PowerShell 并将所有的输出实时打印到控制台，你可以使用 `Process` 类并结合 `RedirectStandardOutput` 和 `RedirectStandardError` 实现这一功能。为了确保可以实时读取 PowerShell 的输出，而不是等到进程结束后再读取，需要使用 `OutputDataReceived` 和 `ErrorDataReceived` 事件，这样你可以逐行捕获输出。

以下是一个完整的示例，展示如何调用 `powershell.exe` 并将 PowerShell 输出实时显示到 `Console` 中：
### 示例代码：

```vb
Imports System.Diagnostics

Sub RunPowerShellCommand()
    ' 定义要运行的 PowerShell 命令
    Dim powerShellCommand As String = "Get-Process" ' 这里可以替换为你需要的 PowerShell 命令
    Dim proc As New Process()

    ' 设置 ProcessStartInfo
    proc.StartInfo.FileName = "powershell.exe"
    proc.StartInfo.Arguments = "-NoProfile -ExecutionPolicy Bypass -Command " & powerShellCommand
    proc.StartInfo.UseShellExecute = False
    proc.StartInfo.RedirectStandardOutput = True   ' 重定向标准输出
    proc.StartInfo.RedirectStandardError = True    ' 重定向标准错误输出
    proc.StartInfo.CreateNoWindow = True           ' 不创建窗口

    ' 启动进程
    proc.Start()

    ' 开始异步读取输出流
    AddHandler proc.OutputDataReceived, AddressOf OutputHandler
    AddHandler proc.ErrorDataReceived, AddressOf OutputHandler

    ' 开始捕获输出和错误流
    proc.BeginOutputReadLine()
    proc.BeginErrorReadLine()

    ' 等待进程结束
    proc.WaitForExit()

    ' 确保关闭进程
    proc.Close()
End Sub

' 处理输出的回调函数
Private Sub OutputHandler(ByVal sendingProcess As Object, ByVal outLine As DataReceivedEventArgs)
    ' 如果输出不为空，则打印到控制台
    If Not String.IsNullOrEmpty(outLine.Data) Then
        Console.WriteLine(outLine.Data)
    End If
End Sub
```

### 解释：
1. **`proc.StartInfo.FileName = "powershell.exe"`**：指定要执行的程序为 `powershell.exe`。
2. **`proc.StartInfo.Arguments`**：传递要执行的 PowerShell 命令。这里使用 `-Command` 参数执行 PowerShell 指令。你可以根据需要替换 `Get-Process` 为其他命令。
3. **`RedirectStandardOutput` 和 `RedirectStandardError`**：启用标准输出和标准错误的重定向。
4. **`OutputDataReceived` 和 `ErrorDataReceived`**：通过事件处理器异步读取输出和错误信息，并逐行输出到控制台。
5. **`proc.BeginOutputReadLine()` 和 `proc.BeginErrorReadLine()`**：开始异步读取输出和错误流。

### 逐行输出的优势：
使用 `OutputDataReceived` 和 `ErrorDataReceived` 事件，你可以逐行捕获输出，而不是等待整个进程完成后再一次性读取。这样可以确保你实时看到 PowerShell 执行的输出，避免等待直到所有输出都生成。

### 调用示例：

```vb
RunPowerShellCommand()
```

这个代码片段会运行 PowerShell 命令并将输出逐行打印到控制台。

`````ad-note
title: Shell 指令参数
collapse: open

在 PowerShell 中，`-NoProfile`、`-ExecutionPolicy Bypass` 和 `-Command` 参数用于控制 PowerShell 执行时的行为。让我们逐一解释这些参数的含义：

### 1. `-NoProfile`
- **作用**：告诉 PowerShell 在启动时**不要加载用户或系统的配置文件**（通常是 `profile.ps1` 文件）。
- **目的**：加快启动速度并防止用户或系统配置文件中的脚本影响当前的 PowerShell 执行。这对安全性或调试有帮助，因为你可以确保 PowerShell 运行时不会被外部脚本所修改。
  
  **示例**：
  ```bash
  powershell.exe -NoProfile
  ```
  这意味着 PowerShell 将不会加载用户和系统的 `profile.ps1` 文件。

### 2. `-ExecutionPolicy Bypass`
- **作用**：设置 PowerShell 的执行策略为 `Bypass`，允许执行所有脚本而不受执行策略的限制。
- **目的**：PowerShell 的执行策略用于控制脚本的执行，默认情况下，某些策略可能阻止未签名或不受信任的脚本运行。通过设置为 `Bypass`，你可以临时禁用这些限制，允许脚本正常运行。

  **常见的执行策略**：
  - `Restricted`：默认策略，禁止运行任何脚本。
  - `RemoteSigned`：本地脚本可以运行，远程下载的脚本需要签名。
  - `Unrestricted`：允许运行所有脚本，但远程下载的脚本会提示确认。
  - `Bypass`：完全绕过执行策略，不做任何限制。

  **示例**：
  ```bash
  powershell.exe -ExecutionPolicy Bypass
  ```
  这将忽略系统上的执行策略设置，允许当前脚本执行，而不会产生执行策略错误。

### 3. `-Command`
- **作用**：指定要执行的 PowerShell 命令或脚本。紧跟在此参数后面的是实际要执行的命令。
- **目的**：使用 `-Command` 参数，PowerShell 会直接运行提供的命令并立即退出。你可以传递任何合法的 PowerShell 命令或脚本。

  **示例**：
  ```bash
  powershell.exe -Command "Get-Process"
  ```
  这将运行 `Get-Process` 命令，列出当前系统中的所有进程。

### 综合示例：
```bash
powershell.exe -NoProfile -ExecutionPolicy Bypass -Command "Get-Process"
```
- `-NoProfile`：不加载用户或系统配置文件。
- `-ExecutionPolicy Bypass`：绕过执行策略，允许执行任何脚本或命令。
- `-Command "Get-Process"`：直接运行 `Get-Process` 命令，列出当前的所有进程。

### 为什么使用这些参数？
1. **-NoProfile**：避免加载用户配置文件可能带来的影响，确保脚本运行时环境更加干净和快速。
2. **-ExecutionPolicy Bypass**：确保脚本不受执行策略限制，通常用于需要临时绕过安全策略的场景。
3. **-Command**：允许直接传递并执行命令或脚本，方便在单个命令行执行一系列操作。

这些参数结合在一起，常用于自动化任务、脚本执行以及确保运行环境不会受到外部因素的干扰。
`````





你遇到的 `System.InvalidOperationException` 错误是由于跨线程操作引起的。具体来说，`ConsoleOutputBox` 控件是由主线程创建的，而异步进程（或其他线程）试图从另一个线程访问和修改该控件，这是不被允许的。Windows Forms 控件只能从创建它们的线程进行访问。

### 解决方案：
你需要使用主线程的 `Invoke` 方法将跨线程调用调度回主线程。这样可以安全地从异步线程访问 UI 控件，如 `ConsoleOutputBox`。

### 如何修正：
在输出捕获的回调方法中，通过 `Invoke` 或 `BeginInvoke` 将数据发送回主线程。以下是修正后的代码：

```vb
' 这是异步输出处理程序，不能直接访问 UI 控件
Private Sub OutputHandler(sender As Object, e As DataReceivedEventArgs)
    If Not String.IsNullOrEmpty(e.Data) Then
        ' 通过 Invoke 调用主线程
        ConsoleOutputBox.Invoke(New Action(Sub()
                                               ConsoleOutputBox.AppendText(e.Data & Environment.NewLine)
                                           End Sub))
    End If
End Sub

' 异步错误处理程序，类似地处理错误输出
Private Sub ErrorHandler(sender As Object, e As DataReceivedEventArgs)
    If Not String.IsNullOrEmpty(e.Data) Then
        ' 通过 Invoke 调用主线程
        ConsoleOutputBox.Invoke(New Action(Sub()
                                               ConsoleOutputBox.AppendText("Error: " & e.Data & Environment.NewLine)
                                           End Sub))
    End If
End Sub
```


### 关键点：
1. **`Invoke` 或 `BeginInvoke`**：将跨线程操作调度回 UI 线程。`Invoke` 是同步的，而 `BeginInvoke` 是异步的，视情况可以选择其中之一。
   
2. **线程安全的 UI 更新**：由于 `ConsoleOutputBox` 是由 UI 线程创建的控件，任何对其属性的修改都需要通过 UI 线程进行。

### 总结：
通过 `Invoke`，你可以安全地从异步线程或进程更新 `ConsoleOutputBox` 的内容，避免 `InvalidOperationException` 异常。


## Shared 关键字的使用
在 VB.NET 中，`Shared` 关键字用于声明类或结构中的共享成员，也就是不需要通过对象实例就可以访问的成员。了解它的工作机制以及为何在某些情况下需要使用 `Shared` 才能使结构体变量赋值，需从结构体与类的区别入手。

### 1. `Shared` 关键字的基本概念：
- **共享成员**：`Shared` 关键字可以用来修饰类或结构的字段、方法、属性、事件等。被修饰为 `Shared` 的成员是全局性的，与类或结构的实例无关。也就是说，不需要实例化该类或结构，就可以通过类名直接访问它。
  
  **非共享成员**则是与特定的实例关联的，每个实例都有自己的成员副本。

```vb
Class MyClass
    Public Shared Count As Integer = 0 ' 共享成员
    Public Name As String = "Instance Name" ' 实例成员
End Class
```
在上例中，`Count` 是一个 `Shared` 成员，可以通过 `MyClass.Count` 来访问，而 `Name` 则是实例成员，需要通过具体的实例如 `Dim obj As New MyClass` 来访问。

### 2. `Shared` 在结构体中的作用：
- **结构体（Structure）**：与类不同，结构体是值类型，而类是引用类型。值类型在使用时会直接包含数据，而引用类型存储的是指向对象的引用。

  结构体的一个特殊之处是，它的实例在声明时会默认初始化为其默认值（一般是0或空），这意味着结构体的字段自动获得默认值，而类的引用类型成员在实例化之前是 `Nothing`。

```vb
Structure MyStruct
    Public Shared MyValue As Integer = 10
End Structure
```

### 3. 为什么结构体中需要使用 `Shared` 来赋值：
- **结构体是值类型**：因为结构体是值类型，当你声明一个结构体实例时，它的非 `Shared` 成员会自动被初始化为默认值。因此，如果你不显式声明 `Shared`，成员会在每次结构体实例化时被初始化为默认值，无法保证这些成员可以跨实例共享状态。

- **`Shared` 允许结构体中的成员不依赖实例**：`Shared` 成员是与类型本身绑定的，不与具体的实例绑定。因此，即便结构体没有被实例化，`Shared` 成员仍然存在，可以被访问和赋值。使用 `Shared` 的场景更适用于需要全局共享的字段或方法，这样可以避免每个实例都维护自己的独立副本。

```vb
Structure MyStruct
    Public Shared MyValue As Integer
End Structure

MyStruct.MyValue = 100 ' 通过类名访问，不需要实例化结构体
```

### 4. 为什么在结构体变量赋值时需要使用 `Shared`？
结构体的成员在赋值时，如果不使用 `Shared`，需要通过实例化该结构体后访问该成员。而 `Shared` 成员则与具体的结构体实例无关，赋值时不需要实例化结构体。如下所示：

```vb
Structure MyStruct
    Public Value As Integer
    Public Shared SharedValue As Integer
End Structure

' 无法直接赋值非Shared字段
MyStruct.Value = 100 ' 错误，Value 是实例成员，需要实例化结构体

' 但可以赋值Shared字段
MyStruct.SharedValue = 100 ' 正确，SharedValue 是共享成员，无需实例化
```

综上，**结构体变量在使用时，`Shared` 关键字允许它的字段不依赖于具体的实例，可以直接通过结构体类型来访问和赋值。**这使得在某些需要全局共享的情况下必须使用 `Shared`，否则会由于值类型的限制而无法正确赋值或访问。

### 总结：
1. `Shared` 成员属于类型本身，而不是某个实例。
2. 结构体是值类型，默认在声明时初始化成员为默认值。
3. `Shared` 在结构体中确保字段可以独立于结构体实例存在和访问，允许全局共享的字段赋值操作。


## 反射(Reflection) 遍历结构体各个成员
在 VB.NET 中，可以通过反射（Reflection）遍历对象的所有字段或属性，从而动态访问它们。这可以让你避免显式列出每个成员，比如 `{run_path.input_temp_path, run_path.output_path, run_path.log_path}`，并自动遍历 `Running_Path` 中的路径字段。

### 示例代码

假设你的 `Running_Path` 是一个类或结构体，并且它有一些公共的字符串字段代表路径，我们可以通过反射来遍历这些字段。

以下是使用反射的方法，遍历 `Running_Path` 的所有字符串属性或字段：

```vb
Imports System.Reflection

Public Sub Init_Running_path()
    run_path.input_temp_path = Form_Main.CalcPath & "/input_template"
    run_path.output_path = Form_Main.CalcPath & "/output"
    run_path.log_path = Form_Main.CalcPath & "/logs"

    ' 遍历 run_path 的所有字段
    Dim fields = GetType(Running_Path).GetFields(BindingFlags.Public Or BindingFlags.Instance)

    For Each field As FieldInfo In fields
        ' 检查字段是否是字符串类型
        If field.FieldType Is GetType(String) Then
            Dim path As String = CType(field.GetValue(run_path), String)
            
            ' 检查并创建目录
            If Not Directory.Exists(path) Then
                Directory.CreateDirectory(path)
            End If
        End If
    Next
End Sub
```

### 解释：
1. `GetType(Running_Path).GetFields()`：通过反射获取 `Running_Path` 中所有公共实例字段。
2. `If field.FieldType Is GetType(String)`：确保我们只处理字符串类型的字段（即路径）。
3. `field.GetValue(run_path)`：使用反射获取字段的值。
4. `Directory.Exists` 和 `Directory.CreateDirectory`：依然检查和创建目录。

这样做的好处是，如果你以后在 `Running_Path` 中增加了新的路径字段，这段代码也不需要更改，会自动处理所有的路径字段。

System.IO.Directory.GetFiles 列出所有路径下文件 
```vb
'删除 run_path.input_temp_path 下所有文件
For Each File In System.IO.Directory.GetFiles(calc_path & "\run_path\input_temp_path")
	System.IO.File.Delete(File)
Next
```


## If 三元运算符
`Dim fileName As String = If(fem_soft = "Adams", "aview.cmd", Path.GetFileName(table_step(i).Input))` 是一行使用 `If` 函数的 VB.NET 代码，用于简化条件分支操作。这行代码的目的是根据 `fem_soft` 的值来决定 `fileName` 的取值。

### 解释：

1. **`If` 函数**：
   - `If` 函数在 VB.NET 中是一种三元操作符，用于简化条件语句的写法。它类似于 C# 中的三元运算符 `condition ? trueValue : falseValue`。
   - 语法格式是：`If(condition, trueResult, falseResult)`。如果 `condition` 为 `True`，返回 `trueResult`，否则返回 `falseResult`。

2. **`fem_soft = "Adams"`**：
   - 这是一个条件判断，检查变量 `fem_soft` 是否等于 `"Adams"`。
   - 如果 `fem_soft` 的值是 `"Adams"`，则条件为 `True`，执行返回 `"aview.cmd"`。
   - 如果 `fem_soft` 不是 `"Adams"`，则条件为 `False`，执行返回 `Path.GetFileName(table_step(i).Input)`。

3. **`"aview.cmd"`**：
   - 当 `fem_soft` 是 `"Adams"` 时，文件名为 `"aview.cmd"`。这表示在 `Adams` 软件情况下，生成的文件名是固定的，即 `aview.cmd`。

4. **`Path.GetFileName(table_step(i).Input)`**：
   - 当 `fem_soft` 不是 `"Adams"` 时，返回的文件名为 `table_step(i).Input` 的文件名部分。`Path.GetFileName` 函数用于提取文件路径中的文件名部分。

### 举例说明：

假设 `table_step(i).Input` 的值是 `"C:\example\input_file.txt"`：

- **当 `fem_soft = "Adams"`**：
  ```vb
  fileName = "aview.cmd"
  ```

- **当 `fem_soft` 不是 `"Adams"`（例如 `"Ansys"`）**：
  ```vb
  fileName = "input_file.txt" ' 提取路径中的文件名部分
  ```

### 小结：
- 这行代码的作用是根据 `fem_soft` 的值来动态决定文件名。如果 `fem_soft` 是 `"Adams"`，则文件名是固定的 `"aview.cmd"`，否则文件名是 `table_step(i).Input` 的文件名部分。
- 使用 `If` 函数使得代码更加简洁，避免了使用冗长的 `If...Else` 结构。


![[attachments/Pasted image 20241012192846.png]]