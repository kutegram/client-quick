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
import "AppManager.js" as AppManager

Magnifier {
    id: root

    property Item editor: parent.editor
    // Defines the center of the content to be magnified in X axis
    // Note: the center of the content to be magnified in Y axis is determined by the cursor position.
    property variant contentCenter: Qt.point(0, 0) //editor's coordinate system

    function show() {
        parent = AppManager.rootObject();
        sourceRect = internal.calculateSourceGeometry();
        internal.calculatePosition();
        root.visible = true;
    }

    function hide() {
        root.visible = false;
    }

    sourceRect: Qt.rect(0, 0, 0, 0)
    visible: false
    scaleFactor: 1.5
    maskFileName: ":/graphics/qtg_graf_magnifier_mask.svg"
    overlayFileName: ":/graphics/qtg_graf_magnifier.svg"

    onContentCenterChanged: internal.updateSourceRect()

    onSourceRectChanged: if (visible) internal.calculatePosition()

    Connections {
        target: editor
        onCursorPositionChanged: internal.updateSourceRect
    }

    // Private
    QtObject {
        id: internal

        function updateSourceRect () {
            if (visible)
                sourceRect = internal.calculateSourceGeometry();
        }

        function calculatePosition() {
            width = sourceRect.width * scaleFactor;
            height = sourceRect.height * scaleFactor;

            var magnifierMargin = -platformStyle.paddingLarge; // the offset between the magnifier and top of the line
            var pos = parent.mapFromItem(editor,
                                         contentCenter.x - width/2,
                                         contentCenter.y - sourceRect.height/2 - height - magnifierMargin);

            root.x = pos.x;
            root.y = pos.y;
        }

        // Calculates and returns the source geometry of the content to be magnified in scene coordinates
        function calculateSourceGeometry() {
            var magniferSize = Qt.size(platformStyle.graphicSizeMedium * 2, platformStyle.graphicSizeMedium * 2);
            var sourceSize = Qt.size(magniferSize.width / scaleFactor, magniferSize.height / scaleFactor);
            var contentCenterScene = editor.mapToItem(null, contentCenter.x, contentCenter.y);

            var rect = Qt.rect(contentCenterScene.x - sourceSize.width / 2,
                               contentCenterScene.y - sourceSize.height / 2,
                               sourceSize.width, sourceSize.height);
            return rect;
        }
    }
}
