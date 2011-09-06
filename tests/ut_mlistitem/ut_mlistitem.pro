include(../common_top.pri)
TARGET = ut_mlistitem

INCLUDEPATH += $$MSRCDIR/corelib/widgets \

SOURCES += ut_mlistitem.cpp \

HEADERS  += ut_mlistitem.h \
            $$MSRCDIR/corelib/widgets/mlistitem.h \

include(../common_bot.pri)
