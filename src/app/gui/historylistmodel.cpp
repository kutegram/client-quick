#include "historylistmodel.h"

#include <QMutexLocker>
#include <QtEndian>
#include <QStringList>
#include <QColor>
#include "telegramclient.h"
#include "tlschema.h"

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
    _offsetDate()
{
    QHash<int, QByteArray> roles;
    roles[MessageIdRole] = "messageId";
    roles[TitleRole] = "title";
    roles[AvatarRole] = "avatar";
    roles[MessageRole] = "message";
    roles[TimestampRole] = "timestamp";
    roles[ReadStateRole] = "readState";
    roles[AttachmentsRole] = "attachments";
    setRoleNames(roles);
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

    beginInsertRows(QModelIndex(), _list.size(), _list.size());

    _items.insert(msgId, prepareListItem(msg));
    _list.append(msgId);

    endInsertRows();
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

QString messageToHtml(TObject message)
{
    if (ID(message) != Message) {
        qWarning() << "[messageToHtml] Invalid object." << ID(message);
        return QString();
    }

    QStringList items;
    QList<char> count;

    QString originalText = message["message"].toString();
    items << QString(originalText).replace('<', "&lt;").replace('>', "&gt;") + ' ';
    count << true;
    TVector entities = message["entities"].toList();

    for (qint32 i = 0; i < entities.size(); ++i) {
        TObject entity = entities[i].toMap();
        qint32 offset = entity["offset"].toInt();
        qint32 length = entity["length"].toInt();
        QString textPart = originalText.mid(offset, length);

        QString sTag;
        QString eTag;

        switch (ID(entity)) {
        case MessageEntityUnknown: //just ignore it.
            //sTag = "<unknown>";
            //eTag = "</unknown>";
            break;

        case MessageEntityMention:
            sTag = "<a href=\"kutegram://profile/" + textPart + "\">";
            eTag = "</a>";
            break;
        case MessageEntityHashtag:
            sTag = "<a href=\"kutegram://search/" + textPart + "\">";
            eTag = "</a>";
            break;
        case MessageEntityBotCommand:
            sTag = "<a href=\"kutegram://execute/" + textPart + "\">";
            eTag = "</a>";
            break;
        case MessageEntityUrl:
            sTag = "<a href=\"" + textPart + "\">";
            eTag = "</a>";
            break;
        case MessageEntityEmail:
            sTag = "<a href=\"mailto:" + textPart + "\">";
            eTag = "</a>";
            break;

        case MessageEntityBold:
            sTag = "<b>";
            eTag = "</b>";
            break;
        case MessageEntityItalic:
            sTag = "<i>";
            eTag = "</i>";
            break;
        case MessageEntityCode:
            sTag = "<code>";
            eTag = "</code>";
            break;
        case MessageEntityPre: //pre causes some issues with paddings
            sTag = "<code language=\"" + entity["language"].toString() + "\">";
            eTag = "</code>";
            break;

        case MessageEntityTextUrl:
            sTag = "<a href=\"" + entity["url"].toString() + "\">";
            eTag = "</a>";
            break;
        case MessageEntityMentionName:
            sTag = "<a href=\"kutegram://profile/" + entity["user_id"].toString() + "\">";
            eTag = "</a>";
            break;
        case InputMessageEntityMentionName:
            sTag = "<a href=\"kutegram://profile/" + getPeerId(entity["user_id"].toMap()).toString() + "\">";
            eTag = "</a>";
            break;
        case MessageEntityPhone:
            sTag = "<a href=\"tel:" + textPart + "\">";
            eTag = "</a>";
            break;
        case MessageEntityCashtag:
            sTag = "<a href=\"kutegram://search/" + textPart + "\">";
            eTag = "</a>";
            break;

        case MessageEntityUnderline:
            sTag = "<u>";
            eTag = "</u>";
            break;
        case MessageEntityStrike:
            sTag = "<s>";
            eTag = "</s>";
            break;
        case MessageEntityBlockquote:
            sTag = "<blockquote>";
            eTag = "</blockquote>";
            break;

        case MessageEntityBankCard:
            sTag = "<a href=\"kutegram://card/" + textPart + "\">";
            eTag = "</a>";
            break;

        case MessageEntitySpoiler:
            sTag = "<a href=\"kutegram://spoiler/" + QString::number(i) + "\" style=\"background-color:gray;color:gray;\">";
            eTag = "</a>";
            break;
        }

        qint32 posStart = 0;
        qint32 jidStart = -1;
        qint32 posEnd = 0;
        qint32 jidEnd = -1;
        for (qint32 j = 0; j < items.size(); ++j) {
            if (!count[j]) continue;

            posStart += items[j].length();
            posEnd += items[j].length();
            if (posStart > offset && jidStart == -1) {
                posStart = items[j].length() - posStart + offset;
                jidStart = j;
            }
            if (posEnd > offset + length && jidEnd == -1) {
                posEnd = items[j].length() - posEnd + offset + length;
                jidEnd = j;
            }
            if (jidStart != -1 && jidEnd != -1) break;
        }

        if (jidStart == -1 || jidEnd == -1) continue;

        QString sItem = items[jidStart];
        items[jidStart] = sItem.mid(0, posStart);
        items.insert(jidStart + 1, sTag);
        count.insert(jidStart + 1, false);
        items.insert(jidStart + 2, sItem.mid(posStart));
        count.insert(jidStart + 2, true);

        if (jidEnd == jidStart) posEnd -= posStart;
        jidEnd += 2;

        QString eItem = items[jidEnd];
        items[jidEnd] = eItem.mid(0, posEnd);
        items.insert(jidEnd + 1, eTag);
        count.insert(jidEnd + 1, false);
        items.insert(jidEnd + 2, eItem.mid(posEnd));
        count.insert(jidEnd + 2, true);
    }

    if (items.last() == " ") items.removeLast();
    else items.last().chop(1);

    return items.join("").replace('\n', "<br>");
}

QColor userColor(qint64 id)
{
   return QColor::fromHsl(id % 360, 140, 140);
}

QString peerNameToHtml(TObject peer)
{
    QString peerName;

    switch (ID(peer)) {
    case TLType::UserEmpty:
    case TLType::User:
        peerName = peer["first_name"].toString() + " " + peer["last_name"].toString();
        break;
    default:
        //this is a chat, probably.
        peerName = peer["title"].toString();
        break;
    }

    peerName = peerName.mid(0, 40) + (peerName.length() > 40 ? "..." : "");

    return "<span style=\"font-weight:bold;color:" + userColor(peer["id"].toLongLong()).name() + "\">" + peerName + "</span>";
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

    return item;
}

void HistoryListModel::sendMessage(QString text)
{
    if (_client && ID(_peer))
        _client->sendMessage(_peer, text);
}
