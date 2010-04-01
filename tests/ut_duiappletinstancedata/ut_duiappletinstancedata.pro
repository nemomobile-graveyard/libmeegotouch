include(../common_top.pri)
TARGET = ut_duiappletinstancedata
INCLUDEPATH += \
    $$DUISRCDIR/extensions/mashup/appletcommunication \
    $$DUISRCDIR/extensions/mashup/appletinterface \
    $$DUISRCDIR/extensions/mashup/mashup \
    $$DUISRCDIR/corelib/widgets

# unit test and unit classes
SOURCES += \
    ut_duiappletinstancedata.cpp \
    $$DUISRCDIR/extensions/mashup/mashup/duiappletinstancedata.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
    ut_duiappletinstancedata.h \
    $$DUISRCDIR/extensions/mashup/mashup/duiappletinstancedata.h \
    $$DUISRCDIR/extensions/mashup/mashup/duiappletid.h

include(../common_bot.pri)
