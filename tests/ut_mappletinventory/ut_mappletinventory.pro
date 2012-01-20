include(../common_top.pri)
include(../common_mextensions.pri)

INCLUDEPATH += \
    $$MSRCDIR/include \
    $$MSRCDIR/corelib/widgets \

MODEL_HEADERS += \
    $$MSRCDIR/corelib/widgets/mwidgetmodel.h \
    $$MSRCDIR/extensions/mashup/mashup/mappletinventorymodel.h

SOURCES += \
    ut_mappletinventory.cpp \
    $$MSRCDIR/extensions/mashup/mashup/mappletinventory.cpp \
    $$MSRCDIR/extensions/mashup/appletinstallation/mappletinstantiator.cpp \
    $$MSRCDIR/corelib/widgets/mwidgetmodel.cpp \

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
    ut_mappletinventory.h \
    $$MSRCDIR/extensions/mashup/mashup/mappletinventory.h \
    $$MSRCDIR/extensions/mashup/appletinstallation/mappletinstantiator.h \
    $$MSRCDIR/extensions/mashup/appletinstallation/mappletinstantiator_p.h \
    $$MSRCDIR/extensions/mashup/mashup/mappletbutton.h \
    $$MSRCDIR/extensions/mashup/mashup/mappletinstancemanager.h \
    $$MSRCDIR/corelib/widgets/mwidgetmodel_p.h \
    $$MODEL_HEADERS

include(../common_bot.pri)
