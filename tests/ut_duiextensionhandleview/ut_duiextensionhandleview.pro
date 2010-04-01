include(../common_top.pri)
include(../common_duiextensions.pri)

INCLUDEPATH += \
    $$DUISRCDIR/corelib/widgets \
    $$DUISRCDIR/corelib/widgets/core \
    $$DUISRCDIR/corelib/core \
    $$DUISRCDIR/corelib/style

DUIGEN_OUTDIR = .
duigenerator_model.name = duigenerator model
duigenerator_model.input = MODEL_HEADERS
duigenerator_model.depends = ../../duigen/duigen
duigenerator_model.output = $$DUIGEN_OUTDIR/gen_${QMAKE_FILE_BASE}data.cpp
duigenerator_model.commands += ../../duigen/duigen --model ${QMAKE_FILE_NAME} $$DUIGEN_OUTDIR
duigenerator_model.clean += $$DUIGEN_OUTDIR/gen_*
duigenerator_model.CONFIG = target_predeps no_link
duigenerator_model.variable_out = GENERATED_SOURCES
duigenerator_style.name = duigenerator style
duigenerator_style.input = STYLE_HEADERS
duigenerator_style.depends = ../../duigen/duigen
duigenerator_style.output = $$DUIGEN_OUTDIR/gen_${QMAKE_FILE_BASE}data.cpp
duigenerator_style.commands += ../../duigen/duigen --style ${QMAKE_FILE_NAME} $$DUIGEN_OUTDIR
duigenerator_style.clean += $$DUIGEN_OUTDIR/gen_*
duigenerator_style.CONFIG = target_predeps no_link
duigenerator_style.variable_out = GENERATED_SOURCES
QMAKE_EXTRA_COMPILERS += duigenerator_model duigenerator_style

MODEL_HEADERS += $$DUISRCDIR/corelib/widgets/duiwidgetmodel.h \
    $$DUISRCDIR/extensions/applicationextension/duiextensionhandlemodel.h

STYLE_HEADERS += $$DUISRCDIR/extensions/style/duiextensionhandlestyle.h

SOURCES += ut_duiextensionhandleview.cpp \
    $$DUISRCDIR/extensions/applicationextension/duiextensionhandleview.cpp \
    $$DUISRCDIR/corelib/widgets/duiwidgetmodel.cpp \
    dummy.cpp \
    ../stubs/stubbase.cpp

HEADERS += ut_duiextensionhandleview.h \
    $$DUISRCDIR/extensions/applicationextension/duiextensionhandleview.h \
    $$DUISRCDIR/extensions/applicationextension/duiextensionhandle.h \
    $$DUISRCDIR/extensions/applicationextension/duiextensionhandlemodel.h \
    $$DUISRCDIR/corelib/widgets/duiwidgetmodel_p.h \
    $$DUISRCDIR/corelib/widgets/core/duiwidgetview_p.h \
    $$DUISRCDIR/extensions/style/duiextensionhandlestyle.h

include(../common_bot.pri)
