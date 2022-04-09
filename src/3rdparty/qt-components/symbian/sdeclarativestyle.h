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

#ifndef SDECLARATIVESTYLE_H
#define SDECLARATIVESTYLE_H

#include <QtCore/qobject.h>
#include <QtCore/qvariant.h>
#include <QtCore/qscopedpointer.h>
#include <QtGui/qcolor.h>

class SDeclarativeStylePrivate;
class SStyleEngine;

class SDeclarativeStyle : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString fontFamilyRegular READ fontFamilyRegular CONSTANT FINAL)

    Q_PROPERTY(int fontSizeLarge READ fontSizeLarge CONSTANT FINAL)
    Q_PROPERTY(int fontSizeMedium READ fontSizeMedium CONSTANT FINAL)
    Q_PROPERTY(int fontSizeSmall READ fontSizeSmall CONSTANT FINAL)
    Q_PROPERTY(int graphicSizeLarge READ graphicSizeLarge CONSTANT FINAL)
    Q_PROPERTY(int graphicSizeMedium READ graphicSizeMedium CONSTANT FINAL)
    Q_PROPERTY(int graphicSizeSmall READ graphicSizeSmall CONSTANT FINAL)
    Q_PROPERTY(int graphicSizeTiny READ graphicSizeTiny CONSTANT FINAL)
    Q_PROPERTY(int paddingLarge READ paddingLarge CONSTANT FINAL)
    Q_PROPERTY(int paddingMedium READ paddingMedium CONSTANT FINAL)
    Q_PROPERTY(int paddingSmall READ paddingSmall CONSTANT FINAL)
    Q_PROPERTY(int borderSizeMedium READ borderSizeMedium CONSTANT FINAL)

    Q_PROPERTY(QColor colorBackground READ colorBackground CONSTANT FINAL)
    Q_PROPERTY(QColor colorNormalLight READ colorNormalLight CONSTANT FINAL)
    Q_PROPERTY(QColor colorNormalMid READ colorNormalMid CONSTANT FINAL)
    Q_PROPERTY(QColor colorNormalDark READ colorNormalDark CONSTANT FINAL)
    Q_PROPERTY(QColor colorNormalLink READ colorNormalLink CONSTANT FINAL)
    Q_PROPERTY(QColor colorPressed READ colorPressed CONSTANT FINAL)
    Q_PROPERTY(QColor colorChecked READ colorChecked CONSTANT FINAL) // DEPRECATED - use colorChecked instead
    Q_PROPERTY(QColor colorLatched READ colorLatched CONSTANT FINAL)
    Q_PROPERTY(QColor colorHighlighted READ colorHighlighted CONSTANT FINAL)
    Q_PROPERTY(QColor colorDisabledLight READ colorDisabledLight CONSTANT FINAL)
    Q_PROPERTY(QColor colorDisabledMid READ colorDisabledMid CONSTANT FINAL)
    Q_PROPERTY(QColor colorDisabledDark READ colorDisabledDark CONSTANT FINAL)
    Q_PROPERTY(QColor colorTextSelection READ colorTextSelection CONSTANT FINAL)
    Q_PROPERTY(QColor colorBackgroundInverted READ colorBackgroundInverted CONSTANT FINAL)
    Q_PROPERTY(QColor colorNormalLightInverted READ colorNormalLightInverted CONSTANT FINAL)
    Q_PROPERTY(QColor colorNormalMidInverted READ colorNormalMidInverted CONSTANT FINAL)
    Q_PROPERTY(QColor colorNormalDarkInverted READ colorNormalDarkInverted CONSTANT FINAL)
    Q_PROPERTY(QColor colorNormalLinkInverted READ colorNormalLinkInverted CONSTANT FINAL)
    Q_PROPERTY(QColor colorPressedInverted READ colorPressedInverted CONSTANT FINAL)
    Q_PROPERTY(QColor colorLatchedInverted READ colorLatchedInverted CONSTANT FINAL)
    Q_PROPERTY(QColor colorHighlightedInverted READ colorHighlightedInverted CONSTANT FINAL)
    Q_PROPERTY(QColor colorDisabledLightInverted READ colorDisabledLightInverted CONSTANT FINAL)
    Q_PROPERTY(QColor colorDisabledMidInverted READ colorDisabledMidInverted CONSTANT FINAL)
    Q_PROPERTY(QColor colorDisabledDarkInverted READ colorDisabledDarkInverted CONSTANT FINAL)
    Q_PROPERTY(QColor colorTextSelectionInverted READ colorTextSelectionInverted CONSTANT FINAL)

public:

    explicit SDeclarativeStyle(SStyleEngine *engine, QObject *parent = 0);
    ~SDeclarativeStyle();

    QString fontFamilyRegular() const;

    int fontSizeLarge() const;
    int fontSizeMedium() const;
    int fontSizeSmall() const;
    int graphicSizeLarge() const;
    int graphicSizeMedium() const;
    int graphicSizeSmall() const;
    int graphicSizeTiny() const;
    int paddingLarge() const;
    int paddingMedium() const;
    int paddingSmall() const;
    int borderSizeMedium() const;

    QColor colorBackground() const;
    QColor colorNormalLight() const;
    QColor colorNormalMid() const;
    QColor colorNormalDark() const;
    QColor colorNormalLink() const;
    QColor colorPressed() const;
    QColor colorChecked() const;
    QColor colorLatched() const;
    QColor colorHighlighted() const;
    QColor colorDisabledLight() const;
    QColor colorDisabledMid() const;
    QColor colorDisabledDark() const;
    QColor colorTextSelection() const;
    QColor colorBackgroundInverted() const;
    QColor colorNormalLightInverted() const;
    QColor colorNormalMidInverted() const;
    QColor colorNormalDarkInverted() const;
    QColor colorNormalLinkInverted() const;
    QColor colorPressedInverted() const;
    QColor colorLatchedInverted() const;
    QColor colorHighlightedInverted() const;
    QColor colorDisabledLightInverted() const;
    QColor colorDisabledMidInverted() const;
    QColor colorDisabledDarkInverted() const;
    QColor colorTextSelectionInverted() const;

Q_SIGNALS:
    void fontParametersChanged();
    void layoutParametersChanged();
    void colorParametersChanged();

protected:
    QScopedPointer<SDeclarativeStylePrivate> d_ptr;

private:
    Q_DISABLE_COPY(SDeclarativeStyle)
    Q_DECLARE_PRIVATE(SDeclarativeStyle)
};

#endif // SDECLARATIVESTYLE_H
