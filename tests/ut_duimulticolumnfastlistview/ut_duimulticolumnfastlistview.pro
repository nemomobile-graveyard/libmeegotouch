include(../common_top.pri)

TARGET = ut_duimulticolumnfastlistview

HEADERS += ut_duimulticolumnfastlistview.h myindexedmodel.h
SOURCES += ut_duimulticolumnfastlistview.cpp myindexedmodel.cpp ../../src/widgets/views/duifastlistview_p.cpp ../../src/.moc/moc_duifastlistview_p.cpp

include(../common_bot.pri)
