include(../common_top.pri)

TARGET = ut_duifastlistviewgroupheader

INCLUDEPATH += ../duifastlistviewcommon
HEADERS += ut_duifastlistviewgroupheader.h \
        ../duifastlistviewcommon/myindexedmodel.h

SOURCES += ut_duifastlistviewgroupheader.cpp  \
        ../duifastlistviewcommon/myindexedmodel.cpp \
        ../../src/widgets/views/duifastlistview_p.cpp \
        ../../src/.moc/moc_duifastlistview_p.cpp

include(../common_bot.pri)
