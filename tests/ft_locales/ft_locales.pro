include(../common_top.pri)

CONFIG += link_pkgconfig
TARGET = ft_locales
PKGCONFIG += gconf-2.0

# unit
TEST_SOURCES =  \
#    $$MSRCDIR/mlocale.cpp \ 

# Input
HEADERS += ft_locales.h
SOURCES += ft_locales.cpp

support_files.files += \
    *.txt

include(../common_bot.pri)
