include(../common_top.pri)
TARGET = ut_mlistitem

INCLUDEPATH += $$MSRCDIR/corelib/widgets \
               $$MSRCDIR/views \

SOURCES += ut_mlistitem.cpp \

HEADERS  += ut_mlistitem.h \
            $$MSRCDIR/corelib/widgets/mlistitem.h \
            $$MSRCDIR/corelib/widgets/mlistitemview.h \
            $$MSRCDIR/corelib/widgets/mlistitemview_p.h \
            $$MSRCDIR/corelib/widgets/mtapstatemachine.h \
            $$MSRCDIR/corelib/widgets/mtapstatemachine_p.h \

include(../common_bot.pri)
