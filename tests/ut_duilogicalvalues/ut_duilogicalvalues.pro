include(../common_top.pri)
TARGET = ut_duilogicalvalues
QT += dbus svg network
LIBRARYPATH += $$DUISRCDIR

SOURCES += \
	ut_duilogicalvalues.cpp \
    ../../src/corelib/theme/duilogicalvalues.cpp \
    ../../src/corelib/style/duistylesheetattribute.cpp

HEADERS += \
	ut_duilogicalvalues.h \
	../../src/corelib/theme/duilogicalvalues.h \
	../../src/corelib/theme/duilogicalvalues_p.h \
	../../src/corelib/style/duistylesheetattribute.h

support_files.files += *.ini

include(../common_bot.pri)
