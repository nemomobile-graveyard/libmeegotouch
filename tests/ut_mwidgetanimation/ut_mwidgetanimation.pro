#-------------------------------------------------
#
# Project created by QtCreator 2011-05-12T09:25:43
#
#-------------------------------------------------

include(../common_top.pri)
TARGET = ut_mwidgetanimation


INCLUDEPATH += $$MSRCDIR/corelib/widgets \
               $$MSRCDIR/corelib/style \
               $$MSRCDIR/scene \
               $$MSRCDIR/views \
               ../../src/corelib/animation/ \
               ../../src/corelib/animation/core \

SOURCES += ut_mwidgetanimation.cpp \
            ../../src/corelib/animation/mwidgetanimation.cpp \

HEADERS  += ut_mwidgetanimation.h \
            ../../src/corelib/animation/mwidgetanimation.h \
            ../../src/corelib/animation/mwidgetanimation_p.h \

include(../common_bot.pri)
