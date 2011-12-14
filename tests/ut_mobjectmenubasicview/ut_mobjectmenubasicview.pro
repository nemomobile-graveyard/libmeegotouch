include(../common_top.pri)
TARGET = ut_mobjectmenubasicview

INCLUDEPATH += $$MSRCDIR/corelib/widgets \
               $$MSRCDIR/corelib/style \
               $$MSRCDIR/views \
               ../../src/views/.moc

STYLE_HEADERS += \
    $$MSRCDIR/views/style/mscenewindowstyle.h \
    $$MSRCDIR/views/style/mobjectmenubasicstyle.h \


# unit test and unit
SOURCES += \
    ut_mobjectmenubasicview.cpp \
    $$MSRCDIR/views/mobjectmenubasicview.cpp \

# unit test and unit
HEADERS += \
    ut_mobjectmenubasicview.h \
    $$MSRCDIR/views/mobjectmenubasicview.h \
    $$MSRCDIR/views/style/mscenewindowstyle.h \
    $$MSRCDIR/views/style/mobjectmenubasicstyle.h \

include(../common_bot.pri)
