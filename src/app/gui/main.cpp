#include "gui/application.h"
#include <QtDeclarative>
#include <QTextCodec>
#include "telegramclient.h"
#include "dialogslistmodel.h"

Q_DECL_EXPORT int main(int argc, char *argv[]) {
    demo::gui::Application app(argc, argv);

    qmlRegisterType<TelegramClient>("ru.curoviyxru.kutegram", 1, 0, "TelegramClient");
    qmlRegisterType<DialogsListModel>("ru.curoviyxru.kutegram", 1, 0, "DialogsListModel");

    QApplication::setApplicationVersion("0.1.0");
    QApplication::setApplicationName("Kutegram");
    QApplication::setOrganizationName("curoviyxru");
    QApplication::setOrganizationDomain("kg.curoviyx.ru");

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);

    return app.run();
}
