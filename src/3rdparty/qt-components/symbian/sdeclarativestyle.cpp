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

#define QT_NO_CAST_FROM_ASCII
#define QT_NO_CAST_TO_ASCII

#include "sdeclarativestyle.h"
#include "sstyleengine.h"
#include "sdeclarative.h"
#include "sdeclarativescreen.h"

#include <QObject>

class SDeclarativeStylePrivate
{
public:
    SDeclarativeStylePrivate() {}

    SStyleEngine *engine;
};

SDeclarativeStyle::SDeclarativeStyle(SStyleEngine *engine, QObject *parent)
    : QObject(parent),
      d_ptr(new SDeclarativeStylePrivate())
{
    Q_D(SDeclarativeStyle);
    d->engine = engine;
    QObject::connect(engine, SIGNAL(fontParametersChanged()), this, SIGNAL(fontParametersChanged()));
    QObject::connect(engine, SIGNAL(layoutParametersChanged()), this, SIGNAL(layoutParametersChanged()));
    QObject::connect(engine, SIGNAL(colorParametersChanged()), this, SIGNAL(colorParametersChanged()));
}

SDeclarativeStyle::~SDeclarativeStyle()
{
}

QString SDeclarativeStyle::fontFamilyRegular() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->fontFamilyParameter(QLatin1String("font-family-regular"));
}

int SDeclarativeStyle::fontSizeLarge() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->layoutParameter(QLatin1String("font-size-large"));
}

int SDeclarativeStyle::fontSizeMedium() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->layoutParameter(QLatin1String("font-size-medium"));
}

int SDeclarativeStyle::fontSizeSmall() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->layoutParameter(QLatin1String("font-size-small"));
}

int SDeclarativeStyle::graphicSizeLarge() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->layoutParameter(QLatin1String("graphic-size-large"));
}

int SDeclarativeStyle::graphicSizeMedium() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->layoutParameter(QLatin1String("graphic-size-medium"));
}

int SDeclarativeStyle::graphicSizeSmall() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->layoutParameter(QLatin1String("graphic-size-small"));
}

int SDeclarativeStyle::graphicSizeTiny() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->layoutParameter(QLatin1String("graphic-size-tiny"));
}

int SDeclarativeStyle::paddingLarge() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->layoutParameter(QLatin1String("padding-large"));
}

int SDeclarativeStyle::paddingMedium() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->layoutParameter(QLatin1String("padding-medium"));
}

int SDeclarativeStyle::paddingSmall() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->layoutParameter(QLatin1String("padding-small"));
}

int SDeclarativeStyle::borderSizeMedium() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->layoutParameter(QLatin1String("border-size-medium"));
}

QColor SDeclarativeStyle::colorBackground() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-background"));
}

QColor SDeclarativeStyle::colorNormalLight() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-normal-light"));
}

QColor SDeclarativeStyle::colorNormalMid() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-normal-mid"));
}

QColor SDeclarativeStyle::colorNormalDark() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-normal-dark"));
}

QColor SDeclarativeStyle::colorNormalLink() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-normal-link"));
}

QColor SDeclarativeStyle::colorPressed() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-pressed"));
}

QColor SDeclarativeStyle::colorChecked() const
{
    qDebug() << "warning: SDeclarativeStyle::colorChecked() is deprecated, use colorLatched() instead";
    return colorLatched();
}

QColor SDeclarativeStyle::colorLatched() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-latched"));
}

QColor SDeclarativeStyle::colorHighlighted() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-highlighted"));
}

QColor SDeclarativeStyle::colorDisabledLight() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-disabled-light"));
}

QColor SDeclarativeStyle::colorDisabledMid() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-disabled-mid"));
}

QColor SDeclarativeStyle::colorDisabledDark() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-disabled-dark"));
}

QColor SDeclarativeStyle::colorTextSelection() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-text-selection"));
}

QColor SDeclarativeStyle::colorBackgroundInverted() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-background-inverse"));
}

QColor SDeclarativeStyle::colorNormalLightInverted() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-normal-light-inverse"));
}

QColor SDeclarativeStyle::colorNormalMidInverted() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-normal-mid-inverse"));
}

QColor SDeclarativeStyle::colorNormalDarkInverted() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-normal-dark-inverse"));
}

QColor SDeclarativeStyle::colorNormalLinkInverted() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-normal-link-inverse"));
}

QColor SDeclarativeStyle::colorPressedInverted() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-pressed-inverse"));
}

QColor SDeclarativeStyle::colorLatchedInverted() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-latched-inverse"));
}

QColor SDeclarativeStyle::colorHighlightedInverted() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-highlighted-inverse"));
}

QColor SDeclarativeStyle::colorDisabledLightInverted() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-disabled-light-inverse"));
}

QColor SDeclarativeStyle::colorDisabledMidInverted() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-disabled-mid-inverse"));
}

QColor SDeclarativeStyle::colorDisabledDarkInverted() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-disabled-dark-inverse"));
}

QColor SDeclarativeStyle::colorTextSelectionInverted() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-text-selection-inverse"));
}
