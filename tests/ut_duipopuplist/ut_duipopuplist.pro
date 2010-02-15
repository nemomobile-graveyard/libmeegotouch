include(../common_top.pri)
TARGET = ut_duipopuplist

# unit

# unit test and unit
SOURCES += \
    ut_duipopuplist.cpp \
    ../../src/widgets/views/duidialogview.cpp \
    ../../src/widgets/views/duibuttongrouplayoutpolicy_p.cpp \
    ../../src/widgets/views/duiscenewindowview.cpp \
    ../../src/widgets/core/duiwidgetview.cpp \
    ../../src/animation/widget/core/duiabstractwidgetanimation.cpp \
    ../../src/.moc/moc_duiabstractwidgetanimation.cpp \

# unit test and unit
HEADERS += \
    ut_duipopuplist.h

INCLUDEPATH += \
    ../../src/style \
    ../../src/widgets \
    ../../src/.moc \
    ../../src/animation/widget/core \
    ../../src/animation/core \
    ../../src/core \
    ../../src/widgets/views \

include(../common_bot.pri)
