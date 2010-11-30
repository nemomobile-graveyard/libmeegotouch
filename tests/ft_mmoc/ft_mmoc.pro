include(../common_top.pri)

# no need to link against qtgui for this command line app test
CONFIG -= meegotouch
QT -= gui

TARGET = ft_mmoc

SOURCES += \
    ft_mmoc.cpp \

HEADERS += \
    ft_mmoc.h \

#QMAKE_CLEAN += \
#    ftmservicefwgen1adaptor.cpp \
#    ftmservicefwgen1adaptor.h \
#    ftmservicefwgen1.cpp \
#    ftmservicefwgen1.h \
#    ftmservicefwgen1proxy.cpp \
#    ftmservicefwgen1proxy.h \

support_files.path = $$[QT_INSTALL_LIBS]/libmeegotouch-tests/ft_mmoc-samples
support_files.files += ft_mmoc-samples/*

include(../common_bot.pri)
