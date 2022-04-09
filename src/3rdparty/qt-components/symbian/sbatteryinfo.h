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

#ifndef SBATTERYINFO_H
#define SBATTERYINFO_H

#include <QtCore/qscopedpointer.h>
#include <QtDeclarative/qdeclarativeitem.h>

class SBatteryInfoPrivate;

class SBatteryInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int batteryLevel READ batteryLevel NOTIFY batteryLevelChanged)
    Q_PROPERTY(BatteryStatus batteryStatus READ batteryStatus NOTIFY batteryStatusChanged)
    Q_PROPERTY(PowerState powerState READ powerState NOTIFY powerStateChanged)

    Q_ENUMS(BatteryStatus)
    Q_ENUMS(PowerState)

public:
    explicit SBatteryInfo(QObject *parent = 0);

    // Direct match to QSystemDeviceInfo::BatteryStatus
    enum BatteryStatus {
        NoBatteryLevel = 0,
        BatteryCritical,
        BatteryVeryLow,
        BatteryLow,
        BatteryNormal
    };

    // Direct match to QSystemDeviceInfo::PowerState
    enum PowerState {
        UnknownPower = 0,
        BatteryPower,
        WallPower,
        WallPowerChargingBattery
    };

    int batteryLevel() const;
    BatteryStatus batteryStatus() const;
    PowerState powerState() const;

Q_SIGNALS:
    void batteryLevelChanged(int level);
    void batteryStatusChanged(BatteryStatus status);
    void powerStateChanged(PowerState state);

protected:
    QScopedPointer<SBatteryInfoPrivate> d_ptr;

private:
#ifdef HAVE_MOBILITY
    Q_PRIVATE_SLOT(d_func(), void _q_batteryLevelChanged(int))
    Q_PRIVATE_SLOT(d_func(), void _q_batteryStatusChanged(QSystemDeviceInfo::BatteryStatus))
    Q_PRIVATE_SLOT(d_func(), void _q_powerStateChanged(QSystemDeviceInfo::PowerState))
#endif

    Q_DISABLE_COPY(SBatteryInfo)
    Q_DECLARE_PRIVATE(SBatteryInfo)
};

QML_DECLARE_TYPE(SBatteryInfo)

#endif // SBATTERYINFO_H
