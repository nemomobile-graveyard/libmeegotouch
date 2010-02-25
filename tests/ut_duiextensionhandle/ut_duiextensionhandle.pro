include(../common_top.pri)

TARGET = ut_duiextensionhandle
INCLUDEPATH += $$DUISRCDIR/applicationextension \
    $$DUISRCDIR/mashup/mashup \
    $$DUISRCDIR/mashup/appletcommunication \
    $$DUISRCDIR/widgets \
    $$DUISRCDIR/style
QT += xml

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

SOURCES += ut_duiextensionhandle.cpp \
    $$DUISRCDIR/applicationextension/duiextensionhandle.cpp \
    $$DUISRCDIR/widgets/core/duiwidgetcontroller.cpp \
    $$DUISRCDIR/widgets/duiwidgetmodel.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

HEADERS += ut_duiextensionhandle.h \
    $$DUISRCDIR/applicationextension/duiextensionhandle.h \
    $$DUISRCDIR/applicationextension/duiextensionhandle_p.h \
    $$DUISRCDIR/applicationextension/duiextensionhandlemodel.h \
    $$DUISRCDIR/widgets/core/duiwidgetcontroller_p.h \
    $$DUISRCDIR/widgets/core/duiwidget_p.h \
    $$DUISRCDIR/widgets/duiwidgetmodel_p.h \
    $$DUISRCDIR/widgets/duiscenewindow_p.h \
    $$DUISRCDIR/widgets/duiobjectmenu.h \
    $$DUISRCDIR/style/duiextensionhandlestyle.h

include(../common_bot.pri)
