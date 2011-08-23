include(../common_top.pri)

TARGET = ut_mpreloader

TEST_HEADERS = $$MSRCDIR/corelib/preloader/mpreloader.h \
        $$MSRCDIR/corelib/core/msystemdirectories.h

INCLUDEPATH += \
        $$MSRCDIR/corelib/core \
        $$MSRCDIR/corelib/preloader

TEST_SOURCES = \
    $$MSRCDIR/corelib/preloader/mpreloader.cpp
    
# Input
HEADERS += \
    ut_mpreloader.h \
    $$TEST_HEADERS

SOURCES += \
    ut_mpreloader.cpp \
    $$TEST_SOURCES \

support_files.files += themedaemon

include(../common_bot.pri)
