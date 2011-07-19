include(../common_top.pri)

TARGET = ut_mlistfilter

INCLUDEPATH += $${M_SOURCE_TREE}/src/corelib/widgets

HEADERS += ut_mlistfilter.h \
           $${M_SOURCE_TREE}/src/corelib/widgets/mlistfilter_p.h

SOURCES += ut_mlistfilter.cpp \
           $${M_SOURCE_TREE}/src/corelib/widgets/mlistfilter.cpp

include(../common_bot.pri )
