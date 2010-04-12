include(../common_top.pri)
TARGET = ut_mpopuplist

# unit

# unit test and unit
SOURCES += \
    ut_mpopuplist.cpp \
    ../../src/views/mdialogview.cpp \
    ../../src/views/mbuttongrouplayoutpolicy_p.cpp \
    ../../src/views/mscenewindowview.cpp \
    ../../src/corelib/widgets/core/mwidgetview.cpp \
    ../../src/corelib/animation/widget/core/mabstractwidgetanimation.cpp \
    ../../src/corelib/.moc/moc_mabstractwidgetanimation.cpp \
    ../../src/corelib/widgets/mcontentitemmodel.cpp \
    ../../src/corelib/.moc/moc_mcontentitemmodel.cpp \
    ../../src/corelib/.gen/gen_mcontentitemmodeldata.cpp \

# unit test and unit
HEADERS += \
    ut_mpopuplist.h

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
