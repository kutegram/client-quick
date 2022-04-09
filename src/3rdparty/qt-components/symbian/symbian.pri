SYMBIAN_PLUGIN.source_path = \
	src/3rdparty/qt-components/symbian

SYMBIAN_PLUGIN.imports_path = \
    com/nokia/symbian

SYMBIAN_PLUGIN.target = \
	$$qtLibraryTarget(embedded_symbian_plugin)

SYMBIAN_PLUGIN.qml_files += \
    qmldir \
    ApplicationWindow.qml \
    AppManager.js \
    BusyIndicator.qml \
    Button.qml \
    ButtonColumn.qml \
    ButtonGroup.js \
    ButtonRow.qml \
    CheckBox.qml \
    CommonDialog.qml \
    ContextMenu.qml \
    Dialog.qml \
    Fader.qml \
    Label.qml \
    ListHeading.qml \
    ListItem.qml \
    ListItemText.qml \
    Menu.qml \
    MenuContent.qml \
    MenuItem.qml \
    MenuLayout.qml \
    Page.qml \
    PageStack.js \
    PageStack.qml \
    Popup.qml \
    ProgressBar.qml \
    QueryDialog.qml \
    RadioButton.qml \
    RectUtils.js \
    ScrollBar.qml \
    ScrollDecorator.qml \
    SectionScroller.js \
    SectionScroller.qml \
    SelectionDialog.qml \
    SelectionListItem.qml \
    Slider.qml \
    StatusBar.qml \
    Switch.qml \
    TabBar.qml \
    TabBarLayout.qml \
    TabButton.qml \
    TabGroup.js \
    TabGroup.qml \
    TextArea.qml \
    TextField.qml \
    TextMagnifier.qml \
    TextContextMenu.qml \
    TextSelectionHandle.qml \
    TextTouchController.qml \
    ToolBar.qml \
    ToolBarLayout.qml \
    ToolButton.qml \
    ToolTip.qml \
    Window.qml

APP_PLUGINS += SYMBIAN_PLUGIN
