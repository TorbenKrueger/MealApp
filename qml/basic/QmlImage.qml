import QtQuick 2.0
import QtGraphicalEffects 1.0

Rectangle {
    id: imgContainer

    width: 100
    height: 100

    color: "transparent"
    radius: 20

    property alias fillMode: image.fillMode
    property alias imgRotation: rotateImagePhoto.angle
    property alias source: image.source

    signal clicked()
    signal pressAndHold()

    Image {
        id: image
        visible: true
        anchors.fill: parent
        anchors.margins: 0
        source: ""
        fillMode: Image.PreserveAspectCrop
        clip: true

        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Item {
                width: image.width
                height: image.height
                Rectangle {
                    anchors.centerIn: parent
                    width: image.adapt ? image.width : Math.min(image.width, image.height)
                    height: image.adapt ? image.height : width
                    radius: imgContainer.radius
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                imgContainer.clicked()
            }
            onPressAndHold: {
                imgContainer.pressAndHold()
            }
        }

        transform: Rotation{
            id: rotateImagePhoto
            angle: 0
            origin.x: image.width / 2
            origin.y: image.height / 2
        }
    }
}
