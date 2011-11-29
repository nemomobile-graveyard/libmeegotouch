include(../common_top.pri)

TARGET = ut_mcolorlistview

INCLUDEPATH += $$MSRCDIR/corelib/widgets \
               $$MSRCDIR/corelib/style \
               $$MSRCDIR/views \
               $$MSRCDIR/views/widgets

SOURCES += ut_mcolorlistview.cpp
HEADERS += ut_mcolorlistview.h

include(../common_bot.pri)
