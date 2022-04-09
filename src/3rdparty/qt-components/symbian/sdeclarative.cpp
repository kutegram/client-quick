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

#include "sdeclarative.h"
#include "siconpool.h"
#include <QCoreApplication>
#include <QTime>
#include <QTimer>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QPixmapCache>
#include <QSysInfo>

#ifdef Q_OS_SYMBIAN
#include <AknUtils.h>
#endif // Q_OS_SYMBIAN

#ifdef Q_OS_WIN
#include <windows.h>
#include <psapi.h>
#endif

//#define Q_DEBUG_SDECLARATIVE
#if defined(Q_DEBUG_SDECLARATIVE)
#include <QDebug>
#endif // Q_DEBUG_SDECLARATIVE

static const int MINUTE_MS = 60*1000;

#ifdef Q_OS_SYMBIAN
_LIT(KTimeFormat, "%J%:1%T");
#endif

class SDeclarativePrivate
{
public:
    SDeclarativePrivate() :
        mListInteractionMode(SDeclarative::TouchInteraction), foreground(true) {}

    int allocatedMemory() const;

    SDeclarative::InteractionMode mListInteractionMode;
    QTimer timer;
    bool foreground;
};

int SDeclarativePrivate::allocatedMemory() const
{
#if defined(Q_OS_SYMBIAN)
    TInt totalAllocated;
    User::AllocSize(totalAllocated);
    return totalAllocated;
#elif defined(Q_OS_WIN)
    PROCESS_MEMORY_COUNTERS pmc;
    if (!GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
        return -1;
    return pmc.WorkingSetSize;
#else
    return -1;
#endif
}

SDeclarative::SDeclarative(QObject *parent) :
    QObject(parent),
    d_ptr(new SDeclarativePrivate)
{
    d_ptr->timer.start(MINUTE_MS);
    connect(&d_ptr->timer, SIGNAL(timeout()), this, SIGNAL(currentTimeChanged()));

    QCoreApplication *application = QCoreApplication::instance();
    if (application)
        application->installEventFilter(this);
}

SDeclarative::~SDeclarative()
{
    d_ptr->timer.stop();
}

SDeclarative::InteractionMode SDeclarative::listInteractionMode() const
{
    return d_ptr->mListInteractionMode;
}

void SDeclarative::setListInteractionMode(SDeclarative::InteractionMode mode)
{
    if (d_ptr->mListInteractionMode != mode) {
        d_ptr->mListInteractionMode = mode;
        emit listInteractionModeChanged();
    }
}

QString SDeclarative::currentTime()
{
#ifdef Q_OS_SYMBIAN
    TBuf<15> time;
    TTime homeTime;
    homeTime.HomeTime();
    TRAP_IGNORE(homeTime.FormatL(time, KTimeFormat));
    // Do the possible arabic indic digit etc. conversions
    AknTextUtils::DisplayTextLanguageSpecificNumberConversion(time);
    return QString(reinterpret_cast<const QChar *>(time.Ptr()), time.Length());
#else
    return QTime::currentTime().toString(QLatin1String("h:mm"));
#endif
}

bool SDeclarative::isForeground()
{
    return d_ptr->foreground;
}

int SDeclarative::privateAllocatedMemory() const
{
    return d_ptr->allocatedMemory();
}

void SDeclarative::privateClearIconCaches()
{
    SIconPool::releaseAll();
    QPixmapCache::clear();
}

void SDeclarative::privateClearComponentCache()
{
    QDeclarativeContext *context = qobject_cast<QDeclarativeContext*>(this->parent());
    if (context)
        context->engine()->clearComponentCache();
}

SDeclarative::S60Version SDeclarative::s60Version() const
{
#ifdef Q_OS_SYMBIAN
    switch (QSysInfo::s60Version()) {
    case QSysInfo::SV_S60_5_2:
        return SV_S60_5_2;
#if QT_VERSION > 0x040703
    case QSysInfo::SV_S60_5_3:
        return SV_S60_5_3;
    case QSysInfo::SV_S60_5_4:
        return SV_S60_5_4;
#endif
    default:
        return SV_S60_UNKNOWN;
    }
#else
    return SV_S60_UNKNOWN;
#endif
}

bool SDeclarative::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == QCoreApplication::instance()) {
        if (event->type() == QEvent::ApplicationActivate) {
            emit currentTimeChanged();
            d_ptr->timer.start(MINUTE_MS);
            d_ptr->foreground = true;
            emit foregroundChanged();
        } else if (event->type() == QEvent::ApplicationDeactivate) {
            d_ptr->timer.stop();
            d_ptr->foreground = false;
            emit foregroundChanged();
        }
    }
    return QObject::eventFilter(obj, event);
}
