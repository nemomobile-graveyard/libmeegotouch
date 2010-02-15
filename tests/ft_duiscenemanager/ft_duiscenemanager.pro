include(../common_top.pri)
TARGET = ft_duiscenemanager
QT += dbus svg network
LIBRARYPATH += $$DUISRCDIR

SOURCES += \
	ft_duiscenemanager.cpp \

HEADERS += \
	ft_duiscenemanager.h \

include(../common_bot.pri)
