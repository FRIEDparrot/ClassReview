
```cpp 
void student::mySlot(QString paraname){
    // QString 转换char*的方法
    // 使用toUtf8编码， 则此时方法是QString先通过.Utf8()转换成QByteArray,再转换为char*
    qDebug()<< paraname.toUtf8().data() << " reolad triggered";
}
```


可以使用 `QString::number(13)` 将数字转换为字符串

对QString的格式进行控制: 
```cpp 
// 控制格式
QString str = QString("鼠标按键位置:  x = %1   y = %2   %3   %4").arg(100).arg(200).arg(300).arg(400);
```

