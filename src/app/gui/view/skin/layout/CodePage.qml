import QtQuick 1.0
import com.nokia.symbian 1.0

Page {
    id: root

    Column {
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 5

        TextField {
            id: codeField
            anchors.left: parent.left
            anchors.right: parent.right
            placeholderText: "Confirmation code"
        }

        Button {
            id: loginButton
            anchors.left: parent.left
            anchors.right: parent.right
            text: "Log in"
            onClicked: telegram.signIn(codeField.text);
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
