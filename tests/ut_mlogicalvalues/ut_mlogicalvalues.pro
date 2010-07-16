include(../common_top.pri)
TARGET = ut_mlogicalvalues
QT += dbus svg network
LIBRARYPATH += $$MSRCDIR

SOURCES += \
	ut_mlogicalvalues.cpp \
    $$MSRCDIR/corelib/theme/mlogicalvalues.cpp \
    $$MSRCDIR/corelib/style/mstylesheetattribute.cpp

HEADERS += \
	ut_mlogicalvalues.h \
        $$MSRCDIR/corelib/theme/mlogicalvalues.h \
        $$MSRCDIR/corelib/theme/mlogicalvalues_p.h \
        $$MSRCDIR/corelib/style/mstylesheetattribute.h

support_files.files += *.ini

include(../common_bot.pri)
