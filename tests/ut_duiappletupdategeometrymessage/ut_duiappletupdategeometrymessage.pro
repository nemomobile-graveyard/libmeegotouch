include(../common_top.pri)
TARGET = ut_duiappletupdategeometrymessage
INCLUDEPATH += $$DUISRCDIR/mashup/appletcommunication $$DUISRCDIR/mashup/appletinterface  $$DUISRCDIR/mashup/mashup

# unit test and unit classes
SOURCES += \
    ut_duiappletupdategeometrymessage.cpp \

# unit test and unit classes
HEADERS += \
    ut_duiappletupdategeometrymessage.h \

include(../common_bot.pri)
