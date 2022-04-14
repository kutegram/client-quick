include(../../../../config.pri)
include(symbian.pri)

TARGETPATH = $$SYMBIAN_PLUGIN.imports_path
TEMPLATE = lib
TARGET = $$SYMBIAN_PLUGIN.target
INCLUDEPATH += $$PWD $$PWD/indicators

CONFIG += qt plugin
QT += declarative svg network script

load(mobilityconfig)
contains(MOBILITY_CONFIG, systeminfo) {
    CONFIG += mobility
    MOBILITY += feedback systeminfo
    DEFINES += HAVE_MOBILITY
    message(Mobility API enabled)
} else {
    message(Mobility API not available)
}

SOURCES += \
	plugin.cpp \
	sbatteryinfo.cpp \
	sdeclarative.cpp \
	sdeclarativefocusscopeitem.cpp \
	sdeclarativeicon.cpp \
	sdeclarativeimageprovider.cpp \
	sdeclarativeimplicitsizeitem.cpp \
	sdeclarativemagnifier.cpp \
	sdeclarativemaskedimage.cpp \
	sdeclarativescreen.cpp \
	sdeclarativestyle.cpp \
	sdeclarativestyleinternal.cpp \
	siconpool.cpp \
	smousegrabdisabler.cpp \
	snetworkinfo.cpp \
	spopupmanager.cpp \
	sstyleengine.cpp \
	sstylefactory.cpp \
	indicators/sdeclarativeindicatorcontainer.cpp \
	indicators/sdeclarativenetworkindicator.cpp

symbian:symbian_internal {
	SOURCES += \
		indicators/sdeclarativeindicator.cpp \
		indicators/sdeclarativeindicatordata.cpp \
		indicators/sdeclarativeindicatordatahandler.cpp \
		indicators/sdeclarativenetworkindicator_p_symbian.cpp \
		indicators/sdeclarativestatuspanedatasubscriber.cpp
} else {
	SOURCES += \
		indicators/sdeclarativenetworkindicator_p.cpp
}

HEADERS += \
	sbatteryinfo.h \
	sdeclarative.h \
	sdeclarativefocusscopeitem.h \
	sdeclarativeicon.h \
	sdeclarativeimageprovider.h \
	sdeclarativeimplicitsizeitem.h \
	sdeclarativemagnifier.h \
	sdeclarativemaskedimage.h \
	sdeclarativemaskedimage_p.h \
	sdeclarativescreen.h \
	sdeclarativescreen_p.h \
	sdeclarativestyle.h \
	sdeclarativestyleinternal.h \
	siconpool.h \
	smousegrabdisabler.h \
	snetworkinfo.h \
	spopupmanager.h \
	sstyleengine.h \
	sstylefactory.h \
	indicators/sdeclarativeindicatorcontainer.h \
	indicators/sdeclarativenetworkindicator.h \
	indicators/sdeclarativenetworkindicator_p.h

symbian:symbian_internal {
	HEADERS +=  \
		indicators/sdeclarativeindicator.h \
		indicators/sdeclarativeindicatordata.h \
		indicators/sdeclarativeindicatordatahandler.h \
		indicators/sdeclarativestatuspanedatasubscriber.h
}

RESOURCES += \
	symbian.qrc

QML_FILES += $$SYMBIAN_PLUGIN.qml_files

symbian {
	TARGET.EPOCALLOWDLLDATA = 1
	TARGET.CAPABILITY += $$APP_CAPABILITY
	TARGET.UID3 = 0xE1E604E4

	LIBS += -lws32 // For CWsScreenDevice
	LIBS += -lcone // For EikonEnv / CoeEnv
	LIBS += -leikcore // For EikonEnv
	LIBS += -leikcoctl // For CEikStatusPane
	LIBS += -lavkon // For AknAppui SetOrientationL
	LIBS += -lhal   // For calculating DPI values
}

win32: LIBS += -lpsapi # for allocated memory info

include(../../../../install.pri)
