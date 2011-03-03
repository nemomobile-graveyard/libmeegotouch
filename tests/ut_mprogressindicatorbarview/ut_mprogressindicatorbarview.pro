
include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/corelib/widgets $$MSRCDIR/corelib/style

TARGET = ut_mprogressindicatorbarview

SOURCES += \
    ut_mprogressindicatorbarview.cpp \
    $$MSRCDIR/views/mprogressindicatorbarview.cpp \

HEADERS += \
    ut_mprogressindicatorbarview.h \
    $$MSRCDIR/views/mprogressindicatorbarview.h \
    $$MSRCDIR/views/mprogressindicatorbarview_p.h \

include(../common_bot.pri)
