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
import "AppManager.js" as AppView

ImplicitSizeItem {
    id: root

    //API
    property alias font: label.font
    property alias text: label.text
    property variant target: null

    implicitWidth: Math.min(privy.maxWidth, (privateStyle.textWidth(text, font) + privy.margin * 2))
    implicitHeight: privateStyle.fontHeight(font) + privy.margin * 2

    onVisibleChanged: {
        if (visible) {
            root.parent = AppView.rootObject()
            privy.calculatePosition()
        }
    }

    QtObject {
        id: privy

        property real margin: platformStyle.paddingMedium
        property real spacing: platformStyle.paddingLarge
        property real maxWidth: screen.width - spacing * 2

        function calculatePosition() {
            if (!target)
                return

            // Determine and set the main position for the ToolTip, order: top, right, left, bottom
            var targetPos = root.parent.mapFromItem(target, 0, 0)

            // Top
            if (targetPos.y >= (root.height + privy.margin + privy.spacing)) {
                root.x = targetPos.x + (target.width / 2) - (root.width / 2)
                root.y = targetPos.y - root.height - privy.margin

            // Right
            } else if (targetPos.x <= (screen.width - target.width - privy.margin - root.width - privy.spacing)) {
                root.x = targetPos.x + target.width + privy.margin;
                root.y = targetPos.y + (target.height / 2) - (root.height / 2)

            // Left
            } else if (targetPos.x >= (root.width + privy.margin + privy.spacing)) {
                root.x = targetPos.x - root.width - privy.margin
                root.y = targetPos.y + (target.height / 2) - (root.height / 2)

            // Bottom
            } else {
                root.x = targetPos.x + (target.width / 2) - (root.width / 2)
                root.y = targetPos.y + target.height + privy.margin
            }

            // Fine-tune the ToolTip position based on the screen borders
            if (root.x > (screen.width - privy.spacing - root.width))
                root.x = screen.width - root.width - privy.spacing

            if (root.x < privy.spacing)
                root.x = privy.spacing;

            if (root.y > (screen.height - root.height - privy.spacing))
                root.y = screen.height - root.height - privy.spacing

            if (root.y < privy.spacing)
                root.y = privy.spacing
        }
    }

    BorderImage {
        id: frame
        anchors.fill: parent
        source: privateStyle.imagePath("qtg_fr_popup")
        border { left: 20; top: 20; right: 20; bottom: 20 }
    }

    Text {
       id: label
       clip: true
       color: platformStyle.colorNormalLight
       elide: Text.ElideRight
       font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeSmall }
       verticalAlignment: Text.AlignVCenter
       horizontalAlignment: Text.AlignHCenter
       anchors.fill: parent
       anchors.margins: privy.margin
    }
}
