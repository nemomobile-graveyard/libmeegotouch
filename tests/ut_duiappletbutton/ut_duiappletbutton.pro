include(../common_top.pri)
include(../common_duiextensions.pri)

SOURCES += \
    ut_duiappletbutton.cpp \
    $$DUISRCDIR/extensions/mashup/mashup/duiappletbutton.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
    ut_duiappletbutton.h \
    $$DUISRCDIR/extensions/mashup/mashup/duiappletbutton.h

include(../common_bot.pri)
