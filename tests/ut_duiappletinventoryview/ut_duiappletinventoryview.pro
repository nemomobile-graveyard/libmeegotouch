include(../common_top.pri)
TARGET = ut_duiappletinventoryview
INCLUDEPATH += $$DUISRCDIR/mashup/mashup
INCLUDEPATH += $$DUISRCDIR/mashup/appletinstallation
INCLUDEPATH += $$DUISRCDIR/style
INCLUDEPATH += $$DUISRCDIR/widgets
INCLUDEPATH += $$DUISRCDIR/widgets/core
INCLUDEPATH += $$DUISRCDIR/core

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

STYLE_HEADERS += $$DUISRCDIR/style/duiappletinventorystyle.h
MODEL_HEADERS += $$DUISRCDIR/widgets/duiwidgetmodel.h \
    $$DUISRCDIR/mashup/mashup/duiappletinventorymodel.h

# unit test and unit
SOURCES += \
    ut_duiappletinventoryview.cpp \
    $$DUISRCDIR/mashup/mashup/duiappletinventoryview.cpp \
    $$TEST_SOURCES \ 

# base classes
SOURCES += \
    $$DUISRCDIR/widgets/core/duiwidgetcontroller.cpp \
    $$DUISRCDIR/widgets/core/duiwidget.cpp \
    $$DUISRCDIR/widgets/duiwidgetmodel.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_duiappletinventoryview.h \
    $$DUISRCDIR/widgets/core/duiwidgetcontroller_p.h \
    $$DUISRCDIR/widgets/views/duiextendingbackgroundview_p.h \
    $$DUISRCDIR/mashup/mashup/duiappletbutton.h \
    $$DUISRCDIR/mashup/mashup/duiappletinventory.h \
    $$DUISRCDIR/mashup/mashup/duiappletinventoryview.h \
    $$DUISRCDIR/mashup/mashup/duiappletinventorymodel.h \
    $$DUISRCDIR/widgets/duiwidgetmodel_p.h \
    $$DUISRCDIR/widgets/duiscenewindow_p.h \
    $$DUISRCDIR/widgets/duiobjectmenu.h

include(../common_bot.pri)
