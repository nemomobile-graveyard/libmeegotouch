include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/corelib/style
TARGET = ut_mbanner

# unit

# unit test and unit
SOURCES += \
    ut_mbanner.cpp \

# service classes
SOURCES += \
#    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_mbanner.h

# service classes
HEADERS += \
#     $$STUBSDIR/mstyle_stub.h \
#     $$STUBSDIR/mtheme_stub.h \

support_files.files += \
    ut_mbanner_pixmap.png

include(../common_bot.pri)
