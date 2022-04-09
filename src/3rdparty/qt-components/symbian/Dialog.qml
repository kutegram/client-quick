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

    property alias title: titleItem.children
    property alias content: contentItem.children
    property alias buttons: buttonItem.children
    property alias visualParent: dialog.visualParent
    property alias status: dialog.status

    // read-only
    property int platformContentMaximumWidth: dialog.width
    // read-only
    property int platformContentMaximumHeight:
        dialog.maxHeight() - titleBar.height - buttonItem.height

    property alias privateTitleHeight: titleBar.height
    property alias privateButtonsHeight: buttonItem.height

    signal accepted
    signal rejected
    signal clickedOutside

    function open() {
        dialog.open()
        dialog.focus = true
    }

    function accept() {
        if (status == DialogStatus.Open) {
            dialog.close()
            accepted()
        }
    }

    function reject() {
        if (status == DialogStatus.Open) {
            dialog.close()
            rejected()
        }
    }

    function close() {
        dialog.close()
    }

    visible: false

    Popup {
        id: dialog

        function defaultHeight() {
            if (root.height > 0)
                return root.height
            else
                return titleBar.height + contentItem.childrenRect.height + buttonItem.height
        }

        function defaultWidth() {
            return root.width > 0 ? root.width : maxWidth()
        }

        function maxWidth() {
            if (screen.width < screen.height)
                return screen.width - 2 * platformStyle.paddingMedium
            else
                return privateStyle.dialogMaxSize
        }

        function maxHeight() {
            if (screen.width < screen.height)
                return privateStyle.dialogMaxSize
            else
                return screen.height - 2 * platformStyle.paddingMedium
        }

        function minWidth() {
            return Math.min(screen.displayWidth, screen.displayHeight) - 2 * platformStyle.paddingMedium
        }

        onFaderClicked: root.clickedOutside()

        width: Math.max(Math.min(defaultWidth(), maxWidth()), minWidth())
        height: Math.max(Math.min(defaultHeight(), maxHeight()), privateStyle.dialogMinSize)

        state: "Hidden"
        visible: true
        anchors.centerIn: parent
        animationDuration: 250

        // Consume all key events that are not processed by children
        Keys.onPressed: event.accepted = true
        Keys.onReleased: event.accepted = true

        BorderImage {
            source: privateStyle.imagePath("qtg_fr_popup")
            border { left: 20; top: 20; right: 20; bottom: 20 }
            anchors.fill: parent
        }

        Item {
            id: titleBar

            height: platformStyle.graphicSizeSmall + 2 * platformStyle.paddingLarge
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }

            BorderImage {
                source: privateStyle.imagePath("qtg_fr_popup_heading")
                border { left: 40; top: 0; right: 40; bottom: 0 }
                anchors.fill: parent
            }

            Item {
                id: titleItem

                clip: true
                anchors {
                    fill: parent
                }
            }
        }

        Item {
            id: contentItem

            clip: true
            anchors {
                top: titleBar.bottom
                left: parent.left
                right: parent.right
                bottom: buttonItem.top
            }
        }

        Item {
            id: buttonItem

            height: childrenRect.height
            clip: true
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
        }

        states: [
            State {
                name: "Visible"
                when: status == DialogStatus.Opening || status == DialogStatus.Open
                PropertyChanges { target: dialog; opacity: 1.0; scale: 1; }
            },
            State {
                name: "Hidden"
                when: status == DialogStatus.Closing || status == DialogStatus.Closed
                PropertyChanges { target: dialog; opacity: 0.0; scale: 0.9; }
            }
        ]

        transitions: [
            Transition {
                from: "Visible"; to: "Hidden"
                SequentialAnimation {
                    ScriptAction {script: status = DialogStatus.Closing }
                    ParallelAnimation {
                        NumberAnimation {
                            properties: "opacity"
                            duration: dialog.animationDuration
                            easing.type: Easing.Linear
                        }
                        NumberAnimation {
                            property: "scale"
                            duration: dialog.animationDuration
                            easing.type: Easing.InQuad
                        }
                    }
                    ScriptAction {script: status = DialogStatus.Closed }
                }
            },
            Transition {
                from: "Hidden"; to: "Visible"
                SequentialAnimation {
                    ScriptAction { script: status = DialogStatus.Opening }
                    PropertyAction { target: dialog; property: "visible"; value: true}
                    ParallelAnimation {
                        NumberAnimation {
                            properties: "opacity"
                            duration: dialog.animationDuration
                            easing.type: Easing.Linear
                        }
                        NumberAnimation {
                            property: "scale"
                            duration: dialog.animationDuration
                            easing.type: Easing.OutQuad
                        }
                    }
                    ScriptAction { script: status = DialogStatus.Open }
                }
            }
        ]
    }
}
