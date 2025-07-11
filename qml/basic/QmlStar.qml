import QtQuick 2.15
import QtQuick.Shapes 1.15

Rectangle {
    id: root
    width: 115 * size
    height: 105 * size
    rotation: 180
    color: "transparent"

    property color starColor: "gold"
    property double size: 0.3

    signal starClicked()

    Shape {
        anchors.fill: parent
        anchors.centerIn: parent

        ShapePath {
            strokeWidth: 2
            strokeColor: starColor

            fillGradient: LinearGradient {
                GradientStop { position: 0; color: starColor }
                GradientStop { position: 1; color: starColor }
            }

            strokeStyle: ShapePath.SolidLine
            startX: 25 * size
            startY: 5 * size
            PathLine { id: aP; x: 25 * size; y: 5 * size }
            PathLine { id: bP; x: 55 * size; y: 20 * size }
            PathLine { id: cP; x: 85 * size; y: aP.y}
            PathLine { id: dP; x: 80 * size; y: 35 * size }

            PathLine { id: eP; x: 105 * size; y: 60 * size }
            PathLine { id: fP; x: 70 * size; y: 65 * size }

            PathLine { id: gP; x: bP.x; y: 95 * size }

            PathLine { id: hP; x: 40 * size; y: fP.y }
            PathLine { id: iP; x: 5 * size; y: eP.y }

            PathLine { id: jP; x: 30 * size; y: dP.y }
        }
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            starClicked()
        }
    }
}
