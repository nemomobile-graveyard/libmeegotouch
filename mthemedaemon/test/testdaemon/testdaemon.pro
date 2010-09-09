include(../../../mkspecs/common.pri)

INCLUDEPATH += . ../../../src/include ../../../src ../../../src/corelib/core ../../../src/corelib/

TEMPLATE = app
DEPENDPATH += .

QT += svg network

#DEFINES += MTHEME_PRINT_DEBUG
#DEFINES += CLOSE_ON_ENTER


DEFINES ~= s/THEMEDIR.*/THEMEDIR=\\\"\"themes\"\\\"/

# enable QString optimizations
DEFINES += QT_USE_FAST_CONCATENATION QT_USE_FAST_OPERATOR_PLUS

# Check for mixing of const and non-const iterators, 
# which can cause problems when built with some compilers:
DEFINES += QT_STRICT_ITERATORS

# Input
SOURCES += ../../main.cpp \
           ../../mthemedaemonserver.cpp \
           ../../../src/corelib/theme/mthemedaemon.cpp \
           ../../../src/corelib/theme/mcommonpixmaps.cpp \
           ../../../src/corelib/theme/mimagedirectory.cpp \
           ../../../src/corelib/theme/mthemedaemonclient.cpp \
           ../../../src/corelib/theme/mthemedaemonprotocol.cpp \
           ../../../src/corelib/theme/mthemeresourcemanager.cpp \
           ../../../src/corelib/core/mcpumonitor.cpp \

contains(DEFINES, HAVE_GCONF) {
    CONFIG += link_pkgconfig
    PKGCONFIG += gconf-2.0
    SOURCES += ../../../src/corelib/core/mgconfitem.cpp
}
} else {
    SOURCES += ../../../src/corelib/core/mgconfitem_stub.cpp
}

HEADERS += \
           ../../mthemedaemonserver.h \
           ../../../src/corelib/theme/mthemedaemon.h \
           ../../../src/corelib/theme/mcommonpixmaps.h \
           ../../../src/corelib/theme/mimagedirectory.h \
           ../../../src/corelib/theme/mthemedaemonclient.h \
           ../../../src/corelib/theme/mthemedaemonprotocol.h \
           ../../../src/corelib/theme/mthemeresourcemanager.h \
           ../../../src/corelib/core/mgconfitem.h \
           ../../../src/corelib/core/mcpumonitor.h \

