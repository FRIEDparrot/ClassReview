在VB.NET中，没有像C语言那样的独立“函数原型声明”部分，因为VB.NET的函数声明（包括Sub和Function）都是在代码定义时同时声明和实现的。不过，你可以通过以下几种方式实现类似C语言函数原型的功能：

### 1. **接口（Interface）**
   接口定义了一组方法的签名，而不提供实现，这类似于C语言中的函数原型。你可以在一个接口中声明方法，然后在类中实现这些方法。

   **示例：**
   ```vb.net
   ' 定义一个接口
   Public Interface IMyInterface
       Sub MySub(param As Integer)
       Function MyFunction(param As Integer) As String
   End Interface

   ' 类实现接口中的方法
   Public Class MyClass
       Implements IMyInterface

       Public Sub MySub(param As Integer) Implements IMyInterface.MySub
           ' 方法的具体实现
           Console.WriteLine("Parameter value: " & param)
       End Sub

       Public Function MyFunction(param As Integer) As String Implements IMyInterface.MyFunction
           Return "Value is " & param.ToString()
       End Function
   End Class
   ```

   这里的`IMyInterface`相当于C语言中的函数原型部分，它描述了需要实现的函数签名。

### 2. **抽象类（MustInherit Class）**
   如果你希望提供部分实现，而只声明某些方法的签名，可以使用`MustInherit`类（相当于C++中的抽象类）。在抽象类中可以定义必须由派生类实现的抽象方法。

   **示例：**
   ```vb.net
   ' 定义一个抽象类
   Public MustInherit Class MyAbstractClass
       ' 抽象方法，只定义签名，不提供实现
       Public MustOverride Sub MySub(param As Integer)

       ' 普通方法，可以在这里提供部分实现
       Public Function MyFunction(param As Integer) As String
           Return "Value is " & param.ToString()
       End Function
   End Class

   ' 子类继承抽象类并实现抽象方法
   Public Class MyClass
       Inherits MyAbstractClass

       Public Overrides Sub MySub(param As Integer)
           Console.WriteLine("Parameter value: " & param)
       End Sub
   End Class
   ```

   在这里，`MustOverride`方法类似于函数原型，要求子类提供具体的实现。

### 3. **外部函数声明（Declare）**
   如果你需要调用Windows API或其他外部库中的函数，可以使用`Declare`语句，这在某种程度上类似于C语言中的函数原型声明。

   **示例：**
   ```vb.net
   ' 声明一个外部函数
   Declare Function MessageBox Lib "user32" Alias "MessageBoxA" (
       ByVal hWnd As IntPtr,
       ByVal lpText As String,
       ByVal lpCaption As String,
       ByVal uType As UInteger
   ) As Integer
   ```

   这里，`Declare`语句声明了`MessageBox`函数，这类似于C语言中的函数原型，但这个主要用于调用外部库的函数。

### 总结
在VB.NET中，接口和抽象类更常用于描述类的行为和方法签名，这类似于C语言中的函数原型声明。根据你的需求，可以选择使用接口、抽象类或者`Declare`语句。

## 采用 Dictionary 实现多不同参数调用函数

