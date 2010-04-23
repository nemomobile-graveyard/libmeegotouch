include( ../common_top.pri )

TARGET = meegotouchsettings
QT += xml

INCLUDEPATH += \
    ../include \
    settingslanguage \
    ../.gen \

include(settingslanguage/settingslanguage.pri)

SOURCES += msettingslibrary.cpp

LIBS += -lmeegotouchcore

include(../common_bot.pri)
