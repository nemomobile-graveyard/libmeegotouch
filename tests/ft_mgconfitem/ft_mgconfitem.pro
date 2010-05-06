include(../common_top.pri)

CONFIG += link_pkgconfig
TARGET = ft_mgconfitem_exec
PKGCONFIG += gconf-2.0

# Input
HEADERS += ft_mgconfitem_exec.h
SOURCES += ft_mgconfitem_exec.cpp

support_files.files += \
    ft_mgconfitem \

include(../common_bot.pri)
