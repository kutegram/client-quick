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

#ifndef SDECLARATIVEICON_H
#define SDECLARATIVEICON_H

#include <QtCore/qscopedpointer.h>
#include <QtDeclarative/qdeclarativeitem.h>

class SDeclarativeIconPrivate;

class SDeclarativeIcon : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(QString iconName READ iconName WRITE setIconName NOTIFY iconNameChanged)
    Q_PROPERTY(QColor iconColor READ iconColor WRITE setIconColor NOTIFY iconColorChanged)

public:
    explicit SDeclarativeIcon(QDeclarativeItem *parent = 0);
    virtual ~SDeclarativeIcon();

    QString iconName() const;
    void setIconName(const QString &name);

    QColor iconColor() const;
    void setIconColor(const QColor &color);

    virtual void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);

Q_SIGNALS:
    void iconNameChanged(const QString &name);
    void iconColorChanged(const QColor &color);

protected:
    QScopedPointer<SDeclarativeIconPrivate> d_ptr;

private:
    Q_DISABLE_COPY(SDeclarativeIcon)
    Q_DECLARE_PRIVATE(SDeclarativeIcon)
};

QML_DECLARE_TYPE(SDeclarativeIcon)

#endif // SDECLARATIVEICON_H
