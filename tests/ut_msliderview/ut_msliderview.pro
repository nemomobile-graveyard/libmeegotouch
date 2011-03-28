
include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/corelib/widgets $$MSRCDIR/corelib/style

TARGET = ut_msliderview

SOURCES += \
    ut_msliderview.cpp \
    $$MSRCDIR/views/msliderview.cpp

HEADERS += \
    ut_msliderview.h \
    $$MSRCDIR/views/msliderview.h \
    $$MSRCDIR/views/msliderview_p.h

include(../common_bot.pri)
