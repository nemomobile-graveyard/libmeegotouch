include(../common_top.pri)
TARGET = ut_mpopuplist

# unit

# unit test and unit
SOURCES += \
    ut_mpopuplist.cpp \
    $$MSRCDIR/views/mdialogview.cpp \
    $$MSRCDIR/views/mbuttongrouplayoutpolicy_p.cpp \

# unit test and unit
HEADERS += \
    ut_mpopuplist.h

INCLUDEPATH += \
    $$MSRCDIR/corelib/widgets \
    $$MSRCDIR/views/.moc \
    $$MSRCDIR/views \

include(../common_bot.pri)
