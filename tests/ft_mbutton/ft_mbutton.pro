include(../common_top.pri)
TARGET = ft_mbutton
QT += dbus svg network
LIBRARYPATH += $$MSRCDIR

SOURCES += \
	ft_mbutton.cpp \

HEADERS += \
	ft_mbutton.h \

include(../common_bot.pri)
