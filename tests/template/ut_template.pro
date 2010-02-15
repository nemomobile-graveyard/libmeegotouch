include(../common_top.pri)
TARGET = ut_mkutclassname

SOURCES += \
    ut_mkutclassname.cpp \
    $$DUISRCDIR/mkutclassname.cpp

HEADERS += \
    ut_mkutclassname.h \
    $$DUISRCDIR/mkutclassname.h

include(../common_bot.pri)
