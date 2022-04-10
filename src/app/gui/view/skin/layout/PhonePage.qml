import QtQuick 1.0
import com.nokia.symbian 1.0

Page {
    id: root

    Column {
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 5

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
