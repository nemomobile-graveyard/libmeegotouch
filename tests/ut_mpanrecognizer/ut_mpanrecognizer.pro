include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/corelib/events
INCLUDEPATH += $$MSRCDIR/corelib/style
INCLUDEPATH += $$MSRCDIR/corelib/theme
TARGET = ut_mpanrecognizer

STYLE_HEADERS += \
    $$MSRCDIR/corelib/style/mstyle.h \
    $$MSRCDIR/corelib/style/mpanrecognizerstyle.h \

# unit test and unit classes
SOURCES += \
    $$MSRCDIR/corelib/style/mstyle.cpp \
    $$MSRCDIR/corelib/events/mpangesture.cpp \
    $$MSRCDIR/corelib/events/mpanrecognizer.cpp \
    ut_mpanrecognizer.cpp \
    
# unit test and unit classes
HEADERS += \
    $$MSRCDIR/corelib/style/mstyle.h \
    $$MSRCDIR/corelib/style/mpanrecognizerstyle.h \
    $$MSRCDIR/corelib/events/mpangesture_p.h \
    ut_mpanrecognizer.h \

include(../common_bot.pri)
