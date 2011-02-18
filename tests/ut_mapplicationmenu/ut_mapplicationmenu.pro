include(../common_top.pri)
TARGET = ut_mapplicationmenu


INCLUDEPATH += $$MSRCDIR/corelib/widgets \
               $$MSRCDIR/corelib/style \
               $$MSRCDIR/scene \
               $$MSRCDIR/views \
               ../../src/views/.moc

SOURCES += \
    ut_mapplicationmenu.cpp \
    $$MSRCDIR/views/mapplicationmenuview.cpp

HEADERS += \
    ut_mapplicationmenu.h \
    $$MSRCDIR/views/mapplicationmenuview_p.h

include(../common_bot.pri)
