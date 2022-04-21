#include "systemhandler.h"

#include <QDesktopServices>
#include <QApplication>

#if defined(Q_OS_SYMBIAN) && defined(SYMBIAN3_READY)
#include <akndiscreetpopup.h> // CAknDiscreetPopup
#include <avkon.hrh> // KAknDiscreetPopupDurationLong

#include <eikenv.h> // CEikonEnv
#include <apgcli.h> // RApaLsSession
#include <apgtask.h> // TApaTaskList, TApaTask

#include <akndiscreetpopup.h>
#include <aknnotewrappers.h>
#include <aknglobalnote.h>
#include <AknCommonDialogsDynMem.h>
#include <aknglobalmsgquery.h>
#include <hwrmlight.h>
#include <e32svr.h>
#include <eikmenup.h>
#include <coemain.h>
#include <MGFetch.h>
#include <coreapplicationuisdomainpskeys.h>
#include <e32property.h>
#endif

SystemHandler::SystemHandler(QObject *parent) :
    QObject(parent), client(static_cast<TelegramClient*>(parent))
{
    connect(client.data(), SIGNAL(updateNewMessage(TObject,qint32,qint32)), this, SLOT(client_updateNewMessage(TObject,qint32,qint32)));

    showChatIcon();
}

void showChatIcon()
{
    //TODO: QSystemTrayIcon
#if defined(Q_OS_SYMBIAN) && defined(SYMBIAN3_READY)
    static bool chatIconStatus = false;
    if (!chatIconStatus) {
        RProperty iProperty;
        iProperty.Set(KPSUidCoreApplicationUIs, KCoreAppUIsUipInd, ECoreAppUIsShow);
        chatIconStatus = true;
    }
#endif
}

void hideChatIcon()
{
    //TODO: QSystemTrayIcon
#if defined(Q_OS_SYMBIAN) && defined(SYMBIAN3_READY)
    RProperty iProperty;
    iProperty.Set(KPSUidCoreApplicationUIs, KCoreAppUIsUipInd, ECoreAppUIsDoNotShow);
#endif
}

void showNotification(QString title, QString message)
{
    //TODO: silent mode
    //TODO: LED blink
    //TODO: sound
    //TODO: vibro
    //TODO: only 1 message in 5 seconds
    //TODO: check if app minimized
    //TODO: icon
    //TODO: desktop
#if defined(Q_OS_SYMBIAN) && defined(SYMBIAN3_READY)
    //static TUid symbianUid = {SYMBIAN_UID};
    TPtrC16 sTitle(reinterpret_cast<const TUint16*>(title.utf16()));
    TPtrC16 sMessage(reinterpret_cast<const TUint16*>(message.utf16()));
    TRAP_IGNORE(CAknDiscreetPopup::ShowGlobalPopupL(sTitle, sMessage, KAknsIIDNone, KNullDesC, 0, 0, KAknDiscreetPopupDurationLong, 0, NULL)); //, symbianUid
#endif
}

void openUrl(const QUrl &url)
{
#if defined(Q_OS_SYMBIAN) && defined(SYMBIAN3_READY)
    static TUid KUidBrowser = {0x10008D39};
    _LIT(KBrowserPrefix, "4 " );

    // convert url to encoded version of QString
    QString encUrl(QString::fromUtf8(url.toEncoded()));
    // using qt_QString2TPtrC() based on
    // <http://qt.gitorious.org/qt/qt/blobs/4.7/src/corelib/kernel/qcore_symbian_p.h#line102>
    TPtrC tUrl(TPtrC16(static_cast<const TUint16*>(encUrl.utf16()), encUrl.length()));

    // Following code based on
    // <http://www.developer.nokia.com/Community/Wiki/Launch_default_web_browser_using_Symbian_C%2B%2B>

    // create a session with apparc server
    RApaLsSession appArcSession;
    User::LeaveIfError(appArcSession.Connect());
    CleanupClosePushL<RApaLsSession>(appArcSession);

    // get the default application uid for application/x-web-browse
    TDataType mimeDatatype(_L8("application/x-web-browse"));
    TUid handlerUID;
    appArcSession.AppForDataType(mimeDatatype, handlerUID);

    // if UiD not found, use the native browser
    if (handlerUID.iUid == 0 || handlerUID.iUid == -1)
        handlerUID = KUidBrowser;

    // Following code based on
    // <http://qt.gitorious.org/qt/qt/blobs/4.7/src/gui/util/qdesktopservices_s60.cpp#line213>

    HBufC* buf16 = HBufC::NewLC(tUrl.Length() + KBrowserPrefix.iTypeLength);
    buf16->Des().Copy(KBrowserPrefix); // Prefix used to launch correct browser view
    buf16->Des().Append(tUrl);

    TApaTaskList taskList(CCoeEnv::Static()->WsSession());
    TApaTask task = taskList.FindApp(handlerUID);
    if (task.Exists()) {
        // Switch to existing browser instance
        task.BringToForeground();
        HBufC8* param8 = HBufC8::NewLC(buf16->Length());
        param8->Des().Append(buf16->Des());
        task.SendMessage(TUid::Uid( 0 ), *param8); // Uid is not used
        CleanupStack::PopAndDestroy(param8);
    } else {
        // Start a new browser instance
        TThreadId id;
        appArcSession.StartDocument(*buf16, handlerUID, id);
    }

    CleanupStack::PopAndDestroy(buf16);
    CleanupStack::PopAndDestroy(&appArcSession);
#else
    QDesktopServices::openUrl(url);
#endif
}

void SystemHandler::client_updateNewMessage(TObject msg, qint32 pts, qint32 pts_count)
{
    //TODO: improve it
    if (msg["out"].toBool()) return;

    showNotification(QApplication::translate("MainWindow", "New message"),
                msg["message"].toString().mid(0, 40));
}
