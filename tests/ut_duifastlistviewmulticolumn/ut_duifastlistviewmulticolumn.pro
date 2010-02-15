include(../common_top.pri)

TARGET = ut_duifastlistviewmulticolumn

INCLUDEPATH += ../duifastlistviewcommon
HEADERS += ut_duifastlistviewmulticolumn.h \
        ../duifastlistviewcommon/myindexedmodel.h

SOURCES += ut_duifastlistviewmulticolumn.cpp  \
        ../duifastlistviewcommon/myindexedmodel.cpp \
        ../../src/widgets/views/duifastlistview_p.cpp \
        ../../src/.moc/moc_duifastlistview_p.cpp

include(../common_bot.pri)