```vb
' 定义委托类型，与不同的分布处理函数签名相匹配
Delegate Sub DistributionHandler(ByVal NumMC As Integer, ByVal param1 As Double, ByVal param2 As Double, ByRef r() As Double)

' 定义字典，将分布类型映射到相应的处理函数
Dim DistributionMap As New Dictionary(Of String, DistributionHandler) From {
    {"正态分布", AddressOf n120},
    {"对数分布", AddressOf n130},
    {"对数正态分布", AddressOf n140},
    {"极值III型Weibull分布", AddressOf n150},
    {"极值I型Gumbel分布", AddressOf n160},
    {"均匀分布", AddressOf n170},
    {"指数分布", AddressOf n180},
    {"伽玛分布", AddressOf n190}
}

' 修改后的 R_QT 方法
Public Sub R_QT(ByVal NumMC As Integer, ByRef SamplingData(,) As Double)
    Try
        Dim XXZZ(NumMC) As Double
        Dim NumVar As Integer
        NumVar = TableVar.Length - 1

        For i = 1 To NumVar
            ' 获取当前变量的分布类型
            Dim distributionType As String = TableVar(i).Type

            ' 检查字典中是否存在对应的处理函数
            If DistributionMap.ContainsKey(distributionType) Then
                ' 根据不同的分布类型，调用对应的函数
                Dim handler As DistributionHandler = DistributionMap(distributionType)

                ' 根据分布类型调用相应的处理函数
                Select Case distributionType
                    Case "正态分布", "对数分布", "对数正态分布", "极值I型Gumbel分布"
                        handler(NumMC, TableVar(i).Loc, TableVar(i).Scale, XXZZ)
                    Case "极值III型Weibull分布"
                        handler(NumMC, TableVar(i).Sharp, TableVar(i).Scale, XXZZ)
                    Case "均匀分布"
                        handler(NumMC, TableVar(i).Loc, TableVar(i).Loc + TableVar(i).Scale, XXZZ)
                    Case "指数分布", "伽玛分布"
                        handler(NumMC, TableVar(i).Sharp, XXZZ)
                End Select
            End If

            ' 填充结果到 SamplingData
            For j = 1 To NumMC
                SamplingData(i, j) = XXZZ(j)
            Next j
        Next
    Catch ex As Exception
        MsgBox(ex.Message & "ERROR IN R_QT")
        Exit Sub
    End Try
End Sub
```

## 将类以序列形式写入文件
是的，在 Visual Basic 中，可以通过使用 `XmlSerializer` 类将一个类序列化为 XML 文件，然后再从 XML 文件中读取并反序列化为另一个类的对象。`XmlSerializer` 允许将类的所有属性保存为 XML 格式，并且可以方便地将它们重新读取到对象中。

### 示例：将类对象写入 XML 文件并读取
以下是一个完整的示例，展示如何将一个类的对象序列化到 XML 文件中，然后从该文件中读取回对象。我们会定义一个简单的类，并演示如何进行序列化和反序列化。

#### 步骤 1: 定义要序列化的类
```vb
Imports System.Xml.Serialization
Imports System.IO

' Define the class that will be serialized and deserialized
Public Class ProcessStepInfo
    Public Property StepName As String
    Public Property Description As String
    Public Property ParameterA As Double
    Public Property ParameterB As Integer

    ' Parameterless constructor is required for XmlSerializer
    Public Sub New()
    End Sub

    ' Constructor for convenience
    Public Sub New(stepName As String, description As String, parameterA As Double, parameterB As Integer)
        Me.StepName = stepName
        Me.Description = description
        Me.ParameterA = parameterA
        Me.ParameterB = parameterB
    End Sub
End Class
```

#### 步骤 2: 序列化类对象到 XML 文件
```vb
Public Sub WriteToXmlFile(filePath As String, stepInfo As ProcessStepInfo)
    Dim serializer As New XmlSerializer(GetType(ProcessStepInfo))
    Using writer As New StreamWriter(filePath)
        serializer.Serialize(writer, stepInfo)
    End Using
End Sub
```
这个方法接受一个文件路径和一个 `ProcessStepInfo` 对象，将对象序列化并保存到指定的 XML 文件中。

#### 步骤 3: 从 XML 文件读取类对象
```vb
Public Function ReadFromXmlFile(filePath As String) As ProcessStepInfo
    Dim serializer As New XmlSerializer(GetType(ProcessStepInfo))
    Using reader As New StreamReader(filePath)
        Return DirectCast(serializer.Deserialize(reader), ProcessStepInfo)
    End Using
End Function
```
这个方法接受一个文件路径，从 XML 文件中读取数据并反序列化为 `ProcessStepInfo` 对象。

#### 使用示例
```vb
Module Program
    Sub Main()
        ' Create an instance of ProcessStepInfo
        Dim originalStepInfo As New ProcessStepInfo("Step A", "Description for Step A", 3.14, 42)

        ' Specify the file path for the XML file
        Dim filePath As String = "stepInfo.xml"

        ' Write the object to an XML file
        WriteToXmlFile(filePath, originalStepInfo)
        Console.WriteLine("Object has been serialized to XML.")

        ' Read the object back from the XML file
        Dim deserializedStepInfo As ProcessStepInfo = ReadFromXmlFile(filePath)
        Console.WriteLine("Object has been deserialized from XML.")

        ' Display the deserialized object properties
        Console.WriteLine("Step Name: " & deserializedStepInfo.StepName)
        Console.WriteLine("Description: " & deserializedStepInfo.Description)
        Console.WriteLine("Parameter A: " & deserializedStepInfo.ParameterA)
        Console.WriteLine("Parameter B: " & deserializedStepInfo.ParameterB)
    End Sub
End Module
```

