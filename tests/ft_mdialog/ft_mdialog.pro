include(../common_top.pri)
TARGET = ft_mdialog
QT += dbus svg network
LIBRARYPATH += $$MSRCDIR

SOURCES += \
	ft_mdialog.cpp \

HEADERS += \
	ft_mdialog.h \

include(../common_bot.pri)
