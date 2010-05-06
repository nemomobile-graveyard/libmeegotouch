include(../common_top.pri)

SFWDIR = ../../mservicemapper
INCLUDEPATH += $$SFWDIR
DEPENDPATH += $$INCLUDEPATH
QT -= dbus svg network gui

TARGET = ut_mservicemapper

# unit
TEST_SOURCES =  \
    $$SFWDIR/mservicemapper.cpp \

# unit test and unit
SOURCES += \
    $$TEST_SOURCES \
    ut_mservicemapper.cpp \

# unit test and unit
HEADERS += \
    ut_mservicemapper.h \
    $$SFWDIR/mservicemapper.h \

include(../common_bot.pri)
