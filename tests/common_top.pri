include(check.pri)

# for defines
include(../mkspecs/common.pri)

# for mmoc and mgen settings
include(../src/predeps.pri)
include(../mkspecs/features/meegotouch_mmoc.prf)
include(../mkspecs/features/meegotouch_mgen.prf)

MSRCDIR = $${M_SOURCE_TREE}/src
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

LIBS += $$mAddLibrary(meegotouchcore) \
        $$mAddLibrary(meegotouchviews)

support_files.files =
support_files.path = $$[QT_INSTALL_LIBS]/libmeegotouch-tests
INSTALLS += support_files
