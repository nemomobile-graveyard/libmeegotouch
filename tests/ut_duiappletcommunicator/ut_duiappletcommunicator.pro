include(../common_top.pri)
TARGET = ut_duiappletcommunicator
INCLUDEPATH += $$DUISRCDIR/mashup/appletcommunication

SOURCES += \
    ut_duiappletcommunicator.cpp \
    $$DUISRCDIR/mashup/appletcommunication/duiappletcommunicator.cpp

HEADERS += \
    ut_duiappletcommunicator.h \
    $$DUISRCDIR/mashup/appletcommunication/duiappletcommunicator.h

include(../common_bot.pri)
