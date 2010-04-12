include(../common_top.pri)
TARGET = ut_mappletinventoryview
INCLUDEPATH += $$MSRCDIR/mashup/mashup
INCLUDEPATH += $$MSRCDIR/mashup/appletinstallation
INCLUDEPATH += $$MSRCDIR/corelib/style
INCLUDEPATH += $$MSRCDIR/corelib/widgets
INCLUDEPATH += $$MSRCDIR/corelib/widgets/core
INCLUDEPATH += $$MSRCDIR/corelib/core

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

STYLE_HEADERS += $$MSRCDIR/corelib/style/mappletinventorystyle.h
MODEL_HEADERS += $$MSRCDIR/corelib/widgets/mwidgetmodel.h \
    $$MSRCDIR/mashup/mashup/mappletinventorymodel.h

# unit test and unit
SOURCES += \
    ut_mappletinventoryview.cpp \
    $$MSRCDIR/mashup/mashup/mappletinventoryview.cpp \
    $$TEST_SOURCES \ 

# base classes
SOURCES += \
    $$MSRCDIR/corelib/widgets/core/mwidgetcontroller.cpp \
    $$MSRCDIR/corelib/widgets/core/mwidget.cpp \
    $$MSRCDIR/corelib/widgets/mwidgetmodel.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_mappletinventoryview.h \
    $$MSRCDIR/corelib/widgets/core/mwidgetcontroller_p.h \
    $$MSRCDIR/corelib/widgets/views/mextendingbackgroundview_p.h \
    $$MSRCDIR/mashup/mashup/mappletbutton.h \
    $$MSRCDIR/mashup/mashup/mappletinventory.h \
    $$MSRCDIR/mashup/mashup/mappletinventoryview.h \
    $$MSRCDIR/mashup/mashup/mappletinventorymodel.h \
    $$MSRCDIR/corelib/widgets/mwidgetmodel_p.h \
    $$MSRCDIR/corelib/widgets/mscenewindow_p.h \
    $$MSRCDIR/corelib/widgets/mobjectmenu.h

include(../common_bot.pri)
