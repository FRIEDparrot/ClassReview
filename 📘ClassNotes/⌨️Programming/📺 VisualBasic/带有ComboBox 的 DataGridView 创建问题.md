在 `DataGridView` 中，即使存在一个空白行，添加一个新的有数据的行一般不会报错。`DataGridView.Rows.Add()` 方法会添加新的行，并且不会因为之前存在的空白行而出错。`DataGridView` 可以包含空白行和有数据的行，二者不会互相干扰。

不过，有以下几种情况可能会导致添加行时出现错误或意外行为：

1. **空白行的自动生成：** 
   如果 `AllowUserToAddRows` 属性设置为 `True`（默认值），`DataGridView` 会在末尾自动显示一个空白行，允许用户手动添加数据。当你调用 `Rows.Add()` 时，这个自动生成的空白行不应引起冲突。

2. **数据验证失败：**
   如果你在 `DataGridView` 中使用了数据验证，并且某些单元格的值不符合验证规则，则可能会在添加新行时引发错误或警告。此时，可以在添加前暂时禁用验证，然后在添加完成后重新启用。

3. **数据绑定模式：**
   如果 `DataGridView` 是绑定到数据源（例如 `DataTable`），那么直接使用 `Rows.Add()` 可能会出错。绑定模式下，应该在数据源中添加行，而不是直接操作 `DataGridView` 的 `Rows` 集合。

### 示例代码
以下是一个在默认情况下添加有数据行的示例：

```vb
' 如果你希望用户不能手动添加行，可以设置 AllowUserToAddRows 为 False
__dataGrid.AllowUserToAddRows = False

' 添加一个空白行（可选）
__dataGrid.Rows.Add()

' 添加一个有数据的行
Dim rowIndex As Integer = __dataGrid.Rows.Add()
__dataGrid.Rows(rowIndex).Cells(0).Value = "数据1"
__dataGrid.Rows(rowIndex).Cells(1).Value = "数据2"
__dataGrid.Rows(rowIndex).Cells(2).Value = "数据3"
```

### 总结
- 在 `DataGridView` 中，即使存在空白行，添加新行通常不会报错。
- 如果出现错误，可能是因为数据验证或绑定的问题，而不是空白行的存在。
- 通过 `AllowUserToAddRows` 控制用户是否可以手动添加空白行，以避免不必要的干扰。




当你调用 `newRow.CreateCells(__dataGrid)` 时，实际上是在创建一个新的 `DataGridViewRow`，其单元格结构与现有的 `DataGridView` 列一致。这一行还没有实际添加到 `DataGridView` 中，你需要手动将它添加到 `DataGridView` 的 `Rows` 集合中。

### 如何使用 `CreateCells` 添加行
`CreateCells` 的主要作用是根据指定的 `DataGridView` 创建一个新的 `DataGridViewRow`，并且让它的单元格与目标 `DataGridView` 的列结构匹配。通常用于准备一个新的行并将数据填充进去。以下是使用 `CreateCells` 添加新行的步骤：

1. 创建一个新的 `DataGridViewRow` 对象。
2. 调用 `CreateCells` 方法来根据现有的 `DataGridView` 创建单元格。
3. 填充行的数据。
4. 将新行添加到 `DataGridView.Rows` 中。

### 示例代码
以下是如何使用 `CreateCells` 添加行的完整示例：

```vb
' 创建一个新的 DataGridViewRow 对象
Dim newRow As New DataGridViewRow()

' 根据现有的 DataGridView 创建单元格结构
newRow.CreateCells(__dataGrid)

' 填充数据到新行的单元格
newRow.Cells(0).Value = "数据1"
newRow.Cells(1).Value = "数据2"
newRow.Cells(2).Value = "数据3"

' 将新行添加到 DataGridView
__dataGrid.Rows.Add(newRow)
```

### 关于绑定模式的注意事项
- 如果你的 `DataGridView` 是绑定到数据源的（例如 `DataTable` 或 `BindingList`），那么应该在数据源中添加数据，而不是直接对 `DataGridView` 使用 `Rows.Add()` 或 `CreateCells` 方法。
- 在绑定模式下直接操作 `DataGridView.Rows` 会导致数据和界面不同步，并且可能引发错误。

### 使用绑定模式添加行的示例
如果 `__dataGrid` 是绑定到 `DataTable` 的，正确的方式是向 `DataTable` 中添加行，例如：

