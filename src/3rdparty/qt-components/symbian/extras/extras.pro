include(../../../../../config.pri)
include(extras.pri)

TARGETPATH = $$EXTRAS_PLUGIN.imports_path
TEMPLATE = lib
TARGET = $$EXTRAS_PLUGIN.target
INCLUDEPATH += $$PWD

CONFIG += qt plugin
QT += declarative network script

HEADERS += \
    sdatetime.h

SOURCES += \
    plugin.cpp \
    sdatetime.cpp

QML_FILES = $$EXTRAS_PLUGIN.qml_files

symbian {
	TARGET.EPOCALLOWDLLDATA = 1
	TARGET.CAPABILITY += $$APP_CAPABILITY
    TARGET.UID3 = 0xE1E604E3
}

include(../../../../../install.pri)
