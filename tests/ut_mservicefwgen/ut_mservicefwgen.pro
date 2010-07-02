include(../common_top.pri)

TARGET = ut_mservicefwgen

MSERVICEFWGEN_EXECUTABLE = $$M_BUILD_TREE/tools/m-servicefwgen
system($$MSERVICEFWGEN_EXECUTABLE -p com.nokia.SomeInterface)
system($$MSERVICEFWGEN_EXECUTABLE -a com.nokia.SomeInterface)
system($$MSERVICEFWGEN_EXECUTABLE -p com.nokia.SomeInterface2)
system($$MSERVICEFWGEN_EXECUTABLE -a com.nokia.SomeInterface2)

SFW_HEADERS += \
    someinterfaceadaptor.h \
    someinterface.h \
    someinterfaceproxy.h \
    someinterface2adaptor.h \
    someinterface2.h \
    someinterface2proxy.h \

SFW_SOURCES += \
    someinterfaceadaptor.cpp \
    someinterface.cpp \
    someinterfaceproxy.cpp \
    someinterface2adaptor.cpp \
    someinterface2.cpp \
    someinterface2proxy.cpp \

# Input
HEADERS += \
    ut_mservicefwgen.h \
    $$SFW_HEADERS \

SOURCES += \
    ut_mservicefwgen.cpp \
    $$SFW_SOURCES \

QMAKE_CLEAN += \
    $$SFW_HEADERS \
    $$SFW_SOURCES \

include(../common_bot.pri)
