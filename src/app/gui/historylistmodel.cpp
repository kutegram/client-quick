#include "historylistmodel.h"

#include <QMutexLocker>
#include <QtEndian>
#include <QStringList>
#include <QColor>
#include <QUrl>
#include <QDomDocument>
#include <QDomNodeList>
#include "telegramclient.h"
#include "tlschema.h"
#include "systemhandler.h"
#include "messageutils.h"
#include <QtGlobal>

using namespace TLType;

HistoryListModel::HistoryListModel(QObject *parent) :
    QAbstractListModel(parent),
    _list(),
    _peer(),
    _items(),
    _messages(),
    _chats(),
    _users(),
    _replies(),
    _client(),
    _mutex(QMutex::Recursive),
    _gotFull(),
    _lastRequestId(),
    _replyRequestId(),
    _offsetId(),
    _offsetDate(),
    _roles()
{
    _roles[MessageIdRole] = "messageId";
    _roles[TitleRole] = "title";
    _roles[AvatarRole] = "avatar";
    _roles[MessageRole] = "message";
    _roles[TimestampRole] = "timestamp";
    _roles[ReadStateRole] = "readState";
    _roles[AttachmentsRole] = "attachments";
    _roles[HasReplyRole] = "hasReply";
    _roles[ReplyTitleRole] = "replyTitle";
    _roles[ReplyMessageRole] = "replyMessage";
    _roles[ReplyColorRole] = "replyColor";
#if QT_VERSION < 0x050000
    setRoleNames(_roles);
#endif
}

QHash<int, QByteArray> HistoryListModel::roleNames() const
{
    return _roles;
}

int HistoryListModel::rowCount(const QModelIndex &parent) const
{
    return _list.count();
}

QVariant HistoryListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() > _list.count())
        return QVariant();

    TObject item = _items[_list[index.row()]];
    switch (role) {
    case MessageIdRole:
        return item["messageId"];
    case TitleRole:
        return item["title"];
    case AvatarRole:
        return item["avatar"];
    case MessageRole:
        return item["message"];
    case TimestampRole:
        return item["timestamp"];
    case ReadStateRole:
        return item["readState"];
    case AttachmentsRole:
        return item["attachments"];
    case HasReplyRole:
        return item["hasReply"];
    case ReplyTitleRole:
        return item["replyTitle"];
    case ReplyMessageRole:
        return item["replyMessage"];
    case ReplyColorRole:
        return item["replyColor"];
    }

    return QVariant();
}

bool HistoryListModel::canFetchMoreUpwards(const QModelIndex &parent) const
{
    return _client && ID(_peer) && !_gotFull && !_lastRequestId && !_replyRequestId && _client->apiReady();
}

void HistoryListModel::fetchMoreUpwards(const QModelIndex &parent)
{
    QMutexLocker locker(&_mutex);
    if (!canFetchMoreUpwards(parent)) return;

    _lastRequestId = _client->getHistory(_peer, _offsetId, _offsetDate, 0, 40);
}

TelegramClient* HistoryListModel::client() const
{
    return _client;
}

void HistoryListModel::setClient(TelegramClient *client)
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
    _replies.clear();
    _gotFull = 0;
    _lastRequestId = 0;
    _replyRequestId = 0;
    _offsetId = 0;
    _offsetDate = 0;

    _client = client;
    connect(_client, SIGNAL(gotMessages(qint64,qint32,TVector,TVector,TVector,qint32,qint32,bool)), this, SLOT(client_gotMessages(qint64,qint32,TVector,TVector,TVector,qint32,qint32,bool)));
    connect(_client, SIGNAL(updateNewMessage(TObject,qint32,qint32)), this, SLOT(client_updateNewMessage(TObject,qint32,qint32)));
    connect(_client, SIGNAL(updateEditMessage(TObject,qint32,qint32)), this, SLOT(client_updateEditMessage(TObject,qint32,qint32)));
    connect(_client, SIGNAL(updateDeleteMessages(TVector,qint32,qint32)), this, SLOT(client_updateDeleteMessages(TVector,qint32,qint32)));

    endResetModel();
    //tryLoadUpwards();
}

