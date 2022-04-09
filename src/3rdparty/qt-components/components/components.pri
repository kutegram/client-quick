QT_COMPONENTS_PLUGIN.source_path = src/3rdparty/qt-components/components

QT_COMPONENTS_PLUGIN.imports_path = \
    Qt/labs/components

QT_COMPONENTS_PLUGIN.target = \
	$$qtLibraryTarget(embedded_qt_components_plugin)

QT_COMPONENTS_PLUGIN.qml_files += \
    qmldir \
    Checkable.qml \
    CheckableGroup.qml \
    CheckableGroup.js

APP_PLUGINS += QT_COMPONENTS_PLUGIN
