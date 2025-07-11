import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    property alias searchText: textFieldSearch.text
    color: primaryColor

    height: 60

    TextField {
        id: textFieldSearch
        height: 60
        anchors.left: parent.left
        anchors.right: buttonFilter.left
        anchors.top: parent.top
        font.pointSize: stdPointSize
        font.bold: true
        anchors.rightMargin: buttonFilter.defWidth - 10
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
        defWidth: 75
        defHeight: 60
        anchors.fill: parent
        anchors2.rightMargin: 5
        anchors2.topMargin: 5
        defColor: primaryColor
        defTextColor: textColor
        defBorderColor: highlightColor
        defBgColor: secondaryColor
        radius: 20
        model: ["Swipeansicht", "Einstellungen"]
        onTriggered: {
            if (index == 0)
                homepage.visible = false
            else if (index == 1)
                sortView.visible = true
        }
    }

    SortView {
        id: sortView
        visible: false
        anchors.fill: parent
    }
}
