import Grocery 1.0
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item
{
    id: itemParent
    height: thisButton.height
    required property int index
    property var recipeListViewRefs
    property var stackViewRefs
    property string buttonText: RecipeModel.readRecipeName(index) === "" ? "Recipe "+(index+1) : RecipeModel.readRecipeName(index)

    Connections
    {
        target: RecipeModel
        function onRecipeNameChanged(changedIndex, newName)
        {
            if (changedIndex === index) {
                buttonText = newName
            }
        }
    }

    RowLayout
    {
        width: itemParent.width
        Button
        {
            id: thisButton
            Layout.fillWidth: true
            Layout.fillHeight: true
            text: buttonText
            onClicked:
            {
                stackViewRefs.push("RecipeView.qml",{index: index});
                recipeListViewRefs.currentRecipeIndex = index
            }
        }
        ToolButton
        {
            onClicked:
            {
                RecipeModel.removeRecipe(index)
            }
            Layout.preferredWidth: AppTheme.recipeDeleteButtonSize
            Layout.preferredHeight: AppTheme.recipeDeleteButtonSize
            icon.source: "../icons/delete.svg"
            icon.width: AppTheme.iconSize
            icon.height: AppTheme.iconSize
            icon.color: "grey"
        }
    }
}
