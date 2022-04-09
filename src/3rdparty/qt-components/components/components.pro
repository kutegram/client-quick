include(../../../../config.pri)
include(components.pri)

TARGETPATH = $$QT_COMPONENTS_PLUGIN.imports_path
TEMPLATE = lib
TARGET = $$QT_COMPONENTS_PLUGIN.target
INCLUDEPATH += $$PWD $$PWD/models

CONFIG += qt plugin
QT += declarative network script

DEFINES += QT_BUILD_COMPONENTS_LIB

QML_FILES += $$QT_COMPONENTS_PLUGIN.qml_files

symbian {
	TARGET.EPOCALLOWDLLDATA = 1
	TARGET.CAPABILITY += $$APP_CAPABILITY
	TARGET.UID3 = 0xE1E604E2
}

HEADERS += qglobalenums.h

SOURCES += plugin.cpp

include(kernel/kernel.pri)
include(models/models.pri)
include(../../../../install.pri)
