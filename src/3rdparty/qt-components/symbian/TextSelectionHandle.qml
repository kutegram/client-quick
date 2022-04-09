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
import "RectUtils.js" as Utils

Item {
    id: root

    property alias imageSource: image.source
    property int editorPos: 0 //character position of the handle in the editor document
    property Item editor: parent.editor
    // the default size of the touch area should come from some style constant
    property variant touchAreaSize: Qt.size(platformStyle.graphicSizeMedium, platformStyle.graphicSizeMedium)
    property alias showImage: image.visible
    property bool showTouchArea: false // for debugging purposes
    property variant viewPortRect: Qt.rect(0, 0, 0, 0) // item's coordinates
    //parent's coordinate system
    property variant center: Qt.point(root.x + root.width / 2, root.y + root.height / 2)

    // Point is in Item's coordinate system
    function containsPoint(point) {
        var touchAreaRect = Qt.rect(touchArea.x, touchArea.y, touchArea.width, touchArea.height);

        if (!Utils.rectContainsRect(viewPortRect, touchAreaRect)
            && Utils.rectIntersectsRect(viewPortRect, touchAreaRect)) {
                touchAreaRect.x = Math.max(touchAreaRect.x, viewPortRect.x);
                touchAreaRect.x = Math.min(touchAreaRect.x, viewPortRect.x + viewPortRect.width - touchAreaRect.width);
                touchAreaRect.y = Math.max(touchAreaRect.y, viewPortRect.y);
                touchAreaRect.y = Math.min(touchAreaRect.y, viewPortRect.y + viewPortRect.height - touchAreaRect.height);
        }

        effectiveTouchArea.x = touchAreaRect.x;
        effectiveTouchArea.y = touchAreaRect.y;

        if (Utils.rectContainsPoint(touchAreaRect, point.x, point.y))
            return true;

        return false;
    }

    // Point is in parent's coordinate system
    function pointDistanceFromCenter(point) {
        return Utils.manhattan(point, root.center);
    }

    function updateGeometry() {
        var geometry = internal.geometryFromEditorPos(root.editorPos);
        root.x = geometry.x;
        root.y = geometry.y;
        root.width = geometry.width;
        root.height = geometry.height;
    }

    onEditorPosChanged: root.updateGeometry()

    // Private
    QtObject {
        id: internal

        // Returns handle geometry from editor position
        function geometryFromEditorPos(editorPos) {
            var rect = root.editor.positionToRectangle(editorPos);
            var pos = root.editor.mapToItem(root.parent,rect.x,rect.y);
            return Qt.rect(pos.x,pos.y,rect.width,rect.height);
        }
    }

    // This could be an Item, but it’s a Rectangle for debugging purposes.
    Rectangle {
        id: touchArea

        visible: root.showTouchArea
        anchors.centerIn: root
        width: root.touchAreaSize.width
        height: Math.max(root.touchAreaSize.height, root.height)
        border.width: 1
        border.color: "white"
        color: "#00000000"
    }

    // This could be an Item, but it’s a Rectangle for debugging purposes.
    Rectangle {
        id: effectiveTouchArea

        visible: root.showTouchArea
        width: root.touchAreaSize.width
        height: Math.max(root.touchAreaSize.height, root.height)
        border.width: 1
        border.color: "yellow"
        color: "#00000000"
    }

    BorderImage {
        id: image

        property real tiny: Math.round(platformStyle.graphicSizeTiny / 2)

        anchors {
            left: root.objectName == "SelectionEnd" ? root.right : undefined
            right: root.objectName == "SelectionBegin" ? root.left : undefined
            leftMargin: root.objectName == "SelectionEnd" ? -editor.cursorRectangle.width : 0
            top: root.top
            topMargin: -tiny / 2 + 1
            bottom: root.bottom
            bottomMargin: -tiny / 2
        }

        border {
            left: 0
            top: tiny
            right: 0
            bottom: tiny
        }
    }
}