QByteArray HistoryListModel::peer() const
{
    return tlSerialize<&writeTLInputPeer>(_peer);
}

void HistoryListModel::setPeer(QByteArray peer)
{
    QMutexLocker locker(&_mutex);
    beginResetModel();

    _list.clear();
    _items.clear();
    _messages.clear();
    _chats.clear();
    _users.clear();
    _replies.clear();
    _gotFull = 0;
    _lastRequestId = 0;
    _replyRequestId = 0;
    _offsetId = 0;
    _offsetDate = 0;

    _peer = tlDeserialize<&readTLInputPeer>(peer).toMap();

    endResetModel();
    //tryLoadUpwards();
}

void HistoryListModel::tryLoadUpwards()
{
    if (canFetchMoreUpwards(QModelIndex())) fetchMoreUpwards(QModelIndex());
}

void HistoryListModel::client_gotMessages(qint64 mtm, qint32 count, TVector m, TVector c, TVector u, qint32 offsetIdOffset, qint32 nextRate, bool inexact)
{
    QMutexLocker locker(&_mutex);

    if (mtm == _lastRequestId) {
        gotHistoryMessages(mtm, count, m, c, u, offsetIdOffset, nextRate, inexact);
        _lastRequestId = 0;
    }
    else if (mtm == _replyRequestId) {
        gotReplyMessages(mtm, count, m, c, u, offsetIdOffset, nextRate, inexact);
        _replyRequestId = 0;
    }
}

void HistoryListModel::gotHistoryMessages(qint64 mtm, qint32 count, TVector m, TVector c, TVector u, qint32 offsetIdOffset, qint32 nextRate, bool inexact)
{
    _gotFull |= m.isEmpty();

    for (qint32 i = 0; i < c.size(); ++i) {
        TObject item = c[i].toMap();
        _chats.insert(item["id"].toLongLong(), item);
    }

    for (qint32 i = 0; i < u.size(); ++i) {
        TObject item = u[i].toMap();
        _users.insert(item["id"].toLongLong(), item);
    }

    for (qint32 i = 0; i < m.size(); ++i) {
        TObject item = m[i].toMap();
        _messages.insert(item["id"].toInt(), item);
    }

    //TODO: cache avatar thumbnails
    //TODO: cache attachments

    TVector requiredReplies;
    if (!m.isEmpty()) {
        emit loadingMessages();
        beginInsertRows(QModelIndex(), 0, m.size() - 1);

        for (qint32 i = 0; i < m.size(); ++i) {
            TObject item = m[i].toMap();
            qint32 msgId = item["id"].toInt();
            qint32 messageDate = item["date"].toInt();

            if (!_offsetDate || messageDate < _offsetDate) {
                _offsetDate = messageDate;
                _offsetId = msgId;
            }

            qint32 replyToId = item["reply_to"].toMap()["reply_to_msg_id"].toInt();
            if (ID(item["reply_to"].toMap()) && !ID(_messages[replyToId])) {
                requiredReplies.append(getInputMessage(item["reply_to"].toMap()));
                _replies.insert(replyToId, msgId);
            }

            _items.insert(msgId, prepareListItem(item));
            _list.insert(0, msgId);
        }

        endInsertRows();
        emit loadedMessages();
    }

    if (!requiredReplies.isEmpty() && _client)
        _replyRequestId = _client->getMessages(requiredReplies);
}

void HistoryListModel::gotReplyMessages(qint64 mtm, qint32 count, TVector m, TVector c, TVector u, qint32 offsetIdOffset, qint32 nextRate, bool inexact)
{
    for (qint32 i = 0; i < c.size(); ++i) {
        TObject item = c[i].toMap();
        _chats.insert(item["id"].toLongLong(), item);
    }

    for (qint32 i = 0; i < u.size(); ++i) {
        TObject item = u[i].toMap();
        _users.insert(item["id"].toLongLong(), item);
    }

    for (qint32 i = 0; i < m.size(); ++i) {
        TObject item = m[i].toMap();
        qint32 msgId = item["id"].toInt();
        _messages.insert(msgId, item);

        qint32 neededId = _replies[msgId];
        _replies.remove(msgId);

        _items.insert(neededId, prepareListItem(_messages[neededId]));

        qint32 arrayIndex = _list.indexOf(neededId);
        if (arrayIndex == -1) continue;
        emit dataChanged(index(arrayIndex), index(arrayIndex));
    }
}

