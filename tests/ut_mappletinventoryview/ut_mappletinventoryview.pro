include(../common_top.pri)
TARGET = ut_mappletinventoryview
INCLUDEPATH += $$MSRCDIR/extensions/mashup/mashup
INCLUDEPATH += $$MSRCDIR/extensions/style
INCLUDEPATH += $$MSRCDIR/extensions/mashup/appletinstallation
INCLUDEPATH += $$MSRCDIR/corelib/style
INCLUDEPATH += $$MSRCDIR/corelib/widgets
INCLUDEPATH += $$MSRCDIR/corelib/widgets/core
INCLUDEPATH += $$MSRCDIR/corelib/core
INCLUDEPATH += $$MSRCDIR/corelib/scene

STYLE_HEADERS += $$MSRCDIR/extensions/style/mappletinventorystyle.h
MODEL_HEADERS += $$MSRCDIR/corelib/widgets/mwidgetmodel.h \
    $$MSRCDIR/extensions/mashup/mashup/mappletinventorymodel.h

# unit test and unit
SOURCES += \
    ut_mappletinventoryview.cpp \
    $$MSRCDIR/extensions/mashup/mashup/mappletinventoryview.cpp \
    $$TEST_SOURCES \ 

# base classes
SOURCES += \
    $$MSRCDIR/corelib/widgets/core/mwidgetcontroller.cpp \
    $$MSRCDIR/corelib/widgets/core/mwidget.cpp \
    $$MSRCDIR/corelib/widgets/mwidgetmodel.cpp \
    $$MSRCDIR/corelib/scene/mscene.cpp \

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_mappletinventoryview.h \
    $$MSRCDIR/corelib/widgets/core/mwidgetcontroller_p.h \
    $$MSRCDIR/views/mextendingbackgroundview_p.h \
    $$MSRCDIR/extensions/applicationextension/mextensionarea.h \
    $$MSRCDIR/extensions/applicationextension/mapplicationextensionarea.h \
    $$MSRCDIR/extensions/mashup/mashup/mappletbutton.h \
    $$MSRCDIR/extensions/mashup/mashup/mappletinventory.h \
    $$MSRCDIR/extensions/mashup/mashup/mappletinventoryview.h \
    $$MSRCDIR/extensions/mashup/mashup/mappletinventorymodel.h \
    $$MSRCDIR/extensions/style/mappletinventorystyle.h \
    $$MSRCDIR/corelib/widgets/mwidgetmodel_p.h \
    $$MSRCDIR/corelib/widgets/mobjectmenu.h \
    $$MSRCDIR/corelib/scene/mscene.h \
    $$MODEL_HEADERS \
    $$STYLE_HEADERS

include(../common_bot.pri)
