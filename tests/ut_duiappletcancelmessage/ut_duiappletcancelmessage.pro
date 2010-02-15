include(../common_top.pri)
TARGET = ut_duiappletcancelmessage
INCLUDEPATH += $$DUISRCDIR/mashup/appletcommunication $$DUISRCDIR/mashup/appletinterface  $$DUISRCDIR/mashup/mashup

# unit test and unit classes
SOURCES += \
    ut_duiappletcancelmessage.cpp \

# unit test and unit classes
HEADERS += \
    ut_duiappletcancelmessage.h \

include(../common_bot.pri)
