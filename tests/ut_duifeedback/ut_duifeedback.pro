#include(../common_top.pri)

# ****************************************************************************
# This is the contents of "../common_top.pri" modified so that libdui library
# is not included. (It's not needed with this test)
# ****************************************************************************
include(../check.pri)

# for defines
include(../../mkspecs/common.pri)

DUISRCDIR = ../../src
#STUBSDIR = ../stubs
#INCLUDEPATH += . $$DUISRCDIR/include
#DEPENDPATH = $$INCLUDEPATH
#QMAKE_LIBDIR += ../../lib /usr/local/lib
CONFIG += debug
QT += testlib
TEMPLATE = app
# DEFINES += QT_NO_DEBUG_OUTPUT
DEFINES += UNIT_TEST
target.path = $$[QT_INSTALL_LIBS]/libdui-tests
INSTALLS += target

#win32|macx {
#    macx {
#        QMAKE_LFLAGS += -F../../lib
#        LIBS += -framework dui
#    }
#    win32:LIBS += -L../../lib -ldui0
#} else {
#    LIBS += ../../lib/libdui.so
#}

QMAKE_CXXFLAGS += -Werror

support_files.files =
support_files.path = $$[QT_INSTALL_LIBS]/libdui-tests
INSTALLS += support_files

# ****************************************************************************
# "../common_top.pri" ends.
# ****************************************************************************

INCLUDEPATH += $$DUISRCDIR/feedback

TARGET = ut_duifeedback

TESTED_SOURCES = $$DUISRCDIR/feedback/duifeedback.cpp \
                 $$DUISRCDIR/feedback/duifeedback_p.cpp

TESTED_HEADERS = $$DUISRCDIR/feedback/duifeedback.h \
                 $$DUISRCDIR/feedback/duifeedback_p.h

copysourcefiles.input = TESTED_SOURCES
copysourcefiles.output = ${QMAKE_FILE_BASE}.cpp
copysourcefiles.commands = $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_BASE}.cpp
copysourcefiles.variable_out = SOURCES
copysourcefiles.CONFIG = target_predeps no_link
QMAKE_EXTRA_COMPILERS += copysourcefiles

copyheaderfiles.input = TESTED_HEADERS
copyheaderfiles.output = ${QMAKE_FILE_BASE}.h
copyheaderfiles.commands = $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_BASE}.h
copyheaderfiles.variable_out = HEADERS
copyheaderfiles.CONFIG = target_predeps no_link
QMAKE_EXTRA_COMPILERS += copyheaderfiles

mocheaderfiles.input = TESTED_HEADERS
mocheaderfiles.output = moc_${QMAKE_FILE_BASE}.cpp
mocheaderfiles.commands = $$QMAKE_MOC $(DEFINES) $(INCPATH) ${QMAKE_FILE_BASE}.h -o moc_${QMAKE_FILE_BASE}.cpp
mocheaderfiles.variable_out = SOURCES
mocheaderfiles.CONFIG = target_predeps no_link
QMAKE_EXTRA_COMPILERS += mocheaderfiles

SOURCES += \
    ut_duifeedback.cpp \
    duifeedbackplayer_mock.cpp \
    duiapplication_mock.cpp

HEADERS += \
    ut_duifeedback.h \
    duifeedbackplayer.h \
    duifeedbackplayer_mock.h \
    duiapplication.h \
    duiapplication_mock.h

include(../common_bot.pri)
