include(../common_top.pri)
include(../common_mextensions.pri)

INCLUDEPATH += $$MSRCDIR/corelib/widgets

QT += xml

SOURCES += ut_mappletsettingsdialog.cpp  \
    $$MSRCDIR/extensions/mashup/mashup/mappletsettingsdialog.cpp \
    ../stubs/stubbase.cpp

HEADERS += ut_mappletsettingsdialog.h \
    $$MSRCDIR/extensions/mashup/mashup/mappletsettingsdialog.h \
    $$MSRCDIR/extensions/mashup/mashup/mappletid.h

include(../common_bot.pri)
