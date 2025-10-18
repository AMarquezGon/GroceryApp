import Grocery 1.0
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ToolBar
{
        property alias titleText: titleLabel.text
        RowLayout
        {
            spacing: 0
            anchors.fill: parent
            ToolButton
            {
                onClicked: stackView.pop()
                enabled: stackView.depth > 1
                implicitWidth: parent.height
                icon.source: stackView.depth > 1 ? "../icons/backArrow.svg" : ""
            }

            Item
            {
                implicitWidth: parent.height
            }

            Label
            {
                id: titleLabel
                text: qsTr("Shopping cart")
                font.pixelSize: AppTheme.fontTitle
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }

            ToolButton
            {
                onClicked: deleteAllItemsPopup.open()
                enabled: !(stackView.depth > 1)
                implicitWidth: parent.height
                icon.source: !(stackView.depth > 1) ? "../icons/trashcan.svg" : ""
            }

            ToolButton
            {
                onClicked:
                {
                    if (stackLayout.currentIndex === 0)
                    {
                        ItemModel.addOrMergeItem("",0,"Grams",ItemModelType.Never)
                        ItemModel.sort(0,Qt.AscendingOrder)
                    }
                    else
                    {
                        if (stackView.depth===1)
                        {
                            RecipeModel.addItemModel()
                            recipeListView.currentRecipeIndex = recipeListView.count-1
                            stackView.push("RecipeView.qml",{ index: recipeListView.count-1 })
                        }
                        else
                        {
                            RecipeModel.getItemModel(recipeListView.currentRecipeIndex).addOrMergeItem("",0,"Grams",ItemModelType.Never)
                        }
                    }
                }
                icon.source: "../icons/add.svg"
                implicitWidth: parent.height
            }
        }
    }
