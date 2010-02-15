include(../common_top.pri)
include(../../mkspecs/common.pri)

TARGET = ut_duiappletsettingsdialog
INCLUDEPATH += $$DUISRCDIR/mashup/mashup \
    $$DUISRCDIR/mashup/appletcommunication \
    $$DUISRCDIR/widgets

QT += xml

SOURCES += ut_duiappletsettingsdialog.cpp  \
    $$DUISRCDIR/mashup/mashup/duiappletsettingsdialog.cpp \
    ../stubs/stubbase.cpp

HEADERS += ut_duiappletsettingsdialog.h \
    $$DUISRCDIR/mashup/mashup/duiappletsettingsdialog.h \
    $$DUISRCDIR/mashup/mashup/duiappletid.h

include(../common_bot.pri)
