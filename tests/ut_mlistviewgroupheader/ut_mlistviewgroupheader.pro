include(../common_top.pri)

TARGET = ut_mlistviewgroupheader

INCLUDEPATH += ../mlistviewcommon
HEADERS += ut_mlistviewgroupheader.h \
        ../mlistviewcommon/myindexedmodel.h

SOURCES += ut_mlistviewgroupheader.cpp  \
        ../mlistviewcommon/myindexedmodel.cpp \
        ../../src/views/mlistview_p.cpp \
        ../../src/views/.moc/moc_mlistview_p.cpp

include(../common_bot.pri)
