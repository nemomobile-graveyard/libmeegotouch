include(../common_top.pri)

TARGET = ut_mlistviewplain

INCLUDEPATH += ../ut_mlistviewcommon
HEADERS += ut_mlistviewplain.h

SOURCES += ut_mlistviewplain.cpp  \
        $$MSRCDIR/corelib/widgets/views/mlistview_p.cpp \
        $$MSRCDIR/corelib/widgets/mcell.cpp \
        ../../src/corelib/.moc/moc_mlistview_p.cpp

include(../common_bot.pri)
