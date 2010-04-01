include( ../common_top.pri )

TARGET = duisettings

INCLUDEPATH += \
    ../include \
    settingslanguage \
    ../.gen \

include(settingslanguage/settingslanguage.pri)

SOURCES += duisettingslibrary.cpp

include(../common_bot.pri)
