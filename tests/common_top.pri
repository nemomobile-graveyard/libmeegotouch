include(check.pri)

# for defines
include(../mkspecs/common.pri)

MSRCDIR = ../../src
STUBSDIR = ../stubs
INCLUDEPATH += \
    . \
    $$MSRCDIR \
    $$STUBSDIR \
    $$MSRCDIR/include \
    $$MSRCDIR/views/style \

DEPENDPATH = $$INCLUDEPATH
QMAKE_LIBDIR += ../../lib /usr/local/lib
CONFIG += debug
CONFIG -= app_bundle
QT += testlib dbus svg network
TEMPLATE = app
# DEFINES += QT_NO_DEBUG_OUTPUT
DEFINES += UNIT_TEST
target.path = $$[QT_INSTALL_LIBS]/libmeegotouch-tests
INSTALLS += target

win32|macx {
    macx {
        QMAKE_LFLAGS += -F../../lib
        LIBS += -framework meegotouch
    }
    win32:LIBS += -lmeegotouchcore0 -lmeegotouchviews0
} else {
    LIBS += -lmeegotouchcore -lmeegotouchviews
}

QMAKE_CXXFLAGS += -Werror

support_files.files =
support_files.path = $$[QT_INSTALL_LIBS]/libmeegotouch-tests
INSTALLS += support_files
