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

    property alias text: textArea.text
    property bool platformSubItemIndicator: false

    signal clicked

    width: parent.width; height: privateStyle.menuItemHeight

    QtObject {
        id: internal

        function bg_postfix() {
            if (activeFocus && symbian.listInteractionMode == Symbian.KeyNavigation)
                return "highlighted"
            else if (mouseArea.pressed && mouseArea.containsMouse && !mouseArea.canceled)
                return "pressed"
            else
                return "popup_normal"
        }

        function textColor() {
            if (activeFocus && symbian.listInteractionMode == Symbian.KeyNavigation)
                return platformStyle.colorHighlighted
            else if (mouseArea.pressed && mouseArea.containsMouse)
                return platformStyle.colorPressed
            else
                return platformStyle.colorNormalLight
        }
    }

    BorderImage {
        source: privateStyle.imagePath("qtg_fr_list_" + internal.bg_postfix())
        border { left: 20; top: 20; right: 20; bottom: 20 }
        anchors.fill: parent
    }

    Text {
        id: textArea
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: platformStyle.paddingLarge
            right: iconLoader.status == Loader.Ready ? iconLoader.left : parent.right
            rightMargin: iconLoader.status == Loader.Ready ? platformStyle.paddingMedium : privateStyle.scrollBarThickness
        }
        font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeMedium }
        color: internal.textColor()
        horizontalAlignment: Text.AlignLeft
        elide: Text.ElideRight
    }

    Loader {
        id: iconLoader
        sourceComponent: root.platformSubItemIndicator ? subItemIcon : undefined
        anchors {
            right: parent.right
            rightMargin: privateStyle.scrollBarThickness
            verticalCenter: parent.verticalCenter
        }
    }

    Component {
        id: subItemIcon

        Image {
            source: privateStyle.imagePath("qtg_graf_drill_down_indicator.svg")
            sourceSize.width: platformStyle.graphicSizeSmall
            sourceSize.height: platformStyle.graphicSizeSmall
        }
    }


    MouseArea {
        id: mouseArea

        property bool canceled: false

        anchors.fill: parent

        onPressed: {
            canceled = false
            symbian.listInteractionMode = Symbian.TouchInteraction
            privateStyle.play(Symbian.BasicItem)
        }
        onClicked: {
            if (!canceled)
                root.clicked()
        }
        onReleased: {
            if (!canceled)
                privateStyle.play(Symbian.PopupClose)
        }
        onExited: canceled = true
    }

    Keys.onPressed: {
        event.accepted = true
        switch (event.key) {
            case Qt.Key_Select:
            case Qt.Key_Enter:
            case Qt.Key_Return: {
                if (symbian.listInteractionMode != Symbian.KeyNavigation)
                    symbian.listInteractionMode = Symbian.KeyNavigation
                else
                    root.clicked()
                break
            }

            case Qt.Key_Up: {
                if (symbian.listInteractionMode != Symbian.KeyNavigation) {
                    symbian.listInteractionMode = Symbian.KeyNavigation
                    if (ListView.view != null)
                        ListView.view.positionViewAtIndex(index, ListView.Beginning)
                } else {
                    if (ListView.view != null)
                        ListView.view.decrementCurrentIndex()
                    else
                        event.accepted = false
                }
                break
            }

            case Qt.Key_Down: {
                if (symbian.listInteractionMode != Symbian.KeyNavigation) {
                    symbian.listInteractionMode = Symbian.KeyNavigation
                    if (ListView.view != null)
                        ListView.view.positionViewAtIndex(index, ListView.Beginning)
                } else {
                    if (ListView.view != null)
                        ListView.view.incrementCurrentIndex()
                    else
                        event.accepted = false
                }
                break
            }
            default: {
                event.accepted = false
                break
            }
        }
    }
}
