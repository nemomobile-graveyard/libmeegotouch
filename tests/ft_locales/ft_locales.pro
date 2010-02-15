include(../common_top.pri)

CONFIG += link_pkgconfig
TARGET = ft_locales
PKGCONFIG += gconf-2.0

# unit
TEST_SOURCES =  \
#    $$DUISRCDIR/duilocale.cpp \ 

# Input
HEADERS += ft_locales.h
SOURCES += ft_locales.cpp

include(../common_bot.pri)
