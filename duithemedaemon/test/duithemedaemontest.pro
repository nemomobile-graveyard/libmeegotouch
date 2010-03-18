include(../../mkspecs/common.pri)

INCLUDEPATH += . ../ ../../src/include ../../src ../../src/core ../../src/theme
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
           ../../src/theme/duithemedaemon.cpp \
           ../../src/theme/duicommonpixmaps.cpp \
           ../../src/theme/duiimagedirectory.cpp \
           ../../src/theme/duithemedaemonclient.cpp \
           ../../src/theme/duithemedaemonprotocol.cpp \
           ../../src/theme/duithemeresourcemanager.cpp \
           ../../src/core/duigconfitem.cpp \
           ../../src/core/duicpumonitor.cpp \


HEADERS += clientmanager.h \
           client.h \
           ../keypresswaiter.h \
           ../../src/theme/iduithemedaemon.h \
           ../../src/theme/duithemedaemon.h \
           ../../src/theme/duicommonpixmaps.h \
           ../../src/theme/duiimagedirectory.h \
           ../../src/theme/duithemedaemonclient.h \
           ../../src/theme/duithemedaemonprotocol.h \
           ../../src/theme/duithemeresourcemanager.h \
           ../../src/core/duigconfitem.h \
           ../../src/core/duicpumonitor.h \

