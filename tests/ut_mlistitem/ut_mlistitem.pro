include(../common_top.pri)
TARGET = ut_mlistitem

INCLUDEPATH += $$MSRCDIR/corelib/widgets \
               $$MSRCDIR/views \

SOURCES += ut_mlistitem.cpp \
           $$MSRCDIR/views/mlistitemview.cpp \

HEADERS  += ut_mlistitem.h \
            $$MSRCDIR/corelib/widgets/mlistitem.h \
            $$MSRCDIR/views/mlistitemview.h \
            $$MSRCDIR/views/mlistitemview_p.h \
            $$MSRCDIR/corelib/widgets/mtapstatemachine.h \
            $$MSRCDIR/corelib/widgets/mtapstatemachine_p.h \

include(../common_bot.pri)
