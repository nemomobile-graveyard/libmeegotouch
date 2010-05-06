include(../common_top.pri)
include(../common_mextensions.pri)

QT += xml

SOURCES += \
    ut_mappletsettings.cpp \
    $$MSRCDIR/extensions/mashup/mashup/mappletsettings.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

HEADERS += \
    ut_mappletsettings.h \
    $$MSRCDIR/extensions/mashup/mashup/mappletsettings.h \
    $$MSRCDIR/extensions/mashup/mashup/mappletid.h

include(../common_bot.pri)
