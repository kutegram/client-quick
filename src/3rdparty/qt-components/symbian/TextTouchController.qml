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
import "RectUtils.js" as Utils


MouseArea {
    id: root

    property Item editor: parent
    property real editorScrolledX: 0
    property real editorScrolledY: 0
    property alias copyEnabled: contextMenu.copyEnabled
    property alias pasteEnabled: contextMenu.pasteEnabled
    property alias cutEnabled: contextMenu.cutEnabled

    function updateGeometry() {
        selectionBegin.updateGeometry();
        selectionEnd.updateGeometry();
    }

    function flickEnded() {
        if (internal.editorHasSelection && internal.selectionVisible())
            contextMenu.show();
        else
            contextMenu.hide();
    }

    onPressed: {
        var currentTouchPoint = root.mapToItem(editor, mouse.x, mouse.y);

        if (currentTouchPoint.x < 0)
            currentTouchPoint.x = 0

        if (currentTouchPoint.y < 0)
            currentTouchPoint.y = 0

        if (internal.tapCounter == 0)
            internal.touchPoint = currentTouchPoint;

        if (!editor.readOnly)
            editor.forceActiveFocus();

        contextMenu.hide();
        internal.handleMoved = false;

        selectionBegin.viewPortRect = internal.mapViewPortRectToHandle(selectionBegin);
        selectionEnd.viewPortRect = internal.mapViewPortRectToHandle(selectionEnd);

        internal.pressedHandle = internal.handleForPoint({x: currentTouchPoint.x, y: currentTouchPoint.y});

        if (internal.pressedHandle != null) {
            internal.handleGrabbed(currentTouchPoint);
            // Position cursor at the pressed selection handle
            // TODO: Add bug ID!!
            var tempStart = editor.selectionStart
            var tempEnd = editor.selectionEnd
            if (internal.pressedHandle == selectionBegin) {
                editor.cursorPosition = editor.selectionStart
                editor.select(tempEnd, tempStart);
            } else {
                editor.cursorPosition = editor.selectionEnd
                editor.select(tempStart, tempEnd);
            }
        }
    }

    onClicked: {
        ++internal.tapCounter;
        if (internal.tapCounter == 1) {
            internal.onSingleTap();
            clickTimer.start();
        } else if (internal.tapCounter == 2 && clickTimer.running) {
            internal.onDoubleTap();
            clickTimer.restart();
        } else if (internal.tapCounter == 3 && clickTimer.running)
            internal.onTripleTap();
    }

    onPressAndHold: {
        clickTimer.stop();
        internal.tapCounter = 0;
        internal.longTap = true
        if (!internal.handleMoved) {
            if (internal.pressedHandle == null) {
                // position the cursor under the long tap and make the cursor handle grabbed
                editor.select(editor.cursorPosition, editor.cursorPosition);
                editor.cursorPosition = editor.positionAt(internal.touchPoint.x,internal.touchPoint.y);
                internal.pressedHandle = selectionEnd;
                if (editor.readOnly)
                    magnifier.hide();
                internal.handleGrabbed(internal.touchPoint);
            }
            contextMenu.hide();
        }

        if (!editor.readOnly || internal.editorHasSelection)
            magnifier.show();
    }

    onReleased: {
        magnifier.hide();

        mouseGrabDisabler.setKeepMouseGrab(root, false);
        internal.forcedSelection = false;

        if ((internal.pressedHandle != null && internal.handleMoved) ||
           (internal.longTap && !editor.readOnly) ||
           (internal.pressedHandle != null && internal.longTap))
            contextMenu.show();
        internal.longTap = false;
    }

    onMousePositionChanged: {

        internal.currentTouchPoint = root.mapToItem(editor, mouse.x, mouse.y);

        if (internal.pressedHandle != null) {
            internal.hitTestPoint = {x:internal.currentTouchPoint.x + internal.touchOffsetFromHitTestPoint.x,
                                     y:internal.currentTouchPoint.y + internal.touchOffsetFromHitTestPoint.y};

            var newPosition = editor.positionAt(internal.hitTestPoint.x, internal.hitTestPoint.y);
            if (newPosition >= 0 && newPosition != editor.cursorPosition) {
                if (internal.hasSelection) {
                    var anchorPos = internal.pressedHandle == selectionBegin ? editor.selectionEnd : editor.selectionStart
                    if (editor.selectionStart == editor.cursorPosition)
                        anchorPos = editor.selectionEnd;
                    else if (editor.selectionEnd == editor.cursorPosition)
                        anchorPos = editor.selectionStart;
                    editor.select(anchorPos, newPosition);
                } else {
                    editor.cursorPosition = newPosition;
                }
                magnifier.show();
                internal.handleMoved = true;
            }
        }
    }

    Connections {
        target: editor
        onTextChanged: internal.onEditorTextChanged
    }

    // Private
    QtObject {
        id: internal

        property bool forcedSelection: false
        property bool hasSelection: editorHasSelection || forcedSelection
        property bool editorHasSelection: editor.selectionStart != editor.selectionEnd
        property bool showHandleTouchArea: false // for debugging purposes
        property bool handleMoved: false
        property bool longTap: false
        property int tapCounter: 0
        property variant pressedHandle: null
        property variant hitTestPoint: Qt.point(0, 0)
        property variant touchOffsetFromHitTestPoint: Qt.point(0, 0)
        property variant touchPoint: Qt.point(0, 0)
        property variant currentTouchPoint: Qt.point(0, 0)

        function onSingleTap() {
            if (!internal.handleMoved) {
                // need to deselect, because if the cursor position doesn't change the selection remains
                // even after setting to cursorPosition
                editor.select(editor.cursorPosition, editor.cursorPosition);
                editor.cursorPosition = editor.positionAt(internal.touchPoint.x, internal.touchPoint.y);
                contextMenu.hide();
                if (!editor.readOnly)
                    editor.openSoftwareInputPanel()
            }
        }

        function onDoubleTap() {
            // assume that the 1st click positions the cursor
            editor.selectWord();
            contextMenu.show();
        }

        function onTripleTap() {
            editor.selectAll();
            contextMenu.show();
        }

        function onEditorTextChanged() {
            if (!internal.editorHasSelection)
                contextMenu.hide();
        }

        function handleGrabbed(currentTouchPoint) {
            mouseGrabDisabler.setKeepMouseGrab(root, true);
            internal.hitTestPoint = root.mapToItem(editor, internal.pressedHandle.center.x, internal.pressedHandle.center.y);
            internal.touchOffsetFromHitTestPoint = {x:internal.hitTestPoint.x - currentTouchPoint.x, y:internal.hitTestPoint.y - currentTouchPoint.y};

            internal.forcedSelection = internal.editorHasSelection;
        }

        function mapViewPortRectToHandle(handle) {
            var position = editor.mapToItem(handle, root.editorScrolledX, root.editorScrolledY);
            var rect = Qt.rect(position.x, position.y, root.width, root.height);
            return rect;
        }

        // point is in Editor's coordinate system
        function handleForPoint(point) {
            var pressed = null;

            if (!editor.readOnly || editorHasSelection) { // to avoid moving the cursor handle in read only editor
                // Find out which handle is being moved
                if (selectionBegin.visible &&
                    selectionBegin.containsPoint(editor.mapToItem(selectionBegin, point.x, point.y))) {
                    pressed = selectionBegin;
                }
                if (selectionEnd.containsPoint(editor.mapToItem(selectionEnd, point.x, point.y))) {
                    var useArea = true;
                    if (pressed != null) {
                        var distance1 = selectionBegin.pointDistanceFromCenter(point);
                        var distance2 = selectionEnd.pointDistanceFromCenter(point);

                        if (distance1 < distance2)
                            useArea = false;
                    }
                    if (useArea)
                        pressed = selectionEnd;
                }
            }
            return pressed;
        }

        function selectionVisible() {
            var startRect = editor.positionToRectangle(editor.selectionStart);
            var endRect = editor.positionToRectangle(editor.selectionEnd);
            var selectionRect = Qt.rect(startRect.x, startRect.y, endRect.x - startRect.x + endRect.width, endRect.y - startRect.y + endRect.height);
            var viewPortRect = Qt.rect(editorScrolledX, editorScrolledY, root.width, root.height);

            return Utils.rectIntersectsRect(selectionRect, viewPortRect) ||
                   Utils.rectContainsRect(viewPortRect, selectionRect) ||
                   Utils.rectContainsRect(selectionRect, viewPortRect);
        }
    }

    TextSelectionHandle {
        id: selectionBegin

        objectName: "SelectionBegin"
        imageSource: "qrc:/graphics/qtg_fr_textfield_handle_start.svg"
        editorPos: editor.selectionStart
        visible: editor.selectionStart != editor.selectionEnd
        showTouchArea: internal.showHandleTouchArea
    }

    TextSelectionHandle { // also acts as the cursor handle when no selection
        id: selectionEnd

        objectName: "SelectionEnd"
        imageSource: "qrc:/graphics/qtg_fr_textfield_handle_end.svg"
        editorPos: editor.selectionEnd
        visible: true
        showImage: internal.hasSelection //show image only in selection mode
        showTouchArea: internal.showHandleTouchArea
    }

    TextContextMenu {
        id: contextMenu

        editor: root.editor
        editorScrolledY: root.editorScrolledY
    }

    TextMagnifier {
        id: magnifier

        editor: root.editor
        contentCenter:internal.hitTestPoint
    }

    MouseGrabDisabler {
        id: mouseGrabDisabler
    }

    Timer {
        id: clickTimer

        interval: 400; repeat: false
        onTriggered: {
            running = false;
            internal.tapCounter = 0;
        }
    }

    Connections {
        target: root.editor
        onActiveFocusChanged: {
            // On focus loss dismiss menu, selection and VKB
            if (!root.editor.activeFocus) {
                contextMenu.hide()
                root.editor.select(root.editor.cursorPosition, root.editor.cursorPosition)
                root.editor.closeSoftwareInputPanel()
            }
        }
    }

}
