include(../common_top.pri)
TARGET = ut_duiappletinstancedata
INCLUDEPATH += $$DUISRCDIR/mashup/appletcommunication $$DUISRCDIR/mashup/appletinterface  $$DUISRCDIR/mashup/mashup $$DUISRCDIR/widgets

# unit test and unit classes
SOURCES += \
    ut_duiappletinstancedata.cpp \
    $$DUISRCDIR/mashup/mashup/duiappletinstancedata.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
    ut_duiappletinstancedata.h \
    $$DUISRCDIR/mashup/mashup/duiappletinstancedata.h \
    $$DUISRCDIR/mashup/mashup/duiappletid.h

include(../common_bot.pri)
