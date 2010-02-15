include(../common_top.pri)
TARGET = ft_duistylesheet
QT += dbus svg network
LIBRARYPATH += $$DUISRCDIR

win32 {
    QMAKE_MOC = perl $${IN_PWD}\..\..\duimoc\duimoc
} else {
    PRE_TARGETDEPS += ../../duigen/duigen
    QMAKE_MOC = PATH=../../duigen:$$(PATH) $${IN_PWD}/../../duimoc/duimoc
}

DUIGEN_OUTDIR = .
duigenerator_style.name = duigenerator style
duigenerator_style.input = STYLE_HEADERS
duigenerator_style.depends = ../../duigen/duigen
duigenerator_style.output = $$DUIGEN_OUTDIR/gen_${QMAKE_FILE_BASE}data.cpp
duigenerator_style.commands += ../../duigen/duigen --style ${QMAKE_FILE_NAME} $$DUIGEN_OUTDIR
duigenerator_style.clean += $$DUIGEN_OUTDIR/gen_*
duigenerator_style.CONFIG = target_predeps no_link
duigenerator_style.variable_out = GENERATED_SOURCES
QMAKE_EXTRA_COMPILERS += duigenerator_style

STYLE_HEADERS += testobjectstyle.h testobject2style.h testobject3style.h

SOURCES += \
	ft_duistylesheet.cpp \
    ../../src/theme/duilogicalvalues.cpp \
    ../../src/style/duistylesheetattribute.cpp

HEADERS += \
	ft_duistylesheet.h \
	testobjectstyle.h \
	testobject2style.h \
	testobject3style.h \
    testwidget.h \
    testwidget2.h \
    testwidget3.h \
    ../../src/theme/duilogicalvalues.h \
    ../../src/style/duistylesheetattribute.h

support_files.files += \
        *.css
	
include(../common_bot.pri)
