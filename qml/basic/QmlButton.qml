import QtQuick 2.0

Rectangle {
    id: button

    property string text: ""
    property alias font: textElement.font
    property color txtColor: "white"

    signal clicked(var mouse)
    signal hovered()
    signal leave()
    signal pressAndHold(var mouse)
    signal doubleClicked(var mouse)

    property string orgColor: "#262626"
    property string pressedColor: Qt.lighter(orgColor)

    color: orgColor
    radius: 20
    height: 60
    width: 200

    Text {
        id: textElement
        text: button.text
        font.pointSize: stdPointSize
        font.bold: true
        color: txtColor
        anchors.centerIn: parent
    }
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            button.clicked(mouse)
        }
        onEntered: {
            button.hovered()
        }
        onExited: {
            color = orgColor
            button.leave()
        }
        onDoubleClicked: {
            button.doubleClicked(mouse)
        }
        onPressAndHold: {
            pressAndHold(mouse)
        }
        onPressed: {
            color = pressedColor
        }
        onReleased: {
            color = orgColor
        }
    }
}
