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

#ifndef SDECLARATIVEMASKEDIMAGE_H
#define SDECLARATIVEMASKEDIMAGE_H

#include <QtDeclarative/qdeclarativeitem.h>

class SDeclarativeMaskedImagePrivate;

class SDeclarativeMaskedImage : public QDeclarativeItem
{
    Q_OBJECT

public:
    Q_PROPERTY(QString imageName READ imageName WRITE setImageName)
    Q_PROPERTY(QString maskName READ maskName WRITE setMaskName)
    Q_PROPERTY(int topMargin READ topMargin WRITE setTopMargin)
    Q_PROPERTY(int bottomMargin READ bottomMargin WRITE setBottomMargin)
    Q_PROPERTY(int leftMargin READ leftMargin WRITE setLeftMargin)
    Q_PROPERTY(int rightMargin READ rightMargin WRITE setRightMargin)
    Q_PROPERTY(bool tiled READ isTiled WRITE setTiled)
    Q_PROPERTY(QPoint offset READ offset WRITE setOffset)

public:
    explicit SDeclarativeMaskedImage(QDeclarativeItem *parent = 0);
    virtual ~SDeclarativeMaskedImage();

    QString imageName() const;
    void setImageName(const QString &name);

    QString maskName() const;
    void setMaskName(const QString &name);

    QPoint offset() const;
    void setOffset(const QPoint &offset);

    bool isTiled() const;
    void setTiled(bool tiled);

    int topMargin() const;
    void setTopMargin(int margin);

    int bottomMargin() const;
    void setBottomMargin(int margin);

    int leftMargin() const;
    void setLeftMargin(int margin);

    int rightMargin() const;
    void setRightMargin(int margin);

    virtual void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);

protected:
    QScopedPointer<SDeclarativeMaskedImagePrivate> d_ptr;

private:
    Q_DISABLE_COPY(SDeclarativeMaskedImage)
    Q_DECLARE_PRIVATE(SDeclarativeMaskedImage)
};

QML_DECLARE_TYPE(SDeclarativeMaskedImage)

#endif // SDECLARATIVEMASKEDIMAGE_H
