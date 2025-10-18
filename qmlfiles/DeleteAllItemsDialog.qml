import Grocery 1.0
import QtQuick
import QtQuick.Controls

Dialog
{
    id: deleteAllItemsPopup
    width: 0.8*parent.width
    x: Math.round((window.width - width) / 2)
    y: Math.round(window.height / 6)
    modal: true
    focus: true
    title: qsTr("Delete all")

    standardButtons: Dialog.Yes | Dialog.Cancel
    onAccepted:
    {
        if(stackLayout.currentIndex === 0)
        {
            ItemModel.clearAllItems()
        }
        else
        {
            RecipeModel.removeAllRecipes()
        }
        deleteAllItemsPopup.close()
    }
    onRejected:
    {
        deleteAllItemsPopup.close()
    }

    contentItem: Text
    {
        text: stackLayout.currentIndex === 0 ? "Do you want to delete all items?" : "Do you want to delete all recipes?"
        color: "white"
    }
}
