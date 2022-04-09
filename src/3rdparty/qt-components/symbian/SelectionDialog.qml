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

CommonDialog {
    id: root

    // Common API
    property alias model: listView.model
    property int selectedIndex: -1
    property Component delegate: defaultDelegate

    privateCloseIcon: true

    Component {
        id: defaultDelegate

        MenuItem {
            text: modelData
            onClicked: {
                selectedIndex = index
                root.accept()
            }
        }
    }

    content: Item {
        id: contentItem

        function preferredHeight() {
            // Need to create artifical binding to listView.delegate because of QTBUG-19037
            var dummy = listView.delegate
            var multiplier = screen.height > screen.width ? 0.45 : 0.6
            var maxHeight = Math.floor(multiplier * screen.height / privateStyle.menuItemHeight)
                * privateStyle.menuItemHeight
            return Math.min(maxHeight, listView.count * privateStyle.menuItemHeight)
        }

        height: preferredHeight()
        width: root.platformContentMaximumWidth

        Item {
            // Clipping item with bottom margin added to align content with rounded background graphics
            anchors.fill: parent
            anchors.bottomMargin: platformStyle.paddingSmall
            clip: true
            ListView {
                id: listView

                currentIndex : -1
                width: contentItem.width
                height: contentItem.height
                delegate: root.delegate
                clip: true

                // Flash scrollbar when navigating to hidden
                // listItem with hw keys
                onContentYChanged: {
                    if (!moving)
                        scrollBar.flash(Symbian.FadeOut)
                }

                Keys.onPressed: {
                    if (event.key == Qt.Key_Up || event.key == Qt.Key_Down)
                        symbian.listInteractionMode = Symbian.KeyNavigation
                }
            }

            ScrollBar {
                id: scrollBar

                flickableItem: listView
                interactive: false
                visible: listView.contentHeight > contentItem.height
                anchors { top: listView.top; right: listView.right }
            }
        }
    }

    onClickedOutside: {
        privateStyle.play(Symbian.PopupClose)
        reject()
    }

    onStatusChanged: {
        if (status == DialogStatus.Opening) {
            if (listView.currentItem != null) {
                listView.currentItem.focus = false
            }
            listView.currentIndex = -1
            listView.positionViewAtIndex(0, ListView.Beginning)
        }
        else if (status == DialogStatus.Open) {
            listView.focus = true
            if (listView.contentHeight > contentItem.height)
                scrollBar.flash(Symbian.FadeInFadeOut)
        }
    }
}
