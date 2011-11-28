include(../common_top.pri)

TARGET = ut_mcolorcomboboxview

INCLUDEPATH += $$MSRCDIR/corelib/widgets \
               $$MSRCDIR/corelib/style \
               $$MSRCDIR/views \
               $$MSRCDIR/views/widgets

SOURCES += \
    ut_mcolorcomboboxview.cpp \

HEADERS += \
    ut_mcolorcomboboxview.h \

include(../common_bot.pri)
