include(../common_top.pri)
TARGET = ut_duipopuplist

# unit

# unit test and unit
SOURCES += \
    ut_duipopuplist.cpp \
    ../../src/views/duidialogview.cpp \
    ../../src/views/duibuttongrouplayoutpolicy_p.cpp \
    ../../src/views/duiscenewindowview.cpp \
    ../../src/corelib/widgets/core/duiwidgetview.cpp \
    ../../src/corelib/animation/widget/core/duiabstractwidgetanimation.cpp \
    ../../src/corelib/.moc/moc_duiabstractwidgetanimation.cpp \
    ../../src/corelib/widgets/duicontentitemmodel.cpp \
    ../../src/corelib/.moc/moc_duicontentitemmodel.cpp \
    ../../src/corelib/.gen/gen_duicontentitemmodeldata.cpp \

# unit test and unit
HEADERS += \
    ut_duipopuplist.h

INCLUDEPATH += \
    ../../src/corelib \
    ../../src/corelib/style \
    ../../src/corelib/widgets \
    ../../src/corelib/.moc \
    ../../src/views/.moc \
    ../../src/corelib/animation/widget/core \
    ../../src/corelib/animation/core \
    ../../src/corelib/core \
    ../../src/corelib/widgets/views \
    ../../src/views \

include(../common_bot.pri)
