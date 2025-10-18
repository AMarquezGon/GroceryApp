pragma Singleton
import QtQuick

QtObject
{
    readonly property color primaryColor: "#106114"
    readonly property color accentColor: "#1c9122"
    readonly property color buttonPressed: "#707070"
    readonly property color buttonNotPressed: "#A0A0A0"

    readonly property int listItemHeight: 30
    readonly property int recipeDeleteButtonSize: 30
    readonly property int iconSize: 24
    readonly property int portionNumberSize: 40
    readonly property int recipeNameSize: 45
    readonly property int bottomBarSize: 50

    readonly property int fontSizeSmall: 10
    readonly property int fontSizeMedium: 12
    readonly property int fontTitle: 20
    readonly property int fontPortions: 20

    readonly property int smallSpacing: 5
    readonly property int bigSpacing: 10
    readonly property int bottomBarSpacing: 20
    readonly property int bottomBarButtonRadius: 20
    readonly property int bottomBarButtonBorderSize: 2
    readonly property int padding: 5
    readonly property int smallPadding: 4
    readonly property double margin: 5
}
