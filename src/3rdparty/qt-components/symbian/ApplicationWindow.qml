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

Window {
    id: root

    property bool fullScreen: false
    default property alias content: contentItem.data
    property alias pageStack: stack
	property alias busyIndicatorRunning: busyIndicator.running
	property alias busyText: busyText.text

    Component.onCompleted: console.log("warning: ApplicationWindow is an experimental component. Use Window instead.")

    Item {
        anchors.top: sbar.bottom
        anchors.bottom: tbar.top
        anchors.left: parent.left
        anchors.right: parent.right
        Item {
            id: contentItem
            anchors.fill: parent
            PageStack {
                id: stack
                anchors.fill: parent
                toolBar: tbar
            }
        }
    }

    StatusBar {
        id: sbar

        width: parent.width
        state: root.fullScreen ? "Hidden" : "Visible"

        states: [
            State {
                name: "Visible"
                PropertyChanges { target: sbar; y: 0; opacity: 1 }
            },
            State {
                name: "Hidden"
                PropertyChanges { target: sbar; y: -height; opacity: 0 }
            }
        ]

        transitions: [
            Transition {
                from: "Hidden"; to: "Visible"
                ParallelAnimation {
                    NumberAnimation { target: sbar; properties: "y"; duration: 200; easing.type: Easing.OutQuad }
                    NumberAnimation { target: sbar; properties: "opacity"; duration: 200; easing.type: Easing.Linear }
                }
            },
            Transition {
                from: "Visible"; to: "Hidden"
                ParallelAnimation {
                    NumberAnimation { target: sbar; properties: "y"; duration: 200; easing.type: Easing.Linear }
                    NumberAnimation { target: sbar; properties: "opacity"; duration: 200; easing.type: Easing.Linear }
                }
            }
        ]
    }
	
	BusyIndicator {
		id: busyIndicator
		anchors { top: sbar.top; left: sbar.left }
		width: height; height: sbar.height
		visible: running
	}
	
	Text {
        id: busyText
        width: Math.round(privateStyle.statusBarHeight * 44 / 26)
        color: platformStyle.colorNormalLight
        verticalAlignment: Text.AlignVCenter
		anchors {
			left: busyIndicator.right; leftMargin: platformStyle.paddingSmall
			top: busyIndicator.top; bottom: busyIndicator.bottom
		}
        font {
            family: platformStyle.fontFamilyRegular;
            pixelSize: Math.round(privateStyle.statusBarHeight * 18 / 26)
            weight: Font.Light
        }
    }

    ToolBar {
        id: tbar

        width: parent.width
        state: root.fullScreen ? "Hidden" : "Visible"

        states: [
            State {
                name: "Visible"
                PropertyChanges { target: tbar; y: parent.height - height; opacity: 1 }
            },
            State {
                name: "Hidden"
                PropertyChanges { target: tbar; y: parent.height; opacity: 0 }
            }
        ]

        transitions: [
            Transition {
                from: "Hidden"; to: "Visible"
                ParallelAnimation {
                    NumberAnimation { target: tbar; properties: "y"; duration: 200; easing.type: Easing.OutQuad }
                    NumberAnimation { target: tbar; properties: "opacity"; duration: 200; easing.type: Easing.Linear }
                }
            },
            Transition {
                from: "Visible"; to: "Hidden"
                ParallelAnimation {
                    NumberAnimation { target: tbar; properties: "y"; duration: 200; easing.type: Easing.Linear }
                    NumberAnimation { target: tbar; properties: "opacity"; duration: 200; easing.type: Easing.Linear }
                }
            }
        ]
    }

    // event preventer when page transition is active
    MouseArea {
        anchors.fill: parent
        enabled: pageStack.busy
    }
}
