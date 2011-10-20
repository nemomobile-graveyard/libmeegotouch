include(../common_top.pri)

TARGET = ut_msystemdirectories

TEST_HEADERS = $$MSRCDIR/corelib/core/msystemdirectories.h

INCLUDEPATH += $$MSRCDIR/corelib/core

HEADERS += ut_msystemdirectories.h \
            $$TEST_HEADERS

SOURCES += ut_msystemdirectories.cpp \
           $$MSRCDIR/corelib/core/msystemdirectories.cpp 

include(../common_bot.pri)
