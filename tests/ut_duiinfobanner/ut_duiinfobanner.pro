include(../common_top.pri)
INCLUDEPATH += $$DUISRCDIR/style
TARGET = ut_duiinfobanner

# unit

# unit test and unit
SOURCES += \
    ut_duiinfobanner.cpp \

# service classes
SOURCES += \
#    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_duiinfobanner.h

# service classes
HEADERS += \
#     $$STUBSDIR/duistyle_stub.h \
#     $$STUBSDIR/duitheme_stub.h \

include(../common_bot.pri)
