#-------------------------------------------------
#
# Project created by QtCreator 2011-05-12T09:25:43
#
#-------------------------------------------------

include(../common_top.pri)
TARGET = ut_mlistitem


INCLUDEPATH += $$MSRCDIR/corelib/widgets \
               $$MSRCDIR/corelib/style \
               $$MSRCDIR/scene \
               $$MSRCDIR/views \
               ../../src/corelib/widgets \
               ../../src/corelib/widgets/core \

SOURCES += ut_mlistitem.cpp \
            ../../src/corelib/widgets/mlistitem.cpp \
            ../../src/corelib/widgets/core/mwidgetcontroller.cpp \

HEADERS  += ut_mlistitem.h \
            ../../src/corelib/widgets/mlistitem.h \

include(../common_bot.pri)