### 说明
1. **定义类**：`ProcessStepInfo` 是我们需要保存到 XML 的类。它有属性 `StepName`、`Description`、`ParameterA` 和 `ParameterB`。注意，类中需要有一个无参数的构造函数，这是 `XmlSerializer` 所要求的。
2. **序列化**：`WriteToXmlFile` 方法使用 `XmlSerializer` 将 `ProcessStepInfo` 对象写入 XML 文件。使用 `StreamWriter` 将 XML 数据写入到文件。
3. **反序列化**：`ReadFromXmlFile` 方法使用 `XmlSerializer` 从 XML 文件中读取数据，并将其反序列化为 `ProcessStepInfo` 对象。
4. **使用示例**：在 `Main` 方法中，创建一个 `ProcessStepInfo` 对象，将其序列化到 `stepInfo.xml` 文件，然后再读取该文件并重新创建对象。最后，打印出反序列化后的对象的属性以验证数据。

### 输出示例
如果运行这个程序，控制台可能会输出以下内容，表示对象的序列化和反序列化成功：
```
Object has been serialized to XML.
Object has been deserialized from XML.
Step Name: Step A
Description: Description for Step A
Parameter A: 3.14
Parameter B: 42
```

### 注意事项
- 在进行序列化时，如果类中有引用类型（如列表、集合等），`XmlSerializer` 也可以处理这些类型，只要它们是可序列化的。
- 确保 XML 文件的路径是可写的，并且程序有足够的权限进行文件操作。
- `XmlSerializer` 不支持一些更复杂的类型（如自定义泛型或接口类型），这些情况下需要考虑自定义序列化。

这个方法可以轻松地将类中的所有信息保存到 XML 文件中，并从 XML 中读取回来，方便数据的持久化和传递。

### 注意事项
- 如果有属性不需要序列化，可以通过使用 `<XmlIgnore>` 属性来忽略它们。例如：
    `<XmlIgnore> Public Property SomeNonSerializableProperty As String`
- 另外，`XmlSerializer` 只能序列化公共属性，因此，私有字段或属性不会被序列化到 XML 中。


## 带有继承的反序列化方法
在继承关系中，如果子类继承了基类的方法，那么在基类的方法中直接使用 `Me.GetType()` 作为类型来反序列化时，`XmlSerializer` 会根据实际的子类类型进行反序列化。所以，即使子类不重写这个方法，序列化和反序列化也可以处理子类的属性。

### 基类方法的适用性
1. **`Me.GetType()` 的作用**：
   - 在基类中使用 `Me.GetType()` 会获取当前实例的实际类型。如果 `from_xml` 方法是在子类的实例上调用的，`Me.GetType()` 返回的就是子类的类型。
   - 因此，`XmlSerializer` 会知道序列化和反序列化时应该处理子类中的额外属性。

2. **反序列化对象的类型**：
   - 当反序列化时，`XmlSerializer` 会使用 XML 中的信息创建一个与当前实例类型匹配的对象。所以，当调用基类的 `from_xml` 方法时，它能正确地反序列化子类类型的数据。

3. **属性复制**：
   - 在 `For Each` 循环中，我们获取的是 `deserializedObj` 的属性列表。由于 `deserializedObj` 是使用 `Me.GetType()` 反序列化得到的，它会包含子类的属性。
   - 使用反射逐一复制属性时，也会复制子类的属性值到当前实例的属性中。

### 是否需要重写
通常情况下，基类中的 `from_xml` 方法就足够处理子类的反序列化了，不需要子类再去重写这个方法，除非：

- 子类中有特殊的反序列化逻辑，需要额外处理某些属性或字段。
- 子类需要在反序列化完成后执行一些额外操作（例如，更新某些依赖字段或调用特定的初始化方法）。

如果这些情况不适用，那么基类的 `from_xml` 方法可以直接在子类中使用。

