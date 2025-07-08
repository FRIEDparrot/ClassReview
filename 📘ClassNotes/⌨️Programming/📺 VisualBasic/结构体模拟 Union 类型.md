未补充: `Throw New NotImplementedException()` 

在 VB.NET 中，结构体的字段是固定的，占用的内存空间在结构体实例创建时就已经分配好了，因此你无法通过条件语句动态释放某些字段的内存空间。不过，可以通过逻辑来控制这些字段的使用，避免在不需要的时候对它们进行赋值。

这里有几个建议，可以更高效地管理这些字段的使用：

### 1. 使用 `Nullable` 类型（可空类型）
对于数值类型的字段，比如 `Integer` 和 `Char`，可以使用 `Nullable` 类型（即 `Integer?` 或 `Char?`），这样在不需要它们时可以设置为 `Nothing`（空值）。虽然这不会减少内存占用，但能明确表达某个字段在当前上下文中不被使用：

```vb.net
Public Structure MapInfo
    Public VarName As String
    Public type As Integer  ' 0 : 位置映射   1: 行 + 单词数映射 
    
    ' used when select method = 0
    Public start_loc As Integer?
    Public selected_length As Integer?
    
    ' used when select method = 1
    Public delm As Char?
    Public line_num As Integer?
    Public word_num As Integer?
    
    Public Text As String       ' 实际上不必保存, 每次打开都会重新读取
    Public FileName As String
End Structure
```

在这种情况下，当 `type` 为 `0` 时，可以设置与行和单词数相关的字段为 `Nothing`：

```vb.net
Dim mapInfo As MapInfo
mapInfo.type = 0
mapInfo.start_loc = 10
mapInfo.selected_length = 5
mapInfo.delm = Nothing
mapInfo.line_num = Nothing
mapInfo.word_num = Nothing
```

这样可以更清晰地管理数据的状态。

### 2. 拆分为多个结构体
如果 `type = 0` 和 `type = 1` 时需要的字段差别很大，可以考虑将它们拆分为两个不同的结构体，并在外层使用一个 `Union` 风格的类或选择使用哪一个：

```vb.net
Public Structure MapInfoPosition
    Public VarName As String
    Public start_loc As Integer
    Public selected_length As Integer
    Public FileName As String
End Structure

Public Structure MapInfoLine
    Public VarName As String
    Public delm As Char
    Public line_num As Integer
    Public word_num As Integer
    Public FileName As String
End Structure

Public Class MapInfo
    Public type As Integer  ' 0 : 位置映射   1: 行 + 单词数映射 
    Public PositionInfo As MapInfoPosition
    Public LineInfo As MapInfoLine
End Class
```

在使用时，根据 `type` 的值选择使用 `PositionInfo` 或 `LineInfo`：
```vb.net
Dim mapInfo As New MapInfo
If mapInfo.type = 0 Then
    mapInfo.PositionInfo = New MapInfoPosition() With {
        .VarName = "Var1",
        .start_loc = 10,
        .selected_length = 5,
        .FileName = "example.txt"
    }
ElseIf mapInfo.type = 1 Then
    mapInfo.LineInfo = New MapInfoLine() With {
        .VarName = "Var2",
        .delm = ","c,
        .line_num = 3,
        .word_num = 5,
        .FileName = "example.txt"
    }
End If
```

这样可以更清楚地区分不同类型的数据结构，只使用需要的数据，逻辑上也更加清晰。虽然这样做会增加一些代码量，但在结构上更加有条理，也能避免不必要的数据赋值。



当前两个剩余模块:
1. 映射保存
2. 响应替换

## 通过协变 covariant 实现返回类型继承
需求: 有一个 Public Class process_data_base_t 实例, 我希望在父类中定义一个 Clone 方法, 而 data_main_steps_t 继承于这个类， 希望 Clone 返回的是 data_main_steps_t 子类类型,

是的，这是可以实现的。可以在父类 `process_data_base_t` 中定义一个通用的 `Clone` 方法，并使用**协变（covariant）**返回类型来返回子类类型的克隆。为此，你需要在 `process_data_base_t` 类中定义一个虚方法（`MustOverride` 或 `Overridable`），并在子类 `data_main_steps_t` 中进行重写，以便返回正确的子类实例。

