#ifndef PEERPHOTOPROVIDER_H
#define PEERPHOTOPROVIDER_H

#include <QDeclarativeImageProvider>
#include <QHash>
#include <QImage>
#include "telegramclient.h"
#include <QPointer>

class PeerPhotoProvider : public QDeclarativeImageProvider
{
private:
    QHash<QString, QImage> _cache;
    QPointer<TelegramClient> _client;
    QImage _userIcon;
    QImage _chatIcon;
    QImage _channelIcon;
public:
    explicit PeerPhotoProvider(TelegramClient *client);
    virtual QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

    static QString getId(TObject peer);
signals:
    
public slots:
    
};

#endif // PEERPHOTOPROVIDER_H
