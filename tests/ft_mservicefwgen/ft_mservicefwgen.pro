include(../common_top.pri)
LIBS =
QT -= gui

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

support_files.files += \
    com.nokia.FtMServiceFwGen1.xml \
    com.nokia.FtMServiceFwGen2.xml \
    com.nokia.FtMServiceFwGen3.xml \
    ftmservicefwgen1adaptor.cpp.correct \
    ftmservicefwgen1adaptor.h.correct \
    ftmservicefwgen1.cpp.correct \
    ftmservicefwgen1.h.correct \
    ftmservicefwgen1proxy.cpp.correct \
    ftmservicefwgen1proxy.h.correct \
    ftmservicefwgen2adaptor.cpp.correct \
    ftmservicefwgen2adaptor.h.correct \
    ftmservicefwgen2.cpp.correct \
    ftmservicefwgen2.h.correct \
    ftmservicefwgen2proxy.cpp.correct \
    ftmservicefwgen2proxy.h.correct \
    ftmservicefwgen3adaptor.cpp.correct \
    ftmservicefwgen3adaptor.h.correct \
    ftmservicefwgen3.cpp.correct \
    ftmservicefwgen3.h.correct \
    ftmservicefwgen3proxy.cpp.correct \
    ftmservicefwgen3proxy.h.correct \

include(../common_bot.pri)
