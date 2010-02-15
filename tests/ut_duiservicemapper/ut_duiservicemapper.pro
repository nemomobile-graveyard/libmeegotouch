include(../common_top.pri)

SFWDIR = ../../duiservicemapper
INCLUDEPATH += $$SFWDIR
DEPENDPATH += $$INCLUDEPATH
QT -= dbus svg network gui

TARGET = ut_duiservicemapper

# unit
TEST_SOURCES =  \
    $$SFWDIR/duiservicemapper.cpp \

# unit test and unit
SOURCES += \
    $$TEST_SOURCES \
    ut_duiservicemapper.cpp \

# unit test and unit
HEADERS += \
    ut_duiservicemapper.h \
    $$SFWDIR/duiservicemapper.h \

include(../common_bot.pri)
