include(../common_top.pri)
include(../common_duiextensions.pri)

INCLUDEPATH += \
    $$DUISRCDIR/corelib/widgets \
    $$DUISRCDIR/corelib/style

# unit test and unit classes
SOURCES += \
    ut_duimashupcanvas.cpp \
    $$DUISRCDIR/extensions/mashup/mashup/duimashupcanvas.cpp \
    $$DUISRCDIR/corelib/widgets/core/duiwidgetcontroller.cpp \
    $$DUISRCDIR/corelib/widgets/core/duiwidget.cpp \
    $$DUISRCDIR/extensions/applicationextension/duiextensionarea.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
   ut_duimashupcanvas.h \
   $$DUISRCDIR/extensions/mashup/mashup/duimashupcanvas.h \
   $$DUISRCDIR/extensions/mashup/mashup/duiappletinstancemanager.h \
   $$DUISRCDIR/extensions/mashup/mashup/duiappletid.h \
   $$DUISRCDIR/corelib/widgets/core/duiwidgetcontroller_p.h \
   $$DUISRCDIR/corelib/widgets/core/duiwidget_p.h \
   $$DUISRCDIR/corelib/widgets/duiscenewindow_p.h \
   $$DUISRCDIR/corelib/widgets/duiobjectmenu.h

include(../common_bot.pri)
