include(../common_top.pri)
TARGET = ut_duiappletsetgeometrymessage
INCLUDEPATH += $$DUISRCDIR/mashup/appletcommunication $$DUISRCDIR/mashup/appletinterface  $$DUISRCDIR/mashup/mashup

# unit test and unit classes
SOURCES += \
    ut_duiappletsetgeometrymessage.cpp

# unit test and unit classes
HEADERS += \
    ut_duiappletsetgeometrymessage.h

include(../common_bot.pri)
