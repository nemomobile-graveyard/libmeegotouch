include( ../common_top.pri )

TARGET = meegotouchsettings

DEFINES += M_BUILD_SETTINGS

QT += xml

INCLUDEPATH += \
    ../include \
    settingslanguage \
    ../.gen \

include(settingslanguage/settingslanguage.pri)

SOURCES += msettingslibrary.cpp

LIBS += $$mAddLibrary(meegotouchcore)

include(../common_bot.pri)
