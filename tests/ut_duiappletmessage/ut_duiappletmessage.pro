include(../common_top.pri)
TARGET = ut_duiappletmessage
INCLUDEPATH += $$DUISRCDIR/mashup/appletcommunication $$DUISRCDIR/mashup/appletinterface  $$DUISRCDIR/mashup/mashup

# unit test and unit classes
SOURCES += \
    ut_duiappletmessage.cpp \

# unit test and unit classes
HEADERS += \
    ut_duiappletmessage.h \

include(../common_bot.pri)
