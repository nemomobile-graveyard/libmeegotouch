include(../common_top.pri)
include(../common_duiextensions.pri)

QT += xml

SOURCES += \
    ut_duiappletsettings.cpp \
    $$DUISRCDIR/extensions/mashup/mashup/duiappletsettings.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

HEADERS += \
    ut_duiappletsettings.h \
    $$DUISRCDIR/extensions/mashup/mashup/duiappletsettings.h \
    $$DUISRCDIR/extensions/mashup/mashup/duiappletid.h

include(../common_bot.pri)
