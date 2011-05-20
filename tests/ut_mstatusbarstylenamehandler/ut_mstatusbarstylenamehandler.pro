include(../common_top.pri)

INCLUDEPATH += $$MSRCDIR/corelib/widgets \
               $$MSRCDIR/corelib/events

TARGET = ut_mstatusbarstylenamehandler

SOURCES += \
    ut_mstatusbarstylenamehandler.cpp \
    $$MSRCDIR/corelib/widgets/mstatusbarstylenamehandler.cpp \
    $$MSRCDIR/corelib/events/mscenewindowevent_p.cpp

HEADERS += \
    ut_mstatusbarstylenamehandler.h \
    $$MSRCDIR/corelib/widgets/mstatusbarstylenamehandler.h \
    $$MSRCDIR/corelib/events/mscenewindowevent_p.h

include(../common_bot.pri)
