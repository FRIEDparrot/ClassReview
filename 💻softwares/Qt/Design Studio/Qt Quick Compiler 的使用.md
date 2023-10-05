qml文件允许逻辑的代码实现和ui控件的实现部分相分离

在 Qt 6 中,UI 和逻辑可以全部在 .ui.qml 文件中定义,无需额外的 .h 和 .cpp 文件。但是对于复杂逻辑的代码，仍然建议在.cpp 文件中进行实现

https://www.youtube.com/watch?v=9BcAYDlpuT8

```python 
Button {
    id: myButton
    text: "Click me!"
    width: 100
}
```

```python
import QtQuick
import QtMultimedia
import QtQuick.Controls

Item {
    id: root

    property bool active: false
    property bool playing: false
    visible: active && playing

    function playUrl(url) {
        playing = true
        mediaPlayer.source = url
        mediaPlayer.play()
    }

    function stop() {
        playing = false
        mediaPlayer.stop()
    }

    onActiveChanged: function() {
        if (!active)
            stop();
    }

    VideoOutput {
        anchors.fill: parent
        id: videoOutput
    }

    MediaPlayer {
        id: mediaPlayer
        videoOutput: videoOutput
        audioOutput: AudioOutput {}
    }

    HoverHandler { id: hover }

    RoundButton {
        width: 50
        height: 50
        opacity: hover.hovered && active ? 1.0 : 0.0
        anchors.centerIn: root
        radius: 25
        text: "\u25A0";
        onClicked: root.stop()

        Behavior on opacity { NumberAnimation { duration: 200 } }
    }
}

```

```cpp 
QQmlComponent component(&engine,
						QUrl::fromLocalFile("MyItem.qml"));
QObject *object = component.create();
delete object;
```