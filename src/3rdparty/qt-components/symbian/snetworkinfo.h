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

#ifndef SNETWORKINFO_H
#define SNETWORKINFO_H

#include <QtCore/qscopedpointer.h>
#include <QtDeclarative/qdeclarativeitem.h>

class SNetworkInfoPrivate;

class SNetworkInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(NetworkMode networkMode READ networkMode NOTIFY networkModeChanged)
    Q_PROPERTY(NetworkStatus networkStatus READ networkStatus NOTIFY networkStatusChanged)
    Q_PROPERTY(int networkSignalStrength READ networkSignalStrength NOTIFY networkSignalStrengthChanged)

    Q_ENUMS(NetworkMode)
    Q_ENUMS(NetworkStatus)

public:
    explicit SNetworkInfo(QObject *parent = 0);

    // Direct match to QSystemNetworkInfo::NetworkMode
    enum NetworkMode {
        UnknownMode=0,
        GsmMode,
        CdmaMode,
        WcdmaMode,
        WlanMode,
        EthernetMode,
        BluetoothMode,
        WimaxMode
    };

    // Direct match to QSystemNetworkInfo::NetworkStatus
    enum NetworkStatus {
        UndefinedStatus = 0,
        NoNetworkAvailable,
        EmergencyOnly,
        Searching,
        Busy,
        Connected,
        HomeNetwork,
        Denied,
        Roaming
    };

    NetworkMode networkMode() const;
    NetworkStatus networkStatus() const;
    int networkSignalStrength() const;

Q_SIGNALS:
    void networkModeChanged(NetworkMode mode);
    void networkStatusChanged(NetworkMode mode, NetworkStatus status);
    void networkSignalStrengthChanged(NetworkMode mode, int strength);

protected:
    QScopedPointer<SNetworkInfoPrivate> d_ptr;

private:
#ifdef HAVE_MOBILITY
    Q_PRIVATE_SLOT(d_func(), void _q_networkModeChanged(QSystemNetworkInfo::NetworkMode))
    Q_PRIVATE_SLOT(d_func(), void _q_networkStatusChanged(QSystemNetworkInfo::NetworkMode, QSystemNetworkInfo::NetworkStatus))
    Q_PRIVATE_SLOT(d_func(), void _q_networkSignalStrengthChanged(QSystemNetworkInfo::NetworkMode, int))
#endif

    Q_DISABLE_COPY(SNetworkInfo)
    Q_DECLARE_PRIVATE(SNetworkInfo)
};

QML_DECLARE_TYPE(SNetworkInfo)

#endif // SNETWORKINFO_H
