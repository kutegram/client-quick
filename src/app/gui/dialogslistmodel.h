#ifndef DIALOGSLISTMODEL_H
#define DIALOGSLISTMODEL_H

#include <QAbstractListModel>
#include <QMutex>
#include <QHash>
#include "telegramstream.h"

class TelegramClient;

class DialogsListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(TelegramClient* client READ client WRITE setClient)
    Q_ENUMS(ModelRole)
public:
    enum ModelRole {
        DialogIdRole = Qt::UserRole + 1,
        TitleRole,
        AvatarRole,
        MessageRole,
        TimestampRole,
        UnreadCountRole
    };

    explicit DialogsListModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    bool canFetchMore(const QModelIndex &parent) const;
    void fetchMore(const QModelIndex &parent);

    TelegramClient* client() const;
    void setClient(TelegramClient* client);
public slots:
    void tryLoad();
    void client_gotDialogs(qint64 mtm, qint32 count, TVector d, TVector m, TVector c, TVector u);
private:
    QList<QByteArray> _list;

    QHash<QByteArray, TObject> _items;

    QHash<qint32, TObject> _messages;
    QHash<qint64, TObject> _chats;
    QHash<qint64, TObject> _users;
    QList<TObject> _dialogs;

    TelegramClient* _client;
    QMutex _mutex;

    bool _gotFull;
    qint64 _lastRequestId;

    qint32 _offsetId;
    qint32 _offsetDate;
    TObject _offsetPeer;

    TObject prepareListItem(TObject d);
};

#endif // DIALOGSLISTMODEL_H
