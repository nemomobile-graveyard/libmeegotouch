include(../../mkspecs/common.pri)

INCLUDEPATH += . ../ ../../src/include ../../src ../../src/corelib ../../src/corelib/core ../../src/corelib/theme
DEPENDPATH += $$INCLUDEPATH
QMAKE_LIBDIR += ../lib
TEMPLATE = app
DEPENDPATH += .

QT += svg network

# override theme directory
DEFINES += IMAGESDIR=\\\"./images\\\"

# we want to be able to access private classes
DEFINES += UNIT_TEST

# Input
SOURCES += main.cpp \
           clientmanager.cpp \
           testclient.cpp \
            ../../src/corelib/theme/mremotethemedaemon.cpp \
           ../../src/corelib/theme/mthemedaemon.cpp \
           ../../src/corelib/theme/mcommonpixmaps.cpp \
           ../../src/corelib/theme/mimagedirectory.cpp \
           ../../src/corelib/theme/mthemedaemonclient.cpp \
           ../../src/corelib/theme/mthemedaemonprotocol.cpp \
           ../../src/corelib/theme/mthemeresourcemanager.cpp \
           ../../src/corelib/core/mcpumonitor.cpp \

contains(DEFINES, HAVE_GCONF) {
    CONFIG += link_pkgconfig
    PKGCONFIG += gconf-2.0
    SOURCES += ../../src/corelib/core/mgconfitem.cpp
} else {
    SOURCES += ../../src/corelib/core/mgconfitem_stub.cpp
}

HEADERS += clientmanager.h \
           testclient.h \
           ../keypresswaiter.h \
           ../../src/corelib/theme/mremotethemedaemon.h \
           ../../src/corelib/theme/imthemedaemon.h \
           ../../src/corelib/theme/mthemedaemon.h \
           ../../src/corelib/theme/mcommonpixmaps.h \
           ../../src/corelib/theme/mimagedirectory.h \
           ../../src/corelib/theme/mthemedaemonclient.h \
           ../../src/corelib/theme/mthemedaemonprotocol.h \
           ../../src/corelib/theme/mthemeresourcemanager.h \
           ../../src/corelib/core/mgconfitem.h \
           ../../src/corelib/core/mcpumonitor.h \

SUBDIRS += testdaemodrgtrgn
