import QtQuick 2.0
import QtQuick.Controls 2.0

ComboBox {
    id: comboBox
    property color primaryCol: "#121212"
    property color secondaryCol: "#262626"
    property color txtColor: "white"
    property color highlightCol: "white"

    z: 1
    height: 60
    width: 180
    anchors.margins: 0
    model: ["1", "2", "3", "4", "5", "6", "7", "8"]
    currentIndex: 0
    font.bold: false
    font.pointSize: stdPointSize

    delegate: ItemDelegate {
        width: comboBox.width
        contentItem: Text {
            text: modelData
            color: txtColor
            font: comboBox.font
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }
        Rectangle  {
            anchors.fill: parent
            color: primaryCol
            visible: comboBox.highlightedIndex === index
        }
        highlighted: comboBox.highlightedIndex === index
    }

    contentItem: Text {
        text: "   " + comboBox.displayText
        font: comboBox.font
        color: txtColor
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 120
        implicitHeight: 40
        color: secondaryCol
        radius: 2
        border.color: highlightCol
    }

    popup: Popup {
        y: comboBox.height - 1
        width: comboBox.width
        implicitHeight: contentItem.implicitHeight
        padding: 1

        Rectangle {
            anchors.fill: parent
            color: secondaryCol
            opacity: 0.8
            z: -1
        }

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: comboBox.popup.visible ? comboBox.delegateModel : null
            currentIndex: comboBox.highlightedIndex
            ScrollIndicator.vertical: ScrollIndicator { }
            delegate: Component {
                Rectangle {
                    width: 100
                    height: 80
                    visible: true
                    color: "transparent"
                    Label {
                        x: 10
                        height: parent.height
                        width: parent.width
                        text: index
                        verticalAlignment: Text.AlignVCenter
                        font.pointSize: stdPointSize
                        font.bold: true
                        color: txtColor
                    }
                }
            }
        }

        background: Rectangle {
            border.color: highlightCol
            color: secondaryCol
            opacity: 0.75
            radius: 2
        }

    }
}
