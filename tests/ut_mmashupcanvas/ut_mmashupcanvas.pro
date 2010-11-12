include(../common_top.pri)
include(../common_mextensions.pri)

INCLUDEPATH += \
    $$MSRCDIR/corelib/widgets \
    $$MSRCDIR/corelib/style

# unit test and unit classes
SOURCES += \
    ut_mmashupcanvas.cpp \
    $$MSRCDIR/extensions/mashup/mashup/mmashupcanvas.cpp \
    $$MSRCDIR/corelib/widgets/core/mwidgetcontroller.cpp \
    $$MSRCDIR/corelib/widgets/core/mwidget.cpp \
    $$MSRCDIR/extensions/applicationextension/mextensionarea.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
   ut_mmashupcanvas.h \
   $$MSRCDIR/extensions/mashup/mashup/mmashupcanvas.h \
   $$MSRCDIR/extensions/mashup/mashup/mappletinstancemanager.h \
   $$MSRCDIR/extensions/mashup/mashup/mappletid.h \
   $$MSRCDIR/corelib/widgets/core/mwidgetcontroller_p.h \
   $$MSRCDIR/corelib/widgets/core/mwidget_p.h \
   $$MSRCDIR/corelib/widgets/mobjectmenu.h

include(../common_bot.pri)
