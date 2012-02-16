include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/views $$MSRCDIR/views/widgets
TARGET = ut_mbubbleitembackground

# unit

# unit test and unit
SOURCES += \
    ut_mbubbleitembackground.cpp \
    $$MSRCDIR/views/widgets/mbubbleitembackground.cpp \

# unit test and unit
HEADERS += \
    ut_mbubbleitembackground.h \
    $$MSRCDIR/views/widgets/mbubbleitembackground_p.h \


include(../common_bot.pri)
