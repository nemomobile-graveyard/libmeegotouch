include(../common_top.pri)
TARGET = ft_mwidget
QT += dbus svg network
LIBRARYPATH += $$MSRCDIR

SOURCES += \
	ft_mwidget.cpp \

HEADERS += \
	ft_mwidget.h \

include(../common_bot.pri)
