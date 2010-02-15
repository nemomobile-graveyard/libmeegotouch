include(../common_top.pri)
TARGET = ut_duisubdatastore
INCLUDEPATH += $$DUISRCDIR/mashup/appletcommunication $$DUISRCDIR/mashup/appletinterface  $$DUISRCDIR/mashup/mashup

SOURCES += \
    ut_duisubdatastore.cpp \

HEADERS += \
    ut_duisubdatastore.h \

include(../common_bot.pri)
