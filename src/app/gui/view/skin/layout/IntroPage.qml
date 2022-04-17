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
                source: "qrc:/images/intro.png";
            }

            ListItemText {
                anchors.left: parent.left
                anchors.right: parent.right
                horizontalAlignment: Text.AlignHCenter
                role: "Title"
                elide: Text.ElideNone
                wrapMode: Text.Wrap
                text: "Kutegram"
            }

            ListItemText {
                anchors.left: parent.left
                anchors.right: parent.right
                horizontalAlignment: Text.AlignHCenter
                role: "SubTitle"
                elide: Text.ElideNone
                wrapMode: Text.Wrap
                text: "Unofficial client for Telegram messenger"
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

        Button {
            id: introButton
            anchors.left: parent.left
            anchors.right: parent.right
            text: "Start"
            onClicked: telegram.start()
            focus: true
        }
    }

    tools: ToolBarLayout {
        ToolButton {
            flat: true
            iconSource: "toolbar-back"
            onClicked: Qt.quit()
        }
    }
}
