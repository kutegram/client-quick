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
import "Constants.js" as C
import "TumblerIndexHelper.js" as TH

CommonDialog {
    id: root

    property int year: 0
    property int month: 1
    property int day: 1
    property int minimumYear: 0
    property int maximumYear: 0
    property alias acceptButtonText: confirmButton.text
    property alias rejectButtonText: rejectButton.text

    // TODO do not dismiss the dialog when empty area is clicked

    content: Item {
        id: dialogContent
        height: tumbler.height + platformStyle.paddingLarge * 2
        width: parent.width

        Tumbler {
            id: tumbler

            function _handleTumblerChanges(index) {
                if (index == 0 || index == 2) {
                    var curYear = yearColumn.selectedIndex + yearList.get(0).value;
                    var curMonth = monthColumn.selectedIndex + 1;

                    var d = dateTime.daysInMonth(curYear, curMonth);

                    if (dayColumn.selectedIndex >= d)
                        dayColumn.selectedIndex = d - 1
                    while (dayList.count > d)
                        dayList.remove(dayList.count - 1)
                    while (dayList.count < d)
                        dayList.append({"value" : dayList.count + 1})
                }
            }

            columns:  [monthColumn, dayColumn, yearColumn]
            onChanged: {
                _handleTumblerChanges(index);
            }
            anchors.centerIn: parent
            height: privateStyle.menuItemHeight * 4
            width: parent.width - platformStyle.paddingMedium * 4
            privateDelayInit: true
            states: State {
                when: screen.currentOrientation == Screen.Landscape || screen.currentOrientation == Screen.LandscapeInverted
                PropertyChanges {
                    target: tumbler; height: privateStyle.menuItemHeight * 3
                }
            }

            TumblerColumn {
                id: dayColumn
                width: privateStyle.menuItemHeight
                items: ListModel {
                    id: dayList
                }
                selectedIndex: root.day - (root.day > 0 ?  1 : 0)
            }

            TumblerColumn {
                id: monthColumn
                privateTextAlignment: Text.AlignLeft
                items: ListModel {
                    id: monthList
                }
                selectedIndex: root.month - (root.month > 0 ?  1 : 0)
            }

            TumblerColumn {
                id: yearColumn
                items: ListModel {
                    id: yearList
                }
                selectedIndex: yearList.length > 0 ? root.year - yearList.get(0).value : 0
                privateResizeToFit: true
            }
        }
    }
    buttons: ToolBar {
        id: buttons
        width: parent.width
        height: privateStyle.toolBarHeightLandscape + 2 * platformStyle.paddingSmall
        Row {
            id: buttonRow
            anchors.centerIn: parent
            spacing: platformStyle.paddingMedium

            ToolButton {
                id: confirmButton
                width: (buttons.width - 3 * platformStyle.paddingMedium) / 2
                onClicked: accept()
                visible: text != ""
            }
            ToolButton {
                id: rejectButton
                width: (buttons.width - 3 * platformStyle.paddingMedium) / 2
                onClicked: reject()
                visible: text != ""
            }
        }
    }
    onMinimumYearChanged: {
        internal.updateYearList()
    }
    onMaximumYearChanged: {
        internal.updateYearList()
    }
    onStatusChanged: {
        if (status == DialogStatus.Opening) {
            if (!internal.initialised)
                internal.initializeDataModels();
            if (year > 0)
                yearColumn.selectedIndex = root.year - yearList.get(0).value;
            tumbler._handleTumblerChanges(2);
            TH.saveIndex(tumbler);
            dayColumn.selectedIndex = root.day - 1;
        }
    }
    onDayChanged: {
        if (dayColumn.items.length > root.day - 1)
            dayColumn.selectedIndex = root.day - 1
    }
    onMonthChanged: {
        monthColumn.selectedIndex = root.month - 1
    }
    onYearChanged: {
        if (internal.initialised)
            yearColumn.selectedIndex = root.year - yearList.get(0).value
    }
    onAccepted: {
        tumbler.privateForceUpdate();
        root.year = yearColumn.selectedIndex + yearList.get(0).value;
        root.month = monthColumn.selectedIndex + 1;
        root.day = dayColumn.selectedIndex + 1;
    }
    onRejected: {
        TH.restoreIndex(tumbler);
    }

    QtObject {
        id: internal

        property variant initialised: false

        function initializeDataModels() {
            var currentYear = new Date().getFullYear();
            minimumYear = minimumYear ? minimumYear : currentYear;
            maximumYear = maximumYear ? maximumYear : currentYear + 20;

            // ensure sane minimum & maximum year
            if (maximumYear < minimumYear)
                year = maximumYear = minimumYear = 0;  // use default values

            for (var y = minimumYear; y <= maximumYear; ++y)
                yearList.append({"value" : y}) // year

            var nDays = dateTime.daysInMonth(root.year, root.month);
            for (var d = 1; d <= nDays; ++d)
                dayList.append({"value" : d})  // day
            for (var m = 1; m <= 12; ++m)
                monthList.append({"value" : dateTime.longMonthName(m)});

            tumbler.privateInitialize()
            internal.initialised = true;
        }

        function updateYearList() {
            if (internal.initialised) {
                var tmp = yearColumn.selectedIndex;
                yearList.clear();
                for (var i = root.minimumYear; i <= root.maximumYear; ++i)
                    yearList.append({"value" : i})
                yearColumn.selectedIndex = tmp;
            }
        }
    }
}
