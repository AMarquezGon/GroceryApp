import "qmlfiles"
import Grocery 1.0
import QtCore
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls


ApplicationWindow
{
    id: window
    width: 360
    height: 520
    visible: true
    title: qsTr("Grocery App")

    Material.theme: Material.Dark
    Material.primary: AppTheme.primaryColor
    Material.accent: AppTheme.accentColor

    Settings
    {
        id: settings
        property string style
    }

    header: HeaderToolBar
    {
        id: headerToolBar
    }

    StackLayout
    {
        id: stackLayout
        currentIndex: 0
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: bottomBar.top

        ListView
        {
            id: itemList
            spacing: AppTheme.bigSpacing
            Layout.fillWidth: true
            Layout.fillHeight: true
            topMargin: AppTheme.margin
            bottomMargin: AppTheme.margin
            model: ItemModel
            delegate: CartItem {
                width: itemList.width
                height: AppTheme.listItemHeight
                modelList: ItemModel
            }
        }
        StackView
        {
            id: stackView
            Layout.fillWidth: true
            Layout.fillHeight: true
            initialItem: ListView
            {
                id: recipeListView
                topMargin: AppTheme.margin
                bottomMargin: AppTheme.margin
                width: stackView.width
                height:stackView.height
                model: RecipeModel.recipeSize
                property int currentRecipeIndex
                delegate: ListOfRecipes
                {
                    width: stackView.width
                    recipeListViewRefs: recipeListView
                    stackViewRefs: stackView
                }
            }
        }
    }

    DeleteAllItemsDialog
    {
        id: deleteAllItemsPopup
    }

    MergeItemsDialog
    {
        id: mergePopup
    }

    Connections
    {
        target: ItemModel
        function onMergeRequested(nameExisting,unitExisting)
        {
            mergePopup.open()
            mergePopup.nameExisting = nameExisting
            mergePopup.unitExisting = unitExisting
        }
    }

    BottomBar
    {
        id: bottomBar
        stackLayout: stackLayout
        onChangeTitleTextRequested: function(newText)
        {
            headerToolBar.titleText = newText
        }
    }

}
