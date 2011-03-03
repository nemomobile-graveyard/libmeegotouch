
include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/corelib/widgets $$MSRCDIR/corelib/style

TARGET = ut_mprogressindicatorspinnerview

SOURCES += \
    ut_mprogressindicatorspinnerview.cpp \
    $$MSRCDIR/views/mspinnerview.cpp \

HEADERS += \
    ut_mprogressindicatorspinnerview.h \
    $$MSRCDIR/views/mspinnerview.h \
    $$MSRCDIR/views/mspinnerview_p.h \

include(../common_bot.pri)
