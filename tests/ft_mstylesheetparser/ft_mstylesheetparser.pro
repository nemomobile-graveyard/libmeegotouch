include(../common_top.pri)
TARGET = ft_mstylesheetparser
QT += dbus svg network
LIBRARYPATH += $$MSRCDIR

INCLUDEPATH += $$MSRCDIR/corelib/core/

SOURCES += \
	ft_mstylesheetparser.cpp \
    $$MSRCDIR/corelib/theme/mlogicalvalues.cpp \
    $$MSRCDIR/corelib/style/mstylesheetattribute.cpp


HEADERS += \
	ft_mstylesheetparser.h \
        $$MSRCDIR/corelib/theme/mlogicalvalues.h \
        $$MSRCDIR/corelib/style/mstylesheetattribute.h

support_files.files += \
        *.css \
        *.ini

include(../common_bot.pri)
