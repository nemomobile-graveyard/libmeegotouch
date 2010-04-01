include(../common_top.pri)
include(../common_duiextensions.pri)

INCLUDEPATH += \
    $$DUISRCDIR/include \
    $$DUISRCDIR/corelib/widgets \
    $$DUISRCDIR/events

win32 {
    QMAKE_MOC = perl $${IN_PWD}\..\..\duimoc\duimoc
} else {
    PRE_TARGETDEPS += ../../duigen/duigen
    QMAKE_MOC = PATH=../../duigen:$$(PATH) $${IN_PWD}/../../duimoc/duimoc
}

DUIGEN_OUTDIR = .
duigenerator_model.name = duigenerator model
duigenerator_model.input = MODEL_HEADERS
duigenerator_model.depends = ../../duigen/duigen
duigenerator_model.output = $$DUIGEN_OUTDIR/gen_${QMAKE_FILE_BASE}data.cpp
duigenerator_model.commands += ../../duigen/duigen --model ${QMAKE_FILE_NAME} $$DUIGEN_OUTDIR
duigenerator_model.clean += $$DUIGEN_OUTDIR/gen_*
duigenerator_model.CONFIG = target_predeps no_link
duigenerator_model.variable_out = GENERATED_SOURCES
QMAKE_EXTRA_COMPILERS += duigenerator_model

MODEL_HEADERS += \
    $$DUISRCDIR/corelib/widgets/duiwidgetmodel.h \
    $$DUISRCDIR/extensions/mashup/mashup/duiappletinventorymodel.h

SOURCES += \
    ut_duiappletinventory.cpp \
    $$DUISRCDIR/extensions/mashup/mashup/duiappletinventory.cpp \
    $$DUISRCDIR/extensions/mashup/appletinstallation/duiappletinstantiator.cpp \
    $$DUISRCDIR/corelib/widgets/core/duiwidgetcontroller.cpp \
    $$DUISRCDIR/corelib/widgets/core/duiwidget.cpp \
    $$DUISRCDIR/corelib/widgets/duiwidgetmodel.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
    ut_duiappletinventory.h \
    $$DUISRCDIR/extensions/mashup/mashup/duiappletinventory.h \
    $$DUISRCDIR/extensions/mashup/mashup/duiappletinventorymodel.h \
    $$DUISRCDIR/extensions/mashup/appletinstallation/duiappletinstantiator.h \
    $$DUISRCDIR/extensions/mashup/appletinstallation/duiappletinstantiator_p.h \
    $$DUISRCDIR/extensions/mashup/mashup/duiappletbutton.h \
    $$DUISRCDIR/extensions/mashup/mashup/duiappletinstancemanager.h \
    $$DUISRCDIR/extensions/mashup/mashup/duiappletid.h \
    $$DUISRCDIR/corelib/widgets/core/duiwidgetcontroller_p.h \
    $$DUISRCDIR/corelib/widgets/core/duiwidget_p.h \
    $$DUISRCDIR/corelib/widgets/duiwidgetmodel_p.h \
    $$DUISRCDIR/corelib/widgets/duiscenewindow_p.h \
    $$DUISRCDIR/corelib/widgets/duiobjectmenu.h

include(../common_bot.pri)
