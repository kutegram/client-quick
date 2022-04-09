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

ImplicitSizeItem {
    id: root

    // Common Public API
    property bool checked: false
    property bool pressed: stateGroup.state == "Pressed" || stateGroup.state == "Dragging"

    signal clicked

    QtObject {
        id: internal
        objectName: "internal"

        property bool canceled

        function press() {
            internal.canceled = false
            privateStyle.play(Symbian.BasicItem)
        }

        function toggle() {
            root.checked = !root.checked
            root.clicked()
            privateStyle.play(Symbian.CheckBox)
        }

        function cancel() {
            internal.canceled = true
        }
    }

    StateGroup {
        id: stateGroup

        states: [
            State { name: "Pressed" },
            State { name: "Dragging" },
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
            },
            Transition {
                to: "Canceled"
                ScriptAction { script: internal.cancel() }
            }
        ]
    }

    implicitWidth: track.width
    implicitHeight: privateStyle.switchButtonHeight

    Image {
        id: track

        function trackPostfix() {
            if (!root.enabled && root.checked)
                return "disabled_on"
            else if (!root.enabled && !root.checked)
                return "disabled_off"
            else
                return "track"
        }

        source: privateStyle.imagePath("qtg_graf_switchbutton_" + track.trackPostfix())
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        sourceSize.width: Symbian.UndefinedSourceDimension
        sourceSize.height: privateStyle.switchButtonHeight
    }

    Item {
        id: fill
        clip: true
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        height: privateStyle.switchButtonHeight
        width: handle.x + handle.width / 2 - mouseArea.drag.minimumX
        visible: root.enabled

        Image {
            source: privateStyle.imagePath("qtg_graf_switchbutton_fill")
            anchors.left: parent.left
            anchors.top: parent.top
            height: parent.height
        }
    }

    Image {
        id: handle
        source: privateStyle.imagePath("qtg_graf_switchbutton_"
                                       + (root.pressed ? "handle_pressed" : "handle_normal"))
        anchors.verticalCenter: root.verticalCenter
        sourceSize.width: privateStyle.switchButtonHeight
        sourceSize.height: privateStyle.switchButtonHeight
        visible: root.enabled

        states: [
            State {
                name: "Off"
                when: !mouseArea.drag.active && !checked
                PropertyChanges {
                    target: handle
                    restoreEntryValues: false
                    x: mouseArea.drag.minimumX
                }
            },
            State {
                name: "On"
                when: !mouseArea.drag.active && checked
                PropertyChanges {
                    target: handle
                    restoreEntryValues: false
                    x: mouseArea.drag.maximumX
                }
            }
        ]

        transitions: [
            Transition {
                to: "Off"
                SmoothedAnimation {properties: "x"; easing.type: Easing.InOutQuad; duration: 200 }
            },
            Transition {
                to: "On"
                SmoothedAnimation {properties: "x"; easing.type: Easing.InOutQuad; duration: 200 }
            }
        ]
    }

    MouseArea {
        id: mouseArea

        property real lastX

        function isChecked() {
            return (handle.x + handle.width / 2 > track.x + (track.width / 2))
        }
        function updateHandlePos() {
            // The middle of the handle follows mouse, the handle is bound to the track
            handle.x = Math.max(track.x, Math.min(mouseArea.lastX - handle.width / 2,
                                                  track.x + track.width - handle.width))
        }

        anchors.fill: parent
        onPressed: stateGroup.state = "Pressed"
        onReleased: {
            if (root.checked == isChecked())
                stateGroup.state = "Canceled"
            stateGroup.state = ""
        }
        onClicked: {
            // Only toggle if released didn't
            if (internal.canceled)
                internal.toggle()
        }
        onCanceled: {
            // Mark as canceled
            stateGroup.state = "Canceled"
            // Reset state. Can't expect a release since mouse was ungrabbed
            stateGroup.state = ""
        }
        onPositionChanged: {
            mouseArea.lastX = mouse.x
            if (mouseArea.drag.active)
                updateHandlePos()
        }
        drag {
            // The handle is moved manually but MouseArea can be used to decide when dragging
            // should start (QApplication::startDragDistance). A dummy target needs to be bound or
            // dragging won't get activated.
            target: Item { visible: false }

            axis: Drag.XandYAxis
            minimumY: 0; maximumY: 0 // keep dragging active eventhough only x axis switches
            minimumX: track.x; maximumX: mouseArea.drag.minimumX + track.width - handle.width
            onActiveChanged: {
                if (mouseArea.drag.active) {
                    updateHandlePos()
                    stateGroup.state = "Dragging"
                } else if (!internal.canceled && root.checked != isChecked()) {
                    internal.toggle()
                }
            }
        }
    }
}
