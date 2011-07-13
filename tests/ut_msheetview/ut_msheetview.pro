
include(../common_top.pri)

TARGET = ut_msheetview

SOURCES += \
    ut_msheetview.cpp \
    $$MSRCDIR/views/msheetview.cpp \
    $$MSRCDIR/views/mscenewindowview.cpp \
    $$MSRCDIR/views/mslotlayout.cpp \

HEADERS += \
    ut_msheetview.h \
    $$MSRCDIR/views/msheetview.h \
    $$MSRCDIR/views/msheetview_p.h \
    $$MSRCDIR/views/mscenewindowview.h \
    $$MSRCDIR/views/mslotlayout.h \

INCLUDEPATH += \
    $$MSRCDIR/corelib/widgets \
    $$MSRCDIR/corelib/style \
    $$MSRCDIR/views \

include(../common_bot.pri)
