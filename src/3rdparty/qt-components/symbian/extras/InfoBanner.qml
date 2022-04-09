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
import "Constants.js" as CONSTANTS

ImplicitSizeItem {
    id: root

    property alias iconSource: image.source
    property alias text: text.text
    property alias timeout : timer.interval
    property bool interactive : false

    signal clicked

    function open() {
        root.parent = internal.rootObject();
        internal.setZOrder();
        background.source = root.interactive ? privateStyle.imagePath("qtg_fr_popup_infobanner_normal")
                                             : privateStyle.imagePath("qtg_fr_popup_infobanner");
        stateGroup.state = "Visible";
        if (timer.interval)
            timer.restart();
    }
    function close() {
        stateGroup.state = "Hidden"
    }

    x: 0
    implicitHeight: internal.bannerHeight()
    implicitWidth: screen.width

    BorderImage {
        id: background

        anchors.fill: parent
        horizontalTileMode: BorderImage.Stretch
        verticalTileMode: BorderImage.Stretch
        border { left: CONSTANTS.INFOBANNER_BORDER_MARGIN; top: CONSTANTS.INFOBANNER_BORDER_MARGIN;
                 right: CONSTANTS.INFOBANNER_BORDER_MARGIN; bottom: CONSTANTS.INFOBANNER_BORDER_MARGIN }
        opacity: CONSTANTS.INFOBANNER_OPACITY

        Image {
            id: image

            anchors { top: parent.top; topMargin: platformStyle.paddingLarge;
                      left: parent.left; leftMargin:  platformStyle.paddingLarge; }
            sourceSize { width: platformStyle.graphicSizeSmall; height: platformStyle.graphicSizeSmall }
            visible: source != ""
        }

        Text {
            id: text

            anchors { top: parent.top; topMargin: internal.textTopMargin(); left: image.visible ? image.right : parent.left;
                      leftMargin: image.visible ? platformStyle.paddingMedium : platformStyle.paddingLarge }
            width: image.visible ? (root.width - image.width - platformStyle.paddingLarge * 2 - platformStyle.paddingMedium)
                                 : (root.width - platformStyle.paddingLarge * 2);
            verticalAlignment: Text.AlignVCenter
            color: mouseArea.pressed ?  platformStyle.colorPressed : platformStyle.colorNormalLight
            wrapMode: Text.Wrap
            font {
                pixelSize: platformStyle.fontSizeMedium
                family: platformStyle.fontFamilyRegular
            }
        }
    }

    Timer {
        id: timer
        interval: CONSTANTS.INFOBANNER_DISMISS_TIMER
        onTriggered: close()
    }

    MouseArea {
        id: mouseArea

        anchors.fill: parent
        onPressed: if (root.interactive) stateGroup.state = "Pressed"
        onReleased: if (root.interactive) stateGroup.state = "Released"
                    else stateGroup.state = "Hidden"
        onPressAndHold: if (root.interactive) {
                            if (timer.running) timer.stop()
                            stateGroup.state = "PressAndHold";
                        }
        onExited: if (root.interactive) {
                      if (stateGroup.state == "Pressed") {
                          stateGroup.state = "Cancelled";
                      } else if (stateGroup.state == "PressAndHold") {
                          stateGroup.state = "Cancelled";
                          if (timer.interval) timer.restart()
                      } else stateGroup.state = "Hidden"
                  }
    }

    QtObject {
        id: internal

        function rootObject() {
            var next = root.parent
            while (next && next.parent)
                next = next.parent
            return next
        }

        function setZOrder() {
            if (root.parent) {
                var maxZ = 0;
                var siblings = root.parent.children;
                for (var i = 0; i < siblings.length; ++i)
                    maxZ = Math.max(maxZ, siblings[i].z);
                root.z = maxZ + 1;
            }
        }

        function bannerHeight() {
            if (text.paintedHeight > privateStyle.fontHeight(text.font))
                return textTopMargin() + text.paintedHeight + platformStyle.paddingLarge;
            return Math.max(image.height, text.paintedHeight) + platformStyle.paddingLarge * 2;
        }

        function textTopMargin() {
            if (image.visible)
                return platformStyle.paddingLarge + image.height / 2 - privateStyle.fontHeight(text.font) / 2;
            return platformStyle.paddingLarge;
        }

        function press() {
            if (root.interactive) {
                privateStyle.play(Symbian.BasicButton);
                background.source = privateStyle.imagePath("qtg_fr_popup_infobanner_pressed");
            }
        }

        function release() {
            if (root.interactive)
                background.source = privateStyle.imagePath("qtg_fr_popup_infobanner_normal");
        }

        function click() {
            if (root.interactive) {
                privateStyle.play(Symbian.BasicButton);
                root.clicked();
            }
        }
    }

    StateGroup {
        id: stateGroup
        state: "Hidden"

        states: [
            State {
                name: "Visible"
                PropertyChanges { target: root; y: 0 }
            },
            State {
                name: "Hidden"
                PropertyChanges { target: root; y: -height }
            },
            State { name: "Pressed" },
            State { name: "PressAndHold" },
            State { name: "Released" },
            State { name: "Cancelled" }
        ]
        transitions: [
            Transition {
                to: "Pressed"
                ScriptAction { script: internal.press(); }
            },
            Transition {
                from: "Pressed"; to: "Released"
                ScriptAction { script: internal.release(); }
                ScriptAction { script: internal.click(); }
            },
            Transition {
                from: "Cancelled"; to: "Released"
                ScriptAction { script: internal.release(); }
            },
            Transition {
                from: "PressAndHold"; to: "Released"
                ScriptAction { script: internal.release(); }
                ScriptAction { script: internal.click(); }
            },
            Transition {
                from: "Hidden"; to: "Visible"
                SequentialAnimation {
                    NumberAnimation { target: root; properties: "y"; duration: CONSTANTS.INFOBANNER_ANIMATION_DURATION; easing.type: Easing.OutQuad }
                    ScriptAction { script: privateStyle.play(Symbian.PopUp) }
                }
            },
            Transition {
                to: "Hidden"
                NumberAnimation { target: root; properties: "y"; duration: CONSTANTS.INFOBANNER_ANIMATION_DURATION; easing.type: Easing.OutQuad }
            }
        ]
    }
}
