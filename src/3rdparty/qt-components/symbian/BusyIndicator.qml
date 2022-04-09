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
import com.nokia.symbian 1.0
import "." 1.0

ImplicitSizeItem {
    id: root

    property bool running: false

    implicitWidth: platformStyle.graphicSizeSmall
    implicitHeight: platformStyle.graphicSizeSmall

    Image {
        id: spinner
        property int index: 1

        // cannot use anchors.fill here because the size will be 0 during
        // construction and that gives out nasty debug warnings
        width: parent.width
        height: parent.height
        sourceSize.width: width
        sourceSize.height: height
        source: privateStyle.imagePath("qtg_anim_spinner_large_" + index)
        smooth: true

        NumberAnimation on index {
            id: numAni
            from: 1; to: 10
            duration: 1000
            running: root.visible
            // QTBUG-19080 is preventing the following line from working
            // We will have to use workaround for now
            // http://bugreports.qt.nokia.com/browse/QTBUG-19080
            // paused: !root.running || !symbian.foreground
            loops: Animation.Infinite
        }

        // START workaround for QTBUG-19080
        Component {
            id: bindingCom
            Binding {
                target: numAni
                property: "paused"
                value: numAni.running ? (!root.running || !symbian.foreground) : false
            }
        }
        Component.onCompleted: bindingCom.createObject(numAni)
        // END workaround
    }
}
