MSRCDIR = ../../src/
STUBSDIR = ../stubs
INCLUDEPATH += \
    . \
    $$STUBSDIR \
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

win32|macx {
    macx {
        QMAKE_LFLAGS += -F../../lib
        LIBS += -framework m
    }
    win32:LIBS += -lmeegotouchcore0
} else {
    LIBS += -lmeegotouchcore
}

QMAKE_CXXFLAGS += -Werror
support_files.files =
support_files.path = $$[QT_INSTALL_LIBS]/libmeegotouch-benchmarks
INSTALLS += support_files

CONFIG-=app_bundle
