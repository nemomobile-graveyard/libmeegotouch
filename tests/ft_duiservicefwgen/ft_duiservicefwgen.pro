include(../common_top.pri)
LIBS =
QT -= gui

TARGET = ft_duiservicefwgen

SOURCES += \
    ft_duiservicefwgen.cpp \

HEADERS += \
    ft_duiservicefwgen.h \

QMAKE_CLEAN += \
    ftduiservicefwgen1adaptor.cpp \
    ftduiservicefwgen1adaptor.h \
    ftduiservicefwgen1.cpp \
    ftduiservicefwgen1.h \
    ftduiservicefwgen1proxy.cpp \
    ftduiservicefwgen1proxy.h \
    ftduiservicefwgen2adaptor.cpp \
    ftduiservicefwgen2adaptor.h \
    ftduiservicefwgen2.cpp \
    ftduiservicefwgen2.h \
    ftduiservicefwgen2proxy.cpp \
    ftduiservicefwgen2proxy.h \
    ftduiservicefwgen3adaptor.cpp \
    ftduiservicefwgen3adaptor.h \
    ftduiservicefwgen3.cpp \
    ftduiservicefwgen3.h \
    ftduiservicefwgen3proxy.cpp \
    ftduiservicefwgen3proxy.h \

support_files.files += \
    com.nokia.FtDuiServiceFwGen1.xml \
    com.nokia.FtDuiServiceFwGen2.xml \
    com.nokia.FtDuiServiceFwGen3.xml \
    ftduiservicefwgen1adaptor.cpp.correct \
    ftduiservicefwgen1adaptor.h.correct \
    ftduiservicefwgen1.cpp.correct \
    ftduiservicefwgen1.h.correct \
    ftduiservicefwgen1proxy.cpp.correct \
    ftduiservicefwgen1proxy.h.correct \
    ftduiservicefwgen2adaptor.cpp.correct \
    ftduiservicefwgen2adaptor.h.correct \
    ftduiservicefwgen2.cpp.correct \
    ftduiservicefwgen2.h.correct \
    ftduiservicefwgen2proxy.cpp.correct \
    ftduiservicefwgen2proxy.h.correct \
    ftduiservicefwgen3adaptor.cpp.correct \
    ftduiservicefwgen3adaptor.h.correct \
    ftduiservicefwgen3.cpp.correct \
    ftduiservicefwgen3.h.correct \
    ftduiservicefwgen3proxy.cpp.correct \
    ftduiservicefwgen3proxy.h.correct \

include(../common_bot.pri)
