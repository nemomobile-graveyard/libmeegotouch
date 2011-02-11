include(../common_top.pri)
TARGET = ut_mtoolbarview


INCLUDEPATH += $$MSRCDIR/corelib/widgets $$MSRCDIR/corelib/style

SOURCES += \
    ut_mtoolbarview.cpp \

HEADERS += \
    ut_mtoolbarview.h

support_files.files += ut_mtoolbarview.css

include(../common_bot.pri)
