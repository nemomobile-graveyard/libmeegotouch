include(../common_top.pri)
TARGET = ut_duiappletobjectmenuactionselectedmessage
INCLUDEPATH += $$DUISRCDIR/mashup/appletcommunication $$DUISRCDIR/mashup/appletinterface  $$DUISRCDIR/mashup/mashup

# unit test and unit classes
SOURCES += \
    ut_duiappletobjectmenuactionselectedmessage.cpp

# unit test and unit classes
HEADERS += \
    ut_duiappletobjectmenuactionselectedmessage.h

include(../common_bot.pri)
