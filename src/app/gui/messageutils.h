#ifndef MESSAGEUTILS_H
#define MESSAGEUTILS_H

#include <QString>
#include <QColor>
#include "telegramstream.h"

QString messageToHtml(TObject message, bool dialog = false);
QColor userColor(qint64 id);
QString getPeerName(TObject peer);
QString peerNameToHtml(TObject peer, bool dialog = false);

#endif // MESSAGEUTILS_H
