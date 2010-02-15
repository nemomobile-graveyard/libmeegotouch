include(check.pri)

# for defines
include(../mkspecs/common.pri)

DUISRCDIR = ../../src
STUBSDIR = ../stubs
INCLUDEPATH += . $$DUISRCDIR $$STUBSDIR  $$DUISRCDIR/include
DEPENDPATH = $$INCLUDEPATH
QMAKE_LIBDIR += ../../lib /usr/local/lib
CONFIG += debug
CONFIG -= app_bundle
QT += testlib dbus svg network
TEMPLATE = app
# DEFINES += QT_NO_DEBUG_OUTPUT
DEFINES += UNIT_TEST
target.path = $$[QT_INSTALL_LIBS]/libdui-tests
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
support_files.path = $$[QT_INSTALL_LIBS]/libdui-tests
INSTALLS += support_files
