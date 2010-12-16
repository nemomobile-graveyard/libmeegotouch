include(../common_top.pri)

CONFIG += link_pkgconfig
TARGET = ft_mcharsetdetector
PKGCONFIG += gconf-2.0

# unit
TEST_SOURCES =  \
#    $$MSRCDIR/mlocale.cpp \

# Input
HEADERS += ft_mcharsetdetector.h
SOURCES += ft_mcharsetdetector.cpp

support_files.files += \
    *.txt

include(../common_bot.pri)
