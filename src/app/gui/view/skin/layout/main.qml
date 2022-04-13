import QtQuick 1.0
import com.nokia.symbian 1.0
import ru.curoviyxru.kutegram 1.0

ApplicationWindow {
    //TODO: fix ToolBar hiding bug
    id: root

    TelegramClient {
        id: telegram
    }

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
    }

    Component.onCompleted: {
        if (!telegram.isLoggedIn()) {
            pageStack.push(introPage);
        }
        else {
            telegram.start();
        }
    }
}
