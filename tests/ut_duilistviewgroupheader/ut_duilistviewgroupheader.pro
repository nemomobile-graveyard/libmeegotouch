include(../common_top.pri)

TARGET = ut_duilistviewgroupheader

INCLUDEPATH += ../duilistviewcommon
HEADERS += ut_duilistviewgroupheader.h \
        ../duilistviewcommon/myindexedmodel.h

SOURCES += ut_duilistviewgroupheader.cpp  \
        ../duilistviewcommon/myindexedmodel.cpp \
        ../../src/widgets/views/duilistview_p.cpp \
        ../../src/.moc/moc_duilistview_p.cpp

include(../common_bot.pri)
