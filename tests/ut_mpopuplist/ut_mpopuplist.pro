include(../common_top.pri)
TARGET = ut_mpopuplist

# unit

# unit test and unit
SOURCES += \
    ut_mpopuplist.cpp \
    ../../src/views/mdialogview.cpp \
    ../../src/views/mbuttongrouplayoutpolicy_p.cpp \

# unit test and unit
HEADERS += \
    ut_mpopuplist.h

INCLUDEPATH += \
    ../../src/corelib/widgets \
    ../../src/views/.moc \
    ../../src/views \

include(../common_bot.pri)
