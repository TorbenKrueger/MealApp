import QtQuick 2.0
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import "../basic"
import "../mealoverview"

Rectangle {
    id: rectangle
    color: primaryColor

    radius: 20

    property var object: null

    onObjectChanged: {
        rating.setRating(object == null ? 0 : object.rating)
    }

    property int listIndex: -1
    property int swipeIndex: -1

    signal mealClicked(var obj)
    signal mealRemoved(var obj)
    signal selectNewMeal(var lIndex, var sIndex)

    Label {
        anchors.left: image.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 10
        anchors.topMargin: 50

        wrapMode: Text.WrapAtWordBoundaryOrAnywhere

        z: 5
        height: 75
        verticalAlignment: Text.AlignVCenter
        text: object == null ? "" : object.name

        font.bold: true
        font.pointSize: stdPointSize
        color: textColor
    }
    Rectangle{
        z: 4
        y: 0
        height: 30
        width: parent.width
        color: "transparent"
        opacity: 0.75
    }

    QmlImage {
        id: image
        width: parent.height - 50
        height: width

        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 5
        source: object == null ? "" : object.picPath
        imgRotation: object == null ? 0 : object.picRotation
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            mealClicked(object)
        }
        onPressAndHold: {
            mealRemoved(object)
        }
    }
    Rating {
        id: rating
        visible: object == null || !object.valid ? false : true
        anchors.left: image.right
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 35
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        enabled: false
    }

    QmlButton {
        z: 2
        txtColor: textColor
        orgColor: secondaryColor
        visible: !rating.visible
        width: parent.width / 2
        height: width / 4
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Gericht auswählen"
        onClicked: {
            navMasterButton.state = 5
            masterView.interactive = false
            selectNewMeal(listIndex, swipeIndex)
        }
    }
    Rectangle {
        anchors.fill: parent
        color: "transparent"
        visible: !rating.visible
        MouseArea {
            anchors.fill: parent
        }
    }
}
