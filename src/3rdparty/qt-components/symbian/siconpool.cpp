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

#include "siconpool.h"

#include <QPainter>
#include <QSvgRenderer>
#include <QPixmap>
#include <QSizeF>
#include <QHash>

//#define Q_DEBUG_ICON
#ifdef Q_DEBUG_ICON
#include <QDebug>
#endif

/*!
  \class SIconPoolKey
 */
struct SIconPoolKey {
public:
    SIconPoolKey(const QString &fileName, const QSize &size, Qt::AspectRatioMode mode, const QColor &color)
        : mFilename(fileName), mSize(size), mMode(mode), mColor(color) {}

    bool operator==(const SIconPoolKey &other) const {
        return other.mFilename == mFilename && other.mSize == mSize && other.mMode == mMode && other.mColor == mColor;
    }

    QString mFilename;
    QSize mSize;
    Qt::AspectRatioMode mMode;
    QColor mColor;
};

/*!
  \class SIconPoolValue
 */
struct SIconPoolValue {
    SIconPoolValue()
        : mPixmap(), mRefCount(0) {}
    SIconPoolValue(const QPixmap &pixmap)
        : mPixmap(pixmap), mRefCount(1) {}
    QPixmap mPixmap;
    int mRefCount;
};

uint qHash(const SIconPoolKey &key)
{
    return qHash(key.mFilename);
}

typedef QHash<SIconPoolKey, SIconPoolValue> SIconPoolData;
Q_GLOBAL_STATIC(SIconPoolData, poolData);

QPixmap SIconPool::get(const QString &fileName,
                       const QSize &size,
                       Qt::AspectRatioMode mode,
                       const QColor &color)
{
    QPixmap pixmap;

    // Size (-n,-n) uses icon's default size (where (-n) is a negative number).

    // Sizes (-n,x) and (x,-n) use the defined dimension (x) and scale the
    // undefined dimension (-n) keeping the aspect ratio.

    // Notice: QML seems to prevent setting (-1,x) or (x,-1) as a size.
    // You can, however, use (-2,x) or (x, -2). There's a "undefined"
    // constant (Symbian.UndefinedSourceDimension) defined in Symbian
    // namespace that can be used for this purpose.

    // If icon width or height is zero, can simply return default constructed QPixmap
    if (!fileName.isEmpty() && size.width() && size.height()) {
        SIconPoolKey key(fileName, size, mode, color);
        SIconPoolData *pool = poolData();

        if (pool->contains(key)) {
            SIconPoolValue value = pool->value(key);
            ++value.mRefCount;
            pixmap = value.mPixmap;
            pool->insert(key, value);
        } else {
            pixmap = loadIcon(fileName, size, mode, color);
            pool->insert(key, SIconPoolValue(pixmap));
        }
#ifdef Q_DEBUG_ICON
        qDebug() << "SIconPool::get()" << key.mFilename << pool->value(key).mRefCount;
#endif
    }
    return pixmap;
}

QPixmap SIconPool::loadIcon(
    const QString &fileName,
    const QSize &size,
    Qt::AspectRatioMode mode,
    const QColor &color)
{
    QPixmap pm;
    // SVG? Use QSvgRenderer
    if (fileName.endsWith(".svg")) {
        QSvgRenderer *renderer = getSvgRenderer(fileName);

        if (renderer->isValid()) {
            QSize renderSize = renderer->defaultSize();

            // If given size is valid, scale default size to it using the given aspect ratio mode
            if (size.isValid()) {
                renderSize.scale(size, mode);

            // If only one dimension is valid, scale other dimension keeping the aspect ratio
            } else if (size.height() > 0) {
                Qt::AspectRatioMode scaleMode = size.height() > renderSize.height()
                    ? Qt::KeepAspectRatioByExpanding
                    : Qt::KeepAspectRatio;
                renderSize.scale(renderSize.width(), size.height(), scaleMode);
            } else if (size.width() > 0) {
                Qt::AspectRatioMode scaleMode = size.width() > renderSize.width()
                    ? Qt::KeepAspectRatioByExpanding
                    : Qt::KeepAspectRatio;
                renderSize.scale(size.width(), renderSize.height(), scaleMode);
            }
            //  Otherwise (-1,-1) was given as size, leave renderSize as icon's default size

            pm = QPixmap(renderSize);
            pm.fill(QColor(0, 0, 0, 0));
            QPainter painter(&pm);
            renderer->render(&painter, QRectF(QPointF(), renderSize));
        }
    } else {
        // Otherwise load with QPixmap
        pm.load(fileName);
        if (!pm.isNull()) {
            pm = pm.scaled(size, mode, Qt::SmoothTransformation);
        }
    }

    if (!pm.isNull() && color.isValid()) {
        // Colorize the icon
        QPixmap mask = pm.alphaChannel();
        pm.fill(color);
        pm.setAlphaChannel(mask);
    }
#ifdef Q_DEBUG_ICON
    if (pm.isNull()) {
        qDebug() << "Fail to load icon: " << filename;
    }
#endif

    return pm;
}

void SIconPool::release(
    const QString &fileName,
    const QSize &size,
    Qt::AspectRatioMode mode,
    const QColor &color)
{
    SIconPoolKey key(fileName, size, mode, color);
    SIconPoolData *pool = poolData();
    if (pool->contains(key)) {
        SIconPoolValue value = pool->value(key);
        // Delete if last reference
        if (!--value.mRefCount) {
            pool->remove(key);
        } else {
            // Update dereferenced value in pool
            pool->insert(key, value);
        }

#ifdef Q_DEBUG_ICON
        qDebug() << "SIconPool::release()" << key.mFilename << value.mRefCount;
#endif
    }
}

void SIconPool::releaseAll() {
    poolData()->clear();
}

QSize SIconPool::defaultSize(const QString &fileName)
{
    QSize defSize;

    // Get the default size from svg renderer or pixmap size
    if (!fileName.isEmpty()) {
        // SVG? Use QSvgRenderer
        if (fileName.endsWith(".svg")) {
            QSvgRenderer *svgRenderer = getSvgRenderer(fileName);
            if (svgRenderer->isValid()) {
                defSize = svgRenderer->defaultSize();
            }
        } else {
            // Otherwise load with QPixmap
            QPixmap pixmap;
            pixmap.load(fileName);
            defSize = pixmap.size();
        }
    }

    return defSize;
}

QSvgRenderer *SIconPool::getSvgRenderer(const QString &fileName)
{
    static QString lastSvgFileName;
    static QSvgRenderer *lastSvgRenderer = 0;

    if (lastSvgFileName == fileName)
        return lastSvgRenderer;

    delete lastSvgRenderer;
    lastSvgRenderer = new QSvgRenderer(fileName);
    lastSvgFileName = fileName;
    return lastSvgRenderer;
}

#ifdef ICON_POOL_UNIT_TEST

int SIconPool::totalCount()
{
    SIconPoolData *pool = poolData();
    return pool->count();
}

int SIconPool::count(const QString &fileName, const QSize &size, Qt::AspectRatioMode mode, const QColor &color)
{
    SIconPoolKey key(fileName, size, mode, color);
    SIconPoolData *pool = poolData();
    if (pool->contains(key)) {
        SIconPoolValue value = pool->value(key);
        return value.mRefCount;
    } else {
        return 0;
    }
}

#endif // ICON_POOL_UNIT_TEST
