include(../common_top.pri)
TARGET = ut_mappletinventoryview
INCLUDEPATH += $$MSRCDIR/extensions/mashup/mashup
INCLUDEPATH += $$MSRCDIR/extensions/style
INCLUDEPATH += $$MSRCDIR/extensions/mashup/appletinstallation
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
mgenerator_style.name = mgenerator style
mgenerator_style.input = STYLE_HEADERS
mgenerator_style.depends = ../../mgen/mgen
mgenerator_style.output = $$MGEN_OUTDIR/gen_${QMAKE_FILE_BASE}data.cpp
mgenerator_style.commands += ../../mgen/mgen --style ${QMAKE_FILE_NAME} $$MGEN_OUTDIR
mgenerator_style.clean += $$MGEN_OUTDIR/gen_*
mgenerator_style.CONFIG = target_predeps no_link
mgenerator_style.variable_out = GENERATED_SOURCES

QMAKE_EXTRA_COMPILERS += mgenerator_model mgenerator_style

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
    $$MSRCDIR/corelib/widgets/mwidgetmodel.cpp

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
    $$MSRCDIR/corelib/widgets/mobjectmenu.h

include(../common_bot.pri)
