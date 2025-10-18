import Grocery 1.0
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item
{
    property string recipeName
    property int index
    property var modelListObject: RecipeModel.getItemModel(index)
    ColumnLayout
    {
        anchors.fill: parent
        anchors.margins: AppTheme.margin
        TextField
        {
            placeholderText: "Recipe name"
            Layout.alignment: Qt.AlignHCenter
            implicitWidth: parent.width*0.8
            implicitHeight: AppTheme.recipeNameSize
            text: RecipeModel.readRecipeName(index)
            property string lastValue
            Component.onCompleted: lastValue = text
            onEditingFinished:
            {
                if(text!==lastValue){
                    RecipeModel.setRecipeName(index,text)
                    lastValue = text
                }
            }
        }
        ListView
        {
            id: itemList
            spacing: AppTheme.bigSpacing
            Layout.fillWidth: true
            Layout.fillHeight: true
            topMargin: AppTheme.margin
            bottomMargin: AppTheme.margin
            clip: true
            model: modelListObject
            delegate: CartItem
            {
                width: itemList.width
                height: AppTheme.listItemHeight
                modelList: modelListObject
                checkBoxVisible: false
            }
        }
        Button
        {
            text: "Add to cart"
            implicitWidth: parent.width/2
            Layout.alignment: Qt.AlignHCenter
            onClicked: portionSelector.open()
        }

    }
    PortionSelector
    {
        id: portionSelector
    }
}
