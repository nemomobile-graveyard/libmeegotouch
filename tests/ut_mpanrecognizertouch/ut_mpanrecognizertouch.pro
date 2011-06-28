include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/corelib/events
INCLUDEPATH += $$MSRCDIR/corelib/style
INCLUDEPATH += $$MSRCDIR/corelib/theme
TARGET = ut_mpanrecognizertouch

STYLE_HEADERS += \
    $$MSRCDIR/corelib/style/mstyle.h \
    $$MSRCDIR/corelib/style/mpanrecognizerstyle.h \

# unit test and unit classes
SOURCES += \
    $$MSRCDIR/corelib/style/mstyle.cpp \
    $$MSRCDIR/corelib/events/mpangesturetouch.cpp \
    $$MSRCDIR/corelib/events/mpanrecognizertouch.cpp \
    ut_mpanrecognizertouch.cpp \

# unit test and unit classes
HEADERS += \
    $$MSRCDIR/corelib/style/mstyle.h \
    $$MSRCDIR/corelib/style/mpanrecognizerstyle.h \
    $$MSRCDIR/corelib/events/mpangesturetouch.h \
    $$MSRCDIR/corelib/events/mpanrecognizertouch.h \
    ut_mpanrecognizertouch.h \

include(../common_bot.pri)
