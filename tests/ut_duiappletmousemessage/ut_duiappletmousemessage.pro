include(../common_top.pri)
TARGET = ut_duiappletmousemessage
INCLUDEPATH += $$DUISRCDIR/mashup/appletcommunication $$DUISRCDIR/mashup/appletinterface  $$DUISRCDIR/mashup/mashup

# unit test and unit classes
SOURCES += \
    ut_duiappletmousemessage.cpp \

# unit test and unit classes
HEADERS += \
    ut_duiappletmousemessage.h \

include(../common_bot.pri)
