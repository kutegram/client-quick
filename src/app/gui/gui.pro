VERSION = 0.2.0

include(../../library/library.pri)

include(../../../config.pri)
include(../../3rdparty/qt-components/components/components.pri)
include(../../3rdparty/qt-components/symbian/symbian.pri)
include(../../3rdparty/qt-components/symbian/extras/extras.pri)

CONFIG += qt mobility

QT += core declarative network xml
TARGET = Kutegram
DESTDIR = $$APP_INSTALL_BINS

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
DEFINES += QT_USE_FAST_CONCATENATION QT_USE_FAST_OPERATOR_PLUS

INCLUDEPATH += \
	"../"

HEADERS += \
    application.h \
    dialogslistmodel.h \
    historylistmodel.h \
    systemhandler.h \
    messageutils.h

SOURCES += \
	main.cpp \
    application.cpp \
    dialogslistmodel.cpp \
    historylistmodel.cpp \
    systemhandler.cpp \
    messageutils.cpp

!symbian:LIBS += -L$$APP_INSTALL_LIBS
symbian: include(platforms/symbian.pri)
unix:!macx:!android:!haiku:!symbian: include(platforms/nix.pri)

include(view/view.pri)
