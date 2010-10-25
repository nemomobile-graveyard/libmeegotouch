include(../common_top.pri)
LIBS -= $$mAddLibrary(meegotouchcore) \
        $$mAddLibrary(meegotouchviews)
QT -= gui

RESOURCES = \
    correctFiles.qrc \
    xmlFiles.qrc \

TARGET = ft_mservicefwgen

SOURCES += \
    ft_mservicefwgen.cpp \

HEADERS += \
    ft_mservicefwgen.h \

QMAKE_CLEAN += \
    ftmservicefwgen1adaptor.cpp \
    ftmservicefwgen1adaptor.h \
    ftmservicefwgen1.cpp \
    ftmservicefwgen1.h \
    ftmservicefwgen1proxy.cpp \
    ftmservicefwgen1proxy.h \
    ftmservicefwgen2adaptor.cpp \
    ftmservicefwgen2adaptor.h \
    ftmservicefwgen2.cpp \
    ftmservicefwgen2.h \
    ftmservicefwgen2proxy.cpp \
    ftmservicefwgen2proxy.h \
    ftmservicefwgen3adaptor.cpp \
    ftmservicefwgen3adaptor.h \
    ftmservicefwgen3.cpp \
    ftmservicefwgen3.h \
    ftmservicefwgen3proxy.cpp \
    ftmservicefwgen3proxy.h \

include(../common_bot.pri)
