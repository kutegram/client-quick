QML_IMPORT_PATH = $$APP_INSTALL_IMPORTS

SIMULATOR_QML_FILES += \
	view/skin/layout/main.qml \
	view/skin/layout/DemoPage.qml

OTHER_FILES += $$SIMULATOR_QML_FILES $$SIMULATOR_QML_IMAGES

RESOURCES += view/skin/app.qrc

!symbian {
	qmlsource = $$PWD/skin
	qmlsource = $$replace(qmlsource, /, \\)
	qmltarget = $$APP_INSTALL_RESOURCES
	qmltarget = $$replace(qmltarget, /, \\)
	!isEqual($$qmlsource, $$qmltarget) {
		copydir.commands = $(COPY_DIR) \"$$qmlsource\" \"$$qmltarget\"
		first.depends = $(first) copydir
		QMAKE_EXTRA_TARGETS += first copydir
	}
} else:symbian {
	qml_resources.sources = $$QML_FILES
	qml_resources.path = $$APP_RESOURCES_BASE_DIR
	images_resources.sources = $$QML_IMAGES
	images_resources.path = $$APP_RESOURCES_BASE_DIR\\images
	DEPLOYMENT += qml_resources images_resources
}
