#include "dialogslistmodel.h"

#include <QMutexLocker>
#include "telegramclient.h"
#include "tlschema.h"

using namespace TLType;

DialogsListModel::DialogsListModel(QObject *parent) :
    QAbstractListModel(parent),
    _list(),
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
    roles[TitleRole] = "title";
    roles[AvatarRole] = "avatar";
    roles[MessageRole] = "message";
    roles[MessageTimeRole] = "messageTime";
    roles[UnreadCountRole] = "unreadCount";
    setRoleNames(roles);
}

int DialogsListModel::rowCount(const QModelIndex & parent) const {
    return _list.count();
}

QVariant DialogsListModel::data(const QModelIndex & index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() > _list.count())
        return QVariant();

    TObject item = _list[index.row()];
    switch (role) {
    case TitleRole:
        return item["title"];
    case AvatarRole:
        return item["avatar"];
    case MessageRole:
        return item["message"];
    case MessageTimeRole:
        return item["messageTime"];
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
    if (!_client || _gotFull || _lastRequestId) return;

    _lastRequestId = _client->getDialogs(_offsetDate, _offsetId, _offsetPeer, 20);
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
    else _gotFull |= d.isEmpty(); //TODO: (d.size() != 20);

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

            _list.append(prepareListItem(item));
        }

        endInsertRows();
    }
}

TObject DialogsListModel::prepareListItem(TObject d)
{
    TObject item;

    //TODO: prepare all data
    QString title;
    TObject peer = d["peer"].toMap();
    qint64 pid = getPeerId(peer).toLongLong();

    switch (ID(peer)) {
    case PeerChat:
    case PeerChannel:
        title = _chats[pid]["title"].toString();
        break;
    case PeerUser:
        title = _users[pid]["first_name"].toString() + " " + _users[pid]["last_name"].toString();
        break;
    }
    item["title"] = title;

    QString message;
    TObject m = _messages[d["top_message"].toInt()];

    if (!m["message"].toString().isEmpty())
        message = m["message"].toString();
    else if (GETID(m["action"].toMap()))
        message = m["action"].toString();
    else if (GETID(m["media"].toMap()))
        message = m["media"].toString();
    else
        message = QVariant(m).toString();
    item["message"] = message;

    return item;
}
