/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include "sdeclarativeicon.h"
#include "siconpool.h"
#include "sdeclarative.h"

#include <QCoreApplication>
#include <QPainter>
#include <QGraphicsScene>
#include <QSvgRenderer>
#include <QPixmap>
#include <QSizeF>
#include <QFileInfo>

//#define Q_DEBUG_ICON
#ifdef Q_DEBUG_ICON
#include <QDebug>
#endif

class SDeclarativeIconPrivate
{
public:
    SDeclarativeIconPrivate() :
        iconLoadAttempted(false) {}

    void releaseFromIconPool();

public:
    QPixmap pixmap;
    QString iconName;
    QString fileName;
    QColor iconColor;
    bool iconLoadAttempted;
    QSize loadedSize;
};

void SDeclarativeIconPrivate::releaseFromIconPool()
{
    // Release previously loaded pixmap from icon pool
    SIconPool::release(fileName, loadedSize, Qt::KeepAspectRatio, iconColor);
    pixmap = QPixmap();
    iconLoadAttempted = false;
}

SDeclarativeIcon::SDeclarativeIcon(QDeclarativeItem *parent) :
    QDeclarativeItem(parent),
    d_ptr(new SDeclarativeIconPrivate)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
}

SDeclarativeIcon::~SDeclarativeIcon()
{
    Q_D(SDeclarativeIcon);
    // Release from icon pool
    if (d->iconLoadAttempted)
        d->releaseFromIconPool();
}

QString SDeclarativeIcon::iconName() const
{
    Q_D(const SDeclarativeIcon);
    return d->iconName;
}

void SDeclarativeIcon::setIconName(const QString &name)
{
    Q_D(SDeclarativeIcon);
    if (d->iconName != name) {

        if (d->iconLoadAttempted) {
            // Release previous one from icon pool
            d->releaseFromIconPool();
        }

        QUrl url(name);
        QFileInfo fileInfo(name);
        QString tmpFileName = SDeclarative::resolveIconFileName(fileInfo.fileName());

        // SDeclarative::resolveIconFileName did nothing, icon not part of qt-components icon set
        if (tmpFileName == fileInfo.fileName())
            // remove "/" what comes from QUrl::path()
            tmpFileName = url.path().remove(0,1);

        // If icon is in resource file use :/ to search it from both applications & qt-components resource file
        if (url.scheme() == "qrc" && !tmpFileName.startsWith(QLatin1String(":/")))
            tmpFileName.prepend(QLatin1String(":/"));

        d->fileName = tmpFileName;
        d->iconName = name;
        update();
        emit iconNameChanged(name);
    }
}

QColor SDeclarativeIcon::iconColor() const
{
    Q_D(const SDeclarativeIcon);
    return d->iconColor;
}

void SDeclarativeIcon::setIconColor(const QColor &color)
{
    Q_D(SDeclarativeIcon);
    if (d->iconColor != color) {
        if (d->iconLoadAttempted) {
            // Release previous one from icon pool
            d->releaseFromIconPool();
        }

        d->iconColor = color;

        update();
        emit iconColorChanged(color);
    }
}

void SDeclarativeIcon::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    Q_D(SDeclarativeIcon);
    QSize rectSize(width(), height());

    if (rectSize.isEmpty())
        return;

    // If the rect size has changed, icon needs to be reloaded
    if (d->iconLoadAttempted && d->loadedSize != rectSize)
        d->releaseFromIconPool();

    // Load pixmap now that rect size is known
    if (!d->iconLoadAttempted && !d->fileName.isEmpty()) {
        // Get pixmap from application's icon pool
        d->pixmap = SIconPool::get(d->fileName, rectSize, Qt::KeepAspectRatio, d->iconColor);
        d->iconLoadAttempted = true;
        d->loadedSize = rectSize;
    }

    // Center pixmap rect
    QRect rect(QPoint(0, 0), d->pixmap.size());
    // Round to nearest integer pixel
    rect.moveCenter(QPoint(width() / 2 + 0.5, height() / 2 + 0.5));

    // Give only topleft coordinates - no pixmap scaling when drawing
    painter->drawPixmap(rect.topLeft().x(), rect.topLeft().y(), d->pixmap);

#ifdef Q_DEBUG_ICON
    qDebug() << "SDeclarativeIcon::paint";
    qDebug() << "iconname = " << iconName();
    qDebug() << "width = " << width();
    qDebug() << "height = " << height();
#endif
}
