include(../common_top.pri)

TARGET = ut_mdetailedlistitem

INCLUDEPATH += $$MSRCDIR/corelib/widgets

HEADERS += ut_mdetailedlistitem.h \
           $$MSRCDIR/corelib/widgets/mdetailedlistitem_p.h

SOURCES += ut_mdetailedlistitem.cpp \
            $$MSRCDIR/corelib/widgets/mdetailedlistitem.cpp

include(../common_bot.pri)

