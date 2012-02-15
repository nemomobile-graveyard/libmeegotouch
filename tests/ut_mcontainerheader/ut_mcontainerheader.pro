include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/views $$MSRCDIR/views/widgets
TARGET = ut_mcontainerheader

# unit

# unit test and unit
SOURCES += \
    ut_mcontainerheader.cpp \
    $$MSRCDIR/views/widgets/mcontainerheader.cpp \

# unit test and unit
HEADERS += \
    ut_mcontainerheader.h \
    $$MSRCDIR/views/widgets/mcontainerheader_p.h \


include(../common_bot.pri)