```vb
' 假设 __dataGrid.DataSource 是绑定到一个 DataTable 对象
Dim table As DataTable = DirectCast(__dataGrid.DataSource, DataTable)

' 创建一个新的行并添加数据
Dim newRow As DataRow = table.NewRow()
newRow("Column1") = "数据1"
newRow("Column2") = "数据2"
newRow("Column3") = "数据3"

' 将新行添加到 DataTable
table.Rows.Add(newRow)
```

### 总结
- `CreateCells` 创建的是一个新的 `DataGridViewRow`，其单元格匹配现有的 `DataGridView` 列结构。你需要手动将这个行添加到 `DataGridView`。
- 如果 `DataGridView` 是绑定到数据源的，应在数据源中添加数据，而不是直接操作 `DataGridView.Rows`。这样可以确保数据和显示的一致性。

![[attachments/Pasted image 20241019002001.png]]
这个错误信息表示 `DataGridView` 中的 `ComboBox` 单元格所接受的值不在它的可用选项列表中。`DataGridViewComboBoxCell` 的值必须是它的 `Items` 集合中已经存在的某一个值。如果你给 `ComboBox` 单元格设置了一个不在列表中的值，就会出现这样的 `ArgumentException` 错误。

1. **确保设置的值在 `ComboBox` 的 `Items` 集合中**：
   在向 `DataGridView` 添加新行并设置 `ComboBox` 单元格的值之前，先确保该值已经存在于 `DataGridViewComboBoxCell` 的 `Items` 集合中。

   ```vb
   ' 假设我们操作的是第 1 列的 ComboBox 单元格
   Dim comboBoxCell As DataGridViewComboBoxCell = DirectCast(newRow.Cells(1), DataGridViewComboBoxCell)
   
   ' 检查值是否在 ComboBox 的选项中
   Dim newValue As String = "你的值"
   If comboBoxCell.Items.Contains(newValue) Then
       comboBoxCell.Value = newValue
   Else
       ' 如果值不存在，可以考虑添加到 ComboBox 的 Items 中
       comboBoxCell.Items.Add(newValue)
       comboBoxCell.Value = newValue
   End If
   ```

2. **使用 `DataError` 事件来捕获错误**：
   你可以在 `DataGridView` 上处理 `DataError` 事件，以避免程序因该错误而崩溃。可以在错误发生时记录信息或做出一些替代处理。

   ```vb
   Private Sub __dataGrid_DataError(sender As Object, e As DataGridViewDataErrorEventArgs) Handles __dataGrid.DataError
       MessageBox.Show("无效的 ComboBox 值，请选择有效的选项。")
       e.Cancel = True ' 防止异常继续抛出
   End Sub
   ```

3. **绑定数据源时添加默认值**：
   如果你的 `DataGridViewComboBoxColumn` 是绑定到数据源的，确保数据源中包含的选项包含所有可能的值。

### 总结
- 在为 `DataGridViewComboBoxCell` 设置值时，确保值在 `Items` 集合中。
- 使用 `DataError` 事件捕获错误，以防止未处理的异常。
- 如果是绑定的数据源，确保 `ComboBox` 的数据源包含所有需要设置的值。

通过这些方法，可以避免 `DataGridViewComboBoxCell` 中的值不在选项列表中导致的错误。



## 带有ComboBox 的 DataView 创建代码

```vb
''' <summary>
''' 添加行, 不是直接操作 DataGridView.Rows 集合(避免出错问题)
''' </summary>
''' <note>加入前必须保证数据的有效性</note>
''' <param name="row"></param>
Private Sub __add_row_to_grid(row As DataGridViewRow)
	' 添加一个空行
	Dim newRowIndex As Integer = __dataGrid.Rows.Add()

	' 循环遍历每一列
	For i = 0 To __dataGrid.ColumnCount - 1
		' 获取目标单元格
		Dim newCell = __dataGrid.Rows(newRowIndex).Cells(i)
		Dim value = row.Cells(i).Value

		' 检查单元格是否为 ComboBox 类型
		If TypeOf newCell Is DataGridViewComboBoxCell Then
			Dim comboBoxCell As DataGridViewComboBoxCell = DirectCast(newCell, DataGridViewComboBoxCell)

			' 检查值是否在 ComboBox 的选项中
			If comboBoxCell.Items.Contains(value) Then
				comboBoxCell.Value = value
			Else
				' 如果值不在 ComboBox 的选项中，可以添加它
				comboBoxCell.Items.Add(value)
				comboBoxCell.Value = value
			End If
		Else
			' 如果不是 ComboBox 单元格，直接设置值
			newCell.Value = value
		End If
	Next
End Sub
```