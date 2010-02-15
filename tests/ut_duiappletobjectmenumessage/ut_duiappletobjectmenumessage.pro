include(../common_top.pri)
TARGET = ut_duiappletobjectmenumessage
INCLUDEPATH += $$DUISRCDIR/mashup/appletcommunication $$DUISRCDIR/mashup/appletinterface  $$DUISRCDIR/mashup/mashup

# unit test and unit classes
SOURCES += \
    ut_duiappletobjectmenumessage.cpp

# unit test and unit classes
HEADERS += \
    ut_duiappletobjectmenumessage.h

include(../common_bot.pri)
