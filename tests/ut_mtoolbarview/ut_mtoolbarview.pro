include(../common_top.pri)
TARGET = ut_mtoolbarview


INCLUDEPATH += $$MSRCDIR/corelib/widgets $$MSRCDIR/corelib/style $$MSRCDIR/views

SOURCES += \
    ut_mtoolbarview.cpp

HEADERS += \
    ut_mtoolbarview.h \
    $$MSRCDIR/views/mtoolbarview_p.h

support_files.files += ut_mtoolbarview.css

include(../common_bot.pri)
