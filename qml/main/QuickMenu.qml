import QtQuick 2.0

Rectangle {
    id: root
    color: "transparent"

    signal triggered(var index)

    onTriggered: {
        select.visible = false
    }

    property var model: []
    property var objects: []
    property int visiHeight: 3
    property int spacing: 2

    property int defHeight: 60
    property int defWidth: 120
    property color defColor: "green"
    property color defTextColor: "white"
    property color defBorderColor: "white"
    property color defBgColor: "darkgray"
    property alias font: fontLoader.font

    property alias anchors2: master.anchors

    Text {
        id: fontLoader
        text: qsTr("")
        font.pointSize: stdPointSize
        visible: false
    }

    Rectangle {
        visible: select.visible
        anchors.fill: parent
        color: "white"
        opacity: 0.3
    }
    Rectangle {
        id: master
        z: 1
        anchors.top: parent.top
        anchors.right: parent.right
        height: 60
        width: 60
        color: defBgColor
        radius: root.radius
        border.color: defTextColor
        Column {
            spacing: visiHeight * 2
            anchors.centerIn: parent
            width: parent.width / 1.25
            Rectangle {
                color: defTextColor
                height: visiHeight
                width: parent.width
            }
            Rectangle {
                color: defTextColor
                height: visiHeight
                width: parent.width
            }
            Rectangle {
                color: defTextColor
                height: visiHeight
                width: parent.width
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (model.join("") == "") {
                    triggered("-1")
                    return;
                }

                for (var i = 0; i < objects.length; i++) {
                    objects[i].destroy()
                }
                objects = []

                for (i = 0; i < model.length; i++) {
                    var obj = Qt.createQmlObject("import QtQuick 2.0;
                    Rectangle {
                        height: "+defHeight+";
                        width: "+defWidth+";
                        color: '"+defColor+"';
                        border.color: '"+defBorderColor+"';
                        radius: 20
                        Text {
                            anchors.centerIn: parent;
                            text: '"+model[i]+"';
                            color: '"+defTextColor+"';
                            font.pointSize: "+fontLoader.font.pointSize+";
                            font.bold: "+fontLoader.font.bold+";
                        }
                    MouseArea {
                        anchors.fill: parent;
                        onClicked: triggered("+i+");
                    }
                    }", col, "obj" + i)
                    objects.push(obj)
                }
                select.visible = true

            }
        }

        Rectangle {
            id: select
            visible: false
            anchors.right: parent.right
            width: defWidth
            Column {
                id: col
                spacing: root.spacing
            }
        }
    }

    MouseArea {
        enabled: select.visible
        anchors.fill: parent
        onClicked: {
            select.visible = false
        }
    }
}
