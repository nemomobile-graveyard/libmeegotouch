include(../common_top.pri)

TARGET = ut_duiapplethandle
INCLUDEPATH += $$DUISRCDIR/mashup/mashup \
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
QMAKE_EXTRA_COMPILERS += duigenerator_model

MODEL_HEADERS += $$DUISRCDIR/widgets/duiwidgetmodel.h \
    $$DUISRCDIR/mashup/mashup/duiapplethandlemodel.h

SOURCES += ut_duiapplethandle.cpp \
    $$DUISRCDIR/mashup/mashup/duiapplethandle.cpp \
    $$DUISRCDIR/widgets/core/duiwidgetcontroller.cpp \
    $$DUISRCDIR/widgets/duiwidgetmodel.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

HEADERS += ut_duiapplethandle.h \
    $$DUISRCDIR/mashup/mashup/duiapplethandle.h \
    $$DUISRCDIR/mashup/mashup/duiapplethandle_p.h \
    $$DUISRCDIR/mashup/mashup/duiapplethandlemodel.h \
    $$DUISRCDIR/widgets/core/duiwidgetcontroller_p.h \
    $$DUISRCDIR/widgets/core/duiwidget_p.h \
    $$DUISRCDIR/widgets/duiwidgetmodel_p.h \
    $$DUISRCDIR/widgets/duiscenewindow_p.h \
    $$DUISRCDIR/widgets/duiobjectmenu.h

include(../common_bot.pri)
