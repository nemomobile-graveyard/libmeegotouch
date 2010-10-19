include(../common_top.pri)
include(../common_mextensions.pri)

INCLUDEPATH += \
    $$MSRCDIR/include \
    $$MSRCDIR/corelib/widgets \
    $$MSRCDIR/corelib/scene \
    $$MSRCDIR/events

MODEL_HEADERS += \
    $$MSRCDIR/corelib/widgets/mwidgetmodel.h \
    $$MSRCDIR/extensions/mashup/mashup/mappletinventorymodel.h

SOURCES += \
    ut_mappletinventory.cpp \
    $$MSRCDIR/extensions/mashup/mashup/mappletinventory.cpp \
    $$MSRCDIR/extensions/mashup/appletinstallation/mappletinstantiator.cpp \
    $$MSRCDIR/corelib/widgets/core/mwidgetcontroller.cpp \
    $$MSRCDIR/corelib/widgets/core/mwidget.cpp \
    $$MSRCDIR/corelib/widgets/mwidgetmodel.cpp \
    $$MSRCDIR/corelib/scene/mscene.cpp \

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
    ut_mappletinventory.h \
    $$MSRCDIR/extensions/mashup/mashup/mappletinventory.h \
    $$MSRCDIR/extensions/mashup/mashup/mappletinventorymodel.h \
    $$MSRCDIR/extensions/mashup/appletinstallation/mappletinstantiator.h \
    $$MSRCDIR/extensions/mashup/appletinstallation/mappletinstantiator_p.h \
    $$MSRCDIR/extensions/mashup/mashup/mappletbutton.h \
    $$MSRCDIR/extensions/mashup/mashup/mappletinstancemanager.h \
    $$MSRCDIR/extensions/mashup/mashup/mappletid.h \
    $$MSRCDIR/corelib/widgets/core/mwidgetcontroller_p.h \
    $$MSRCDIR/corelib/widgets/core/mwidget_p.h \
    $$MSRCDIR/corelib/widgets/mwidgetmodel_p.h \
    $$MSRCDIR/corelib/widgets/mobjectmenu.h \
    $$MSRCDIR/corelib/scene/mscene.h \
    $$MODEL_HEADERS

include(../common_bot.pri)
