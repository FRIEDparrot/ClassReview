对于按照不同类型的多子条件判断的如下办法:
```vb
 Select Case type
            Case MAPPING_TYPE_LOC_LENGTH
                cmp = Function(x, y)
                          If x.start_loc <> y.start_loc Then
                              If Not reverse Then
                                  x.start_loc.CompareTo(y.start_loc)
                              Else
                                  y.start_loc.CompareTo(x.start_loc)
                              End If
                          Else
                              If Not reverse Then
                                  x.selected_length.CompareTo(y.selected_length)
                              Else
                                  y.selected_length.CompareTo(x.selected_length)
                              End If
                          End If
                      End Function
            Case MAPPING_TYPE_LINE_WORDIDX
                cmp = Function(x, y)
                          If x.line_num <> y.line_num Then
                              If Not reverse Then
                                  x.line_num.CompareTo(y.line_num)
                              Else
                                  y.line_num.CompareTo(x.line_num)
                              End If
                          Else
                              If Not reverse Then
                                  x.word_num.CompareTo(y.word_num)
                              Else
                                  y.word_num.CompareTo(x.word_num)
                              End If
                          End If
                      End Function
            Case Else
                Form_Debug.Log("invalid mapping type to sort : " & type, LOG_LEVEL_ERROR)
                Return True
        End Select
```

可以简化重构为如下的代码:
```vb
Select Case type
	Case MAPPING_TYPE_LOC_LENGTH
		cmp = Function(x, y)
				  Dim result As Integer = x.start_loc.CompareTo(y.start_loc)
				  If result = 0 Then result = x.selected_length.CompareTo(y.selected_length)
				  Return If(reverse, -result, result)
			  End Function
	Case MAPPING_TYPE_LINE_WORDIDX
		cmp = Function(x, y)
				  Dim result As Integer = x.line_num.CompareTo(y.line_num)
				  If result = 0 Then result = x.word_num.CompareTo(y.word_num)
				  Return If(reverse, -result, result)
			  End Function
	Case Else
		Form_Debug.Log("invalid mapping type to sort : " & type, LOG_LEVEL_ERROR)
		Return True
End Select
```

