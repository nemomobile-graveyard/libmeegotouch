include(../common_top.pri)
TARGET = ut_duiappletpixmaptakenintousemessage
INCLUDEPATH += $$DUISRCDIR/mashup/appletcommunication $$DUISRCDIR/mashup/appletinterface  $$DUISRCDIR/mashup/mashup

# unit test and unit classes
SOURCES += \
   ut_duiappletpixmaptakenintousemessage.cpp

# unit test and unit classes
HEADERS += \
   ut_duiappletpixmaptakenintousemessage.h

include(../common_bot.pri)
