include( ../common.pri )
include( ../../mkspecs/common.pri )

# make sure mgen executable is found
win32 {
    debug {
        M_MGEN_EXECUTABLE = $$M_BUILD_TREE/mgen/debug/mgen.exe
    } else {
        M_MGEN_EXECUTABLE = $$M_BUILD_TREE/mgen/release/mgen.exe
    }
} else {
    M_MGEN_EXECUTABLE = $$M_BUILD_TREE/mgen/mgen
}

include( ../../mkspecs/features/meegotouch_mgen.prf )

LIBS += $$mAddLibrary(meegotouchsettings) \
        $$mAddLibrary(meegotouchviews) \
        $$mAddLibrary(meegotouchextensions)

TEMPLATE = app
TARGET = testwidget

# Input
SOURCES += testwidgetmodel.cpp \
    main.cpp \
    testwidget.cpp \
    testwidgetview.cpp
HEADERS += testwidget.h \
    testwidgetmodel.h \
    testwidgetstyle.h \
    testwidgetview.h
MODEL_HEADERS += testwidgetmodel.h
STYLE_HEADERS += testwidgetstyle.h
