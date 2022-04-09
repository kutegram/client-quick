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

import QtQuick 1.0
import "." 1.0

Item {
    id: root

    // The status of the page. One of the following:
    //      PageStatus.Inactive - the page is not visible
    //      PageStatus.Activating - the page is transitioning into becoming the active page
    //      PageStatus.Active - the page is the current active page
    //      PageStatus.Deactivating - the page is transitioning into becoming inactive
    property int status: PageStatus.Inactive

    property PageStack pageStack

    // Defines orientation lock for a page
    property int orientationLock: PageOrientation.Automatic

    property Item tools: null

    visible: false

    width: visible && parent ? parent.width : internal.previousWidth
    height: visible && parent ? parent.height : internal.previousHeight

    onWidthChanged: internal.previousWidth = visible ? width : internal.previousWidth
    onHeightChanged: internal.previousHeight = visible ? height : internal.previousHeight

    onStatusChanged: {
        if (status == PageStatus.Activating)
            internal.orientationLockCheck();
    }

    onOrientationLockChanged: {
        if (status == PageStatus.Activating || status == PageStatus.Active)
            internal.orientationLockCheck();
    }

    QtObject {
        id: internal
        property int previousWidth: 0
        property int previousHeight: 0

        function isScreenInPortrait() {
            return screen.currentOrientation == Screen.Portrait || screen.currentOrientation == Screen.PortraitInverted;
        }

        function isScreenInLandscape() {
            return screen.currentOrientation == Screen.Landscape || screen.currentOrientation == Screen.LandscapeInverted;
        }

        function orientationLockCheck() {
            switch (orientationLock) {
            case PageOrientation.Automatic:
                screen.allowedOrientations = Screen.Default
                break
            case PageOrientation.LockPortrait:
                screen.allowedOrientations = Screen.Portrait
                break
            case PageOrientation.LockLandscape:
                screen.allowedOrientations = Screen.Landscape
                break
            case PageOrientation.LockPrevious:
                screen.allowedOrientations = screen.currentOrientation
                break
            case PageOrientation.Manual:
            default:
                // Do nothing
                // In manual mode it is expected that orientation is set
                // explicitly to "screen.allowedOrientations" by the user.
                break
            }
        }
    }
}
