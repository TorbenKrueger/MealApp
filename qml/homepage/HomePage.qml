import QtQuick 2.0
import QtQuick.Controls 2.0

import CModel 1.0

import "../basic"
import "../settings"
import "../main"

Rectangle {
    id: homepageRect
    color: primaryColor

    property alias isCheckAll: sortView.isCheckAll

    ListView {
        id: list
        //cacheBuffer: 60 * mealList.count
        model: MealListModel {
            id: model
            list: mealList
        }
        spacing: 5
//        delegate: Rectangle {
//            color: "blue"
//            height: 20
//            width: list.width
//            Text {
//                color: "white"
//                anchors.centerIn: parent
//                text: name
//            }
//        }

        delegate: QmlListDelegate {
            width: parent.width
            height: object.filter(textFieldSearch.text) && object.categorieType(sortView.categories) ? 60 : 0
            visible: object.filter(textFieldSearch.text) && object.categorieType(sortView.categories)

            color: secondaryColor
            txtColor: textColor
            borderColor: highlightColor

            onClicked: mealSelected(object)
            onPressAndHold: readyToRemove(object)
        }
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: textFieldSearch.bottom
        anchors.bottom: parent.bottom
        anchors.rightMargin: 5
        anchors.leftMargin: 5
        anchors.topMargin: 5
        anchors.bottomMargin: 10
        Rectangle {
            anchors.fill: parent
            color: "transparent"
            border.color: "transparent"
        }
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: homepageRect.top
        anchors.bottom: list.top
        color: homepageRect.color
    }

    TextField {
        id: textFieldSearch
        height: 60
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        font.pointSize: stdPointSize
        font.bold: true
        anchors.rightMargin: 70
        anchors.leftMargin: 5
        anchors.topMargin: 5
        placeholderText: qsTr("Suchen")
        color: textColor
        background: Rectangle {
            color: secondaryColor
            border.color: highlightColor
            radius: 20
        }
        onEditingFinished: {
            list.forceActiveFocus()
        }
    }

    QuickMenu {
        id: buttonFilter
        defWidth: 130
        defHeight: 60
        anchors.fill: parent
        anchors.rightMargin: 5
        anchors.topMargin: 5
        defColor: primaryColor
        defTextColor: textColor
        defBorderColor: highlightColor
        defBgColor: secondaryColor
        radius: 20
        model: ["Swipeansicht", "Sortierung", "Einstellungen"]
        onTriggered: {
            if (index === 0) {
                homepage.visible = false
                navMasterButton.state = 8
            }
            else if (index === 1) {
                sortView.visible = true
            }
            else if (index === 2) {
                settings.visible = true
            }

        }
    }

    SortView {
        id: sortView
        visible: false
        anchors.fill: parent
    }
    Settings {
        id: settings
        visible: false
        anchors.fill: parent
    }
}
