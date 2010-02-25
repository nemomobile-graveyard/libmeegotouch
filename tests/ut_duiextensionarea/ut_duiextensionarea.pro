include(../common_top.pri)
TARGET = ut_duiextensionarea
INCLUDEPATH += \
    $$DUISRCDIR/applicationextension \
    $$DUISRCDIR/mashup/mashup \
    $$DUISRCDIR/widgets \
    $$DUISRCDIR/style

# unit test and unit classes
SOURCES += \
    ut_duiextensionarea.cpp \
    $$DUISRCDIR/applicationextension/duiextensionarea.cpp \
    $$DUISRCDIR/widgets/core/duiwidgetcontroller.cpp \
    $$DUISRCDIR/widgets/core/duiwidget.cpp 

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
   ut_duiextensionarea.h \
   $$DUISRCDIR/applicationextension/duiextensionarea.h \
   $$DUISRCDIR/mashup/mashup/duiappletinstancemanager.h \
   $$DUISRCDIR/mashup/mashup/duiappletid.h \
   $$DUISRCDIR/widgets/core/duiwidgetcontroller_p.h \
   $$DUISRCDIR/widgets/core/duiwidget_p.h \
   $$DUISRCDIR/widgets/duiscenewindow_p.h \
   $$DUISRCDIR/widgets/duiobjectmenu.h

include(../common_bot.pri)
