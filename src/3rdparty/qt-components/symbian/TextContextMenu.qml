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
import "AppManager.js" as AppManager

Item {
    id: root

    property Item editor: null
    property real editorScrolledY: 0
    property bool copyEnabled: false
    property bool pasteEnabled: false
    property bool cutEnabled: false

    function show() {
        parent = AppManager.rootObject();
        internal.calculatePosition();
        root.visible = true;
    }

    function hide() {
        root.visible = false;
    }

    x: 0; y: 0
    visible: false

    // Private
    QtObject {
        id: internal
        // Reposition the context menu so that it centers on top of the selection
        function calculatePosition() {
            if (editor) {
                var rectStart = editor.positionToRectangle(editor.selectionStart);
                var rectEnd = editor.positionToRectangle(editor.selectionEnd);

                var posStart = editor.mapToItem(parent, rectStart.x, rectStart.y);
                var posEnd = editor.mapToItem(parent, rectEnd.x, rectEnd.y);

                var selectionCenterX = (posEnd.x + posStart.x) / 2;
                if (posStart.y != posEnd.y)
                    // we have multiline selection so center to the screen
                    selectionCenterX = parent.width / 2;

                var editorScrolledParent = editor.mapToItem(parent, 0, editorScrolledY);
                var contextMenuMargin = 10; // the space between the context menu and the line above/below
                var contextMenuAdjustedRowHeight = row.height + contextMenuMargin;

                var tempY = Math.max(editorScrolledParent.y - contextMenuAdjustedRowHeight,
                                     posStart.y - contextMenuAdjustedRowHeight);
                if (tempY < 0)
                    // it doesn't fit to the top -> try bottom
                    tempY = Math.min(editorScrolledParent.y + editor.height + contextMenuMargin,
                                     posEnd.y + rectEnd.height + contextMenuMargin);

                if (tempY + contextMenuAdjustedRowHeight > parent.height)
                    //it doesn't fit to the bottom -> center
                    tempY= (editorScrolledParent.y + editor.height) / 2 - row.height / 2;

                root.x = Math.max(0, Math.min(selectionCenterX - row.width / 2, parent.width - row.width));
                root.y = tempY;
            }
        }
    }

    ButtonRow {
        id: row

        function visibleButtonCount() {
            var count = 0
            if (copyButton.visible) ++count
            if (cutButton.visible) ++count
            if (pasteButton.visible) ++count
            return count
        }

        exclusive: false
        width: Math.round(privateStyle.buttonSize * 1.5) * visibleButtonCount()

        onWidthChanged: internal.calculatePosition()
        onHeightChanged: internal.calculatePosition()

        Button {
            id: copyButton
            iconSource: privateStyle.imagePath("qtg_toolbar_copy")
            visible: root.copyEnabled
            onClicked: editor.copy()
        }
        Button {
            id: cutButton
            iconSource: privateStyle.imagePath("qtg_toolbar_cut")
            visible: root.cutEnabled
            onClicked: {
                editor.cut()
                root.visible = false
            }
        }
        Button {
            id: pasteButton
            iconSource: privateStyle.imagePath("qtg_toolbar_paste")
            visible: root.pasteEnabled
            onClicked: {
                editor.paste()
                root.visible = false
            }
        }
    }
}
