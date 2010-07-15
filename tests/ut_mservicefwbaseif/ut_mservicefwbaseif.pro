include(../common_top.pri)

# need to copy the units here in order to avoid force the inclusion of the local mservicefwproxy.h
# which contains the stubbed proxy that does not rely on qdbus
SFWDIR = $$MSRCDIR/corelib/servicefwif
TEST_SOURCE = mservicefwbaseif.cpp
TEST_SRC_FROM = $$SFWDIR/$$TEST_SOURCE
TEST_HEADER = mservicefwbaseif.h
TEST_HDR_FROM = $$SFWDIR/$$TEST_HEADER

QMAKE_CLEAN += $$TEST_HEADER $$TEST_SOURCE

INCLUDEPATH = \
    . \
    $$MSRCDIR/include \
    $$SFWDIR/include \

DEPENDPATH = $$INCLUDEPATH

TARGET = ut_mservicefwbaseif

SOURCES += \
    ut_mservicefwbaseif.cpp \
    mservicefwproxy.cpp \
    $$SFWDIR/$$TEST_SOURCE \

HEADERS += \
    ut_mservicefwbaseif.h \
    mservicefwproxy.h \
    $$SFWDIR/$$TEST_HEADER \

include(../common_bot.pri)
