include(../common_top.pri)
TARGET = ft_duidialog
QT += dbus svg network
LIBRARYPATH += $$DUISRCDIR

SOURCES += \
	ft_duidialog.cpp \

HEADERS += \
	ft_duidialog.h \

include(../common_bot.pri)
