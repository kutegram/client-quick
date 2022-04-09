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
import Qt.labs.components 1.0 as QtComponents

ImplicitSizeItem {
    id: root

    // Common Public API
    property alias stepSize: model.stepSize
    property alias minimumValue: model.minimumValue
    property alias maximumValue: model.maximumValue
    property alias value: model.value
    property int orientation: Qt.Horizontal
    property bool pressed: track.inputActive
    property bool valueIndicatorVisible: false
    property string valueIndicatorText: ""
    property alias inverted: model.inverted

    // Symbian specific
    signal valueChanged(real value)
    implicitWidth: orientation == Qt.Horizontal ? 150 : privateStyle.menuItemHeight
    implicitHeight: orientation == Qt.Horizontal ? privateStyle.menuItemHeight : 150

    onActiveFocusChanged: {
        if (!root.activeFocus)
            track.activeKey = undefined
    }

    QtComponents.RangeModel {
        id: model
        value: 0.0
        stepSize: 0.0
        minimumValue: 0.0
        maximumValue: 1.0
        positionAtMinimum: 0
        positionAtMaximum: orientation == Qt.Horizontal ? track.width - handle.width : track.height - handle.height
        onValueChanged: root.valueChanged(value)
    }

    BorderImage {
        id: track
        objectName: "track"
        property bool tapOnTrack: false
        property variant activeKey
        property bool inputActive: handleMouseArea.pressed || !!track.activeKey
        onInputActiveChanged: {
            if (!valueIndicatorVisible)
                return
            if (inputActive)
                valueIndicator.show = true
            else
                indicatorTimer.restart()
        }

        states: [
            State {
                name: "Horizontal"
                when: orientation == Qt.Horizontal

                PropertyChanges {
                    target: track
                    height: privateStyle.sliderThickness
                    width: undefined
                    source: privateStyle.imagePath("qtg_fr_slider_h_track_normal")
                    border {
                        left: 20
                        right: 20
                        top: 0
                        bottom: 0
                    }
                    smooth: true
                }

                AnchorChanges {
                    target: track
                    anchors {
                        left: root.left
                        right: root.right
                        top: undefined
                        bottom: undefined
                        horizontalCenter: undefined
                        verticalCenter: root.verticalCenter
                    }
                }
            },
            State {
                name: "Vertical"
                when: orientation == Qt.Vertical

                PropertyChanges {
                    target: track
                    height: undefined
                    width: privateStyle.sliderThickness
                    source: privateStyle.imagePath("qtg_fr_slider_v_track_normal")
                    border {
                        left: 0
                        right: 0
                        top: 20
                        bottom: 20
                    }
                    smooth: true
                }

                AnchorChanges {
                    target: track
                    anchors {
                        left: undefined
                        right: undefined
                        top: root.top
                        bottom: root.bottom
                        horizontalCenter: root.horizontalCenter
                        verticalCenter: undefined
                    }
                }
            }
        ]

        anchors.leftMargin: platformStyle.paddingMedium
        anchors.rightMargin: platformStyle.paddingMedium
        anchors.topMargin: platformStyle.paddingMedium
        anchors.bottomMargin: platformStyle.paddingMedium

        MouseArea {
            id: trackMouseArea
            objectName: "trackMouseArea"

            anchors.fill: parent

            onClicked: {
                if (track.tapOnTrack) {
                    if (orientation == Qt.Horizontal) {
                        if (handle.x > (mouseX - handle.width / 2)) {
                            model.value = inverted ? model.value + model.stepSize : model.value - model.stepSize
                            handle.x = model.position
                        } else {
                            model.value = inverted ? model.value - model.stepSize : model.value + model.stepSize
                            handle.x = model.position
                        }
                    } else {
                        if (handle.y > (mouseY - handle.height / 2)) {
                            model.value = inverted ? model.value + model.stepSize : model.value - model.stepSize
                            handle.y = model.position
                        } else {
                            model.value = inverted ? model.value - model.stepSize : model.value + model.stepSize
                            handle.y = model.position
                        }
                    }
                }
            }

            Image {
                id: handle
                objectName: "handle"
                x: orientation == Qt.Horizontal ? (handleMouseArea.pressed ? x : model.position) : 0
                y: orientation == Qt.Horizontal ? 0 : (handleMouseArea.pressed ? y : model.position)

                sourceSize.height: platformStyle.graphicSizeSmall
                sourceSize.width: platformStyle.graphicSizeSmall

                anchors.verticalCenter: orientation == Qt.Horizontal ? parent.verticalCenter : undefined
                anchors.horizontalCenter: orientation == Qt.Horizontal ? undefined : parent.horizontalCenter

                source: {
                    var handleIcon = "qtg_graf_slider_"
                        + (orientation == Qt.Horizontal ? "h" : "v")
                        + "_handle_"
                        + (handleMouseArea.pressed ? "pressed" : "normal")
                    privateStyle.imagePath(handleIcon)
                }

                onXChanged: valueIndicator.position()
                onYChanged: valueIndicator.position()

                MouseArea {
                    id: handleMouseArea
                    objectName: "handleMouseArea"

                    height: platformStyle.graphicSizeMedium
                    width: platformStyle.graphicSizeMedium
                    anchors.verticalCenter: orientation == Qt.Horizontal ? parent.verticalCenter : undefined
                    anchors.horizontalCenter: orientation == Qt.Horizontal ? undefined : parent.horizontalCenter

                    drag.target: handle
                    drag.axis: Drag.XandYAxis
                    drag.minimumX: orientation == Qt.Horizontal ? model.positionAtMinimum : 0
                    drag.maximumX: orientation == Qt.Horizontal ? model.positionAtMaximum : 0
                    drag.minimumY: orientation == Qt.Horizontal ? 0 : model.positionAtMinimum
                    drag.maximumY: orientation == Qt.Horizontal ? 0 : model.positionAtMaximum

                    onPositionChanged: model.position = orientation == Qt.Horizontal ? handle.x : handle.y
                    onPressed: privateStyle.play(Symbian.BasicSlider)
                    onReleased: privateStyle.play(Symbian.BasicSlider)
                }
            }
        }
    }

    Keys.onPressed: {
        internal.handleKeyPressEvent(event)
    }

    Keys.onReleased: {
        internal.handleKeyReleaseEvent(event)
    }

    Component {
        id: valueIndicatorComponent
        ToolTip { text: root.valueIndicatorText == "" ? model.value : root.valueIndicatorText }
    }

    Loader {
        id: valueIndicator

        property int spacing: 2 * platformStyle.paddingLarge
        // Must match with the "maxWidth" padding defined in ToolTip
        property int toolTipPadding: platformStyle.paddingLarge
        property bool show: false
        sourceComponent: valueIndicator.show ? valueIndicatorComponent : undefined
        onLoaded: position()

        function position() {
            if (!valueIndicatorVisible || status != Loader.Ready)
                return

            var point = null
            if (orientation == Qt.Horizontal) {
                point = root.mapFromItem(track, handle.x + handle.width / 2 - valueIndicator.item.width / 2, 0)

                // Check if valueIndicator will be positioned beyond the right or
                // left boundaries and adjust if needed to keep it fully
                // visible on screen. In case the valueIndicator is so wide that it
                // does not fit the screen, it's positioned to left of the screen.
                var rightStop = screen.width - toolTipPadding
                var valueIndicatorLeftEdge = root.mapToItem(null, point.x, 0)
                var valueIndicatorRightEdge = root.mapToItem(null, point.x + valueIndicator.item.width, 0)

                if (valueIndicatorLeftEdge.x < toolTipPadding)
                    point.x = root.mapFromItem(null, toolTipPadding, 0).x
                else if (valueIndicatorRightEdge.x > rightStop)
                    point.x = root.mapFromItem(null, rightStop - valueIndicator.item.width, 0).x

                valueIndicator.item.x = point.x
                valueIndicator.item.y = point.y - valueIndicator.spacing - valueIndicator.item.height
            } else {
                point = root.mapFromItem(track, 0, handle.y + handle.height / 2 - valueIndicator.item.height / 2)
                valueIndicator.item.x = point.x - valueIndicator.spacing - valueIndicator.item.width
                valueIndicator.item.y = point.y
            }
        }

        Timer {
            id: indicatorTimer
            interval: 750
            onTriggered: {
                if (!track.inputActive)
                    valueIndicator.show = false
            }
        }
    }

    QtObject {
        id: internal

        function handleKeyPressEvent(keyEvent) {
            var oldValue = model.value
            if (orientation == Qt.Horizontal) {
                if (keyEvent.key == Qt.Key_Left) {
                    model.value = inverted ? model.value + model.stepSize : model.value - model.stepSize
                } else if (keyEvent.key == Qt.Key_Right) {
                    model.value = inverted ? model.value - model.stepSize : model.value + model.stepSize
                }
            } else { //Vertical
                if (keyEvent.key == Qt.Key_Up) {
                    model.value = inverted ? model.value + model.stepSize : model.value - model.stepSize
                } else if (keyEvent.key == Qt.Key_Down) {
                    model.value = inverted ? model.value - model.stepSize : model.value + model.stepSize
                }
            }
            if (oldValue != model.value)
                keyEvent.accepted = true

            if (keyEvent.accepted ||
                keyEvent.key == Qt.Key_Select ||
                keyEvent.key == Qt.Key_Return ||
                keyEvent.key == Qt.Key_Enter)
                track.activeKey = keyEvent.key
        }

        function handleKeyReleaseEvent(keyEvent) {
            if (track.activeKey == keyEvent.key) {
                if (!keyEvent.isAutoRepeat)
                    track.activeKey = undefined
                keyEvent.accepted = true
            }
        }
    }
}
