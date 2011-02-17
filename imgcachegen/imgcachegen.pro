include(../mkspecs/common.pri)

INCLUDEPATH += ../src/include \
               ../src/corelib/theme \
               ../src/corelib/core

TEMPLATE = app
TARGET = imgcachegen

QT += svg
contains(DEFINES, HAVE_MEEGOGRAPHICSSYSTEM) {
    QT += meegographicssystemhelper
}

SOURCES += imgcachegen.cpp \
           ../src/corelib/theme/mimagedirectory.cpp \
           ../src/corelib/theme/mthemedaemon.cpp \
           ../src/corelib/theme/mthemedaemonprotocol.cpp \
           ../src/corelib/theme/mpixmaphandle.cpp \
           ../src/corelib/theme/mthemedaemonclient.cpp \
           ../src/corelib/theme/mcommonpixmaps.cpp \
           ../src/corelib/core/mcpumonitor.cpp \
           ../src/corelib/core/msystemdirectories.cpp \
           ../src/corelib/core/mgraphicssystemhelper.cpp

HEADERS += ../src/corelib/theme/mimagedirectory.h \
           ../src/corelib/theme/mthemedaemon.h \
           ../src/corelib/theme/mthemedaemonprotocol.h \
           ../src/corelib/theme/mpixmaphandle.h \
           ../src/corelib/theme/mthemedaemonclient.h \
           ../src/corelib/theme/mcommonpixmaps.h \
           ../src/corelib/core/mcpumonitor.h \
           ../src/corelib/core/msystemdirectories.h \
           ../src/corelib/core/mgraphicssystemhelper.h

target.path = $$M_INSTALL_BIN
INSTALLS += target

