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
#ifndef SDECLARATIVEMAGNIFIER_H
#define SDECLARATIVEMAGNIFIER_H

#include <QtDeclarative/qdeclarativeitem.h>
#include <QtCore/qrect.h>
#include <QtGui/qpixmap.h>

class SDeclarativeMagnifierPrivate;

class SDeclarativeMagnifier : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(QRectF sourceRect READ sourceRect WRITE setSourceRect NOTIFY sourceRectChanged)
    Q_PROPERTY(qreal scaleFactor READ scaleFactor WRITE setScaleFactor NOTIFY scaleFactorChanged)
    Q_PROPERTY(QString overlayFileName READ overlayFileName WRITE setOverlayFileName NOTIFY overlayFileNameChanged)
    Q_PROPERTY(QString maskFileName READ maskFileName WRITE setMaskFileName NOTIFY maskFileNameChanged)

public:
    explicit SDeclarativeMagnifier(QDeclarativeItem *parent = 0);
    virtual ~SDeclarativeMagnifier();

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *styleOption, QWidget *widget);

    void setSourceRect(const QRectF &rect);
    QRectF sourceRect() const;

    void setScaleFactor(qreal scaleFactor);
    qreal scaleFactor() const;

    void setOverlayFileName(const QString &overlayFileName);
    QString overlayFileName() const;

    void setMaskFileName(const QString &maskFileName);
    QString maskFileName() const;

signals:
    void sourceRectChanged();
    void scaleFactorChanged();
    void overlayFileNameChanged();
    void maskFileNameChanged();

protected:
    QScopedPointer<SDeclarativeMagnifierPrivate> d_ptr;

private:
    Q_DISABLE_COPY(SDeclarativeMagnifier)
    Q_DECLARE_PRIVATE(SDeclarativeMagnifier)
};

#endif // SDECLARATIVEMAGNIFIER_H
