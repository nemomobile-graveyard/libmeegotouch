include(../common_top.pri)

INCLUDEPATH += $$DUISRCDIR/.moc

TARGET = ut_duiphysics2dpanning

# unit test and unit
SOURCES += \
    ut_duiphysics2dpanning.cpp \
    ../../src/widgets/duiphysics2dpanning.cpp \

# unit test and unit
HEADERS += \
    ut_duiphysics2dpanning.h \


include(../common_bot.pri)
