import QtQuick 2.0
import QtGraphicalEffects 1.0

Rectangle {
    id: root

    property color txtColor: "white"
    property color borderColor: "white"

    property bool showPictures: true

    property var picObject: null

    signal clicked(var object)
    signal pressAndHold(var object)

    radius: 20

    Text {
        x: 10
        height: parent.height
        width: parent.width
        text: name
        verticalAlignment: Text.AlignVCenter
        font.pointSize: stdPointSize
        font.bold: true
        color: txtColor
    }
    Rectangle {
        anchors.fill: parent
        visible: false;
        anchors.margins: 1
        radius: root.radius
        border.color: borderColor
        color: "transparent"
    }

    QmlImage {
        id: img
        height: parent.height
        width: height
        anchors.right: parent.right
        anchors.rightMargin: 0

        imgRotation: pictureRotation
        //source: object.picturePath
    }

    MouseArea {
        anchors.fill: parent
        onClicked: root.clicked(object)
        onPressAndHold: root.pressAndHold(object)
    }
}
