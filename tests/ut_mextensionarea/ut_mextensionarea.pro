include(../common_top.pri)
include(../common_mextensions.pri)

INCLUDEPATH += \
    $$MSRCDIR/corelib/widgets \
    $$MSRCDIR/corelib/style

# unit test and unit classes
SOURCES += \
    ut_mextensionarea.cpp \
    $$MSRCDIR/extensions/applicationextension/mextensionarea.cpp \
    $$MSRCDIR/corelib/widgets/core/mwidgetcontroller.cpp \
    $$MSRCDIR/corelib/widgets/core/mwidget.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
   ut_mextensionarea.h \
   $$MSRCDIR/extensions/applicationextension/mextensionarea.h \
   $$MSRCDIR/extensions/mashup/mashup/mappletinstancemanager.h \
   $$MSRCDIR/extensions/mashup/mashup/mappletid.h \
   $$MSRCDIR/corelib/widgets/core/mwidgetcontroller_p.h \
   $$MSRCDIR/corelib/widgets/core/mwidget_p.h \
   $$MSRCDIR/corelib/widgets/mscenewindow_p.h \
   $$MSRCDIR/corelib/widgets/mobjectmenu.h

include(../common_bot.pri)
