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

#include "snetworkinfo.h"

#ifdef HAVE_MOBILITY
#include <QSystemNetworkInfo>
using namespace QtMobility;
#endif

class SNetworkInfoPrivate
{
    Q_DECLARE_PUBLIC(SNetworkInfo)

public:
    SNetworkInfoPrivate(SNetworkInfo *qq) : q_ptr(qq) {}

#ifdef HAVE_MOBILITY
    void _q_networkModeChanged(QSystemNetworkInfo::NetworkMode mode);
    void _q_networkStatusChanged(QSystemNetworkInfo::NetworkMode mode, QSystemNetworkInfo::NetworkStatus status);
    void _q_networkSignalStrengthChanged(QSystemNetworkInfo::NetworkMode mode, int strength);

    QSystemNetworkInfo networkInfo;
#endif

private:
    SNetworkInfo *q_ptr;
};

#ifdef HAVE_MOBILITY
void SNetworkInfoPrivate::_q_networkModeChanged(QSystemNetworkInfo::NetworkMode mode)
{
    emit q_ptr->networkModeChanged(static_cast<SNetworkInfo::NetworkMode>(mode));
}

void SNetworkInfoPrivate::_q_networkStatusChanged(QSystemNetworkInfo::NetworkMode mode, QSystemNetworkInfo::NetworkStatus status)
{
    emit q_ptr->networkStatusChanged(static_cast<SNetworkInfo::NetworkMode>(mode), static_cast<SNetworkInfo::NetworkStatus>(status));
}

void SNetworkInfoPrivate::_q_networkSignalStrengthChanged(QSystemNetworkInfo::NetworkMode mode, int strength)
{
    emit q_ptr->networkSignalStrengthChanged(static_cast<SNetworkInfo::NetworkMode>(mode), strength);
}
#endif

SNetworkInfo::SNetworkInfo(QObject *parent) :
    QObject(parent), d_ptr(new SNetworkInfoPrivate(this))
{
#ifdef HAVE_MOBILITY
    connect(&d_ptr->networkInfo, SIGNAL(networkModeChanged(QSystemNetworkInfo::NetworkMode)),
            this, SLOT(_q_networkModeChanged(QSystemNetworkInfo::NetworkMode)));
    connect(&d_ptr->networkInfo, SIGNAL(networkStatusChanged(QSystemNetworkInfo::NetworkMode, QSystemNetworkInfo::NetworkStatus)),
            this, SLOT(_q_networkStatusChanged(QSystemNetworkInfo::NetworkMode, QSystemNetworkInfo::NetworkStatus)));
    connect(&d_ptr->networkInfo, SIGNAL(networkSignalStrengthChanged(QSystemNetworkInfo::NetworkMode, int)),
            this, SLOT(_q_networkSignalStrengthChanged(QSystemNetworkInfo::NetworkMode, int)));
#endif
}

SNetworkInfo::NetworkMode SNetworkInfo::networkMode() const
{
#ifdef HAVE_MOBILITY
    return static_cast<SNetworkInfo::NetworkMode>(d_ptr->networkInfo.currentMode());
#else
    return UnknownMode;
#endif
}

SNetworkInfo::NetworkStatus SNetworkInfo::networkStatus() const
{
#ifdef HAVE_MOBILITY
    return static_cast<SNetworkInfo::NetworkStatus>(d_ptr->networkInfo.networkStatus(d_ptr->networkInfo.currentMode()));
#else
    return UndefinedStatus;
#endif
}

int SNetworkInfo::networkSignalStrength() const
{
#ifdef HAVE_MOBILITY
    return QSystemNetworkInfo::networkSignalStrength(d_ptr->networkInfo.currentMode());
#else
    return 0;
#endif
}

#include "moc_snetworkinfo.cpp"
