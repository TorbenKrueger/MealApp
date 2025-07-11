import QtQuick 2.15

Rectangle {
    id: comp
    radius: 20
    color: "transparent"
    signal itemClicked(var index)
    signal itemPressAndHold(var index)

    Rectangle {
        id: rectangle
        height: 60
        visible: !buyed
        width: items.width
        color: secondaryColor
        radius: parent.radius
        Text {
            anchors.fill: parent
            anchors.leftMargin: 10
            text: quantity + " " + unit + " " + name
            font.pixelSize: 12
            verticalAlignment: Text.AlignVCenter
            font.bold: true
            color: textColor
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                itemClicked(index)
            }
            onPressAndHold: {
                itemPressAndHold(index)
            }
        }
    }
}
