
include(../common_top.pri)
INCLUDEPATH += \
    $$MSRCDIR/corelib/widgets \
    $$MSRCDIR/corelib/style \
    $$MSRCDIR/views \

TARGET = ut_mescapebuttonpanelview

SOURCES += \
    ut_mescapebuttonpanelview.cpp \

HEADERS += \
    ut_mescapebuttonpanelview.h

support_files.files += \
    ut_mescapebuttonpanelview.css \

include(../common_bot.pri)
