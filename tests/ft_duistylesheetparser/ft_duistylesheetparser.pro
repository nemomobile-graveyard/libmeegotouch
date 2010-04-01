include(../common_top.pri)
TARGET = ft_duistylesheetparser
QT += dbus svg network
LIBRARYPATH += $$DUISRCDIR

SOURCES += \
	ft_duistylesheetparser.cpp \
    ../../src/corelib/theme/duilogicalvalues.cpp \
    ../../src/corelib/style/duistylesheetattribute.cpp


HEADERS += \
	ft_duistylesheetparser.h \
	../../src/corelib/theme/duilogicalvalues.h \
	../../src/corelib/style/duistylesheetattribute.h

support_files.files += \
        *.css \
        *.ini

include(../common_bot.pri)
