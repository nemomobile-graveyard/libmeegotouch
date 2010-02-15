DUISRCDIR = ../../src/
STUBSDIR = ../stubs
INCLUDEPATH += . $$DUISRCDIR $$STUBSDIR $$DUISRCDIR/include $$DUISRCDIR/core $$DUISRCDIR/widgets $$DUISRCDIR/workspace $$DUISRCDIR/style ../memorybenchmark/

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
    win32:LIBS += -L../../lib -ldui0
} else {
    LIBS += ../../lib/libdui.so
}

QMAKE_CXXFLAGS += -Werror
support_files.files =
support_files.path = $$[QT_INSTALL_LIBS]/libdui-benchmarks
INSTALLS += support_files

CONFIG-=app_bundle
