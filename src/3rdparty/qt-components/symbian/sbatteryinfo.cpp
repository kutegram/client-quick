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

#include "sbatteryinfo.h"

#ifdef HAVE_MOBILITY
#include <QSystemDeviceInfo>
using namespace QtMobility;
#endif

class SBatteryInfoPrivate
{
    Q_DECLARE_PUBLIC(SBatteryInfo)

public:
    SBatteryInfoPrivate(SBatteryInfo *qq) : q_ptr(qq) {}

#ifdef HAVE_MOBILITY
    void _q_batteryLevelChanged(int level);
    void _q_batteryStatusChanged(QSystemDeviceInfo::BatteryStatus status);
    void _q_powerStateChanged(QSystemDeviceInfo::PowerState state);

    QSystemDeviceInfo deviceInfo;
#endif

private:
    SBatteryInfo *q_ptr;
};

#ifdef HAVE_MOBILITY
void SBatteryInfoPrivate::_q_batteryLevelChanged(int level)
{
    qDebug() << "_q_batteryLevelChanged():" << level;
    emit q_ptr->batteryLevelChanged(level);
}

void SBatteryInfoPrivate::_q_batteryStatusChanged(QSystemDeviceInfo::BatteryStatus status)
{
    qDebug() << "_q_batteryStatusChanged():" << status;
    emit q_ptr->batteryStatusChanged(static_cast<SBatteryInfo::BatteryStatus>(status));
}

void SBatteryInfoPrivate::_q_powerStateChanged(QSystemDeviceInfo::PowerState state)
{
    qDebug() << "_q_powerStateChanged():" << state;
    emit q_ptr->powerStateChanged(static_cast<SBatteryInfo::PowerState>(state));
}
#endif

SBatteryInfo::SBatteryInfo(QObject *parent) :
    QObject(parent), d_ptr(new SBatteryInfoPrivate(this))
{
#ifdef HAVE_MOBILITY
    connect(&d_ptr->deviceInfo, SIGNAL(batteryLevelChanged(int)),
            this, SLOT(_q_batteryLevelChanged(int)));
    connect(&d_ptr->deviceInfo, SIGNAL(batteryStatusChanged(QSystemDeviceInfo::BatteryStatus)),
            this, SLOT(_q_batteryStatusChanged(QSystemDeviceInfo::BatteryStatus)));
    connect(&d_ptr->deviceInfo, SIGNAL(powerStateChanged(QSystemDeviceInfo::PowerState)),
            this, SLOT(_q_powerStateChanged(QSystemDeviceInfo::PowerState)));
#endif
}

int SBatteryInfo::batteryLevel() const
{
#ifdef HAVE_MOBILITY
    return d_ptr->deviceInfo.batteryLevel();
#else
    return 0;
#endif
}

SBatteryInfo::BatteryStatus SBatteryInfo::batteryStatus() const
{
#ifdef HAVE_MOBILITY
    return static_cast<SBatteryInfo::BatteryStatus>(d_ptr->deviceInfo.batteryStatus());
#else
    return NoBatteryLevel;
#endif
}

SBatteryInfo::PowerState SBatteryInfo::powerState() const
{
#ifdef HAVE_MOBILITY
    return static_cast<SBatteryInfo::PowerState>(d_ptr->deviceInfo.currentPowerState());
#else
    return UnknownPower;
#endif
}

#include "moc_sbatteryinfo.cpp"
