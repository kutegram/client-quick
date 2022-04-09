include(../../../config.pri)
include(../../3rdparty/qt-components/components/components.pri)
include(../../3rdparty/qt-components/symbian/symbian.pri)
include(../../3rdparty/qt-components/symbian/extras/extras.pri)

CONFIG += qt mobility

QT += core declarative network
TARGET = EmbeddedComponents
DESTDIR = $$APP_INSTALL_BINS

INCLUDEPATH += \
	"../"

HEADERS += \
    application.h

SOURCES += \
	main.cpp \
    application.cpp

symbian {
	isEmpty(ICON):exists($${TARGET}.svg):ICON = $${TARGET}.svg

	TARGET.UID3 = 0xE1E604E5
	TARGET.CAPABILITY += $$APP_CAPABILITY
	TARGET.EPOCHEAPSIZE = 0x400000 0x4000000
	TARGET.EPOCSTACKSIZE = 0x14000

	supported_platforms = \
		"; S60 5th Edition / Symbian^1 " \
		"[0x1028315F],0,0,0,{\"S60ProductID\"}" \
		"; Symbian^3 (including Symbian Anna) " \
		"[0x20022E6D],0,0,0,{\"S60ProductID\"}" \
		"; Symbian Belle " \
		"[0x2003A678],0,0,0,{\"S60ProductID\"}"
	default_deployment.pkg_prerules -= pkg_platform_dependencies
	supported_platforms_deployment.pkg_prerules += supported_platforms
	DEPLOYMENT += supported_platforms_deployment

	vendor_info = \
		" " \
		"; Localised Vendor name" \
		"%{\"Pavel Osipov\"}" \
		" " \
		"; Unique Vendor name" \
		":\"Pavel Osipov\"" \
		" "
	header = "$${LITERAL_HASH}{\"Embedded Components\"},(0xE1E604E5),1,0,0,TYPE=SA,RU"
	package.pkg_prerules += vendor_info header
	DEPLOYMENT += package
	DEPLOYMENT.installer_header = "$${LITERAL_HASH}{\"Demo Installer\"},(0x2002CCCF),1,0,0"

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
} else {
	LIBS += -L$$APP_INSTALL_LIBS
}

include(view/view.pri)
