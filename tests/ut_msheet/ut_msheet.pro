include(../common_top.pri)
TARGET = ut_msheet

INCLUDEPATH += $$MSRCDIR/corelib/widgets

LIBS += -lcontextsubscriber

# unit test
SOURCES += \
    ut_msheet.cpp \
    ../stubs/stubbase.cpp \

# unit test
HEADERS += \
    ut_msheet.h \

include(../common_bot.pri)
