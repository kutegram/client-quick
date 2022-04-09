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

#include "spopupmanager.h"

#ifdef Q_OS_SYMBIAN
#include <aknappui.h>
#include <avkon.rsg>
#if defined(HAVE_SYMBIAN_INTERNAL)
#include <aknsmallindicator.h>
#endif // HAVE_SYMBIAN_INTERNAL
#endif // Q_OS_SYMBIAN

//#define Q_DEBUG_SPOPUPMANAGER
#if defined(Q_DEBUG_SPOPUPMANAGER)
#include <QDebug>
#endif // Q_DEBUG_SPOPUPMANAGER

class SPopupManagerPrivate
{
public:
    SPopupManagerPrivate() : popupStackDepth(0) {}

    int popupStackDepth;
};

SPopupManager::SPopupManager(QObject *parent) :
    QObject(parent),
    d_ptr(new SPopupManagerPrivate)
{
}

SPopupManager::~SPopupManager()
{
}

int SPopupManager::popupStackDepth() const
{
    return d_ptr->popupStackDepth;
}

#if defined(Q_OS_SYMBIAN) && defined(HAVE_SYMBIAN_INTERNAL)
static void DoShowIndicatorPopupL()
{
    MEikAppUiFactory *factory = CEikonEnv::Static()->AppUiFactory();
    if (factory) {
        CEikStatusPane* statusPane = factory->StatusPane();
        if (statusPane && statusPane->CurrentLayoutResId() != R_AVKON_WIDESCREEN_PANE_LAYOUT_IDLE_FLAT_NO_SOFTKEYS) {
            // statusPane SwitchLayoutL is needed for positioning Avkon indicator popup (opened from StatusBar) correctly
            statusPane->SwitchLayoutL(R_AVKON_WIDESCREEN_PANE_LAYOUT_IDLE_FLAT_NO_SOFTKEYS);
        }
    }
    CAknSmallIndicator* indicator = CAknSmallIndicator::NewLC(TUid::Uid(0));
    indicator->HandleIndicatorTapL();
    CleanupStack::PopAndDestroy(indicator);
}
#endif // Q_OS_SYMBIAN && HAVE_SYMBIAN_INTERNAL

void SPopupManager::privateShowIndicatorPopup()
{
#if defined(Q_OS_SYMBIAN) && defined(HAVE_SYMBIAN_INTERNAL)
    QT_TRAP_THROWING(DoShowIndicatorPopupL());
#endif // Q_OS_SYMBIAN && HAVE_SYMBIAN_INTERNAL
}

void SPopupManager::privateNotifyPopupOpen()
{
    d_ptr->popupStackDepth++;
    emit popupStackDepthChanged();
}

void SPopupManager::privateNotifyPopupClose()
{
    if (d_ptr->popupStackDepth > 0) {
        d_ptr->popupStackDepth--;
        emit popupStackDepthChanged();
    }
}
