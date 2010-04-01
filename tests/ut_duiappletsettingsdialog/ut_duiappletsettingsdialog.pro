include(../common_top.pri)
include(../common_duiextensions.pri)

INCLUDEPATH += $$DUISRCDIR/corelib/widgets

QT += xml

SOURCES += ut_duiappletsettingsdialog.cpp  \
    $$DUISRCDIR/extensions/mashup/mashup/duiappletsettingsdialog.cpp \
    ../stubs/stubbase.cpp

HEADERS += ut_duiappletsettingsdialog.h \
    $$DUISRCDIR/extensions/mashup/mashup/duiappletsettingsdialog.h \
    $$DUISRCDIR/extensions/mashup/mashup/duiappletid.h

include(../common_bot.pri)