### 示例：基类方法在子类中的使用
假设有以下类结构：

```vb
' Base class
Public Class ProcessInfoBase
    Public Property StepName As String
    Public Property Description As String

    ' Encoding property
    Public Property encoding As System.Text.Encoding = System.Text.Encoding.UTF8

    ' Method to read object from XML
    Public Function from_xml(ByVal file_path As String, Optional encode As System.Text.Encoding = Nothing) As Boolean
        ' 使用传入的编码参数，如果为 Nothing 则使用默认编码
        If encode Is Nothing Then
            encode = Me.encoding
        End If

        Dim serializer As New XmlSerializer(Me.GetType)

        Try
            ' 使用指定的编码读取 XML 文件
            Using reader As New StreamReader(file_path, encoding:=encode)
                ' 反序列化 XML 文件到一个新的对象
                Dim deserializedObj = serializer.Deserialize(reader)

                ' 将反序列化对象的属性逐一赋值给当前实例
                For Each prop In deserializedObj.GetType().GetProperties()
                    Dim value = prop.GetValue(deserializedObj)
                    prop.SetValue(Me, value)
                Next
            End Using

            Return False
        Catch ex As Exception
            Console.WriteLine("[Error] Read Xml file " & file_path & " failed: " & ex.Message)
            Return True
        End Try
    End Function
End Class

' Derived class
Public Class StepAInfo
    Inherits ProcessInfoBase

    Public Property ParameterA As Double
    Public Property ParameterB As Integer
End Class
```

### 使用示例
当在 `StepAInfo` 实例上调用 `from_xml` 方法时：

```vb
Dim stepA As New StepAInfo()
Dim filePath As String = "stepAInfo.xml"
Dim success As Boolean = stepA.from_xml(filePath)

If Not success Then
    Console.WriteLine("Object successfully deserialized from XML.")
    Console.WriteLine("StepName: " & stepA.StepName)
    Console.WriteLine("Description: " & stepA.Description)
    Console.WriteLine("ParameterA: " & stepA.ParameterA)
    Console.WriteLine("ParameterB: " & stepA.ParameterB)
Else
    Console.WriteLine("Failed to deserialize the object from XML.")
End If
```

- `from_xml` 方法会根据 `Me.GetType()` 自动检测 `StepAInfo` 类型，并创建一个 `StepAInfo` 对象。
- 反序列化后，通过反射复制属性时，会包括 `ParameterA` 和 `ParameterB` 这些子类特有的属性。

### 重写示例（可选）
如果你确实有特殊需求，可以在子类中重写 `from_xml` 方法，然后调用基类的 `from_xml` 方法，再执行额外的逻辑：

```vb
' In StepAInfo class
Public Overrides Function from_xml(ByVal file_path As String,
                                   Optional encode As System.Text.Encoding = Nothing) As Boolean
    ' 调用基类的方法
    Dim result As Boolean = MyBase.from_xml(file_path, encode)

    ' 在这里进行额外的处理，例如重新计算依赖字段
    If Not result Then
        ' 例如，重新初始化一些计算字段
        ' InitializeCalculatedValues()
    End If

    Return result
End Function
```

这种情况下，基类已经处理了大部分反序列化逻辑，子类只需要在反序列化后做一些额外处理。

### 总结
- 基类的 `from_xml` 方法已经足够处理子类的反序列化，除非有特殊需求，否则不需要重写。
- `Me.GetType()` 让 `XmlSerializer` 能正确识别和处理子类类型。
- 子类重写的主要原因是为了添加额外的反序列化后处理逻辑。

## 更新 DataGridView 标题
要在 ComboBox 的选择发生变化时，根据新的需求来更新 `DataGridView` 控件的列标题和内容，可以按照以下步骤操作：
1. **更新列标题**：在 `ComboBox` 的 `SelectedIndexChanged` 事件中，检查当前选择，然后根据选择来更新 `DataGridView` 的列标题。
2. **更新列内容**：根据新选项，更新表格中每一行的数据。

下面是一个示例，演示如何在 `ComboBox` 更新选择时，动态调整 `DataGridView` 的列标题，并根据需求更新内容：

### 示例代码：

