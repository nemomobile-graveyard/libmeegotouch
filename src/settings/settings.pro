include( ../common_top.pri )

TARGET = $$qtLibraryTarget(meegotouchsettings)
QT += xml
DEFINES += M_BUILD_SETTINGS

INCLUDEPATH += \
    ../include \
    settingslanguage \
    ../.gen \

include(settingslanguage/settingslanguage.pri)

SOURCES += msettingslibrary.cpp

LIBS += $$mAddLibrary(meegotouchcore)

include(../common_bot.pri)
