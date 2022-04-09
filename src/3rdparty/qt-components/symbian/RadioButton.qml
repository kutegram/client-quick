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
    property alias checked: checkable.checked
    property bool pressed: stateGroup.state == "Pressed"
    signal clicked
    property alias text: label.text

    // Symbian specific API
    property alias platformExclusiveGroup: checkable.exclusiveGroup

    QtObject {
        id: internal
        objectName: "internal"

        function press() {
            privateStyle.play(Symbian.BasicItem)
        }

        function toggle() {
            privateStyle.play(Symbian.CheckBox)
            clickedEffect.restart()
            checkable.toggle()
            root.clicked()
        }

        function icon_postfix() {
            if (pressed)
                return "pressed"
            else if (root.checked) {
                if (!root.enabled)
                    return "disabled_selected"
                else
                    return "normal_selected"
            } else {
                if (!root.enabled)
                    return "disabled_unselected"
                else
                    return "normal_unselected"
            }
        }
    }

    StateGroup {
        id: stateGroup

        states: [
            State { name: "Pressed" },
            State { name: "Canceled" }
        ]

        transitions: [
            Transition {
                to: "Pressed"
                ScriptAction { script: internal.press() }
            },
            Transition {
                from: "Pressed"
                to: ""
                ScriptAction { script: internal.toggle() }
            }
        ]
    }

    implicitWidth: privateStyle.textWidth(label.text, label.font) + platformStyle.paddingMedium + privateStyle.buttonSize
    implicitHeight: privateStyle.buttonSize

    Image {
        id: image
        source: privateStyle.imagePath("qtg_graf_radiobutton_" + internal.icon_postfix())
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        sourceSize.width: privateStyle.buttonSize
        sourceSize.height: privateStyle.buttonSize
    }
    Text {
        id: label
        elide: Text.ElideRight
        anchors.left: image.right
        anchors.leftMargin: platformStyle.paddingMedium
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right

        font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeMedium }
        color: {
            if (!root.enabled)
                platformStyle.colorDisabledLight
            else if (pressed)
                platformStyle.colorPressed
            else
                platformStyle.colorNormalLight
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        onPressed: stateGroup.state = "Pressed"
        onReleased: stateGroup.state = ""
        onClicked: stateGroup.state = ""
        onExited: stateGroup.state = "Canceled"
        onCanceled: {
            // Mark as canceled
            stateGroup.state = "Canceled"
            // Reset state. Can't expect a release since mouse was ungrabbed
            stateGroup.state = ""
        }
    }

    ParallelAnimation {
        id: clickedEffect
        SequentialAnimation {
            PropertyAnimation {
                target: image
                property: "scale"
                from: 1.0
                to: 0.8
                easing.type: Easing.Linear
                duration: 50
            }
            PropertyAnimation {
                target: image
                property: "scale"
                from: 0.8
                to: 1.0
                easing.type: Easing.OutQuad
                duration: 170
            }
        }
    }

    Keys.onPressed: {
        if (event.key == Qt.Key_Select || event.key == Qt.Key_Return || event.key == Qt.Key_Enter) {
            checkable.toggle()
            clickedEffect.restart()
            root.clicked()
            event.accepted = true
        }
    }

    Checkable {
        id: checkable
        value: root.text
        enabled: true
    }
}
