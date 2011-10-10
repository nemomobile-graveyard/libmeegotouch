include(../common_top.pri)

CONFIG += link_pkgconfig
TARGET = ft_mstringsearch
PKGCONFIG += gconf-2.0

# unit
TEST_SOURCES =  \
#    $$MSRCDIR/mlocale.cpp \

# Input
HEADERS += ft_mstringsearch.h
SOURCES += ft_mstringsearch.cpp

#support_files.files += \
#    *.txt

include(../common_bot.pri)
