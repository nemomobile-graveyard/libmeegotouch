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

DEFINES += DUITHEME_PRINT_DEBUG
#DEFINES += CLOSE_ON_ENTER

# enable QString optimizations
DEFINES += QT_USE_FAST_CONCATENATION QT_USE_FAST_OPERATOR_PLUS

# Check for mixing of const and non-const iterators, 
# which can cause problems when built with some compilers:
DEFINES += QT_STRICT_ITERATORS

!win32:CONFIG += link_pkgconfig
PKGCONFIG += gconf-2.0

# Input
SOURCES += main.cpp \
           duithemedaemonserver.cpp \
           ../src/corelib/theme/duithemedaemon.cpp \
           ../src/corelib/theme/duicommonpixmaps.cpp \
           ../src/corelib/theme/duiimagedirectory.cpp \
           ../src/corelib/theme/duithemedaemonclient.cpp \
           ../src/corelib/theme/duithemedaemonprotocol.cpp \
           ../src/corelib/theme/duithemeresourcemanager.cpp \
           ../src/corelib/core/duigconfitem.cpp \
           ../src/corelib/core/duicpumonitor.cpp \

HEADERS += \
           duithemedaemonserver.h \
           ../src/corelib/theme/duithemedaemon.h \
           ../src/corelib/theme/duicommonpixmaps.h \
           ../src/corelib/theme/duiimagedirectory.h \
           ../src/corelib/theme/duithemedaemonclient.h \
           ../src/corelib/theme/duithemedaemonprotocol.h \
           ../src/corelib/theme/duithemeresourcemanager.h \
           ../src/corelib/core/duigconfitem.h \
           ../src/corelib/core/duicpumonitor.h \
           keypresswaiter.h \

QMAKE_EXTRA_TARGETS += check
check.depends = $$TARGET
check.commands = $$system(true)

QMAKE_EXTRA_TARGETS += check-xml
check-xml.depends = $$TARGET
check-xml.commands = $$system(true)

target.path = $$DUI_INSTALL_BIN
INSTALLS += target \
