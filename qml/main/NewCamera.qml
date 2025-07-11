import QtQuick 2.0

import QtQuick.Controls 2.0
import QtMultimedia 5.12

import "../basic"

Rectangle {
    id: root
    color: "#121212"

    property var cameraObj: undefined

    property var object: null

    signal pictureTaken(var path)

    property string tmpPath: ""

    onVisibleChanged: {
        if (visible){
            cameraObj = Qt.createQmlObject(" import QtQuick 2.0;import QtQuick.Controls 2.0;import QtMultimedia 5.12;
        Camera {
            id: camera;
            Component.onCompleted: {
                camera.imageCapture.resolution = camera.imageCapture.supportedResolutions[camera.imageCapture.supportedResolutions.length - 1];
            }

            imageProcessing.whiteBalanceMode: CameraImageProcessing.WhiteBalanceFlash;
            exposure {
                exposureCompensation: -1.0;
                exposureMode: Camera.ExposurePortrait;
            }
            flash.mode: Camera.FlashRedEyeReduction;
            captureMode: Camera.CaptureStillImage;
            imageCapture {
                onImageCaptured: {
                    photoPreview.source = preview;
                    cameraPrev.visible = false;
                    takePicture.visible = false;
                    accept.visible = true;
                    rejected.visible = true;
                }
            }
            focus.focusMode: Camera.FocusContinuous;
            imageCapture.onImageSaved: {
                tmpPath = path
            }
            onError: {
                        console.log('error:' + errorString);
                    }
        }", root, "cameraObj")
            cameraPrev.source = cameraObj
        }
        else {
            if (cameraObj != undefined)
                cameraObj.destroy()

            takePicture.visible = true
            cameraPrev.visible = true
            photoPreview.source = ""
            accept.visible = false
            rejected.visible = false
        }
    }

    VideoOutput {
        id: cameraPrev
        z: 5
        height: parent.height - 100
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        autoOrientation: true
        fillMode: VideoOutput.PreserveAspectFit
        focus : visible // to receive focus and capture key events when visible
        MouseArea {
            anchors.fill: parent
            onClicked: {
                cameraObj.focus.focusPointMode = Camera.FocusPointCustom
                cameraObj.focus.customFocusPoint.x = mouseX
                cameraObj.focus.customFocusPoint.x = mouseY
            }
        }
    }

    Image {
        id: photoPreview
        visible: !cameraPrev.visible
        anchors.fill: cameraPrev
        fillMode: VideoOutput.PreserveAspectFit
    }
    Row {
        anchors.top: cameraPrev.bottom
        anchors.left: cameraPrev.left
        anchors.right: cameraPrev.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        spacing: 5
        anchors.topMargin: 10
        anchors.bottomMargin: 10

        QmlButton {
            id: takePicture
            txtColor: textColor
            orgColor: secondaryColor
            width: parent.width / 2 - 2.5
            text: "Foto aufnehmen"
            onClicked: {
                loading.running = true
                cameraObj.imageCapture.capture()
            }
        }
        QmlButton {
            id: cancle
            txtColor: textColor
            orgColor: secondaryColor
            width: parent.width / 2 - 2.5
            text: "Abbrechen"
            onClicked: {
                root.visible = false
            }
        }
    }

    Row {
        anchors.top: cameraPrev.bottom
        anchors.left: cameraPrev.left
        anchors.right: cameraPrev.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        spacing: 5
        anchors.topMargin: 10
        anchors.bottomMargin: 10

        QmlButton {
            id: accept
            txtColor: textColor
            orgColor: secondaryColor
            width: parent.width / 2 - 2.5
            visible: false
            text: "Okay"
            onVisibleChanged: {
                if (visible) {
                    loading.running = false
                }
            }

            onClicked: {
                takePicture.visible = true
                accept.visible = false
                rejected.visible = false
                pictureTaken(tmpPath)
                root.visible = false
            }
        }
        QmlButton {
            id: rejected
            txtColor: textColor
            orgColor: secondaryColor
            width: parent.width / 2 - 2.5
            text: "Neues Foto\naufnehmen"
            visible: false
            onClicked: {
                takePicture.visible = true
                cameraPrev.visible = true
                photoPreview.source = ""
                accept.visible = false
                rejected.visible = false
                object.savePicture("") // löschen
            }
        }
    }

    BusyIndicator {
        id: loading
        z: 15
        width: 150
        height: 150
        anchors.verticalCenter: cameraPrev.verticalCenter
        anchors.horizontalCenter: cameraPrev.horizontalCenter
        running: false
    }
    Rectangle {
        z: 14
        anchors.fill: parent
        visible: loading.running
        opacity: 0.75
        color: "white"
    }
    Rectangle {
        z: 13
        anchors.fill: cameraPrev
        color: root.color
        visible: loading.running
    }
}

