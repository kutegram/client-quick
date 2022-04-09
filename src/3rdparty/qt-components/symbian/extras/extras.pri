EXTRAS_PLUGIN.source_path  = src/3rdparty/qt-components/symbian/extras
EXTRAS_PLUGIN.imports_path = com/nokia/extras
EXTRAS_PLUGIN.target       = $$qtLibraryTarget(embedded_extras_plugin)

EXTRAS_PLUGIN.qml_files += \
    qmldir \
    Constants.js \
    DatePickerDialog.qml \
    InfoBanner.qml \
    RatingIndicator.qml \
    TimePickerDialog.qml \
    Tumbler.js \
    Tumbler.qml \
    TumblerColumn.qml \
    TumblerDialog.qml \
    TumblerIndexHelper.js \
    TumblerTemplate.qml

APP_PLUGINS += EXTRAS_PLUGIN
