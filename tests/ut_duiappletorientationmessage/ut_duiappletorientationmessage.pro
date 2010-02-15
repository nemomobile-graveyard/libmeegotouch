include(../common_top.pri)
TARGET = ut_duiappletorientationmessage
INCLUDEPATH += $$DUISRCDIR/mashup/appletcommunication $$DUISRCDIR/mashup/appletinterface  $$DUISRCDIR/mashup/mashup

# unit test and unit classes
SOURCES += \
    ut_duiappletorientationmessage.cpp \

# unit test and unit classes
HEADERS += \
    ut_duiappletorientationmessage.h \

include(../common_bot.pri)
