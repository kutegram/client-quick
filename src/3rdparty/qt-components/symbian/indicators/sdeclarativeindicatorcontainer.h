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

#ifndef SDECLARATIVEINDICATORCONTAINER_H
#define SDECLARATIVEINDICATORCONTAINER_H

#include "sdeclarativeimplicitsizeitem.h"

class SDeclarativeIndicatorContainerPrivate;

class SDeclarativeIndicatorContainer : public SDeclarativeImplicitSizeItem
{
    Q_OBJECT
    Q_PROPERTY(QColor indicatorColor READ indicatorColor WRITE setIndicatorColor NOTIFY indicatorColorChanged)
    Q_PROPERTY(int indicatorWidth READ indicatorWidth WRITE setIndicatorWidth NOTIFY indicatorSizeChanged)
    Q_PROPERTY(int indicatorHeight READ indicatorHeight WRITE setIndicatorHeight NOTIFY indicatorSizeChanged)
    Q_PROPERTY(int indicatorPadding READ indicatorPadding WRITE setIndicatorPadding NOTIFY indicatorPaddingChanged)
    Q_PROPERTY(int maxIndicatorCount READ maxIndicatorCount WRITE setMaxIndicatorCount NOTIFY maxIndicatorCountChanged)


public:
    SDeclarativeIndicatorContainer(QDeclarativeItem *parent = 0);
    ~SDeclarativeIndicatorContainer();

    QColor indicatorColor() const;
    void setIndicatorColor(const QColor &color);

    int indicatorWidth() const;
    void setIndicatorWidth(int width);

    int indicatorHeight() const;
    void setIndicatorHeight(int height);

    int indicatorPadding() const;
    void setIndicatorPadding(int padding);

    int maxIndicatorCount() const;
    void setMaxIndicatorCount(int maxCount);

public slots:
    void layoutChildren();

signals:
    void indicatorColorChanged(const QColor &color);
    void indicatorSizeChanged();
    void indicatorPaddingChanged(int padding);
    void maxIndicatorCountChanged(int maxCount);

protected:
    QScopedPointer<SDeclarativeIndicatorContainerPrivate> d_ptr;

private:
    Q_DISABLE_COPY(SDeclarativeIndicatorContainer)
    Q_DECLARE_PRIVATE(SDeclarativeIndicatorContainer)

private slots:
    void doLayoutChildren();
};

QML_DECLARE_TYPE(SDeclarativeIndicatorContainer)

#endif // SDECLARATIVEINDICATORCONTAINER_H

