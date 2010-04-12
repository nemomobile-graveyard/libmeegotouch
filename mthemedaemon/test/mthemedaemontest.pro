include(../../mkspecs/common.pri)

INCLUDEPATH += . ../ ../../src/include ../../src ../../src/corelib ../../src/corelib/core ../../src/corelib/theme
DEPENDPATH += $$INCLUDEPATH
QMAKE_LIBDIR += ../lib
TEMPLATE = app
DEPENDPATH += .

QT += svg network

#DEFINES += PRINT_INFO_MESSAGES

# Check for mixing of const and non-const iterators, 
# which can cause problems when built with some compilers:
DEFINES += QT_STRICT_ITERATORS

# override theme directory
DEFINES += IMAGESDIR=\\\"./images\\\"

!win32:CONFIG += link_pkgconfig
PKGCONFIG += gconf-2.0

# Input
SOURCES += main.cpp \
           clientmanager.cpp \
           client.cpp \
           ../../src/corelib/theme/mthemedaemon.cpp \
           ../../src/corelib/theme/mcommonpixmaps.cpp \
           ../../src/corelib/theme/mimagedirectory.cpp \
           ../../src/corelib/theme/mthemedaemonclient.cpp \
           ../../src/corelib/theme/mthemedaemonprotocol.cpp \
           ../../src/corelib/theme/mthemeresourcemanager.cpp \
           ../../src/corelib/core/mgconfitem.cpp \
           ../../src/corelib/core/mcpumonitor.cpp \


HEADERS += clientmanager.h \
           client.h \
           ../keypresswaiter.h \
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
