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

var self
var clickHandlers = []
var visibleButtons = []
var resizing = false
var checkingOverallExclusivity = false
var checkedBtn

function create(that) {
    destroy()
    self = that

    // If the item is not visible at this stage, we store the value of the property
    // checkedButton to ensure a proper initialization. The value is restored in
    // initCheckedButton().
    if (!self.visible)
        checkedBtn = self.checkedButton

    buildItems()
    self.childrenChanged.connect(buildItems)
    self.widthChanged.connect(resizeButtons)
    self.exclusiveChanged.connect(checkOverallExclusivity)
    self.checkedButtonChanged.connect(checkOverallExclusivity)
    self.visibleChanged.connect(initCheckedButton)
}

function destroy() {
    if (self !== undefined) {
        self.childrenChanged.disconnect(buildItems)
        self.widthChanged.disconnect(resizeButtons)
        self.exclusiveChanged.disconnect(checkOverallExclusivity)
        self.checkedButtonChanged.disconnect(checkOverallExclusivity)
        self.visibleChanged.disconnect(initCheckedButton)
        releaseItemConnections()
        self = undefined
    }
}

function initCheckedButton() {
    // When the item becomes visible, restore the value of checkedButton property
    // that was stored in the create function.
    if (self.visible && checkedBtn !== null) {
        self.checkedButton = checkedBtn
        checkedBtn = null
    }
}

function buildItems() {
    releaseItemConnections()
    visibleButtons = []
    for (var i = 0; i < self.children.length; i++) {
        var item = self.children[i]

        // set up item connections
        clickHandlers[i] = checkExclusive(item)
        item.clicked.connect(clickHandlers[i])
        item.visibleChanged.connect(buildItems)

        // update visibleButtons array
        if (item.visible)
            visibleButtons.push(item)
    }
    checkOverallExclusivity()
    resizeButtons()
}

function releaseItemConnections() {
    for (var i = 0; i < self.children.length; i++) {
        self.children[i].clicked.disconnect(clickHandlers[i])
        self.children[i].visibleChanged.disconnect(buildItems)
    }
    clickHandlers = []
}

function checkOverallExclusivity() {
    if (!checkingOverallExclusivity && self.exclusive) {
        // prevent re-entrant calls
        checkingOverallExclusivity = true
        if (visibleButtons.length > 0) {
            if ((self.checkedButton === null || !self.checkedButton.visible))
                self.checkedButton = visibleButtons[0]
            self.checkedButton.checked = true
        }
        else {
            self.checkedButton = null
        }

        for (var i = 0; i < self.children.length; i++) {
            var item = self.children[i]
            // e.g CheckBox can be added to ButtonGroup but doesn't have "checkable" property
            if (self.exclusive && item.hasOwnProperty("checkable"))
                item.checkable = true
            if (item !== self.checkedButton)
                item.checked = false
        }
        checkingOverallExclusivity = false
    }
}

function checkExclusive(item) {
    var button = item
    return function() {
        if (self.exclusive) {
            for (var i = 0, ref; (ref = visibleButtons[i]); i++)
                ref.checked = button === ref
        }
        self.checkedButton = button
    }
}

function resizeButtons() {
    if (!resizing && visibleButtons.length && self.privateDirection == Qt.Horizontal) {
        // if ButtonRow has implicit size, a loop may occur where resizing individual
        // Button affects ButtonRow size, which triggers again resizing...
        // therefore we prevent re-entrant resizing attempts
        resizing = true
        var buttonWidth = self.width / visibleButtons.length
        for (var i = 0; i < self.children.length; i++) {
            self.children[i].width = self.children[i].visible ? buttonWidth : 0
        }
        resizing = false
    }
}

// Binding would not work properly if visibleButtons would just be returned,
// it would miss visibility changes. In the long run ButtonGroup.js could be
// refactored.
function visibleItems(item) {
    var visibleChildren = []
    for (var i = 0; i < item.children.length; i++) {
        if (item.children[i].visible)
            visibleChildren.push(item.children[i])
    }
    return visibleChildren
}
