import QtQuick 1.0
import com.nokia.symbian 1.0

Page {
    id: root

    Column {
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 5

        Button {
            id: introButton
            anchors.left: parent.left
            anchors.right: parent.right
            text: "Start"
            onClicked: telegram.start()
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
