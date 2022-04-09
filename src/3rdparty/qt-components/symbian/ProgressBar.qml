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
import Qt.labs.components 1.0
import "." 1.0

ImplicitSizeItem {
    id: root

    // Common Public API
    property alias minimumValue: model.minimumValue
    property alias maximumValue: model.maximumValue
    property alias value: model.value
    property bool indeterminate: false

    implicitWidth: Math.max(50, screen.width / 2) // TODO: use screen.displayWidth
    implicitHeight: privateStyle.sliderThickness

    BorderImage {
        id: background
        source: privateStyle.imagePath("qtg_fr_progressbar_track")
        border { left: 20; top: 0; right: 20; bottom: 0 }

        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter

        width: parent.width
        height: parent.height

        states: [
            State {
                name: "determinate"
                when: indeterminate == false
                PropertyChanges {
                    target: frame
                    visible: true
                }
                PropertyChanges {
                    target: indeterminateMaskedImage
                    visible: false
                }
                PropertyChanges {
                    target: indeterminateMaskedImageExtra
                    visible: false
                }
                PropertyChanges {
                    target: indeterminateAnimation
                    running: false
                }
            },
            State {
                name: "indeterminate"
                when: indeterminate == true
                PropertyChanges {
                    target: frame
                    visible: false
                }
                PropertyChanges {
                    target: indeterminateMaskedImage
                    visible: true
                }
                PropertyChanges {
                    target: indeterminateMaskedImageExtra
                    visible: true
                }
                PropertyChanges {
                    target: indeterminateAnimation
                    running: true
                }
            }
        ]

        ParallelAnimation {
            id: indeterminateAnimation
            loops: Animation.Infinite
            running: true

            PropertyAnimation { target: indeterminateMaskedImage; property: "offset.x"; from: height; to: 1; easing.type: Easing.Linear; duration: privateStyle.sliderThickness * 25 }
            PropertyAnimation { target: indeterminateMaskedImageExtra; property: "offset.x"; from: 0; to: 1 - height; easing.type: Easing.Linear; duration: privateStyle.sliderThickness * 25 }
        }

        Item {
            clip: true

            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            width: model.position

            BorderImage {
                id: frame
                source: privateStyle.imagePath("qtg_fr_progressbar_fill")
                border { left: 20; top: 0; right: 20; bottom: 0 }

                anchors.left: parent.left
                anchors.top: parent.top

                width: root.width
                height: parent.height
            }
        }

        MaskedImage {
            id: indeterminateMaskedImage
            anchors.fill: parent

            topMargin: 0
            bottomMargin: 0
            leftMargin: 20
            rightMargin: 20

            tiled: true
            imageName: "qtg_graf_progressbar_wait"
            maskName: "qtg_fr_progressbar_mask"
        }

        // Secondary tile to keep the bar full when the animation scrolls
        MaskedImage {
            id: indeterminateMaskedImageExtra
            anchors.fill: parent

            topMargin: 0
            bottomMargin: 0
            leftMargin: 20
            rightMargin: 20

            tiled: false
            imageName: "qtg_graf_progressbar_wait"
            maskName: "qtg_fr_progressbar_mask"
        }
    }

    RangeModel {
        id: model
        minimumValue: 0.0
        maximumValue: 1.0
        positionAtMinimum: 0.0
        positionAtMaximum: background.width
    }
}

