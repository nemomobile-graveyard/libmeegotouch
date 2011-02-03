include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/corelib/widgets
INCLUDEPATH += $$MSRCDIR/corelib/style
INCLUDEPATH += $$MSRCDIR/corelib/theme
TARGET = ut_mtapstatemachine

STYLE_HEADERS += \
    $$MSRCDIR/corelib/style/mstyle.h \
    $$MSRCDIR/corelib/style/mtapstatemachinestyle.h \

# unit test and unit classes
SOURCES += \
    ut_mtapstatemachine.cpp \
    $$MSRCDIR/corelib/style/mstyle.cpp \
    $$MSRCDIR/corelib/widgets/mtapstatemachine.cpp \
    
# unit test and unit classes
HEADERS += \
    ut_mtapstatemachine.h \
    $$MSRCDIR/corelib/style/mstyle.h \
    $$MSRCDIR/corelib/style/mtapstatemachinestyle.h \

include(../common_bot.pri)
