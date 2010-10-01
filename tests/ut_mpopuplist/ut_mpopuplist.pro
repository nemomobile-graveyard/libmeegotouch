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
    ut_mpopuplist.h \
    $$MSRCDIR/views/mdialoginternalbox_p.h

INCLUDEPATH += \
    $$MSRCDIR/corelib/widgets \
    ../../src/views/.moc \
    $$MSRCDIR/views \

include(../common_bot.pri)
