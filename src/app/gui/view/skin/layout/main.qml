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
                //TODO: do not allow to push twice
                if (!telegram.isLoggedIn()) pageStack.push(phonePage);
                break;
            case TelegramClient.LOGGED_IN:
                //TODO: do not allow to push twice
                pageStack.push(dialogsPage);
                break;
            }
        }
        onGotSentCode: {
            //TODO: do not allow to push twice
            if (!telegram.isLoggedIn()) pageStack.push(codePage);
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
