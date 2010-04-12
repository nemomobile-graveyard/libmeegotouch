include(../common_top.pri)
TARGET = ft_mscalableimage
QT += dbus svg network
LIBRARYPATH += $$MSRCDIR

SOURCES += \
	ft_mscalableimage.cpp \

HEADERS += \
	ft_mscalableimage.h \

QMAKE_CLEAN += test2.png

include(../common_bot.pri)
