include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/corelib/events
INCLUDEPATH += $$MSRCDIR/corelib/style
INCLUDEPATH += $$MSRCDIR/corelib/theme
TARGET = ut_mtapandholdrecognizer

STYLE_HEADERS += \
    $$MSRCDIR/corelib/style/mstyle.h \
    $$MSRCDIR/corelib/style/mtapandholdrecognizerstyle.h \

# unit test and unit classes
SOURCES += \
    $$MSRCDIR/corelib/style/mstyle.cpp \
    $$MSRCDIR/corelib/events/mtapandholdgesture.cpp \
    $$MSRCDIR/corelib/events/mtapandholdrecognizer.cpp \
    ut_mtapandholdrecognizer.cpp \

# unit test and unit classes
HEADERS += \
    $$MSRCDIR/corelib/style/mstyle.h \
    $$MSRCDIR/corelib/style/mtapandholdrecognizerstyle.h \
    $$MSRCDIR/corelib/events/mtapandholdgesture_p.h \
    ut_mtapandholdrecognizer.h \

include(../common_bot.pri)
