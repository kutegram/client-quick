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

ImplicitSizeItem {
    id: root

    // Common Public API
    property Item tab
    property bool checked: internal.tabGroup != null && internal.tabGroup.currentTab == tab
    property bool pressed: stateGroup.state == "Pressed" && mouseArea.containsMouse
    property alias text: label.text
    property alias iconSource: imageLoader.source

    signal clicked

    implicitWidth: Math.max(2 * imageLoader.width, 2 * platformStyle.paddingMedium + privateStyle.textWidth(label.text, label.font))
    implicitHeight: internal.portrait ? privateStyle.tabBarHeightPortrait : privateStyle.tabBarHeightLandscape

    QtObject {
        id: internal

        property Item tabGroup: Utils.findParent(tab, "currentTab")
        property bool portrait: screen.currentOrientation == Screen.Portrait || screen.currentOrientation == Screen.PortraitInverted

        function press() {
            privateStyle.play(Symbian.BasicButton)
        }
        function click() {
            root.clicked()
            privateStyle.play(Symbian.BasicButton)
            if (internal.tabGroup)
                internal.tabGroup.currentTab = tab
        }

        function isButtonRow(item) {
            return (item &&
                    item.hasOwnProperty("checkedButton") &&
                    item.hasOwnProperty("privateDirection") &&
                    item.privateDirection == Qt.Horizontal)
        }

        function imageName() {
            // If the parent of a TabButton is ButtonRow, segmented-style graphics
            // are used to create a seamless row of buttons. Otherwise normal
            // TabButton graphics are utilized.
            if (isButtonRow(parent))
                return parent.privateGraphicsName(root, 1)
            else
                return "qtg_fr_tab_"
        }

        function modeName() {
            if (isButtonRow(parent)) {
                return parent.privateModeName(root, 1)
            } else {
                return root.checked ? "active" : "passive_normal"
            }
        }

        function modeNamePressed() {
            if (isButtonRow(parent)) {
                return "pressed"
            } else {
                return "passive_pressed"
            }
        }
    }

    StateGroup {
        id: stateGroup

        states: [
            State {
                name: "Pressed"
                PropertyChanges { target: pressedGraphics; opacity: 1 }
            },
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
                NumberAnimation {
                    target: pressedGraphics
                    property: "opacity"
                    to: 0
                    duration: 150
                    easing.type: Easing.Linear
                }
                ScriptAction { script: internal.click() }
            }
        ]
    }

    BorderImage {
        id: background

        source: privateStyle.imagePath(internal.imageName() + internal.modeName())
        anchors.fill: parent
        border {
            left: platformStyle.borderSizeMedium
            top: platformStyle.borderSizeMedium
            right: platformStyle.borderSizeMedium
            bottom: platformStyle.borderSizeMedium
        }
    }

    BorderImage {
        id: pressedGraphics

        source: privateStyle.imagePath(internal.imageName() + internal.modeNamePressed())
        anchors.fill: parent
        opacity: 0

        border {
            left: platformStyle.borderSizeMedium
            top: platformStyle.borderSizeMedium
            right: platformStyle.borderSizeMedium
            bottom: platformStyle.borderSizeMedium
        }
    }

    Text {
        id: label

        objectName: "label"
        // hide in landscape if icon is present
        visible: !(iconSource.toString() && !internal.portrait)
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            leftMargin: platformStyle.paddingMedium
            rightMargin: platformStyle.paddingMedium
            bottomMargin: iconSource.toString()
                ? platformStyle.paddingSmall
                : Math.ceil((parent.height - label.height) / 2.0)
        }
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
        font {
            family: platformStyle.fontFamilyRegular;
            pixelSize: platformStyle.fontSizeSmall
            weight: Font.Light
        }
        color: {
            if (root.pressed)
                platformStyle.colorPressed
            else if (root.checked)
                platformStyle.colorNormalLight
            else
                platformStyle.colorNormalMid
        }
    }

    Loader {
        // imageLoader acts as wrapper for Image and Icon items. The Image item is
        // shown when the source points to a image (jpg, png). Icon item is used for
        // locigal theme icons which are colorised.
        id: imageLoader

        property url source
        property string iconId: source.toString()
		
/* Commented because of the bug QTCOMPONENTS-920: https://bugreports.qt-project.org/browse/QTCOMPONENTS-920		
  
        property string iconId

        // function tries to figure out if the source is image or icon
        function inspectSource() {
            var fileName = imageLoader.source.toString()
            if (fileName.length) {
                fileName = fileName.substr(fileName.lastIndexOf("/") + 1)
                var fileBaseName = fileName
                if (fileName.indexOf(".") >= 0)
                    fileBaseName = fileName.substr(0, fileName.indexOf("."))

                // empty file extension and .svg are treated as icons
                if (fileName == fileBaseName || fileName.substr(fileName.length - 4).toLowerCase() == ".svg")
                    imageLoader.iconId = fileBaseName
                else
                    imageLoader.iconId = ""
            } else {
                imageLoader.iconId = ""
            }
        }

        Component.onCompleted: inspectSource()
        onSourceChanged: inspectSource()
*/
        width : platformStyle.graphicSizeSmall
        height : platformStyle.graphicSizeSmall
        sourceComponent: {
            if (iconId)
                return iconComponent
            if (source.toString())
                return imageComponent
            return undefined
        }
        anchors {
            top: parent.top
            topMargin : !parent.text || !internal.portrait
                ? Math.floor((parent.height - imageLoader.height) / 2.0)
                : platformStyle.paddingSmall
            horizontalCenter: parent.horizontalCenter
        }

        Component {
            id: imageComponent

            Image {
                id: image

                objectName: "image"
                source: imageLoader.iconId ? "" : imageLoader.source
                sourceSize.width: width
                sourceSize.height: height
                fillMode: Image.PreserveAspectFit
                smooth: true
                anchors.fill: parent
            }
        }

        Component {
            id: iconComponent

            Icon {
                id: icon

                objectName: "icon"
                anchors.fill: parent
                iconColor: label.color
                iconName: imageLoader.iconId
            }
        }
    }

    MouseArea {
        id: mouseArea

        onPressed: stateGroup.state = "Pressed"
        onReleased: stateGroup.state = ""
        onCanceled: {
            // Mark as canceled
            stateGroup.state = "Canceled"
            // Reset state
            stateGroup.state = ""
        }
        onExited: {
            if (pressed)
                stateGroup.state = "Canceled"
        }

        anchors.fill: parent
    }
}
