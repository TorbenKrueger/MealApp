import QtQuick 2.0

Rectangle {
    id: root
    height: 30
    width: 270
    radius: 30

    visible: false

    property int closeAfter: 2000

    property string text: "Das ist ein Test"
    property alias font: textElement.font
    property color txtColor: "white"

    function show(message)
    {
        autoClose.start()
        if (message != "")
            text = message
        visible = true
    }
    function close()
    {
        visible = false
    }
    Timer {
        id: autoClose
        interval: closeAfter
        onTriggered: close()
        repeat: false
    }
    Text {
        id: textElement
        text: root.text
        anchors.centerIn: root
        font.pointSize: stdPointSize
        color: txtColor
    }
}
