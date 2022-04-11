import QtQuick 1.0
import com.nokia.symbian 1.0
import ru.curoviyxru.kutegram 1.0

Page {
    id: root

    TabBarLayout {
        id: tabBarLayout
        anchors { left: parent.left; right: parent.right; top: parent.top }
        TabButton { text: "All chats"; checked: true; }
    }

    ListView {
        id: dialogsView
        anchors { left: parent.left; right: parent.right; top: tabBarLayout.bottom; bottom: parent.bottom }
        cacheBuffer: height
        clip: true
        focus: true

        model: DialogsListModel {
            id: dataModel
            client: telegram
        }

        delegate: ListItem {
            id: listItem
            Column {
                anchors.fill: listItem.paddingItem
                ListItemText {
                    id: titleText
                    anchors { left: parent.left; right: parent.right; }
                    mode: listItem.mode
                    role: "Title"
                    text: title
                }
                ListItemText {
                    id: subtitleText
                    anchors { left: parent.left; right: parent.right; }
                    mode: listItem.mode
                    role: "SubTitle"
                    text: message
                }
            }

            Component { id: historyPage; HistoryPage { peer: dialogId; } }

            onClicked: {
                pageStack.push(historyPage);
            }
        }

        onMovementEnded: {
            if (dialogsView.atYEnd) {
                dataModel.tryLoad();
            }
        }

        ScrollDecorator {
            flickableItem: parent
        }
    }

    tools: ToolBarLayout {
        ToolButton {
            flat: true
            iconSource: "toolbar-back"
            onClicked: Qt.quit()
        }
    }

    onStatusChanged: {
        if (status == PageStatus.Active) {
            dataModel.tryLoad();
        }
    }
}
