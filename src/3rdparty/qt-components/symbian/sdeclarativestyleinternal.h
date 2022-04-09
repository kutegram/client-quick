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

#ifndef SDECLARATIVESTYLEINTERNAL_H
#define SDECLARATIVESTYLEINTERNAL_H

#include <QtCore/qobject.h>
#include <QtCore/qvariant.h>
#include <QtCore/qscopedpointer.h>
#include <QtGui/qcolor.h>
#include <QUrl>

class SDeclarativeStyleInternalPrivate;
class SStyleEngine;

class SDeclarativeStyleInternal : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int statusBarHeight READ statusBarHeight CONSTANT FINAL)
    Q_PROPERTY(int tabBarHeightPortrait READ tabBarHeightPortrait CONSTANT FINAL)
    Q_PROPERTY(int tabBarHeightLandscape READ tabBarHeightLandscape CONSTANT FINAL)
    Q_PROPERTY(int toolBarHeightPortrait READ toolBarHeightPortrait CONSTANT FINAL)
    Q_PROPERTY(int toolBarHeightLandscape READ toolBarHeightLandscape CONSTANT FINAL)
    Q_PROPERTY(int scrollBarThickness READ scrollBarThickness CONSTANT FINAL)
    Q_PROPERTY(int sliderThickness READ sliderThickness CONSTANT FINAL)
    Q_PROPERTY(int menuItemHeight READ menuItemHeight CONSTANT FINAL)
    Q_PROPERTY(int textFieldHeight READ textFieldHeight CONSTANT FINAL)
    Q_PROPERTY(int switchButtonHeight READ switchButtonHeight CONSTANT FINAL)
    Q_PROPERTY(int dialogMinSize READ dialogMinSize CONSTANT FINAL)
    Q_PROPERTY(int dialogMaxSize READ dialogMaxSize CONSTANT FINAL)
    Q_PROPERTY(int ratingIndicatorImageWidth READ ratingIndicatorImageWidth CONSTANT FINAL)
    Q_PROPERTY(int ratingIndicatorImageHeight READ ratingIndicatorImageHeight CONSTANT FINAL)
    Q_PROPERTY(int buttonSize READ buttonSize CONSTANT FINAL)

public:

    explicit SDeclarativeStyleInternal(SStyleEngine *engine, QObject *parent = 0);
    ~SDeclarativeStyleInternal();

    int statusBarHeight() const;
    int tabBarHeightPortrait() const;
    int tabBarHeightLandscape() const;
    int toolBarHeightPortrait() const;
    int toolBarHeightLandscape() const;
    int scrollBarThickness() const;
    int sliderThickness() const;
    int menuItemHeight() const;
    int textFieldHeight() const;
    int switchButtonHeight() const;
    int dialogMinSize() const;
    int dialogMaxSize() const;
    int ratingIndicatorImageWidth() const;
    int ratingIndicatorImageHeight() const;
    int buttonSize() const;

    Q_INVOKABLE void play(int effect);
    Q_INVOKABLE int textWidth(const QString &text, const QFont &font) const;
    Q_INVOKABLE int fontHeight(const QFont &font) const;
    Q_INVOKABLE QUrl toolBarIconPath(const QUrl &path, bool inverted = false) const;
    Q_INVOKABLE QString imagePath(const QString &path, bool inverted = false) const;

Q_SIGNALS:
    void layoutParametersChanged();
    void colorParametersChanged();

protected:
    QScopedPointer<SDeclarativeStyleInternalPrivate> d_ptr;

private:
    Q_DISABLE_COPY(SDeclarativeStyleInternal)
    Q_DECLARE_PRIVATE(SDeclarativeStyleInternal)
};

#endif // SDECLARATIVESTYLEINTERNAL_H
