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
           ../../src/corelib/theme/duithemedaemon.cpp \
           ../../src/corelib/theme/duicommonpixmaps.cpp \
           ../../src/corelib/theme/duiimagedirectory.cpp \
           ../../src/corelib/theme/duithemedaemonclient.cpp \
           ../../src/corelib/theme/duithemedaemonprotocol.cpp \
           ../../src/corelib/theme/duithemeresourcemanager.cpp \
           ../../src/corelib/core/duigconfitem.cpp \
           ../../src/corelib/core/duicpumonitor.cpp \


HEADERS += clientmanager.h \
           client.h \
           ../keypresswaiter.h \
           ../../src/corelib/theme/iduithemedaemon.h \
           ../../src/corelib/theme/duithemedaemon.h \
           ../../src/corelib/theme/duicommonpixmaps.h \
           ../../src/corelib/theme/duiimagedirectory.h \
           ../../src/corelib/theme/duithemedaemonclient.h \
           ../../src/corelib/theme/duithemedaemonprotocol.h \
           ../../src/corelib/theme/duithemeresourcemanager.h \
           ../../src/corelib/core/duigconfitem.h \
           ../../src/corelib/core/duicpumonitor.h \

SUBDIRS += testdaemodrgtrgn
