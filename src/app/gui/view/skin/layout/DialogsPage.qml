import QtQuick 1.0
import com.nokia.symbian 1.0
import ru.curoviyxru.kutegram 1.0

Page {
    id: root

    TabBarLayout {
        id: tabBarLayout
        anchors { left: parent.left; right: parent.right; top: parent.top }
        TabButton { tab: tab1content; text: "All chats" }
    }

    TabGroup {
        id: tabGroup
        anchors { left: parent.left; right: parent.right; top: tabBarLayout.bottom; bottom: parent.bottom }

        Page {
            id: tab1content
            anchors.fill: parent
            ListView {
                id: dialogsView
                anchors.fill: parent
                cacheBuffer: height
                clip: true
                model: DialogsListModel {
                    id: dataModel
                    client: telegram
                }
                delegate: ListItem {
                    id: listItem
                    clip: true

                    Column {
                        anchors.fill: listItem.paddingItem
                        ListItemText {
                            id: titleText
                            mode: listItem.mode
                            role: "Title"
                            text: title
                        }
                        ListItemText {
                            id: subtitleText
                            mode: listItem.mode
                            role: "SubTitle"
                            text: message
                        }
                    }
                }
                onMovementEnded: {
                    if (dialogsView.atYEnd) {
                        dataModel.tryLoad();
                    }
                }
            }
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
