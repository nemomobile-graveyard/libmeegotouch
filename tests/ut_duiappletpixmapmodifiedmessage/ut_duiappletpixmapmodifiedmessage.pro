include(../common_top.pri)
TARGET = ut_duiappletpixmapmodifiedmessage
INCLUDEPATH += $$DUISRCDIR/mashup/appletcommunication $$DUISRCDIR/mashup/appletinterface  $$DUISRCDIR/mashup/mashup

# unit test and unit classes
SOURCES += \
    ut_duiappletpixmapmodifiedmessage.cpp

# unit test and unit classes
HEADERS += \
    ut_duiappletpixmapmodifiedmessage.h

include(../common_bot.pri)
