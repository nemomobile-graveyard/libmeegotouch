include(../common_top.pri)
TARGET = ut_mgriditem

INCLUDEPATH += $$MSRCDIR/corelib/widgets \

SOURCES += ut_mgriditem.cpp \

HEADERS  += ut_mgriditem.h \
            $$MSRCDIR/corelib/widgets/mgriditem.h \

include(../common_bot.pri)
