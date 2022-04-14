import QtQuick 1.0
import com.nokia.symbian 1.0
import ru.curoviyxru.kutegram 1.0

ApplicationWindow {
    id: root

    Component { id: introPage; IntroPage {} }
    Component { id: phonePage; PhonePage {} }
    Component { id: codePage; CodePage {} }
    Component { id: dialogsPage; DialogsPage {} }

    Connections {
        target: telegram
        onStateChanged: {
            switch (state) {
            case TelegramClient.INITED:
                if (pageStack.depth == 1 && !telegram.isLoggedIn())
                    pageStack.push(phonePage);
                break;
            case TelegramClient.LOGGED_IN:
                if (pageStack.depth == 0 || pageStack.depth == 3)
                    pageStack.push(dialogsPage);
                break;
            }
        }
        onGotSentCode: {
            if (pageStack.depth == 2 && !telegram.isLoggedIn())
                pageStack.push(codePage);
        }
        onGotSocketError: {
            if (dialog.status == DialogStatus.Open || dialog.status == DialogStatus.Opening)
                return;
            dialogText.text = "Got socket error: " + error + ".\nPlease, check your Internet connection and restart the application.";
            dialog.open();
        }
        onGotMTError: {
            if (dialog.status == DialogStatus.Open || dialog.status == DialogStatus.Opening)
                return;
            dialogText.text = "Got MT error: " + error_code + ".\nPlease, report it to developer.";
            dialog.open();
        }
        onGotDHError: {
            if (dialog.status == DialogStatus.Open || dialog.status == DialogStatus.Opening)
                return;
            dialogText.text = "Got DH error: " + (fail ? 1 : 0) + ".\nRestart the application and try to log in again.";
            dialog.open();
        }
    }

    Component.onCompleted: {
        if (!telegram.isLoggedIn()) {
            pageStack.push(introPage);
        }
        else {
            telegram.start();
        }
    }

    CommonDialog {
        id: dialog
        titleText: "Error"
        titleIcon: "qrc:/icons/error.svg"
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
                    text: "Error message"
                }
            }
        }
    }
}
