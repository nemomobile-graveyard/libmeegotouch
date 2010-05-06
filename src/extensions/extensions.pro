include( ../common_top.pri )

TARGET = meegotouchextensions
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

LIBS += -lmeegotouchviews -lmeegotouchsettings -lmeegotouchcore

SOURCES += mextensionslibrary.cpp

include(../common_bot.pri)
