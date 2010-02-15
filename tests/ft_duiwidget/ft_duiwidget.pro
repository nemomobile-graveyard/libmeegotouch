include(../common_top.pri)
TARGET = ft_duiwidget
QT += dbus svg network
LIBRARYPATH += $$DUISRCDIR

SOURCES += \
	ft_duiwidget.cpp \

HEADERS += \
	ft_duiwidget.h \

include(../common_bot.pri)
