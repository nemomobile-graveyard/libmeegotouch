include(../common_top.pri)
TARGET = ut_duiappletsettings
INCLUDEPATH += $$DUISRCDIR/mashup/mashup

QT += xml

SOURCES += \
    ut_duiappletsettings.cpp \
    $$DUISRCDIR/mashup/mashup/duiappletsettings.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

HEADERS += \
    ut_duiappletsettings.h \
    $$DUISRCDIR/mashup/mashup/duiappletsettings.h \
    $$DUISRCDIR/mashup/mashup/duiappletid.h

include(../common_bot.pri)
