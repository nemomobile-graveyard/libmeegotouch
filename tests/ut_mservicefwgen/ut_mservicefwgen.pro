include(../common_top.pri)

TARGET = ut_mservicefwgen

MSERVICEFWGEN_EXECUTABLE = $$M_BUILD_TREE/tools/m-servicefwgen
system($$MSERVICEFWGEN_EXECUTABLE -p com.nokia.SomeInterface)
system($$MSERVICEFWGEN_EXECUTABLE -a com.nokia.SomeInterface)

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
    ut_mservicefwgen.h \
    $$SFW_HEADERS \

SOURCES += \
    ut_mservicefwgen.cpp \
    $$SFW_SOURCES \

QMAKE_CLEAN += \
    $$SFW_HEADERS \
    $$SFW_SOURCES \

include(../common_bot.pri)