void HistoryListModel::client_updateNewMessage(TObject msg, qint32 pts, qint32 pts_count)
{
    QMutexLocker locker(&_mutex);

    //TODO: test PTS sequence
    //TODO: load needed information

    if (!peersEqual(msg["peer_id"].toMap(), _peer)) return;

    qint32 msgId = msg["id"].toInt();
    _messages.insert(msgId, msg);

    emit addingMessage();
    beginInsertRows(QModelIndex(), _list.size(), _list.size());

    _items.insert(msgId, prepareListItem(msg));
    _list.append(msgId);

    endInsertRows();
    emit addedMessage();
}

void HistoryListModel::client_updateEditMessage(TObject msg, qint32 pts, qint32 pts_count)
{
    QMutexLocker locker(&_mutex);

    //TODO: test PTS sequence
    //TODO: load needed information

    if (!peersEqual(msg["peer_id"].toMap(), _peer)) return;

    qint32 msgId = msg["id"].toInt();
    _messages.insert(msgId, msg);

    _items.insert(msgId, prepareListItem(msg));

    qint32 arrayIndex = _list.indexOf(msgId);
    if (arrayIndex == -1) return;
    emit dataChanged(index(arrayIndex), index(arrayIndex));
}

void HistoryListModel::client_updateDeleteMessages(TVector msgs, qint32 pts, qint32 pts_count)
{
    QMutexLocker locker(&_mutex);

    //TODO: test PTS sequence
    //TODO: load needed information

    for (qint32 i = 0; i < msgs.size(); ++i) {
        qint32 msgId = msgs[i].toInt();

        qint32 arrayIndex = _list.indexOf(msgId);
        if (arrayIndex == -1) continue;

        beginRemoveRows(QModelIndex(), arrayIndex, arrayIndex);

        _replies.remove(_replies.key(msgId));
        _messages.remove(msgId);
        _items.remove(msgId);
        _list.removeAt(arrayIndex);

        endRemoveRows();
    }
}

TObject HistoryListModel::getMessagePeer(TObject m)
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

TObject HistoryListModel::prepareListItem(TObject m)
{
    //TODO: prepare all data
    TObject item;

    TObject peer = getMessagePeer(m);

    item["title"] = peerNameToHtml(peer);
    item["message"] = messageToHtml(m);
    item["messageId"] = m["id"].toInt();

    bool hasReply = ID(m["reply_to"].toMap());
    item["hasReply"] = hasReply;
    if (hasReply) {
        TObject replyMessage = _messages[m["reply_to"].toMap()["reply_to_msg_id"].toInt()];
        TObject replyPeer = getMessagePeer(replyMessage);
        item["replyColor"] = userColor(replyPeer["id"].toLongLong());
        item["replyTitle"] = peerNameToHtml(replyPeer);
        item["replyMessage"] = messageToHtml(replyMessage);
    }

    return item;
}

void HistoryListModel::sendMessage(QString text)
{
    if (_client && ID(_peer))
        _client->sendMessage(_peer, text);
}

void HistoryListModel::onLinkActivated(QString linkStr, qint32 messageId)
{
    QMutexLocker locker(&_mutex);

    QUrl link(linkStr);

    if (link.scheme() == "kutegram") {
        if (link.host() == "spoiler") {
            TObject listItem = _items[messageId];
            QDomDocument dom;
            dom.setContent(listItem["message"].toString(), false);

            QDomNodeList list = dom.elementsByTagName("a");
            for (qint32 i = 0; i < list.count(); ++i) {
                QDomElement node = list.at(i).toElement();
                if (node.attribute("href") == link.toString()) {
                    node.removeAttribute("href");
                    node.removeAttribute("style");
                    break;
                }
            }
            listItem["message"] = dom.toString(-1);
            _items.insert(messageId, listItem);

            qint32 arrayIndex = _list.indexOf(messageId);
            if (arrayIndex == -1) return;
            emit dataChanged(index(arrayIndex), index(arrayIndex));
        }
    }
    else openUrl(link);
}
