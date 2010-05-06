include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/corelib/style
TARGET = ut_minfobanner

# unit

# unit test and unit
SOURCES += \
    ut_minfobanner.cpp \

# service classes
SOURCES += \
#    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_minfobanner.h

# service classes
HEADERS += \
#     $$STUBSDIR/mstyle_stub.h \
#     $$STUBSDIR/mtheme_stub.h \

include(../common_bot.pri)
