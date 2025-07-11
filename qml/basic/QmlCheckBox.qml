import QtQuick 2.15
import QtQuick.Controls 2.15

CheckBox {
    id: control
    text: qsTr("CheckBox")
    checked: false

    property color checkColor: "#21be2b"
    property color uncheckColor: "#17a81a"
    property color txtColor: "white"
    property color borderColor: "white"
    property int radius: 3

    indicator: Rectangle {
        implicitWidth: 26
        implicitHeight: 26
        x: control.leftPadding
        y: parent.height / 2 - height / 2
        radius: parent.radius
        border.color: borderColor

        Rectangle {
            width: 14
            height: 14
            x: 6
            y: 6
            radius: 2
            color: control.down ? uncheckColor : checkColor
            visible: control.checked
        }
//        Rectangle {
//            width: 14
//            height: 7
//            x: 6
//            y: 6
//            radius: 2
//            color: checkColor
//            visible: control.checkState = Qt.PartiallyChecked
//        }
    }

    contentItem: Text {
        text: control.text
        font: control.font
        color: txtColor
        verticalAlignment: Text.AlignVCenter
        leftPadding: control.indicator.width + control.spacing
    }
}
