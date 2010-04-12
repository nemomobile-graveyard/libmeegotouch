include( ../common_top.pri )

TARGET = meegotouchsettings
QT += xml

INCLUDEPATH += \
    ../include \
    settingslanguage \
    ../.gen \

include(settingslanguage/settingslanguage.pri)

SOURCES += msettingslibrary.cpp

include(../common_bot.pri)
