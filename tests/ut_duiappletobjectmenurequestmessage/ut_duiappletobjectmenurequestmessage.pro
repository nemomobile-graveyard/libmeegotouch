include(../common_top.pri)
TARGET = ut_duiappletobjectmenurequestmessage
INCLUDEPATH += $$DUISRCDIR/mashup/appletcommunication $$DUISRCDIR/mashup/appletinterface  $$DUISRCDIR/mashup/mashup

# unit test and unit classes
SOURCES += \
    ut_duiappletobjectmenurequestmessage.cpp

# unit test and unit classes
HEADERS += \
    ut_duiappletobjectmenurequestmessage.h

include(../common_bot.pri)
