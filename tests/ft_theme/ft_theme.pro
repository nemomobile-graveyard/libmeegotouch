include(../common_top.pri)
TARGET = ft_theme
QT += dbus svg network
LIBRARYPATH += $$DUISRCDIR

SOURCES += \
    ft_theme.cpp \
    testthemedaemon.cpp \
    testview.cpp \
    testview2.cpp

HEADERS += \
    ft_theme.h \
    testthemedaemon.h \
    testview.h \
    testview2.h \
    $$DUISRCDIR/theme/iduithemedaemon.h

include(../common_bot.pri)
