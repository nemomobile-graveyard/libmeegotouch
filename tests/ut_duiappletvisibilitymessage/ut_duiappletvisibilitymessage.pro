include(../common_top.pri)
TARGET = ut_duiappletvisibilitymessage
INCLUDEPATH += $$DUISRCDIR/mashup/appletcommunication $$DUISRCDIR/mashup/appletinterface  $$DUISRCDIR/mashup/mashup

# unit test and unit classes
SOURCES += \
    ut_duiappletvisibilitymessage.cpp

# unit test and unit classes
HEADERS += \
    ut_duiappletvisibilitymessage.h

include(../common_bot.pri)