### 1. 父类定义 `Clone` 方法：
首先，在 `process_data_base_t` 中定义一个通用的 `Clone` 方法，该方法是虚拟的（`Overridable`），并且返回类型为 `process_data_base_t`：

```vb.net
Public Class process_data_base_t
    ' 父类中的通用字段和方法...

    ' 定义一个虚拟的 Clone 方法，返回父类类型
    Public Overridable Function Clone() As process_data_base_t
        Return DirectCast(Me.MemberwiseClone(), process_data_base_t)
    End Function
End Class
```

### 2. 子类 `data_main_steps_t` 重写 `Clone` 方法：
在 `data_main_steps_t` 类中重写 `Clone` 方法，并且确保返回类型为 `data_main_steps_t`：

```vb.net
Public Class data_main_steps_t
    Inherits process_data_base_t

    ' 子类中的特定字段和方法...
	 
    ' 重写 Clone 方法，返回 data_main_steps_t 类型
    Public Overrides Function Clone() As data_main_steps_t
        ' 创建一个浅复制并转换为 data_main_steps_t 类型
        Dim clonedObject As data_main_steps_t = DirectCast(MyBase.Clone(), data_main_steps_t)

        ' 如果需要，手动复制其他复杂类型的属性以实现深复制
        ' For example: clonedObject.SomeProperty = Me.SomeProperty.DeepClone()

        Return clonedObject
    End Function
End Class
```

### 3. 使用示例：
现在，当你调用子类 `data_main_steps_t` 的 `Clone` 方法时，它将返回一个 `data_main_steps_t` 类型的对象：

```vb.net
Dim originalData As New data_main_steps_t()
' 初始化 originalData 的各项属性和字段...

' 克隆对象，类型为 data_main_steps_t
Dim clonedData As data_main_steps_t = originalData.Clone()
```

### 4. 解释和注意事项：
- **协变的返回类型**：`Public Overrides Function Clone() As data_main_steps_t` 方法在子类中重写时，返回类型是 `data_main_steps_t`，这是 VB.NET 允许的，因为 `data_main_steps_t` 是 `process_data_base_t` 的子类。
- **调用 `MyBase.Clone()`**：在子类中，使用 `MyBase.Clone()` 调用父类的 `Clone` 方法，可以利用父类的浅复制逻辑，然后再将其转换为子类类型。
- **深度复制**：如果子类中包含引用类型的属性（如数组、列表、其他类实例），并且希望进行深度复制，可以在 `Clone` 方法中手动复制这些属性。

### 5. 深度复制示例（可选）：
如果 `data_main_steps_t` 中有引用类型的字段，并且你希望进行深度复制，可以在 `Clone` 方法中手动进行，例如：

```vb.net
Public Overrides Function Clone() As data_main_steps_t
    ' 创建一个浅复制并转换为 data_main_steps_t 类型
    Dim clonedObject As data_main_steps_t = DirectCast(MyBase.Clone(), data_main_steps_t)

    ' 深度复制引用类型字段（如果有）
    clonedObject.SomeList = New List(Of String)(Me.SomeList)
    clonedObject.SomeObject = Me.SomeObject.Clone()

    Return clonedObject
End Function
```

这样，`Clone()` 方法既保留了 `data_main_steps_t` 类型的返回类型，也确保了子类对象的完整复制。


## MustInherit 和 MustOverride 
在 VB.NET 中，`MustOverride` 和 `MustInherit` 是两个关键字，它们通常用于定义抽象类和抽象方法，帮助创建类层次结构。这些关键字在对象继承中用于强制子类提供特定的实现，从而实现更灵活和可扩展的设计。下面详细解释它们的用法和区别:
c
### 1. **`MustInherit`**
   - `MustInherit` 关键字用于标记一个类为**抽象类**。抽象类不能被实例化，它们只能作为基类存在。
   - 如果你定义了一个 `MustInherit` 类，那么你不能直接创建该类的实例，只能通过继承它来创建具体的子类实例。
   - 抽象类可以包含抽象方法（用 `MustOverride` 修饰的方法）或非抽象方法。

   **用法示例**：
   ```vb.net
   ' 定义一个抽象类
   Public MustInherit Class Shape
       ' 非抽象方法，可以在基类中提供部分实现
       Public Sub DisplayShapeType()
           Console.WriteLine("This is a shape.")
       End Sub

       ' 抽象方法，必须在子类中实现
       Public MustOverride Function GetArea() As Double
   End Class
   ```

   - 这个 `Shape` 类定义了一个 `DisplayShapeType` 方法，该方法有具体实现。但它也包含了一个 `MustOverride` 方法 `GetArea()`，要求每个继承 `Shape` 的类都必须实现自己的 `GetArea` 方法。

   **创建子类**：
   ```vb.net
   ' 继承自 Shape 的具体类
   Public Class Circle
       Inherits Shape

       Public Property Radius As Double

       ' 实现抽象方法
       Public Overrides Function GetArea() As Double
           Return Math.PI * Radius * Radius
       End Function
   End Class
   ```

   - 在 `Circle` 类中，必须实现 `GetArea` 方法，因为这是从 `Shape` 类继承下来的 `MustOverride` 方法。这样可以确保所有形状都有自己具体的 `GetArea` 实现。

   **不能创建抽象类的实例**：
   ```vb.net
   Dim shape As New Shape() ' 错误：不能实例化 MustInherit 类
   Dim circle As New Circle() ' 正确：可以实例化具体子类
   ```

