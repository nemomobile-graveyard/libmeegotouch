include(../common_top.pri)

SOURCES += ft_duivideowidget.cpp \
#$$DUISRCDIR/video/duivideowidget.cpp \
#$$DUISRCDIR/video/duisink.c

HEADERS += ft_duivideowidget.h \
$$DUISRCDIR/video/duivideowidget.h \
$$DUISRCDIR/video/duisink.h

QT += testlib

CONFIG += link_pkgconfig
PKGCONFIG += gstreamer-0.10
INCLUDEPATH += $$DUISRCDIR/video

include(../common_bot.pri)
