include(../common_top.pri)

INCLUDEPATH += $$MSRCDIR/corelib/.moc

TARGET = ut_mphysics2dpanning

# unit test and unit
SOURCES += \
    ut_mphysics2dpanning.cpp \
    ../../src/corelib/widgets/mphysics2dpanning.cpp \

# unit test and unit
HEADERS += \
    ut_mphysics2dpanning.h \


include(../common_bot.pri)
