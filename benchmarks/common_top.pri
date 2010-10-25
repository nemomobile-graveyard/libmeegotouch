# for defines
include(../mkspecs/common.pri)

MSRCDIR = $${M_SOURCE_TREE}/src
STUBSDIR = ../stubs
INCLUDEPATH += \
    . \
    ../common \
    $$MSRCDIR/include \
    $$MSRCDIR/corelib/core \
    $$MSRCDIR/corelib/widgets \
    $$MSRCDIR/corelib/workspace \
    $$MSRCDIR/corelib/style \
    $$MSRCDIR/views \
    $$MSRCDIR/views/style \
    ../memorybenchmark/

DEPENDPATH = $$INCLUDEPATH
QMAKE_LIBDIR += ../../lib
CONFIG += debug
QT += testlib dbus svg
TEMPLATE = app
# DEFINES += QT_NO_DEBUG_OUTPUT
DEFINES += UNIT_TEST
target.path = $$[QT_INSTALL_LIBS]/libmeegotouch-benchmarks
INSTALLS += target

LIBS += $$mAddLibrary(meegotouchcore)

support_files.files =
support_files.path = $$[QT_INSTALL_LIBS]/libmeegotouch-benchmarks
INSTALLS += support_files

CONFIG-=app_bundle
