#include "gui/application.h"
#include <QtDeclarative>
#include <QTextCodec>
#include "telegramclient.h"
#include "dialogslistmodel.h"
#include "historylistmodel.h"
#include "systemhandler.h"

Q_DECL_EXPORT int main(int argc, char *argv[]) {
    gui::Application app(argc, argv);

    qmlRegisterType<TelegramClient>("ru.curoviyxru.kutegram", 1, 0, "TelegramClient");
    qmlRegisterType<DialogsListModel>("ru.curoviyxru.kutegram", 1, 0, "DialogsListModel");
    qmlRegisterType<HistoryListModel>("ru.curoviyxru.kutegram", 1, 0, "HistoryListModel");
    qmlRegisterType<SystemHandler>("ru.curoviyxru.kutegram", 1, 0, "SystemHandler");

    QApplication::setApplicationVersion(BUILD_VERSION);
    QApplication::setApplicationName("Kutegram");
    QApplication::setOrganizationName("curoviyxru");
    QApplication::setOrganizationDomain("kg.curoviyx.ru");

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);

    return app.run();
}
