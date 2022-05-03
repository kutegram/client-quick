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
    messageutils.h \
    peerphotoprovider.h

SOURCES += \
	main.cpp \
    application.cpp \
    dialogslistmodel.cpp \
    historylistmodel.cpp \
    systemhandler.cpp \
    messageutils.cpp \
    peerphotoprovider.cpp

symbian {
    isEmpty(ICON):exists($${TARGET}.svg):ICON = $${TARGET}.svg

    ICON = view/skin/icons/hicolor/scalable/apps/kutegram.svg

    TARGET.UID3 = 0xE0713D51
    DEFINES += SYMBIAN_UID=$$TARGET.UID3
    DEFINES += BUILD_PLATFORM=\"Symbian\"
    TARGET.CAPABILITY += $$APP_CAPABILITY
    TARGET.EPOCHEAPSIZE = 0x400000 0x4000000
    TARGET.EPOCSTACKSIZE = 0x14000

    supported_platforms = \
        "[0x1028315F],0,0,0,{\"S60ProductID\"}" \ # Symbian^1
        "[0x20022E6D],0,0,0,{\"S60ProductID\"}" \ # Symbian^3
        "[0x102032BE],0,0,0,{\"S60ProductID\"}" \ # Symbian 9.2
        "[0x102752AE],0,0,0,{\"S60ProductID\"}" \ # Symbian 9.3
        "[0x2003A678],0,0,0,{\"S60ProductID\"}"   # Symbian Belle
	default_deployment.pkg_prerules -= pkg_platform_dependencies
	supported_platforms_deployment.pkg_prerules += supported_platforms
	DEPLOYMENT += supported_platforms_deployment

	vendor_info = \
		" " \
		"; Localised Vendor name" \
        "%{\"curoviyxru\"}" \
		" " \
		"; Unique Vendor name" \
        ":\"curoviyxru\"" \
		" "
    header = "$${LITERAL_HASH}{\"Kutegram\"},(0xE0713D51),0,2,0,TYPE=SA,RU"
	package.pkg_prerules += vendor_info header
	DEPLOYMENT += package
    DEPLOYMENT.installer_header = "$${LITERAL_HASH}{\"Kutegram Installer\"},(0xE5E0AFB2),0,2,0"

	for(plugin, APP_PLUGINS) {
		pluginstub = pluginstub$${plugin}
		pluginstubsources = $${pluginstub}.sources
		$$pluginstubsources = $$symbianRemoveSpecialCharacters($$eval($${plugin}.target)).dll
		pluginstubpath = $${pluginstub}.path
		$$pluginstubpath = $$APP_IMPORTS_BASE_DIR/$$eval($${plugin}.imports_path)

		resources = resources$${plugin}
		resourcessources = $${resources}.sources
		for(qmlfile, $$list($$eval($${plugin}.qml_files))) {
			$$resourcessources += $$APP_SOURCE_TREE/$$eval($${plugin}.source_path)/$$qmlfile
		}
		resourcespath = $${resources}.path
		$$resourcespath = $$APP_IMPORTS_BASE_DIR/$$eval($${plugin}.imports_path)

		DEPLOYMENT += $$pluginstub $$resources
	}

    contains(SYMBIAN_VERSION, Symbian3) {
        DEFINES += SYMBIAN3_READY=1
        LIBS += -lavkon \
            -laknnotify \
            -lhwrmlightclient \
            -lapgrfx \
            -lcone \
            -lws32 \
            -lbitgdi \
            -lfbscli \
            -laknskins \
            -laknskinsrv \
            -leikcore \
            -lapmime \
            -lefsrv \
            -leuser \
            -lcommondialogs \
            -lesock \
            -lmediaclientaudio \
            -lprofileengine \
            -lcntmodel \
            -lbafl \
            -lmgfetch
        message(Symbian^3 ready mode)
    }
} else {
	LIBS += -L$$APP_INSTALL_LIBS
}

include(view/view.pri)
