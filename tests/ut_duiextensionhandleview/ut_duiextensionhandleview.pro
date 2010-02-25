include(../common_top.pri)

TARGET = ut_duiextensionhandleview
INCLUDEPATH += $$DUISRCDIR/applicationextension \
    $$DUISRCDIR/mashup/appletcommunication \
    $$DUISRCDIR/mashup/mashup \
    $$DUISRCDIR/widgets \
    $$DUISRCDIR/widgets/core \
    $$DUISRCDIR/core \
    $$DUISRCDIR/style

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

MODEL_HEADERS += $$DUISRCDIR/widgets/duiwidgetmodel.h \
    $$DUISRCDIR/applicationextension/duiextensionhandlemodel.h

STYLE_HEADERS += $$DUISRCDIR/style/duiextensionhandlestyle.h

SOURCES += ut_duiextensionhandleview.cpp \
           $$DUISRCDIR/applicationextension/duiextensionhandleview.cpp \
           $$DUISRCDIR/widgets/duiwidgetmodel.cpp \
           ../stubs/stubbase.cpp

HEADERS += ut_duiextensionhandleview.h \
           $$DUISRCDIR/applicationextension/duiextensionhandleview.h \
           $$DUISRCDIR/applicationextension/duiextensionhandle.h \
           $$DUISRCDIR/applicationextension/duiextensionhandlemodel.h \
           $$DUISRCDIR/widgets/duiwidgetmodel_p.h \
           $$DUISRCDIR/widgets/core/duiwidgetview_p.h \
           $$DUISRCDIR/style/duiextensionhandlestyle.h

include(../common_bot.pri)
