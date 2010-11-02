include( ../common_top.pri )

TARGET = meegotouchextensions

DEFINES += M_BUILD_EXTENSIONS

QT += xml

INCLUDEPATH += \
    ../include \
    ../include/private \
    applicationextension \
    mashup/mashup \
    style \
    ../corelib/.gen \

contains(DEFINES, HAVE_DBUS) {
    include(applicationextension/applicationextension.pri)
    include(mashup/appletcommunication/appletcommunication.pri)
    include(mashup/appletinterface/appletinterface.pri)
    include(mashup/appletinstallation/appletinstallation.pri)
    include(mashup/mashup/mashup.pri)
    include(style/style.pri)
}


LIBS += $$mAddLibrary(meegotouchcore) $$mAddLibrary(meegotouchsettings) $$mAddLibrary(meegotouchviews)

SOURCES += mextensionwatcher.cpp \
           mextensionslibrary.cpp

HEADERS += mextensionwatcher.h

include(../common_bot.pri)
