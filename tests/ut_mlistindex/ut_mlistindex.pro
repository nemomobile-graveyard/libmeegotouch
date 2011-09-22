include(../common_top.pri)

INCLUDEPATH += $$MSRCDIR/views \
            $$MSRCDIR/views/widgets \
            $$MSRCDIR/corelib/.gen \
            $$MSRCDIR/views/.gen \
            ../../src/corelib/.gen \
            ../../src/views/.gen \

HEADERS += ut_mlistindex.h \
        $$MSRCDIR/views/widgets/mlistindex.h \
        $$MSRCDIR/views/widgets/mlistindex_p.h \
        $$MSRCDIR/views/widgets/mlistindextooltip.h \
        $$MSRCDIR/views/widgets/mlistindextooltip_p.h \
        $$MSRCDIR/views/mlistindexview.h \
        $$MSRCDIR/views/mlistindexview_p.h \
        $$MSRCDIR/views/style/mlistindexstyle.h \
        $$MSRCDIR/views/style/mlistindextooltipstyle.h \
        $$MSRCDIR/views/widgets/mlistindexmodel.h \

SOURCES += ut_mlistindex.cpp \
        $$MSRCDIR/views/widgets/mlistindex.cpp \
        $$MSRCDIR/views/widgets/mlistindextooltip.cpp \
        $$MSRCDIR/views/mlistindexview.cpp \
        ../../src/views/.gen/gen_mlistindexstyledata.cpp \
        ../../src/views/.gen/gen_mlistindextooltipstyledata.cpp \
        ../../src/views/.gen/gen_mlistindexmodeldata.cpp \

TARGET = ut_mlistindex

include(../common_bot.pri)
