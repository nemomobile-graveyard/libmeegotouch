include(../common_top.pri)
TARGET = ut_mgraphicshighlighteffect


INCLUDEPATH += $$MSRCDIR/corelib/widgets \
               $$MSRCDIR/corelib/style \
               $$MSRCDIR/scene \
               $$MSRCDIR/views \
               $$MSRCDIR/corelib/effects \
               $$MSRCDIR/views/effects \
               $$MSRCDIR/views/style \

SOURCES += ut_mgraphicshighlighteffect.cpp \
            $$MSRCDIR/views/effects/mgraphicshighlighteffect.cpp \
            ../../src/views/.gen/gen_mgraphicshighlighteffectstyledata.cpp \

HEADERS  += ut_mgraphicshighlighteffect.h \
            $$MSRCDIR/views/effects/mgraphicshighlighteffect.h \
            $$MSRCDIR/views/effects/mgraphicshighlighteffect_p.h \
            $$MSRCDIR/views/style/mgraphicshighlighteffectstyle.h \


include(../common_bot.pri)
