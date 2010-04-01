include(common_duisettings.pri)

INCLUDEPATH += \
    $$DUISRCDIR/extensions/applicationextension \
    $$DUISRCDIR/extensions/mashup/appletcommunication \
    $$DUISRCDIR/extensions/mashup/appletinstallation \
    $$DUISRCDIR/extensions/mashup/appletinterface \
    $$DUISRCDIR/extensions/mashup/mashup \
    $$DUISRCDIR/extensions/style \

LIBS += -lduiextensions
