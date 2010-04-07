INCLUDEPATH += . ../../../src/include ../../../src ../../../src/corelib/core ../../../src/corelib/

TEMPLATE = app
DEPENDPATH += .

QT += svg network

#DEFINES += DUITHEME_PRINT_DEBUG
#DEFINES += CLOSE_ON_ENTER

DEFINES += THEMEDIR=\\\"\"themes\"\\\"

# enable QString optimizations
DEFINES += QT_USE_FAST_CONCATENATION QT_USE_FAST_OPERATOR_PLUS

# Check for mixing of const and non-const iterators, 
# which can cause problems when built with some compilers:
DEFINES += QT_STRICT_ITERATORS

CONFIG += link_pkgconfig
PKGCONFIG += gconf-2.0

# Input
SOURCES += ../../main.cpp \
           ../../duithemedaemonserver.cpp \
           ../../../src/corelib/theme/duithemedaemon.cpp \
           ../../../src/corelib/theme/duicommonpixmaps.cpp \
           ../../../src/corelib/theme/duiimagedirectory.cpp \
           ../../../src/corelib/theme/duithemedaemonclient.cpp \
           ../../../src/corelib/theme/duithemedaemonprotocol.cpp \
           ../../../src/corelib/theme/duithemeresourcemanager.cpp \
           ../../../src/corelib/core/duigconfitem.cpp \
           ../../../src/corelib/core/duicpumonitor.cpp \

HEADERS += \
           ../../duithemedaemonserver.h \
           ../../../src/corelib/theme/duithemedaemon.h \
           ../../../src/corelib/theme/duicommonpixmaps.h \
           ../../../src/corelib/theme/duiimagedirectory.h \
           ../../../src/corelib/theme/duithemedaemonclient.h \
           ../../../src/corelib/theme/duithemedaemonprotocol.h \
           ../../../src/corelib/theme/duithemeresourcemanager.h \
           ../../../src/corelib/core/duigconfitem.h \
           ../../../src/corelib/core/duicpumonitor.h \

