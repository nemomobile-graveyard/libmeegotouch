include(../common_top.pri)
TARGET = ft_duibutton
QT += dbus svg network
LIBRARYPATH += $$DUISRCDIR

SOURCES += \
	ft_duibutton.cpp \

HEADERS += \
	ft_duibutton.h \

include(../common_bot.pri)