```vb
Private Sub ComboBox_Res_SelectedIndexChanged(sender As Object, e As EventArgs) Handles ComboBox_Res.SelectedIndexChanged
    ' 检查当前 ComboBox 的选择
    Select Case ComboBox_Res.SelectedItem.ToString()
        Case "位置映射"
            ' 更新 DataGridView 列标题为 "长度", "数值"
            DataGridView_Res.Columns(0).HeaderText = "长度"
            DataGridView_Res.Columns(1).HeaderText = "数值"
            
            ' 更新 DataGridView 内容（假设有一个包含 MapInfo 结构体的列表 mapList）
            For Each map As MapInfo In mapList
                If map.select_method = 0 Then
                    ' 假设 DataGridView_Res 里有 2 列，第 1 列显示长度，第 2 列显示数值
                    DataGridView_Res.Rows.Add(map.Length, map.Text)
                End If
            Next

        Case "单词映射"
            ' 更新 DataGridView 列标题为 "行号", "单词数"
            DataGridView_Res.Columns(0).HeaderText = "行号"
            DataGridView_Res.Columns(1).HeaderText = "单词数"
            
            ' 更新 DataGridView 内容（假设有一个包含 MapInfo 结构体的列表 mapList）
            For Each map As MapInfo In mapList
                If map.select_method = 1 Then
                    ' 假设 DataGridView_Res 里有 2 列，第 1 列显示行号，第 2 列显示单词数
                    DataGridView_Res.Rows.Add(map.LineNumber, map.WordIndex)
                End If
            Next

    End Select
End Sub
```

### 解释：

1. **事件绑定**：将 `ComboBox` 的 `SelectedIndexChanged` 事件绑定到方法 `ComboBox_Res_SelectedIndexChanged`。当 `ComboBox` 的选择发生变化时，触发此方法。
2. **更新列标题**：
   - 使用 `DataGridView_Res.Columns(index).HeaderText` 来修改列标题。
   - 根据 `ComboBox` 的选择，分别设置为 "长度, 数值" 或 "行号, 单词数"。
3. **更新表格内容**：
   - 假设存在一个 `mapList` 列表，保存了所有的 `MapInfo` 结构体。
   - 使用 `For Each` 循环遍历 `mapList`，并根据 `select_method` 进行过滤。
   - 根据选择的模式，添加相应的内容到 `DataGridView` 的行中。

### 注意事项：

- 在更新 `DataGridView` 内容时，可能需要清空原有的数据，以防止内容重复，可以在循环前调用 `DataGridView_Res.Rows.Clear()`：
  ```vb
  DataGridView_Res.Rows.Clear()
  ```
- 如果 `DataGridView` 中的列数不变，可以直接更新列标题。如果列数会变化（比如增加或减少列），则需要先调整列数。
- `mapList` 应该是你之前加载数据或生成的 `MapInfo` 列表，需要确保它已经包含了更新后的内容。

### 总结：

这种方式通过监听 `ComboBox` 的选择变化，可以动态地调整 `DataGridView` 的列标题和内容，适应不同的需求。根据数据源的变化，可以随时更新 `DataGridView` 中的显示内容，保持界面的实时性。

## 空白符匹配
`Char.IsWhiteSpace` 方法会同时匹配空格（`' '`）和制表符（`\t`）等所有 Unicode 定义的空白字符。它包括以下常见的空白字符：

- 普通空格（`' '`）
- 制表符（`\t`）
- 换行符（`\n`）
- 回车符（`\r`）
- 不间断空格（`'\u00A0'`）
- 其他 Unicode 定义的空白字符

因此，使用 `Char.IsWhiteSpace` 可以方便地检查字符是否为任何类型的空白符，包括空格和制表符。


`line_text.Substring(currentStart, i - currentStart + 1).Trim()` 其中 Trim 类似于 python 的 strip 函数, 会去除所有的空白字符

