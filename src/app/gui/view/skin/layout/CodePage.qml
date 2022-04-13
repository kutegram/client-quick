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
                source: "qrc:/images/code.png";
            }

            ListItemText {
                anchors.left: parent.left
                anchors.right: parent.right
                horizontalAlignment: Text.AlignHCenter
                role: "Title"
                elide: Text.ElideNone
                wrapMode: Text.Wrap
                text: "Confirmation code"
            }

            ListItemText {
                anchors.left: parent.left
                anchors.right: parent.right
                horizontalAlignment: Text.AlignHCenter
                role: "SubTitle"
                elide: Text.ElideNone
                wrapMode: Text.Wrap
                text: "Please enter your confirmation code to log in."
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
            text: "Confirm"
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