### 2. **`MustOverride`**
   - `MustOverride` 关键字用于标记**抽象方法**，这些方法**没有实现**，只能在子类中提供具体的实现。
   - 使用 `MustOverride` 的方法不能有方法体，意味着不能在方法中包含任何代码。
   - 任何继承包含 `MustOverride` 方法的类，必须提供这些方法的实现。

   **用法示例**：
   ```vb.net
   Public MustInherit Class Animal
       ' 抽象方法，要求每个子类都必须实现 Speak 方法
       Public MustOverride Sub Speak()
   End Class

   ' 子类必须实现 Speak 方法
   Public Class Dog
       Inherits Animal

       Public Overrides Sub Speak()
           Console.WriteLine("Woof!")
       End Sub
   End Class

   Public Class Cat
       Inherits Animal

       Public Overrides Sub Speak()
           Console.WriteLine("Meow!")
       End Sub
   End Class
   ```

   - 在 `Animal` 类中，`Speak` 方法使用了 `MustOverride` 关键字，因此 `Animal` 类本身不能直接实例化，必须通过子类如 `Dog` 或 `Cat` 来实现。
   - 在子类中，`MustOverride` 方法 `Speak` 必须使用 `Overrides` 关键字进行重写，并提供具体的实现。

   **调用示例**：
   ```vb.net
   Dim dog As New Dog()
   dog.Speak() ' 输出: Woof!

   Dim cat As New Cat()
   cat.Speak() ' 输出: Meow!
   ```

### 3. **`MustInherit` 与 `MustOverride` 的关系**
   - `MustInherit` 关键字用于类，表示该类是抽象的，不能直接创建实例。
   - `MustOverride` 关键字用于方法，表示该方法没有实现，必须在继承的子类中提供实现。
   - 通常在需要让基类定义一种通用接口，但让每个子类自己决定实现细节时，会使用 `MustOverride`。
   - `MustInherit` 类可以包含 `MustOverride` 方法，也可以包含普通的非抽象方法，提供一些通用的功能给子类使用。

### **总结**
- `MustInherit` 强制类只能被继承，不能直接实例化。
- `MustOverride` 强制子类必须实现某个方法，从而确保继承层次中某些方法的实现是强制的。
- 使用它们可以定义出更灵活、易扩展的类结构，适合需要多态行为的场景。


需要注意:  基类中的方法签名与子类重写的方法保持一致，并且子类重写的 `Clone` 方法实际返回子类类型的对象，数据不会丢失。即使方法的签名在基类中是 `process_data_base_t` 类型，当你在子类中进行 `DirectCast` 时，只要返回的实际对象是 `data_main_steps_t` 类型，那么数据就不会丢失。
### 关键点解释
1. **对象数据不会丢失的原因**：
   - `MustOverride` 方法定义的签名是 `process_data_base_t` 类型，这是因为在基类中定义了方法的接口。
   - 子类中 `Overrides` 的方法实际返回一个 `data_main_steps_t` 的对象（即使它是通过 `process_data_base_t` 类型返回的）。
   - 当你在调用时用 `DirectCast` 转换回 `data_main_steps_t` 类型，实际的数据和属性不会丢失，因为对象本身从未被改变，只是编译器在某些上下文中认为它是 `process_data_base_t`。

