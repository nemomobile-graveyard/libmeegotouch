include(../common_top.pri)
TARGET = ut_duilogicalvalues
QT += dbus svg network
LIBRARYPATH += $$DUISRCDIR

SOURCES += \
	ut_duilogicalvalues.cpp \
    ../../src/theme/duilogicalvalues.cpp \
    ../../src/style/duistylesheetattribute.cpp

HEADERS += \
	ut_duilogicalvalues.h \
	../../src/theme/duilogicalvalues.h \
	../../src/theme/duilogicalvalues_p.h \
	../../src/style/duistylesheetattribute.h

support_files.files += *.ini

include(../common_bot.pri)
