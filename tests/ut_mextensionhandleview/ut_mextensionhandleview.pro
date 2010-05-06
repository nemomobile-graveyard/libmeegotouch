include(../common_top.pri)
include(../common_mextensions.pri)

INCLUDEPATH += \
    $$MSRCDIR/corelib/widgets \
    $$MSRCDIR/corelib/widgets/core \
    $$MSRCDIR/corelib/core \
    $$MSRCDIR/corelib/style

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

MODEL_HEADERS += $$MSRCDIR/corelib/widgets/mwidgetmodel.h \
    $$MSRCDIR/extensions/applicationextension/mextensionhandlemodel.h

STYLE_HEADERS += $$MSRCDIR/extensions/style/mextensionhandlestyle.h

SOURCES += ut_mextensionhandleview.cpp \
    $$MSRCDIR/extensions/applicationextension/mextensionhandleview.cpp \
    $$MSRCDIR/corelib/widgets/mwidgetmodel.cpp \
    dummy.cpp \
    ../stubs/stubbase.cpp

HEADERS += ut_mextensionhandleview.h \
    $$MSRCDIR/extensions/applicationextension/mextensionhandleview.h \
    $$MSRCDIR/extensions/applicationextension/mextensionhandle.h \
    $$MSRCDIR/extensions/applicationextension/mextensionhandlemodel.h \
    $$MSRCDIR/corelib/widgets/mwidgetmodel_p.h \
    $$MSRCDIR/corelib/widgets/core/mwidgetview_p.h \
    $$MSRCDIR/extensions/style/mextensionhandlestyle.h

include(../common_bot.pri)
