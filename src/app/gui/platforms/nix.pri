# its only for old MeeGo SDK
exists($$QMAKE_INCDIR_QT"/../qmsystem2/qmkeys.h"):!contains(MEEGO_EDITION,harmattan): {
    MEEGO_VERSION_MAJOR     = 1
    MEEGO_VERSION_MINOR     = 2
    MEEGO_VERSION_PATCH     = 0
    MEEGO_EDITION           = harmattan
    DEFINES += MEEGO_EDITION_HARMATTAN
}

TARGET = kutegram
isEmpty(PREFIX):PREFIX = /usr
BINDIR = $$PREFIX/bin
DATADIR = /tmp/$$APPNAME

maemo5 {
    QT += opengl
    DEFINES += BUILD_PLATFORM=\"\\\"Maemo\\\"\"
#    DEFINES += Q_WS_MAEMO_5

    BINDIR = /opt/maemo/usr/bin
    DATADIR = /home/user/tmp/$$APPNAME
}

contains(MEEGO_EDITION,harmattan) {
    QT += opengl
    CONFIG += qt-boostable qdeclarative-boostable
    DEFINES += BUILD_PLATFORM=\"\\\"MeeGo\\\"\"
#    DEFINES += Q_WS_MAEMO_5

    BINDIR = /opt/usr/bin
    DATADIR = /home/user/tmp/$$APPNAME
    DEFINES += MEEGO_EDITION_HARMATTAN
}

INSTALLS += target desktop icons

# maemo: /opt/maemo/usr/bin (+link: /usr/bin)
# meego: /opt/usr/bin
#  *nix: /usr/bin
target.path =$$BINDIR

# maemo: /usr/share/applications/hildon/kutegram.desktop
# meego: /usr/share/applications/kutegram_harmattan.desktop
#  *nix: /usr/share/applications/kutegram.desktop
desktop.path = $$PREFIX/share/applications
maemo5: desktop.path = $$PREFIX/share/applications/hildon
desktop.files = kutegram.desktop
contains(MEEGO_EDITION,harmattan){
    desktop.files = kutegram_harmattan.desktop
}

# /usr/share/icons/...
icons.path = $$PREFIX/share/icons
icons.files += icons/*

contains(MEEGO_EDITION,harmattan) {
    # /usr/share/kutegram/splash.png
    splash.path = $$PREFIX/share/$$APPNAME
    sphash.files = splash.png

    INSTALLS += splash
}

OTHER_FILES += qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    qtc_packaging/debian_fremantle/kutegram.links \
    qtc_packaging/debian_fremantle/rules \
    qtc_packaging/debian_fremantle/README \
    qtc_packaging/debian_fremantle/copyright \
    qtc_packaging/debian_fremantle/control \
    qtc_packaging/debian_fremantle/compat \
    qtc_packaging/debian_fremantle/changelog \
    kutegram_harmattan.desktop \
    kutegram.desktop \
    splash.png \
    view/skin/icons/hicolor/64x64/apps/kutegram.png \
    view/skin/icons/hicolor/80x80/apps/kutegram.png \
    view/skin/icons/hicolor/scalable/apps/kutegram.svg
