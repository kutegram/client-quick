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

#include <qpainter.h>
#include <qapplication.h>
#include <qbitmap.h>
#include <qmargins.h>

#include "sdeclarativemaskedimage.h"
#include "sdeclarativemaskedimage_p.h"
#include "siconpool.h"
#include "sdeclarative.h"

SDeclarativeMaskedImagePrivate::~SDeclarativeMaskedImagePrivate()
{
    releasePixmaps();
}

// Assumes that the bounding rect of this item has been set
void SDeclarativeMaskedImagePrivate::createTiledPixmap()
{
    Q_Q(SDeclarativeMaskedImage);

    const int width = static_cast<int>(q->width());
    const int height = static_cast<int>(q->height());

    IconParams params;
    params.fileName = SDeclarative::resolveIconFileName(imageName);
    params.size = QSize(width, height);

    QPixmap tile = SIconPool::get(params.fileName, params.size);
    tileSize = tile.width();

    pixmap = QPixmap(width, height);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    // Fill using tiling
    painter.fillRect(QRect(0, 0, width, height), QBrush(tile));
}

// Assumes that the bounding rect of this item has been set
void SDeclarativeMaskedImagePrivate::createNonTiledPixmap()
{
    Q_Q(SDeclarativeMaskedImage);

    IconParams params;
    params.fileName = SDeclarative::resolveIconFileName(imageName);
    params.size = QSize(q->width(), q->height());

    pixmap = SIconPool::get(params.fileName, params.size);
    requestedFromIconPool.append(params);
    tileSize = pixmap.width();
}

// Assumes that the bounding rect of this item has been set
void SDeclarativeMaskedImagePrivate::createMask()
{
    Q_Q(SDeclarativeMaskedImage);

    const int width = static_cast<int>(q->width());
    const int height = static_cast<int>(q->height());

    // paint the mask from its frame graphics to the mask pixmap
    mask = QBitmap(width, height);
    mask.fill(Qt::white);
    QPainter painter(&mask);

    // If margins are set, draw the mask like QML BorderImage draws the image.
    if (topMargin >= 0 && bottomMargin >= 0 && leftMargin >= 0 && rightMargin >= 0) {
        IconParams params;
        params.fileName = SDeclarative::resolveIconFileName(maskName);
        params.size = QSize();

        QPixmap mask = SIconPool::get(params.fileName, params.size);
        requestedFromIconPool.append(params);

        QMargins margins(leftMargin, topMargin, rightMargin, bottomMargin);
        QTileRules rules(Qt::StretchTile, Qt::StretchTile);
        qDrawBorderPixmap(&painter, QRect(0, 0, width, height), margins, mask, mask.rect(), margins, rules);
    }

    painter.end();
}

void SDeclarativeMaskedImagePrivate::releasePixmaps()
{
    foreach(const IconParams &params, requestedFromIconPool)
        SIconPool::release(params.fileName, params.size);

    requestedFromIconPool.clear();
    pixmapsCreated = false;
}

SDeclarativeMaskedImage::SDeclarativeMaskedImage(QDeclarativeItem *parent) :
    QDeclarativeItem(parent), d_ptr(new SDeclarativeMaskedImagePrivate(this))
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
}

SDeclarativeMaskedImage::~SDeclarativeMaskedImage()
{
}

QString SDeclarativeMaskedImage::imageName() const
{
    Q_D(const SDeclarativeMaskedImage);
    return d->imageName;
}

void SDeclarativeMaskedImage::setImageName(const QString &name)
{
    Q_D(SDeclarativeMaskedImage);
    if (d->imageName != name) {
        d->imageName = name;
        d->releasePixmaps();
    }
}

QString SDeclarativeMaskedImage::maskName() const
{
    Q_D(const SDeclarativeMaskedImage);
    return d->maskName;
}

void SDeclarativeMaskedImage::setMaskName(const QString &name)
{
    Q_D(SDeclarativeMaskedImage);
    if (d->maskName != name) {
        d->maskName = name;
        d->releasePixmaps();
    }
}

QPoint SDeclarativeMaskedImage::offset() const
{
    Q_D(const SDeclarativeMaskedImage);
    return d->offset;
}

void SDeclarativeMaskedImage::setOffset(const QPoint &offset)
{
    Q_D(SDeclarativeMaskedImage);
    if (d->offset != offset) {
        d->offset = offset;
        update();
    }
}

bool SDeclarativeMaskedImage::isTiled() const
{
    Q_D(const SDeclarativeMaskedImage);
    return d->tiled;
}

void SDeclarativeMaskedImage::setTiled(bool tiled)
{
    Q_D(SDeclarativeMaskedImage);
    if (tiled != d->tiled) {
        d->tiled = tiled;
        update();
    }
}

int SDeclarativeMaskedImage::topMargin() const
{
    Q_D(const SDeclarativeMaskedImage);
    return d->topMargin;
}

void SDeclarativeMaskedImage::setTopMargin(int margin)
{
    Q_D(SDeclarativeMaskedImage);
    if (margin != d->topMargin) {
        d->topMargin = margin;
        d->releasePixmaps();
        update();
    }
}

int SDeclarativeMaskedImage::bottomMargin() const
{
    Q_D(const SDeclarativeMaskedImage);
    return d->bottomMargin;
}

void SDeclarativeMaskedImage::setBottomMargin(int margin)
{
    Q_D(SDeclarativeMaskedImage);
    if (margin != d->bottomMargin) {
        d->bottomMargin = margin;
        d->releasePixmaps();
        update();
    }
}

int SDeclarativeMaskedImage::leftMargin() const
{
    Q_D(const SDeclarativeMaskedImage);
    return d->leftMargin;
}

void SDeclarativeMaskedImage::setLeftMargin(int margin)
{
    Q_D(SDeclarativeMaskedImage);
    if (margin != d->leftMargin) {
        d->leftMargin = margin;
        d->releasePixmaps();
        update();
    }
}

int SDeclarativeMaskedImage::rightMargin() const
{
    Q_D(const SDeclarativeMaskedImage);
    return d->rightMargin;
}

void SDeclarativeMaskedImage::setRightMargin(int margin)
{
    Q_D(SDeclarativeMaskedImage);
    if (margin != d->rightMargin) {
        d->rightMargin = margin;
        d->releasePixmaps();
        update();
    }
}

void SDeclarativeMaskedImage::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    Q_D(SDeclarativeMaskedImage);
    QSize rectSize(width(), height());

    // If size has changed, need to recreate pixmaps
    if (d->createdSize != rectSize)
        d->releasePixmaps();

    // Create pixmap and mask if not done yet
    if (!d->pixmapsCreated) {
        if (d->tiled)
            d->createTiledPixmap();
        else
            d->createNonTiledPixmap();

        d->createMask();
        d->pixmapsCreated = true;
        d->createdSize = rectSize;
    }

    // draw the frame masked with the mask
    QPixmap imageFrame(rectSize);
    imageFrame.fill(Qt::transparent);
    QPainter p(&imageFrame);

    int offsetX = QApplication::layoutDirection() == Qt::RightToLeft ?
        d->tileSize + d->offset.x() : -d->offset.x();

    p.drawPixmap(QPointF(0, 0), d->pixmap, QRectF(QPointF(offsetX, -d->offset.y()), rectSize));
    p.end(); // Can't set mask when painter's active

    // Set the mask in the frame
    if (!d->mask.isNull())
        imageFrame.setMask(d->mask);

    // Update the frame in the screen
    painter->drawPixmap(QPointF(0, 0), imageFrame);
}