2. **返回对象的类型与实际对象类型一致**：
   - 假设我们有以下代码：
     ```vb.net
     Public MustInherit Class process_data_base_t
         Public MustOverride Function Clone() As process_data_base_t
     End Class

     Public Class data_main_steps_t
         Inherits process_data_base_t

         Public Property SomeProperty As Integer

         Public Overrides Function Clone() As process_data_base_t
             Dim clonedObject As New data_main_steps_t()
             clonedObject.SomeProperty = Me.SomeProperty
             ' 其他需要复制的属性
             Return clonedObject
         End Function
     End Class
     ```
- 在上面的代码中，`Clone` 方法重写了基类的方法，返回类型是 `process_data_base_t`，但实际上返回的是 `data_main_steps_t` 类型的对象。
- 因此，当你用 `DirectCast` 将结果转换回 `data_main_steps_t` 时，所有 `data_main_steps_t` 的数据都会被保留。
3. **使用 `DirectCast` 转换回子类对象**：
   ```vb.net
   Dim originalData As New data_main_steps_t()
   originalData.SomeProperty = 42

   ' 克隆对象，方法签名是 process_data_base_t，但实际返回的是 data_main_steps_t
   Dim clonedData As data_main_steps_t = DirectCast(originalData.Clone(), data_main_steps_t)

   ' 在转换回 data_main_steps_t 类型后，SomeProperty 的值仍然是 42
   Console.WriteLine(clonedData.SomeProperty) ' 输出：42
   ```

- `originalData.Clone()` 实际上返回了一个 `data_main_steps_t` 的实例，只是由于签名的关系它被视为 `process_data_base_t`。
- 使用 `DirectCast` 将其转换为 `data_main_steps_t` 后，类型转换不会影响对象中数据的完整性，`SomeProperty` 以及其他字段的值都保持不变。

### 结论
- **数据不会丢失**：即使方法的返回类型在签名中是基类类型，实际返回的是子类类型的对象时，数据不会丢失。通过类型转换，可以恢复对所有子类特有属性的访问。
- **关键在于实际对象的类型**：重要的是实际返回的对象是 `data_main_steps_t` 类型，而不是方法签名的类型。对象的内容和状态不受方法签名的影响。


![[attachments/Pasted image 20241017212627.png]]
这个错误信息是由于 `DataGridViewComboBoxCell` 的值无效引起的，常见的原因包括：

1. **ComboBox中不存在的值**：如果在 `DataGridView` 的某个 `ComboBoxCell` 中设置了一个值，但这个值不在 `ComboBox` 的可选项列表中，就会导致此异常。检查你设置到 `ComboBox` 的值是否都存在于 `ComboBox` 的 `Items` 列表中。

2. **绑定的数据源不一致**：如果 `DataGridViewComboBoxColumn` 绑定了一个数据源，而试图在其中设置的值不在这个数据源中，也会导致该错误。确保 `ComboBox` 的 `DataSource` 和要设置的值是一致的。

3. **空值处理不当**：如果 `ComboBox` 不允许空值 (`null` 或 `DBNull`)，而试图将 `null` 设置为其值，就会触发此异常。确保在数据源或 `DataGridView` 单元格设置时正确处理空值。

**解决方法**：
- 可以在 `DataError` 事件中处理这个错误，避免程序崩溃并进行相应的错误处理。例如：

```csharp
private void dataGridView1_DataError(object sender, DataGridViewDataErrorEventArgs e)
{
    // 可以记录错误或者显示自定义提示
    MessageBox.Show("无效的选择，请选择有效的选项。");
    e.Cancel = true; // 防止异常继续传播
}
```

这样可以捕捉到 `DataGridView` 的错误并进行处理，使得程序更加健壮。

```sh
# Package Manager Console
Update-Package -Reinstall
```


对于.NET 3.5 的相关问题: 将 My.Settings.Save() 修改为:
```
My.Settings.Default.Save()
```



## ComboBox 添加 Tag 类
以下是使用 Visual Basic (VB.NET) 重写的代码示例，包含自定义类以及覆盖 `ComboBox` 的 `Item` 属性的实现。

### 方法一：使用自定义类并覆盖 ComboBox 的 Item 属性

