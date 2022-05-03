#ifndef HISTORYLISTMODEL_H
#define HISTORYLISTMODEL_H

#include <QAbstractListModel>
#include <QMutex>
#include <QHash>
#include "telegramstream.h"
#include "telegramclient.h"
#include <QPointer>

class HistoryListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(TelegramClient* client READ client WRITE setClient)
    Q_PROPERTY(QByteArray peer READ peer WRITE setPeer)
    Q_ENUMS(ModelRole)
public:
    enum ModelRole {
        MessageIdRole = Qt::UserRole + 1,
        TitleRole,
        AvatarRole,
        MessageRole,
        TimestampRole,
        ReadStateRole,
        AttachmentsRole,
        HasReplyRole,
        ReplyTitleRole,
        ReplyMessageRole,
        ReplyColorRole
    };

    explicit HistoryListModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    bool canFetchMoreUpwards(const QModelIndex &parent) const;
    void fetchMoreUpwards(const QModelIndex &parent);

    TelegramClient* client() const;
    void setClient(TelegramClient* client);

    QByteArray peer() const;
    void setPeer(QByteArray peer);

    void gotHistoryMessages(qint64 mtm, qint32 count, TVector m, TVector c, TVector u, qint32 offsetIdOffset, qint32 nextRate, bool inexact);
    void gotReplyMessages(qint64 mtm, qint32 count, TVector m, TVector c, TVector u, qint32 offsetIdOffset, qint32 nextRate, bool inexact);
signals:
    void loadingMessages();
    void loadedMessages();
    void addingMessage();
    void addedMessage();
public slots:
    void tryLoadUpwards();
    void client_gotMessages(qint64 mtm, qint32 count, TVector m, TVector c, TVector u, qint32 offsetIdOffset, qint32 nextRate, bool inexact);
    void client_updateNewMessage(TObject message, qint32 pts, qint32 pts_count);
    void client_updateEditMessage(TObject message, qint32 pts, qint32 pts_count);
    void client_updateDeleteMessages(TVector messages, qint32 pts, qint32 pts_count);
    void onLinkActivated(QString link, qint32 messageId);
    void sendMessage(QString text);
private:
    QList<qint32> _list;
    TObject _peer;

    QHash<qint32, TObject> _items;

    QHash<qint32, TObject> _messages;
    QHash<qint64, TObject> _chats;
    QHash<qint64, TObject> _users;
    QHash<qint32, qint32> _replies;

    QPointer<TelegramClient> _client;
    QMutex _mutex;

    bool _gotFull;
    qint64 _lastRequestId;
    qint64 _replyRequestId;

    qint32 _offsetId;
    qint32 _offsetDate;

    TObject prepareListItem(TObject m);
    TObject getMessagePeer(TObject m);
};

#endif // HISTORYLISTMODEL_H
