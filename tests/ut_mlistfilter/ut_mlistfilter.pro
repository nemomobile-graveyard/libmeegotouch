include(../common_top.pri)

TARGET = ut_mlistfilter

INCLUDEPATH += $${MSRCDIR}/corelib/widgets

HEADERS += ut_mlistfilter.h \
           $${MSRCDIR}/corelib/widgets/mlistfilter_p.h

SOURCES += ut_mlistfilter.cpp \
           $${MSRCDIR}/corelib/widgets/mlistfilter.cpp

include(../common_bot.pri )
