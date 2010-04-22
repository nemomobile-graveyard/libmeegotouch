include(../mkspecs/common.pri)

INCLUDEPATH += \
    . \
    ../src/include \
    ../src/corelib \
    ../src/corelib/core \

DEPENDPATH += $$INCLUDEPATH
QMAKE_LIBDIR += ../lib
TEMPLATE = app
DEPENDPATH += .

QT += svg network

DEFINES += MTHEME_PRINT_DEBUG
#DEFINES += CLOSE_ON_ENTER

# Check for mixing of const and non-const iterators, 
# which can cause problems when built with some compilers:
DEFINES += QT_STRICT_ITERATORS

!win32:CONFIG += link_pkgconfig
PKGCONFIG += gconf-2.0

# Input
SOURCES += main.cpp \
           mthemedaemonserver.cpp \
           ../src/corelib/theme/mthemedaemon.cpp \
           ../src/corelib/theme/mcommonpixmaps.cpp \
           ../src/corelib/theme/mimagedirectory.cpp \
           ../src/corelib/theme/mthemedaemonclient.cpp \
           ../src/corelib/theme/mthemedaemonprotocol.cpp \
           ../src/corelib/theme/mthemeresourcemanager.cpp \
           ../src/corelib/core/mgconfitem.cpp \
           ../src/corelib/core/mcpumonitor.cpp \

HEADERS += \
           mthemedaemonserver.h \
           ../src/corelib/theme/mthemedaemon.h \
           ../src/corelib/theme/mcommonpixmaps.h \
           ../src/corelib/theme/mimagedirectory.h \
           ../src/corelib/theme/mthemedaemonclient.h \
           ../src/corelib/theme/mthemedaemonprotocol.h \
           ../src/corelib/theme/mthemeresourcemanager.h \
           ../src/corelib/core/mgconfitem.h \
           ../src/corelib/core/mcpumonitor.h \
           keypresswaiter.h \

QMAKE_EXTRA_TARGETS += check
check.depends = $$TARGET
check.commands = $$system(true)

QMAKE_EXTRA_TARGETS += check-xml
check-xml.depends = $$TARGET
check-xml.commands = $$system(true)

target.path = $$M_INSTALL_BIN
INSTALLS += target \
