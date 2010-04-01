include(../common_top.pri)

# need to copy the units here in order to avoid force the inclusion of the local duiservicefwproxy.h
# which contains the stubbed proxy that does not rely on qdbus
# see further down for a failed attempt to get this to work some other way
SFWDIR = $$DUISRCDIR/corelib/servicefwif
TEST_SOURCE = duiservicefwbaseif.cpp
TEST_SRC_FROM = $$SFWDIR/$$TEST_SOURCE
TEST_HEADER = duiservicefwbaseif.h
TEST_HDR_FROM = $$SFWDIR/$$TEST_HEADER

QMAKE_CLEAN += $$TEST_HEADER $$TEST_SOURCE

INCLUDEPATH = \
    . \
    $$DUISRCDIR/include \
    $$SFWDIR/include \

DEPENDPATH = $$INCLUDEPATH

TARGET = ut_duiservicefwbaseif

SOURCES += \
    ut_duiservicefwbaseif.cpp \
    duiservicefwproxy.cpp \
    $$SFWDIR/$$TEST_SOURCE \

HEADERS += \
    ut_duiservicefwbaseif.h \
    duiservicefwproxy.h \
    $$SFWDIR/$$TEST_HEADER \

include(../common_bot.pri)
