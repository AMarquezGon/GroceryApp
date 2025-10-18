import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle
{
    id: bottomBar
    anchors.right: parent.right
    anchors.left: parent.left
    anchors.bottom: parent.bottom
    implicitHeight: AppTheme.bottomBarSize
    Layout.fillWidth: true
    required property var stackLayout
    color: AppTheme.buttonPressed

    signal changeTitleTextRequested(string newText)

    ButtonGroup
    {
        buttons: [cartButton,recipeButton]
    }
    RowLayout
    {
        id:bottomBarLayout
        spacing: AppTheme.bottomBarSpacing
        anchors.fill:parent
        Item { Layout.fillWidth: true}
        Button
        {
            id: cartButton
            text: "Cart"
            implicitWidth: parent.width*0.4
            icon.source: "../icons/shoppingCart.svg"
            background: Rectangle
            {
                radius: AppTheme.bottomBarButtonRadius
                color: !cartButton.down ? AppTheme.buttonPressed : AppTheme.buttonNotPressed
                border.width: AppTheme.bottomBarButtonBorderSize
                border.color: "white"
                Behavior on color
                {
                    ColorAnimation{duration: 150}
                }
            }
            onClicked:
            {
                stackLayout.currentIndex = 0;
                changeTitleTextRequested("Shopping cart")
                if(stackView.depth>1)
                {
                    stackView.pop();
                }
            }
        }
        Button
        {
            id: recipeButton
            text: "Recipes"
            implicitWidth: parent.width*0.4
            icon.source: "../icons/recipe.svg"

            onClicked:
            {
                stackLayout.currentIndex = 1;
                changeTitleTextRequested("Recipes")
            }
            background: Rectangle
            {
                radius: AppTheme.bottomBarButtonRadius
                color: !recipeButton.down ? AppTheme.buttonPressed : AppTheme.buttonNotPressed
                border.width: AppTheme.bottomBarButtonBorderSize
                border.color: "white"
                Behavior on color{
                    ColorAnimation{duration: 150}
                }
            }
        }
        Item{ Layout.fillWidth: true }
    }
}
