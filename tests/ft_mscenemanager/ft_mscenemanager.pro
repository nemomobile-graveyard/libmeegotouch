include(../common_top.pri)
TARGET = ft_mscenemanager
QT += dbus svg network
LIBRARYPATH += $$MSRCDIR

SOURCES += \
	ft_mscenemanager.cpp \

HEADERS += \
	ft_mscenemanager.h \

include(../common_bot.pri)
