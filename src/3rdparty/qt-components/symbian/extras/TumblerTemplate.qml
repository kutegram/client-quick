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
import com.nokia.symbian 1.0
import "." 1.0
import "Constants.js" as C

Item {
    id: template
    objectName: "tumblerColumn" + index

    property Item tumblerColumn
    property int index: -1
    property bool firstColumn: false
    property Item view: viewContainer.item

    opacity: enabled ? C.TUMBLER_OPACITY_FULL : C.TUMBLER_OPACITY
    width: childrenRect.width
    visible: tumblerColumn ? tumblerColumn.visible : false
    enabled: tumblerColumn ? tumblerColumn.enabled : true
    onTumblerColumnChanged: {
        if (tumblerColumn)
            viewContainer.sourceComponent = tumblerColumn.privateLoopAround ? pViewComponent : lViewComponent
    }

    Loader {
        id: viewContainer
        anchors.left: parent.left
        width: tumblerColumn ? tumblerColumn.width + divider.width : 0
        height: root.height - container.height // decrease by text
    }

    BorderImage {
        id: divider
        anchors.left: parent.left
        height: firstColumn ? 0 : viewContainer.height
        width: firstColumn ? 0 : Math.round(platformStyle.paddingSmall / 2)
        source: privateStyle.imagePath("qtg_fr_tumbler_divider")
        border { 
            left: platformStyle.borderSizeMedium
            top: platformStyle.borderSizeMedium
            right: platformStyle.borderSizeMedium
            bottom: platformStyle.borderSizeMedium
        }
    }

    Component {
        // Component for loop around column
        id: pViewComponent

        PathView {
            id: pView

            model: tumblerColumn ? tumblerColumn.items : undefined
            currentIndex: tumblerColumn ? tumblerColumn.selectedIndex : 0
            // highlight locates in the middle (ratio 0.5) if items do not fully occupy the Tumbler
            preferredHighlightBegin: privateStyle.menuItemHeight * pView.count > height ?
                ((privateStyle.menuItemHeight / 2) + (pView.height / 2)) /
                    (privateStyle.menuItemHeight * pView.count) :
                0.5
            preferredHighlightEnd: preferredHighlightBegin
            highlightRangeMode: PathView.StrictlyEnforceRange
            clip: true
            delegate: defaultDelegate
            highlight: defaultHighlight
            interactive: template.enabled
            anchors.fill: parent

            onMovementStarted: {
                internal.movementCount++
            }
            onMovementEnded: {
                internal.movementCount--
                root.changed(template.index) // got index from delegate
            }
            // a workaround for QTCOMPONENTS-724 that is fixed in Qt 4.7.4 (QTBUG-15356)
            onPreferredHighlightEndChanged: { offset += 0.000001; offset -= 0.000001 }

            path: Path {
                startX: pView.width / 2;
                startY: privateStyle.menuItemHeight * pView.count > pView.height ?
                        -(privateStyle.menuItemHeight / 2) :
                        (pView.height - privateStyle.menuItemHeight * pView.count) / 2
                PathLine {
                    x: pView.width / 2
                    y: privateStyle.menuItemHeight * pView.count > pView.height ?
                        privateStyle.menuItemHeight * pView.count - (privateStyle.menuItemHeight / 2) :
                        (pView.height + privateStyle.menuItemHeight * pView.count) / 2
                }
            }
        }
    }

    Component {
        // Component for non loop around column
        id: lViewComponent

        ListView {
            id: lView

            model: tumblerColumn ? tumblerColumn.items : undefined
            currentIndex: tumblerColumn ? tumblerColumn.selectedIndex : 0
            preferredHighlightBegin: Math.floor((height - privateStyle.menuItemHeight) / 2)
            preferredHighlightEnd: preferredHighlightBegin + privateStyle.menuItemHeight
            highlightRangeMode: ListView.StrictlyEnforceRange
            clip: true
            delegate: defaultDelegate
            highlight: defaultHighlight
            interactive: template.enabled
            anchors.fill: parent

            onMovementStarted: {
                internal.movementCount++
            }
            onMovementEnded: {
                internal.movementCount--
                root.changed(template.index) // got index from delegate
            }
        }
    }

    Item {
        id: container
        anchors.top: viewContainer.bottom
        width: tumblerColumn ? tumblerColumn.width : 0
        height: internal.hasLabel ? privateStyle.menuItemHeight : 0 // internal.hasLabel is from root tumbler

        Text {
            id: label

            text: tumblerColumn ? tumblerColumn.label : ""
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: platformStyle.colorNormalLight
            font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeLarge }
            anchors.fill: parent
        }
    }

    Component {
        id: defaultDelegate

        Item {
            id: delegateItem
            width: tumblerColumn.width
            height: privateStyle.menuItemHeight

            Text {
                text: !!value ? value : ""
                elide: tumblerColumn.privateResizeToFit ? Text.ElideNone : Text.ElideRight
                horizontalAlignment: tumblerColumn.privateTextAlignment
                verticalAlignment: Text.AlignVCenter
                color: (tumblerColumn.privateLoopAround ? delegateItem.PathView.isCurrentItem : delegateItem.ListView.isCurrentItem) ?
                        platformStyle.colorHighlighted : platformStyle.colorNormalLight
                font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeLarge }
                anchors { fill: parent; margins: platformStyle.paddingLarge }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (template.view.interactive) {
                            tumblerColumn.selectedIndex = index;  // got index from delegate
                            root.changed(template.index);
                        }
                    }
                    onPressed: privateStyle.play(Symbian.BasicItem)
                    onReleased: {
                        if (template.view.currentIndex == index)
                            privateStyle.play(Symbian.BasicItem);
                    }
                }

                Component.onCompleted: {
                    if (tumblerColumn.privateResizeToFit && paintedWidth > templateInternal.columnFitWidth)
                        templateInternal.columnFitWidth = paintedWidth;
                    templateInternal.delegatesCount++;
                }
            }
        }
    }

    Component {
        id: defaultHighlight

        BorderImage {
            id: highlight
            objectName: "highlight"
            width: tumblerColumn ? tumblerColumn.width + divider.width : 0
            height: privateStyle.menuItemHeight
            source: privateStyle.imagePath("qtg_fr_tumbler_highlight")
            border { 
                left: platformStyle.borderSizeMedium
                top: platformStyle.borderSizeMedium
                right: platformStyle.borderSizeMedium
                bottom: platformStyle.borderSizeMedium
            }
        }
    }

    QtObject {
        id: templateInternal

        property int columnFitWidth: -1
        property int delegatesCount: 0

        onDelegatesCountChanged: {
            if (tumblerColumn.privateResizeToFit && delegatesCount == tumblerColumn.items.count)
                asyncTimer.running = true
        }
    }

    Timer {
        id: asyncTimer
        interval: 1
        onTriggered: {
            tumblerColumn.width = templateInternal.columnFitWidth + platformStyle.paddingLarge * 2;
        }
    }

    Connections {
        target: template.view
        onCurrentIndexChanged: {
            if (template.view.moving && !template.view.flicking)
                privateStyle.play(Symbian.ItemScroll);
        }
    }
}
