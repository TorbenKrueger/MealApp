import QtQuick 2.0
import QtQuick.Dialogs 1.0

ColorDialog {
    id: colorDialog
    title: "Bitte eine Farbe auswählen"
    visible: false;
    signal colorSelected(var color)

    function show(){visible=true;}
    function close(){visible=false;}

    onAccepted: {
        colorSelected(colorDialog.color)
        close()
    }
    onRejected: {
        close()
    }
}