如下为修改前的代码, 需要采用 split 分成不同的单词:
```vb
''' <summary>
''' 公用函数: 获取下标为 loc 的字段在一行中所在单词的位置 (不存在, 返回- 1, 不修改 word_res)
''' </summary>
''' <param name="line_text"> 该行中的单词 </param>
''' <param name="loc"> 一般是光标位置的下标 </param>
''' <param name="deliminator"> 分隔符(" "表示空白符,包含"\t") </param>
''' <param name="word_res"> 字段选取结果 </param>
''' <param name="start_loc"> 字段起始下标 </param> 
''' <returns>单词的位置(-1: 不存在该单词)</returns>
Public Shared Function word_idx_in_line(ByVal line_text As String,
									  ByVal loc As Integer,
									  Optional ByVal deliminator As Char = " ",
									  Optional ByRef word_res As String = Nothing,
									  Optional ByRef start_loc As Integer = -1) As Integer
	'检查 loc 在有效范围, 所在位置是否是分隔符(仅考虑光标在单词前面和中间, 兼容选择该部分的情况)
	If (loc < 0 OrElse loc >= line_text.Length) OrElse line_text(loc) = deliminator OrElse
		(deliminator = " " AndAlso line_text(loc) = vbTab) Then Return -1

	' 根据分隔符处理不同的情况
	Dim words As String()

	' 如果空格, 使用正则表达式拆分，将多个空格或制表符视为一个分隔符, 否则直接拆分
	words = If(deliminator = " "c, RegularExpressions.Regex.Split(line_text, "\s+"), line_text.Split(deliminator))

	' 遍历句子的字符部分，找到光标所在的单词(实际上只需统计之前deliminator个数)
	Dim idx As Integer = 0
	Dim num As Integer = 0
	While idx < loc
		'如果分隔符为空格且有连续空格, 跳过(包括开头空格)
		While deliminator = " "c AndAlso (line_text(idx) = vbTab OrElse words(idx) = " ")
			If idx = loc Then Return -1
			idx += 1
		End While

		If line_text(idx) = deliminator OrElse (deliminator = " " AndAlso line_text(idx) = vbTab) Then
			num += 1
			idx += 1
		Else
			'不是 deliminator, 则一定是单词
			start_loc = idx  '记录单词起始下标 
			idx += words(num).Length
			If idx >= loc Then Exit While
		End If
	End While

	word_res = words(num)
	Return num
End Function 
```

修改后代码如下, 可以不使用 split 分割单词, 实际执行效率是更高的:

```vb
Public Shared Function word_idx_in_line(ByVal line_text As String,
                                          ByVal loc As Integer,
                                          Optional ByVal deliminator As Char = " ",
                                          Optional ByRef word_res As String = Nothing,
                                          Optional ByRef start_loc As Integer = -1) As Integer
	'检查 loc 在有效范围, 所在位置是否是分隔符(仅考虑光标在单词前面和中间, 兼容选择该部分的情况)
	If (loc < 0 OrElse loc >= line_text.Length) Then Return -1
	If line_text(loc) = deliminator OrElse
		(deliminator = " " AndAlso Char.IsWhiteSpace(line_text(loc))) Then Return -1

	' 跳过开头的空白字符（包括空格和制表符）
	Dim idx As Integer = 0
	While idx < line_text.Length AndAlso (Char.IsWhiteSpace(line_text(idx)) AndAlso deliminator = " "c OrElse line_text(idx) = deliminator)
		idx += 1
	End While
	If idx > loc Then Return -1   '整个行全都是空白字符(实际上是冗余的)

	Dim currentStart As Integer = idx
	Dim wordIndex As Integer = 0
	Dim inWord As Boolean = False  ' 标记当前是否在单词中

	' 遍历句子的字符部分，找到光标所在的单词(实际上只需统计之前deliminator个数)
	For i As Integer = idx To line_text.Length - 1
		' 检查当前字符是否是分隔符
		If deliminator = " "c AndAlso Char.IsWhiteSpace(line_text(i)) OrElse
		   deliminator <> " "c AndAlso line_text(i) = deliminator Then
			' 如果之前在单词状态，意味着单词结束
			If inWord Then
				inWord = False
			End If
		Else
			' 如果是非分隔符字符且之前不是在单词中，意味着进入了新单词
			If Not inWord Then
				inWord = True
				wordIndex += 1
				currentStart = i
			End If
			' 如果当前下标 i 大于等于 loc，意味着光标处于该单词范围内
			If i >= loc Then
				word_res = line_text.Substring(currentStart, i - currentStart + 1).Trim()
				start_loc = currentStart
				Return wordIndex
			End If
		End If
	Next
	Return -1
End Function
```
