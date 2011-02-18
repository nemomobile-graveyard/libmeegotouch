include(../common_top.pri)
TARGET = ut_mapplicationmenuview

INCLUDEPATH += $$MSRCDIR/corelib/widgets \
               $$MSRCDIR/corelib/style \
               $$MSRCDIR/views \
               ../../src/views/.moc

# unit test and unit
SOURCES += \
    ut_mapplicationmenuview.cpp \
    $$MSRCDIR/views/mapplicationmenuview.cpp \

# unit test and unit
HEADERS += \
    ut_mapplicationmenuview.h \
    $$MSRCDIR/views/mapplicationmenuview_p.h \

include(../common_bot.pri)
