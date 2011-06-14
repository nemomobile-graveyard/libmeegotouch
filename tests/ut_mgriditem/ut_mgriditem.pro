#-------------------------------------------------
#
# Project created by QtCreator 2011-05-12T09:25:43
#
#-------------------------------------------------

include(../common_top.pri)
TARGET = ut_mgriditem


INCLUDEPATH += $$MSRCDIR/corelib/widgets \
               $$MSRCDIR/corelib/style \
               $$MSRCDIR/scene \
               $$MSRCDIR/views \
               ../../src/corelib/widgets/ \

SOURCES += ut_mgriditem.cpp \
            ../../src/corelib/widgets/mgriditem.cpp \
            ../../src/corelib/widgets/core/mwidgetcontroller.cpp \

HEADERS  += ut_mgriditem.h \
            ../../src/corelib/widgets/mgriditem.h \

include(../common_bot.pri)