```vb
Imports System
Imports System.Windows.Forms

' 自定义类，包含 Text 和 Tag 属性
Public Class ComboBoxItem
    Public Property Text As String
    Public Property Tag As Object

    Public Sub New(ByVal text As String, ByVal tag As Object)
        Me.Text = text
        Me.Tag = tag
    End Sub

    ' 覆盖 ToString 方法，使 ComboBox 显示 Text 属性
    Public Overrides Function ToString() As String
        Return Text
    End Function
End Class

' 自定义的 ComboBox，重写 Items 属性
Public Class CustomComboBox
    Inherits ComboBox

    ' 重写 Items 属性
    Public Shadows Property Items As New ComboBox.ObjectCollection(Me)
End Class

Public Class MainForm
    Inherits Form

    Private WithEvents comboBox As CustomComboBox

    Public Sub New()
        comboBox = New CustomComboBox()
        comboBox.Location = New Drawing.Point(10, 10)
        comboBox.Width = 200

        ' 添加项目
        comboBox.Items.Add(New ComboBoxItem("Item 1", 123))
        comboBox.Items.Add(New ComboBoxItem("Item 2", "SomeTag"))
        comboBox.Items.Add(New ComboBoxItem("Item 3", New With {.ID = 1, .Description = "Complex Tag"}))

        Me.Controls.Add(comboBox)
    End Sub

    Private Sub ComboBox_SelectedIndexChanged(sender As Object, e As EventArgs) Handles comboBox.SelectedIndexChanged
        Dim selectedItem As ComboBoxItem = TryCast(comboBox.SelectedItem, ComboBoxItem)
        If selectedItem IsNot Nothing Then
            MessageBox.Show($"Selected: {selectedItem.Text}, Tag: {selectedItem.Tag}")
        End If
    End Sub

    <STAThread>
    Public Shared Sub Main()
        Application.EnableVisualStyles()
        Application.SetCompatibleTextRenderingDefault(False)
        Application.Run(New MainForm())
    End Sub
End Class
```

### 解释
- **`ComboBoxItem` 类**：使用 VB.NET 定义包含 `Text` 和 `Tag` 属性的类，并重写 `ToString` 方法以便在 `ComboBox` 中显示 `Text`。
- **`CustomComboBox` 类**：继承自 `ComboBox` 并重写了 `Items` 属性，使用 `Shadows` 关键字隐藏基类的 `Items` 属性。
- **`MainForm` 类**：创建并配置 `CustomComboBox`，向其中添加 `ComboBoxItem` 对象。利用 `SelectedIndexChanged` 事件显示选中项的 `Text` 和 `Tag` 值。

### 方法二：使用 `KeyValuePair` (不推荐直接覆盖 `Items` 属性)
这种方法不需要覆盖 `ComboBox` 的 `Items` 属性，而是直接利用 `KeyValuePair`，这种情况下代码更简洁，适用于不需要自定义复杂行为的场景：

```vb
Imports System
Imports System.Windows.Forms

Public Class MainForm
    Inherits Form

    Private WithEvents comboBox As ComboBox

    Public Sub New()
        comboBox = New ComboBox()
        comboBox.Location = New Drawing.Point(10, 10)
        comboBox.Width = 200

        ' 添加 KeyValuePair 项目
        comboBox.Items.Add(New KeyValuePair(Of String, Object)("Item 1", 123))
        comboBox.Items.Add(New KeyValuePair(Of String, Object)("Item 2", "SomeTag"))
        comboBox.Items.Add(New KeyValuePair(Of String, Object)("Item 3", New With {.ID = 1, .Description = "Complex Tag"}))

        ' 让 ComboBox 显示键
        comboBox.DisplayMember = "Key"

        Me.Controls.Add(comboBox)
    End Sub

    Private Sub ComboBox_SelectedIndexChanged(sender As Object, e As EventArgs) Handles comboBox.SelectedIndexChanged
        Dim selectedItem As KeyValuePair(Of String, Object) = DirectCast(comboBox.SelectedItem, KeyValuePair(Of String, Object))
        MessageBox.Show($"Selected: {selectedItem.Key}, Tag: {selectedItem.Value}")
    End Sub

    <STAThread>
    Public Shared Sub Main()
        Application.EnableVisualStyles()
        Application.SetCompatibleTextRenderingDefault(False)
        Application.Run(New MainForm())
    End Sub
End Class
```

这两种方法都可以为 `ComboBox` 项目设置 `Tag` 值，第一种方法适合需要自定义 `ComboBox` 行为的场景，第二种方法则更简单直接。根据实际需求选择合适的方法即可。



