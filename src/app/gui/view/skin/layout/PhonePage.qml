import QtQuick 1.0
import com.nokia.symbian 1.0

Page {
    id: root

    Item {
        id: introColumn
        anchors.top: parent.top
        anchors.bottom: controlsColumn.top
        anchors.left: parent.left
        anchors.right: parent.right

        Column {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: platformStyle.paddingLarge
            spacing: platformStyle.paddingLarge

            Image {
                anchors.left: parent.left
                anchors.right: parent.right
                height: 160
                asynchronous: true
                fillMode: Image.PreserveAspectFit
                smooth: true
                source: "qrc:/images/phone.png";
            }

            ListItemText {
                anchors.left: parent.left
                anchors.right: parent.right
                horizontalAlignment: Text.AlignHCenter
                role: "Title"
                elide: Text.ElideNone
                wrapMode: Text.Wrap
                text: "Your phone number"
            }

            ListItemText {
                anchors.left: parent.left
                anchors.right: parent.right
                horizontalAlignment: Text.AlignHCenter
                role: "SubTitle"
                elide: Text.ElideNone
                wrapMode: Text.Wrap
                text: "Please enter your phone number in international format. (with plus in beginning and without spaces or dashes)"
            }
        }
    }

    Column {
        id: controlsColumn
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: platformStyle.paddingLarge
        spacing: platformStyle.paddingLarge

        focus: true

        TextField {
            id: phoneField
            anchors.left: parent.left
            anchors.right: parent.right
            placeholderText: "Phone number"
        }

        Button {
            id: sendButton
            anchors.left: parent.left
            anchors.right: parent.right
            text: "Send code"
            onClicked: telegram.sendCode(phoneField.text)
        }
    }

    tools: ToolBarLayout {
        ToolButton {
            flat: true
            iconSource: "toolbar-back"
            onClicked: pageStack.depth <= 1 ? Qt.quit() : pageStack.pop()
        }
    }
}
