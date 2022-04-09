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

#ifndef SDECLARATIVESCREEN_P_H
#define SDECLARATIVESCREEN_P_H

#include "sdeclarativescreen.h"
#include <QtCore/qpointer.h>
#include <QtGui/qgraphicsview.h>

QT_FORWARD_DECLARE_CLASS(QDeclarativeEngine)

class SDeclarativeScreenPrivate
{
    Q_DECLARE_PUBLIC(SDeclarativeScreen)
public:
    SDeclarativeScreenPrivate(SDeclarativeScreen *qq, QDeclarativeEngine *engine);
    ~SDeclarativeScreenPrivate();

    void updateOrientationAngle();
    void _q_initView(const QSize &);
    void _q_updateScreenSize(const QSize &);
    void _q_desktopResized(int);
    bool isLandscapeScreen() const;
    QSize currentScreenSize() const;
    QSize adjustedSize(const QSize &size) const;
    bool portraitAllowed() const;
    bool landscapeAllowed() const;

public:
    SDeclarativeScreen *q_ptr;
    SDeclarativeScreen::Orientation currentOrientation;
    SDeclarativeScreen::Orientations allowedOrientations;
    qreal dpi;
    QSize screenSize;  // "logical" screen
    QSize displaySize; // "physical" display
    bool settingDisplay;
    QPointer<QGraphicsView> gv;
    bool initCalled;
    bool initDone;
    QDeclarativeEngine *engine;

    static SDeclarativeScreenPrivate *d_ptr(SDeclarativeScreen *screen) {
        Q_ASSERT(screen);
        return screen->d_func();
    }
};

#endif // SDECLARATIVESCREEN_P_H
