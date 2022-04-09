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
import "AppManager.js" as Utils

Item {
    id: root

    property Item visualParent
    property int status: DialogStatus.Closed
    property int animationDuration: 500
    property Item fader

    signal faderClicked

    function open() {
        fader = faderComponent.createObject(visualParent ? visualParent : Utils.rootObject())
        fader.animationDuration = root.animationDuration
        root.parent = fader
        status = DialogStatus.Opening
        fader.state = "Visible"
        platformPopupManager.privateNotifyPopupOpen()
    }

    function close() {
        if (status != DialogStatus.Closed) {
            status = DialogStatus.Closing
            if (fader)
                fader.state = "Hidden"
        }
    }

    onStatusChanged: {
        if (status == DialogStatus.Closed && fader) {
            // Temporarily setting root window as parent
            // otherwise transition animation jams
            root.parent = null
            fader.destroy()
            root.parent = parentCache.oldParent
            platformPopupManager.privateNotifyPopupClose()
        }
    }

    Component.onCompleted: {
        parentCache.oldParent = parent
    }

    //if this is not given, application may crash in some cases
    Component.onDestruction: {
        if (parentCache.oldParent != null) {
            parent = parentCache.oldParent
        }
    }

    QtObject {
        id: parentCache
        property QtObject oldParent: null
    }

    //This eats mouse events when popup area is clicked
    MouseArea {
        anchors.fill: parent
    }

    Component {
        id: faderComponent

        Fader {
            onClicked: root.faderClicked()
        }
    }
}
