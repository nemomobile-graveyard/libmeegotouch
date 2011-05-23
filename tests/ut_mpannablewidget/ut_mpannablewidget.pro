include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/corelib/widgets \
              $$MSRCDIR/corelib/core \

TARGET = ut_mpannablewidget

HEADERS += \
    ut_mpannablewidget.h \
    $$MSRCDIR/corelib/core/mdynamicpropertywatcher.h \


SOURCES += \
    ut_mpannablewidget.cpp \
    $$MSRCDIR/corelib/core/mdynamicpropertywatcher.cpp \


include(../common_bot.pri)
