import Grocery 1.0
import QtQuick
import QtQuick.Controls

Dialog
{
    id: mergePopup
    width: 0.8*parent.width
    height: 0.35*parent.height
    x: Math.round((window.width - width) / 2)
    y: Math.round(window.height / 6)
    modal: true
    focus: true
    title: qsTr("Merge items")
    property string nameExisting
    property string unitExisting

    standardButtons: Dialog.Yes | Dialog.No
    onAccepted:
    {
        ItemModel.merge(nameExisting,unitExisting)
        mergePopup.close()
    }
    onRejected:
    {
        mergePopup.close()
    }

    contentItem: Text
    {
        text: "One or more items with the same name and units were found. Would you like to merge all similar items?"
        wrapMode: Text.Wrap
        color: "white"
    }
}
