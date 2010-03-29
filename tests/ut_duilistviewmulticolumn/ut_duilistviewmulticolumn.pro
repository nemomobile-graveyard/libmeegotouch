include(../common_top.pri)

TARGET = ut_duilistviewmulticolumn

INCLUDEPATH += ../duilistviewcommon
HEADERS += ut_duilistviewmulticolumn.h \
        ../duilistviewcommon/myindexedmodel.h

SOURCES += ut_duilistviewmulticolumn.cpp  \
        ../duilistviewcommon/myindexedmodel.cpp \
        ../../src/widgets/views/duilistview_p.cpp \
        ../../src/.moc/moc_duilistview_p.cpp

include(../common_bot.pri)
