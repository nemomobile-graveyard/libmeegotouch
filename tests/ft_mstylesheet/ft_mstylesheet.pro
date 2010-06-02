include(../common_top.pri)
TARGET = ft_mstylesheet
QT += dbus svg network
LIBRARYPATH += $$MSRCDIR

win32 {
    QMAKE_MOC = $${IN_PWD}\..\..\mmoc\mmoc
    contains(DEFINES, IS_ARMEL) {
        QMAKE_MOC = perl.exe $${IN_PWD}\..\..\mmoc\mmoc.pl
    }
} else {
    PRE_TARGETDEPS += ../../mgen/mgen
    QMAKE_MOC = PATH=../../mgen:$$(PATH) $${IN_PWD}/../../mmoc/mmoc
    contains(DEFINES, IS_ARMEL) {
        QMAKE_MOC = PATH=../../mgen:$$(PATH) perl $${IN_PWD}/../../mmoc/mmoc.pl
    }
}

MGEN_OUTDIR = .
mgenerator_style.name = mgenerator style
mgenerator_style.input = STYLE_HEADERS
mgenerator_style.depends = ../../mgen/mgen
mgenerator_style.output = $$MGEN_OUTDIR/gen_${QMAKE_FILE_BASE}data.cpp
mgenerator_style.commands += ../../mgen/mgen --style ${QMAKE_FILE_NAME} $$MGEN_OUTDIR
mgenerator_style.clean += $$MGEN_OUTDIR/gen_*
mgenerator_style.CONFIG = target_predeps no_link
mgenerator_style.variable_out = GENERATED_SOURCES
QMAKE_EXTRA_COMPILERS += mgenerator_style

STYLE_HEADERS += testobjectstyle.h testobject2style.h testobject3style.h

SOURCES += \
	ft_mstylesheet.cpp \
    ../../src/corelib/theme/mlogicalvalues.cpp \
    ../../src/corelib/style/mstylesheetattribute.cpp

HEADERS += \
	ft_mstylesheet.h \
	testobjectstyle.h \
	testobject2style.h \
	testobject3style.h \
    testwidget.h \
    testwidget2.h \
    testwidget3.h \
    ../../src/corelib/theme/mlogicalvalues.h \
    ../../src/corelib/style/mstylesheetattribute.h

support_files.files += \
        *.css
	
include(../common_bot.pri)
