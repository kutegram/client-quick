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

#include "sdeclarativeindicatorcontainer.h"
#if defined(Q_OS_SYMBIAN) && defined(HAVE_SYMBIAN_INTERNAL)
#include "sdeclarativeindicatordatahandler.h"
#endif // Q_OS_SYMBIAN && HAVE_SYMBIAN_INTERNAL

class SDeclarativeIndicatorContainerPrivate
{
public:
    SDeclarativeIndicatorContainerPrivate();
    ~SDeclarativeIndicatorContainerPrivate();

#if defined(Q_OS_SYMBIAN) && defined(HAVE_SYMBIAN_INTERNAL)
    CSDeclarativeIndicatorDataHandler *dataHandler;
#else
    int *dataHandler; // dummy
#endif // Q_OS_SYMBIAN && HAVE_SYMBIAN_INTERNAL

    QColor indicatorColor;
    QSize indicatorSize;
    int indicatorPadding;
    int maxIndicatorCount;
    bool layoutRequestPending;

};

SDeclarativeIndicatorContainerPrivate::SDeclarativeIndicatorContainerPrivate()
    : dataHandler(0), layoutRequestPending(false)
{
}

SDeclarativeIndicatorContainerPrivate::~SDeclarativeIndicatorContainerPrivate()
{
    delete dataHandler;
}


SDeclarativeIndicatorContainer::SDeclarativeIndicatorContainer(QDeclarativeItem *parent)
    : SDeclarativeImplicitSizeItem(parent), d_ptr(new SDeclarativeIndicatorContainerPrivate)
{
#if defined(Q_OS_SYMBIAN) && defined(HAVE_SYMBIAN_INTERNAL)
    QT_TRAP_THROWING(d_ptr->dataHandler = CSDeclarativeIndicatorDataHandler::NewL(this));
#endif // Q_OS_SYMBIAN && HAVE_SYMBIAN_INTERNAL

    connect(this, SIGNAL(indicatorSizeChanged()), this, SLOT(layoutChildren()));
    connect(this, SIGNAL(indicatorPaddingChanged(int)), this, SLOT(layoutChildren()));
    connect(this, SIGNAL(maxIndicatorCountChanged(int)), this, SLOT(layoutChildren()));
}

SDeclarativeIndicatorContainer::~SDeclarativeIndicatorContainer()
{
}

QColor SDeclarativeIndicatorContainer::indicatorColor() const
{
    Q_D(const SDeclarativeIndicatorContainer);
    return d->indicatorColor;
}

void SDeclarativeIndicatorContainer::setIndicatorColor(const QColor &color)
{
    Q_D(SDeclarativeIndicatorContainer);
    if (d->indicatorColor != color) {
        d->indicatorColor = color;
        emit indicatorColorChanged(d->indicatorColor);
    }
}

int SDeclarativeIndicatorContainer::indicatorWidth() const
{
    Q_D(const SDeclarativeIndicatorContainer);
    return d->indicatorSize.width();
}

void SDeclarativeIndicatorContainer::setIndicatorWidth(int width)
{
    Q_D(SDeclarativeIndicatorContainer);
    if (d->indicatorSize.width() != width) {
        d->indicatorSize.setWidth(width);
        emit indicatorSizeChanged();
    }
}

int SDeclarativeIndicatorContainer::indicatorHeight() const
{
    Q_D(const SDeclarativeIndicatorContainer);
    return d->indicatorSize.height();
}

void SDeclarativeIndicatorContainer::setIndicatorHeight(int height)
{
    Q_D(SDeclarativeIndicatorContainer);
    if (d->indicatorSize.height() != height) {
        d->indicatorSize.setHeight(height);
        emit indicatorSizeChanged();
    }
}

int SDeclarativeIndicatorContainer::indicatorPadding() const
{
    Q_D(const SDeclarativeIndicatorContainer);
    return d->indicatorPadding;
}

void SDeclarativeIndicatorContainer::setIndicatorPadding(int padding)
{
    Q_D(SDeclarativeIndicatorContainer);
    if (d->indicatorPadding != padding) {
        d->indicatorPadding = padding;
        emit indicatorPaddingChanged(d->indicatorPadding);
    }
}

int SDeclarativeIndicatorContainer::maxIndicatorCount() const
{
    Q_D(const SDeclarativeIndicatorContainer);
    return d->maxIndicatorCount;
}

void SDeclarativeIndicatorContainer::setMaxIndicatorCount(int maxCount)
{
    Q_D(SDeclarativeIndicatorContainer);
    if (d->maxIndicatorCount != maxCount) {
        d->maxIndicatorCount = maxCount;
        emit maxIndicatorCountChanged(d->maxIndicatorCount);
    }
}

void SDeclarativeIndicatorContainer::layoutChildren()
{
    Q_D(SDeclarativeIndicatorContainer);
    if (!d->layoutRequestPending) {
        d->layoutRequestPending = true;
        QMetaObject::invokeMethod(this, "doLayoutChildren", Qt::QueuedConnection);
    }
}

void SDeclarativeIndicatorContainer::doLayoutChildren()
{
    Q_D(SDeclarativeIndicatorContainer);

    int xPosition = 0;
    int itemsShown = 0;
    const QSize itemSize(d->indicatorSize);

    for (int i = 0; i < childItems().count(); i++) {
        QDeclarativeItem *child = qobject_cast<QDeclarativeItem *>(childItems().at(i)->toGraphicsObject());
        if (child && child->isVisible()) {
            if (itemsShown >= d->maxIndicatorCount && d->maxIndicatorCount >= 0) {
                child->setSize(QSize(0, 0));
                continue;
            }

            if (itemsShown++)
                xPosition += d->indicatorPadding;

            child->setPos(xPosition, 0);
            child->setSize(itemSize);

            xPosition += child->width();
        }
    }

    setImplicitWidthNotify(xPosition);
    setImplicitHeightNotify(itemSize.height());
    d->layoutRequestPending = false;
}

