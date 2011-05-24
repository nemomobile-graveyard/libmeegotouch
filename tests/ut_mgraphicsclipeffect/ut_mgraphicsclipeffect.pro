#-------------------------------------------------
#
# Project created by QtCreator 2011-05-12T09:25:43
#
#-------------------------------------------------

include(../common_top.pri)
TARGET = ut_mgraphicsclipeffect


INCLUDEPATH += $$MSRCDIR/corelib/widgets \
               $$MSRCDIR/corelib/style \
               $$MSRCDIR/scene \
               $$MSRCDIR/views \
               ../../src/corelib/effects/ \

SOURCES += ut_mgraphicsclipeffect.cpp \
            ../../src/corelib/effects/mgraphicsclipeffect.cpp

HEADERS  += ut_mgraphicsclipeffect.h \
            ../../src/corelib/effects/mgraphicsclipeffect.h \
            ../../src/corelib/effects/mgraphicsclipeffect_p.h \

include(../common_bot.pri)
