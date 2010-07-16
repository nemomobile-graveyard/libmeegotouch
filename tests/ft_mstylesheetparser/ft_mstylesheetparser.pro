include(../common_top.pri)
TARGET = ft_mstylesheetparser
QT += dbus svg network
LIBRARYPATH += $$MSRCDIR

SOURCES += \
	ft_mstylesheetparser.cpp \
    ../../src/corelib/theme/mlogicalvalues.cpp \
    ../../src/corelib/style/mstylesheetattribute.cpp


HEADERS += \
	ft_mstylesheetparser.h \
	../../src/corelib/theme/mlogicalvalues.h \
	../../src/corelib/style/mstylesheetattribute.h

support_files.files += \
        *.css \
        *.ini

include(../common_bot.pri)
