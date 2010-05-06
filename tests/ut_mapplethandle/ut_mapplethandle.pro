include(../common_top.pri)
include(../common_mextensions.pri)

INCLUDEPATH += \
    $$MSRCDIR/corelib/widgets \
    $$MSRCDIR/corelib/style
QT += xml

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
mgenerator_style.name = mgenerator style
mgenerator_style.input = STYLE_HEADERS
mgenerator_style.depends = ../../mgen/mgen
mgenerator_style.output = $$MGEN_OUTDIR/gen_${QMAKE_FILE_BASE}data.cpp
mgenerator_style.commands += ../../mgen/mgen --style ${QMAKE_FILE_NAME} $$MGEN_OUTDIR
mgenerator_style.clean += $$MGEN_OUTDIR/gen_*
mgenerator_style.CONFIG = target_predeps no_link
mgenerator_style.variable_out = GENERATED_SOURCES
QMAKE_EXTRA_COMPILERS += mgenerator_model mgenerator_style

MODEL_HEADERS += \
    $$MSRCDIR/corelib/widgets/mwidgetmodel.h \
    $$MSRCDIR/extensions/applicationextension/mextensionhandlemodel.h \
    $$MSRCDIR/extensions/mashup/mashup/mapplethandlemodel.h

STYLE_HEADERS += \
    $$MSRCDIR/extensions/style/mextensionhandlestyle.h \
    $$MSRCDIR/extensions/style/mapplethandlestyle.h

SOURCES += \
    ut_mapplethandle.cpp \
    $$MSRCDIR/extensions/applicationextension/mextensionhandle.cpp \
    $$MSRCDIR/extensions/mashup/mashup/mapplethandle.cpp \
    $$MSRCDIR/corelib/widgets/core/mwidgetcontroller.cpp \
    $$MSRCDIR/corelib/widgets/mwidgetmodel.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

HEADERS += \
    ut_mapplethandle.h \
    $$MSRCDIR/extensions/applicationextension/mextensionhandle.h \
    $$MSRCDIR/extensions/applicationextension/mextensionhandle_p.h \
    $$MSRCDIR/extensions/applicationextension/mextensionhandlemodel.h \
    $$MSRCDIR/extensions/mashup/mashup/mapplethandle.h \
    $$MSRCDIR/extensions/mashup/mashup/mapplethandle_p.h \
    $$MSRCDIR/extensions/mashup/mashup/mapplethandlemodel.h \
    $$MSRCDIR/corelib/widgets/core/mwidgetcontroller_p.h \
    $$MSRCDIR/corelib/widgets/core/mwidget_p.h \
    $$MSRCDIR/corelib/widgets/mwidgetmodel_p.h \
    $$MSRCDIR/corelib/widgets/mscenewindow_p.h \
    $$MSRCDIR/corelib/widgets/mobjectmenu.h \
    $$STYLE_HEADERS

include(../common_bot.pri)
