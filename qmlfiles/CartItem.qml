import Grocery 1.0
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item
{
    id:itemParent
    width: parent.width
    property var modelList 
    required property int index
    required property string name
    required property double quantity
    required property string unit
    required property bool checkedStatus
    property string itemNameString: name
    property string itemQuantityString: quantity===0?"":quantity.toString()
    property string units: unit
    property bool checkBoxVisible: true

    RowLayout
    {
        id: itemLayout
        width: itemParent.width
        spacing: AppTheme.smallSpacing
        height: parent.height
        property bool isEditing: false

        CheckBox
        {
            id: checkBox
            Layout.fillHeight: true
            Layout.preferredWidth: AppTheme.listItemHeight
            checked: checkedStatus
            visible: itemParent.checkBoxVisible
            onClicked:
            {
                modelList.set(index,ItemModelType.CheckedRole, checked);
                modelList.sort(0,Qt.AscendingOrder);
            }
        }

        TextField
        {
            id: itemName
            text: itemNameString
            font.pixelSize: AppTheme.fontSizeMedium
            leftPadding: AppTheme.padding
            rightPadding: AppTheme.padding
            selectByMouse: true
            Layout.fillWidth: true
            Layout.preferredHeight: AppTheme.listItemHeight
            placeholderText: qsTr("Item")
            verticalAlignment: Text.AlignVCenter
            onEditingFinished:
            {
                itemLayout.isEditing = false
                modelList.set(index,ItemModelType.NameRole, text)
            }
            Keys.onReturnPressed:
            {
                itemLayout.isEditing = false
                itemName.focus = false
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked: itemName.forceActiveFocus()
            }

        }
        TextField
        {
            id: itemQuantity
            text: itemQuantityString
            font.pixelSize: AppTheme.fontSizeMedium
            selectByMouse: true
            leftPadding: AppTheme.padding
            rightPadding: AppTheme.padding
            placeholderText: qsTr("#")
            validator: DoubleValidator
            {
                bottom: 0.0
                top: Infinity
                decimals: 1
                notation: DoubleValidator.StandardNotation
            }
            inputMethodHints: Qt.ImhFormattedNumbersOnly | Qt.ImhPreferNumbers
            horizontalAlignment: Text.AlignHCenter
            Layout.preferredWidth: 0.12*itemLayout.width
            Layout.preferredHeight: AppTheme.listItemHeight
            verticalAlignment: Text.AlignVCenter
            onEditingFinished:
            {
                itemLayout.isEditing = false
                modelList.set(index,ItemModelType.QuantityRole, text)
            }
            Keys.onReturnPressed:
            {
                itemLayout.isEditing = false
                itemQuantity.focus = false
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked: itemQuantity.forceActiveFocus()
            }

        }
        ComboBox
        {
            id: unitBox
            Layout.preferredWidth: 0.2*itemLayout.width
            Layout.preferredHeight: AppTheme.listItemHeight
            font.pixelSize: AppTheme.fontSizeSmall
            model: ["Grams","Units"]
            leftPadding: AppTheme.smallPadding
            rightPadding: AppTheme.smallPadding
            currentIndex: units==="Grams"?0:1

            delegate: ItemDelegate
            {
                width: parent.width
                height: 30
                contentItem: Text
                {
                    text: modelData
                    leftPadding: AppTheme.smallPadding
                    rightPadding: AppTheme.smallPadding
                    font.pixelSize: AppTheme.fontSizeMedium
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "white"
                }
            }
            contentItem: Label
            {
                text: unitBox.displayText
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                leftPadding: 0
                rightPadding: AppTheme.smallPadding
                font.pixelSize: AppTheme.fontSizeMedium
                onTextChanged: modelList.set(index,ItemModelType.UnitRole, unitBox.displayText)
            }

        }

        ToolButton
        {
            onClicked:
            {
                modelList.removeAt(index)
            }
            Layout.preferredWidth: AppTheme.listItemHeight
            Layout.preferredHeight: AppTheme.listItemHeight
            icon.source: "../icons/delete.svg"
            icon.width: AppTheme.iconSize
            icon.height: AppTheme.iconSize
            icon.color: "grey"
        }
    }
    Rectangle
    {
        anchors.fill: parent
        visible: checkedStatus
        color: "transparent"
        border.color: "transparent"
        // Draw a horizontal line in the middle
        Canvas
        {
            anchors.fill: parent
            onPaint: {
                var ctx = getContext("2d");
                ctx.clearRect(0, 0, width, height);
                ctx.beginPath();
                ctx.moveTo(itemName.x-2, itemName.y+itemName.height/2);
                ctx.lineTo(unitBox.x+unitBox.width+2, itemName.y+itemName.height/2);
                ctx.lineWidth = 2;
                ctx.strokeStyle = "grey";
                ctx.stroke();
            }
        }
        // Make sure MouseArea passes events through to underlying controls
        MouseArea
        {
            anchors.fill: parent
            acceptedButtons: Qt.NoButton
        }
    }

}
