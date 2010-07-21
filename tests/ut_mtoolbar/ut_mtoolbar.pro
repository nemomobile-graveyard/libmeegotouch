include(../common_top.pri)
TARGET = ut_mtoolbar


INCLUDEPATH += $$MSRCDIR/corelib/widgets $$MSRCDIR/corelib/style

SOURCES += \
    ut_mtoolbar.cpp \
    $$MSRCDIR/corelib/widgets/mtoolbar.cpp \
    $$MSRCDIR/corelib/widgets/core/mwidgetcontroller.cpp \
    $$MSRCDIR/corelib/widgets/core/mwidget.cpp \

HEADERS += \
    ut_mtoolbar.h \
    $$MSRCDIR/corelib/widgets/mtoolbar_p.h \
    $$MSRCDIR/corelib/widgets/core/mwidgetcontroller_p.h \

include(../common_bot.pri)
