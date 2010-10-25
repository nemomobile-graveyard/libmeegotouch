include(common_msettings.pri)

INCLUDEPATH += \
    $$MSRCDIR/extensions/applicationextension \
    $$MSRCDIR/extensions/mashup/appletcommunication \
    $$MSRCDIR/extensions/mashup/appletinstallation \
    $$MSRCDIR/extensions/mashup/appletinterface \
    $$MSRCDIR/extensions/mashup/mashup \
    $$MSRCDIR/extensions/style \

LIBS += $$mAddLibrary(meegotouchextensions)
