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

contains(DEFINES, HAVE_MEEGOGRAPHICSSYSTEM) {
    QT += meegographicssystemhelper
}

DEFINES += MTHEME_PRINT_DEBUG
#DEFINES += CLOSE_ON_ENTER

# enable QString optimizations
DEFINES += QT_USE_FAST_CONCATENATION QT_USE_FAST_OPERATOR_PLUS

# Check for mixing of const and non-const iterators, 
# which can cause problems when built with some compilers:
DEFINES += QT_STRICT_ITERATORS

!win32:CONFIG += link_pkgconfig
PKGCONFIG += gobject-2.0
PKGCONFIG += x11

# Input
SOURCES += main.cpp \
           mthemedaemonserver.cpp \
           ../src/corelib/theme/mthemedaemon.cpp \
           ../src/corelib/theme/mcommonpixmaps.cpp \
           ../src/corelib/theme/mimagedirectory.cpp \
           ../src/corelib/theme/mthemedaemonclient.cpp \
           ../src/corelib/theme/mpixmaphandle.cpp \
           ../src/corelib/theme/mthemedaemonprotocol.cpp \
           ../src/corelib/core/mcpumonitor.cpp \
           ../src/corelib/core/mgraphicssystemhelper.cpp \
           ../src/corelib/core/msystemdirectories.cpp \

contains(DEFINES, HAVE_GCONF) {
    PKGCONFIG += gconf-2.0
    SOURCES += ../src/corelib/core/mgconfitem.cpp
} else {
    SOURCES += ../src/corelib/core/mgconfitem_stub.cpp
}

HEADERS += \
           mthemedaemonserver.h \
           ../src/corelib/theme/mthemedaemon.h \
           ../src/corelib/theme/mcommonpixmaps.h \
           ../src/corelib/theme/mimagedirectory.h \
           ../src/corelib/theme/mpixmaphandle.h \
           ../src/corelib/theme/mthemedaemonclient.h \
           ../src/corelib/theme/mthemedaemonprotocol.h \
           ../src/corelib/core/mgconfitem.h \
           ../src/corelib/core/mcpumonitor.h \
           ../src/corelib/core/mgraphicssystemhelper.h \
           ../src/corelib/core/msystemdirectories.h \
           keypresswaiter.h \

QMAKE_EXTRA_TARGETS += check
check.depends = $$TARGET
check.commands = $$system(true)

QMAKE_EXTRA_TARGETS += check-xml
check-xml.depends = $$TARGET
check-xml.commands = $$system(true)

target.path = $$M_INSTALL_BIN
INSTALLS += target \
