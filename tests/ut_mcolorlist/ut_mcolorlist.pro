include(../common_top.pri)

TARGET = ut_mcolorlist

INCLUDEPATH += $$MSRCDIR/corelib/widgets \
               $$MSRCDIR/views/ \
               $$MSRCDIR/views/widgets \

SOURCES += \
    ut_mcolorlist.cpp \

HEADERS += \
    ut_mcolorlist.h

include(../common_bot.pri)
