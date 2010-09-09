# hardcoded to avoid dependency on files created by configure
DEFINES += THEMEDIR=\\\"\"/usr/share/themes\"\\\"

INCLUDEPATH += . \
    ../ \
    ../../src/include \
    ../../src \
    ../../src/corelib \
    ../../src/corelib/core \
    ../../src/corelib/theme

DEPENDPATH += $$INCLUDEPATH
QMAKE_LIBDIR += ../lib
TEMPLATE = app
DEPENDPATH += .
QT += svg \
    network

#DEFINES += PRINT_INFO_MESSAGES

# Check for mixing of const and non-const iterators,
# which can cause problems when built with some compilers:
DEFINES += QT_STRICT_ITERATORS

# Input
SOURCES += benchmarkremoteclient.cpp \
    ../../src/corelib/theme/mthemedaemon.cpp \
    ../../src/corelib/theme/mcommonpixmaps.cpp \
    ../../src/corelib/theme/mimagedirectory.cpp \
    ../../src/corelib/theme/mthemedaemonclient.cpp \
    ../../src/corelib/theme/mthemedaemonprotocol.cpp \
    ../../src/corelib/theme/mthemeresourcemanager.cpp \
    ../../src/corelib/core/mcpumonitor.cpp \
    main.cpp \
    benchmarklocalclient.cpp \
    common.cpp \
    ../../src/corelib/theme/mlocalthemedaemon.cpp

contains(DEFINES, HAVE_GCONF) {
    SOURCES += ../../src/corelib/core/mgconfitem.cpp

    CONFIG += link_pkgconfig
    PKGCONFIG += gconf-2.0
} else {
    SOURCES += ../../src/corelib/core/mgconfitem_stub.cpp
}

HEADERS += benchmarkremoteclient.h \
    ../../src/corelib/theme/imthemedaemon.h \
    ../../src/corelib/theme/mthemedaemon.h \
    ../../src/corelib/theme/mcommonpixmaps.h \
    ../../src/corelib/theme/mimagedirectory.h \
    ../../src/corelib/theme/mthemedaemonclient.h \
    ../../src/corelib/theme/mthemedaemonprotocol.h \
    ../../src/corelib/theme/mthemeresourcemanager.h \
    ../../src/corelib/core/mgconfitem.h \
    ../../src/corelib/core/mcpumonitor.h \
    benchmarklocalclient.h \
    common.h \
    ../../src/corelib/theme/mthemedaemon.h \
    ../../src/corelib/theme/mlocalthemedaemon.h
