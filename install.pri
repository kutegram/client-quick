isEmpty(TARGETPATH):error($$basename(_PRO_FILE_) must define TARGETPATH)
DESTDIR = $$APP_INSTALL_IMPORTS/$$member(TARGETPATH, 0)

NATIVE_FILES = $$QML_FILES native/qmldir
NATIVE_FILES -= qmldir

!symbian {
	for(qmlfile, QML_FILES) {
		ARGUMENTS = $$_PRO_FILE_PWD_/$$qmlfile $$DESTDIR
		target = copy_$$lower($$basename(qmlfile))
		target = $$replace(target, \\., _)
		commands = $${target}.commands
		$$commands += $$QMAKE_COPY $$replace(ARGUMENTS, /, $$QMAKE_DIR_SEP)
		QMAKE_EXTRA_TARGETS += $$target
		POST_TARGETDEPS += $$target
	}
}

OTHER_FILES += $$QML_FILES

target.path = $$APP_INSTALL_IMPORTS/$$member(TARGETPATH, 0)
INSTALLS += target

for(targetpath, $$list($$unique(TARGETPATH))) {
	installpath = $$APP_INSTALL_IMPORTS/$$targetpath
	installpath = $$replace(installpath, \\\\, /)
	!isEqual(targetpath, $$member(TARGETPATH, 0)) {
		qmltarget = qmltarget_$$replace(targetpath, /, _)
		eval($${qmltarget}.CONFIG += no_check_exist executable)
		eval($${qmltarget}.files = $$DESTDIR/$(TARGET))
		eval($${qmltarget}.files += stfu)
		eval($${qmltarget}.path = $$installpath)
		INSTALLS += $${qmltarget}
	}

	qmlfiles = qmlfiles_$$replace(targetpath, /, _)
	eval($${qmlfiles}.files = $$QML_FILES)
	eval($${qmlfiles}.path = $$installpath)

	qmlimages = qmlimages_$$replace(targetpath, /, _)
	eval($${qmlimages}.files = $$QML_IMAGES)
	eval($${qmlimages}.path = $$installpath/images)

	INSTALLS += $${qmlfiles} $${qmlimages}
}
