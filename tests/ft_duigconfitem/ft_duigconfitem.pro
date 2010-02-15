include(../common_top.pri)

CONFIG += link_pkgconfig
TARGET = ft_duigconfitem_exec
PKGCONFIG += gconf-2.0

# Input
HEADERS += ft_duigconfitem_exec.h
SOURCES += ft_duigconfitem_exec.cpp

support_files.files += \
    ft_duigconfitem \

include(../common_bot.pri)
