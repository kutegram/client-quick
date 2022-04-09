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

    property alias content: contentArea.children
    property Popup containingPopup: null

    signal itemClicked()

    height: flickableArea.height

    QtObject {
        id: internal
        // Add padding in context menu case to align content area bottom with rounded background graphics.
        property int clipMargin: containingPopup.objectName != "OptionsMenu" ? platformStyle.paddingSmall : 0
        property int preferredHeight: privateStyle.menuItemHeight * ((screen.width < screen.height) ? 5 : 3)
    }

    BorderImage {
        source: containingPopup.objectName == "OptionsMenu"
            ? privateStyle.imagePath("qtg_fr_popup_options") : privateStyle.imagePath("qtg_fr_popup") 
        border { left: 20; top: 20; right: 20; bottom: 20 }
        anchors.fill: parent
    }

    Item {
        height: flickableArea.height - internal.clipMargin
        width: root.width
        clip: true
        Flickable {
            id: flickableArea

            property int index: 0
            property bool itemAvailable: (contentArea.children[0] != undefined) && (contentArea.children[0].children[0] != undefined)
            property int itemHeight: itemAvailable ? Math.max(1, contentArea.children[0].children[0].height) : 1
            property int interactionMode: symbian.listInteractionMode

            height: contentArea.height; width: root.width
            clip: true
            contentHeight: contentArea.childrenRect.height
            contentWidth: width

            Item {
                id: contentArea

                property int itemsHidden: Math.floor(flickableArea.contentY / flickableArea.itemHeight)

                width: flickableArea.width
                height: childrenRect.height > internal.preferredHeight
                    ? internal.preferredHeight - (internal.preferredHeight % flickableArea.itemHeight)
                    : childrenRect.height

                onWidthChanged: {
                    for (var i = 0; i < children.length; ++i)
                        children[i].width = width
                }

                onItemsHiddenChanged: {
                    // Check that popup is really open in order to prevent unnecessary feedback
                    if (containingPopup.status == DialogStatus.Open
                        && symbian.listInteractionMode == Symbian.TouchInteraction)
                        privateStyle.play(Symbian.ItemScroll)
                }

                Component.onCompleted: {
                    for (var i = 0; i < children.length; ++i) {
                        if (children[i].clicked != undefined)
                            children[i].clicked.connect(root.itemClicked)
                    }
                }
            }

            onVisibleChanged: {
                enabled = visible
                if (itemAvailable)
                    contentArea.children[0].children[0].focus = visible
                contentY = 0
                index = 0
            }

            onInteractionModeChanged: {
                if (symbian.listInteractionMode == Symbian.KeyNavigation) {
                    contentY = 0
                    if (itemAvailable)
                        contentArea.children[0].children[index].focus = true
                } else if (symbian.listInteractionMode == Symbian.TouchInteraction) {
                    index = 0
                }
            }

            Keys.onPressed: {
                if (itemAvailable && (event.key == Qt.Key_Down || event.key == Qt.Key_Up)) {
                    if (event.key == Qt.Key_Down && index < contentArea.children[0].children.length - 1) {
                        index++
                        if (index * itemHeight > contentY + height - itemHeight) {
                            contentY = index * itemHeight - height + itemHeight
                            scrollBar.flash(Symbian.FadeOut)
                        }
                    } else if (event.key == Qt.Key_Up && index > 0) {
                        index--
                        if (index * itemHeight < contentY) {
                            contentY = index * itemHeight
                            scrollBar.flash(Symbian.FadeOut)
                        }
                    }
                    contentArea.children[0].children[index].focus = true
                    event.accepted = true
                }
            }
        }

        ScrollBar {
            id: scrollBar
            flickableItem: flickableArea
            interactive: false
            visible: flickableArea.height < flickableArea.contentHeight
            anchors {
                top: flickableArea.top
                right: flickableArea.right
            }
        }
    }

    Connections {
        target: containingPopup
        onStatusChanged: {
            if (containingPopup.status == DialogStatus.Open)
                scrollBar.flash(Symbian.FadeInFadeOut)
        }
    }
}
