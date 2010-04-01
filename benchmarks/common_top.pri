DUISRCDIR = ../../src/
STUBSDIR = ../stubs
INCLUDEPATH += \
    . \
    $$STUBSDIR \
    $$DUISRCDIR/include \
    $$DUISRCDIR/corelib/core \
    $$DUISRCDIR/corelib/widgets \
    $$DUISRCDIR/corelib/workspace \
    $$DUISRCDIR/corelib/style \
    $$DUISRCDIR/views \
    $$DUISRCDIR/views/style \
    ../memorybenchmark/

DEPENDPATH = $$INCLUDEPATH
QMAKE_LIBDIR += ../../lib
CONFIG += debug
QT += testlib dbus svg
TEMPLATE = app
# DEFINES += QT_NO_DEBUG_OUTPUT
DEFINES += UNIT_TEST
target.path = $$[QT_INSTALL_LIBS]/libdui-benchmarks
INSTALLS += target

win32|macx {
    macx {
        QMAKE_LFLAGS += -F../../lib
        LIBS += -framework dui
    }
    win32:LIBS += -lduicore0
} else {
    LIBS += -lduicore
}

QMAKE_CXXFLAGS += -Werror
support_files.files =
support_files.path = $$[QT_INSTALL_LIBS]/libdui-benchmarks
INSTALLS += support_files

CONFIG-=app_bundle
