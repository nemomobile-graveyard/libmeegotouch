include(../common_top.pri)

TARGET = ut_duimulticolumnlistview

HEADERS += ut_duimulticolumnlistview.h myindexedmodel.h
SOURCES += ut_duimulticolumnlistview.cpp myindexedmodel.cpp ../../src/corelib/widgets/views/duilistview_p.cpp ../../src/corelib/.moc/moc_duilistview_p.cpp

include(../common_bot.pri)
