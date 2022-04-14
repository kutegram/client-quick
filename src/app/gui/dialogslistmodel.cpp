#include "dialogslistmodel.h"

#include <QMutexLocker>
#include <QtEndian>
#include "telegramclient.h"
#include "tlschema.h"
#include "messageutils.h"

using namespace TLType;

DialogsListModel::DialogsListModel(QObject *parent) :
    QAbstractListModel(parent),
    _list(),
    _items(),
    _messages(),
    _chats(),
    _users(),
    _dialogs(),
    _client(),
    _mutex(QMutex::Recursive),
    _gotFull(),
    _lastRequestId(),
    _offsetId(),
    _offsetDate(),
    _offsetPeer()
{
    QHash<int, QByteArray> roles;
    roles[DialogIdRole] = "dialogId";
    roles[TitleRole] = "title";
    roles[AvatarRole] = "avatar";
    roles[MessageRole] = "message";
    roles[TimestampRole] = "timestamp";
    roles[UnreadCountRole] = "unreadCount";
    setRoleNames(roles);
}

QByteArray getKey(TObject peer)
{
    QByteArray idArray(sizeof(qint64) + sizeof(qint32), 0);
    qToLittleEndian(getPeerId(peer).toLongLong(), (uchar*) idArray.data());
    qToLittleEndian(ID(peer), (uchar*) idArray.data() + sizeof(qint64));
    return idArray;
}

int DialogsListModel::rowCount(const QModelIndex &parent) const {
    return _list.count();
}

QVariant DialogsListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() > _list.count())
        return QVariant();

    TObject item = _items[_list[index.row()]];
    switch (role) {
    case DialogIdRole:
        return item["dialogId"];
    case TitleRole:
        return item["title"];
    case AvatarRole:
        return item["avatar"];
    case MessageRole:
        return item["message"];
    case TimestampRole:
        return item["timestamp"];
    case UnreadCountRole:
        return item["unreadCount"];
    }

    return QVariant();
}

bool DialogsListModel::canFetchMore(const QModelIndex &parent) const
{
    return _client && !_gotFull && !_lastRequestId && _client->apiReady();
}

//TODO: fix some duplicate results
void DialogsListModel::fetchMore(const QModelIndex &parent)
{
    QMutexLocker locker(&_mutex);
    if (!canFetchMore(parent)) return;

    _lastRequestId = _client->getDialogs(_offsetDate, _offsetId, _offsetPeer, 40);
}

TelegramClient* DialogsListModel::client() const
{
    return _client;
}

void DialogsListModel::setClient(TelegramClient *client)
{
    QMutexLocker locker(&_mutex);
    beginResetModel();

    if (_client)
        disconnect(_client, 0, this, 0);

    _list.clear();
    _items.clear();
    _messages.clear();
    _chats.clear();
    _users.clear();
    _dialogs.clear();
    _gotFull = 0;
    _lastRequestId = 0;
    _offsetId = 0;
    _offsetDate = 0;
    _offsetPeer = TObject();

    _client = client;
    connect(_client, SIGNAL(gotDialogs(qint64,qint32,TVector,TVector,TVector,TVector)), this, SLOT(client_gotDialogs(qint64,qint32,TVector,TVector,TVector,TVector)));

    endResetModel();
    //tryLoad();
}

void DialogsListModel::tryLoad()
{
    if (canFetchMore(QModelIndex())) fetchMore(QModelIndex());
}

void DialogsListModel::client_gotDialogs(qint64 mtm, qint32 count, TVector d, TVector m, TVector c, TVector u)
{
    QMutexLocker locker(&_mutex);
    if (!_lastRequestId || _lastRequestId != mtm)
        return;

    _lastRequestId = 0;

    if (!count) _gotFull = true;
    else _gotFull |= d.isEmpty();

    for (qint32 i = 0; i < m.size(); ++i) {
        TObject item = m[i].toMap();
        _messages.insert(item["id"].toInt(), item);
    }

    for (qint32 i = 0; i < c.size(); ++i) {
        TObject item = c[i].toMap();
        _chats.insert(item["id"].toLongLong(), item);
    }

    for (qint32 i = 0; i < u.size(); ++i) {
        TObject item = u[i].toMap();
        _users.insert(item["id"].toLongLong(), item);
    }

    //TODO: Cache thumbnails and request avatars for dialogs only

    if (!d.isEmpty()) {
        beginInsertRows(QModelIndex(), _list.size(), _list.size() + d.size() - 1);

        for (qint32 i = 0; i < d.size(); ++i) {
            TObject item = d[i].toMap();
            _dialogs.append(item);

            TObject topMessage = _messages[item["top_message"].toInt()];
            if (!_offsetDate || topMessage["date"].toInt() < _offsetDate) {
                _offsetDate = topMessage["date"].toInt();
                _offsetId = topMessage["id"].toInt();

                switch (ID(topMessage["peer_id"].toMap())) {
                case PeerChat:
                case PeerChannel:
                    _offsetPeer = getInputPeer(_chats[getPeerId(topMessage["peer_id"].toMap()).toLongLong()]);
                    break;
                case PeerUser:
                    _offsetPeer = getInputPeer(_users[getPeerId(topMessage["peer_id"].toMap()).toLongLong()]);
                    break;
                }
            }

            QByteArray key = getKey(item["peer"].toMap());
            _items.insert(key, prepareListItem(item));
            _list.append(key);
        }

        endInsertRows();
    }
}

TObject DialogsListModel::getMessagePeer(TObject m)
{
    TObject from = m["from_id"].toMap();
    if (!ID(from))
        from = m["peer_id"].toMap();

    switch (ID(from)) {
    case PeerChannel:
    case PeerChat:
        from = _chats[getPeerId(from).toLongLong()];
        break;
    case PeerUser:
        from = _users[getPeerId(from).toLongLong()];
        break;
    }

    return from;
}

TObject DialogsListModel::prepareListItem(TObject d)
{
    //TODO: prepare all data
    TObject item;

    TObject peer = d["peer"].toMap();
    qint64 pid = getPeerId(peer).toLongLong();
    switch (ID(peer)) {
    case PeerChat:
    case PeerChannel:
        item["title"] = _chats[pid]["title"].toString();
        item["dialogId"] = tlSerialize<&writeTLInputPeer>(getInputPeer(_chats[pid]));
        break;
    case PeerUser:
        item["title"] = QString(_users[pid]["first_name"].toString() + " " + _users[pid]["last_name"].toString());
        item["dialogId"] = tlSerialize<&writeTLInputPeer>(getInputPeer(_users[pid]));
        break;
    }

    TObject m = _messages[d["top_message"].toInt()];
    TObject mp = getMessagePeer(m);
    item["message"] = QString(peerNameToHtml(mp, true) + " " + messageToHtml(m, true));

    return item;
}
