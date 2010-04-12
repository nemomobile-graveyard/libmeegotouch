include(../common_top.pri)
include(../common_mextensions.pri)

SOURCES += \
    ut_mappletbutton.cpp \
    $$MSRCDIR/extensions/mashup/mashup/mappletbutton.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
    ut_mappletbutton.h \
    $$MSRCDIR/extensions/mashup/mashup/mappletbutton.h

include(../common_bot.pri)
