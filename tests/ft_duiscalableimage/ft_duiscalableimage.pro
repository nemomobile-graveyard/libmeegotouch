include(../common_top.pri)
TARGET = ft_duiscalableimage
QT += dbus svg network
LIBRARYPATH += $$DUISRCDIR

SOURCES += \
	ft_duiscalableimage.cpp \

HEADERS += \
	ft_duiscalableimage.h \

QMAKE_CLEAN += test2.png

include(../common_bot.pri)
