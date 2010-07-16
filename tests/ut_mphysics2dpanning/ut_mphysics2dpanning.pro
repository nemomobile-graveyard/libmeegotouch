include(../common_top.pri)

INCLUDEPATH += ../../src/corelib/.moc

TARGET = ut_mphysics2dpanning

# unit test and unit
SOURCES += \
    ut_mphysics2dpanning.cpp \
    $$MSRCDIR/corelib/widgets/mphysics2dpanning.cpp \

# unit test and unit
HEADERS += \
    ut_mphysics2dpanning.h \


include(../common_bot.pri)
