#ifndef SYSTEMHANDLER_H
#define SYSTEMHANDLER_H

#include <QObject>
#include <QPointer>
#include <QUrl>
#include "telegramclient.h"

void showChatIcon();
void hideChatIcon();
void openUrl(const QUrl &url);
void showNotification(QString title, QString message);

class SystemHandler : public QObject
{
    Q_OBJECT
private:
    QPointer<TelegramClient> client;
public:
    explicit SystemHandler(QObject *parent = 0);

signals:

public slots:
    void client_updateNewMessage(TObject message, qint32 pts, qint32 pts_count);
};

#endif // SYSTEMHANDLER_H
