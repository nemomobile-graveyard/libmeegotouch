include(../common_top.pri)

TARGET = ut_mbannerview

INCLUDEPATH += $$MSRCDIR/corelib/widgets \
               $$MSRCDIR/views \

SOURCES += \
    ut_mbannerview.cpp \
    $$MSRCDIR/views/mbannerview.cpp \
    $$MSRCDIR/views/mscenewindowview.cpp \



HEADERS += \
    ut_mbannerview.h \
    $$MSRCDIR/views/mbannerview.h \
    $$MSRCDIR/views/mbannerview_p.h \
    $$MSRCDIR/views/mscenewindowview.h \

include(../common_bot.pri)
