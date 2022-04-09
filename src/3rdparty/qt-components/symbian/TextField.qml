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

FocusScopeItem {
    id: root

    // Common Public API
    property alias placeholderText: placeholder.text
    property alias inputMethodHints: textInput.inputMethodHints
    property alias font: textInput.font
    property alias cursorPosition: textInput.cursorPosition
    property alias readOnly: textInput.readOnly
    property alias echoMode: textInput.echoMode
    property alias acceptableInput: textInput.acceptableInput
    property alias inputMask: textInput.inputMask
    property alias validator: textInput.validator
    property alias selectedText: textInput.selectedText
    property alias selectionEnd: textInput.selectionEnd
    property alias selectionStart: textInput.selectionStart
    property alias text: textInput.text
    property bool errorHighlight: !acceptableInput
    property alias maximumLength: textInput.maximumLength

    function copy() {
        textInput.copy()
    }

    function paste() {
        textInput.paste()
    }

    function cut() {
        textInput.cut()
    }

    function select(start, end) {
        textInput.select(start, end)
    }

    function selectAll() {
        textInput.selectAll()
    }

    function selectWord() {
        textInput.selectWord()
    }

    function positionAt(x) {
        return textInput.positionAt(mapToItem(textInput, x, 0).x)
    }

    function positionToRectangle(pos) {
        var rect = textInput.positionToRectangle(pos)
        rect.x = mapFromItem(textInput, rect.x, 0).x
        return rect;
    }

    // API extensions
    implicitWidth: platformLeftMargin + platformRightMargin + flick.tiny * 2 +
                   Math.max(privateStyle.textWidth(text, textInput.font), priv.minWidth)
    implicitHeight: Math.max(privateStyle.textFieldHeight,
                             2 * platformStyle.paddingMedium + privateStyle.fontHeight(textInput.font))

    property bool enabled: true // overriding due to QTBUG-15797 and related bugs

    property real platformLeftMargin: platformStyle.paddingSmall
    property real platformRightMargin: platformStyle.paddingSmall

    // Private data
    QtObject {
        id: priv
        // Minimum width is either placeholder text lenght or 5 spaces on current font.
        // Using placeholder text lenght as minimum width prevents implicit sized item from
        // shrinking on focus gain.
        property real minWidth: placeholder.text ? privateStyle.textWidth(placeholder.text, textInput.font)
                                                 : privateStyle.textWidth("     ", textInput.font)

        function bg_postfix() {
            if (root.errorHighlight)
                return "error"
            else if (root.readOnly || !root.enabled)
                return "uneditable"
            else if (textInput.activeFocus)
                return "highlighted"
            else
                return "editable"
        }
    }

    BorderImage {
        id: frame
        anchors.fill: parent
        source: privateStyle.imagePath("qtg_fr_textfield_" + priv.bg_postfix())
        border {
            left: platformStyle.borderSizeMedium
            top: platformStyle.borderSizeMedium
            right: platformStyle.borderSizeMedium
            bottom: platformStyle.borderSizeMedium
        }
        smooth: true
    }

    Item {
        id: container

        anchors {
            left: root.left; leftMargin: root.platformLeftMargin
            right: root.right; rightMargin: root.platformRightMargin
            verticalCenter : root.verticalCenter
        }
        clip: true
        height: root.height

        Flickable {
            id: flick

            property real tiny: Math.round(platformStyle.graphicSizeTiny / 2)

            function ensureVisible(rect) {
                if (Math.round(contentX) > Math.round(rect.x))
                    contentX = rect.x
                else if (Math.round(contentX + width) < Math.round(rect.x + rect.width))
                  contentX = rect.x + rect.width - width
            }

            anchors {
                left: parent.left; leftMargin: tiny
                right: parent.right; rightMargin: tiny
                verticalCenter : parent.verticalCenter
            }

            boundsBehavior: Flickable.StopAtBounds
            contentWidth: textInput.paintedWidth
            height: textInput.paintedHeight

            TextInput {
                id: textInput; objectName: "textInput"

                property real paintedWidth: textInput.model.paintedWidth
                property real paintedHeight: textInput.model.paintedHeight
                property variant cursorRectangle: textInput.positionToRectangle(textInput.cursorPosition)

                // TODO: See TODO: Refactor implicit size...
                property variant model: Text {
                    font: textInput.font
                    text: textInput.text
                    horizontalAlignment: textInput.horizontalAlignment
                    visible: false
                    opacity: 0
                }

                autoScroll: false
                cursorVisible: activeFocus && !selectedText
                enabled: root.enabled
                color: platformStyle.colorNormalDark
                focus: true
                font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeMedium }
                selectedTextColor: platformStyle.colorNormalLight
                selectionColor: platformStyle.colorTextSelection
                width: Math.max(flick.width, paintedWidth)

                onEnabledChanged: {
                    if (!enabled) {
                        select(0, 0)
                        // De-focusing requires setting focus elsewhere, in this case editor's parent
                        if (root.parent)
                            root.parent.forceActiveFocus()
                    }
                }

                onCursorPositionChanged: flick.ensureVisible(textInput.positionToRectangle(textInput.cursorPosition))

                TextTouchController {
                    id: touchController

                    anchors {
                        left: parent.left;
                        leftMargin: -flick.tiny
                        verticalCenter : parent.verticalCenter
                    }
                    height: root.height
                    width: Math.max(root.width, flick.contentWidth + flick.tiny * 2)
                    editorScrolledX: flick.contentX
                    editorScrolledY: flick.contentY
                    copyEnabled: textInput.selectedText
                    cutEnabled: !textInput.readOnly && textInput.selectedText
                    // TODO: QtQuick 1.1 has textEdit.canPaste
                    pasteEnabled: !textInput.readOnly
                    Component.onCompleted: flick.movementEnded.connect(touchController.flickEnded)
                    Connections { target: screen; onCurrentOrientationChanged: touchController.updateGeometry() }
                    Connections {
                        target: textInput
                        onHeightChanged: touchController.updateGeometry()
                        onWidthChanged: touchController.updateGeometry()
                        onHorizontalAlignmentChanged: touchController.updateGeometry()
                        onFontChanged: touchController.updateGeometry()
                    }
                }
            }
        }

        Text {
            id: placeholder; objectName: "placeholder"
            anchors {
                left: parent.left; leftMargin: flick.tiny
                right: parent.right; rightMargin: flick.tiny
                verticalCenter : parent.verticalCenter
            }
            color: platformStyle.colorNormalMid
            font: textInput.font
            visible: (!textInput.activeFocus || readOnly) && !textInput.text && text
        }

    }
}
