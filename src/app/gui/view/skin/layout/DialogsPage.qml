import QtQuick 1.0
import com.nokia.symbian 1.0
import ru.curoviyxru.kutegram 1.0

Page {
    id: root

    //TODO: dialog filters
//    Flickable {
//        id: tabBarFlickable
//        anchors { left: parent.left; right: parent.right; top: parent.top }
//        height: tabBar.height
//        contentHeight: tabBar.height
//        contentWidth: tabBar.fullWidth
//        flickableDirection: Flickable.HorizontalFlick
//        boundsBehavior: Flickable.StopAtBounds
//        TabBarLayout {
//            id: tabBar
//            anchors { left: parent.left; top: parent.top; }
//            TabButton { text: "All chats"; checked: true }
//            TabButton { text: "All chats"; }
//            TabButton { text: "All chats"; }
//            TabButton { text: "All chats"; }
//            TabButton { text: "All chats"; }
//            TabButton { text: "All chats"; }
//        }
//    }

    ListView {
        id: dialogsView
        anchors {
            left: parent.left;
            right: parent.right;
            //top: tabBarFlickable.bottom;
            top: parent.top;
            bottom: parent.bottom
        }
        cacheBuffer: height
        clip: true
        focus: true

        model: DialogsListModel {
            id: dataModel
            client: telegram
        }

        delegate: ListItem {
            id: listItem
            Row {
                anchors.fill: listItem.paddingItem
                spacing: platformStyle.paddingMedium

                Image {
                    asynchronous: true
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    source: avatar
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    width: parent.height
                    height: parent.height

//                    BusyIndicator {
//                        running: parent.state == Image.Loading
//                        visible: parent.state == Image.Loading
//                        anchors.top: parent.top
//                        anchors.bottom: parent.bottom
//                        width: parent.width
//                        height: parent.height
//                    }
                }
                Column {
                    anchors.fill: parent.paddingItem
                    ListItemText {
                        id: titleText
                        anchors { left: parent.left; }
                        mode: listItem.mode
                        role: "Title"
                        text: title
                    }
                    ListItemText {
                        id: subtitleText
                        anchors { left: parent.left; }
                        mode: listItem.mode
                        role: "SubTitle"
                        text: message
                    }
                }
            }

            Component { id: historyPage; HistoryPage { peer: dialogId; } }

            onClicked: {
                pageStack.push(historyPage);
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

        ToolButton {
            flat: true
            iconSource: "toolbar-menu"
            onClicked: {
                optionsMenu.open();
            }

            Menu {
                id: optionsMenu
                content: MenuLayout {
                    MenuItem {
                        text: "Log out"
                        onClicked: {
                            pageStack.clear();
                            pageStack.push(introPage);
                            telegram.reset();
                        }
                    }
                    MenuItem {
                        text: "About"
                        onClicked: {
                            dialog.open();
                        }

                        CommonDialog {
                            id: dialog
                            titleText: "About"
                            titleIcon: "qrc:/icons/information_userguide.svg"
                            buttonTexts: ["OK"]
                            content: Flickable {
                                anchors.fill: parent
                                height: 200
                                contentHeight: dialogColumn.height

                                Column {
                                    id: dialogColumn
                                    anchors.fill: parent
                                    anchors.margins: platformStyle.paddingLarge
                                    height: 200

                                    ListItemText {
                                        anchors.left: parent.left
                                        anchors.right: parent.right
                                        id: dialogText
                                        role: "SubTitle"
                                        elide: Text.ElideNone
                                        wrapMode: Text.Wrap
                                        text: "Kutegram by curoviyxru\nAn unofficial Qt-based client for Telegram messenger.\nProject's website: http://kg.curoviyx.ru\nTelegram channel: https://t.me/kutegram\nTelegram chat: https://t.me/kutegramchat"
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    onStatusChanged: {
        if (status == PageStatus.Active) {
            dataModel.tryLoad();
        }
    }
}
