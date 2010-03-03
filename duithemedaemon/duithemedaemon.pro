include(../mkspecs/common.pri)

INCLUDEPATH += . ../src/include ../src ../src/core
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
           ../src/theme/duithemedaemon.cpp \
           ../src/theme/duicommonpixmaps.cpp \
           ../src/theme/duiimagedirectory.cpp \
           ../src/theme/duithemedaemonclient.cpp \
           ../src/theme/duithemedaemonprotocol.cpp \
           ../src/theme/duithemeresourcemanager.cpp \
           ../src/core/duigconfitem.cpp \
           ../src/core/duicpumonitor.cpp \

HEADERS += \
           duithemedaemonserver.h \
           ../src/theme/duithemedaemon.h \
           ../src/theme/duicommonpixmaps.h \
           ../src/theme/duiimagedirectory.h \
           ../src/theme/duithemedaemonclient.h \
           ../src/theme/duithemedaemonprotocol.h \
           ../src/theme/duithemeresourcemanager.h \
           ../src/core/duigconfitem.h \
           ../src/core/duicpumonitor.h \
           keypresswaiter.h \

QMAKE_EXTRA_TARGETS += check
check.depends = $$TARGET
check.commands = $$system(true)

QMAKE_EXTRA_TARGETS += check-xml
check-xml.depends = $$TARGET
check-xml.commands = $$system(true)

target.path = $$DUI_INSTALL_BIN
INSTALLS += target \
