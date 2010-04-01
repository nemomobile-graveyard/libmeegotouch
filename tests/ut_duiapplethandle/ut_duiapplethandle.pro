include(../common_top.pri)
include(../common_duiextensions.pri)

INCLUDEPATH += \
    $$DUISRCDIR/corelib/widgets \
    $$DUISRCDIR/corelib/style
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

MODEL_HEADERS += \
    $$DUISRCDIR/corelib/widgets/duiwidgetmodel.h \
    $$DUISRCDIR/extensions/applicationextension/duiextensionhandlemodel.h \
    $$DUISRCDIR/extensions/mashup/mashup/duiapplethandlemodel.h

STYLE_HEADERS += \
    $$DUISRCDIR/extensions/style/duiextensionhandlestyle.h \
    $$DUISRCDIR/extensions/style/duiapplethandlestyle.h

SOURCES += \
    ut_duiapplethandle.cpp \
    $$DUISRCDIR/extensions/applicationextension/duiextensionhandle.cpp \
    $$DUISRCDIR/extensions/mashup/mashup/duiapplethandle.cpp \
    $$DUISRCDIR/corelib/widgets/core/duiwidgetcontroller.cpp \
    $$DUISRCDIR/corelib/widgets/duiwidgetmodel.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

HEADERS += \
    ut_duiapplethandle.h \
    $$DUISRCDIR/extensions/applicationextension/duiextensionhandle.h \
    $$DUISRCDIR/extensions/applicationextension/duiextensionhandle_p.h \
    $$DUISRCDIR/extensions/applicationextension/duiextensionhandlemodel.h \
    $$DUISRCDIR/extensions/mashup/mashup/duiapplethandle.h \
    $$DUISRCDIR/extensions/mashup/mashup/duiapplethandle_p.h \
    $$DUISRCDIR/extensions/mashup/mashup/duiapplethandlemodel.h \
    $$DUISRCDIR/corelib/widgets/core/duiwidgetcontroller_p.h \
    $$DUISRCDIR/corelib/widgets/core/duiwidget_p.h \
    $$DUISRCDIR/corelib/widgets/duiwidgetmodel_p.h \
    $$DUISRCDIR/corelib/widgets/duiscenewindow_p.h \
    $$DUISRCDIR/corelib/widgets/duiobjectmenu.h \
    $$STYLE_HEADERS

include(../common_bot.pri)
