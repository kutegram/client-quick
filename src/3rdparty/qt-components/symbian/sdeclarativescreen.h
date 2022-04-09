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

#ifndef SDECLARATIVESCREEN_H
#define SDECLARATIVESCREEN_H

#include <QtDeclarative/qdeclarativeitem.h>

class SDeclarativeScreenPrivate;
QT_FORWARD_DECLARE_CLASS(QDeclarativeEngine)

class SDeclarativeScreen : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int width READ width NOTIFY widthChanged FINAL)
    Q_PROPERTY(int height READ height NOTIFY heightChanged FINAL)
    Q_PROPERTY(int displayWidth READ displayWidth CONSTANT FINAL)
    Q_PROPERTY(int displayHeight READ displayHeight CONSTANT FINAL)

    Q_PROPERTY(int rotation READ rotation NOTIFY currentOrientationChanged FINAL)
    Q_PROPERTY(Orientation currentOrientation READ currentOrientation NOTIFY currentOrientationChanged FINAL)
    Q_PROPERTY(Orientations allowedOrientations READ allowedOrientations WRITE setAllowedOrientations NOTIFY allowedOrientationsChanged FINAL)

    Q_PROPERTY(qreal dpi READ dpi CONSTANT FINAL)
    Q_PROPERTY(DisplayCategory displayCategory READ displayCategory CONSTANT FINAL)
    Q_PROPERTY(Density density READ density CONSTANT FINAL)

    Q_ENUMS(Orientation DisplayCategory Density)
    Q_FLAGS(Orientations)

public:
    explicit SDeclarativeScreen(QDeclarativeEngine *engine, QObject *parent = 0);
    virtual ~SDeclarativeScreen();

    enum Orientation {
        Default = 0,
        Portrait = 1,
        Landscape = 2,
        PortraitInverted = 4,
        LandscapeInverted = 8,
        All = 15
    };

    enum DisplayCategory {
        Small,
        Normal,
        Large,
        ExtraLarge
    };

    enum Density {
        Low,
        Medium,
        High,
        ExtraHigh
   };

    Q_DECLARE_FLAGS(Orientations, Orientation)

    int width() const;
    int height() const;
    int displayWidth() const;
    int displayHeight() const;

    int rotation() const;
    Orientation currentOrientation() const;
    Orientations allowedOrientations() const;
    void setAllowedOrientations(Orientations orientations);

    qreal dpi() const;
    DisplayCategory displayCategory() const;
    Density density() const;

    Q_INVOKABLE void privateSetDisplay(int width, int height, qreal dpi);

Q_SIGNALS:
    void widthChanged();
    void heightChanged();
    void currentOrientationChanged();
    void allowedOrientationsChanged();
    void displayChanged();
    void privateAboutToChangeOrientation();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    QScopedPointer<SDeclarativeScreenPrivate> d_ptr;

private:
    Q_PRIVATE_SLOT(d_func(), void _q_initView(const QSize &))
    Q_PRIVATE_SLOT(d_func(), void _q_updateScreenSize(const QSize &))
    Q_PRIVATE_SLOT(d_func(), void _q_desktopResized(int))
    Q_DISABLE_COPY(SDeclarativeScreen)
    Q_DECLARE_PRIVATE(SDeclarativeScreen)
};

QML_DECLARE_TYPE(SDeclarativeScreen)

Q_DECLARE_OPERATORS_FOR_FLAGS(SDeclarativeScreen::Orientations)

#endif // SDECLARATIVESCREEN_H
