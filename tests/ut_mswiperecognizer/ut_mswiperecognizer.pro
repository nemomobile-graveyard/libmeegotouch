include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/corelib/events
INCLUDEPATH += $$MSRCDIR/corelib/style
INCLUDEPATH += $$MSRCDIR/corelib/theme
TARGET = ut_mswiperecognizer

STYLE_HEADERS += \
    $$MSRCDIR/corelib/style/mstyle.h \
    $$MSRCDIR/corelib/style/mswiperecognizerstyle.h \

# unit test and unit classes
SOURCES += \
    $$MSRCDIR/corelib/style/mstyle.cpp \
    $$MSRCDIR/corelib/events/mswipegesture.cpp \
    $$MSRCDIR/corelib/events/mswiperecognizer.cpp \
    ut_mswiperecognizer.cpp \

# unit test and unit classes
HEADERS += \
    $$MSRCDIR/corelib/style/mstyle.h \
    $$MSRCDIR/corelib/style/mswiperecognizerstyle.h \
    $$MSRCDIR/corelib/events/mswipegesture_p.h \
    ut_mswiperecognizer.h \

include(../common_bot.pri)
