include(../common_top.pri)

INCLUDEPATH += $$DUISRCDIR/corelib/.moc

TARGET = ut_duiphysics2dpanning

# unit test and unit
SOURCES += \
    ut_duiphysics2dpanning.cpp \
    ../../src/corelib/widgets/duiphysics2dpanning.cpp \

# unit test and unit
HEADERS += \
    ut_duiphysics2dpanning.h \


include(../common_bot.pri)
