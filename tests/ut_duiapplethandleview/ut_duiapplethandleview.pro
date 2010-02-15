include(../common_top.pri)

TARGET = ut_duiapplethandleview
INCLUDEPATH += $$DUISRCDIR/mashup/mashup \
    $$DUISRCDIR/mashup/appletcommunication \
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
QMAKE_EXTRA_COMPILERS += duigenerator_model

MODEL_HEADERS += $$DUISRCDIR/widgets/duiwidgetmodel.h \
    $$DUISRCDIR/mashup/mashup/duiapplethandlemodel.h

SOURCES += ut_duiapplethandleview.cpp \
           $$DUISRCDIR/mashup/mashup/duiapplethandleview.cpp \
           $$DUISRCDIR/widgets/duiwidgetmodel.cpp \
           ../stubs/stubbase.cpp

HEADERS += ut_duiapplethandleview.h \
           $$DUISRCDIR/mashup/mashup/duiapplethandleview.h \
           $$DUISRCDIR/mashup/mashup/duiapplethandle.h \
           $$DUISRCDIR/mashup/mashup/duiapplethandlemodel.h \
           $$DUISRCDIR/widgets/duiwidgetmodel_p.h \
           $$DUISRCDIR/widgets/core/duiwidgetview_p.h

include(../common_bot.pri)
