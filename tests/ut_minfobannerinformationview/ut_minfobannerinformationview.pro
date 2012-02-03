include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/corelib/widgets $$MSRCDIR/views
TARGET = ut_minfobannerinformationview

# unit

# unit test and unit
SOURCES += \
    ut_minfobannerinformationview.cpp \

# unit test and unit
HEADERS += \
    ut_minfobannerinformationview.h \
    $$MSRCDIR/views/minfobannerinformationview_p.h


include(../common_bot.pri)
