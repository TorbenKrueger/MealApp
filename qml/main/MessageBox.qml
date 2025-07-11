import QtQuick 2.0

import "../basic"

Rectangle {
    id: rectangle
    visible: true
    anchors.fill: parent
    color: "transparent"

    property bool onlyOkVisible: false
    property string title: "Überschrift"
    property string text: ""

    property string okText: "Okay"
    property string nokText: "Abbrechen"

    property int pointSize: stdPointSize

    signal accepted()
    signal rejected()

    function show()
    {
        rectangle.visible = true
    }

    function close()
    {
        rectangle.visible = false
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            rejected()
            close()
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "white"
        opacity: 0.75

    }

    Rectangle {
        id: rectangle3
        width: 300
        height: 200
        radius: 20
        color: primaryColor
        border.color: highlightColor
        anchors.centerIn: parent

        QmlButton {
            id: qmlButton
            txtColor: textColor
            orgColor: "transparent"
            text: okText
            width: onlyOkVisible ? parent.width : parent.width / 2 - 1
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            font.pointSize: pointSize
            anchors.margins: 1
            onClicked: {
                rectangle.visible = false
                accepted()
            }
        }

        QmlButton {
            id: qmlButton1
            txtColor: textColor
            orgColor: "transparent"
            text: nokText
            visible: !onlyOkVisible
            anchors.bottom: parent.bottom
            anchors.left: qmlButton.right
            anchors.bottomMargin: 1
            font.pointSize: pointSize
            width: parent.width / 2 - 1
            onClicked: {
                rectangle.visible = false
                rejected()
            }
        }

        Rectangle {
            id: rectangle1
            height: 60
            color: "transparent"
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.leftMargin: 1
            anchors.topMargin: 1
            anchors.rightMargin: 1

            Text {
                id: text1
                color: textColor
                text: title
                anchors.fill: parent
                anchors.leftMargin: 10
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                font.pointSize: headLinePointSize
                font.bold: true
            }
        }

        Rectangle {
            id: rectangle2
            color: "transparent"
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: rectangle1.bottom
            anchors.bottom: qmlButton.top
            anchors.rightMargin: 10
            anchors.leftMargin: 10
            anchors.topMargin: 10
            anchors.bottomMargin: 10

            Text {
                id: text2
                color: textColor
                text: rectangle.text
                anchors.fill: parent
                anchors.margins: 3
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                font.pointSize: pointSize
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            }
        }
    }
}
