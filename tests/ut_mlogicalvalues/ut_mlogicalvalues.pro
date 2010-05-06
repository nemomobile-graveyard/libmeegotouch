include(../common_top.pri)
TARGET = ut_mlogicalvalues
QT += dbus svg network
LIBRARYPATH += $$MSRCDIR

SOURCES += \
	ut_mlogicalvalues.cpp \
    ../../src/corelib/theme/mlogicalvalues.cpp \
    ../../src/corelib/style/mstylesheetattribute.cpp

HEADERS += \
	ut_mlogicalvalues.h \
	../../src/corelib/theme/mlogicalvalues.h \
	../../src/corelib/theme/mlogicalvalues_p.h \
	../../src/corelib/style/mstylesheetattribute.h

support_files.files += *.ini

include(../common_bot.pri)
