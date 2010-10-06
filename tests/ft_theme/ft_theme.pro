include(../common_top.pri)
TARGET = ft_theme
QT += dbus svg network
LIBRARYPATH += $$MSRCDIR

SOURCES += \
    ft_theme.cpp \
    testthemedaemon.cpp \
    testview.cpp \
    testview2.cpp \
    $$MSRCDIR/corelib/theme/mpixmaphandle.cpp \

HEADERS += \
    ft_theme.h \
    testthemedaemon.h \
    testview.h \
    testview2.h \
    $$MSRCDIR/corelib/theme/imthemedaemon.h \
    $$MSRCDIR/corelib/theme/mpixmaphandle.cpp \

include(../common_bot.pri)
