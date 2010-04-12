include(../common_top.pri)

TARGET = ut_mlistviewmulticolumn

INCLUDEPATH += ../mlistviewcommon
HEADERS += ut_mlistviewmulticolumn.h \
        ../mlistviewcommon/myindexedmodel.h

SOURCES += ut_mlistviewmulticolumn.cpp  \
        ../mlistviewcommon/myindexedmodel.cpp \
        ../../src/views/mlistview_p.cpp \
        ../../src/views/.moc/moc_mlistview_p.cpp

include(../common_bot.pri)
