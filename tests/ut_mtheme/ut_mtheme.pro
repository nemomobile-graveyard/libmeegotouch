include(../common_top.pri)

TARGET = ut_mtheme

INCLUDEPATH += $$MSRCDIR/corelib/theme

SOURCES += \
    ut_mtheme.cpp \

HEADERS += \
    ut_mtheme.h \

support_files.files += \
    ut_mtheme.svg \
    ut_mtheme_second.svg \

include(../common_bot.pri)
