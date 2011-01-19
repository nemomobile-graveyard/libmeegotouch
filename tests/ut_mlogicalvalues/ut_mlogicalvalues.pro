include(../common_top.pri)
TARGET = ut_mlogicalvalues
QT += dbus svg network
LIBRARYPATH += $$MSRCDIR

INCLUDEPATH += $$MSRCDIR/corelib/core/

SOURCES += \
	ut_mlogicalvalues.cpp \
    $$MSRCDIR/corelib/theme/mlogicalvalues.cpp \
    $$MSRCDIR/corelib/style/mstylesheetattribute.cpp \
    $$MSRCDIR/corelib/core/msystemdirectories.cpp \
    $$MSRCDIR/corelib/style/muniquestringcache.cpp \

HEADERS += \
	ut_mlogicalvalues.h \
        $$MSRCDIR/corelib/theme/mlogicalvalues.h \
        $$MSRCDIR/corelib/theme/mlogicalvalues_p.h \
        $$MSRCDIR/corelib/style/mstylesheetattribute.h \
        $$MSRCDIR/corelib/core/msystemdirectories.h \
        $$MSRCDIR/corelib/style/muniquestringcache.h \

support_files.files += *.ini

include(../common_bot.pri)
