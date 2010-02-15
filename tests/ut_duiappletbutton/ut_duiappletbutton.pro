include(../common_top.pri)
TARGET = ut_duiappletbutton
INCLUDEPATH += $$DUISRCDIR/mashup/mashup $$DUISRCDIR/mashup/appletinterface

SOURCES += \
    ut_duiappletbutton.cpp \
    $$DUISRCDIR/mashup/mashup/duiappletbutton.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
    ut_duiappletbutton.h \
    $$DUISRCDIR/mashup/mashup/duiappletbutton.h

include(../common_bot.pri)
