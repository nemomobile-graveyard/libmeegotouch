include(../common_top.pri)

INCLUDEPATH += $$MSRCDIR/corelib/core

TARGET = ut_mdynamicpropertywatcher

SOURCES += \
    ut_mdynamicpropertywatcher.cpp \
    $$MSRCDIR/corelib/core/mdynamicpropertywatcher.cpp \

HEADERS += \
    ut_mdynamicpropertywatcher.h \
    $$MSRCDIR/corelib/core/mdynamicpropertywatcher.h \

include(../common_bot.pri)
