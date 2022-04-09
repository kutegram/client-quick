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
    id: button

    // Common Public API
    property bool checked: false
    property bool checkable: false
    property bool pressed: (stateGroup.state == "Pressed" || stateGroup.state == "AutoRepeating") && mouseArea.containsMouse
    property alias text: label.text
    property url iconSource
    property alias font: label.font

    signal clicked

    // Symbian specific signals and properties
    signal platformReleased
    signal platformPressAndHold

    // Symbian specific API
    property bool platformInverted: false
    property bool platformAutoRepeat: false

    implicitWidth: Math.max(container.contentWidth + 2 * internal.horizontalPadding, privateStyle.buttonSize)
    implicitHeight: Math.max(container.contentHeight + 2 * internal.verticalPadding, privateStyle.buttonSize)

    QtObject {
        id: internal
        objectName: "internal"

        property int autoRepeatInterval: 60
        property int verticalPadding: (privateStyle.buttonSize - platformStyle.graphicSizeSmall) / 2
        property int horizontalPadding: label.text ? platformStyle.paddingLarge : verticalPadding

        // "pressed" is a transient state, see press() function
        function modeName() {
            if (belongsToButtonRow())
                return parent.privateModeName(button, 0)
            else if (!button.enabled)
                return "disabled"
            else if (button.checked)
                return "latched"
            else
                return "normal"
        }

        function toggleChecked() {
            if (checkable)
                checked = !checked
        }

        function press() {
            if (!belongsToButtonGroup()) {
                if (checkable && checked)
                    privateStyle.play(Symbian.SensitiveButton)
                else
                    privateStyle.play(Symbian.BasicButton)
            } else if (checkable && !checked) {
                privateStyle.play(Symbian.BasicButton)
            }
            highlight.source = privateStyle.imagePath(internal.imageName() + "pressed",
                                                      button.platformInverted)
            container.scale = 0.95
            highlight.opacity = 1
        }

        function release() {
            container.scale = 1
            highlight.opacity = 0
            if (tapRepeatTimer.running)
                tapRepeatTimer.stop()
            button.platformReleased()
        }

        function click() {
            if ((checkable && checked && !belongsToButtonGroup()) || !checkable)
                privateStyle.play(Symbian.BasicButton)
            internal.toggleChecked()
            clickedEffect.restart()
            button.clicked()
        }

        function repeat() {
            if (!checkable)
                privateStyle.play(Symbian.SensitiveButton)
            button.clicked()
        }

        // The function imageName() handles fetching correct graphics for the Button.
        // If the parent of a Button is ButtonRow, segmented-style graphics are used to create a
        // seamless row of buttons. Otherwise normal Button graphics are utilized.
        function imageName() {
            if (belongsToButtonRow())
                return parent.privateGraphicsName(button, 0)
            return "qtg_fr_pushbutton_"
        }

        function belongsToButtonGroup() {
            return button.parent
                   && button.parent.hasOwnProperty("checkedButton")
                   && button.parent.exclusive
        }

        function belongsToButtonRow() {
            return button.parent
                    && button.parent.hasOwnProperty("checkedButton")
                    && button.parent.hasOwnProperty("privateDirection")
                    && button.parent.privateDirection == Qt.Horizontal
                    && button.parent.children.length > 1
        }
    }

    StateGroup {
        id: stateGroup

        states: [
            State { name: "Pressed" },
            State { name: "AutoRepeating" },
            State { name: "Canceled" }
        ]

        transitions: [
            Transition {
                to: "Pressed"
                ScriptAction { script: internal.press() }
            },
            Transition {
                from: "Pressed"
                to: "AutoRepeating"
                ScriptAction { script: tapRepeatTimer.start() }
            },
            Transition {
                from: "Pressed"
                to: ""
                ScriptAction { script: internal.release() }
                ScriptAction { script: internal.click() }
            },
            Transition {
                from: "Pressed"
                to: "Canceled"
                ScriptAction { script: internal.release() }
            },
            Transition {
                from: "AutoRepeating"
                ScriptAction { script: internal.release() }
            }
        ]
    }

    BorderImage {
        source: privateStyle.imagePath(internal.imageName() + internal.modeName(),
                                       button.platformInverted)
        border { left: 20; top: 20; right: 20; bottom: 20 }
        anchors.fill: parent

        BorderImage {
            id: highlight
            border { left: 20; top: 20; right: 20; bottom: 20 }
            opacity: 0
            anchors.fill: parent
        }
    }

    Item {
        id: container

        // Having both icon and text simultaneously is unspecified but supported by implementation
        property int spacing: (icon.height && label.text) ? platformStyle.paddingSmall : 0
        property int contentWidth: Math.max(icon.width, label.textWidth)
        property int contentHeight: icon.height + spacing + label.height

        width: Math.min(contentWidth, button.width - 2 * internal.horizontalPadding)
        height: Math.min(contentHeight, button.height - 2 * internal.verticalPadding)
        clip: true
        anchors.centerIn: parent

        Image {
            id: icon
            source: button.iconSource
            sourceSize.width: platformStyle.graphicSizeSmall
            sourceSize.height: platformStyle.graphicSizeSmall
            smooth: true
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Text {
            id: label
            elide: Text.ElideRight
            property int textWidth: text ? privateStyle.textWidth(text, font) : 0
            anchors {
                top: icon.bottom
                topMargin: parent.spacing
                left: parent.left
                right: parent.right
            }
            height: text ? privateStyle.fontHeight(font) : 0
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeLarge }
            color: {
                if (!button.enabled)
                    return button.platformInverted ? platformStyle.colorDisabledLightInverted
                                                   : platformStyle.colorDisabledLight
                else if (button.pressed)
                    return button.platformInverted ? platformStyle.colorPressedInverted
                                                   : platformStyle.colorPressed
                else if (button.checked)
                    return button.platformInverted ? platformStyle.colorLatchedInverted
                                                   : platformStyle.colorLatched
                else
                    return button.platformInverted ? platformStyle.colorNormalLightInverted
                                                   : platformStyle.colorNormalLight
            }
        }
    }

    MouseArea {
        id: mouseArea

        anchors.fill: parent

        onPressed: stateGroup.state = "Pressed"

        onReleased: stateGroup.state = ""

        onCanceled: {
            // Mark as canceled
            stateGroup.state = "Canceled"
            // Reset state. Can't expect a release since mouse was ungrabbed
            stateGroup.state = ""
        }

        onPressAndHold: {
            if (stateGroup.state != "Canceled" && platformAutoRepeat)
                stateGroup.state = "AutoRepeating"
            button.platformPressAndHold()
        }

        onExited: stateGroup.state = "Canceled"
    }

    Timer {
        id: tapRepeatTimer

        interval: internal.autoRepeatInterval; running: false; repeat: true
        onTriggered: internal.repeat()
    }

    ParallelAnimation {
        id: clickedEffect
        PropertyAnimation {
            target: container
            property: "scale"
            from: 0.95
            to: 1.0
            easing.type: Easing.Linear
            duration: 100
        }
        PropertyAnimation {
            target: highlight
            property: "opacity"
            from: 1
            to: 0
            easing.type: Easing.Linear
            duration: 150
        }
    }

    Keys.onPressed: {
        if (event.key == Qt.Key_Select || event.key == Qt.Key_Return || event.key == Qt.Key_Enter) {
            stateGroup.state = "Pressed"
            event.accepted = true
        }
    }

    Keys.onReleased: {
        if (event.key == Qt.Key_Select || event.key == Qt.Key_Return || event.key == Qt.Key_Enter) {
            stateGroup.state = ""
            event.accepted = true
        }
    }
}
