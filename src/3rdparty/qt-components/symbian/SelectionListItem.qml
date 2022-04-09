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

ListItem {
    id: root
    property string title: ""
    property string subTitle: ""
    implicitHeight: background.height + 2 * platformStyle.paddingLarge

    onModeChanged: {
        if (root.mode == "pressed") {
            pressed.source = privateStyle.imagePath("qtg_fr_choice_list_pressed")
            pressed.opacity = 1
        } else {
            releasedEffect.restart()
        }
    }

    BorderImage {
        id: background
        height: privateStyle.menuItemHeight - platformStyle.paddingSmall // from layout spec.
        anchors {
            left: parent.left
            leftMargin: platformStyle.paddingLarge
            right: parent.right
            rightMargin: privateStyle.scrollBarThickness
            verticalCenter: parent.verticalCenter
        }
        border {
            left: platformStyle.borderSizeMedium
            top: platformStyle.borderSizeMedium
            right: platformStyle.borderSizeMedium
            bottom: platformStyle.borderSizeMedium
        }
        source: privateStyle.imagePath("qtg_fr_choice_list_") + internal.getBackground()

        BorderImage {
            id: pressed
            border {
                left: platformStyle.borderSizeMedium
                top: platformStyle.borderSizeMedium
                right: platformStyle.borderSizeMedium
                bottom: platformStyle.borderSizeMedium
            }
            opacity: 0
            anchors.fill: parent
        }

        Column {
            anchors {
                verticalCenter: background.verticalCenter
                right: indicator.left
                rightMargin: platformStyle.paddingMedium
                left: background.left
                leftMargin: platformStyle.paddingLarge
            }

            Loader {
                sourceComponent: title != "" ? titleText : undefined
                width: parent.width // elide requires explicit width
            }

            Loader {
                sourceComponent: subTitle != "" ? subTitleText : undefined
                width: parent.width // elide requires explicit width
            }
        }
        Image {
            id: indicator
            source: root.mode == "disabled" ? privateStyle.imagePath("qtg_graf_choice_list_indicator_disabled") :
                                              privateStyle.imagePath("qtg_graf_choice_list_indicator")
            sourceSize.width: platformStyle.graphicSizeSmall
            sourceSize.height: platformStyle.graphicSizeSmall
            anchors {
                right: background.right
                rightMargin: platformStyle.paddingSmall
                verticalCenter: parent.verticalCenter
            }
        }
    }

    Component {
        id: titleText
        ListItemText {
            mode: root.mode
            role: "SelectionTitle"
            text: root.title
        }
    }
   Component {
        id: subTitleText
        ListItemText {
            mode: root.mode
            role: "SelectionSubTitle"
            text: root.subTitle
        }
    }

    QtObject {
        id: internal
        function getBackground() {
            if (root.mode == "highlighted")
                return "highlighted"
            else if (root.mode == "disabled")
                return "disabled"
            else
                return "normal"
        }
    }

    SequentialAnimation {
        id: releasedEffect
        PropertyAnimation {
            target: pressed
            property: "opacity"
            to: 0
            easing.type: Easing.Linear
            duration: 150
        }
    }
}
