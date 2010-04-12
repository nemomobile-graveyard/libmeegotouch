include(../common_top.pri)
include(../common_mextensions.pri)

INCLUDEPATH += \
    $$MSRCDIR/include \
    $$MSRCDIR/corelib/widgets \
    $$MSRCDIR/events

win32 {
    QMAKE_MOC = perl $${IN_PWD}\..\..\mmoc\mmoc
} else {
    PRE_TARGETDEPS += ../../mgen/mgen
    QMAKE_MOC = PATH=../../mgen:$$(PATH) $${IN_PWD}/../../mmoc/mmoc
}

MGEN_OUTDIR = .
mgenerator_model.name = mgenerator model
mgenerator_model.input = MODEL_HEADERS
mgenerator_model.depends = ../../mgen/mgen
mgenerator_model.output = $$MGEN_OUTDIR/gen_${QMAKE_FILE_BASE}data.cpp
mgenerator_model.commands += ../../mgen/mgen --model ${QMAKE_FILE_NAME} $$MGEN_OUTDIR
mgenerator_model.clean += $$MGEN_OUTDIR/gen_*
mgenerator_model.CONFIG = target_predeps no_link
mgenerator_model.variable_out = GENERATED_SOURCES
QMAKE_EXTRA_COMPILERS += mgenerator_model

MODEL_HEADERS += \
    $$MSRCDIR/corelib/widgets/mwidgetmodel.h \
    $$MSRCDIR/extensions/mashup/mashup/mappletinventorymodel.h

SOURCES += \
    ut_mappletinventory.cpp \
    $$MSRCDIR/extensions/mashup/mashup/mappletinventory.cpp \
    $$MSRCDIR/extensions/mashup/appletinstallation/mappletinstantiator.cpp \
    $$MSRCDIR/corelib/widgets/core/mwidgetcontroller.cpp \
    $$MSRCDIR/corelib/widgets/core/mwidget.cpp \
    $$MSRCDIR/corelib/widgets/mwidgetmodel.cpp

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
    $$MSRCDIR/corelib/widgets/mscenewindow_p.h \
    $$MSRCDIR/corelib/widgets/mobjectmenu.h

include(../common_bot.pri)
