include(../common_top.pri)

TARGET = ut_duiservicefwgen

system(dui-servicefwgen -p com.nokia.SomeInterface)
system(dui-servicefwgen -a com.nokia.SomeInterface)

SFW_HEADERS += \
    someinterfaceadaptor.h \
    someinterface.h \
    someinterfaceproxy.h \

SFW_SOURCES += \
    someinterfaceadaptor.cpp \
    someinterface.cpp \
    someinterfaceproxy.cpp \

# Input
HEADERS += \
    ut_duiservicefwgen.h \
    $$SFW_HEADERS \

SOURCES += \
    ut_duiservicefwgen.cpp \
    $$SFW_SOURCES \

QMAKE_CLEAN += \
    $$SFW_HEADERS \
    $$SFW_SOURCES \

include(../common_bot.pri)
