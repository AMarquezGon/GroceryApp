import Grocery 1.0
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Dialog
{
    id: addItemsPopup
    width: 0.8*parent.width
    height: 0.55*parent.height
    x: Math.round((window.width - width) / 2)
    y: Math.round(window.height / 6)
    padding: 0
    modal: true
    focus: true
    title: qsTr("Number of portions")
    property string nameExisting
    property string unitExisting

    standardButtons: Dialog.Ok | Dialog.Cancel
    onAccepted:
    {
        ItemModel.addRecipeItems(modelListObject,wheel.currentIndex+1)
        addItemsPopup.close()
    }
    onRejected:
    {
        addItemsPopup.close()
    }
    Component
    {
        id: highlight
        Rectangle
        {
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width/2
            height: AppTheme.portionNumberSize
            color: AppTheme.buttonPressed
            radius: 4
        }
    }
    contentItem: ListView
    {
        id: wheel
        width: parent.width
        height: parent.height
        model: 20
        clip: true
        delegate: Text
        {
            text: index+1
            width: wheel.width
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment:   Text.AlignVCenter
            font.pixelSize: AppTheme.fontPortions
            color: wheel.currentIndex === index ? "white" : "#cccccc"
            height: AppTheme.portionNumberSize
        }
        preferredHighlightBegin: height/2 - 20        // 20 = delegate/2
        preferredHighlightEnd:   height/2 - 20
        highlight: highlight
        highlightFollowsCurrentItem: false
        highlightRangeMode: ListView.StrictlyEnforceRange
        boundsBehavior: Flickable.StopAtBounds        // no overshoot
        flickDeceleration: 3000
    }
}
