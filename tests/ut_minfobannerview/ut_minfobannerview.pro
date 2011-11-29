include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/corelib/widgets $$MSRCDIR/views
TARGET = ut_minfobannerview

# unit

# unit test and unit
SOURCES += \
    ut_minfobannerview.cpp \

# unit test and unit
HEADERS += \
    ut_minfobannerview.h \
    $$MSRCDIR/views/minfobannereventview_p.h


include(../common_bot.pri)
