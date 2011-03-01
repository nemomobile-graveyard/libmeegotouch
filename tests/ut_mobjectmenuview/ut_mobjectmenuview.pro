include(../common_top.pri)
TARGET = ut_mobjectmenuview

INCLUDEPATH += $$MSRCDIR/corelib/widgets \
               $$MSRCDIR/corelib/style \
               $$MSRCDIR/views \
               ../../src/views/.moc

# unit test and unit
SOURCES += \
    ut_mobjectmenuview.cpp \
    $$MSRCDIR/views/mobjectmenuview.cpp \

# unit test and unit
HEADERS += \
    ut_mobjectmenuview.h \
    $$MSRCDIR/views/mobjectmenuview_p.h \

include(../common_bot.pri)
