include(../common_top.pri)
TARGET = ft_duistylesheetparser
QT += dbus svg network
LIBRARYPATH += $$DUISRCDIR

SOURCES += \
	ft_duistylesheetparser.cpp \
    ../../src/theme/duilogicalvalues.cpp \
    ../../src/style/duistylesheetattribute.cpp


HEADERS += \
	ft_duistylesheetparser.h \
	../../src/theme/duilogicalvalues.h \
	../../src/style/duistylesheetattribute.h

support_files.files += \
        *.css \
        *.ini

include(../common_bot.pri)
